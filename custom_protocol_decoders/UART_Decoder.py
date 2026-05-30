# DECODER_MYNAME = "UART"
#
# Decodes asynchronous serial (UART) data on one digital channel.
#
# PARAM: baud_rate     | Baud Rate       | select | 9600,19200,38400,57600,115200,230400 | 115200
# PARAM: data_bits     | Data Bits       | select | 5,6,7,8                              | 8
# PARAM: parity        | Parity          | select | None,Even,Odd                        | None
# PARAM: stop_bits     | Stop Bits       | select | 1,1.5,2                              | 1
# PARAM: logic_channel | Logic Channel   | digital_channel |                              |
# PARAM: invert        | Invert Polarity | bool   |                                      | false
#
# Output rows (within this decoder's Device slot):
#   ch 0 : decoded byte as hex (e.g. "0x41")
#   ch 1 : printable ASCII (or "." for non-printable bytes)
#
# Framing notes:
#   - The signal is sampled in the middle of each bit cell. That means after
#     detecting the falling edge of the start bit we advance 1.5 bit times to
#     land in the middle of data bit 0, then 1 bit time per subsequent bit.
#   - Parity is checked but does not block output -- mismatches are flagged
#     by appending the suffix " !PE" to the hex annotation.
#   - With Invert Polarity on, every sampled bit is inverted before the byte
#     is assembled. Useful for RS-232 lines that have already been level-
#     shifted to inverted TTL.
#   - Each decoded byte is wrapped in SAMPLE_PACKET_START / SAMPLE_PACKET_END
#     markers so an attached PacketPresenter sees one "packet" per byte.

import sys

from active_pro import (
    append,
    wait_for, wait_time,
    falling_edge, rising_edge,
)


# sample_type constants understood by PacketPresenter (mirrors defines.h).
SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

# Output row indices.
CH_HEX   = 0
CH_ASCII = 1


def decode(params):
    """UART 8N1-style decoder, generalized via PARAM lines."""

    # --- Parameter parsing -------------------------------------------------
    baud      = int(params.get("baud_rate", "115200"))
    nbits     = int(params.get("data_bits", "8"))
    parity    = params.get("parity", "None")
    stop_bits = float(params.get("stop_bits", "1"))
    ch        = int(params.get("logic_channel", "0"))
    invert    = params.get("invert", "false").lower() == "true"

    bit_time = 1.0 / baud

    # --- Edge polarity for the start bit -----------------------------------
    # Normal (non-inverted) UART: idle high, start bit is a falling edge.
    # Inverted UART (idle low, start bit rising): use rising_edge instead.
    start_edge = rising_edge(ch) if invert else falling_edge(ch)

    print(f"UART decoder: {baud} baud, {nbits}{parity[0]}{stop_bits}, "
          f"ch={ch}, invert={invert}", file=sys.stderr)

    # --- Main loop ---------------------------------------------------------
    while True:
        # 1. Wait for the start bit.
        start = yield from wait_for(start_edge)
        if start is None:
            return

        # PacketPresenter packet boundary -- one byte per packet.
        append(start.t, start.t, CH_HEX,
               text="", sample_type=SAMPLE_PACKET_START)

        # 2. Skip to the middle of data bit 0.
        m = yield from wait_time(1.5 * bit_time)
        if m is None:
            return

        # 3. Sample each data bit. LSB first (standard UART).
        byte_val = 0
        for i in range(nbits):
            bit = m.d[ch] ^ (1 if invert else 0)
            byte_val |= (bit << i)

            # Advance to the middle of the next bit, unless this is the
            # last data bit -- in which case the next wait will be parity
            # or the stop bit.
            if i < nbits - 1:
                m = yield from wait_time(bit_time)
                if m is None:
                    return

        # 4. Parity bit (if any).
        parity_error = False
        if parity != "None":
            m = yield from wait_time(bit_time)
            if m is None:
                return
            parity_bit = m.d[ch] ^ (1 if invert else 0)

            # Even parity: bit makes the total number of 1s even.
            # Odd  parity: bit makes the total number of 1s odd.
            ones = bin(byte_val).count("1") + parity_bit
            if parity == "Even" and (ones & 1) != 0:
                parity_error = True
            elif parity == "Odd" and (ones & 1) == 0:
                parity_error = True

        # 5. Compute the end-of-byte timestamp for the annotations.
        #    (We don't need to actually sample the stop bit -- we just need
        #     to position the end timestamp at the trailing edge of the byte.)
        bytes_end_t = start.t + (1 + nbits + (1 if parity != "None" else 0)) * bit_time

        # 6. Emit annotations.
        hex_text = f"0x{byte_val:02X}"
        if parity_error:
            hex_text += " !PE"
        append(start.t, bytes_end_t, CH_HEX,
               text=hex_text, sample_type=SAMPLE_DATA, data=byte_val)

        ch_char = chr(byte_val) if 32 <= byte_val < 127 else "."
        append(start.t, bytes_end_t, CH_ASCII,
               text=ch_char, sample_type=SAMPLE_DATA_ALT, data=byte_val)

        append(bytes_end_t, bytes_end_t, CH_HEX,
               text="", sample_type=SAMPLE_PACKET_END)

        # 7. Skip past the stop bit(s) before looking for the next start.
        m = yield from wait_time(stop_bits * bit_time)
        if m is None:
            return
