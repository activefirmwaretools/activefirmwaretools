# DECODER_MYNAME = "My Decoder"
#
# Replace this line with a one-sentence description of what your decoder does.
# (The first non-PARAM comment line above the code shows in the picker's
# Description column. Multi-line descriptions get truncated -- keep it short.)
#
# PARAMETER LINES (optional, one per line, all in this top comment block):
#   Format:  # PARAM: <key> | <Label> | <type> | <options> | <default>
#   Types:
#     digital_channel -- spinbox 0..7. Names a digital channel; the host
#                        forces that channel ON when capture starts and
#                        warns if it collides with another decoder.
#     analog_channel  -- spinbox 1..8 (Pro: 1..4 valid; Ultra: 1..8).
#                        Names an analog channel; same auto-on + collision
#                        behavior as digital_channel. Decoder script reads
#                        the param value and subtracts 1 to index m.a[].
#     select  -- dropdown.   options = "opt1,opt2,opt3"   default = one of them
#     int     -- spinbox.    options = "min,max" (or "" for unconstrained)
#     float   -- spinbox.    options = "min,max" (or "")
#     bool    -- checkbox.   options = ""                  default = true|false
#     string  -- text field. options = ""                  default = any text
#   Read the user's value at runtime via params["<key>"] (always a string).
#
# Sample PARAM lines below -- these ARE active as-written. Delete the ones
# you don't need; edit the rest.
# PARAM: digital_ch | Digital Trigger Ch  | digital_channel |                                       |
# PARAM: analog_ch  | Analog Probe Ch     | analog_channel  |                                       | 1
# PARAM: baud_rate  | Baud Rate           | select          | 9600,19200,38400,57600,115200,230400  | 115200
# PARAM: threshold  | Threshold (V)       | float           | 0.0,5.0                               | 2.5
# PARAM: invert     | Invert Digital      | bool            |                                       | false
#
# ===========================================================================
# API reference (safe to delete from your own decoders).
# ===========================================================================
#
# The decoder has ONE notion of "current time". Every wait operation
# advances time. There are exactly two wait operations:
#
#   m = yield from wait_for(condition)   # advance until condition is true
#   m = yield from wait_time(seconds)    # advance by a fixed amount of time
#
# Both return a Moment -- a snapshot of all signals at the new current time:
#   m.t        -- timestamp in seconds (float)
#   m.d[0..7]  -- digital channel bits (tuple of 0/1)
#   m.bits     -- digital channels packed as a byte
#   m.a[0..7]  -- analog channel voltages (list of float)
# Both return None on end-of-capture or timeout -- always check before using.
#
# Build conditions from these factories:
#   rising_edge(channel)                                  -- digital rising
#   falling_edge(channel)                                 -- digital falling
#   high(channel)                                         -- digital == 1
#   low(channel)                                          -- digital == 0
#   voltage_rises(channel, threshold, hysteresis=0.0)     -- analog rising
#   voltage_falls(channel, threshold, hysteresis=0.0)     -- analog falling
#   voltage_above(channel, threshold)                     -- analog level above
#   voltage_below(channel, threshold)                     -- analog level below
#   voltage_between(channel, low, high)                   -- analog window
#
# Combine conditions:
#   all_of(c1, c2, ...)   -- AND  (every sub-condition true at the same moment)
#   any_of(c1, c2, ...)   -- OR
#   not_(c)               -- NOT
#
# Channels:
#   digital: 0..7 (logic bit)
#   analog:  0..7 (Pro: 0..3 valid; Ultra: 0..7)
#
# Output (each decoder slot gets 8 output channels):
#   append(t_start, t_end, channel, text, color=0, sample_type=0, data=0)
#     channel:      0-based output row index
#     color:        ARGB int, 0 = use theme color
#     sample_type:  byte type marker controlling PacketPresenter framing:
#         SAMPLE_DATA         (0) -- normal data byte
#         SAMPLE_DATA_ALT     (1) -- alternate data channel
#         SAMPLE_PACKET_START (2) -- tells PP: a new packet starts here
#         SAMPLE_PACKET_END   (3) -- tells PP: this packet ends here
#     data:         u32 payload for PP scripts

from active_pro import (
    append,
    wait_for, wait_time,
    rising_edge, falling_edge, high, low,
    voltage_rises, voltage_falls,
    voltage_above, voltage_below, voltage_between,
    all_of, any_of, not_,
)


# sample_type values understood by PacketPresenter (defines.h constants).
SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

# Output channel layout for this template.
CH_HEX     = 0   # decoded byte as hex (e.g. "0x41")
CH_ASCII   = 1   # printable ASCII rendering
CH_VOLTAGE = 2   # analog voltage at the start of the byte


def decode(params):
    """
    Demonstrates every condition factory and combinator, ending with a
    realistic UART-style frame loop. Delete sections you don't need.

    params is a dict of strings (one per PARAM line). Cast with int(),
    float(), or compare == "true" for bools.
    """
    digital_ch = int(params.get("digital_ch", "0"))
    analog_ch  = int(params.get("analog_ch", "1")) - 1   # analog_channel is 1-indexed in PARAM
    baud_rate  = int(params.get("baud_rate", "115200"))
    threshold  = float(params.get("threshold", "2.5"))
    invert     = params.get("invert", "false").lower() == "true"

    bit_time = 1.0 / baud_rate

    # ----- 1. Digital edge ---------------------------------------------------
    m = yield from wait_for(falling_edge(digital_ch))
    if m is None:
        return
    append(m.t, m.t, CH_HEX, text="[1: falling edge]")

    # ----- 2. Analog edge with hysteresis ------------------------------------
    # Schmitt-trigger style: voltage must dip below (threshold - 0.1) before
    # the next rising edge can fire.
    m = yield from wait_for(voltage_rises(analog_ch, threshold,
                                          hysteresis=0.1))
    if m is None:
        return
    append(m.t, m.t, CH_VOLTAGE,
           text=f"[2: analog rise at {m.a[analog_ch]:.2f} V]",
           color=0xFFAACCFF)

    # ----- 3. Levels (no transition required) --------------------------------
    m = yield from wait_for(high(digital_ch))
    if m is None: return

    m = yield from wait_for(voltage_above(analog_ch, threshold))
    if m is None: return

    # ----- 4. Compound: digital edge AND analog above threshold --------------
    # "Trigger only on a falling edge while the supply rail is good."
    m = yield from wait_for(all_of(
        falling_edge(digital_ch),
        voltage_above(analog_ch, threshold),
    ))
    if m is None:
        return
    append(m.t, m.t, CH_HEX,
           text="[4: digital edge + analog OK]", color=0xFFFFAA00)

    # ----- 5. Compound: analog edge gated by digital level -------------------
    # "Analog rail crossed 2.5 V upward WHILE chip-select was asserted."
    m = yield from wait_for(all_of(
        voltage_rises(analog_ch, threshold, hysteresis=0.1),
        high(digital_ch),
    ))
    if m is None:
        return
    append(m.t, m.t, CH_VOLTAGE,
           text="[5: analog rise + digital HIGH]", color=0xFFFFAA88)

    # ----- 6. OR / NOT combinators -------------------------------------------
    # Falling edge AND (analog above 4.5 V OR a different channel below 0.3 V),
    # AND digital channel 7 is NOT high.
    m = yield from wait_for(all_of(
        falling_edge(digital_ch),
        any_of(
            voltage_above(0, 4.5),
            voltage_below(1, 0.3),
        ),
        not_(high(7)),
    ))
    if m is None:
        return

    # ----- 7. Fixed time advance ---------------------------------------------
    m = yield from wait_time(0.001)
    if m is None: return

    # ----- 8. Realistic UART-style frame loop --------------------------------
    # Each start bit is qualified by "analog rail above threshold" so glitches
    # during brown-out don't decode. Three output channels per byte plus
    # PacketPresenter framing markers around each byte.
    while True:
        # Falling start bit, gated by supply being good.
        start = yield from wait_for(all_of(
            falling_edge(digital_ch),
            voltage_above(analog_ch, threshold),
        ))
        if start is None:
            return

        # PP packet start.
        append(start.t, start.t, CH_HEX,
               text="", sample_type=SAMPLE_PACKET_START)

        # 8 data bits at bit_time spacing.
        byte_val = 0
        ok = True
        for bit in range(8):
            m = yield from wait_time(bit_time)
            if m is None:
                ok = False
                break
            data_bit = m.d[digital_ch] ^ (1 if invert else 0)
            byte_val |= (data_bit << bit)
        if not ok:
            return

        end = m

        # Three parallel output rows for the same byte.
        append(start.t, end.t, CH_HEX,
               text=f"0x{byte_val:02X}",
               sample_type=SAMPLE_DATA, data=byte_val)

        ch_char = chr(byte_val) if 32 <= byte_val < 127 else "."
        append(start.t, end.t, CH_ASCII,
               text=ch_char, color=0xFF88FF88,
               sample_type=SAMPLE_DATA_ALT, data=byte_val)

        mv = int(start.a[analog_ch] * 1000)
        append(start.t, end.t, CH_VOLTAGE,
               text=f"{mv} mV", color=0xFFAACCFF)

        # PP packet end.
        append(end.t, end.t, CH_HEX,
               text="", sample_type=SAMPLE_PACKET_END)

        # Skip past the stop bit before looking for the next start bit.
        m = yield from wait_time(bit_time)
        if m is None:
            return
