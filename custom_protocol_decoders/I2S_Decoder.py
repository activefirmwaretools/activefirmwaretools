# DECODER_MYNAME = "I2S"
#
# Decodes I2S digital audio on three digital channels: BCLK, LRCLK, and SD.
#
# PARAM: bclk_channel  | BCLK Channel        | digital_channel |  |
# PARAM: lrclk_channel | LRCLK / WS Channel  | digital_channel |  |
# PARAM: sd_channel    | SD / DATA Channel   | digital_channel |  |
# PARAM: bits_per_word | Bits per Word       | select | 8,16,24,32 | 16
# PARAM: format        | Frame Format        | select | I2S,LeftJustified,RightJustified | I2S
# PARAM: signed        | Treat as Signed     | bool   |       | true
#
# Output rows:
#   ch 0 : Left  channel sample (hex + signed decimal)
#   ch 1 : Right channel sample (hex + signed decimal)
#
# Protocol notes:
#   - BCLK runs continuously. Data is sampled on the BCLK rising edge.
#   - LRCLK (Word Select) selects channel:
#       I2S format:           LRCLK=0 -> Left,  LRCLK=1 -> Right.  Data bit 0
#                             of a word starts ONE BCLK *after* the LRCLK edge.
#       LeftJustified format: data starts on the same BCLK edge as the LRCLK
#                             transition (no 1-cycle delay).
#       RightJustified:       data aligned to the trailing edge of the LRCLK
#                             period -- we still sample on rising BCLK and
#                             take the LAST `bits_per_word` bits before the
#                             next LRCLK transition.
#   - Words are MSB first.  We accumulate bits left-to-right.

import sys

from active_pro import (
    append, wait_for,
    rising_edge, falling_edge, any_of,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_LEFT  = 0
CH_RIGHT = 1


def decode(params):
    bclk    = int(params.get("bclk_channel", "0"))
    lrclk   = int(params.get("lrclk_channel", "1"))
    sd      = int(params.get("sd_channel", "2"))
    bpw     = int(params.get("bits_per_word", "16"))
    fmt     = params.get("format", "I2S")
    signed  = params.get("signed", "true").lower() == "true"

    print(f"I2S: BCLK={bclk} LRCLK={lrclk} SD={sd} "
          f"{bpw}-bit {fmt} signed={signed}", file=sys.stderr)

    sign_bit = 1 << (bpw - 1)
    sign_mask = (1 << bpw) - 1

    # Track LRCLK transitions to define word boundaries.  Each transition
    # marks the start of a new word; the previous word's channel is
    # whatever LRCLK was BEFORE the transition (or, for I2S, the bit that
    # was just released).
    ws_change = any_of(rising_edge(lrclk), falling_edge(lrclk))

    # Find the first LRCLK transition to sync.
    edge = yield from wait_for(ws_change)
    if edge is None:
        return

    while True:
        # Word "channel" for I2S: bits captured after a LRCLK transition
        # belong to the OPPOSITE side from the new LRCLK level (I2S has a
        # 1-bit delay).  For LeftJustified the word matches the new level.
        new_ws = edge.d[lrclk]
        if fmt == "I2S":
            this_side_left = (new_ws == 1)   # opposite of new level
        else:
            this_side_left = (new_ws == 0)   # left when LRCLK=0

        # Skip the alignment bit for I2S only (data starts on second BCLK).
        if fmt == "I2S":
            e0 = yield from wait_for(rising_edge(bclk))
            if e0 is None:
                return
            # Note: do not capture the bit on this edge -- it's the leftover
            # last bit of the previous frame's other channel.

        # Collect bits_per_word bits, MSB first, sampling on BCLK rising.
        word_val = 0
        word_start_t = None
        bits_seen = 0

        while bits_seen < bpw:
            ev = yield from wait_for(any_of(rising_edge(bclk), ws_change))
            if ev is None:
                return
            # If LRCLK changed mid-word, the device truncated this word
            # (or our frame format is wrong).  Bail out and resync.
            if ev.d[lrclk] != new_ws:
                edge = ev
                break

            bit = ev.d[sd]
            if word_start_t is None:
                word_start_t = ev.t
            word_val = (word_val << 1) | bit
            bits_seen += 1
            word_end_t = ev.t
        else:
            # Loop ran to completion -- we have a full word.
            if signed and (word_val & sign_bit):
                signed_val = word_val - (1 << bpw)
            else:
                signed_val = word_val
            text = f"0x{word_val & sign_mask:0{(bpw + 3)//4}X}  ({signed_val})"
            ch_out = CH_LEFT if this_side_left else CH_RIGHT
            append(word_start_t, word_end_t, ch_out,
                   text=text, data=word_val & 0xFFFFFFFF,
                   sample_type=SAMPLE_DATA)

            # Now wait for the next LRCLK transition to begin the next word.
            edge = yield from wait_for(ws_change)
            if edge is None:
                return
            continue

        # Fell out of the inner loop because LRCLK changed early; ev is
        # the new LRCLK edge -- loop continues with it.
