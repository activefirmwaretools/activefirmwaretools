# DECODER_MYNAME = "PWM"
#
# Measures per-cycle PWM duty cycle (%) and frequency (Hz) on one digital channel.
#
# PARAM: channel        | Logic Channel    | digital_channel |  |
# PARAM: active_level   | Active Level     | select | high,low | high
# PARAM: report         | Report Style     | select | duty_pct,duty_us,freq_only,all | all
#
# Output rows:
#   ch 0 : per-cycle annotation (e.g. "50.0% @ 1.00 kHz")
#
# Protocol notes:
#   - A "cycle" is measured between two consecutive same-direction edges
#     (e.g. rising-to-rising when active=high).
#   - Duty is (active_time / period) * 100.
#   - First incomplete cycle is skipped.

import sys

from active_pro import (
    append, wait_for,
    rising_edge, falling_edge,
)


SAMPLE_DATA = 0

CH_OUT = 0


def decode(params):
    ch       = int(params.get("channel", "0"))
    active   = params.get("active_level", "high")
    report   = params.get("report", "all")

    if active == "low":
        start_edge = falling_edge(ch)
        end_edge   = rising_edge(ch)
    else:
        start_edge = rising_edge(ch)
        end_edge   = falling_edge(ch)

    print(f"PWM: ch={ch} active={active} report={report}", file=sys.stderr)

    # Sync: find the first leading edge to set t0.
    e0 = yield from wait_for(start_edge)
    if e0 is None:
        return

    while True:
        # End of active phase.
        eoff = yield from wait_for(end_edge)
        if eoff is None:
            return
        # Next cycle start.
        e1 = yield from wait_for(start_edge)
        if e1 is None:
            return

        period   = e1.t - e0.t
        active_t = eoff.t - e0.t
        if period <= 0.0:
            e0 = e1
            continue

        duty_pct = 100.0 * active_t / period
        freq_hz  = 1.0 / period

        if report == "duty_pct":
            text = f"{duty_pct:5.1f}%"
        elif report == "duty_us":
            text = f"{active_t * 1e6:.2f} us / {period * 1e6:.2f} us"
        elif report == "freq_only":
            text = _fmt_freq(freq_hz)
        else:
            text = f"{duty_pct:5.1f}% @ {_fmt_freq(freq_hz)}"

        append(e0.t, e1.t, CH_OUT, text=text,
               data=int(round(duty_pct * 10)),
               sample_type=SAMPLE_DATA, color=0xFFFFCC88)

        e0 = e1


def _fmt_freq(hz):
    if hz >= 1e6:
        return f"{hz / 1e6:.3f} MHz"
    if hz >= 1e3:
        return f"{hz / 1e3:.3f} kHz"
    return f"{hz:.3f} Hz"
