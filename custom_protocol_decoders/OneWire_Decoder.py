# DECODER_MYNAME = "1-Wire"
#
# Decodes Dallas / Maxim 1-Wire bus on a single digital channel.
#
# PARAM: channel | Logic Channel | digital_channel |  |
#
# Output rows (within this decoder's Device slot):
#   ch 0 : bus events (RESET / PRESENCE / no-presence)
#   ch 1 : ROM command name (e.g. "SKIP ROM", "READ ROM")
#   ch 2 : raw data bytes (e.g. "0xA5")
#
# Protocol notes (open-drain pulled-up bus, idle high):
#   - Bit slot is ~60 us. Master pulls low, then releases:
#       - Low pulse  < ~15 us  -> bit = 1
#       - Low pulse >= ~15 us  -> bit = 0
#   - Reset:    master holds low >=480 us.
#   - Presence: slave pulls low 60-240 us, 15-60 us after release.
#   - Bytes are LSB-first.
#   - Each transaction (RESET..next RESET) is wrapped in
#     SAMPLE_PACKET_START / SAMPLE_PACKET_END markers.

import sys

from active_pro import (
    append, wait_for, wait_time,
    falling_edge, rising_edge, high,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_BUS  = 0
CH_ROM  = 1
CH_DATA = 2

# Known ROM commands (incomplete -- add more as needed).
ROM_COMMANDS = {
    0x33: "READ ROM",
    0x55: "MATCH ROM",
    0xCC: "SKIP ROM",
    0xF0: "SEARCH ROM",
    0xEC: "ALARM SEARCH",
    0x3C: "OVERDRIVE SKIP",
    0x69: "OVERDRIVE MATCH",
}


def decode(params):
    ch = int(params.get("channel", "0"))

    # Timing thresholds (seconds).
    RESET_MIN  = 400e-6   # master low >= 480 us per spec; allow some slack
    BIT_THRESH = 15e-6    # < 15 us low = 1, >= 15 us low = 0

    print(f"1-Wire decoder: ch={ch}", file=sys.stderr)

    packet_open = False

    while True:
        # ----- Wait for a master pull-low (start of any slot) ---------------
        fall = yield from wait_for(falling_edge(ch))
        if fall is None:
            if packet_open:
                append(fall.t if fall else 0, fall.t if fall else 0,
                       CH_BUS, text="", sample_type=SAMPLE_PACKET_END)
            return

        # ----- Measure how long the line stays low --------------------------
        rise = yield from wait_for(rising_edge(ch))
        if rise is None:
            return
        low_dur = rise.t - fall.t

        # ----- Classify ----------------------------------------------------
        if low_dur >= RESET_MIN:
            # Close any in-flight packet.
            if packet_open:
                append(fall.t, fall.t, CH_BUS,
                       text="", sample_type=SAMPLE_PACKET_END)
            append(fall.t, rise.t, CH_BUS, text="RESET",
                   sample_type=SAMPLE_PACKET_START)
            packet_open = True

            # ----- Look for presence pulse within ~60 us ----------------
            # Slave pulls low 15-60 us after master release. We wait up
            # to 80 us for a falling edge -- if it doesn't appear, no
            # device is present.
            pres_fall = yield from wait_for(falling_edge(ch), timeout=80e-6)
            if pres_fall is None:
                append(rise.t, rise.t + 80e-6, CH_BUS, text="(no presence)",
                       color=0xFFFF8888, sample_type=SAMPLE_DATA)
                continue
            pres_rise = yield from wait_for(rising_edge(ch))
            if pres_rise is None:
                return
            append(pres_fall.t, pres_rise.t, CH_BUS, text="PRESENCE",
                   color=0xFF88FF88, sample_type=SAMPLE_DATA)

            # Read the next byte as the ROM command.
            byte_val, byte_start, byte_end = yield from _read_byte(
                ch, BIT_THRESH)
            if byte_val is None:
                return
            cmd_name = ROM_COMMANDS.get(byte_val, f"0x{byte_val:02X}")
            append(byte_start, byte_end, CH_ROM,
                   text=cmd_name, data=byte_val,
                   color=0xFFAAEEFF, sample_type=SAMPLE_DATA)
            continue

        # Not a reset -- it was a bit slot. The leading edge was the
        # falling we already consumed; classify and accumulate as a byte.
        # Rewind: we need to gather 7 more bits for a complete byte.
        bit0 = 1 if low_dur < BIT_THRESH else 0
        byte_val = bit0
        byte_start = fall.t
        ok = True
        for bitpos in range(1, 8):
            f2 = yield from wait_for(falling_edge(ch))
            if f2 is None:
                ok = False
                break
            r2 = yield from wait_for(rising_edge(ch))
            if r2 is None:
                ok = False
                break
            bit = 1 if (r2.t - f2.t) < BIT_THRESH else 0
            byte_val |= (bit << bitpos)
            byte_end = r2.t
        if not ok:
            return
        append(byte_start, byte_end, CH_DATA,
               text=f"0x{byte_val:02X}", data=byte_val,
               sample_type=SAMPLE_DATA)


def _read_byte(ch, bit_thresh):
    """Read one LSB-first byte from the bus. Returns (val, t_start, t_end)
    or (None, None, None) on capture-end."""
    byte_val = 0
    byte_start = None
    byte_end = None
    for bitpos in range(8):
        f = yield from wait_for(falling_edge(ch))
        if f is None:
            return None, None, None
        r = yield from wait_for(rising_edge(ch))
        if r is None:
            return None, None, None
        if byte_start is None:
            byte_start = f.t
        byte_end = r.t
        bit = 1 if (r.t - f.t) < bit_thresh else 0
        byte_val |= (bit << bitpos)
    return byte_val, byte_start, byte_end
