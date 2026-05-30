# DECODER_MYNAME = "RGMII"
#
# Decodes one direction of an RGMII Ethernet PHY interface (RX or TX).
# RGMII total signal count is 12 (RXC/RXD0-3/RXCTL + TXC/TXD0-3/TXCTL),
# which exceeds a single ACTIVE device's 8 logic channels -- so this
# decoder handles ONE DIRECTION at a time (6 channels).  Use Device A
# for one direction and Device B for the other if you need both.
#
# PARAM: clk_channel | CLK Channel (RXC or TXC)        | digital_channel |  |
# PARAM: d0_channel  | D0 Channel                      | digital_channel |  |
# PARAM: d1_channel  | D1 Channel                      | digital_channel |  |
# PARAM: d2_channel  | D2 Channel                      | digital_channel |  |
# PARAM: d3_channel  | D3 Channel                      | digital_channel |  |
# PARAM: ctl_channel | CTL Channel (RX_CTL or TX_CTL)  | digital_channel |  |
# PARAM: rate        | Data Rate                       | select | DDR,SDR | DDR
# PARAM: direction   | Direction Label                 | select | RX,TX | RX
# PARAM: show_idle   | Annotate Idle                   | bool   |  | false
#
# Output rows:
#   ch 0 : frame event (PREAMBLE_START, SFD, FRAME_END, ERR)
#   ch 1 : header bytes (dest MAC, src MAC, ethertype/length)
#   ch 2 : payload bytes (hex blob, truncated at ~24 bytes per emission)
#
# Protocol notes:
#   - SDR (10/100 Mb/s reduced): one 4-bit nibble per clock cycle, sampled
#     on the rising edge.  Two clocks = one byte (low nibble first).
#   - DDR (1 Gb/s): nibble on rising edge AND on falling edge of clock;
#     two nibbles per clock = one byte per clock (low nibble = rising edge).
#   - TX_CTL / RX_CTL conveys TX_EN/RX_DV on the rising edge and
#     TX_ER/RX_ER on the falling edge, XORed (per RGMII v2.0 sec 3.4).
#     We track only the rising-edge bit ("data valid").
#   - A frame begins when CTL goes high.  Preamble is 7x 0x55, SFD is 0xD5.
#   - Frame ends when CTL goes low.  No FCS check (would require ~64K
#     bytes of state to do efficiently here; capture longer if needed).

import sys

from active_pro import (
    append, wait_for,
    rising_edge, falling_edge, any_of,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_EVT     = 0
CH_HDR     = 1
CH_PAYLOAD = 2


def decode(params):
    clk = int(params.get("clk_channel", "0"))
    d   = [int(params.get(f"d{i}_channel", str(i + 1))) for i in range(4)]
    ctl = int(params.get("ctl_channel", "5"))
    rate = params.get("rate", "DDR")
    direction = params.get("direction", "RX")
    show_idle = params.get("show_idle", "false").lower() == "true"

    print(f"RGMII: CLK={clk} D[3:0]={d[3]},{d[2]},{d[1]},{d[0]} CTL={ctl} "
          f"{rate} ({direction})", file=sys.stderr)

    if rate == "DDR":
        edge_cond = any_of(rising_edge(clk), falling_edge(clk))
    else:
        edge_cond = rising_edge(clk)

    # Frame state.
    in_frame = False
    saw_sfd = False
    frame_bytes = bytearray()
    pending_low_nib = None        # SDR only: last low nibble waiting for high
    pending_low_t = None
    frame_start_t = None
    last_payload_emit_idx = 0

    while True:
        edge = yield from wait_for(edge_cond)
        if edge is None:
            return

        # On RGMII, CTL is sampled at rising edge (= TX_EN / RX_DV).  At
        # falling edge it carries the error bit XORed -- we ignore the
        # error bit for simplicity here.
        if rate == "DDR":
            # Determine whether this is rising or falling clock edge.
            # We can't tell from the moment alone -- we track parity via
            # the alternating-edge state machine.  Simpler: just always
            # latch the nibble; CTL polling on the rising edge requires
            # us to distinguish edges.  Use the clock level *after* the
            # edge: rising -> clk=1, falling -> clk=0.
            is_rising = (edge.d[clk] == 1)
        else:
            is_rising = True

        ctl_valid = edge.d[ctl] if is_rising else None

        nibble = (edge.d[d[3]] << 3) | (edge.d[d[2]] << 2) | \
                 (edge.d[d[1]] << 1) | edge.d[d[0]]

        if rate == "DDR":
            # Rising edge nibble is low nibble; falling edge is high nibble.
            if is_rising:
                # New byte starts here; check CTL for data-valid transition.
                if ctl_valid == 1 and not in_frame:
                    in_frame = True
                    saw_sfd = False
                    frame_bytes = bytearray()
                    frame_start_t = edge.t
                    last_payload_emit_idx = 0
                    append(edge.t, edge.t, CH_EVT, text="PREAMBLE_START",
                           sample_type=SAMPLE_PACKET_START, color=0xFFAAAAFF)
                elif ctl_valid == 0 and in_frame:
                    # CTL deasserted -- end of frame.
                    _flush_payload(frame_bytes, frame_start_t, edge.t,
                                   last_payload_emit_idx, force=True)
                    append(edge.t, edge.t, CH_EVT,
                           text=f"FRAME_END ({len(frame_bytes)}B)",
                           sample_type=SAMPLE_PACKET_END,
                           color=0xFFAAAAFF, data=len(frame_bytes))
                    in_frame = False
                    continue
                if not in_frame:
                    continue
                pending_low_nib = nibble
                pending_low_t = edge.t
            else:
                # Falling edge: complete the byte
                if not in_frame or pending_low_nib is None:
                    continue
                byte_val = (nibble << 4) | pending_low_nib
                frame_bytes.append(byte_val)
                pending_low_nib = None
                _process_byte(frame_bytes, byte_val, pending_low_t, edge.t,
                              saw_sfd_ref=[saw_sfd])
                # _process_byte may emit SFD annotation as a side effect; we
                # check the new saw_sfd state via the byte_val ourselves.
                if not saw_sfd and byte_val == 0xD5:
                    saw_sfd = True
                # Periodically flush the payload row to keep annotations
                # readable.
                if saw_sfd and (len(frame_bytes) - last_payload_emit_idx) >= 24:
                    last_payload_emit_idx = _flush_payload(
                        frame_bytes, frame_start_t, edge.t,
                        last_payload_emit_idx)
        else:
            # SDR: rising edge only, one nibble per cycle, low nibble first.
            if ctl_valid == 1 and not in_frame:
                in_frame = True
                saw_sfd = False
                frame_bytes = bytearray()
                frame_start_t = edge.t
                last_payload_emit_idx = 0
                append(edge.t, edge.t, CH_EVT, text="PREAMBLE_START",
                       sample_type=SAMPLE_PACKET_START, color=0xFFAAAAFF)
            elif ctl_valid == 0 and in_frame:
                _flush_payload(frame_bytes, frame_start_t, edge.t,
                               last_payload_emit_idx, force=True)
                append(edge.t, edge.t, CH_EVT,
                       text=f"FRAME_END ({len(frame_bytes)}B)",
                       sample_type=SAMPLE_PACKET_END, color=0xFFAAAAFF,
                       data=len(frame_bytes))
                in_frame = False
                pending_low_nib = None
                continue
            if not in_frame:
                continue
            if pending_low_nib is None:
                pending_low_nib = nibble
                pending_low_t = edge.t
            else:
                byte_val = (nibble << 4) | pending_low_nib
                frame_bytes.append(byte_val)
                pending_low_nib = None
                _process_byte(frame_bytes, byte_val, pending_low_t, edge.t,
                              saw_sfd_ref=[saw_sfd])
                if not saw_sfd and byte_val == 0xD5:
                    saw_sfd = True
                if saw_sfd and (len(frame_bytes) - last_payload_emit_idx) >= 24:
                    last_payload_emit_idx = _flush_payload(
                        frame_bytes, frame_start_t, edge.t,
                        last_payload_emit_idx)


def _process_byte(frame_bytes, byte_val, t_start, t_end, saw_sfd_ref):
    """Emit header annotations as we reach known fields."""
    idx = len(frame_bytes) - 1
    saw_sfd = saw_sfd_ref[0]
    # Preamble bytes and SFD
    if not saw_sfd:
        if byte_val == 0xD5:
            append(t_start, t_end, CH_EVT, text="SFD (0xD5)",
                   sample_type=SAMPLE_DATA, color=0xFF88FF88)
        return
    payload_idx = idx - frame_bytes.index(0xD5) - 1   # index after SFD
    if payload_idx == 5:
        mac = ":".join(f"{frame_bytes[idx - 5 + i + 1]:02X}" for i in range(6))
        # The above is messy; recompute simply.
    # Simpler: compute payload from SFD index each time.
    try:
        sfd_pos = frame_bytes.index(0xD5)
    except ValueError:
        return
    payload = frame_bytes[sfd_pos + 1:]
    if len(payload) == 6:
        mac = ":".join(f"{b:02X}" for b in payload)
        append(t_start - (t_end - t_start) * 5, t_end, CH_HDR,
               text=f"DEST {mac}", sample_type=SAMPLE_DATA,
               color=0xFFAAEEFF)
    elif len(payload) == 12:
        mac = ":".join(f"{b:02X}" for b in payload[6:12])
        append(t_start - (t_end - t_start) * 5, t_end, CH_HDR,
               text=f"SRC  {mac}", sample_type=SAMPLE_DATA,
               color=0xFFAAEEFF)
    elif len(payload) == 14:
        et = (payload[12] << 8) | payload[13]
        if et >= 0x0600:
            label = _ethertype_name(et)
            text = f"EtherType 0x{et:04X} ({label})"
        else:
            text = f"Length {et}"
        append(t_start - (t_end - t_start), t_end, CH_HDR,
               text=text, sample_type=SAMPLE_DATA, color=0xFFFFCC88,
               data=et)


def _flush_payload(frame_bytes, t_start, t_end, from_idx, force=False):
    try:
        sfd_pos = frame_bytes.index(0xD5)
    except ValueError:
        return from_idx
    payload_start = max(from_idx, sfd_pos + 1 + 14)  # skip header (14 B)
    if not force and len(frame_bytes) - payload_start < 24:
        return from_idx
    chunk = frame_bytes[payload_start:]
    if not chunk:
        return from_idx
    hex_blob = " ".join(f"{b:02X}" for b in chunk[:24])
    if len(chunk) > 24:
        hex_blob += " ..."
    append(t_start, t_end, CH_PAYLOAD,
           text=f"[{len(chunk)}B] {hex_blob}",
           sample_type=SAMPLE_DATA)
    return len(frame_bytes)


def _ethertype_name(et):
    table = {
        0x0800: "IPv4",  0x0806: "ARP",  0x86DD: "IPv6",
        0x8100: "VLAN",  0x8863: "PPPoE Discovery",
        0x8864: "PPPoE Session", 0x88CC: "LLDP", 0x88F7: "PTP",
        0x8847: "MPLS",
    }
    return table.get(et, "unknown")
