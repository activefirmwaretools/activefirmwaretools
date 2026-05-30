# DECODER_MYNAME = "MIDI"
#
# Decodes a MIDI byte stream (31250 baud) on one digital channel, TTL/UART side.
#
# PARAM: channel | Logic Channel | digital_channel |  |
# PARAM: invert  | Invert        | bool |  | false
#
# Output rows:
#   ch 0 : raw byte (hex)
#   ch 1 : interpreted message (e.g. "NoteOn ch=1 note=60 (C4) vel=100")
#
# Protocol notes:
#   - UART framing: 8N1 at 31250 baud (32 us per bit).
#   - Status bytes have MSB=1; data bytes have MSB=0.
#   - Running status: when no new status byte is sent between messages,
#     the previous channel-voice status repeats.
#   - System Real-Time (0xF8..0xFF) and System Common (0xF1..0xF7)
#     are decoded; SysEx (0xF0..0xF7) is shown as a hex blob.

import sys

from active_pro import (
    append, wait_for, wait_time,
    falling_edge, rising_edge,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_BYTE = 0
CH_MSG  = 1

BAUD = 31250

NOTE_NAMES = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"]


def _note(num):
    return f"{NOTE_NAMES[num % 12]}{(num // 12) - 1}"


def decode(params):
    ch     = int(params.get("channel", "0"))
    invert = params.get("invert", "false").lower() == "true"
    bit_time = 1.0 / BAUD
    start_edge = rising_edge(ch) if invert else falling_edge(ch)

    print(f"MIDI: ch={ch} {BAUD} baud invert={invert}", file=sys.stderr)

    running_status = None
    expected_data = 0
    data_buf = []
    msg_start_t = None
    in_sysex = False
    sysex_buf = []

    while True:
        start = yield from wait_for(start_edge)
        if start is None:
            return

        # Sample 8 bits, middle of each bit cell.
        m = yield from wait_time(1.5 * bit_time)
        if m is None:
            return
        byte_val = 0
        for i in range(8):
            bit = m.d[ch] ^ (1 if invert else 0)
            byte_val |= (bit << i)
            if i < 7:
                m = yield from wait_time(bit_time)
                if m is None:
                    return
        end_t = start.t + 10 * bit_time   # approximate end of stop bit

        append(start.t, end_t, CH_BYTE,
               text=f"0x{byte_val:02X}", data=byte_val,
               sample_type=SAMPLE_DATA)

        # ----- Interpret the byte stream ----------------------------------
        if in_sysex:
            if byte_val == 0xF7:
                hex_blob = " ".join(f"{b:02X}" for b in sysex_buf)
                append(msg_start_t, end_t, CH_MSG,
                       text=f"SysEx [{len(sysex_buf)}B] {hex_blob}",
                       sample_type=SAMPLE_DATA, color=0xFFCCAAFF)
                in_sysex = False
                sysex_buf = []
            else:
                sysex_buf.append(byte_val)
            continue

        if byte_val & 0x80:
            # Status byte
            if byte_val == 0xF0:
                in_sysex = True
                sysex_buf = []
                msg_start_t = start.t
                continue
            if 0xF8 <= byte_val <= 0xFF:
                # System Real-Time -- single byte, doesn't disturb running status
                rt_name = {
                    0xF8: "Clock", 0xFA: "Start", 0xFB: "Continue",
                    0xFC: "Stop", 0xFE: "ActiveSense", 0xFF: "Reset",
                }.get(byte_val, f"RT 0x{byte_val:02X}")
                append(start.t, end_t, CH_MSG, text=rt_name,
                       sample_type=SAMPLE_DATA, color=0xFF88FF88)
                continue

            running_status = byte_val
            msg_start_t = start.t
            data_buf = []
            high_nib = byte_val & 0xF0
            if high_nib in (0xC0, 0xD0):
                expected_data = 1
            elif 0xF1 <= byte_val <= 0xF6:
                expected_data = {0xF1: 1, 0xF2: 2, 0xF3: 1}.get(byte_val, 0)
            else:
                expected_data = 2
        else:
            # Data byte
            if running_status is None:
                # Spurious data with no status -- skip
                continue
            data_buf.append(byte_val)
            if len(data_buf) >= expected_data:
                _emit_message(running_status, data_buf, msg_start_t, end_t)
                data_buf = []


def _emit_message(status, data, t_start, t_end):
    ch_num = (status & 0x0F) + 1
    kind   = status & 0xF0
    if kind == 0x80 and len(data) >= 2:
        msg = f"NoteOff ch={ch_num} note={data[0]} ({_note(data[0])}) vel={data[1]}"
        color = 0xFFAAEEFF
    elif kind == 0x90 and len(data) >= 2:
        if data[1] == 0:
            msg = f"NoteOff(vel0) ch={ch_num} note={data[0]} ({_note(data[0])})"
        else:
            msg = f"NoteOn  ch={ch_num} note={data[0]} ({_note(data[0])}) vel={data[1]}"
        color = 0xFFAAFFAA
    elif kind == 0xA0 and len(data) >= 2:
        msg = f"Aftertouch ch={ch_num} note={data[0]} ({_note(data[0])}) p={data[1]}"
        color = 0xFFFFCC88
    elif kind == 0xB0 and len(data) >= 2:
        msg = f"CC ch={ch_num} cc#{data[0]}={data[1]}"
        color = 0xFFFFCC88
    elif kind == 0xC0 and len(data) >= 1:
        msg = f"ProgramChange ch={ch_num} prog={data[0]}"
        color = 0xFFFFCC88
    elif kind == 0xD0 and len(data) >= 1:
        msg = f"ChannelPressure ch={ch_num} p={data[0]}"
        color = 0xFFFFCC88
    elif kind == 0xE0 and len(data) >= 2:
        bend = (data[1] << 7) | data[0]
        msg = f"PitchBend ch={ch_num} val={bend}"
        color = 0xFFCCAAFF
    else:
        msg = f"0x{status:02X} " + " ".join(f"{b:02X}" for b in data)
        color = 0xFFCCCCCC
    append(t_start, t_end, CH_MSG, text=msg,
           sample_type=SAMPLE_DATA, color=color)
