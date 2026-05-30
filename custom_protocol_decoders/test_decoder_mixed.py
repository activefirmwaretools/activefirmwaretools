# DECODER_MYNAME = "Qualified Digital Edge"
#
# Example decoder: finds digital edges qualified by an analog level condition (combines digital + analog via all_of).
#
# Typical uses:
#   - Count clock edges only while an enable line (analog) is asserted.
#   - Catch data transitions only when a supply rail sits in its valid window.
#   - Find switching events that happen during a thermistor alarm range.
#
# Output rows:
#   ch 0 -- "Q#n" tag at every qualified edge (running count).
#   ch 1 -- "V=x.xx" snapshot of the analog voltage at that edge.
#
# PARAM: digital_channel | Digital Channel      | digital_channel |              |
# PARAM: digital_edge    | Digital Edge         | select | rising,falling,any   | rising
# PARAM: analog_channel  | Analog Channel       | analog_channel  |              | 1
# PARAM: analog_cond     | Analog Condition     | select | above,below,between  | above
# PARAM: threshold       | Threshold (V)        | float  | -10,10               | 1.65
# PARAM: threshold_high  | Upper Threshold (V)  | float  | -10,10               | 3.30

from active_pro import (
    append, wait_for,
    rising_edge, falling_edge,
    voltage_above, voltage_below, voltage_between,
    any_of, all_of,
)


def decode(params):
    d_ch     = int(params.get("digital_channel", "0"))
    d_edge   = params.get("digital_edge", "rising")
    a_ch     = int(params.get("analog_channel", "1")) - 1   # UI is 1-based
    a_cond   = params.get("analog_cond", "above")
    thr      = float(params.get("threshold", "1.65"))
    thr_hi   = float(params.get("threshold_high", "3.30"))

    if d_edge == "falling":
        edge_cond = falling_edge(d_ch)
    elif d_edge == "any":
        edge_cond = any_of(rising_edge(d_ch), falling_edge(d_ch))
    else:
        edge_cond = rising_edge(d_ch)

    if a_cond == "below":
        level_cond = voltage_below(a_ch, thr)
    elif a_cond == "between":
        lo, hi = (thr, thr_hi) if thr <= thr_hi else (thr_hi, thr)
        level_cond = voltage_between(a_ch, lo, hi)
    else:
        level_cond = voltage_above(a_ch, thr)

    # all_of feeds every logic sub-sample through the edge state machine AND
    # tests the sample-and-hold analog value at that moment. Because _AllOf
    # runs every sub-condition (no short-circuit), the edge detector advances
    # even on samples where the analog level disqualifies the moment -- so we
    # never "double-fire" once the analog condition returns to true.
    qualified = all_of(edge_cond, level_cond)

    count = 0
    while True:
        m = yield from wait_for(qualified)
        if m is None:
            return
        count += 1
        append(m.t, m.t + 1e-6,
               channel=0,
               text=f"Q#{count}",
               data=count,
               sample_type=1,
               color=0xFF00FFAA)
        append(m.t, m.t + 1e-6,
               channel=1,
               text=f"V={m.a[a_ch]:.2f}",
               sample_type=2,
               color=0xFFFFAA00)
