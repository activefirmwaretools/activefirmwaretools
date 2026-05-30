# DECODER_MYNAME = "RS-485"
#
# Decodes RS-485 differential UART traffic on two analog channels by computing A-B per sample.
# Use this on the wire itself, not the TTL side.
#
# RS-485 idle ("mark", logic 1) is defined as A < B (negative differential).
# A start bit drives A > B (positive differential, "space", logic 0).
# After differential decode, the resulting logic stream is identical to
# the standard UART (idle high, falling-edge start) so framing follows
# the usual rules.
#
# PARAM: a_channel  | A Analog Channel | analog_channel |  | 1
# PARAM: b_channel  | B Analog Channel | analog_channel |  | 2
# PARAM: baud_rate  | Baud Rate        | select | 9600,19200,38400,57600,115200,230400 | 115200
# PARAM: data_bits  | Data Bits        | select | 7,8                             | 8
# PARAM: parity     | Parity           | select | None,Even,Odd                   | None
# PARAM: stop_bits  | Stop Bits        | select | 1,2                             | 1
# PARAM: threshold  | Diff Threshold (V) | float | 0,5                            | 0.2
# PARAM: invert_diff | Invert Differential | bool |                               | false
#
# Output rows:
#   ch 0 : raw byte hex
#   ch 1 : printable ASCII
#   ch 2 : parity verdict
#
# Convention: bit = 1 if (A - B) < -threshold, bit = 0 if (A - B) > +threshold.
# This matches the EIA/TIA-485 spec.  Use `invert_diff` if your probe
# pairing is reversed.

import sys

from active_pro import (
    append, wait_for, wait_time,
    voltage_rises, voltage_falls,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_HEX   = 0
CH_ASCII = 1
CH_PAR   = 2


def decode(params):
    a_ch       = int(params.get("a_channel", "1")) - 1
    b_ch       = int(params.get("b_channel", "2")) - 1
    baud       = int(params.get("baud_rate", "115200"))
    nbits      = int(params.get("data_bits", "8"))
    parity     = params.get("parity", "None")
    stop_bits  = float(params.get("stop_bits", "1"))
    thresh     = float(params.get("threshold", "0.2"))
    invert     = params.get("invert_diff", "false").lower() == "true"

    bit_time = 1.0 / baud
    sign = -1.0 if invert else 1.0

    print(f"RS-485: A=ch{a_ch + 1} B=ch{b_ch + 1} {baud} {nbits}{parity[0]}{stop_bits} "
          f"thr={thresh}V invert={invert}", file=sys.stderr)

    # A start bit is a transition from idle (diff < 0) to active (diff > 0).
    # In sample-and-hold polling we manually compute diff at each
    # moment rather than relying on a built-in differential primitive
    # (the API exposes per-channel voltage conditions, not differential).
    #
    # Strategy: scan for the first moment where the differential crosses
    # the +threshold from below.  We approximate this by looking for a
    # rising crossing on channel A and confirming via the diff sign at
    # the moment of crossing -- but that misses true differential edges
    # where A is steady and B drops.  Simpler and more robust: walk the
    # captured analog stream by polling at fixed time steps once we know
    # we're in idle.
    #
    # Implementation: use voltage_rises on a "virtual" differential
    # channel by riding either A rising or B falling.  We handle the
    # general case by sampling at fine intervals after each detected
    # edge.

    # Polling tick: a small fraction of bit_time so we can catch a start
    # bit on either side of the differential pair.
    poll = bit_time / 8.0

    # Find first start: wait for ((A - B) * sign) to exceed threshold.
    while True:
        m = yield from wait_time(poll)
        if m is None:
            return
        diff = sign * (m.a[a_ch] - m.a[b_ch])
        if diff > thresh:
            start_t = m.t
            break

    while True:
        # We're at the start of a start bit (diff > +thresh).  Sample at
        # the middle of bit 0 (1.5 bit times later).
        m = yield from wait_time(1.5 * bit_time)
        if m is None:
            return

        # Build byte.
        byte_val = 0
        for i in range(nbits):
            diff = sign * (m.a[a_ch] - m.a[b_ch])
            bit = 1 if diff < -thresh else 0   # mark = idle high (=1)
            byte_val |= (bit << i)
            if i < nbits - 1:
                m = yield from wait_time(bit_time)
                if m is None:
                    return

        # Parity bit
        parity_error = False
        if parity != "None":
            m = yield from wait_time(bit_time)
            if m is None:
                return
            diff = sign * (m.a[a_ch] - m.a[b_ch])
            par_bit = 1 if diff < -thresh else 0
            ones = bin(byte_val).count("1") + par_bit
            if parity == "Even" and (ones & 1) != 0:
                parity_error = True
            elif parity == "Odd" and (ones & 1) == 0:
                parity_error = True

        end_t = start_t + (1 + nbits + (1 if parity != "None" else 0)) * bit_time

        hex_text = f"0x{byte_val:02X}"
        if parity_error:
            hex_text += " !PE"
        append(start_t, end_t, CH_HEX, text=hex_text,
               data=byte_val, sample_type=SAMPLE_DATA)
        append(start_t, end_t, CH_ASCII,
               text=chr(byte_val) if 32 <= byte_val < 127 else ".",
               data=byte_val, sample_type=SAMPLE_DATA_ALT,
               color=0xFF88FF88)
        if parity != "None":
            append(end_t, end_t, CH_PAR,
                   text=("PAR FAIL" if parity_error else "PAR OK"),
                   color=(0xFFFF8888 if parity_error else 0xFF88FF88),
                   sample_type=SAMPLE_DATA)

        # Skip past stop bit, then resync to next start bit.
        m = yield from wait_time(stop_bits * bit_time)
        if m is None:
            return

        while True:
            m = yield from wait_time(poll)
            if m is None:
                return
            diff = sign * (m.a[a_ch] - m.a[b_ch])
            if diff > thresh:
                start_t = m.t
                break
