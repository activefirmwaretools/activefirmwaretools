# DECODER_MYNAME = "Analog Edge Counter"
#
# Example/test decoder: counts rising threshold crossings on a configurable analog channel ("A#1", "A#2", ...).
#
# A crossing fires when the channel voltage goes from below `threshold` to
# above `threshold`. Optional `hysteresis` requires the voltage to dip
# (threshold - hysteresis) before re-arming, so it won't double-count noise
# riding the threshold line.
#
# PARAM: channel    | Analog Channel | analog_channel |  | 1
# PARAM: threshold  | Threshold (V)  | float | -10,10 | 1.65
# PARAM: hysteresis | Hysteresis (V) | float | 0,5   | 0.1

from active_pro import append, wait_for, voltage_rises


def decode(params):
    # Analog channels are labelled 1..N in the UI (CH1..CHN); convert to the
    # API's 0-based index.
    ch         = int(params.get("channel", "1")) - 1
    threshold  = float(params.get("threshold", "1.65"))
    hysteresis = float(params.get("hysteresis", "0.1"))
    count = 0
    while True:
        m = yield from wait_for(voltage_rises(ch, threshold, hysteresis))
        if m is None:
            return
        count += 1
        append(m.t, m.t + 1e-6,
               channel=0,
               text=f"A#{count}",
               data=count,
               sample_type=1,
               color=0xFF00AAFF)
