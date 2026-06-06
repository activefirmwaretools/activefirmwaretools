# DECODER_MYNAME = "I3C"
#
# Decodes MIPI I3C (v1.1) Single-Data-Rate (SDR) traffic on SCL and SDA.
# Recognizes the 0x7E broadcast preamble, the full Common Command Code
# (CCC) table, ENTDAA dynamic-address-assignment byte structure
# (PID/BCR/DCR/NEWADDR), CCC-specific parameter byte labeling, In-Band
# Interrupts (IBIs), Hot-Join, HDR-mode entry markers, and write-T-bit
# parity validation.
#
# PARAM: scl_channel       | SCL Channel              | digital_channel |  |
# PARAM: sda_channel       | SDA Channel              | digital_channel |  |
# PARAM: check_parity      | Check Write T-bit Parity | bool |  | true
# PARAM: assume_i3c_bus    | Assume I3C Bus           | bool |  | false
#
# `assume_i3c_bus` -- when true, the very first START is treated as an
# I3C transaction even if the first byte is not 0x7E (useful when the
# capture began mid-stream after a 0x7E broadcast had already happened).
# When false, the decoder waits to see a 0x7E broadcast before promoting
# the bus to I3C mode; pre-promotion transactions are labeled as legacy
# I2C. Once promoted, the I3C state persists for the rest of the capture.
#
# Output rows (within this decoder's Device slot):
#   ch 0 : Address byte + direction
#            BCAST 0x7E W       -- I3C master-initiated transaction preamble
#            I3C   0xNN R/W     -- private I3C transfer (post-broadcast)
#            I2C   0xNN R/W     -- legacy I2C device
#            IBI   0xNN R       -- target-initiated IBI (heuristic)
#            HOT-JOIN 0x02 W    -- target requesting dynamic address
#   ch 1 : CCC name (e.g. "CCC ENTDAA [B] (0x07)") OR per-byte field
#          label produced by the CCC's parameter schema and by ENTDAA
#          (e.g. "PID[0]", "BCR Tgt IBIReq", "NEWDA=0x1A", "MWL_MSB").
#   ch 2 : Raw data byte (e.g. "0xA5"). Suppressed for CCC bytes and
#          for ENTDAA bytes -- those go on ch 1 with their schema label
#          so the data row isn't double-displayed.
#   ch 3 : 9th SCL bit
#            ACK / NAK          -- on address bytes (open-drain)
#            T=0 (end)          -- read T-bit: target says "end of data"
#            T=1 (more)         -- read T-bit: target has more data
#            T=N (par OK)       -- write T-bit, odd parity verified
#            T=N (PAR ERR!)     -- write T-bit, parity mismatch
#   ch 4 : Framing/annotations
#            START / RESTART / STOP
#            HDR ENTRY (...)    -- ENTHDR0..7 detected; SDR decoding past
#                                  this point will be meaningless until
#                                  a subsequent STOP / HDR exit pattern.
#
# Each START..STOP transaction is wrapped in SAMPLE_PACKET_START /
# SAMPLE_PACKET_END so any attached PacketPresenter can frame it.
#
# ============================================================================
# Protocol reference (kept here so the editor view is self-contained)
# ============================================================================
#
# I3C SDR signaling vs. I2C:
#   - START / STOP / RESTART are identical: SDA falling/rising while SCL high.
#   - Bits are sampled MSB-first on each SCL rising edge.
#   - 9 SCL clocks per byte (8 data + 1 "ninth bit").
#   - Address-byte ninth bit is ACK/NAK (open-drain), same as I2C.
#   - Data-byte ninth bit is the T-bit, NOT ACK:
#         Write (controller -> target): T = odd parity over the 8 data
#                                       bits (1 if data has even number
#                                       of 1-bits, 0 otherwise).
#         Read  (target  -> controller): T = 1 means "more data follows",
#                                       T = 0 means "end of data". The
#                                       controller can preempt by holding
#                                       SDA low after bit 8 to force T=0.
#
# Every controller-initiated I3C transfer begins with the broadcast
# address 0x7E. Sequence templates:
#
#   Broadcast write CCC (e.g. RSTDAA, SETMWL):
#     START + 0x7E+W + CCC + [param bytes ...] + STOP
#
#   Direct write CCC (e.g. SETNEWDA):
#     START + 0x7E+W + CCC
#            + RESTART + TARGET+W + [param bytes ...]
#            [+ RESTART + TARGET2+W + [param bytes ...]]
#            + STOP
#
#   Direct read CCC (e.g. GETPID, GETBCR):
#     START + 0x7E+W + CCC
#            + RESTART + TARGET+R + [response bytes ...]
#            [+ RESTART + ...]
#            + STOP
#
#   Private write (no CCC, just data after broadcast):
#     START + 0x7E+W + RESTART + TARGET+W + [data ...] + STOP
#
#   Private read:
#     START + 0x7E+W + RESTART + TARGET+R + [data ...] + STOP
#
#   ENTDAA (dynamic address assignment) -- a special direct read CCC:
#     START + 0x7E+W + ENTDAA(0x07)
#            + RESTART + 0x7E+R
#                + PID[0..5]   (6 bytes target -> controller, with T-bit)
#                + BCR          (1 byte target -> controller, with T-bit)
#                + DCR          (1 byte target -> controller, with T-bit)
#                + NEWADDR      (1 byte controller -> target;
#                                bit 0 = odd parity over bits 7..1;
#                                ninth bit is ACK/NAK from target,
#                                NOT a T-bit)
#            [+ RESTART + 0x7E+R + next target's PID..NEWADDR]
#            + STOP
#
#   Hot-Join (uninitialized target asks for address):
#     The bus-idle target pulls SDA low. To the bit stream this looks
#     like START + 0x02+W. The controller's ENTDAA cycle then runs to
#     assign an address.
#
#   IBI (In-Band Interrupt, target asks for service):
#     Target pulls SDA low during bus idle -- looks like START + the
#     target's own 7-bit dynamic address + R=1. After ACK, an optional
#     Mandatory Data Byte (MDB) follows with T-bit.
#
# BCR (Bus Characteristics Register) bit map:
#     7:6  Device Role           00=Tgt, 01=Sec-Ctrl, 1x=reserved
#     5    Advanced capabilities (GETCAPS supported)
#     4    Virtual target support
#     3    Offline capable
#     2    IBI payload follows IBI address byte (MDB or more)
#     1    IBI request capable
#     0    Max data speed limitation (consult GETMXDS)
#
# DCR (Device Characteristics Register) is an 8-bit device-type code;
# the spec leaves the mapping to MIPI alliance assignments -- we just
# show the raw value.

import sys

from active_pro import (
    append,
    wait_for,
    rising_edge, falling_edge, high,
    all_of, any_of,
)


# ----- sample_type constants understood by PacketPresenter -------------------
SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

# ----- Output channel layout -------------------------------------------------
CH_ADDR  = 0   # address byte + R/W direction
CH_CCC   = 1   # CCC name OR per-byte field label (PID[i], BCR, NEWDA, ...)
CH_DATA  = 2   # raw data byte hex
CH_ACKT  = 3   # 9th-bit annotation (ACK/NAK or T-bit)
CH_NOTE  = 4   # START/STOP/RESTART framing + HDR/IBI/HotJoin tags

# ----- Color hints (ARGB) ----------------------------------------------------
COLOR_DEFAULT  = 0
COLOR_BCAST    = 0xFFFFCC66
COLOR_CCC      = 0xFFFFAA44
COLOR_ENTDAA   = 0xFFAACCFF
COLOR_HOTJOIN  = 0xFFFFAACC
COLOR_IBI      = 0xFFFF7777
COLOR_HDR      = 0xFFFFAA00
COLOR_PAR_ERR  = 0xFFFF3333

I3C_BROADCAST_ADDR = 0x7E
HOT_JOIN_ADDR_W    = 0x04   # raw byte = (0x02 << 1) | W=0


# ----- CCC name lookup (MIPI I3C v1.1.1, broadcast 0x00-0x7F + direct 0x80-0xFE)
# Codes not in the table are flagged "?". Suffix "(D)" identifies direct CCCs
# in the same name string so the row can show direction at a glance.
CCC_NAMES = {
    # ----- Broadcast CCCs (0x00..0x7F) --------------------------------------
    0x00: "ENEC",       0x01: "DISEC",
    0x02: "ENTAS0",     0x03: "ENTAS1",
    0x04: "ENTAS2",     0x05: "ENTAS3",
    0x06: "RSTDAA",     0x07: "ENTDAA",
    0x08: "DEFTGTS",    0x09: "SETMWL",
    0x0A: "SETMRL",     0x0B: "ENTTM",
    0x0C: "SETBUSCON",  0x0D: "ENDXFER",
    0x12: "ENTHDR0",    0x13: "ENTHDR1",
    0x14: "ENTHDR2",    0x15: "ENTHDR3",
    0x16: "ENTHDR4",    0x17: "ENTHDR5",
    0x18: "ENTHDR6",    0x19: "ENTHDR7",
    0x1A: "SETXTIME",   0x1B: "SETAASA",
    0x1C: "RSTACT",     0x1D: "DEFGRPA",
    0x1E: "RSTGRPA",    0x1F: "MLANE",
    # ----- Direct CCCs (0x80..0xFE) -----------------------------------------
    0x80: "ENEC (D)",        0x81: "DISEC (D)",
    0x82: "ENTAS0 (D)",      0x83: "ENTAS1 (D)",
    0x84: "ENTAS2 (D)",      0x85: "ENTAS3 (D)",
    0x86: "RSTDAA (D)",      0x87: "SETDASA",
    0x88: "SETNEWDA",        0x89: "SETMWL (D)",
    0x8A: "SETMRL (D)",      0x8B: "GETMWL",
    0x8C: "GETMRL",          0x8D: "GETPID",
    0x8E: "GETBCR",          0x8F: "GETDCR",
    0x90: "GETSTATUS",       0x91: "GETACCCR",
    0x92: "ENDXFER (D)",     0x93: "SETBRGTGT",
    0x94: "GETMXDS",         0x95: "GETCAPS",
    0x96: "SETROUTE",        0x97: "SETXTIME (D)",
    0x98: "GETXTIME",        0x99: "RSTACT (D)",
    0x9A: "SETGRPA",         0x9B: "RSTGRPA (D)",
    0x9C: "MLANE (D)",
}


def _ccc_text(ccc):
    """Render the CCC byte as 'CCC <name> [B|D] (0xNN)'."""
    name = CCC_NAMES.get(ccc, "?")
    kind = "B" if ccc < 0x80 else "D"
    return f"CCC {name} [{kind}] (0x{ccc:02X})"


def _ccc_base(ccc):
    """Strip the ' (D)' suffix from a CCC name so direct/broadcast pairs
    share the same parameter schema lookup."""
    if ccc is None:
        return None
    name = CCC_NAMES.get(ccc, None)
    if name is None:
        return None
    return name.split(" ")[0]


# Parameter byte schemas. Keyed by the base CCC name (stripped of "(D)").
# Each entry is a list of labels indexed by data-byte position within the
# CCC's data phase. Returns None for byte indices outside the schema --
# those fall back to a generic "data" label.
CCC_PARAM_SCHEMA = {
    "ENEC":      ["EVT_MASK"],
    "DISEC":     ["EVT_MASK"],
    "SETMWL":    ["MWL_MSB", "MWL_LSB"],
    "SETMRL":    ["MRL_MSB", "MRL_LSB", "IBI_PAYLOAD_LEN"],
    "GETMWL":    ["MWL_MSB", "MWL_LSB"],
    "GETMRL":    ["MRL_MSB", "MRL_LSB", "IBI_PAYLOAD_LEN"],
    "SETNEWDA":  ["NEWDA"],
    "SETDASA":   ["NEWDA"],
    "GETPID":    ["PID[0]", "PID[1]", "PID[2]", "PID[3]", "PID[4]", "PID[5]"],
    "GETBCR":    ["BCR"],
    "GETDCR":    ["DCR"],
    "GETSTATUS": ["STATUS_MSB", "STATUS_LSB"],
    "GETMXDS":   ["MAX_RD_SPEED", "MAX_WR_SPEED",
                  "MAX_RD_TURN_MSB", "MAX_RD_TURN_MID", "MAX_RD_TURN_LSB"],
    "GETCAPS":   ["HDRCAP"],
    "SETXTIME":  ["XTIME_SUBCMD", "XTIME_BYTE1", "XTIME_BYTE2", "XTIME_BYTE3"],
    "RSTACT":    ["RSTACT_SUBCMD"],
    "ENDXFER":   ["ENDXFER_SUBCMD"],
    "DEFGRPA":   ["GRP_ADDR", "DCR_LSB", "DCR_MSB", "BCR_LSB", "BCR_MSB"],
    "SETBUSCON": ["BUSCON_CTX"],
    "ENTTM":     ["TM_SUBCMD"],
    "ENTAS0":    [],   # no params
    "ENTAS1":    [],
    "ENTAS2":    [],
    "ENTAS3":    [],
    "RSTDAA":    [],   # no params
    "ENTDAA":    [],   # handled separately (PID/BCR/DCR/NEWADDR flow)
    "SETAASA":   [],   # no params (broadcast)
    "SETROUTE":  [],   # variable; we just label generically
    "MLANE":     ["MLANE_SUBCMD"],
    "RSTGRPA":   [],   # no params (broadcast)
    "DEFTGTS":   [],   # variable-length payload; we just show raw bytes
}


def _ccc_param_label(ccc, byte_idx):
    """Return the schema label for the byte at index byte_idx within the
    current CCC's data phase, or None if no specific label applies."""
    base = _ccc_base(ccc)
    if base is None:
        return None
    schema = CCC_PARAM_SCHEMA.get(base)
    if not schema:
        return None
    if 0 <= byte_idx < len(schema):
        return schema[byte_idx]
    return None


def _odd_parity_bit(byte_val):
    """Compute the T-bit a controller must drive for a private/CCC write
    of byte_val: 1 if the byte has an even number of 1-bits, else 0
    (so the count of 1-bits including the T-bit is always odd)."""
    return 0 if (bin(byte_val & 0xFF).count("1") & 1) else 1


def _bcr_summary(bcr):
    """Render a BCR byte as a compact role + flags string.
       7:6 role; 5 ADV; 4 VTGT; 3 OFFL; 2 IBI+payload; 1 IBIReq; 0 LimSpd."""
    role = (bcr >> 6) & 0x3
    role_name = {0: "Tgt", 1: "Sec-Ctrl"}.get(role, f"R{role}")
    flags = []
    if bcr & 0x20: flags.append("ADV")
    if bcr & 0x10: flags.append("VTGT")
    if bcr & 0x08: flags.append("OFFL")
    if bcr & 0x04: flags.append("IBI+PL")
    if bcr & 0x02: flags.append("IBIReq")
    if bcr & 0x01: flags.append("LimSpd")
    return role_name + (" " + ",".join(flags) if flags else "")


def _event_mask_summary(mask):
    """Render an ENEC/DISEC event mask byte:
         bit 0 = ENINT (interrupts), bit 1 = ENCR (controller-role request),
         bit 3 = ENHJ (hot-join). Other bits are reserved."""
    flags = []
    if mask & 0x01: flags.append("INT")
    if mask & 0x02: flags.append("CR")
    if mask & 0x08: flags.append("HJ")
    return ",".join(flags) if flags else "none"


def _read_byte(scl, sda):
    """Sample 8 MSB-first data bits on rising SCL edges, then the 9th bit
    on the next rising SCL edge. Returns
       (byte_val, byte_start_t, ninth_edge, ninth_bit)
    or (None, None, None, None) on end-of-capture."""
    byte_val = 0
    byte_start_t = None
    for _ in range(8):
        edge = yield from wait_for(rising_edge(scl))
        if edge is None:
            return None, None, None, None
        if byte_start_t is None:
            byte_start_t = edge.t
        byte_val = (byte_val << 1) | edge.d[sda]
    ninth = yield from wait_for(rising_edge(scl))
    if ninth is None:
        return None, None, None, None
    return byte_val, byte_start_t, ninth, ninth.d[sda]


def _read_byte_first_bit_known(scl, sda, first_d, first_t):
    """Same as _read_byte but the SCL rising edge for bit 7 (MSB) has
    already been observed by the caller. first_d/first_t are that bit's
    SDA value and timestamp."""
    byte_val = first_d
    byte_start_t = first_t
    for _ in range(7):
        edge = yield from wait_for(rising_edge(scl))
        if edge is None:
            return None, None, None, None
        byte_val = (byte_val << 1) | edge.d[sda]
    ninth = yield from wait_for(rising_edge(scl))
    if ninth is None:
        return None, None, None, None
    return byte_val, byte_start_t, ninth, ninth.d[sda]


def _t_bit_label(byte_val, t_bit, is_read, check_parity):
    """Format the 9th-bit annotation text for a DATA byte (T-bit).
       Reads get 'more/end' semantics; writes get parity verdict."""
    if is_read:
        return f"T={t_bit} ({'more' if t_bit else 'end'})", False
    if not check_parity:
        return f"T={t_bit}", False
    expected = _odd_parity_bit(byte_val)
    if t_bit == expected:
        return f"T={t_bit} (par OK)", False
    return f"T={t_bit} (PAR ERR!)", True


def decode(params):
    scl = int(params.get("scl_channel", "0"))
    sda = int(params.get("sda_channel", "1"))
    check_parity   = params.get("check_parity",   "true").lower() == "true"
    assume_i3c_bus = params.get("assume_i3c_bus", "false").lower() == "true"

    print(f"I3C decoder: SCL=ch{scl}, SDA=ch{sda}, "
          f"check_parity={check_parity}, assume_i3c={assume_i3c_bus}",
          file=sys.stderr)

    # Persistent across transactions: once we see a 0x7E broadcast we know
    # the bus is I3C. Subsequent first-bytes that aren't 0x7E are then
    # interpreted as IBI / Hot-Join / private addressing rather than I2C.
    bus_is_i3c = assume_i3c_bus

    while True:
        # ----- Wait for a START condition (SDA falls while SCL high) --------
        start = yield from wait_for(all_of(falling_edge(sda), high(scl)))
        if start is None:
            return
        append(start.t, start.t, CH_NOTE,
               text="START", sample_type=SAMPLE_PACKET_START)

        # Per-transaction state -- reset at each START.
        first_byte      = True   # next byte read is the 7-bit address + R/W
        is_read         = False  # direction of the CURRENT sub-transfer
        expecting_ccc   = False  # next data byte is the CCC
        last_ccc        = None   # most recent CCC seen this transaction
        in_entdaa       = False  # inside ENTDAA's PID/BCR/DCR/NEWADDR flow
        entdaa_idx      = 0      # 0..8: byte position within ENTDAA cycle
        sub_byte_idx    = 0      # data byte index since last addr/RESTART
        addressed_tgt   = None   # 7-bit address of the current sub-target
                                 # (None if still in broadcast phase)

        # When the post-9th-bit "what's next?" wait observes an SCL rising
        # edge (a new byte beginning rather than STOP/RESTART), it has
        # already consumed bit 7 of that new byte. Stash it here so the
        # next byte-read picks up from bit 6.
        pre_consumed = None   # None or (sda_value, t)

        ended = False
        while not ended:
            # ----- Read one byte (with optional first-bit pre-consumed) -----
            if pre_consumed is not None:
                pd, pt = pre_consumed
                pre_consumed = None
                byte_val, byte_start_t, ninth_edge, ninth_bit = \
                    yield from _read_byte_first_bit_known(scl, sda, pd, pt)
            else:
                byte_val, byte_start_t, ninth_edge, ninth_bit = \
                    yield from _read_byte(scl, sda)
            if byte_val is None:
                return

            # ===============================================================
            # Address byte (first byte after START or RESTART)
            # ===============================================================
            if first_byte:
                addr = byte_val >> 1
                is_read = bool(byte_val & 1)
                rw = "R" if is_read else "W"

                if addr == I3C_BROADCAST_ADDR:
                    # Broadcast preamble. Promote the bus to I3C and expect
                    # a CCC on the next byte if this is a write.
                    bus_is_i3c    = True
                    addressed_tgt = None
                    expecting_ccc = (not is_read)
                    text  = f"BCAST 0x7E {rw}"
                    color = COLOR_BCAST
                elif byte_val == HOT_JOIN_ADDR_W and bus_is_i3c:
                    # Address 0x02 with W=0 on a known I3C bus = Hot-Join.
                    addressed_tgt = addr
                    text  = "HOT-JOIN 0x02 W"
                    color = COLOR_HOTJOIN
                    append(byte_start_t, ninth_edge.t, CH_NOTE,
                           text="HOT-JOIN", color=COLOR_HOTJOIN)
                elif bus_is_i3c and is_read and last_ccc is None:
                    # On an I3C bus, a START whose first byte is a target
                    # read (R=1) BEFORE any 0x7E in this transaction is
                    # almost certainly an IBI from that target.
                    addressed_tgt = addr
                    text  = f"IBI 0x{addr:02X} R"
                    color = COLOR_IBI
                    append(byte_start_t, ninth_edge.t, CH_NOTE,
                           text=f"IBI from 0x{addr:02X}", color=COLOR_IBI)
                elif bus_is_i3c:
                    # Post-broadcast private I3C sub-transfer (after a
                    # RESTART that followed a 0x7E+W + maybe CCC).
                    addressed_tgt = addr
                    text  = f"I3C 0x{addr:02X} {rw}"
                    color = COLOR_DEFAULT
                else:
                    # Bus not yet known to be I3C: treat as legacy I2C.
                    addressed_tgt = addr
                    text  = f"I2C 0x{addr:02X} {rw}"
                    color = COLOR_DEFAULT

                append(byte_start_t, ninth_edge.t, CH_ADDR,
                       text=text, color=color,
                       sample_type=SAMPLE_DATA, data=byte_val)
                # 9th bit on an address byte is ACK/NAK (open-drain).
                append(ninth_edge.t, ninth_edge.t, CH_ACKT,
                       text=("NAK" if ninth_bit else "ACK"),
                       sample_type=SAMPLE_DATA_ALT, data=ninth_bit)

                first_byte   = False
                sub_byte_idx = 0
                # NOTE: in_entdaa stays set across the RESTART+0x7E+R that
                # begins each target's probe; entdaa_idx is reset on
                # RESTART (below), not here.

            # ===============================================================
            # Data byte (CCC, ENTDAA payload, or generic data)
            # ===============================================================
            else:
                # -------- CCC byte (first data byte after 0x7E+W) ----------
                if expecting_ccc:
                    last_ccc      = byte_val
                    in_entdaa     = (byte_val == 0x07)
                    entdaa_idx    = 0
                    expecting_ccc = False
                    append(byte_start_t, ninth_edge.t, CH_CCC,
                           text=_ccc_text(byte_val), color=COLOR_CCC,
                           sample_type=SAMPLE_DATA, data=byte_val)
                    # Flag HDR-mode entries so the user knows SDR decoding
                    # stops being meaningful past this point.
                    if 0x12 <= byte_val <= 0x19:
                        append(byte_start_t, ninth_edge.t, CH_NOTE,
                               text=f"HDR ENTRY ({CCC_NAMES.get(byte_val,'?')})",
                               color=COLOR_HDR)
                    # CCC byte's 9th bit is a T-bit (controller write).
                    t_text, par_err = _t_bit_label(
                        byte_val, ninth_bit, False, check_parity)
                    append(ninth_edge.t, ninth_edge.t, CH_ACKT,
                           text=t_text,
                           color=(COLOR_PAR_ERR if par_err else 0),
                           sample_type=SAMPLE_DATA_ALT, data=ninth_bit)

                # -------- ENTDAA payload (PID/BCR/DCR/NEWADDR) -------------
                elif in_entdaa:
                    if entdaa_idx <= 5:
                        # PID[0..5] -- from target (read direction).
                        append(byte_start_t, ninth_edge.t, CH_CCC,
                               text=f"PID[{entdaa_idx}]=0x{byte_val:02X}",
                               color=COLOR_ENTDAA,
                               sample_type=SAMPLE_DATA, data=byte_val)
                        t_text, par_err = _t_bit_label(
                            byte_val, ninth_bit, True, check_parity)
                        append(ninth_edge.t, ninth_edge.t, CH_ACKT,
                               text=t_text,
                               sample_type=SAMPLE_DATA_ALT, data=ninth_bit)
                    elif entdaa_idx == 6:
                        # BCR -- from target, with decoded summary.
                        append(byte_start_t, ninth_edge.t, CH_CCC,
                               text=f"BCR 0x{byte_val:02X} ({_bcr_summary(byte_val)})",
                               color=COLOR_ENTDAA,
                               sample_type=SAMPLE_DATA, data=byte_val)
                        t_text, _ = _t_bit_label(
                            byte_val, ninth_bit, True, check_parity)
                        append(ninth_edge.t, ninth_edge.t, CH_ACKT,
                               text=t_text,
                               sample_type=SAMPLE_DATA_ALT, data=ninth_bit)
                    elif entdaa_idx == 7:
                        # DCR -- from target.
                        append(byte_start_t, ninth_edge.t, CH_CCC,
                               text=f"DCR 0x{byte_val:02X}",
                               color=COLOR_ENTDAA,
                               sample_type=SAMPLE_DATA, data=byte_val)
                        t_text, _ = _t_bit_label(
                            byte_val, ninth_bit, True, check_parity)
                        append(ninth_edge.t, ninth_edge.t, CH_ACKT,
                               text=t_text,
                               sample_type=SAMPLE_DATA_ALT, data=ninth_bit)
                    else:
                        # entdaa_idx == 8 -- NEWADDR byte from controller.
                        # Layout: bits 7..1 = 7-bit assigned DA;
                        #         bit 0    = odd parity over bits 7..1.
                        # 9th SCL bit is ACK/NAK from target (NOT a T-bit).
                        new_da = byte_val >> 1
                        addr_par = byte_val & 1
                        exp_par = 0 if (bin(new_da).count("1") & 1) else 1
                        par_ok = (addr_par == exp_par)
                        par_tag = "" if par_ok else " ADDR-PAR ERR!"
                        append(byte_start_t, ninth_edge.t, CH_CCC,
                               text=f"NEWADDR DA=0x{new_da:02X}{par_tag}",
                               color=(COLOR_PAR_ERR if not par_ok
                                      else COLOR_ENTDAA),
                               sample_type=SAMPLE_DATA, data=byte_val)
                        append(ninth_edge.t, ninth_edge.t, CH_ACKT,
                               text=("NAK" if ninth_bit else "ACK"),
                               sample_type=SAMPLE_DATA_ALT, data=ninth_bit)
                    entdaa_idx += 1

                # -------- Generic data byte (private xfer or CCC param) ----
                else:
                    # Raw byte on ch 2 always.
                    append(byte_start_t, ninth_edge.t, CH_DATA,
                           text=f"0x{byte_val:02X}",
                           sample_type=SAMPLE_DATA, data=byte_val)

                    # If the current sub-transfer is governed by a CCC,
                    # try to attach a field name on ch 1.
                    field = _ccc_param_label(last_ccc, sub_byte_idx)
                    if field is not None:
                        # Some fields get extra decoded context.
                        base = _ccc_base(last_ccc)
                        if field == "BCR":
                            extra = f" ({_bcr_summary(byte_val)})"
                        elif field == "EVT_MASK":
                            extra = f" ({_event_mask_summary(byte_val)})"
                        elif field == "NEWDA":
                            extra = f" (DA=0x{byte_val >> 1:02X})"
                        else:
                            extra = ""
                        append(byte_start_t, ninth_edge.t, CH_CCC,
                               text=f"{field}=0x{byte_val:02X}{extra}",
                               color=COLOR_CCC,
                               sample_type=SAMPLE_DATA, data=byte_val)

                    # 9th bit: T-bit on I3C bus; ACK/NAK on legacy I2C.
                    if bus_is_i3c:
                        t_text, par_err = _t_bit_label(
                            byte_val, ninth_bit, is_read, check_parity)
                        append(ninth_edge.t, ninth_edge.t, CH_ACKT,
                               text=t_text,
                               color=(COLOR_PAR_ERR if par_err else 0),
                               sample_type=SAMPLE_DATA_ALT, data=ninth_bit)
                    else:
                        append(ninth_edge.t, ninth_edge.t, CH_ACKT,
                               text=("NAK" if ninth_bit else "ACK"),
                               sample_type=SAMPLE_DATA_ALT, data=ninth_bit)

                    sub_byte_idx += 1

            # ===============================================================
            # After the 9th bit, classify STOP / RESTART / next byte.
            # STOP and RESTART are SDA edges *while SCL is high*; a normal next
            # byte is just an SCL rising edge with SDA stable at bit 7. Lumping
            # the SCL rising edge into the same any_of() as the SDA edges and
            # disambiguating by level misreads every next byte (bit 7 == 1 ->
            # false STOP, bit 7 == 0 -> false RESTART; the "next byte" branch is
            # unreachable). Consume the 9th bit's SCL fall and the next SCL rise
            # first, then watch the plateau for an SDA edge (STOP/RESTART) vs the
            # SCL fall (clean data bit) so the clock edge can't be mistaken for
            # an SDA event. See I2C_Decoder for the full rationale.
            scl_fell = yield from wait_for(falling_edge(scl))
            if scl_fell is None:
                return
            next_rise = yield from wait_for(rising_edge(scl))
            if next_rise is None:
                return
            cand_d = next_rise.d[sda]    # provisional bit 7 of next byte
            cand_t = next_rise.t

            ev = yield from wait_for(any_of(
                all_of(rising_edge(sda),  high(scl)),   # STOP
                all_of(falling_edge(sda), high(scl)),   # Repeated START
                falling_edge(scl),                      # clean data bit
            ))
            if ev is None:
                return

            if ev.d[scl] == 0:
                # ----- next byte ------------------------------------------
                # next_rise clocked bit 7 of the next byte; stash it so the
                # next iteration picks up from bit 6.
                pre_consumed = (cand_d, cand_t)

            elif ev.d[sda] == 1:
                # ----- STOP -----------------------------------------------
                append(ev.t, ev.t, CH_NOTE,
                       text="STOP", sample_type=SAMPLE_PACKET_END)
                ended = True

            else:
                # ----- RESTART --------------------------------------------
                append(ev.t, ev.t, CH_NOTE, text="RESTART",
                       sample_type=SAMPLE_DATA)
                first_byte    = True
                sub_byte_idx  = 0
                expecting_ccc = False   # CCC already consumed if it existed
                addressed_tgt = None
                if in_entdaa:
                    # ENTDAA cycles probe one target per RESTART iteration.
                    entdaa_idx = 0
                # last_ccc and bus_is_i3c persist -- a direct CCC's
                # parameter bytes appear after RESTART, and the bus is
                # still I3C across sub-transfers.
