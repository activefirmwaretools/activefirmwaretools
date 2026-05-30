# DECODER_MYNAME = "4-20mA Loop"
#
# Decodes a 4-20mA industrial current loop sensed across a shunt resistor on one analog channel.
# Converts the instantaneous voltage to loop current (mA) and optionally to an engineering
# unit using a linear scale.
#
# PARAM: channel       | Analog Channel        | analog_channel |  | 1
# PARAM: shunt_ohms    | Shunt Resistance (Ω)  | float | 1,1000 | 250.0
# PARAM: report_period | Report Period (ms)    | float | 1,10000 | 100.0
# PARAM: unit_label    | Engineering Unit      | string|     | mA
# PARAM: scale_at_4    | EU Value at 4 mA      | float |     | 4.0
# PARAM: scale_at_20   | EU Value at 20 mA     | float |     | 20.0
# PARAM: alarm_low_mA  | Alarm Low (mA)        | float | 0,30 | 3.6
# PARAM: alarm_high_mA | Alarm High (mA)       | float | 0,30 | 22.0
#
# Output rows:
#   ch 0 : averaged current (mA) over each report period
#   ch 1 : engineering-unit value
#   ch 2 : alarm flags (LOOP_BROKEN, OVER_RANGE, UNDER_RANGE)
#
# Notes:
#   - Default 250 Ω shunt converts 4-20 mA to 1-5 V (Foxboro standard).
#   - "Loop broken" is reported when current drops below alarm_low_mA
#     (typically <3.6 mA on a 4 mA process variable).
#   - The decoder samples by polling the analog buffer at fixed time
#     intervals; the *captured* analog sample rate determines actual
#     resolution.

import sys

from active_pro import (
    append, wait_for, wait_time,
)


SAMPLE_DATA = 0

CH_CURRENT = 0
CH_EU      = 1
CH_ALARM   = 2


def decode(params):
    a_ch    = int(params.get("channel", "1")) - 1
    shunt   = max(0.001, float(params.get("shunt_ohms", "250.0")))
    period  = max(0.001, float(params.get("report_period", "100.0"))) / 1000.0
    unit    = params.get("unit_label", "mA")
    eu4     = float(params.get("scale_at_4", "4.0"))
    eu20    = float(params.get("scale_at_20", "20.0"))
    alarm_lo = float(params.get("alarm_low_mA", "3.6"))
    alarm_hi = float(params.get("alarm_high_mA", "22.0"))

    print(f"4-20mA: A{a_ch + 1} shunt={shunt}Ω period={period*1000}ms "
          f"EU(4mA)={eu4} EU(20mA)={eu20} alarms=[{alarm_lo}, {alarm_hi}]mA",
          file=sys.stderr)

    poll = period / 16.0   # sub-sample within each report period
    n = 0
    sum_mA = 0.0
    win_start_t = None
    last_alarm = None

    while True:
        m = yield from wait_time(poll)
        if m is None:
            return
        if win_start_t is None:
            win_start_t = m.t

        v = m.a[a_ch]
        mA = (v / shunt) * 1000.0   # I = V / R, then to mA
        sum_mA += mA
        n += 1

        if (m.t - win_start_t) >= period:
            avg_mA = sum_mA / n
            t_end = m.t

            append(win_start_t, t_end, CH_CURRENT,
                   text=f"{avg_mA:6.3f} mA",
                   data=int(round(avg_mA * 1000)) & 0xFFFFFFFF,
                   sample_type=SAMPLE_DATA, color=0xFFAAEEFF)

            # Linear interpolation between (4 mA -> eu4) and (20 mA -> eu20).
            slope = (eu20 - eu4) / 16.0   # per mA
            eu_val = eu4 + slope * (avg_mA - 4.0)
            append(win_start_t, t_end, CH_EU,
                   text=f"{eu_val:.3f} {unit}",
                   sample_type=SAMPLE_DATA, color=0xFFAAFFAA)

            # Alarms
            new_alarm = None
            if avg_mA < alarm_lo:
                new_alarm = "LOOP_BROKEN / UNDER_RANGE"
                color = 0xFFFF8888
            elif avg_mA > alarm_hi:
                new_alarm = "OVER_RANGE"
                color = 0xFFFFAA88
            if new_alarm and new_alarm != last_alarm:
                append(win_start_t, t_end, CH_ALARM,
                       text=new_alarm, color=color,
                       sample_type=SAMPLE_DATA)
            elif new_alarm is None and last_alarm is not None:
                append(win_start_t, win_start_t, CH_ALARM,
                       text="OK", color=0xFF88FF88,
                       sample_type=SAMPLE_DATA)
            last_alarm = new_alarm

            sum_mA = 0.0
            n = 0
            win_start_t = None
