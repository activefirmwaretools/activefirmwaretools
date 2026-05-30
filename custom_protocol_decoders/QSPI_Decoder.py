# DECODER_MYNAME = "QSPI"
#
# Decodes Quad SPI (QSPI) flash transactions on six digital channels: CLK, CS, IO0-IO3.
# Supports common command/address/data lane width combinations (1-1-1, 1-1-2, 1-1-4, 1-4-4,
# 4-4-4) plus configurable address byte count and dummy-cycle count.
#
# PARAM: clk_channel | CLK Channel | digital_channel |  |
# PARAM: cs_channel  | CS  Channel | digital_channel |  |
# PARAM: io0_channel | IO0 (MOSI)  | digital_channel |  |
# PARAM: io1_channel | IO1 (MISO)  | digital_channel |  |
# PARAM: io2_channel | IO2 (WP#)   | digital_channel |  |
# PARAM: io3_channel | IO3 (HOLD#) | digital_channel |  |
# PARAM: mode        | Lane Mode   | select | 1-1-1,1-1-2,1-1-4,1-4-4,4-4-4 | 1-1-4
# PARAM: addr_bytes  | Address Bytes | int | 0,5 | 3
# PARAM: dummy_cycles| Dummy SCK Cycles after Address | int | 0,32 | 0
# PARAM: data_bytes  | Data Bytes to Capture (0=until CS deasserts) | int |  | 0
#
# Output rows:
#   ch 0 : command byte (with named opcode where known)
#   ch 1 : address (hex)
#   ch 2 : data bytes (hex sequence)
#
# Protocol notes:
#   - CS active low; transaction frames CS-low..CS-high.
#   - Data sampled on the rising edge of CLK (CPOL=0 CPHA=0 typical).
#   - In single-lane mode, IO0 is MOSI for the host, IO1 is MISO for the
#     target -- both sampled per clock; the decoder displays the IO0
#     stream as the host-driven bytes.
#   - In x4 mode, all four IOs carry one nibble per clock (IO3..IO0 MSB
#     first within the nibble), and two clocks make one byte.

import sys

from active_pro import (
    append, wait_for,
    rising_edge, falling_edge,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_CMD  = 0
CH_ADDR = 1
CH_DATA = 2

OPCODES = {
    0x03: "READ",
    0x0B: "FAST READ",
    0x3B: "FAST READ DUAL OUTPUT",
    0x6B: "FAST READ QUAD OUTPUT",
    0xBB: "FAST READ DUAL IO",
    0xEB: "FAST READ QUAD IO",
    0x02: "PAGE PROGRAM",
    0x32: "QUAD PAGE PROGRAM",
    0x20: "SECTOR ERASE (4K)",
    0xD8: "BLOCK ERASE (64K)",
    0x60: "CHIP ERASE",
    0xC7: "CHIP ERASE",
    0x06: "WRITE ENABLE",
    0x04: "WRITE DISABLE",
    0x05: "READ STATUS REG 1",
    0x35: "READ STATUS REG 2",
    0x01: "WRITE STATUS REG",
    0x9F: "READ JEDEC ID",
    0xAB: "RELEASE PWR DOWN / ID",
    0xB9: "POWER DOWN",
    0xB7: "ENTER 4-BYTE ADDR MODE",
    0xE9: "EXIT 4-BYTE ADDR MODE",
    0x66: "ENABLE RESET",
    0x99: "RESET",
}


def decode(params):
    clk = int(params.get("clk_channel", "0"))
    cs  = int(params.get("cs_channel",  "1"))
    io  = [int(params.get(f"io{n}_channel", str(n + 2))) for n in range(4)]
    mode = params.get("mode", "1-1-4")
    addr_bytes  = int(params.get("addr_bytes", "3"))
    dummy_cyc   = int(params.get("dummy_cycles", "0"))
    data_bytes  = int(params.get("data_bytes", "0"))

    cmd_lanes, addr_lanes, data_lanes = (int(x) for x in mode.split("-"))

    print(f"QSPI: mode={mode} addr={addr_bytes}B dummy={dummy_cyc} "
          f"data_cap={data_bytes or 'all'}", file=sys.stderr)

    while True:
        # ----- Wait for CS falling -----------------------------------------
        cs_low = yield from wait_for(falling_edge(cs))
        if cs_low is None:
            return
        append(cs_low.t, cs_low.t, CH_CMD, text="CS↓",
               sample_type=SAMPLE_PACKET_START)

        # ----- Read command byte on cmd_lanes -------------------------------
        cmd_val, cmd_start_t, cmd_end_t = \
            yield from _read_byte(clk, cs, io, cmd_lanes)
        if cmd_val is None:
            append(cs_low.t, cs_low.t, CH_CMD, text="(empty)",
                   sample_type=SAMPLE_PACKET_END)
            continue
        name = OPCODES.get(cmd_val, f"CMD 0x{cmd_val:02X}")
        append(cmd_start_t, cmd_end_t, CH_CMD,
               text=f"{name} (0x{cmd_val:02X})",
               data=cmd_val, sample_type=SAMPLE_DATA, color=0xFFFFCC88)

        # ----- Read address -------------------------------------------------
        addr_val = 0
        addr_start_t = None
        addr_end_t = cmd_end_t
        for _ in range(addr_bytes):
            byte_val, bs, be = \
                yield from _read_byte(clk, cs, io, addr_lanes)
            if byte_val is None:
                break
            if addr_start_t is None:
                addr_start_t = bs
            addr_end_t = be
            addr_val = (addr_val << 8) | byte_val
        if addr_bytes > 0 and addr_start_t is not None:
            append(addr_start_t, addr_end_t, CH_ADDR,
                   text=f"0x{addr_val:0{addr_bytes * 2}X}",
                   data=addr_val & 0xFFFFFFFF,
                   sample_type=SAMPLE_DATA, color=0xFFAAEEFF)

        # ----- Dummy cycles -------------------------------------------------
        for _ in range(dummy_cyc):
            edge = yield from wait_for(rising_edge(clk))
            if edge is None:
                return
            if edge.d[cs] == 1:
                break   # CS deasserted early

        # ----- Read data bytes ----------------------------------------------
        data_collected = []
        data_start_t = None
        data_end_t = addr_end_t
        n_target = data_bytes if data_bytes > 0 else 1 << 30
        while len(data_collected) < n_target:
            byte_val, bs, be = \
                yield from _read_byte(clk, cs, io, data_lanes)
            if byte_val is None:
                break
            if data_start_t is None:
                data_start_t = bs
            data_end_t = be
            data_collected.append(byte_val)
            # If CS has deasserted, _read_byte returns None on the next call.

        if data_collected:
            hex_blob = " ".join(f"{b:02X}" for b in data_collected)
            if len(hex_blob) > 90:
                hex_blob = hex_blob[:86] + " ..."
            append(data_start_t, data_end_t, CH_DATA,
                   text=f"[{len(data_collected)}B] {hex_blob}",
                   sample_type=SAMPLE_DATA)

        # ----- Wait for CS rising to close packet ---------------------------
        cs_hi = yield from wait_for(rising_edge(cs))
        if cs_hi is None:
            append(data_end_t, data_end_t, CH_CMD, text="CS↑ (EOC)",
                   sample_type=SAMPLE_PACKET_END)
            return
        append(cs_hi.t, cs_hi.t, CH_CMD, text="CS↑",
               sample_type=SAMPLE_PACKET_END)


def _read_byte(clk, cs, io, lanes):
    """Read one byte across `lanes` IO lines (1, 2, or 4), MSB first.
    Returns (byte_val, t_start, t_end) or (None, None, None) when CS rises
    before completion."""
    if lanes == 1:
        bits_per_clk = 1
        io_lines = [io[0]]
    elif lanes == 2:
        bits_per_clk = 2
        io_lines = [io[1], io[0]]            # IO1=MSB, IO0=LSB per nibble pos
    else:
        bits_per_clk = 4
        io_lines = [io[3], io[2], io[1], io[0]]  # IO3..IO0

    byte_val = 0
    bits_collected = 0
    t_start = None
    t_end = None
    while bits_collected < 8:
        edge = yield from wait_for(rising_edge(clk))
        if edge is None:
            return None, None, None
        if edge.d[cs] == 1:
            return None, None, None
        if t_start is None:
            t_start = edge.t
        t_end = edge.t
        for ln in io_lines:
            byte_val = (byte_val << 1) | edge.d[ln]
            bits_collected += 1
            if bits_collected >= 8:
                break
    return byte_val & 0xFF, t_start, t_end
