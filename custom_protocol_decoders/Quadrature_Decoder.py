# DECODER_MYNAME = "Quadrature Encoder"
#
# Tracks an A/B quadrature encoder (optional Z index), emitting cumulative count and direction.
#
# PARAM: a_channel | A Channel        | digital_channel |  |
# PARAM: b_channel | B Channel        | digital_channel |  |
# PARAM: z_channel | Z Channel (-1=none) | int |  | -1
# PARAM: mode      | Decode Mode      | select | x1,x2,x4 | x4
# PARAM: report    | Annotation       | select | every_count,every_n,direction_changes | every_n
# PARAM: every_n   | Annotate Every N | int    |  | 10
#
# Output rows:
#   ch 0 : count + direction (e.g. "+125  CW")
#   ch 1 : Z index pulse marker
#
# Protocol notes:
#   - The standard quadrature table maps (prev AB, curr AB) -> step:
#       Forward sequence (CW):  00 -> 01 -> 11 -> 10 -> 00
#       Reverse sequence (CCW): 00 -> 10 -> 11 -> 01 -> 00
#   - x1 counts only one edge per cycle (rising A).
#   - x2 counts both A edges.
#   - x4 counts every A and B edge -- highest resolution.
#   - Invalid transitions (e.g. 00->11) are skipped silently; they
#     usually mean a sample was missed.

import sys

from active_pro import (
    append, wait_for,
    rising_edge, falling_edge, any_of,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1

CH_COUNT = 0
CH_INDEX = 1

# Lookup: (prev_ab, curr_ab) -> delta (+1, -1, or 0).
_DELTA = {
    (0b00, 0b01): +1, (0b01, 0b11): +1, (0b11, 0b10): +1, (0b10, 0b00): +1,
    (0b00, 0b10): -1, (0b10, 0b11): -1, (0b11, 0b01): -1, (0b01, 0b00): -1,
}


def decode(params):
    a_ch = int(params.get("a_channel", "0"))
    b_ch = int(params.get("b_channel", "1"))
    z_ch = int(params.get("z_channel", "-1"))
    mode = params.get("mode", "x4")
    report = params.get("report", "every_n")
    every_n = max(1, int(params.get("every_n", "10")))

    print(f"Quadrature: A={a_ch} B={b_ch} Z={z_ch} mode={mode} "
          f"report={report}", file=sys.stderr)

    # In x1 we count only on rising-A events; in x2 on any A edge; x4 on all.
    if mode == "x1":
        edge_cond = rising_edge(a_ch)
    elif mode == "x2":
        edge_cond = any_of(rising_edge(a_ch), falling_edge(a_ch))
    else:
        edge_cond = any_of(rising_edge(a_ch), falling_edge(a_ch),
                           rising_edge(b_ch), falling_edge(b_ch))

    # Optional Z monitoring: ride it on every edge by checking d[z_ch].
    z_cond = rising_edge(z_ch) if z_ch >= 0 else None

    count = 0
    last_count = 0
    last_dir = 0   # +1 or -1
    prev_ab = None

    while True:
        if z_cond is not None:
            ev = yield from wait_for(any_of(edge_cond, z_cond))
        else:
            ev = yield from wait_for(edge_cond)
        if ev is None:
            return

        # Z index pulse?
        if z_ch >= 0 and ev.d[z_ch] == 1:
            # Z pulses can coincide with A/B edges; we annotate Z then fall
            # through to also process the A/B edge below.
            append(ev.t, ev.t, CH_INDEX, text="Z",
                   color=0xFFFF8800, sample_type=SAMPLE_DATA_ALT)

        ab = (ev.d[a_ch] << 1) | ev.d[b_ch]
        if prev_ab is None:
            prev_ab = ab
            continue

        delta = _DELTA.get((prev_ab, ab), 0)
        prev_ab = ab
        if delta == 0:
            continue
        count += delta

        # Annotation strategy
        dir_changed = (delta != last_dir) and (last_dir != 0)
        emit = False
        if report == "every_count":
            emit = True
        elif report == "every_n":
            if abs(count - last_count) >= every_n:
                emit = True
        elif report == "direction_changes":
            if dir_changed:
                emit = True

        if emit:
            sign = "+" if count >= 0 else ""
            tag  = "CW" if delta > 0 else "CCW"
            append(ev.t, ev.t, CH_COUNT,
                   text=f"{sign}{count}  {tag}",
                   data=count & 0xFFFFFFFF,
                   sample_type=SAMPLE_DATA,
                   color=0xFF88FFAA if delta > 0 else 0xFFFFAA88)
            last_count = count

        last_dir = delta
