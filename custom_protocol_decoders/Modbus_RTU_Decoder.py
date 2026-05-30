# DECODER_MYNAME = "Modbus RTU"
#
# Decodes Modbus RTU framing on top of a UART signal (one digital channel).
# The transport is plain serial -- Modbus RTU groups bytes into frames by
# observing >=3.5-character idle gaps between frames.
#
# PARAM: channel    | Logic Channel | digital_channel |  |
# PARAM: baud_rate  | Baud Rate     | select | 9600,19200,38400,57600,115200 | 19200
# PARAM: parity     | Parity        | select | None,Even,Odd | Even
# PARAM: invert     | Invert        | bool   |        | false
#
# Output rows:
#   ch 0 : address byte
#   ch 1 : function code name + raw
#   ch 2 : data bytes (hex blob)
#   ch 3 : CRC verdict ("CRC OK" / "CRC FAIL got=0xXXXX want=0xXXXX")
#
# Protocol notes:
#   - Standard frame: addr(1) + func(1) + data(N) + CRC(2).
#   - Inter-character spacing inside a frame is < 1.5 char times.
#   - Inter-frame spacing is >= 3.5 char times.  We use this gap to split
#     bytestreams into frames.
#   - CRC-16 polynomial 0xA001 (reflected 0x8005), initial value 0xFFFF.

import sys

from active_pro import (
    append, wait_for, wait_time,
    falling_edge, rising_edge,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_ADDR = 0
CH_FUNC = 1
CH_DATA = 2
CH_CRC  = 3

FUNC_NAMES = {
    0x01: "Read Coils",
    0x02: "Read Discrete Inputs",
    0x03: "Read Holding Regs",
    0x04: "Read Input Regs",
    0x05: "Write Single Coil",
    0x06: "Write Single Reg",
    0x07: "Read Exception Status",
    0x0F: "Write Multiple Coils",
    0x10: "Write Multiple Regs",
    0x11: "Report Slave ID",
    0x14: "Read File Record",
    0x15: "Write File Record",
    0x16: "Mask Write Reg",
    0x17: "Read/Write Multiple Regs",
}


def _crc16(data):
    crc = 0xFFFF
    for b in data:
        crc ^= b
        for _ in range(8):
            crc = (crc >> 1) ^ 0xA001 if (crc & 1) else (crc >> 1)
    return crc & 0xFFFF


def decode(params):
    ch       = int(params.get("channel", "0"))
    baud     = int(params.get("baud_rate", "19200"))
    parity   = params.get("parity", "Even")
    invert   = params.get("invert", "false").lower() == "true"

    bit_time   = 1.0 / baud
    bits_per_char = 1 + 8 + (1 if parity != "None" else 0) + 1  # start+data+par+stop
    char_time   = bit_time * bits_per_char
    frame_gap   = 3.5 * char_time

    start_edge = rising_edge(ch) if invert else falling_edge(ch)

    print(f"Modbus RTU: ch={ch} {baud} 8{parity[0]}1 invert={invert}",
          file=sys.stderr)

    frame_bytes = []
    frame_times = []   # (start_t, end_t) per byte

    while True:
        # Wait for next start bit.  If it doesn't appear within frame_gap,
        # we've passed an inter-frame gap and should flush.
        sb = yield from wait_for(start_edge, timeout=frame_gap)
        if sb is None:
            if frame_bytes:
                _emit_frame(frame_bytes, frame_times)
                frame_bytes = []
                frame_times = []
            # Continue waiting indefinitely for the next start bit.
            sb = yield from wait_for(start_edge)
            if sb is None:
                return

        # Sample 8 data bits middle-of-bit.
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
        # Skip parity (if any) + stop bit time
        if parity != "None":
            m = yield from wait_time(bit_time)
            if m is None:
                return
        m = yield from wait_time(bit_time)
        if m is None:
            return

        frame_bytes.append(byte_val)
        frame_times.append((sb.t, sb.t + bits_per_char * bit_time))


def _emit_frame(bytes_list, times_list):
    if len(bytes_list) < 4:
        return   # too short to be a valid Modbus RTU frame
    addr = bytes_list[0]
    func = bytes_list[1]
    data = bytes_list[2:-2]
    crc_got = bytes_list[-2] | (bytes_list[-1] << 8)
    crc_want = _crc16(bytes_list[:-2])

    t0 = times_list[0][0]
    t_end = times_list[-1][1]
    t_addr_end = times_list[0][1]
    t_func_end = times_list[1][1]
    t_data_start = times_list[2][0] if data else t_func_end
    t_data_end   = times_list[-3][1] if data else t_func_end
    t_crc_start  = times_list[-2][0]

    append(t0, t0, CH_ADDR, text="", sample_type=SAMPLE_PACKET_START)
    append(t0, t_addr_end, CH_ADDR,
           text=f"ADDR {addr}", data=addr, sample_type=SAMPLE_DATA,
           color=0xFFAAEEFF)

    fname = FUNC_NAMES.get(func & 0x7F, f"Func 0x{func:02X}")
    if func & 0x80:
        fname = f"EXCEPTION ({fname})"
        f_color = 0xFFFF8888
    else:
        f_color = 0xFFFFCC88
    append(times_list[1][0], t_func_end, CH_FUNC,
           text=f"{fname} (0x{func:02X})", data=func,
           sample_type=SAMPLE_DATA, color=f_color)

    if data:
        hex_blob = " ".join(f"{b:02X}" for b in data)
        if len(hex_blob) > 100:
            hex_blob = hex_blob[:96] + " ..."
        append(t_data_start, t_data_end, CH_DATA,
               text=f"[{len(data)}B] {hex_blob}",
               sample_type=SAMPLE_DATA)

    if crc_got == crc_want:
        append(t_crc_start, t_end, CH_CRC,
               text=f"CRC OK (0x{crc_got:04X})",
               color=0xFF88FF88, sample_type=SAMPLE_DATA)
    else:
        append(t_crc_start, t_end, CH_CRC,
               text=f"CRC FAIL got=0x{crc_got:04X} want=0x{crc_want:04X}",
               color=0xFFFF8888, sample_type=SAMPLE_DATA)

    append(t_end, t_end, CH_ADDR, text="", sample_type=SAMPLE_PACKET_END)
