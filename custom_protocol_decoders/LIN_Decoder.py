# DECODER_MYNAME = "LIN bus"
#
# Decodes ISO 17987 LIN (Local Interconnect Network) frames on one analog channel.
# LIN is a single-wire 12V automotive bus carrying a UART-style stream framed by a long
# dominant BREAK at the start of every header.
#
# PARAM: channel    | Analog Channel    | analog_channel |  | 1
# PARAM: baud_rate  | Baud Rate         | select | 2400,9600,10417,19200,20000 | 19200
# PARAM: threshold  | Threshold (V)     | float  | 0,12 | 6.0
# PARAM: hysteresis | Hysteresis (V)    | float  | 0,3  | 0.5
# PARAM: data_bytes | Max Data Bytes    | int    | 1,8  | 8
# PARAM: classic_chk| Classic Checksum  | bool   |      | false
#
# Output rows:
#   ch 0 : break + sync
#   ch 1 : protected ID (PID) -- raw + parity verdict + ID
#   ch 2 : data bytes (hex blob)
#   ch 3 : checksum verdict
#
# Notes:
#   - LIN idle is HIGH (recessive ~12 V).  A BREAK is at least 13 bit
#     times of low (~0 V).
#   - After BREAK + 1+ idle bits, the Sync byte 0x55 (LSB first) is sent,
#     followed by the PID byte (6-bit ID + 2 parity bits).
#   - 1..8 data bytes follow, then a checksum byte.  "Classic" checksum
#     sums data bytes only; "enhanced" checksum (LIN 2.x) includes the
#     PID as well.

import sys

from active_pro import (
    append, wait_for, wait_time,
    voltage_falls, voltage_rises,
)


SAMPLE_DATA         = 0
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_SYNC = 0
CH_PID  = 1
CH_DATA = 2
CH_CHK  = 3


def _lin_pid_id(pid):
    return pid & 0x3F


def _lin_pid_parity_ok(pid):
    id_ = pid & 0x3F
    p0_calc = ((id_ >> 0) ^ (id_ >> 1) ^ (id_ >> 2) ^ (id_ >> 4)) & 1
    p1_calc = (~((id_ >> 1) ^ (id_ >> 3) ^ (id_ >> 4) ^ (id_ >> 5))) & 1
    p0 = (pid >> 6) & 1
    p1 = (pid >> 7) & 1
    return (p0 == p0_calc) and (p1 == p1_calc)


def _lin_chk(data, pid=None):
    """Enhanced if pid given, classic if pid is None.  Returns 8-bit value."""
    s = 0
    if pid is not None:
        s += pid
    for b in data:
        s += b
        if s >= 256:
            s = (s + 1) - 256   # 1's-complement add
    return (~s) & 0xFF


def decode(params):
    a_ch    = int(params.get("channel", "1")) - 1
    baud    = int(params.get("baud_rate", "19200"))
    thr     = float(params.get("threshold", "6.0"))
    hyst    = float(params.get("hysteresis", "0.5"))
    nbytes  = max(1, min(8, int(params.get("data_bytes", "8"))))
    classic = params.get("classic_chk", "false").lower() == "true"

    bit_time = 1.0 / baud
    break_min = 13.0 * bit_time

    print(f"LIN: A{a_ch + 1} {baud} bps thr={thr}V data<={nbytes} "
          f"classic={classic}", file=sys.stderr)

    while True:
        # ---- Find a BREAK: voltage_falls then stays low >= break_min ----
        f = yield from wait_for(voltage_falls(a_ch, thr, hyst))
        if f is None:
            return
        r = yield from wait_for(voltage_rises(a_ch, thr, hyst))
        if r is None:
            return
        if (r.t - f.t) < break_min:
            continue

        append(f.t, r.t, CH_SYNC, text="BREAK",
               sample_type=SAMPLE_PACKET_START, color=0xFFAAAAFF)

        # ---- Sync byte (must be 0x55) ----------------------------------
        sb = yield from wait_for(voltage_falls(a_ch, thr, hyst))
        if sb is None:
            return
        sync_val, _, sync_end = yield from _read_uart_byte(
            a_ch, thr, bit_time, sb)
        if sync_val is None:
            return
        sync_color = 0xFF88FF88 if sync_val == 0x55 else 0xFFFF8888
        append(sb.t, sync_end, CH_SYNC,
               text=f"SYNC 0x{sync_val:02X}",
               color=sync_color, sample_type=SAMPLE_DATA)

        # ---- PID byte --------------------------------------------------
        pb = yield from wait_for(voltage_falls(a_ch, thr, hyst))
        if pb is None:
            return
        pid_val, _, pid_end = yield from _read_uart_byte(
            a_ch, thr, bit_time, pb)
        if pid_val is None:
            return
        id_field = _lin_pid_id(pid_val)
        par_ok = _lin_pid_parity_ok(pid_val)
        pid_color = 0xFFAAEEFF if par_ok else 0xFFFF8888
        text = f"PID 0x{pid_val:02X} ID=0x{id_field:02X}"
        if not par_ok:
            text += " !PAR"
        append(pb.t, pid_end, CH_PID, text=text,
               data=pid_val, color=pid_color, sample_type=SAMPLE_DATA)

        # ---- Data bytes ------------------------------------------------
        data = []
        last_end = pid_end
        data_start = None
        for i in range(nbytes):
            db = yield from wait_for(voltage_falls(a_ch, thr, hyst))
            if db is None:
                break
            byte_val, _, byte_end = yield from _read_uart_byte(
                a_ch, thr, bit_time, db)
            if byte_val is None:
                break
            data.append(byte_val)
            if data_start is None:
                data_start = db.t
            last_end = byte_end
            # Crude check: if the gap to the next falling edge would have
            # to be the checksum (no more data on the bus), stop early.

        if data:
            hex_blob = " ".join(f"{b:02X}" for b in data)
            append(data_start, last_end, CH_DATA,
                   text=f"[{len(data)}B] {hex_blob}",
                   sample_type=SAMPLE_DATA)

        # ---- Checksum byte -------------------------------------------
        cb = yield from wait_for(voltage_falls(a_ch, thr, hyst))
        if cb is None:
            append(last_end, last_end, CH_SYNC, text="",
                   sample_type=SAMPLE_PACKET_END)
            continue
        chk_val, _, chk_end = yield from _read_uart_byte(
            a_ch, thr, bit_time, cb)
        if chk_val is None:
            return
        if classic:
            chk_want = _lin_chk(data, pid=None)
        else:
            chk_want = _lin_chk(data, pid=pid_val)
        chk_ok = (chk_val == chk_want)
        text = (f"CHK OK (0x{chk_val:02X})" if chk_ok
                else f"CHK FAIL: got 0x{chk_val:02X} want 0x{chk_want:02X}")
        color = 0xFF88FF88 if chk_ok else 0xFFFF8888
        append(cb.t, chk_end, CH_CHK, text=text,
               color=color, sample_type=SAMPLE_DATA)

        append(chk_end, chk_end, CH_SYNC, text="",
               sample_type=SAMPLE_PACKET_END)


def _read_uart_byte(a_ch, thr, bit_time, start):
    """Read one UART byte (8N1) starting from `start` (falling crossing of
    the start bit).  Returns (byte_val, start_t, end_t) or
    (None, None, None) on end-of-capture."""
    # Skip to middle of bit 0.
    m = yield from wait_time(1.5 * bit_time)
    if m is None:
        return None, None, None
    byte_val = 0
    for i in range(8):
        bit = 1 if m.a[a_ch] > thr else 0
        byte_val |= (bit << i)
        if i < 7:
            m = yield from wait_time(bit_time)
            if m is None:
                return None, None, None
    end_t = start.t + 10 * bit_time
    return byte_val, start.t, end_t
