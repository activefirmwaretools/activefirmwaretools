# DECODER_MYNAME = "PMBus"
#
# Decodes PMBus 1.x traffic on SCL/SDA digital channels with named commands and LINEAR11/16.
# PMBus runs on SMBus (which is itself I2C plus PEC), adding a standardized command code
# table plus encodings such as LINEAR11 / LINEAR16.  This decoder reports command names
# instead of raw codes and decodes the most common data formats.
#
# PARAM: scl_channel  | SCL Channel       | digital_channel |  |
# PARAM: sda_channel  | SDA Channel       | digital_channel |  |
# PARAM: expect_pec   | Expect PEC byte   | bool |  | true
# PARAM: vout_exponent| VOUT_MODE Exp (2's-comp 5b) | int | -16,15 | -9
#
# Output rows:
#   ch 0 : address + R/W
#   ch 1 : command name (e.g. "READ_VOUT")
#   ch 2 : decoded data (raw + interpretation, e.g. "0x3000 = 12.000V")
#   ch 3 : ACK / NAK
#   ch 4 : PEC verdict
#
# Notes:
#   - PMBus 1.x command set is large; we name the most common.
#   - LINEAR11 is a 16-bit format: [exponent:5 signed][mantissa:11 signed].
#     Used by READ_VIN, READ_IOUT, READ_TEMPERATURE_*, READ_FAN_SPEED_1, etc.
#   - LINEAR16 ("VOUT mode") is unsigned-mantissa scaled by an exponent
#     read from VOUT_MODE.  We don't track VOUT_MODE live -- the user
#     supplies the expected exponent via the PARAM (typical default: -9
#     for 12-bit DACs at 4.88 mV/LSB).

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


# A practical subset of PMBus 1.x commands.  Extend as needed.
PMBUS_CMDS = {
    0x00: ("PAGE", "byte"),
    0x01: ("OPERATION", "byte"),
    0x02: ("ON_OFF_CONFIG", "byte"),
    0x03: ("CLEAR_FAULTS", "send"),
    0x10: ("WRITE_PROTECT", "byte"),
    0x15: ("STORE_USER_ALL", "send"),
    0x19: ("CAPABILITY", "byte"),
    0x20: ("VOUT_MODE", "byte"),
    0x21: ("VOUT_COMMAND", "linear16"),
    0x22: ("VOUT_TRIM", "linear16"),
    0x24: ("VOUT_MAX", "linear16"),
    0x25: ("VOUT_MARGIN_HIGH", "linear16"),
    0x26: ("VOUT_MARGIN_LOW", "linear16"),
    0x40: ("VOUT_OV_FAULT_LIMIT", "linear16"),
    0x46: ("IOUT_OC_FAULT_LIMIT", "linear11"),
    0x4F: ("OT_FAULT_LIMIT", "linear11"),
    0x51: ("OT_WARN_LIMIT", "linear11"),
    0x55: ("VIN_OV_FAULT_LIMIT", "linear11"),
    0x58: ("VIN_UV_FAULT_LIMIT", "linear11"),
    0x78: ("STATUS_BYTE", "byte"),
    0x79: ("STATUS_WORD", "word"),
    0x7A: ("STATUS_VOUT", "byte"),
    0x7B: ("STATUS_IOUT", "byte"),
    0x7C: ("STATUS_INPUT", "byte"),
    0x7D: ("STATUS_TEMPERATURE", "byte"),
    0x7E: ("STATUS_CML", "byte"),
    0x88: ("READ_VIN", "linear11"),
    0x89: ("READ_IIN", "linear11"),
    0x8B: ("READ_VOUT", "linear16"),
    0x8C: ("READ_IOUT", "linear11"),
    0x8D: ("READ_TEMPERATURE_1", "linear11"),
    0x8E: ("READ_TEMPERATURE_2", "linear11"),
    0x8F: ("READ_TEMPERATURE_3", "linear11"),
    0x90: ("READ_FAN_SPEED_1", "linear11"),
    0x96: ("READ_POUT", "linear11"),
    0x97: ("READ_PIN", "linear11"),
    0x98: ("PMBUS_REVISION", "byte"),
    0x99: ("MFR_ID", "block"),
    0x9A: ("MFR_MODEL", "block"),
    0x9B: ("MFR_REVISION", "block"),
}


def _crc8(prev, byte_val):
    crc = prev ^ byte_val
    for _ in range(8):
        crc = ((crc << 1) ^ 0x07) & 0xFF if (crc & 0x80) else (crc << 1) & 0xFF
    return crc


def _decode_linear11(word):
    """Signed 11-bit mantissa, signed 5-bit exponent (both 2's-complement)."""
    mant = word & 0x07FF
    if mant & 0x0400:
        mant -= 0x0800
    exp = (word >> 11) & 0x1F
    if exp & 0x10:
        exp -= 0x20
    return mant * (2.0 ** exp)


def _decode_linear16(word, exponent):
    """Unsigned mantissa, exponent supplied externally (from VOUT_MODE)."""
    return word * (2.0 ** exponent)


def decode(params):
    scl = int(params.get("scl_channel", "0"))
    sda = int(params.get("sda_channel", "1"))
    expect_pec  = params.get("expect_pec", "true").lower() == "true"
    vout_exp    = int(params.get("vout_exponent", "-9"))

    print(f"PMBus: SCL={scl} SDA={sda} PEC={expect_pec} VOUT_exp={vout_exp}",
          file=sys.stderr)

    while True:
        start = yield from wait_for(all_of(falling_edge(sda), high(scl)))
        if start is None:
            return
        append(start.t, start.t, CH_ADDR, text="START",
               sample_type=SAMPLE_PACKET_START)

        # Read addr byte
        addr_byte, addr_start, addr_ack = \
            yield from _read_byte_with_ack(scl, sda)
        if addr_byte is None:
            return
        addr_val = addr_byte >> 1
        rw = "R" if (addr_byte & 1) else "W"
        append(addr_start, addr_ack.t, CH_ADDR,
               text=f"ADDR 0x{addr_val:02X} {rw}",
               data=addr_byte, sample_type=SAMPLE_DATA)
        append(addr_ack.t, addr_ack.t, CH_ACK,
               text=("NAK" if addr_ack.d[sda] else "ACK"),
               sample_type=SAMPLE_DATA_ALT)

        cmd_code = None
        cmd_kind = None
        data_bytes = []
        last_t = addr_ack.t
        crc = _crc8(0, addr_byte)
        ended = False

        # Try to read the next byte, but check for STOP between bytes.
        while not ended:
            ev = yield from wait_for(any_of(
                all_of(rising_edge(sda), high(scl)),    # STOP
                all_of(falling_edge(sda), high(scl)),   # RESTART
                rising_edge(scl),                       # bit 7 of next byte
            ))
            if ev is None:
                return
            if ev.d[scl] == 1 and ev.d[sda] == 1:
                # STOP -- finalize.
                if expect_pec and data_bytes:
                    pec_got = data_bytes[-1]
                    pec_payload = data_bytes[:-1]
                else:
                    pec_got = None
                    pec_payload = data_bytes
                # Decode data interpretation
                if cmd_code is not None:
                    text = _format_payload(cmd_kind, pec_payload, vout_exp)
                    if text:
                        append(last_t, ev.t, CH_DATA, text=text,
                               sample_type=SAMPLE_DATA, color=0xFFAAEEFF)
                if expect_pec and data_bytes:
                    crc_payload = crc
                    for b in pec_payload:
                        crc_payload = _crc8(crc_payload, b)
                    if pec_got == crc_payload:
                        append(ev.t, ev.t, CH_PEC,
                               text=f"PEC OK (0x{pec_got:02X})",
                               color=0xFF88FF88, sample_type=SAMPLE_DATA)
                    else:
                        append(ev.t, ev.t, CH_PEC,
                               text=f"PEC FAIL: got 0x{pec_got:02X} want 0x{crc_payload:02X}",
                               color=0xFFFF8888, sample_type=SAMPLE_DATA)
                append(ev.t, ev.t, CH_ADDR, text="STOP",
                       sample_type=SAMPLE_PACKET_END)
                ended = True
                break
            elif ev.d[scl] == 1 and ev.d[sda] == 0:
                # RESTART; treat as new transfer
                append(ev.t, ev.t, CH_ADDR, text="RESTART",
                       sample_type=SAMPLE_DATA)
                ended = True
                break
            else:
                # SCL rising: ev is bit 7 of next byte
                byte_val = ev.d[sda]
                bs = ev.t
                for _ in range(7):
                    e = yield from wait_for(rising_edge(scl))
                    if e is None:
                        return
                    byte_val = (byte_val << 1) | e.d[sda]
                ack = yield from wait_for(rising_edge(scl))
                if ack is None:
                    return

                if cmd_code is None:
                    cmd_code = byte_val
                    crc = _crc8(crc, byte_val)
                    name, kind = PMBUS_CMDS.get(byte_val,
                                                (f"0x{byte_val:02X}", "byte"))
                    cmd_kind = kind
                    append(bs, ack.t, CH_CMD,
                           text=name, data=byte_val,
                           color=0xFFFFCC88, sample_type=SAMPLE_DATA)
                else:
                    data_bytes.append(byte_val)
                    crc = _crc8(crc, byte_val)
                    append(bs, ack.t, CH_DATA,
                           text=f"0x{byte_val:02X}", data=byte_val,
                           sample_type=SAMPLE_DATA)

                append(ack.t, ack.t, CH_ACK,
                       text=("NAK" if ack.d[sda] else "ACK"),
                       sample_type=SAMPLE_DATA_ALT)
                last_t = ack.t


def _format_payload(kind, payload, vout_exp):
    if not payload:
        return ""
    raw_hex = " ".join(f"0x{b:02X}" for b in payload)
    if kind == "byte" and len(payload) >= 1:
        return raw_hex
    if kind == "word" and len(payload) >= 2:
        w = payload[0] | (payload[1] << 8)
        return f"{raw_hex}  = 0x{w:04X}"
    if kind == "linear11" and len(payload) >= 2:
        w = payload[0] | (payload[1] << 8)
        return f"{raw_hex}  = {_decode_linear11(w):.4g}"
    if kind == "linear16" and len(payload) >= 2:
        w = payload[0] | (payload[1] << 8)
        return f"{raw_hex}  = {_decode_linear16(w, vout_exp):.4g}V"
    if kind == "block":
        # First byte is block length
        n = payload[0]
        s = bytes(payload[1:1 + n]).decode("ascii", errors="replace")
        return f'len={n} "{s}"'
    return raw_hex


def _read_byte_with_ack(scl, sda):
    byte_val = 0
    byte_start_t = None
    for _ in range(8):
        e = yield from wait_for(rising_edge(scl))
        if e is None:
            return None, None, None
        if byte_start_t is None:
            byte_start_t = e.t
        byte_val = (byte_val << 1) | e.d[sda]
    ack = yield from wait_for(rising_edge(scl))
    if ack is None:
        return None, None, None
    return byte_val, byte_start_t, ack
