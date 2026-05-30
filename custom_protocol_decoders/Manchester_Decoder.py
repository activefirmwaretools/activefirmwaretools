# DECODER_MYNAME = "Manchester"
#
# Decodes Manchester-encoded data on one digital channel using midpoint-edge sampling.
#
# PARAM: channel  | Logic Channel   | digital_channel |  |
# PARAM: bit_rate | Bit Rate (bps)  | int    |       | 10000
# PARAM: polarity | Convention      | select | thomas,ieee | thomas
# PARAM: bits_per_word | Word Size  | int    |       | 8
# PARAM: order    | Bit Order       | select | msb,lsb | msb
#
# Output rows:
#   ch 0 : decoded word (hex)
#
# Conventions:
#   G.E. Thomas:  '0' = low-to-high mid-bit edge, '1' = high-to-low
#   IEEE 802.3:   '0' = high-to-low mid-bit edge, '1' = low-to-high
#
# Method:
#   Each bit has a guaranteed mid-bit transition.  Bit-boundary transitions
#   may or may not occur depending on adjacent bit values.  We sync on the
#   first edge, sample 0.75 of a bit period later (the mid-bit transition),
#   then continue at 1 bit period per step.

import sys

from active_pro import (
    append, wait_for, wait_time,
    rising_edge, falling_edge, any_of,
)


SAMPLE_DATA         = 0
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_WORD = 0


def decode(params):
    ch       = int(params.get("channel", "0"))
    bit_rate = int(params.get("bit_rate", "10000"))
    polarity = params.get("polarity", "thomas")
    word_bits = max(1, int(params.get("bits_per_word", "8")))
    order    = params.get("order", "msb")

    bit_time = 1.0 / bit_rate
    half_bit = bit_time / 2.0
    quarter  = bit_time / 4.0

    if polarity == "ieee":
        # IEEE 802.3: '0' is high-then-low mid-bit edge (falling at midpoint).
        # So a falling mid-bit edge means '0'; rising mid-bit edge means '1'.
        falling_bit, rising_bit = 0, 1
    else:
        # G.E. Thomas: '0' is low-then-high (rising at midpoint).
        falling_bit, rising_bit = 1, 0

    print(f"Manchester: ch={ch} {bit_rate} bps {polarity} "
          f"{word_bits}-bit {order}-first", file=sys.stderr)

    # Find first edge as sync; sample bit cells at bit_time offsets.
    edge = yield from wait_for(any_of(rising_edge(ch), falling_edge(ch)))
    if edge is None:
        return

    # We expect this first edge to be a mid-bit transition.  The first
    # bit's value is determined by its direction.
    if edge.d[ch] == 1:
        first_bit = rising_bit
    else:
        first_bit = falling_bit

    bits = [first_bit]
    word_start_t = edge.t
    # Walk forward in bit-time steps, picking out the mid-bit transition
    # of each cell.  At each step we wait `bit_time` then sample the
    # signal at +/- quarter-bit windows to classify the mid-bit edge.
    while True:
        # Advance to roughly the next mid-bit transition.  Look for an
        # edge within one bit time; if we see none, the signal stopped.
        m = yield from wait_for(
            any_of(rising_edge(ch), falling_edge(ch)),
            timeout=bit_time * 1.5)
        if m is None:
            # End of message; emit partial word if any.
            if len(bits) > 0:
                _emit_word(bits, word_start_t, edge.t, word_bits, order)
            return

        # Was this edge a mid-bit or a bit-boundary transition?  Bit-
        # boundary edges occur exactly between bit cells and indicate
        # repeated same-value bits (one mid-bit edge per cell).  We use
        # the time since the last sampled edge: if it's ~bit_time, it was
        # a mid-bit transition (one cell); if it's ~half_bit, it's a
        # bit-boundary transition between two cells with different mid-
        # bit directions.
        dt = m.t - edge.t
        if dt < bit_time - quarter:
            # Bit-boundary transition -- skip it (don't sample as new bit).
            edge = m
            continue

        # mid-bit transition: classify by direction.
        bit_val = rising_bit if m.d[ch] == 1 else falling_bit
        bits.append(bit_val)
        edge = m

        if len(bits) == word_bits:
            _emit_word(bits, word_start_t, m.t, word_bits, order)
            bits = []
            word_start_t = m.t


def _emit_word(bits, t_start, t_end, word_bits, order):
    val = 0
    if order == "lsb":
        for i, b in enumerate(bits):
            val |= (b & 1) << i
    else:  # msb first
        for b in bits:
            val = (val << 1) | (b & 1)
    nibbles = (word_bits + 3) // 4
    text = f"0x{val:0{nibbles}X}"
    if len(bits) < word_bits:
        text += f" ({len(bits)}b partial)"
    append(t_start, t_end, CH_WORD, text=text,
           data=val & 0xFFFFFFFF, sample_type=SAMPLE_DATA)
