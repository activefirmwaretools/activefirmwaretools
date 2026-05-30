# DECODER_MYNAME = "SWD"
#
# Decodes ARM Serial Wire Debug on two digital channels (SWCLK + SWDIO).
#
# PARAM: swclk_channel | SWCLK Channel | digital_channel |  |
# PARAM: swdio_channel | SWDIO Channel | digital_channel |  |
#
# Output rows (within this decoder's Device slot):
#   ch 0 : packet header (e.g. "AP READ A[2:3]=0x4")
#   ch 1 : ACK (OK / WAIT / FAULT)
#   ch 2 : data word (32-bit hex) + parity status
#
# Protocol notes:
#   - SWDIO is sampled on the rising edge of SWCLK.
#   - The target drives SWDIO during ACK and read data; the host drives
#     it during the request and write data.  Direction changes are bracketed
#     by a 1-cycle turnaround (TRN) -- the line is undriven for one bit.
#   - Packet format (host phase):
#       Start(1) APnDP(1) RnW(1) A[2:3](2) Parity(1) Stop(0) Park(1)  = 8 bits
#     Then TRN (1 bit), ACK (3 bits), TRN if write, then 32 data bits +
#     1 parity bit. Read: TRN comes after data.
#   - We treat any 50+ cycle SWCLK-quiet gap as a line reset / packet break
#     and resync.
#   - Each transaction is wrapped in SAMPLE_PACKET_START / SAMPLE_PACKET_END.

import sys

from active_pro import (
    append, wait_for,
    rising_edge,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_HDR  = 0
CH_ACK  = 1
CH_DATA = 2

ACK_NAMES = {0b001: "OK", 0b010: "WAIT", 0b100: "FAULT"}


def decode(params):
    swclk = int(params.get("swclk_channel", "0"))
    swdio = int(params.get("swdio_channel", "1"))

    print(f"SWD decoder: SWCLK=ch{swclk} SWDIO=ch{swdio}", file=sys.stderr)

    # Resync loop: find a start bit (SWDIO=1) on a rising SWCLK edge,
    # then attempt to decode a request packet.
    while True:
        edge = yield from wait_for(rising_edge(swclk))
        if edge is None:
            return

        # Start bit must be 1.
        if edge.d[swdio] != 1:
            continue

        pkt_start_t = edge.t
        append(pkt_start_t, pkt_start_t, CH_HDR,
               text="", sample_type=SAMPLE_PACKET_START)

        # ----- 7 more request bits (APnDP, RnW, A2, A3, Par, Stop, Park) ----
        bits = [1]   # start bit already known
        bit_times = [edge.t]
        for _ in range(7):
            e = yield from wait_for(rising_edge(swclk))
            if e is None:
                return
            bits.append(e.d[swdio])
            bit_times.append(e.t)
        last_req_t = bit_times[-1]

        ap_n_dp = bits[1]
        r_n_w   = bits[2]
        a2_a3   = (bits[3] | (bits[4] << 1))    # encoded as 2 bits, addr nibble
        par_in  = bits[5]
        # Validate parity of (APnDP, RnW, A2, A3).
        ones = bits[1] + bits[2] + bits[3] + bits[4]
        par_ok = ((ones & 1) == par_in)

        tag = "AP" if ap_n_dp else "DP"
        op  = "READ" if r_n_w else "WRITE"
        addr = a2_a3 << 2   # bits[3] is A[2], bits[4] is A[3], so addr nibble
        text = f"{tag} {op} A=0x{addr:X}"
        if not par_ok:
            text += " !PAR"

        append(pkt_start_t, last_req_t, CH_HDR,
               text=text, data=(bits[1] << 3) | (bits[2] << 2) | a2_a3,
               sample_type=SAMPLE_DATA,
               color=0xFFAAEEFF)

        # ----- TRN cycle (1 bit, line undriven) -----------------------------
        e = yield from wait_for(rising_edge(swclk))
        if e is None:
            return

        # ----- ACK (3 bits, target driven, LSB first) -----------------------
        ack = 0
        ack_start_t = None
        for i in range(3):
            ea = yield from wait_for(rising_edge(swclk))
            if ea is None:
                return
            if ack_start_t is None:
                ack_start_t = ea.t
            ack |= (ea.d[swdio] << i)
            ack_end_t = ea.t
        ack_name = ACK_NAMES.get(ack, f"0b{ack:03b}")
        ack_color = 0xFF88FF88 if ack == 0b001 else 0xFFFF8888
        append(ack_start_t, ack_end_t, CH_ACK,
               text=ack_name, data=ack, color=ack_color,
               sample_type=SAMPLE_DATA_ALT)

        # Only OK transactions exchange data.
        if ack != 0b001:
            append(ack_end_t, ack_end_t, CH_HDR,
                   text="", sample_type=SAMPLE_PACKET_END)
            continue

        # ----- TRN for write only (read keeps the same direction) ----------
        if r_n_w == 0:
            e = yield from wait_for(rising_edge(swclk))
            if e is None:
                return

        # ----- 32 data bits + 1 parity (LSB first) -------------------------
        data_val = 0
        data_start_t = None
        for i in range(32):
            ed = yield from wait_for(rising_edge(swclk))
            if ed is None:
                return
            if data_start_t is None:
                data_start_t = ed.t
            data_val |= (ed.d[swdio] << i)
            data_end_t = ed.t

        ep = yield from wait_for(rising_edge(swclk))
        if ep is None:
            return
        par_data_in = ep.d[swdio]
        ones_data = bin(data_val).count("1")
        data_par_ok = ((ones_data & 1) == par_data_in)

        dtext = f"0x{data_val:08X}"
        if not data_par_ok:
            dtext += " !PAR"
        append(data_start_t, ep.t, CH_DATA,
               text=dtext, data=data_val & 0xFFFFFFFF,
               sample_type=SAMPLE_DATA)

        append(ep.t, ep.t, CH_HDR,
               text="", sample_type=SAMPLE_PACKET_END)

        # For read: 1 TRN cycle (we don't strictly need to consume it since
        # the next loop iteration just finds the next start bit).
