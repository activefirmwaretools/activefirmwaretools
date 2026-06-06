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
        # START is SDA falling while SCL is high.
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

        # When the between-byte classifier finds the next SCL pulse was a data
        # bit (not a STOP/RESTART), it stashes that already-sampled MSB here so
        # the byte loop resumes mid-byte instead of re-clocking it.
        pending_bit   = None     # value of bit 7, already sampled
        pending_bit_t = None     # its timestamp (becomes byte_start_t)

        while not ended:
            # ----- Sample 8 data bits, MSB first ----------------------------
            if pending_bit is None:
                byte_val     = 0
                byte_start_t = None
                bits_to_read = 8
            else:
                # Bit 7 was consumed by the previous between-byte probe.
                byte_val      = pending_bit
                byte_start_t  = pending_bit_t
                bits_to_read  = 7
                pending_bit   = None
                pending_bit_t = None

            for bit in range(bits_to_read):
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

            # ----- After the ACK bit, classify what comes next --------------
            # STOP / repeated-START are SDA edges *while SCL is high*; a normal
            # next data bit is just an SCL rising edge with SDA stable at the
            # bit value (I2C keeps SDA stable for the whole SCL-high pulse and
            # only changes it while SCL is low). The original decoder lumped the
            # next-bit SCL rising edge into the same any_of() as the SDA edges
            # and disambiguated by level -- so every next byte whose bit 7 was 0
            # printed "RESTART" and whose bit 7 was 1 printed "STOP". The fix:
            # consume the ACK's SCL fall and the next SCL rise first, then watch
            # the SCL-high plateau for an SDA edge (STOP/RESTART) vs the SCL fall
            # (clean data bit). Now the clock edge can't be mistaken for an SDA
            # event.
            scl_fell = yield from wait_for(falling_edge(scl))
            if scl_fell is None:
                return
            next_rise = yield from wait_for(rising_edge(scl))
            if next_rise is None:
                return
            cand_bit   = next_rise.d[sda]    # provisional bit 7 of next byte
            cand_bit_t = next_rise.t

            event = yield from wait_for(any_of(
                all_of(rising_edge(sda),  high(scl)),   # STOP
                all_of(falling_edge(sda), high(scl)),   # repeated START
                falling_edge(scl),                      # clean data bit
            ))
            if event is None:
                return

            if event.d[scl] == 0:
                # SCL fell with SDA steady -> next_rise clocked bit 7 of the
                # next data byte. Resume the byte loop mid-byte.
                pending_bit   = cand_bit
                pending_bit_t = cand_bit_t
            elif event.d[sda] == 1:
                # SDA rose while SCL high -> STOP.
                append(event.t, event.t, CH_ADDR, text="STOP",
                       sample_type=SAMPLE_PACKET_END)
                ended = True
            else:
                # SDA fell while SCL high -> repeated START, new addr next.
                append(event.t, event.t, CH_ADDR, text="RESTART",
                       sample_type=SAMPLE_DATA)
                first_byte = True
                # pending_bit stays None: the address byte clocks fresh.
