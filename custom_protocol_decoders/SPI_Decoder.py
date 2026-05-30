# DECODER_MYNAME = "SPI"
#
# Decodes 4-wire SPI traffic (CS / SCK / MOSI / MISO) on four digital channels.
#
# PARAM: clk_channel   | SCK Channel    | digital_channel |  |
# PARAM: mosi_channel  | MOSI Channel   | digital_channel |  |
# PARAM: miso_channel  | MISO Channel   | digital_channel |  |
# PARAM: cs_channel    | CS Channel     | digital_channel |  |
# PARAM: cpol          | Clock Polarity | select | 0,1 | 0
# PARAM: cpha          | Clock Phase    | select | 0,1 | 0
# PARAM: bits_per_word | Bits per Word  | int    |     | 8
#
# Output rows (within this decoder's Device slot):
#   ch 0 : MOSI word (e.g. "0xA5")
#   ch 1 : MISO word
#
# Protocol notes:
#   - CS is assumed active-low. A transaction begins on CS falling and
#     ends on CS rising.
#   - SPI mode is set by CPOL and CPHA:
#         CPOL=0 : SCK idles low.   CPOL=1 : SCK idles high.
#         CPHA=0 : sample on the leading edge of SCK.
#         CPHA=1 : sample on the trailing edge of SCK.
#     "Leading" means the first transition out of idle; "trailing" means
#     the return to idle. The table:
#         CPOL=0 CPHA=0 : sample on rising  (mode 0)
#         CPOL=0 CPHA=1 : sample on falling (mode 1)
#         CPOL=1 CPHA=0 : sample on falling (mode 2)
#         CPOL=1 CPHA=1 : sample on rising  (mode 3)
#   - Data is MSB-first (the most common SPI convention). To handle LSB-
#     first devices, swap the bit-accumulation expression below.
#   - Each chip-select assertion is wrapped in SAMPLE_PACKET_START /
#     SAMPLE_PACKET_END markers so an attached PacketPresenter sees one
#     "packet" per CS transaction.

import sys

from active_pro import (
    append,
    wait_for,
    rising_edge, falling_edge, low, high,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_MOSI = 0
CH_MISO = 1


def decode(params):
    sck  = int(params.get("clk_channel",   "0"))
    mosi = int(params.get("mosi_channel",  "1"))
    miso = int(params.get("miso_channel",  "2"))
    cs   = int(params.get("cs_channel",    "3"))
    cpol = int(params.get("cpol",          "0"))
    cpha = int(params.get("cpha",          "0"))
    word_bits = int(params.get("bits_per_word", "8"))

    # Which SCK edge to sample on -- see table in the header comment.
    sample_on_rising = (cpol ^ cpha) == 0
    sample_edge = rising_edge(sck) if sample_on_rising else falling_edge(sck)

    mode = (cpol << 1) | cpha
    print(f"SPI decoder: mode {mode} (CPOL={cpol} CPHA={cpha}), "
          f"{word_bits} bits/word, "
          f"SCK=ch{sck} MOSI=ch{mosi} MISO=ch{miso} CS=ch{cs}",
          file=sys.stderr)

    while True:
        # ----- Wait for CS to assert (falling edge) -------------------------
        cs_low = yield from wait_for(falling_edge(cs))
        if cs_low is None:
            return

        append(cs_low.t, cs_low.t, CH_MOSI,
               text="CS↓", sample_type=SAMPLE_PACKET_START)

        # ----- Read words until CS deasserts --------------------------------
        # Approach: between each sampled bit we also need to detect CS
        # rising. We do this by sampling SCK edges and bailing out if we
        # observe high(cs) on the resulting Moment. (CS rising mid-bit
        # is unusual but possible during aborts; we handle it cleanly.)
        word_index = 0
        mosi_val = 0
        miso_val = 0
        word_start_t = None
        bits_collected = 0

        while True:
            edge = yield from wait_for(sample_edge)
            if edge is None:
                return

            # Detect a CS-rising abort: if CS went high before this SCK
            # edge, this edge will see d[cs] == 1.
            if edge.d[cs] == 1:
                break

            if bits_collected == 0:
                word_start_t = edge.t

            mosi_bit = edge.d[mosi]
            miso_bit = edge.d[miso]

            # MSB first.
            mosi_val = (mosi_val << 1) | mosi_bit
            miso_val = (miso_val << 1) | miso_bit
            bits_collected += 1

            if bits_collected >= word_bits:
                fmt = f"0x{{:0{(word_bits + 3) // 4}X}}"
                append(word_start_t, edge.t, CH_MOSI,
                       text=fmt.format(mosi_val),
                       sample_type=SAMPLE_DATA, data=mosi_val)
                append(word_start_t, edge.t, CH_MISO,
                       text=fmt.format(miso_val),
                       sample_type=SAMPLE_DATA_ALT, data=miso_val)
                word_index += 1
                mosi_val = 0
                miso_val = 0
                bits_collected = 0
                word_start_t = None

        # ----- Wait for the actual CS rising edge to close the packet ------
        # (We may have already observed d[cs]==1 above, but the rising-edge
        # condition wants an edge transition. If CS is already high, this
        # returns None / no-op past the boundary -- in practice the loop
        # above only breaks when CS is observed high, so the explicit edge
        # is just for the closing timestamp.)
        cs_hi = yield from wait_for(rising_edge(cs))
        if cs_hi is None:
            # Capture ended right at CS deassert -- still emit the close
            # marker at our last known time.
            append(edge.t, edge.t, CH_MOSI,
                   text="CS↑", sample_type=SAMPLE_PACKET_END)
            return

        append(cs_hi.t, cs_hi.t, CH_MOSI,
               text="CS↑", sample_type=SAMPLE_PACKET_END)

        # Flag a partial trailing word, if any, so it doesn't get silently
        # dropped. (E.g. abort after 5 of 8 bits.)
        if bits_collected > 0:
            mosi_text = f"0x{mosi_val:X} (partial {bits_collected}b)"
            miso_text = f"0x{miso_val:X} (partial {bits_collected}b)"
            append(word_start_t or cs_hi.t, cs_hi.t, CH_MOSI,
                   text=mosi_text, sample_type=SAMPLE_DATA, data=mosi_val)
            append(word_start_t or cs_hi.t, cs_hi.t, CH_MISO,
                   text=miso_text, sample_type=SAMPLE_DATA_ALT, data=miso_val)
