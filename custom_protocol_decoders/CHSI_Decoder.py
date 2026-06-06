# DECODER_MYNAME = "CHSI"
#
# Decodes CHSI (two-wire serial) traffic on SCL and SDA digital channels.
# CHSI is identical to I2C EXCEPT there is no ACK/NAK clock: each byte is just
# 8 data bits, framed by START / STOP. There is no 9th acknowledge bit.
#
# PARAM: scl_channel | SCL Channel | digital_channel |  |
# PARAM: sda_channel | SDA Channel | digital_channel |  |
#
# Output rows (within this decoder's Device slot):
#   ch 0 : START / STOP / RESTART markers
#   ch 1 : data bytes (e.g. "0xA5") -- every byte, including the first, is data
#
# Protocol notes:
#   - START condition: SDA falls while SCL is high.
#   - STOP  condition: SDA rises while SCL is high.
#   - Data bits are sampled on the rising edge of SCL, LSB first.
#   - Bytes are exactly 8 bits; there is NO ACK/NAK bit after a byte.
#   - Each CHSI transaction (START..STOP) is wrapped in SAMPLE_PACKET_START /
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


def decode(params):
    scl = int(params.get("scl_channel", "0"))
    sda = int(params.get("sda_channel", "1"))

    print(f"CHSI decoder: SCL=ch{scl}, SDA=ch{sda}", file=sys.stderr)

    while True:
        # ----- Wait for a START condition -----------------------------------
        # START is SDA falling while SCL is high.
        start = yield from wait_for(all_of(falling_edge(sda), high(scl)))
        if start is None:
            return

        append(start.t, start.t, CH_ADDR,
               text="START", sample_type=SAMPLE_PACKET_START)

        # A CHSI transaction is a run of plain data bytes between START and
        # STOP. Unlike I2C, the first byte is NOT a special address byte -- it
        # is decoded exactly like every other byte.
        ended = False

        # When the between-byte classifier finds the next SCL pulse was a data
        # bit (not a STOP/RESTART), it stashes that already-sampled FIRST bit
        # (bit 0 -- CHSI is LSB-first) here so the byte loop resumes mid-byte
        # instead of re-clocking it.
        pending_bit   = None     # value of bit 0, already sampled
        pending_bit_t = None     # its timestamp (becomes byte_start_t)

        while not ended:
            # ----- Sample 8 data bits, LSB first (NO ACK bit) ---------------
            if pending_bit is None:
                byte_val     = 0
                byte_start_t = None
                start_idx    = 0
            else:
                # Bit 0 (LSB) was consumed by the previous between-byte probe.
                byte_val      = (pending_bit & 1)      # already in bit-0 slot
                byte_start_t  = pending_bit_t
                start_idx     = 1
                pending_bit   = None
                pending_bit_t = None

            byte_end_t = byte_start_t
            for bit_idx in range(start_idx, 8):
                edge = yield from wait_for(rising_edge(scl))
                if edge is None:
                    return
                if byte_start_t is None:
                    byte_start_t = edge.t
                byte_val |= (edge.d[sda] << bit_idx)   # LSB first
                byte_end_t = edge.t      # last bit clocked = end of the byte

            # ----- Emit annotation for this byte (every byte is data) -------
            append(byte_start_t, byte_end_t, CH_DATA,
                   text=f"0x{byte_val:02X}",
                   sample_type=SAMPLE_DATA, data=byte_val)

            # ----- After the byte, classify what comes next -----------------
            # STOP / repeated-START are SDA edges *while SCL is high*; a normal
            # next data bit is just an SCL rising edge with SDA stable at the
            # bit value (SDA is stable for the whole SCL-high pulse and only
            # changes while SCL is low). Consume the byte's last SCL fall and
            # the next SCL rise first, then watch the SCL-high plateau for an
            # SDA edge (STOP/RESTART) vs the SCL fall (clean data bit) -- so the
            # clock edge can't be mistaken for an SDA event.
            scl_fell = yield from wait_for(falling_edge(scl))
            if scl_fell is None:
                return
            next_rise = yield from wait_for(rising_edge(scl))
            if next_rise is None:
                return
            cand_bit   = next_rise.d[sda]    # provisional bit 0 (LSB) of next byte
            cand_bit_t = next_rise.t

            event = yield from wait_for(any_of(
                all_of(rising_edge(sda),  high(scl)),   # STOP
                all_of(falling_edge(sda), high(scl)),   # repeated START
                falling_edge(scl),                      # clean data bit
            ))
            if event is None:
                return

            if event.d[scl] == 0:
                # SCL fell with SDA steady -> next_rise clocked bit 0 (LSB) of
                # the next data byte. Resume the byte loop mid-byte.
                pending_bit   = cand_bit
                pending_bit_t = cand_bit_t
            elif event.d[sda] == 1:
                # SDA rose while SCL high -> STOP.
                append(event.t, event.t, CH_ADDR, text="STOP",
                       sample_type=SAMPLE_PACKET_END)
                ended = True
            else:
                # SDA fell while SCL high -> repeated START.
                append(event.t, event.t, CH_ADDR, text="RESTART",
                       sample_type=SAMPLE_DATA)
                # pending_bit stays None: the next byte clocks fresh.
