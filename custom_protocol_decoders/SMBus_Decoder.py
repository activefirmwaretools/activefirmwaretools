# DECODER_MYNAME = "SMBus"
#
# Decodes SMBus traffic on SCL/SDA (a strict-protocol I2C superset).
# Compared with the plain I2C decoder, this one:
#   - reports a transfer's command code (the first data byte after the
#     7-bit address) by name when known,
#   - validates the optional trailing PEC (Packet Error Code, CRC-8) byte.
#
# PARAM: scl_channel  | SCL Channel    | digital_channel |  |
# PARAM: sda_channel  | SDA Channel    | digital_channel |  |
# PARAM: expect_pec   | Expect PEC     | bool |  | false
#
# Output rows:
#   ch 0 : address + R/W (e.g. "ADDR 0x2C W")
#   ch 1 : command code  (e.g. "CMD 0x06")
#   ch 2 : data bytes
#   ch 3 : ACK / NAK
#   ch 4 : PEC verdict ("PEC OK" / "PEC FAIL: got=0xXX want=0xXX")
#
# Each START..STOP is wrapped in SAMPLE_PACKET_START / SAMPLE_PACKET_END.

import sys

from active_pro import (
    append, wait_for,
    rising_edge, falling_edge, high,
    all_of, any_of,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_ADDR = 0
CH_CMD  = 1
CH_DATA = 2
CH_ACK  = 3
CH_PEC  = 4


def _crc8(prev, byte_val):
    """SMBus CRC-8 (poly 0x07, init 0x00)."""
    crc = prev ^ byte_val
    for _ in range(8):
        if crc & 0x80:
            crc = ((crc << 1) ^ 0x07) & 0xFF
        else:
            crc = (crc << 1) & 0xFF
    return crc


def decode(params):
    scl = int(params.get("scl_channel", "0"))
    sda = int(params.get("sda_channel", "1"))
    expect_pec = params.get("expect_pec", "false").lower() == "true"

    print(f"SMBus: SCL={scl} SDA={sda} PEC={expect_pec}", file=sys.stderr)

    while True:
        # ----- START condition (SDA falls while SCL high) -------------------
        start = yield from wait_for(all_of(falling_edge(sda), high(scl)))
        if start is None:
            return
        append(start.t, start.t, CH_ADDR, text="START",
               sample_type=SAMPLE_PACKET_START)

        bytes_seen = []        # all bytes after addr -- last is PEC if used
        first_byte = True
        ended = False
        pending_bit   = None   # bit 7 already clocked by the between-byte probe
        pending_bit_t = None

        while not ended:
            byte_val, byte_start_t, ack_edge = \
                yield from _read_byte_with_ack(scl, sda, pending_bit,
                                               pending_bit_t)
            pending_bit = None
            pending_bit_t = None
            if byte_val is None:
                return
            ack_bit = ack_edge.d[sda]

            if first_byte:
                addr = byte_val >> 1
                rw   = "R" if (byte_val & 1) else "W"
                append(byte_start_t, ack_edge.t, CH_ADDR,
                       text=f"ADDR 0x{addr:02X} {rw}",
                       data=byte_val, sample_type=SAMPLE_DATA)
                first_byte = False
                address_byte = byte_val
            elif len(bytes_seen) == 0:
                append(byte_start_t, ack_edge.t, CH_CMD,
                       text=f"CMD 0x{byte_val:02X}",
                       data=byte_val, sample_type=SAMPLE_DATA,
                       color=0xFFFFCC88)
                bytes_seen.append(byte_val)
            else:
                append(byte_start_t, ack_edge.t, CH_DATA,
                       text=f"0x{byte_val:02X}",
                       data=byte_val, sample_type=SAMPLE_DATA)
                bytes_seen.append(byte_val)

            append(ack_edge.t, ack_edge.t, CH_ACK,
                   text=("NAK" if ack_bit else "ACK"),
                   sample_type=SAMPLE_DATA_ALT, data=ack_bit)

            # ----- Between-byte classification ------------------------------
            # See I2C_Decoder for the full rationale. STOP / repeated-START are
            # SDA edges *while SCL is high*; a normal next bit is just an SCL
            # rising edge with SDA stable. Consume the ACK's SCL fall and the
            # next SCL rise first, then watch the plateau for an SDA edge
            # (STOP/RESTART) vs the SCL fall (clean data bit) -- so the clock
            # edge can't be mistaken for an SDA event.
            scl_fell = yield from wait_for(falling_edge(scl))
            if scl_fell is None:
                return
            next_rise = yield from wait_for(rising_edge(scl))
            if next_rise is None:
                return
            cand_bit   = next_rise.d[sda]
            cand_bit_t = next_rise.t
            ev = yield from wait_for(any_of(
                all_of(rising_edge(sda),  high(scl)),   # STOP
                all_of(falling_edge(sda), high(scl)),   # repeated START
                falling_edge(scl),                      # clean data bit
            ))
            if ev is None:
                return
            if ev.d[scl] == 0:
                # SCL fell with SDA steady -> data bit (bit 7 already sampled).
                pending_bit   = cand_bit
                pending_bit_t = cand_bit_t
                continue
            if ev.d[sda] == 1:
                # STOP -- transfer ends.
                if expect_pec and len(bytes_seen) >= 1:
                    crc = _crc8(0, address_byte)
                    for b in bytes_seen[:-1]:
                        crc = _crc8(crc, b)
                    pec_got  = bytes_seen[-1]
                    pec_want = crc
                    if pec_got == pec_want:
                        text = f"PEC OK (0x{pec_got:02X})"
                        color = 0xFF88FF88
                    else:
                        text = f"PEC FAIL: got 0x{pec_got:02X} want 0x{pec_want:02X}"
                        color = 0xFFFF8888
                    append(ev.t, ev.t, CH_PEC, text=text, color=color,
                           sample_type=SAMPLE_DATA)
                append(ev.t, ev.t, CH_ADDR, text="STOP",
                       sample_type=SAMPLE_PACKET_END)
                ended = True
            else:
                # SDA fell and SCL held high -> repeated START, new addr byte.
                append(ev.t, ev.t, CH_ADDR, text="RESTART",
                       sample_type=SAMPLE_DATA)
                first_byte = True
                bytes_seen = []


def _read_byte_with_ack(scl, sda, first_bit=None, first_bit_t=None):
    """Read a byte MSB first on SCL rising edges, then the ACK bit. If
    first_bit is given it is bit 7 (already sampled at an SCL rising edge by the
    between-byte probe) and only 7 more bits are clocked. Returns
    (byte_val, byte_start_t, ack_edge_moment) or (None, None, None)."""
    if first_bit is None:
        byte_val = 0
        byte_start_t = None
        nbits = 8
    else:
        byte_val = first_bit & 1
        byte_start_t = first_bit_t
        nbits = 7
    for _ in range(nbits):
        e = yield from wait_for(rising_edge(scl))
        if e is None:
            return None, None, None
        if byte_start_t is None:
            byte_start_t = e.t
        byte_val = (byte_val << 1) | e.d[sda]
    ack_edge = yield from wait_for(rising_edge(scl))
    if ack_edge is None:
        return None, None, None
    return byte_val, byte_start_t, ack_edge
