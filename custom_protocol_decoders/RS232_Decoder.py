# DECODER_MYNAME = "RS-232"
#
# Decodes RS-232 UART traffic on one analog channel by thresholding the +/-12V signal at 0 V.
# Use this when probing directly on a DB9 / DB25 wire rather than the TTL-side UART.
#
# PARAM: channel    | Analog Channel  | analog_channel |  | 1
# PARAM: baud_rate  | Baud Rate       | select | 1200,2400,4800,9600,19200,38400,57600,115200 | 9600
# PARAM: data_bits  | Data Bits       | select | 5,6,7,8                                | 8
# PARAM: parity     | Parity          | select | None,Even,Odd                          | None
# PARAM: stop_bits  | Stop Bits       | select | 1,1.5,2                                | 1
# PARAM: threshold  | Threshold (V)   | float  | -12,12                                 | 0.0
# PARAM: hysteresis | Hysteresis (V)  | float  | 0,5                                    | 0.5
#
# Output rows:
#   ch 0 : raw byte hex (e.g. "0x41")
#   ch 1 : printable ASCII (or "." for non-printable)
#   ch 2 : parity verdict ("PAR OK" / "PAR FAIL")
#
# Protocol notes:
#   - RS-232 line idles HIGH at +5..+15 V (logic "mark").  A start bit
#     drives the line LOW to -5..-15 V (logic "space").  After thresholding
#     at 0 V, mark = HIGH (logic 1), space = LOW (logic 0) -- same polarity
#     as a TTL UART, so the framing math matches the UART decoder.
#   - The start of a byte is a falling crossing through `threshold`
#     (going below threshold-hysteresis from above threshold+hysteresis).

import sys

from active_pro import (
    append, wait_for, wait_time,
    voltage_falls, voltage_above, voltage_below,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_HEX   = 0
CH_ASCII = 1
CH_PAR   = 2


def decode(params):
    a_ch       = int(params.get("channel", "1")) - 1   # UI is 1-based
    baud       = int(params.get("baud_rate", "9600"))
    nbits      = int(params.get("data_bits", "8"))
    parity     = params.get("parity", "None")
    stop_bits  = float(params.get("stop_bits", "1"))
    threshold  = float(params.get("threshold", "0.0"))
    hysteresis = float(params.get("hysteresis", "0.5"))

    bit_time = 1.0 / baud

    print(f"RS-232: A{a_ch + 1} {baud} {nbits}{parity[0]}{stop_bits} "
          f"thr={threshold:+.2f}V hyst={hysteresis:.2f}V", file=sys.stderr)

    start_cond = voltage_falls(a_ch, threshold, hysteresis)

    while True:
        start = yield from wait_for(start_cond)
        if start is None:
            return
        append(start.t, start.t, CH_HEX, text="",
               sample_type=SAMPLE_PACKET_START)

        # Skip to middle of bit 0
        m = yield from wait_time(1.5 * bit_time)
        if m is None:
            return
        byte_val = 0
        for i in range(nbits):
            # bit = 1 if voltage above threshold, else 0
            bit = 1 if m.a[a_ch] > threshold else 0
            byte_val |= (bit << i)
            if i < nbits - 1:
                m = yield from wait_time(bit_time)
                if m is None:
                    return

        # Parity bit (if any)
        parity_error = False
        if parity != "None":
            m = yield from wait_time(bit_time)
            if m is None:
                return
            par_bit = 1 if m.a[a_ch] > threshold else 0
            ones = bin(byte_val).count("1") + par_bit
            if parity == "Even" and (ones & 1) != 0:
                parity_error = True
            elif parity == "Odd" and (ones & 1) == 0:
                parity_error = True

        end_t = start.t + (1 + nbits + (1 if parity != "None" else 0)) * bit_time

        hex_text = f"0x{byte_val:02X}"
        if parity_error:
            hex_text += " !PE"
        append(start.t, end_t, CH_HEX, text=hex_text,
               data=byte_val, sample_type=SAMPLE_DATA)

        ch_char = chr(byte_val) if 32 <= byte_val < 127 else "."
        append(start.t, end_t, CH_ASCII, text=ch_char,
               data=byte_val, sample_type=SAMPLE_DATA_ALT,
               color=0xFF88FF88)

        if parity != "None":
            append(end_t, end_t, CH_PAR,
                   text=("PAR FAIL" if parity_error else "PAR OK"),
                   color=(0xFFFF8888 if parity_error else 0xFF88FF88),
                   sample_type=SAMPLE_DATA)

        append(end_t, end_t, CH_HEX, text="", sample_type=SAMPLE_PACKET_END)

        # Skip past stop bit(s) before searching for the next start.
        m = yield from wait_time(stop_bits * bit_time)
        if m is None:
            return
