# DECODER_MYNAME = "WS2812 / NeoPixel"
#
# Decodes WS2812 / WS2812B / SK6812 single-wire pulse-coded RGB pixels.
#
# PARAM: channel  | Logic Channel  | digital_channel |  |
# PARAM: order    | Color Order    | select | GRB,RGB,GRBW | GRB
# PARAM: bit_rate | Bit Rate       | select | 800kHz,400kHz | 800kHz
#
# Output rows:
#   ch 0 : pixel index + decoded color (e.g. "#005 RGB(255, 64, 0)")
#   ch 1 : frame boundary marker
#
# Protocol notes:
#   - One bit takes ~1.25 us (800 kHz).  Each bit is a fixed-period pulse
#     with the duty ratio encoding the value:
#       0-bit: high ~0.35 us, low ~0.90 us  (high  < ~0.6 us = 0)
#       1-bit: high ~0.90 us, low ~0.35 us  (high >= ~0.6 us = 1)
#   - A reset / latch is signaled by holding the line low for >= 50 us
#     (WS2812B) or >= 280 us (older WS2812).  We use 50 us.
#   - Pixels are streamed MSB first: GRB, RGB, or GRBW (4 bytes per pixel
#     for RGBW variants like SK6812).
#   - SAMPLE_PACKET_START / SAMPLE_PACKET_END bracket each frame
#     (start = first bit after a reset, end = next reset).

import sys

from active_pro import (
    append, wait_for,
    rising_edge, falling_edge,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_PIXEL = 0
CH_FRAME = 1


def decode(params):
    ch       = int(params.get("channel", "0"))
    order    = params.get("order", "GRB")
    bit_rate = params.get("bit_rate", "800kHz")

    bytes_per_pixel = 4 if order == "GRBW" else 3
    if bit_rate == "400kHz":
        # 400 kHz: bit period ~2.5 us. 0-bit high ~0.5 us, 1-bit high ~1.2 us.
        # Threshold at ~0.85 us.
        ONE_THRESH = 0.85e-6
        RESET_MIN  = 50e-6
    else:
        ONE_THRESH = 0.6e-6
        RESET_MIN  = 50e-6

    print(f"WS2812: ch={ch} order={order} rate={bit_rate}", file=sys.stderr)

    frame_open = False
    pixel_index = 0
    byte_accum = 0
    bits_in_byte = 0
    bytes_for_pixel = []
    pixel_start_t = None

    while True:
        rise = yield from wait_for(rising_edge(ch))
        if rise is None:
            return

        fall = yield from wait_for(falling_edge(ch))
        if fall is None:
            return

        high_dur = fall.t - rise.t
        bit_val = 1 if high_dur >= ONE_THRESH else 0

        if not frame_open:
            append(rise.t, rise.t, CH_FRAME, text="FRAME",
                   color=0xFFAAAAFF, sample_type=SAMPLE_PACKET_START)
            frame_open = True
            pixel_index = 0
            byte_accum = 0
            bits_in_byte = 0
            bytes_for_pixel = []
            pixel_start_t = rise.t

        byte_accum = (byte_accum << 1) | bit_val
        bits_in_byte += 1
        if bits_in_byte == 8:
            bytes_for_pixel.append(byte_accum & 0xFF)
            byte_accum = 0
            bits_in_byte = 0
            if len(bytes_for_pixel) == bytes_per_pixel:
                # Order to RGB(W)
                if order == "GRB":
                    g, r, b = bytes_for_pixel
                    text = f"#{pixel_index:03d} RGB({r}, {g}, {b})"
                    data = (r << 16) | (g << 8) | b
                elif order == "RGB":
                    r, g, b = bytes_for_pixel
                    text = f"#{pixel_index:03d} RGB({r}, {g}, {b})"
                    data = (r << 16) | (g << 8) | b
                else:  # GRBW
                    g, r, b, w = bytes_for_pixel
                    text = f"#{pixel_index:03d} RGBW({r}, {g}, {b}, {w})"
                    data = (r << 16) | (g << 8) | b
                # Color the annotation roughly the pixel color for visual punch.
                # If the pixel is very dim, fall back to white-ish.
                if order != "GRBW":
                    pr, pg, pb = (r, g, b)
                else:
                    pr, pg, pb = (r, g, b)
                if max(pr, pg, pb) < 16:
                    color = 0xFFAAAAAA
                else:
                    color = 0xFF000000 | (pr << 16) | (pg << 8) | pb
                append(pixel_start_t, fall.t, CH_PIXEL,
                       text=text, data=data,
                       color=color, sample_type=SAMPLE_DATA)
                pixel_index += 1
                bytes_for_pixel = []
                pixel_start_t = None

            if pixel_start_t is None and bits_in_byte == 0:
                # Next pixel starts at the next rising edge -- we'll grab
                # the timestamp from there.
                pass

        # Check for reset: peek the gap to the next rising edge.  We can't
        # easily look ahead, so we detect reset reactively -- the next
        # wait_for(rising_edge) above can return None or a much later edge.
        # Instead, look at the duration of the LOW phase by waiting a
        # bounded amount of time first.
        # (We approximate: if more than RESET_MIN passes before the next
        # rising edge we treat it as end-of-frame.  Implemented via timeout.)
        next_rise = yield from wait_for(rising_edge(ch), timeout=RESET_MIN)
        if next_rise is None:
            # Reset boundary
            if frame_open:
                append(fall.t, fall.t + RESET_MIN, CH_FRAME, text="RESET",
                       color=0xFFCCCCFF, sample_type=SAMPLE_PACKET_END)
                frame_open = False
            # Keep looping; next outer iteration will catch the next rising.
            continue
        # We consumed the next rising; process it as the start of the next bit.
        rise2 = next_rise
        fall2 = yield from wait_for(falling_edge(ch))
        if fall2 is None:
            return
        high_dur = fall2.t - rise2.t
        bit_val = 1 if high_dur >= ONE_THRESH else 0
        if pixel_start_t is None:
            pixel_start_t = rise2.t
        byte_accum = (byte_accum << 1) | bit_val
        bits_in_byte += 1
        if bits_in_byte == 8:
            bytes_for_pixel.append(byte_accum & 0xFF)
            byte_accum = 0
            bits_in_byte = 0
            if len(bytes_for_pixel) == bytes_per_pixel:
                if order == "GRB":
                    g, r, b = bytes_for_pixel
                    text = f"#{pixel_index:03d} RGB({r}, {g}, {b})"
                    data = (r << 16) | (g << 8) | b
                elif order == "RGB":
                    r, g, b = bytes_for_pixel
                    text = f"#{pixel_index:03d} RGB({r}, {g}, {b})"
                    data = (r << 16) | (g << 8) | b
                else:
                    g, r, b, w = bytes_for_pixel
                    text = f"#{pixel_index:03d} RGBW({r}, {g}, {b}, {w})"
                    data = (r << 16) | (g << 8) | b
                pr, pg, pb = (r, g, b)
                color = 0xFFAAAAAA if max(pr, pg, pb) < 16 else \
                        0xFF000000 | (pr << 16) | (pg << 8) | pb
                append(pixel_start_t, fall2.t, CH_PIXEL,
                       text=text, data=data,
                       color=color, sample_type=SAMPLE_DATA)
                pixel_index += 1
                bytes_for_pixel = []
                pixel_start_t = None
