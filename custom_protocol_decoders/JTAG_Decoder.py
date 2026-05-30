# DECODER_MYNAME = "JTAG"
#
# Decodes IEEE 1149.1 JTAG on four digital channels: TCK, TMS, TDI, TDO.
#
# PARAM: tck_channel | TCK Channel | digital_channel |  |
# PARAM: tms_channel | TMS Channel | digital_channel |  |
# PARAM: tdi_channel | TDI Channel | digital_channel |  |
# PARAM: tdo_channel | TDO Channel | digital_channel |  |
#
# Output rows:
#   ch 0 : TAP state name on every TCK rising edge transition
#   ch 1 : Shifted-IR bitstring (LSB first), reported on exit
#   ch 2 : Shifted-DR bitstring (LSB first), reported on exit
#
# Protocol notes:
#   - TDI / TMS are valid on TCK rising edge.
#   - TDO is updated on TCK falling edge.
#   - The TAP state machine has 16 states. We track it explicitly and
#     emit state names per edge as well as collected bitstrings for
#     SHIFT_IR / SHIFT_DR runs.

import sys

from active_pro import (
    append, wait_for,
    rising_edge,
)


SAMPLE_DATA         = 0
SAMPLE_DATA_ALT     = 1
SAMPLE_PACKET_START = 2
SAMPLE_PACKET_END   = 3

CH_STATE = 0
CH_IR    = 1
CH_DR    = 2

# TAP state machine. (current_state, tms_value) -> next_state
TLR  = "Test-Logic-Reset"
RTI  = "Run-Test/Idle"
SDS  = "Select-DR"; CDR = "Capture-DR"; SHDR = "Shift-DR"; E1DR = "Exit1-DR"
PDR  = "Pause-DR"; E2DR = "Exit2-DR"; UDR  = "Update-DR"
SIS  = "Select-IR"; CIR = "Capture-IR"; SHIR = "Shift-IR"; E1IR = "Exit1-IR"
PIR  = "Pause-IR"; E2IR = "Exit2-IR"; UIR  = "Update-IR"

NEXT = {
    (TLR,  0): RTI,  (TLR,  1): TLR,
    (RTI,  0): RTI,  (RTI,  1): SDS,
    (SDS,  0): CDR,  (SDS,  1): SIS,
    (CDR,  0): SHDR, (CDR,  1): E1DR,
    (SHDR, 0): SHDR, (SHDR, 1): E1DR,
    (E1DR, 0): PDR,  (E1DR, 1): UDR,
    (PDR,  0): PDR,  (PDR,  1): E2DR,
    (E2DR, 0): SHDR, (E2DR, 1): UDR,
    (UDR,  0): RTI,  (UDR,  1): SDS,
    (SIS,  0): CIR,  (SIS,  1): TLR,
    (CIR,  0): SHIR, (CIR,  1): E1IR,
    (SHIR, 0): SHIR, (SHIR, 1): E1IR,
    (E1IR, 0): PIR,  (E1IR, 1): UIR,
    (PIR,  0): PIR,  (PIR,  1): E2IR,
    (E2IR, 0): SHIR, (E2IR, 1): UIR,
    (UIR,  0): RTI,  (UIR,  1): SDS,
}


def decode(params):
    tck = int(params.get("tck_channel", "0"))
    tms = int(params.get("tms_channel", "1"))
    tdi = int(params.get("tdi_channel", "2"))
    tdo = int(params.get("tdo_channel", "3"))

    print(f"JTAG: TCK={tck} TMS={tms} TDI={tdi} TDO={tdo}", file=sys.stderr)

    # Start in the safe state -- five TCK cycles with TMS=1 always reaches
    # Test-Logic-Reset, so we'll converge regardless of initial guess.
    state = TLR

    # Shift accumulators (LSB first into list).
    ir_bits_in  = []   # TDI bits captured during SHIFT_IR
    ir_bits_out = []   # TDO bits during SHIFT_IR
    dr_bits_in  = []
    dr_bits_out = []
    ir_t_start  = None
    dr_t_start  = None

    while True:
        edge = yield from wait_for(rising_edge(tck))
        if edge is None:
            return

        tms_bit = edge.d[tms]
        tdi_bit = edge.d[tdi]
        tdo_bit = edge.d[tdo]

        # Collect a shift bit BEFORE the state transition (TDI/TDO valid on
        # this rising edge belong to the current SHIFT_* state).
        if state == SHIR:
            if ir_t_start is None:
                ir_t_start = edge.t
            ir_bits_in.append(tdi_bit)
            ir_bits_out.append(tdo_bit)
        elif state == SHDR:
            if dr_t_start is None:
                dr_t_start = edge.t
            dr_bits_in.append(tdi_bit)
            dr_bits_out.append(tdo_bit)

        next_state = NEXT[(state, tms_bit)]

        # On exiting a shift state, flush the bitstring.
        if state == SHIR and next_state != SHIR:
            if ir_bits_in:
                ir_text = _bits_to_hex(ir_bits_in)
                append(ir_t_start, edge.t, CH_IR,
                       text=f"IR={ir_text} ({len(ir_bits_in)}b)",
                       data=_bits_to_int(ir_bits_in) & 0xFFFFFFFF,
                       sample_type=SAMPLE_DATA, color=0xFFFFCC88)
            ir_bits_in.clear(); ir_bits_out.clear(); ir_t_start = None
        elif state == SHDR and next_state != SHDR:
            if dr_bits_in:
                dr_in_text  = _bits_to_hex(dr_bits_in)
                dr_out_text = _bits_to_hex(dr_bits_out)
                append(dr_t_start, edge.t, CH_DR,
                       text=f"DR TDI={dr_in_text} TDO={dr_out_text} "
                            f"({len(dr_bits_in)}b)",
                       data=_bits_to_int(dr_bits_in) & 0xFFFFFFFF,
                       sample_type=SAMPLE_DATA, color=0xFF88FFAA)
            dr_bits_in.clear(); dr_bits_out.clear(); dr_t_start = None

        # Annotate the state transition itself (one short tag per edge).
        if next_state != state:
            append(edge.t, edge.t, CH_STATE,
                   text=next_state, sample_type=SAMPLE_DATA_ALT)

        state = next_state


def _bits_to_int(bits):
    """LSB-first bit list -> integer."""
    v = 0
    for i, b in enumerate(bits):
        v |= (b & 1) << i
    return v


def _bits_to_hex(bits):
    v = _bits_to_int(bits)
    nibbles = (len(bits) + 3) // 4
    return f"0x{v:0{nibbles}X}"
