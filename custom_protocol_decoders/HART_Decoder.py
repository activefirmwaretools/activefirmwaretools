# DECODER_MYNAME = "HART"
#
# Decodes HART Bell 202 FSK traffic modulated on a 4-20mA current loop (one analog channel).
# HART (Highway Addressable Remote Transducer) uses 1200 baud FSK with mark=1200 Hz and
# space=2200 Hz, riding on top of the slowly-varying DC loop current.
#
# PARAM: channel    | Analog Channel        | analog_channel |  | 1
# PARAM: ac_amplitude | Min Peak AC Amplitude (V) | float | 0.001,5 | 0.05
#
# Output rows:
#   ch 0 : raw byte (hex) + parity status
#   ch 1 : HART frame summary (preamble, delimiter, addr, cmd, byte count)
#   ch 2 : data payload (hex blob)
#
# Method:
#   - The decoder estimates the instantaneous frequency between zero
#     crossings of the AC component (signal minus its 50-sample moving
#     mean).  Half-cycle period ~= 1 / (2*freq); periods near 0.83 ms
#     (1200 Hz) emit a "1" bit, near 0.45 ms (2200 Hz) emit a "0" bit.
#   - Bits are accumulated into UART-style 8O1 framing (1 start bit,
#     8 data LSB-first, 1 odd parity, 1 stop bit -- HART spec).
#   - HART frames begin with 5-20 preamble bytes (0xFF), a start
#     delimiter, address (1 or 5 bytes), command, byte count, data,
#     and a longitudinal parity check (XOR of all preceding bytes
#     after the preamble).
#
# Limitations:
#   - This decoder uses very simple zero-crossing tone detection and is
#     intended for clean lab captures.  Real plant signals with heavy
#     noise will require a true Goertzel / FFT implementation.

import sys
from collections import deque

from active_pro import (
    append, wait_time,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_BYTE  = 0
CH_FRAME = 1
CH_DATA  = 2

BAUD = 1200
BIT_TIME = 1.0 / BAUD
MARK_HZ  = 1200.0
SPACE_HZ = 2200.0
MARK_PERIOD  = 1.0 / MARK_HZ
SPACE_PERIOD = 1.0 / SPACE_HZ


def decode(params):
    a_ch    = int(params.get("channel", "1")) - 1
    min_amp = float(params.get("ac_amplitude", "0.05"))

    poll = BIT_TIME / 32.0   # ~26 us at 1200 baud
    window = 64              # samples in DC-removal moving average
    samples = deque(maxlen=window)

    print(f"HART: A{a_ch + 1} poll={poll*1e6:.1f}us "
          f"min_amp={min_amp}V", file=sys.stderr)

    last_zc_t = None
    half_period_buf = []   # last 4 half periods for averaging
    bit_buf = []           # accumulated bits since last UART byte
    accumulating_byte = False
    byte_start_t = None
    frame_bytes = []
    frame_start_t = None
    in_preamble = False

    prev_sample_above = None

    while True:
        m = yield from wait_time(poll)
        if m is None:
            return
        v = m.a[a_ch]
        samples.append(v)
        if len(samples) < window // 2:
            continue
        dc = sum(samples) / len(samples)
        ac = v - dc
        if abs(ac) < min_amp:
            # Too quiet to be HART; if we have a partial byte, drop it.
            bit_buf = []
            accumulating_byte = False
            if frame_bytes:
                _emit_frame(frame_bytes, frame_start_t, m.t)
                frame_bytes = []
                frame_start_t = None
            prev_sample_above = None
            continue

        above = ac > 0
        if prev_sample_above is None:
            prev_sample_above = above
            continue
        if above == prev_sample_above:
            continue
        # Zero crossing.
        if last_zc_t is None:
            last_zc_t = m.t
            prev_sample_above = above
            continue
        half_T = m.t - last_zc_t
        last_zc_t = m.t
        prev_sample_above = above

        # Classify the half-cycle: closer to MARK or SPACE half-period?
        if abs(half_T - MARK_PERIOD / 2.0) < abs(half_T - SPACE_PERIOD / 2.0):
            bit_classification = 1   # mark
        else:
            bit_classification = 0   # space

        half_period_buf.append((m.t, bit_classification))

        # We need ~2 half periods per bit at MARK (1200Hz), ~3-4 at SPACE.
        # Build bits at 1200 baud: at each bit-time boundary, take the
        # majority of half-cycles in that bit cell.
        # Simpler: align by the first observed transition and clock out
        # bits every BIT_TIME from then on.
        # For brevity in this lab-grade decoder we approximate: every time
        # we accumulate enough half periods that span >= BIT_TIME, emit a
        # bit equal to the majority classification within that span.
        if len(half_period_buf) >= 2:
            span = half_period_buf[-1][0] - half_period_buf[0][0]
            if span >= BIT_TIME:
                ones = sum(b for _, b in half_period_buf)
                bit_val = 1 if ones * 2 >= len(half_period_buf) else 0
                half_period_buf = []

                # Feed bit into UART framer (8O1).
                if not accumulating_byte:
                    if bit_val == 0:
                        accumulating_byte = True
                        byte_start_t = m.t
                        bit_buf = []
                    # else: idle / mark -- keep waiting
                else:
                    bit_buf.append(bit_val)
                    if len(bit_buf) == 9:   # 8 data + parity
                        byte_val = 0
                        for i in range(8):
                            byte_val |= (bit_buf[i] << i)
                        par_bit = bit_buf[8]
                        ones_count = bin(byte_val).count("1") + par_bit
                        par_ok = (ones_count & 1) == 1
                        text = f"0x{byte_val:02X}"
                        if not par_ok:
                            text += " !PE"
                        append(byte_start_t, m.t, CH_BYTE,
                               text=text, data=byte_val,
                               sample_type=SAMPLE_DATA,
                               color=0xFFAAEEFF if par_ok else 0xFFFF8888)

                        # Frame assembly: preamble 0xFF runs, then delimiter.
                        if byte_val == 0xFF and not frame_bytes:
                            in_preamble = True
                            frame_start_t = byte_start_t
                            frame_bytes = [byte_val]
                        elif in_preamble and byte_val == 0xFF:
                            frame_bytes.append(byte_val)
                        elif in_preamble:
                            in_preamble = False
                            frame_bytes.append(byte_val)
                        elif frame_bytes:
                            frame_bytes.append(byte_val)
                            # Heuristic: emit frame on idle gap; we have no
                            # gap detection here, so emit conservatively
                            # when frame length looks reasonable (>= 8 +
                            # byte_count + 1).  Try parsing byte_count.
                            if len(frame_bytes) >= 8:
                                # Find delimiter index = first non-0xFF
                                try:
                                    di = next(i for i, b in enumerate(frame_bytes)
                                              if b != 0xFF)
                                except StopIteration:
                                    di = None
                                if di is not None and len(frame_bytes) - di >= 5:
                                    # short addr=1B, long=5B; bit 7 of delim
                                    delim = frame_bytes[di]
                                    addr_len = 5 if (delim & 0x80) else 1
                                    bc_idx = di + 1 + addr_len + 1
                                    if bc_idx < len(frame_bytes):
                                        bc = frame_bytes[bc_idx]
                                        total = bc_idx + 1 + bc + 1
                                        if len(frame_bytes) >= total:
                                            _emit_frame(frame_bytes,
                                                        frame_start_t, m.t)
                                            frame_bytes = []
                                            frame_start_t = None
                                            in_preamble = False
                        bit_buf = []
                        accumulating_byte = False


def _emit_frame(frame_bytes, t_start, t_end):
    # Find delimiter
    try:
        di = next(i for i, b in enumerate(frame_bytes) if b != 0xFF)
    except StopIteration:
        return
    preamble = frame_bytes[:di]
    delim = frame_bytes[di]
    addr_len = 5 if (delim & 0x80) else 1
    addr = frame_bytes[di + 1: di + 1 + addr_len]
    if di + 1 + addr_len >= len(frame_bytes):
        return
    cmd = frame_bytes[di + 1 + addr_len]
    bc_idx = di + 2 + addr_len
    if bc_idx >= len(frame_bytes):
        return
    bc = frame_bytes[bc_idx]
    data = frame_bytes[bc_idx + 1: bc_idx + 1 + bc]
    chk_idx = bc_idx + 1 + bc
    chk_got = frame_bytes[chk_idx] if chk_idx < len(frame_bytes) else None

    # Longitudinal parity check: XOR of delim + addr + cmd + bc + data
    lrc = delim
    for b in addr + [cmd, bc] + data:
        lrc ^= b
    chk_ok = (chk_got == lrc) if chk_got is not None else False

    addr_str = ":".join(f"{b:02X}" for b in addr)
    summary = (f"PRE x{len(preamble)} DELIM=0x{delim:02X} "
               f"ADDR={addr_str} CMD={cmd} BC={bc}")
    if chk_got is not None:
        summary += (f" CHK={'OK' if chk_ok else 'FAIL'} "
                    f"(0x{chk_got:02X}/0x{lrc:02X})")
    color = 0xFF88FF88 if chk_ok else 0xFFFF8888
    append(t_start, t_end, CH_FRAME, text=summary,
           color=color, sample_type=SAMPLE_DATA)
    if data:
        hex_blob = " ".join(f"{b:02X}" for b in data)
        append(t_start, t_end, CH_DATA,
               text=f"[{len(data)}B] {hex_blob}",
               sample_type=SAMPLE_DATA)
