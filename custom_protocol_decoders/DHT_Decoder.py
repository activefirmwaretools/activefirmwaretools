# DECODER_MYNAME = "DHT11 / DHT22"
#
# Decodes DHT11 / DHT22 (AM2302) humidity + temperature single-wire frames.
#
# PARAM: channel | Logic Channel | digital_channel |  |
# PARAM: sensor  | Sensor Model  | select | DHT22,DHT11 | DHT22
#
# Output rows:
#   ch 0 : humidity (%RH)
#   ch 1 : temperature (degC)
#   ch 2 : checksum verdict ("CHK OK" / "CHK FAIL: got=0xXX want=0xXX")
#
# Protocol notes:
#   - MCU pulls the line low for ~18 ms (DHT11) or ~1 ms (DHT22) to wake
#     the sensor.  We detect any wide low pulse > 500 us as a start.
#   - Sensor responds: ~80 us low + ~80 us high (presence).
#   - 40 data bits follow, MSB first, formatted as:
#       byte 0: humidity integer (DHT11) or humidity high byte (DHT22)
#       byte 1: humidity decimal (DHT11=0) or humidity low byte (DHT22)
#       byte 2: temperature integer / temp high byte
#       byte 3: temperature decimal / temp low byte (sign bit in 0x80 of byte2 for DHT22)
#       byte 4: checksum = (b0 + b1 + b2 + b3) & 0xFF
#   - Each bit: 50 us low, then high for 26-28 us (0) or 70 us (1).
#     Threshold ~50 us between low-then-high after each bit-start low.

import sys

from active_pro import (
    append, wait_for,
    rising_edge, falling_edge,
)


SAMPLE_DATA         = 0
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_HUM  = 0
CH_TEMP = 1
CH_CHK  = 2


def decode(params):
    ch     = int(params.get("channel", "0"))
    sensor = params.get("sensor", "DHT22")

    START_MIN = 500e-6           # MCU start pulse low duration min
    BIT_HIGH_THRESH = 50e-6      # high time threshold to distinguish 0 vs 1

    print(f"DHT decoder: ch={ch} sensor={sensor}", file=sys.stderr)

    while True:
        # ----- Find MCU start pulse -----------------------------------------
        f0 = yield from wait_for(falling_edge(ch))
        if f0 is None:
            return
        r0 = yield from wait_for(rising_edge(ch))
        if r0 is None:
            return
        if (r0.t - f0.t) < START_MIN:
            continue   # too short, keep scanning

        append(f0.t, r0.t, CH_HUM, text="START",
               sample_type=SAMPLE_PACKET_START, color=0xFFAAAAFF)

        # ----- Sensor presence: low ~80us, high ~80us -----------------------
        # Skip them: just wait for the next falling edge that begins bit 0.
        f1 = yield from wait_for(falling_edge(ch))
        if f1 is None:
            return
        r1 = yield from wait_for(rising_edge(ch))   # end of presence low
        if r1 is None:
            return
        # Now we're at the start of the high portion of presence; wait for
        # the falling edge that begins data bit 0.
        f_bit0 = yield from wait_for(falling_edge(ch))
        if f_bit0 is None:
            return

        # ----- Read 40 bits -------------------------------------------------
        bits = []
        # f_bit0 marks the start of bit-0's 50us low.
        last_fall = f_bit0
        for i in range(40):
            rh = yield from wait_for(rising_edge(ch))    # end of low
            if rh is None:
                return
            # The next falling edge ends this bit's high phase.
            fb = yield from wait_for(falling_edge(ch))
            if fb is None:
                # Last bit -- compute high duration to the end of capture
                # if needed; conservatively call it a 0.
                bits.append(0)
                break
            high_dur = fb.t - rh.t
            bits.append(1 if high_dur >= BIT_HIGH_THRESH else 0)
            last_fall = fb

        if len(bits) < 40:
            continue

        # ----- Decode 5 bytes ----------------------------------------------
        b = [0, 0, 0, 0, 0]
        for i, bit in enumerate(bits):
            b[i // 8] |= (bit << (7 - (i % 8)))

        if sensor == "DHT11":
            humidity = float(b[0])
            temp     = float(b[2])
        else:  # DHT22 / AM2302
            humidity = ((b[0] << 8) | b[1]) / 10.0
            raw_t    = ((b[2] & 0x7F) << 8) | b[3]
            temp     = raw_t / 10.0
            if b[2] & 0x80:
                temp = -temp

        chk_want = (b[0] + b[1] + b[2] + b[3]) & 0xFF
        chk_got  = b[4]
        chk_ok = (chk_want == chk_got)

        bit_end_t = (last_fall.t if last_fall else r1.t)
        append(f_bit0.t, bit_end_t, CH_HUM,
               text=f"H = {humidity:.1f} %RH",
               data=int(round(humidity * 10)),
               sample_type=SAMPLE_DATA, color=0xFF88CCFF)
        append(f_bit0.t, bit_end_t, CH_TEMP,
               text=f"T = {temp:.1f} °C",
               data=int(round(temp * 10)) & 0xFFFFFFFF,
               sample_type=SAMPLE_DATA, color=0xFFFF8888)

        if chk_ok:
            append(bit_end_t, bit_end_t, CH_CHK,
                   text=f"CHK OK (0x{chk_got:02X})",
                   color=0xFF88FF88, sample_type=SAMPLE_DATA)
        else:
            append(bit_end_t, bit_end_t, CH_CHK,
                   text=f"CHK FAIL: got 0x{chk_got:02X} want 0x{chk_want:02X}",
                   color=0xFFFF8888, sample_type=SAMPLE_DATA)

        append(bit_end_t, bit_end_t, CH_HUM, text="",
               sample_type=SAMPLE_PACKET_END)
