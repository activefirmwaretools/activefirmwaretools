# DECODER_MYNAME = "I2C"
#
# Decodes I2C (two-wire serial) traffic on SCL and SDA digital channels.
#
# PARAM: scl_channel | SCL Channel | digital_channel |  |
# PARAM: sda_channel | SDA Channel | digital_channel |  |
#
# Output rows (within this decoder's Device slot):
#   ch 0 : address byte + R/W direction (e.g. "ADDR 0x50 W")
#   ch 1 : data bytes (e.g. "0xA5")
#   ch 2 : ACK / NAK markers after each byte
#
# Protocol notes:
#   - START condition: SDA falls while SCL is high.
#   - STOP  condition: SDA rises while SCL is high.
#   - Data bits are sampled on the rising edge of SCL, MSB first.
#   - After every 8 bits the master releases SDA and the slave drives
#     either 0 (ACK) or 1 (NAK) for one more SCL clock; we sample that
#     bit on the same rising-edge rule.
#   - Each I2C transaction (START..STOP) is wrapped in SAMPLE_PACKET_START /
#     SAMPLE_PACKET_END markers so an attached PacketPresenter can frame it.

import sys

from active_pro import (
    append,
    wait_for,
    rising_edge, falling_edge, high,
    all_of, any_of,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_ADDR = 0
CH_DATA = 1
CH_ACK  = 2


def decode(params):
    scl = int(params.get("scl_channel", "0"))
    sda = int(params.get("sda_channel", "1"))

    print(f"I2C decoder: SCL=ch{scl}, SDA=ch{sda}", file=sys.stderr)

    while True:
        # ----- Wait for a START condition -----------------------------------
        # START is SDA falling while SCL is high. We detect it as a falling
        # edge on SDA combined with the level constraint that SCL == 1 at
        # that same moment.
        start = yield from wait_for(all_of(falling_edge(sda), high(scl)))
        if start is None:
            return

        append(start.t, start.t, CH_ADDR,
               text="START", sample_type=SAMPLE_PACKET_START)

        # An I2C transaction may contain multiple bytes between START and
        # STOP, with the very first byte carrying the 7-bit address + R/W
        # bit. After that, byte direction depends on R/W and is not
        # tracked here -- every subsequent byte is annotated on the data row.
        first_byte = True
        ended = False

        while not ended:
            # ----- Sample 8 data bits, MSB first ----------------------------
            byte_val = 0
            byte_start_t = None

            for bit in range(8):
                edge = yield from wait_for(rising_edge(scl))
                if edge is None:
                    return
                if byte_start_t is None:
                    byte_start_t = edge.t
                byte_val = (byte_val << 1) | edge.d[sda]

            # ----- Sample the ACK / NAK bit ---------------------------------
            ack_edge = yield from wait_for(rising_edge(scl))
            if ack_edge is None:
                return
            ack_bit = ack_edge.d[sda]

            # ----- Emit annotations for this byte ---------------------------
            if first_byte:
                addr  = byte_val >> 1
                rw    = "R" if (byte_val & 1) else "W"
                text  = f"ADDR 0x{addr:02X} {rw}"
                append(byte_start_t, ack_edge.t, CH_ADDR,
                       text=text, sample_type=SAMPLE_DATA, data=byte_val)
                first_byte = False
            else:
                append(byte_start_t, ack_edge.t, CH_DATA,
                       text=f"0x{byte_val:02X}",
                       sample_type=SAMPLE_DATA, data=byte_val)

            append(ack_edge.t, ack_edge.t, CH_ACK,
                   text=("NAK" if ack_bit else "ACK"),
                   sample_type=SAMPLE_DATA_ALT, data=ack_bit)

            # ----- After the ACK bit, watch for STOP / repeated START -------
            # The bus is now between bytes. Either a STOP (SDA rising while
            # SCL high) ends the transaction, or another data byte begins
            # with the next SCL rising edge. A repeated START (SDA falling
            # while SCL high) restarts the transaction without an
            # intervening STOP -- we handle that the same as a fresh START.
            stop_or_repstart_or_data = yield from wait_for(any_of(
                all_of(rising_edge(sda),  high(scl)),   # STOP
                all_of(falling_edge(sda), high(scl)),   # Repeated START
                rising_edge(scl),                       # Next byte begins
            ))
            if stop_or_repstart_or_data is None:
                return

            # Decide which event happened based on signal state at the moment.
            if stop_or_repstart_or_data.d[scl] == 1 and \
               stop_or_repstart_or_data.d[sda] == 1:
                # STOP -- end of this transaction.
                append(stop_or_repstart_or_data.t, stop_or_repstart_or_data.t,
                       CH_ADDR, text="STOP",
                       sample_type=SAMPLE_PACKET_END)
                ended = True
            elif stop_or_repstart_or_data.d[scl] == 1 and \
                 stop_or_repstart_or_data.d[sda] == 0:
                # Repeated START -- new transaction, addr byte coming next.
                append(stop_or_repstart_or_data.t, stop_or_repstart_or_data.t,
                       CH_ADDR, text="RESTART", sample_type=SAMPLE_DATA)
                first_byte = True
                # Loop back into the byte-sample state with first_byte=True.
            else:
                # SCL rising edge with no STOP / RESTART -- this rising edge
                # is actually bit 0 of the next byte. Recreate the byte
                # accumulator with this first bit captured.
                byte_val = stop_or_repstart_or_data.d[sda]
                byte_start_t = stop_or_repstart_or_data.t

                for bit in range(7):
                    edge = yield from wait_for(rising_edge(scl))
                    if edge is None:
                        return
                    byte_val = (byte_val << 1) | edge.d[sda]

                ack_edge = yield from wait_for(rising_edge(scl))
                if ack_edge is None:
                    return
                ack_bit = ack_edge.d[sda]

                append(byte_start_t, ack_edge.t, CH_DATA,
                       text=f"0x{byte_val:02X}",
                       sample_type=SAMPLE_DATA, data=byte_val)
                append(ack_edge.t, ack_edge.t, CH_ACK,
                       text=("NAK" if ack_bit else "ACK"),
                       sample_type=SAMPLE_DATA_ALT, data=ack_bit)
                # Fall through the inner loop to look again for STOP / next.
