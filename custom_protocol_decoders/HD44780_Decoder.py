# DECODER_MYNAME = "HD44780 LCD"
#
# Decodes HD44780 character LCD traffic in 4-bit mode (RS, E, D4-D7) -- the common Arduino wiring.
# Each transferred byte is two nibbles latched on the falling edge of E.
#
# PARAM: rs_channel | RS Channel | digital_channel |  |
# PARAM: e_channel  | E Channel  | digital_channel |  |
# PARAM: d4_channel | D4 Channel | digital_channel |  |
# PARAM: d5_channel | D5 Channel | digital_channel |  |
# PARAM: d6_channel | D6 Channel | digital_channel |  |
# PARAM: d7_channel | D7 Channel | digital_channel |  |
#
# Output rows:
#   ch 0 : commands (decoded by name where known, e.g. "Clear", "Function Set")
#   ch 1 : data writes (printable char + hex)
#
# Protocol notes:
#   - Each byte is sent as two 4-bit nibbles: HIGH nibble first, then LOW.
#   - Data is valid on the falling edge of E (enable).
#   - RS=0 selects the command register, RS=1 the data register.
#   - We do NOT decode reads (R/W=1) -- typical embedded code writes only.
#     Reads on the data lines would be misinterpreted as writes; capture
#     R/W externally if you need to disambiguate.

import sys

from active_pro import (
    append, wait_for,
    falling_edge,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_CMD  = 0
CH_DATA = 1


def _decode_cmd(cmd):
    """Return a human-readable name for a command byte, plus its raw hex."""
    if cmd == 0x01:
        name = "Clear Display"
    elif cmd == 0x02 or cmd == 0x03:
        name = "Return Home"
    elif (cmd & 0xFC) == 0x04:
        # Entry mode set: 0000 01 I/D S
        id_ = (cmd & 0x02) != 0
        s   = (cmd & 0x01) != 0
        name = f"Entry Mode: cursor={'right' if id_ else 'left'} shift={'on' if s else 'off'}"
    elif (cmd & 0xF8) == 0x08:
        # Display on/off: 0000 1 D C B
        d = (cmd & 0x04) != 0
        c = (cmd & 0x02) != 0
        b = (cmd & 0x01) != 0
        name = f"Display: {'on' if d else 'off'} cursor={'on' if c else 'off'} blink={'on' if b else 'off'}"
    elif (cmd & 0xF0) == 0x10:
        # Cursor/display shift
        sc = (cmd & 0x08) != 0
        rl = (cmd & 0x04) != 0
        name = f"Shift {'display' if sc else 'cursor'} {'right' if rl else 'left'}"
    elif (cmd & 0xE0) == 0x20:
        # Function set: 001 DL N F xx
        dl = (cmd & 0x10) != 0
        n  = (cmd & 0x08) != 0
        f  = (cmd & 0x04) != 0
        name = f"Function Set: {8 if dl else 4}-bit {'2-line' if n else '1-line'} {'5x10' if f else '5x8'}"
    elif (cmd & 0xC0) == 0x40:
        name = f"Set CGRAM addr 0x{cmd & 0x3F:02X}"
    elif cmd & 0x80:
        # Set DDRAM addr.  Row 1 typically 0x00..0x27, row 2 typically 0x40..0x67.
        addr = cmd & 0x7F
        if addr < 0x40:
            row = 1
            col = addr
        else:
            row = 2
            col = addr - 0x40
        name = f"Set DDRAM addr 0x{addr:02X} (row {row} col {col})"
    else:
        name = f"Cmd 0x{cmd:02X}"
    return name


def decode(params):
    rs = int(params.get("rs_channel", "0"))
    e  = int(params.get("e_channel",  "1"))
    d4 = int(params.get("d4_channel", "2"))
    d5 = int(params.get("d5_channel", "3"))
    d6 = int(params.get("d6_channel", "4"))
    d7 = int(params.get("d7_channel", "5"))

    print(f"HD44780: RS={rs} E={e} D4-D7={d4},{d5},{d6},{d7}",
          file=sys.stderr)

    pending_nibble = None
    pending_rs = None
    pending_start_t = None

    while True:
        edge = yield from wait_for(falling_edge(e))
        if edge is None:
            return

        nib = (edge.d[d4] << 0) | (edge.d[d5] << 1) | \
              (edge.d[d6] << 2) | (edge.d[d7] << 3)
        rs_val = edge.d[rs]

        if pending_nibble is None:
            pending_nibble = nib
            pending_rs = rs_val
            pending_start_t = edge.t
            continue

        # Combine: first latched nibble is the HIGH nibble.
        byte_val = (pending_nibble << 4) | nib

        if pending_rs == 0:
            # Command
            name = _decode_cmd(byte_val)
            append(pending_start_t, edge.t, CH_CMD,
                   text=f"{name} (0x{byte_val:02X})",
                   data=byte_val, sample_type=SAMPLE_DATA,
                   color=0xFFFFCC88)
        else:
            # Data
            ch_char = chr(byte_val) if 32 <= byte_val < 127 else "."
            append(pending_start_t, edge.t, CH_DATA,
                   text=f"'{ch_char}' (0x{byte_val:02X})",
                   data=byte_val, sample_type=SAMPLE_DATA,
                   color=0xFFAAEEFF)

        pending_nibble = None
        pending_rs = None
        pending_start_t = None
