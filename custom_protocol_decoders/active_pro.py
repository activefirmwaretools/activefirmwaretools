"""
active_pro -- Active-Pro user-defined Python protocol decoder runtime.

This module ships with the ActiveProDebugger app.  User decoder scripts
import from it and never modify it.  A decoder script has the shape::

    from active_pro import (
        append, wait_for, wait_time,
        rising_edge, falling_edge, high, low,
        voltage_rises, voltage_falls,
        voltage_above, voltage_below, voltage_between,
        all_of, any_of, not_,
    )

    def decode(params):
        while True:
            m = yield from wait_for(falling_edge(0))       # UART start bit
            if m is None:
                return
            byte_val = 0
            for bit in range(8):
                m = yield from wait_time(1.0 / 115200)
                if m is None: return
                byte_val |= (m.d[0] << bit)
            append(m.t, m.t, channel=0, text=f"0x{byte_val:02X}")

The C++ host (DecoderArenaExport + DecoderInstance + CustomDecoderThread,
Phase 8 post-capture model) finalizes the capture, snapshots write_pos +
session metadata into the SHM control header, then spawns Python.  The
buffer is constant for the lifetime of the subprocess; wait_for / wait_time
return None once the cursor catches up to write_pos, letting decode() exit
cleanly.

Byte-for-byte layouts are mirrored from the C++ headers:
  decodercontrolheader.h  -> _ControlHeader  (56 B, version 4)
  annotationring.h        -> _RingHeader (64 B), _RECORD_FMT (160 B)
  logicsamplebuffer.h     -> SampleLogicType  (16 B, native)
  analogsamplebuffer.h    -> AnalogSampleLevel0Type (stride read from
                             AP_ANALOG_RECORD_SIZE / control header)

If the C++ side bumps AP_CONTROL_HEADER_VERSION or
AP_ANNOTATION_RING_VERSION, this module raises at startup -- the script
won't silently misread a newer host.
"""

from __future__ import annotations

import importlib
import importlib.util
import io
import json
import mmap
import os
import struct
import sys
import threading
import time
from typing import Any, Callable, Generator, Optional


# ---------------------------------------------------------------------------
# Layout constants -- KEEP IN SYNC WITH C++ HEADERS.
# ---------------------------------------------------------------------------

# decodercontrolheader.h: AP_CONTROL_HEADER_MAGIC = 'APCH' little-endian.
_CTRL_MAGIC = 0x48435041
_CTRL_VERSION = 4
_CTRL_SIZE = 56

# Field offsets within DecoderControlHeader (little-endian).
_OFF_CTRL_MAGIC = 0
_OFF_CTRL_VERSION = 4
_OFF_CTRL_RECORD_SIZE = 8
_OFF_CTRL_IS_ULTRA = 12
_OFF_CTRL_TIMESTAMP_FREQ = 16
_OFF_CTRL_WRITE_POS = 24
_OFF_CTRL_ANALOG_USED = 32
_OFF_CTRL_ANALOG_TOTAL = 36
_OFF_CTRL_STEPS_PER_RECORD = 40
# reserved0 (u32) at 44
_OFF_CTRL_CAPTURE_END_TICK = 48     # quint64: absolute final capture timestamp

# annotationring.h: AP_ANNOTATION_RING_MAGIC = 'APAR' little-endian.
_RING_MAGIC = 0x52415041
_RING_VERSION = 2
_RING_HEADER_SIZE = 64
_RING_RECORD_SIZE = 160     # AnnotationRecord; 156 declared + 4 trailing pad

_OFF_RING_MAGIC = 0
_OFF_RING_VERSION = 4
_OFF_RING_RECORD_SIZE = 8
_OFF_RING_RECORD_COUNT = 12
_OFF_RING_WRITE_POS = 16
_OFF_RING_READ_POS = 24
_OFF_RING_DECODER_SAMPLE_POS = 32   # quint64: Python writes current logic sub-sample index
_OFF_RING_DECODER_ANALOG_POS = 40   # quint64: Python writes current analog record index

# Record packing:
#   timestamp_start  u64    +0
#   timestamp_end    u64    +8
#   channel          u32   +16
#   sample_type      u32   +20
#   color            u32   +24
#   data             u32   +28
#   text_len         u16   +32
#   reserved0        u16   +34
#   text             120s  +36
#   (4 bytes trailing pad to 160; struct module won't add it with '<',
#    so we pack the leading 156 bytes and write a fixed 4-byte zero tail.)
_RECORD_HEAD_FMT = "<QQIIIIHH120s"
_RECORD_HEAD_SIZE = struct.calcsize(_RECORD_HEAD_FMT)
assert _RECORD_HEAD_SIZE == 156, f"AnnotationRecord head fmt is {_RECORD_HEAD_SIZE}, expected 156"
_RECORD_TAIL_PAD = b"\x00\x00\x00\x00"          # 4 bytes, total record = 160

# logicsamplebuffer.h: SampleLogicType.  Fixed 16 B layout.
#   qint64 Timestamp; quint8 [Min, Max, Left, Right, V1, V2, V3, Triggered]
_LOGIC_FMT = "<q8B"
_LOGIC_SIZE = struct.calcsize(_LOGIC_FMT)
assert _LOGIC_SIZE == 16, f"SampleLogicType fmt is {_LOGIC_SIZE}, expected 16"

# Sub-sample timing within a logic record (nanoseconds), per plan rev (5)
# Step 0.6: Active-Pro packs 4 sub-samples at 0/2/4/6 ns offsets; Ultra
# packs 5 at 0/2/4/6/8 ns.  Sub-sample spacing is hardware-fixed at 2 ns
# regardless of the bucket-level timestamp_frequency.
_SUBSAMPLE_PRO_OFFSETS_NS = (0.0, 2.0, 4.0, 6.0)
_SUBSAMPLE_ULTRA_OFFSETS_NS = (0.0, 2.0, 4.0, 6.0, 8.0)
_NS_PER_S = 1.0e-9


# ---------------------------------------------------------------------------
# Helpers: read fields from a mmap.
# ---------------------------------------------------------------------------

def _u32(mm: mmap.mmap, off: int) -> int:
    return struct.unpack_from("<I", mm, off)[0]


def _u64(mm: mmap.mmap, off: int) -> int:
    return struct.unpack_from("<Q", mm, off)[0]


def _f64(mm: mmap.mmap, off: int) -> float:
    return struct.unpack_from("<d", mm, off)[0]


# ---------------------------------------------------------------------------
# Platform-aware SHM open.
#
# Windows: child receives the tagname via AP_<TAG>_NAME, opens via
#          mmap.mmap(-1, size, tagname=name).  CreateFileMapping has already
#          run host-side, so the named section exists.
#
# Unix: child receives an inherited fd via AP_<TAG>_FD.  We mmap the fd
#       directly.
# ---------------------------------------------------------------------------

def _open_arena(env_prefix: str, writable: bool = False) -> mmap.mmap:
    """Open the SHM region described by AP_<env_prefix>_{NAME,FD,SIZE}."""
    size_s = os.environ.get(env_prefix + "_SIZE")
    if not size_s:
        raise RuntimeError(f"{env_prefix}_SIZE not set in environment")
    size = int(size_s)

    prot = mmap.ACCESS_WRITE if writable else mmap.ACCESS_READ

    if sys.platform == "win32":
        name = os.environ.get(env_prefix + "_NAME") or ""
        if not name:
            raise RuntimeError(f"{env_prefix}_NAME required on Windows")
        return mmap.mmap(-1, size, tagname=name, access=prot)

    fd_s = os.environ.get(env_prefix + "_FD")
    if fd_s is None or fd_s == "":
        raise RuntimeError(f"{env_prefix}_FD required on Unix")
    fd = int(fd_s)
    return mmap.mmap(fd, size, mmap.MAP_SHARED,
                     mmap.PROT_WRITE | mmap.PROT_READ if writable else mmap.PROT_READ)


# ---------------------------------------------------------------------------
# Control header view.  One per logical arena (Logic, Analog, ...), all
# living in the single combined region named by AP_CONTROL_NAME at offset
# AP_<TAG>_CTRL_OFFSET.
#
# Phase 8 (Step 8.10) single-mode: write_pos is a one-shot snapshot taken
# by DecoderArenaExport::onCaptureStart before the subprocess spawns. The
# streaming-era evict_epoch / last_evict_delta / capture_active fields are
# gone from the C++ header.
# ---------------------------------------------------------------------------

class _ControlHeader:
    """Read-only view over a DecoderControlHeader inside the control SHM."""

    __slots__ = ("write_pos", "record_size", "is_ultra",
                 "timestamp_frequency", "analog_channels_used",
                 "analog_channels_total", "steps_per_record",
                 "capture_end_tick")

    def __init__(self, mm: mmap.mmap, base_offset: int):
        magic = _u32(mm, base_offset + _OFF_CTRL_MAGIC)
        if magic != _CTRL_MAGIC:
            raise RuntimeError(
                f"control header magic mismatch: 0x{magic:08X} != 0x{_CTRL_MAGIC:08X}")
        version = _u32(mm, base_offset + _OFF_CTRL_VERSION)
        if version != _CTRL_VERSION:
            raise RuntimeError(
                f"control header version mismatch: host={version} runtime={_CTRL_VERSION}")
        # Every field is read once at construction; the host wrote them all
        # before this process spawned and they never change.
        self.record_size           = _u32(mm, base_offset + _OFF_CTRL_RECORD_SIZE)
        self.is_ultra              = _u32(mm, base_offset + _OFF_CTRL_IS_ULTRA)
        self.timestamp_frequency   = _f64(mm, base_offset + _OFF_CTRL_TIMESTAMP_FREQ)
        self.write_pos             = _u64(mm, base_offset + _OFF_CTRL_WRITE_POS)
        self.analog_channels_used  = _u32(mm, base_offset + _OFF_CTRL_ANALOG_USED)
        self.analog_channels_total = _u32(mm, base_offset + _OFF_CTRL_ANALOG_TOTAL)
        self.steps_per_record      = _u32(mm, base_offset + _OFF_CTRL_STEPS_PER_RECORD)
        self.capture_end_tick      = _u64(mm, base_offset + _OFF_CTRL_CAPTURE_END_TICK)


# ---------------------------------------------------------------------------
# Sample objects.
# ---------------------------------------------------------------------------

# Precomputed bit-decomposition for all 256 byte values.  Builds the .d view
# in a single tuple lookup instead of an 8-iteration list comprehension per
# LogicSample.  Tuple (not list) so it's shared safely between samples.
_BIT_TUPLES = tuple(
    tuple((b >> i) & 1 for i in range(8)) for b in range(256)
)


class LogicSample:
    """One logic sub-sample: timestamp + 8 channel bits."""

    __slots__ = ("t", "bits", "d")

    def __init__(self, t_seconds: float, byte_val: int):
        self.t = t_seconds
        self.bits = byte_val
        self.d = _BIT_TUPLES[byte_val]


class AnalogSample:
    """One analog sample row: timestamp + per-channel voltages."""

    __slots__ = ("t", "a")

    def __init__(self, t_seconds: float, channels: list):
        self.t = t_seconds
        self.a = channels


# ---------------------------------------------------------------------------
# LogicAccessor -- indexable view across all logic sub-samples in the arena.
#
# Phase 8 (Step 8.10) single-mode: the buffer is finalized before this
# process spawned.  write_pos is constant.  refresh() exists so the existing
# user-API contract still works, but it just returns the cached count.
# ---------------------------------------------------------------------------

class LogicAccessor:
    """Indexable view over the per-step logic samples in the captured arena."""

    def __init__(self, data_mm: mmap.mmap, ctrl: _ControlHeader):
        self._mm = data_mm
        self._ctrl = ctrl
        self._record_size = ctrl.record_size or _LOGIC_SIZE
        self._tick_to_s = (1.0 / ctrl.timestamp_frequency
                          if ctrl.timestamp_frequency > 0 else 0.0)
        self._steps = ctrl.steps_per_record or (5 if ctrl.is_ultra else 4)
        self._offsets_ns = (_SUBSAMPLE_ULTRA_OFFSETS_NS
                            if ctrl.is_ultra else _SUBSAMPLE_PRO_OFFSETS_NS)
        if len(self._offsets_ns) < self._steps:
            # Future hardware with more steps -- fall back to linear 2 ns.
            self._offsets_ns = tuple(2.0 * k for k in range(self._steps))

        # Byte offsets within a 16-byte logic record for each sub-sample step.
        # Record layout (logicsamplebuffer.h SampleLogicType):
        #   +0..+7   qint64 Timestamp
        #   +8       Min      +9   Max
        #   +10      Left     +11  Right
        #   +12      V1       +13  V2       +14  V3       +15  Triggered
        # Pro packs four sub-samples in order VL, V1, V2, VR.
        # Ultra packs five:                   VL, V1, V2, V3, VR.
        if ctrl.is_ultra:
            self._step_byte_offsets = (10, 12, 13, 14, 11)
        else:
            self._step_byte_offsets = (10, 12, 13, 11)

        # Snapshot the total sub-sample count once. write_pos is constant
        # for the lifetime of this process.
        self._total_sub_samples = (ctrl.write_pos // self._record_size) * self._steps

    # ----- Public properties ------------------------------------------------

    @property
    def is_ultra(self) -> bool:
        return bool(self._ctrl.is_ultra)

    @property
    def steps_per_record(self) -> int:
        return self._steps

    @property
    def sample_rate(self) -> float:
        """Effective per-sub-sample rate in samples/second."""
        return 1.0e9 / 2.0

    # ----- Sized + indexable ------------------------------------------------

    def refresh(self) -> int:
        """Return the (fixed) total sub-sample count.

        Phase 8 single-mode: write_pos was snapshotted before this process
        spawned and the buffer is read-only for the run.  This is a no-op
        accessor kept for API compatibility with decoder scripts that call
        it inside their loops.
        """
        return self._total_sub_samples

    def __len__(self) -> int:
        return self._total_sub_samples

    def __getitem__(self, idx: int) -> LogicSample:
        if idx < 0:
            idx += self._total_sub_samples
        if idx < 0 or idx >= self._total_sub_samples:
            raise IndexError(f"sample {idx} out of range (total={self._total_sub_samples})")
        rec_idx, step = divmod(idx, self._steps)
        off = rec_idx * self._record_size
        ts, vmin, vmax, vleft, vright, v1, v2, v3, _trig = struct.unpack_from(
            _LOGIC_FMT, self._mm, off)
        if self.is_ultra:
            # Ultra packs five sub-samples per record: VL, V1, V2, V3, VR.
            byte_val = (vleft, v1, v2, v3, vright)[step]
        else:
            # Pro packs four: VL, V1, V2, VR.
            byte_val = (vleft, v1, v2, vright)[step]
        t_seconds = (ts * self._tick_to_s) + self._offsets_ns[step] * _NS_PER_S
        return LogicSample(t_seconds, byte_val)


# ---------------------------------------------------------------------------
# AnalogAccessor -- one record per timestamp, stride from control header.
# ---------------------------------------------------------------------------

class AnalogAccessor:
    """Indexable view over the analog L0 arena."""

    def __init__(self, data_mm: mmap.mmap, ctrl: _ControlHeader):
        self._mm = data_mm
        self._ctrl = ctrl
        self._record_size = ctrl.record_size
        if self._record_size <= 0:
            raise RuntimeError("analog arena record_size not set by host")
        self._channels_total = ctrl.analog_channels_total or 8
        self._channels_used = ctrl.analog_channels_used or self._channels_total
        self._tick_to_s = (1.0 / ctrl.timestamp_frequency
                          if ctrl.timestamp_frequency > 0 else 0.0)
        # Per-record packing: timestamp (qint64) + N floats + Triggered byte
        # + trailing pad.  We use a fmt that consumes only the meaningful
        # leading bytes; the trailing padding is implicit in the stride.
        self._fmt = "<q" + ("f" * self._channels_total)
        self._fmt_size = struct.calcsize(self._fmt)

        self._total_records = ctrl.write_pos // self._record_size

    @property
    def channels_used(self) -> int:
        return self._channels_used

    @property
    def channels_total(self) -> int:
        return self._channels_total

    def refresh(self) -> int:
        """Return the (fixed) total record count.  Single-mode no-op."""
        return self._total_records

    def __len__(self) -> int:
        return self._total_records

    def __getitem__(self, idx: int) -> AnalogSample:
        if idx < 0:
            idx += self._total_records
        if idx < 0 or idx >= self._total_records:
            raise IndexError(f"analog sample {idx} out of range")
        off = idx * self._record_size
        unpacked = struct.unpack_from(self._fmt, self._mm, off)
        ts = unpacked[0]
        chans = list(unpacked[1:1 + self._channels_total])
        return AnalogSample(ts * self._tick_to_s, chans)


# ---------------------------------------------------------------------------
# Annotation output (the ring buffer the C++ DecoderInstance drains).
# ---------------------------------------------------------------------------

class _Output:
    """Producer-side view of the AnnotationRingBuffer."""

    def __init__(self, mm: mmap.mmap, ch_base: int,
                 timestamp_frequency: float):
        magic = _u32(mm, _OFF_RING_MAGIC)
        if magic != _RING_MAGIC:
            raise RuntimeError(
                f"ring magic mismatch: 0x{magic:08X} != 0x{_RING_MAGIC:08X}")
        version = _u32(mm, _OFF_RING_VERSION)
        if version != _RING_VERSION:
            raise RuntimeError(
                f"ring version mismatch: host={version} runtime={_RING_VERSION}")
        self._mm = mm
        self._record_size = _u32(mm, _OFF_RING_RECORD_SIZE)
        # C++ writes sizeof(AnnotationRecord) = 160 (packed fields 156 + 4 tail
        # pad).  Validate against the full stride constant, not the struct size.
        if self._record_size != _RING_RECORD_SIZE:
            raise RuntimeError(
                f"ring record_size {self._record_size} != expected {_RING_RECORD_SIZE}")
        self._record_count = _u32(mm, _OFF_RING_RECORD_COUNT)
        if self._record_count == 0:
            raise RuntimeError("ring record_count is zero")
        self._ch_base = ch_base
        self._tps = (timestamp_frequency if timestamp_frequency > 0
                    else 1.0e9)  # fallback; should never trip in practice

    def _write_pos(self) -> int:
        return _u64(self._mm, _OFF_RING_WRITE_POS)

    def _read_pos(self) -> int:
        return _u64(self._mm, _OFF_RING_READ_POS)

    def _publish_write_pos(self, new_val: int) -> None:
        struct.pack_into("<Q", self._mm, _OFF_RING_WRITE_POS, new_val)

    def append(self, t_start: float, t_end: float, channel: int,
               text: str, color: int = 0,
               sample_type: int = 0, data: int = 0) -> None:
        """Push one annotation onto the ring.  Blocks if full."""
        wp = self._write_pos()
        # Spin-wait if full; the C++ consumer advances read_pos as it drains
        # via DecoderInstance::drainOnce() polled every ~50 ms.
        while wp - self._read_pos() >= self._record_count:
            time.sleep(0.0005)
        slot = wp % self._record_count
        off = _RING_HEADER_SIZE + slot * _RING_RECORD_SIZE

        ts_start_ticks = int(round(t_start * self._tps))
        ts_end_ticks = int(round(t_end * self._tps))
        out_channel = (self._ch_base + int(channel)) & 0xFFFFFFFF

        text_bytes = text.encode("utf-8", errors="replace")[:120]
        struct.pack_into(
            _RECORD_HEAD_FMT, self._mm, off,
            ts_start_ticks & 0xFFFFFFFFFFFFFFFF,
            ts_end_ticks & 0xFFFFFFFFFFFFFFFF,
            out_channel,
            int(sample_type) & 0xFFFFFFFF,
            int(color) & 0xFFFFFFFF,
            int(data) & 0xFFFFFFFF,
            len(text_bytes) & 0xFFFF,
            0,
            text_bytes,
        )
        # Zero the trailing 4-byte pad slot so a future producer that
        # reuses this slot starts from a clean tail.
        self._mm[off + _RECORD_HEAD_SIZE:off + _RING_RECORD_SIZE] = _RECORD_TAIL_PAD

        # Release-store of write_pos; consumer's acquire-load of write_pos
        # pairs with this.  CPython's GIL + x86 store ordering make this
        # safe in practice; the C++ side reads via std::atomic acquire.
        self._publish_write_pos(wp + 1)


# Module-global output handle, set by _run() before the decode generator
# starts.  User scripts call the bare append() function.
_OUTPUT: Optional[_Output] = None

# Current decoder progress positions, written to the ring header every drive
# loop iteration so C++ DisplayProgress() can compute a live percentage.
_decoder_sample_pos: int = 0
_decoder_analog_pos: int = 0


def update_analog_progress(pos: int) -> None:
    """Report the analog record index the decoder is currently processing.

    Call this periodically inside any analog-processing loop so the C++
    status bar can show accurate progress when the decoder uses analog data.
    """
    global _decoder_analog_pos
    _decoder_analog_pos = pos


# Set by _monitor_stdin_for_stop() thread when C++ sends "stop\n" or closes stdin.
_stop_requested: threading.Event = threading.Event()


def _monitor_stdin_for_stop() -> None:
    """Background thread: watch stdin for 'stop' or EOF and set _stop_requested."""
    try:
        while True:
            line = sys.stdin.readline()
            if not line or line.strip() == "stop":
                _stop_requested.set()
                return
    except Exception:
        _stop_requested.set()


# Module-global output ring mmap, set by _run() before decode() starts. A
# daemon thread (see _publish_progress_periodically) wakes every
# _PROGRESS_INTERVAL_S and writes the latest _I / _J cursor positions into
# the ring header so the C++ status bar advances at a steady cadence
# independent of per-iteration decoder cost.
_RING_MM: Optional["mmap.mmap"] = None

# Cadence for the background progress publisher.
_PROGRESS_INTERVAL_S = 0.2

# Set by _run() to stop the background publisher when the decoder exits.
_progress_stop: threading.Event = threading.Event()


def _publish_progress(sample_pos: int, analog_pos: int) -> None:
    """Write current cursor positions into the annotation ring header.
    Unthrottled -- the caller is responsible for cadence. Used by the
    background publisher and for the terminal flush at decoder exit."""
    mm = _RING_MM
    if mm is None:
        return
    try:
        struct.pack_into("<Q", mm, _OFF_RING_DECODER_SAMPLE_POS, sample_pos)
        struct.pack_into("<Q", mm, _OFF_RING_DECODER_ANALOG_POS, analog_pos)
    except Exception:
        pass


def _publish_progress_periodically() -> None:
    """Daemon thread: every _PROGRESS_INTERVAL_S, snapshot the cursors and
    write them to SHM. Reads of _I / _J / _decoder_*_pos are unsynchronized;
    CPython int loads are atomic under the GIL (no torn reads), and a
    snapshot a few iterations stale is fine for a percentage display.
    CPython's switch-interval (5 ms default) preempts the wait_for hot loop
    often enough that this thread gets scheduled on time."""
    while not _progress_stop.wait(_PROGRESS_INTERVAL_S):
        _publish_progress(max(_I, _decoder_sample_pos),
                          max(_J, _decoder_analog_pos))


def append(t_start: float, t_end: float, channel: int,
           text: str, color: int = 0,
           sample_type: int = 0, data: int = 0) -> None:
    """Emit one annotation record.  channel is added to AP_OUT_CH_BASE."""
    if _OUTPUT is None:
        raise RuntimeError("active_pro.append() called outside _run() context")
    _OUTPUT.append(t_start, t_end, channel, text, color, sample_type, data)


# ---------------------------------------------------------------------------
# Wait API -- user-facing.
#
# Mental model: the decoder has one notion of "current time". Every wait
# operation advances time. There are exactly two operations:
#
#   yield from wait_for(condition)   -- advance until the condition is true
#   yield from wait_time(seconds)    -- advance by a fixed amount of time
#
# Both return a Moment -- a snapshot of all signals at the new current time:
#   m.t        -- timestamp in seconds (float)
#   m.d[0..7]  -- digital channel bits (tuple of 0/1)
#   m.bits     -- digital channels packed as a byte
#   m.a[0..7]  -- analog channel voltages (list of float)
# Return None on end-of-capture or timeout.
#
# Conditions are built from these factories:
#   rising_edge(channel)                                  -- digital rising
#   falling_edge(channel)                                 -- digital falling
#   high(channel)                                         -- digital level == 1
#   low(channel)                                          -- digital level == 0
#   voltage_rises(channel, threshold, hysteresis=0.0)     -- analog rising
#   voltage_falls(channel, threshold, hysteresis=0.0)     -- analog falling
#   voltage_above(channel, threshold)                     -- analog level above
#   voltage_below(channel, threshold)                     -- analog level below
#   voltage_between(channel, low, high)                   -- analog window
#
# Combinators:
#   all_of(c1, c2, ...)   -- AND  (every sub-condition true at the same moment)
#   any_of(c1, c2, ...)   -- OR
#   not_(c)               -- NOT
#
# Channels:
#   For digital functions, channel is 0..7 (logic bit).
#   For analog functions, channel is 0..7 (Pro: 0..3 valid; Ultra: 0..7).
# ---------------------------------------------------------------------------

# --- Current-time model ----------------------------------------------------
#
# The decoder has exactly one notion of "now": _T, a time in seconds. Signals
# are sample-and-hold -- the value at _T is the value of the most recent stored
# sample at or before _T, held until the next stored sample. Captures are
# transition-compressed: nothing is stored while a signal sits still, so an
# arbitrary amount of time (and many bit periods) can pass between two adjacent
# stored samples. Driving off _T instead of a sample index is what lets
# wait_time() step across those gaps and always read the held level -- exactly
# like sampling a real bus at fixed intervals.
#
# _I and _J are the held cursors: the indices of the most recent logic / analog
# sample at or before _T. They only ever move forward (time only advances), so a
# whole decode run stays O(samples). Both wait operations move _T forward and
# leave _I / _J pointing at the held samples for the new _T.

_LOGIC: Optional[LogicAccessor]     = None
_ANALOG: Optional["AnalogAccessor"] = None
_T: float = 0.0       # current decoder time, seconds
_I: int = 0           # held logic sub-sample index  (_LOGIC[_I].t <= _T)
_J: int = 0           # held analog record index     (_ANALOG[_J].t <= _T)
_END_T: float = 0.0   # true end of capture, seconds (host capture_end_tick)


# --- Internal helpers ------------------------------------------------------

def _hold_logic_to(t: float) -> None:
    """Advance the held logic cursor _I to the most recent sub-sample at or
    before t. Never moves backward (time only advances)."""
    global _I
    l = _LOGIC
    if l is None:
        return
    n = len(l)
    if n == 0:
        return
    i = _I if _I < n else n - 1
    while i + 1 < n and l[i + 1].t <= t:
        i += 1
    _I = i


def _hold_analog_to(t: float) -> None:
    """Advance the held analog cursor _J to the most recent record at or
    before t. Never moves backward."""
    global _J
    a = _ANALOG
    if a is None:
        return
    n = len(a)
    if n == 0:
        return
    j = _J if _J < n else n - 1
    while j + 1 < n and a[j + 1].t <= t:
        j += 1
    _J = j


def _capture_end_time() -> Optional[float]:
    """End of the captured time window in seconds, or None if nothing was
    captured. wait_time() returns None once _T would pass this.

    Logic records are transition-only, so the last stored sample is the last
    edge -- an idle bus at the end of the capture stores nothing even though the
    held value remains valid until capture actually stopped. The host reports
    that true end as capture_end_tick (-> _END_T); we honor it so a decoder can
    sample the held tail (e.g. a UART stop bit) instead of dropping the final
    symbol. Guarded so a stale/zero _END_T never truncates below a real sample."""
    end: Optional[float] = None
    if _LOGIC is not None and len(_LOGIC):
        end = _LOGIC[len(_LOGIC) - 1].t
    if _ANALOG is not None and len(_ANALOG):
        ta = _ANALOG[len(_ANALOG) - 1].t
        end = ta if end is None else max(end, ta)
    if _END_T > 0.0:
        end = _END_T if end is None else max(end, _END_T)
    return end


class _Cursor:
    """Mutable sticky cursor used by sample-and-hold lookups so total cost
    across a decoder run stays O(N)."""
    __slots__ = ("idx",)
    def __init__(self) -> None:
        self.idx = 0


def _analog_held_at(t: float, cursor: "_Cursor") -> Optional[AnalogSample]:
    """Sample-and-hold lookup: the most recent analog sample with
    analog[j].t <= t. None if no sample exists at or before t."""
    a = _ANALOG
    if a is None:
        return None
    n = len(a)
    if n == 0:
        return None
    j = cursor.idx
    if j >= n:
        j = n - 1
    while j + 1 < n and a[j + 1].t <= t:
        j += 1
    cursor.idx = j
    s = a[j]
    if s.t > t:
        return None
    return s


def _logic_held_at(t: float, cursor: "_Cursor") -> Optional[LogicSample]:
    """Sample-and-hold lookup on the logic buffer."""
    l = _LOGIC
    if l is None:
        return None
    n = len(l)
    if n == 0:
        return None
    i = cursor.idx
    if i >= n:
        i = n - 1
    while i + 1 < n and l[i + 1].t <= t:
        i += 1
    cursor.idx = i
    s = l[i]
    if s.t > t:
        return None
    return s


class Moment:
    """Snapshot of all signals at a point in time. Returned from every
    wait_for / wait_time call."""
    __slots__ = ("t", "d", "bits", "a")
    def __init__(self, t: float, d: tuple, bits: int, a: list) -> None:
        self.t    = t
        self.d    = d
        self.bits = bits
        self.a    = a


def _held_moment() -> Moment:
    """Snapshot of every signal at the current time _T using the held cursors.
    Logic bits and analog voltages are each the most recent value at or before
    _T (0 / 0.0 before the first sample of that kind exists)."""
    if _LOGIC is not None and 0 <= _I < len(_LOGIC):
        lsamp = _LOGIC[_I]
        if lsamp.t <= _T:
            d, bits = lsamp.d, lsamp.bits
        else:
            d, bits = (0,) * 8, 0
    else:
        d, bits = (0,) * 8, 0

    if _ANALOG is not None and 0 <= _J < len(_ANALOG):
        asamp = _ANALOG[_J]
        a_vals = asamp.a if asamp.t <= _T else [0.0] * 8
    else:
        a_vals = [0.0] * 8

    return Moment(_T, d, bits, a_vals)


# --- Conditions ------------------------------------------------------------

class _Condition:
    """A condition tested against the current moment.

    .primary  -- 'logic' or 'analog'; tells wait_for which buffer to scan.
    .test()   -- returns True when the condition fires at this sample.
    """
    primary = "logic"
    def test(self, lsamp: Optional[LogicSample],
                   asamp: Optional[AnalogSample]) -> bool:
        raise NotImplementedError


class _RisingEdge(_Condition):
    primary = "logic"
    def __init__(self, channel: int) -> None:
        self.channel = channel
        self._prev: Optional[int] = None
    def test(self, lsamp, asamp) -> bool:
        if lsamp is None:
            return False
        cur = lsamp.d[self.channel]
        fired = (self._prev == 0 and cur == 1)
        self._prev = cur
        return fired


class _FallingEdge(_Condition):
    primary = "logic"
    def __init__(self, channel: int) -> None:
        self.channel = channel
        self._prev: Optional[int] = None
    def test(self, lsamp, asamp) -> bool:
        if lsamp is None:
            return False
        cur = lsamp.d[self.channel]
        fired = (self._prev == 1 and cur == 0)
        self._prev = cur
        return fired


class _High(_Condition):
    primary = "logic"
    def __init__(self, channel: int) -> None:
        self.channel = channel
    def test(self, lsamp, asamp) -> bool:
        return lsamp is not None and lsamp.d[self.channel] == 1


class _Low(_Condition):
    primary = "logic"
    def __init__(self, channel: int) -> None:
        self.channel = channel
    def test(self, lsamp, asamp) -> bool:
        return lsamp is not None and lsamp.d[self.channel] == 0


class _VoltageRises(_Condition):
    primary = "analog"
    def __init__(self, channel: int, threshold: float,
                 hysteresis: float = 0.0) -> None:
        self.channel    = channel
        self.threshold  = threshold
        self.hysteresis = hysteresis
        self._arm_lo = threshold - hysteresis
        self._armed: Optional[bool] = None
    def test(self, lsamp, asamp) -> bool:
        if asamp is None:
            return False
        v = asamp.a[self.channel]
        if self._armed is None:
            self._armed = v <= self._arm_lo
            return False
        if self._armed:
            if v > self.threshold:
                self._armed = False
                return True
        else:
            if v <= self._arm_lo:
                self._armed = True
        return False


class _VoltageFalls(_Condition):
    primary = "analog"
    def __init__(self, channel: int, threshold: float,
                 hysteresis: float = 0.0) -> None:
        self.channel    = channel
        self.threshold  = threshold
        self.hysteresis = hysteresis
        self._arm_hi = threshold + hysteresis
        self._armed: Optional[bool] = None
    def test(self, lsamp, asamp) -> bool:
        if asamp is None:
            return False
        v = asamp.a[self.channel]
        if self._armed is None:
            self._armed = v >= self._arm_hi
            return False
        if self._armed:
            if v < self.threshold:
                self._armed = False
                return True
        else:
            if v >= self._arm_hi:
                self._armed = True
        return False


class _VoltageAbove(_Condition):
    primary = "analog"
    def __init__(self, channel: int, threshold: float) -> None:
        self.channel   = channel
        self.threshold = threshold
    def test(self, lsamp, asamp) -> bool:
        return asamp is not None and asamp.a[self.channel] > self.threshold


class _VoltageBelow(_Condition):
    primary = "analog"
    def __init__(self, channel: int, threshold: float) -> None:
        self.channel   = channel
        self.threshold = threshold
    def test(self, lsamp, asamp) -> bool:
        return asamp is not None and asamp.a[self.channel] < self.threshold


class _VoltageBetween(_Condition):
    primary = "analog"
    def __init__(self, channel: int, low: float, high: float) -> None:
        self.channel = channel
        self.low     = low
        self.high    = high
    def test(self, lsamp, asamp) -> bool:
        if asamp is None:
            return False
        v = asamp.a[self.channel]
        return self.low <= v <= self.high


class _AllOf(_Condition):
    def __init__(self, *conds: _Condition) -> None:
        self.conds = conds
        self.primary = ("analog"
                        if all(c.primary == "analog" for c in conds)
                        else "logic")
    def test(self, lsamp, asamp) -> bool:
        # Non-short-circuiting so child state machines all advance.
        results = [c.test(lsamp, asamp) for c in self.conds]
        return all(results)


class _AnyOf(_Condition):
    def __init__(self, *conds: _Condition) -> None:
        self.conds = conds
        self.primary = ("analog"
                        if all(c.primary == "analog" for c in conds)
                        else "logic")
    def test(self, lsamp, asamp) -> bool:
        results = [c.test(lsamp, asamp) for c in self.conds]
        return any(results)


class _Not(_Condition):
    def __init__(self, cond: _Condition) -> None:
        self.cond = cond
        self.primary = cond.primary
    def test(self, lsamp, asamp) -> bool:
        return not self.cond.test(lsamp, asamp)


# --- Public factory functions ----------------------------------------------

def rising_edge(channel: int) -> _Condition:
    """Digital rising edge on channel (0..7)."""
    return _RisingEdge(channel)


def falling_edge(channel: int) -> _Condition:
    """Digital falling edge on channel."""
    return _FallingEdge(channel)


def high(channel: int) -> _Condition:
    """Digital channel is currently at logic high (no transition required)."""
    return _High(channel)


def low(channel: int) -> _Condition:
    """Digital channel is currently at logic low."""
    return _Low(channel)


def voltage_rises(channel: int, threshold: float,
                  hysteresis: float = 0.0) -> _Condition:
    """Analog rising threshold crossing. Fires when the voltage rises above
    `threshold`. With hysteresis > 0, the voltage must first be at or below
    (threshold - hysteresis) before the next crossing can fire (asymmetric
    re-arm only -- the trigger level itself is always `threshold`)."""
    return _VoltageRises(channel, threshold, hysteresis)


def voltage_falls(channel: int, threshold: float,
                  hysteresis: float = 0.0) -> _Condition:
    """Analog falling threshold crossing."""
    return _VoltageFalls(channel, threshold, hysteresis)


def voltage_above(channel: int, threshold: float) -> _Condition:
    """Analog channel is currently above threshold (no crossing required)."""
    return _VoltageAbove(channel, threshold)


def voltage_below(channel: int, threshold: float) -> _Condition:
    """Analog channel is currently below threshold."""
    return _VoltageBelow(channel, threshold)


def voltage_between(channel: int, low: float, high: float) -> _Condition:
    """Analog channel is currently within [low, high]."""
    return _VoltageBetween(channel, low, high)


def all_of(*conds: _Condition) -> _Condition:
    """AND combinator: every sub-condition must be true at the same moment."""
    if not conds:
        raise ValueError("all_of requires at least one condition")
    return _AllOf(*conds)


def any_of(*conds: _Condition) -> _Condition:
    """OR combinator: at least one sub-condition must be true at the moment."""
    if not conds:
        raise ValueError("any_of requires at least one condition")
    return _AnyOf(*conds)


def not_(cond: _Condition) -> _Condition:
    """NOT combinator. Beware: not_(rising_edge(0)) fires on almost every
    sample because most samples are not instantaneous edges."""
    return _Not(cond)


# --- The two user-facing wait operations -----------------------------------
#
# Phase 8 (Step 8.10) single-mode: the data buffer is finalized before this
# process spawned. wait_for / wait_time scan straight through to the snapshot
# write_pos and return None when they hit the end -- no yielding to the
# driver loop is required. The yield statement at the bottom of each
# function is unreachable; it makes Python parse these as generator functions
# so `yield from wait_for(...)` callers keep working without source changes.

def wait_for(condition: _Condition,
             timeout: Optional[float] = None
             ) -> Generator[None, None, Optional[Moment]]:
    """Advance until condition.test() returns True at the current moment.
    Returns a Moment snapshot at the event, or None on end-of-capture/timeout.

    wait_for scans the stored samples of the buffer matching condition.primary
    ('logic' for digital conditions and any combinator containing a digital
    sub-condition; 'analog' only when every sub-condition is analog) starting
    from the current held position. Edges are detected between consecutive
    stored samples, so the first transition after 'now' is found no matter how
    far away it sits. The other domain's value at the event time is supplied to
    test() by sample-and-hold. A level condition that is already true at the
    current time fires immediately at the current time (not back at the last
    stored transition). On success _T and both held cursors land on the
    event."""
    global _T, _I, _J
    if _LOGIC is None or _ANALOG is None:
        raise RuntimeError("wait_for called outside the decoder run context")

    entry_t = _T

    if condition.primary == "analog":
        log_cursor = _Cursor(); log_cursor.idx = _I
        a = _ANALOG
        n = len(a)
        i = _J
        while i < n:
            asamp = a[i]
            lsamp_held = _logic_held_at(asamp.t, log_cursor)
            if condition.test(lsamp_held, asamp):
                _J = i
                _T = asamp.t if asamp.t > entry_t else entry_t
                _hold_logic_to(_T)
                return _held_moment()
            if timeout is not None and (asamp.t - entry_t) >= timeout:
                return None
            i += 1
            # Progress is published by the background publisher reading _I / _J.
        return None
    else:
        ana_cursor = _Cursor(); ana_cursor.idx = _J
        l = _LOGIC
        n = len(l)
        i = _I
        while i < n:
            lsamp = l[i]
            asamp_held = _analog_held_at(lsamp.t, ana_cursor)
            if condition.test(lsamp, asamp_held):
                _I = i
                _T = lsamp.t if lsamp.t > entry_t else entry_t
                _hold_analog_to(_T)
                return _held_moment()
            if timeout is not None and (lsamp.t - entry_t) >= timeout:
                return None
            i += 1
        return None
    # Unreachable: marks wait_for as a generator function so `yield from`
    # callers parse correctly.
    yield  # pragma: no cover


def wait_time(seconds: float
              ) -> Generator[None, None, Optional[Moment]]:
    """Advance the current time by `seconds` and return a Moment with the
    sample-and-held value at that exact time. Returns None once time runs past
    the end of the capture.

    This is pure time stepping: it does NOT scan to the next stored transition.
    Calling wait_time(1.0 / baud) in a loop samples the bus at fixed intervals
    and reads the held level at each point, so a long run of identical bits
    (which stores no samples at all) behaves exactly like a busy line -- every
    step returns whatever level is being held at that instant."""
    global _T
    if _LOGIC is None or _ANALOG is None:
        raise RuntimeError("wait_time called outside the decoder run context")

    end_t = _capture_end_time()
    if end_t is None:
        return None

    new_t = _T + seconds
    if new_t > end_t:
        return None

    _T = new_t
    _hold_logic_to(_T)
    _hold_analog_to(_T)
    return _held_moment()
    # Unreachable: generator marker.
    yield  # pragma: no cover


def wait_for_edge(channel: int, edge: str = 'rising',
                  timeout: Optional[float] = None
                  ) -> Generator[None, None, Optional[Moment]]:
    """Convenience wrapper: advance until an edge fires on a digital channel.

    edge: 'rising' (default), 'falling', or 'any'.
    Equivalent to `wait_for(rising_edge(channel))` etc. Returns a Moment at
    the edge, or None on end-of-capture/timeout.
    """
    if edge == 'rising':
        cond = _RisingEdge(channel)
    elif edge == 'falling':
        cond = _FallingEdge(channel)
    elif edge == 'any':
        cond = _AnyOf(_RisingEdge(channel), _FallingEdge(channel))
    else:
        raise ValueError(
            f"wait_for_edge: edge must be 'rising', 'falling', or 'any' (got {edge!r})")
    m = yield from wait_for(cond, timeout)
    return m


# --- Runtime setup (used by _run before calling user's decode) -------------

def _install_context(logic: LogicAccessor, analog: "AnalogAccessor",
                     end_t: float = 0.0) -> None:
    """Stash buffers as module state so wait_for / wait_time can see them.
    Resets the current time and held cursors to the start of the capture.
    end_t is the host's true capture-end time in seconds (0.0 if unknown)."""
    global _LOGIC, _ANALOG, _T, _I, _J, _END_T
    _LOGIC  = logic
    _ANALOG = analog
    _T      = 0.0
    _I      = 0
    _J      = 0
    _END_T  = end_t


# ---------------------------------------------------------------------------
# Driver.  Invoked when active_pro is run as __main__.
# ---------------------------------------------------------------------------

def _open_control() -> mmap.mmap:
    return _open_arena("AP_CONTROL", writable=False)


def _load_user_decoder() -> Callable[[Any, Any], Generator]:
    """Import the user decoder module and return its `decode` callable."""
    script = os.environ.get("AP_DECODER_SCRIPT")
    if not script and len(sys.argv) > 1:
        script = sys.argv[1]
    if not script:
        raise RuntimeError("AP_DECODER_SCRIPT (or argv[1]) is required")
    script_path = os.path.abspath(script)
    if not os.path.isfile(script_path):
        raise RuntimeError(f"decoder script not found: {script_path}")

    spec = importlib.util.spec_from_file_location("user_decoder", script_path)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"could not load decoder spec for {script_path}")
    mod = importlib.util.module_from_spec(spec)
    sys.modules["user_decoder"] = mod
    spec.loader.exec_module(mod)

    # Inject params dict so decoders can access params["key"] without importing.
    # AP_PARAMS is a JSON-serialized {key: value} map set by DecoderInstance.
    try:
        mod.params = json.loads(os.environ.get("AP_PARAMS", "{}") or "{}")
    except Exception:
        mod.params = {}

    if not hasattr(mod, "decode"):
        raise RuntimeError("decoder script must define a `decode(params)` function")
    return mod.decode, mod.params


def _wait_for_start(stdin: io.TextIOBase) -> bool:
    """Block until the host sends 'start\\n' on stdin.  Returns False on EOF."""
    line = stdin.readline()
    if not line:
        return False
    return line.strip() == "start"


def _run() -> int:
    """Entry point.  Wired by `python active_pro.py <script>` from C++."""
    global _OUTPUT

    print("DBG: _run() starting", file=sys.stderr, flush=True)

    # Step 1: open shared regions.
    print("DBG: opening control SHM", file=sys.stderr, flush=True)
    ctrl_mm = _open_control()
    logic_ctrl_off = int(os.environ.get("AP_LOGIC_CTRL_OFFSET", "0"))
    analog_ctrl_off = int(os.environ.get("AP_ANALOG_CTRL_OFFSET", "0"))
    out_ch_base = int(os.environ.get("AP_OUT_CH_BASE", "0"))
    print(f"DBG: ctrl offsets logic={logic_ctrl_off} analog={analog_ctrl_off} ch_base={out_ch_base}", file=sys.stderr, flush=True)

    logic_ctrl = _ControlHeader(ctrl_mm, logic_ctrl_off)
    analog_ctrl = _ControlHeader(ctrl_mm, analog_ctrl_off)
    print("DBG: control headers OK", file=sys.stderr, flush=True)

    logic_mm = _open_arena("AP_LOGIC", writable=False)
    print("DBG: logic arena open", file=sys.stderr, flush=True)
    analog_mm = _open_arena("AP_ANALOG", writable=False)
    print("DBG: analog arena open", file=sys.stderr, flush=True)
    out_mm = _open_arena("AP_OUT", writable=True)
    print("DBG: output ring open", file=sys.stderr, flush=True)

    _OUTPUT = _Output(out_mm, out_ch_base, logic_ctrl.timestamp_frequency)
    print("DBG: output object created", file=sys.stderr, flush=True)

    logic = LogicAccessor(logic_mm, logic_ctrl)
    analog = AnalogAccessor(analog_mm, analog_ctrl)

    # Alias this module so user scripts that do `from active_pro import append`
    # get the __main__ instance where _OUTPUT is already set, not a fresh copy.
    sys.modules.setdefault('active_pro', sys.modules['__main__'])

    # Step 2: load the user script.
    print("DBG: loading user decoder", file=sys.stderr, flush=True)
    decode_fn, decode_params = _load_user_decoder()
    print("DBG: user decoder loaded OK", file=sys.stderr, flush=True)

    # Step 2b: install the buffers as module state so the user's decode()
    # can call wait_for() / wait_time() without threading cursors. The capture
    # end (true window length, past the last transition) comes from the control
    # header so time-stepping decoders can sample the held idle tail.
    end_t = (logic_ctrl.capture_end_tick / logic_ctrl.timestamp_frequency
             if logic_ctrl.timestamp_frequency > 0 else 0.0)
    _install_context(logic, analog, end_t)

    # Step 3: hand off to the host -- the C++ side waits for 'ready'
    # before sending 'start'.
    print("ready", flush=True)
    print("DBG: sent ready, waiting for start", file=sys.stderr, flush=True)
    if not _wait_for_start(sys.stdin):
        print("DBG: no start received, exiting", file=sys.stderr, flush=True)
        return 0
    print("DBG: start received, running decode()", file=sys.stderr, flush=True)

    # Watch for "stop\n" or stdin EOF from the C++ host in a background thread
    # so a long-running decode() can be aborted promptly.
    t = threading.Thread(target=_monitor_stdin_for_stop, daemon=True)
    t.start()

    # Make the output ring mmap globally visible so the background progress
    # publisher can write cursor positions to it.
    global _RING_MM
    _RING_MM = out_mm

    # Start the background progress publisher. CPython's switch interval
    # (5 ms default) ensures this thread is scheduled on schedule even while
    # the decoder is in a tight pure-Python wait_for loop.
    _progress_stop.clear()
    pub = threading.Thread(target=_publish_progress_periodically, daemon=True)
    pub.start()

    # Step 4: drive the decoder generator. wait_for / wait_time don't yield
    # in single-mode (post-capture), so next(gen) typically runs decode() to
    # completion in one shot; the loop body fires at most a handful of times.
    gen = decode_fn(decode_params)
    if not hasattr(gen, "__next__"):
        raise RuntimeError("decode() must be a generator (use 'yield from')")

    try:
        while True:
            if _stop_requested.is_set():
                print("DBG: stop requested, exiting", file=sys.stderr, flush=True)
                return 0
            try:
                next(gen)
            except StopIteration:
                print("DBG: generator finished cleanly", file=sys.stderr, flush=True)
                return 0
    except KeyboardInterrupt:
        return 0
    finally:
        # Stop the background publisher and do a final flush so the C++
        # status bar lands cleanly on the terminal cursor positions.
        _progress_stop.set()
        _publish_progress(max(_I, _decoder_sample_pos),
                          max(_J, _decoder_analog_pos))
        try:
            gen.close()
        except Exception:
            pass


if __name__ == "__main__":
    sys.exit(_run())
