# DECODER_MYNAME = "Logic Edge Counter"
#
# Example/test decoder: counts rising edges on a configurable logic channel and labels each ("#1", "#2", ...).
#
# Use this to confirm the Custom Decoder pipeline is alive: pick it from
# the Custom Decoder picker on any Device, set Logic Channel to a bit
# you're driving, capture, stop. Annotations appear in the decoder track
# at every rising edge of that bit.
#
# PARAM: channel | Logic Channel | digital_channel |  |

from active_pro import append, wait_for_edge


def decode(params):
    ch    = int(params.get("channel", "0"))
    count = 0
    while True:
        m = yield from wait_for_edge(ch, edge='rising')
        if m is None:
            return
        count += 1
        append(m.t, m.t + 1e-6,
               channel=0,
               text=f"#{count}",
               data=count,
               sample_type=1,
               color=0xFFFFAA00)
