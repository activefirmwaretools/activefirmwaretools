# DECODER_MYNAME = "CAN bus"
#
# Decodes ISO 11898 CAN 2.0A/B frames on the CANH/CANL differential pair using two analog channels.
#
# A CAN "dominant" bit drives CANH high and CANL low (differential
# positive); a "recessive" bit floats both lines to the bus midpoint
# (differential ~0).  After differential thresholding, dominant = 0
# and recessive = 1 -- matching the standard CAN logical convention.
#
# PARAM: canh_channel | CANH Analog Channel | analog_channel |  | 1
# PARAM: canl_channel | CANL Analog Channel | analog_channel |  | 2
# PARAM: bit_rate     | Bit Rate (bps)      | select | 10000,20000,33333,50000,83333,100000,125000,250000,500000,800000,1000000 | 500000
# PARAM: threshold    | Diff Threshold (V)  | float  | 0,5  | 0.9
# PARAM: invert       | Invert Differential | bool   |      | false
# PARAM: sample_point | Sample Point (%)    | int    | 50,90 | 75
#
# Output rows:
#   ch 0 : frame summary (e.g. "ID=0x123 DLC=8 D=...")
#   ch 1 : individual fields (SOF, ID, RTR, IDE, DLC, DATA, CRC, ACK, EOF)
#   ch 2 : errors / form errors
#
# Limitations:
#   - Bit timing is assumed nominal (no PLL).  The sample point is taken
#     at `sample_point`% of each bit time after the SOF transition.
#   - Bit stuffing (a stuffed bit inserted after 5 consecutive same-value
#     bits) is removed.
#   - CRC is checked using polynomial 0x4599 (CAN 2.0).
#   - Extended (29-bit) and standard (11-bit) IDs are both detected.

import sys

from active_pro import (
    append, wait_for, wait_time,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_FRAME = 0
CH_FIELD = 1
CH_ERR   = 2


def _diff_bit(m, h_ch, l_ch, threshold, sign):
    """1 = recessive, 0 = dominant."""
    d = sign * (m.a[h_ch] - m.a[l_ch])
    return 0 if d > threshold else 1


def _crc15(bits):
    """CAN CRC-15 with poly 0x4599 over a list of 0/1 bits."""
    crc = 0
    for b in bits:
        crc_next = (crc << 1) & 0x7FFF
        if (b ^ ((crc >> 14) & 1)) == 1:
            crc_next ^= 0x4599
        crc = crc_next
    return crc & 0x7FFF


def decode(params):
    h_ch     = int(params.get("canh_channel", "1")) - 1
    l_ch     = int(params.get("canl_channel", "2")) - 1
    baud     = int(params.get("bit_rate", "500000"))
    threshold = float(params.get("threshold", "0.9"))
    invert   = params.get("invert", "false").lower() == "true"
    sp       = max(50, min(90, int(params.get("sample_point", "75")))) / 100.0
    sign     = -1.0 if invert else 1.0

    bit_time = 1.0 / baud

    print(f"CAN: H=ch{h_ch + 1} L=ch{l_ch + 1} {baud} bps thr={threshold} "
          f"sp={int(sp*100)}%", file=sys.stderr)

    poll = bit_time / 16.0

    while True:
        # ----- Scan idle for first dominant (SOF) ---------------------------
        sof_t = None
        while True:
            m = yield from wait_time(poll)
            if m is None:
                return
            if _diff_bit(m, h_ch, l_ch, threshold, sign) == 0:
                sof_t = m.t
                break

        # ----- Sample bits at sample_point of each cell ---------------------
        # Skip to first sample point.
        m = yield from wait_time(sp * bit_time)
        if m is None:
            return
        bits = [0]   # SOF dominant
        prev_bits = [0]   # for bit-stuffing tracking
        run = 1
        # We'll collect bits until we observe the End-of-Frame (7 recessive)
        # or run out of data.  Bit stuffing removes the bit after 5 same.
        unstuffed_bits = [0]
        max_bits = 200   # safety cap

        for k in range(max_bits):
            m2 = yield from wait_time(bit_time)
            if m2 is None:
                break
            b = _diff_bit(m2, h_ch, l_ch, threshold, sign)
            bits.append(b)

            # Bit-stuffing removal applies from SOF up to and including CRC.
            # We don't know the end of CRC until we parse SRR/IDE/DLC, so
            # we just collect raw bits here and unstuff afterwards.
            if bits[-1] == prev_bits[-1] if prev_bits else False:
                run += 1
            else:
                run = 1
            prev_bits.append(b)

            # End-of-frame heuristic: 7 consecutive recessive bits after
            # the start of CRC area.  We'll search after collecting >= 50
            # bits.
            if len(bits) > 32:
                tail = bits[-7:]
                if all(v == 1 for v in tail):
                    break

        end_t = m2.t if m2 else (sof_t + bit_time * len(bits))

        # Mark SOF
        append(sof_t, sof_t + bit_time, CH_FIELD, text="SOF",
               sample_type=SAMPLE_PACKET_START, color=0xFFAAAAFF)

        # ----- Unstuff (remove stuffed bit after 5 same-value bits) --------
        unstuffed = []
        run = 0
        last = None
        for b in bits:
            if last is None:
                unstuffed.append(b)
                run = 1
                last = b
                continue
            if b == last:
                run += 1
                if run == 6:
                    # The 6th same-value bit is a stuff bit -- skip it.
                    run = 0
                    last = 1 - b   # next "different" bit ends the run
                    continue
                else:
                    unstuffed.append(b)
            else:
                unstuffed.append(b)
                run = 1
                last = b

        # ----- Parse SOF + ID -----------------------------------------------
        if len(unstuffed) < 19:
            append(end_t, end_t, CH_ERR, text="frame too short",
                   color=0xFFFF8888, sample_type=SAMPLE_DATA)
            continue

        # SOF is unstuffed[0] (already known dominant)
        # 11-bit ID is unstuffed[1..11]
        id_bits = unstuffed[1:12]
        std_id = 0
        for b in id_bits:
            std_id = (std_id << 1) | b
        rtr_or_srr = unstuffed[12]
        ide = unstuffed[13]
        rb0 = unstuffed[14]
        ext_id = None
        if ide == 1:
            # Extended frame -- 18 more ID bits + RTR + r1 + r0
            if len(unstuffed) < 39:
                append(end_t, end_t, CH_ERR, text="extended frame truncated",
                       color=0xFFFF8888, sample_type=SAMPLE_DATA)
                continue
            ext_bits = unstuffed[14:32]
            ext_id = 0
            for b in ext_bits:
                ext_id = (ext_id << 1) | b
            full_id = (std_id << 18) | ext_id
            rtr = unstuffed[32]
            r1  = unstuffed[33]
            r0  = unstuffed[34]
            ctrl_start = 35
        else:
            rtr = rtr_or_srr
            full_id = std_id
            ctrl_start = 15   # rb0 is at 14, dlc starts at 15

        dlc_bits = unstuffed[ctrl_start:ctrl_start + 4]
        if len(dlc_bits) < 4:
            append(end_t, end_t, CH_ERR, text="DLC truncated",
                   color=0xFFFF8888, sample_type=SAMPLE_DATA)
            continue
        dlc = (dlc_bits[0] << 3) | (dlc_bits[1] << 2) | \
              (dlc_bits[2] << 1) | dlc_bits[3]
        dlc_clamped = min(8, dlc)

        data_start = ctrl_start + 4
        data_bytes_bits = unstuffed[data_start:data_start + 8 * dlc_clamped]
        data_bytes = []
        for byte_i in range(dlc_clamped):
            v = 0
            for bit_i in range(8):
                idx = byte_i * 8 + bit_i
                if idx >= len(data_bytes_bits):
                    break
                v = (v << 1) | data_bytes_bits[idx]
            data_bytes.append(v)

        crc_start = data_start + 8 * dlc_clamped
        crc_bits = unstuffed[crc_start:crc_start + 15]
        if len(crc_bits) < 15:
            append(end_t, end_t, CH_ERR, text="CRC truncated",
                   color=0xFFFF8888, sample_type=SAMPLE_DATA)
            continue
        crc_rx = 0
        for b in crc_bits:
            crc_rx = (crc_rx << 1) | b
        crc_calc = _crc15(unstuffed[0:crc_start])
        crc_ok = (crc_rx == crc_calc)

        # ----- Emit annotations --------------------------------------------
        id_str = f"0x{full_id:08X}" if ide else f"0x{full_id:03X}"
        rtr_str = " RTR" if rtr else ""
        data_hex = " ".join(f"{b:02X}" for b in data_bytes)
        text = f"ID={id_str}{' EXT' if ide else ''}{rtr_str} DLC={dlc} D=[{data_hex}]"
        if not crc_ok:
            text += f" !CRC(got=0x{crc_rx:04X} want=0x{crc_calc:04X})"
        append(sof_t, end_t, CH_FRAME, text=text,
               data=full_id & 0xFFFFFFFF, sample_type=SAMPLE_DATA,
               color=(0xFF88FF88 if crc_ok else 0xFFFF8888))

        # Per-field rows (rough timestamps -- proportional within frame).
        def t_at(bit_idx):
            return sof_t + bit_idx * bit_time
        append(t_at(1), t_at(12), CH_FIELD,
               text=f"ID {id_str}", data=full_id & 0xFFFFFFFF,
               sample_type=SAMPLE_DATA, color=0xFFAAEEFF)
        append(t_at(ctrl_start), t_at(ctrl_start + 4), CH_FIELD,
               text=f"DLC={dlc}", data=dlc, sample_type=SAMPLE_DATA,
               color=0xFFFFCC88)
        if data_bytes:
            append(t_at(data_start), t_at(crc_start), CH_FIELD,
                   text=f"DATA [{data_hex}]", sample_type=SAMPLE_DATA)
        append(t_at(crc_start), t_at(crc_start + 15), CH_FIELD,
               text=f"CRC 0x{crc_rx:04X} {'OK' if crc_ok else 'FAIL'}",
               color=(0xFF88FF88 if crc_ok else 0xFFFF8888),
               sample_type=SAMPLE_DATA)
        append(end_t, end_t, CH_FRAME, text="EOF",
               sample_type=SAMPLE_PACKET_END)
