# DECODER_MYNAME = "PS/2"
#
# Decodes PS/2 keyboard / mouse traffic on CLK + DATA digital channels.
#
# PARAM: clk_channel  | CLK Channel    | digital_channel |  |
# PARAM: data_channel | DATA Channel   | digital_channel |  |
# PARAM: device       | Device Type    | select | keyboard,mouse | keyboard
#
# Output rows:
#   ch 0 : raw byte (hex)
#   ch 1 : meaning (key name for keyboard, mouse-byte role for mouse)
#   ch 2 : parity verdict ("PAR OK" / "PAR FAIL")
#
# Protocol notes:
#   - PS/2 frame is 11 bits per byte:
#       start bit (always 0), 8 data bits (LSB first), odd parity, stop bit (1).
#   - DATA is sampled on the FALLING edge of CLK (device-to-host direction).
#   - Each transmitted byte is wrapped in SAMPLE_PACKET_START/END so that
#     a PacketPresenter can group keystroke / mouse packets.

import sys

from active_pro import (
    append, wait_for,
    falling_edge,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_BYTE = 0
CH_MEAN = 1
CH_PAR  = 2

# Subset of PS/2 set-2 scan codes (make codes only).  Add more as needed.
SCAN_SET2 = {
    0x1C: "A", 0x32: "B", 0x21: "C", 0x23: "D", 0x24: "E", 0x2B: "F",
    0x34: "G", 0x33: "H", 0x43: "I", 0x3B: "J", 0x42: "K", 0x4B: "L",
    0x3A: "M", 0x31: "N", 0x44: "O", 0x4D: "P", 0x15: "Q", 0x2D: "R",
    0x1B: "S", 0x2C: "T", 0x3C: "U", 0x2A: "V", 0x1D: "W", 0x22: "X",
    0x35: "Y", 0x1A: "Z",
    0x16: "1", 0x1E: "2", 0x26: "3", 0x25: "4", 0x2E: "5",
    0x36: "6", 0x3D: "7", 0x3E: "8", 0x46: "9", 0x45: "0",
    0x5A: "ENTER", 0x66: "BACKSPACE", 0x29: "SPACE", 0x76: "ESC",
    0x0D: "TAB", 0x12: "L-SHIFT", 0x59: "R-SHIFT",
    0x14: "L-CTRL", 0x11: "L-ALT",
    0x05: "F1", 0x06: "F2", 0x04: "F3", 0x0C: "F4", 0x03: "F5",
    0x0B: "F6", 0x83: "F7", 0x0A: "F8", 0x01: "F9", 0x09: "F10",
    0x78: "F11", 0x07: "F12",
    0xF0: "(break)",
    0xE0: "(extended)",
}


def decode(params):
    clk  = int(params.get("clk_channel", "0"))
    dat  = int(params.get("data_channel", "1"))
    dev  = params.get("device", "keyboard")

    print(f"PS/2: CLK={clk} DATA={dat} device={dev}", file=sys.stderr)

    mouse_byte_index = 0   # 0=button, 1=dx, 2=dy
    mouse_btns = 0
    last_was_break = False

    while True:
        # Each frame begins with a start bit (DATA=0) sampled on a falling
        # CLK edge.  Find a falling edge where DATA reads 0.
        edge = yield from wait_for(falling_edge(clk))
        if edge is None:
            return
        if edge.d[dat] != 0:
            continue   # not a start bit, keep scanning

        start_t = edge.t
        append(start_t, start_t, CH_BYTE, text="",
               sample_type=SAMPLE_PACKET_START)

        # ----- 8 data bits LSB first ----------------------------------------
        byte_val = 0
        ok = True
        for i in range(8):
            e = yield from wait_for(falling_edge(clk))
            if e is None:
                ok = False
                break
            byte_val |= (e.d[dat] << i)
        if not ok:
            return

        # ----- Parity bit (odd) -------------------------------------------
        ep = yield from wait_for(falling_edge(clk))
        if ep is None:
            return
        par_bit = ep.d[dat]
        ones = bin(byte_val).count("1") + par_bit
        par_ok = (ones & 1) == 1

        # ----- Stop bit ---------------------------------------------------
        es = yield from wait_for(falling_edge(clk))
        if es is None:
            return
        end_t = es.t

        append(start_t, end_t, CH_BYTE,
               text=f"0x{byte_val:02X}", data=byte_val,
               sample_type=SAMPLE_DATA)

        if par_ok:
            append(end_t, end_t, CH_PAR, text="PAR OK",
                   color=0xFF88FF88, sample_type=SAMPLE_DATA_ALT)
        else:
            append(end_t, end_t, CH_PAR,
                   text=f"PAR FAIL ({par_bit})",
                   color=0xFFFF8888, sample_type=SAMPLE_DATA_ALT)

        # ----- Interpret meaning ------------------------------------------
        if dev == "keyboard":
            name = SCAN_SET2.get(byte_val, f"sc {byte_val:02X}")
            if byte_val == 0xF0:
                last_was_break = True
                mean_text = "(break next)"
            else:
                prefix = "↑ " if last_was_break else "↓ "
                mean_text = prefix + name
                last_was_break = False
            append(start_t, end_t, CH_MEAN,
                   text=mean_text, sample_type=SAMPLE_DATA,
                   color=0xFFAAEEFF)
        else:  # mouse
            if mouse_byte_index == 0:
                mouse_btns = byte_val
                l = "L" if (byte_val & 0x01) else "-"
                r = "R" if (byte_val & 0x02) else "-"
                m = "M" if (byte_val & 0x04) else "-"
                mean_text = f"BTN [{l}{m}{r}] flags=0x{byte_val:02X}"
                mouse_byte_index = 1
            elif mouse_byte_index == 1:
                dx = byte_val - 256 if (mouse_btns & 0x10) else byte_val
                mean_text = f"dX = {dx:+d}"
                mouse_byte_index = 2
            else:
                dy = byte_val - 256 if (mouse_btns & 0x20) else byte_val
                mean_text = f"dY = {dy:+d}"
                mouse_byte_index = 0
            append(start_t, end_t, CH_MEAN,
                   text=mean_text, sample_type=SAMPLE_DATA,
                   color=0xFFAAEEFF)

        append(end_t, end_t, CH_BYTE, text="",
               sample_type=SAMPLE_PACKET_END)
