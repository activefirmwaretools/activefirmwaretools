# DECODER_MYNAME = "DMX512"
#
# Decodes DMX512 lighting protocol on one digital channel (TTL/UART side, not the RS-485 wire).
# A capture on the differential bus itself should use the RS-485 decoder instead.
#
# PARAM: channel | Logic Channel | digital_channel |  |
# PARAM: invert  | Invert        | bool |  | false
# PARAM: max_slots | Max Slots to Decode | int | 1,512 | 32
#
# Output rows:
#   ch 0 : frame structure (BREAK, MAB, START CODE, slot count)
#   ch 1 : slot values (e.g. "ch001 = 255")
#
# Protocol notes:
#   - 250 kbaud, 8N2 framing (1 start bit, 8 data LSB-first, 2 stop bits).
#   - A "BREAK" is a low pulse >= 88 us (22 bit times) signaling a new packet.
#   - Then a Mark-After-Break (MAB) of >= 8 us idle high.
#   - Then a START CODE byte (usually 0x00 for "Null Start Code" / standard
#     dimmer data) followed by up to 512 slot bytes.
#   - The packet ends with another BREAK or simply end-of-capture.

import sys

from active_pro import (
    append, wait_for, wait_time,
    falling_edge, rising_edge,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_FRAME = 0
CH_SLOT  = 1

BAUD = 250000


def decode(params):
    ch        = int(params.get("channel", "0"))
    invert    = params.get("invert", "false").lower() == "true"
    max_slots = max(1, min(512, int(params.get("max_slots", "32"))))

    bit_time = 1.0 / BAUD   # 4 us
    break_min = 88e-6       # 22 bit times
    BREAK_LEVEL = 1 if invert else 0   # break = low (or high if inverted)

    print(f"DMX512: ch={ch} invert={invert} slots<={max_slots}",
          file=sys.stderr)

    while True:
        # ---- Find a BREAK (long low pulse) ---------------------------------
        # Edge that goes into the break level:
        into_break = falling_edge(ch) if not invert else rising_edge(ch)
        out_of_break = rising_edge(ch) if not invert else falling_edge(ch)

        f0 = yield from wait_for(into_break)
        if f0 is None:
            return
        r0 = yield from wait_for(out_of_break)
        if r0 is None:
            return
        if (r0.t - f0.t) < break_min:
            continue   # too short -- it was a normal start bit

        append(f0.t, r0.t, CH_FRAME, text="BREAK",
               sample_type=SAMPLE_PACKET_START, color=0xFFAAAAFF)

        # MAB starts now (line idle).  We'll just expect the first start
        # bit on the next falling edge.

        # ---- Read bytes until next BREAK or end-of-capture ----------------
        slot_index = -1   # -1 = next byte is the start code
        ended = False
        last_byte_end_t = r0.t
        while not ended:
            # Next falling edge is start of a byte's start bit; or it could
            # be the start of another BREAK if the upstream device pulled
            # the line low for >= break_min.  We detect that case by
            # measuring the start-bit's low time before sampling.
            sb = yield from wait_for(into_break)
            if sb is None:
                ended = True
                break

            # Probe whether this is a BREAK by waiting up to break_min for
            # a rising edge.
            sb_end = yield from wait_for(out_of_break, timeout=break_min * 1.2)
            if sb_end is not None and (sb_end.t - sb.t) >= break_min:
                # Another BREAK -- start of next packet.  Don't consume; let
                # the outer loop re-detect it cleanly.  We re-emit here as
                # a packet-end marker on this packet.
                append(last_byte_end_t, sb.t, CH_FRAME,
                       text=f"END ({slot_index + 1} slots)",
                       sample_type=SAMPLE_PACKET_END)
                # Loop back: outer while True iteration will see the BREAK.
                # We can simulate this by emitting an artificial packet now,
                # since sb / sb_end already capture the BREAK.
                append(sb.t, sb_end.t, CH_FRAME, text="BREAK",
                       sample_type=SAMPLE_PACKET_START, color=0xFFAAAAFF)
                slot_index = -1
                last_byte_end_t = sb_end.t
                continue
            elif sb_end is None:
                # Capture ended.
                ended = True
                break

            # Normal start bit -- sample 8 data bits, 1.5 bit_time to middle
            # of bit 0.
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
            byte_end_t = sb.t + 11 * bit_time   # 1 start + 8 data + 2 stop

            if slot_index == -1:
                append(sb.t, byte_end_t, CH_FRAME,
                       text=f"START CODE 0x{byte_val:02X}",
                       data=byte_val, sample_type=SAMPLE_DATA,
                       color=0xFFAAEEFF)
            else:
                if slot_index < max_slots:
                    append(sb.t, byte_end_t, CH_SLOT,
                           text=f"ch{slot_index + 1:03d} = {byte_val}",
                           data=(slot_index << 8) | byte_val,
                           sample_type=SAMPLE_DATA)
                elif slot_index == max_slots:
                    append(sb.t, byte_end_t, CH_SLOT,
                           text=f"(more slots truncated; max_slots={max_slots})",
                           sample_type=SAMPLE_DATA, color=0xFFCCCCCC)
            slot_index += 1
            last_byte_end_t = byte_end_t

            # Wait a brief moment to clear the stop bits before looking for
            # the next start bit.  Stop bits are >= 2 bit times.
            mq = yield from wait_time(2 * bit_time)
            if mq is None:
                ended = True
                break
