# Active-Pro MCP Server — Context for AI Assistants

> **How to use this file:** Import from your project's `CLAUDE.md` (or equivalent)
> with `@~/.active-pro-mcp/ACTIVE_PRO_MCP.md` (or wherever you installed it).
> Claude Code and Claude Desktop will load it automatically on session start,
> so the assistant knows what the Active-Pro can do without rediscovering it
> each conversation.

---

## What this MCP controls

The Active-Pro MCP Server exposes the **Active-Pro Debugger & Logic Analyzer**
(made by Active Firmware Tools) to AI assistants as a set of typed tools. The
assistant can start captures, read signals, drive outputs, search decoded
protocol data, and export analysis-ready snapshots of firmware behavior — all
by calling tools, not by generating instructions for the user to follow manually.

The MCP is a thin Python wrapper around the Active-Pro application's local
Automation API (TCP socket on `127.0.0.1:37800`). The Active-Pro application
must be running (version 3.8+) for any tool to work.

Hardware product family: `Active Debugger`, `Active-Pro`, `Active-Pro Ultra`.
Ultra adds analog channels 4–8 and is the flagship.

---

## The one concept you probably don't know: Active Debug Port (ADP)

This is the feature most foreign to a stock LLM, so read this section carefully
before reasoning about what captures contain.

**ADP is a firmware-instrumented debug channel, not a hardware bus.** Embedded
firmware calls functions like:

- `ACTIVEText(channel, "message")` — printf-style string
- `ACTIVEprintf(channel, "fmt %d", val)` — formatted string
- `ACTIVEValue(channel, value)` — signed int, rendered as a graphed waveform
- `ACTIVELabel(channel, "name")` — names a channel for the UI

These calls transmit over 1 or 2 GPIO pins (or the Cortex-M SWV pin) using a
small packet protocol, at speeds up to tens of Mbit/s. The Active-Pro pod
captures and decodes those packets alongside hardware signals, all on a single
nanosecond-accurate timeline.

**Why it matters for reasoning about captures:**

- Firmware runs at full speed — ADP is not JTAG, nothing halts.
- Each ADP "channel" is a firmware-defined logical stream (0–63 per port), named
  by the firmware author. A channel called `speed_loop` means the firmware
  called `ACTIVELabel(x, "speed_loop")` — so channel names are semantic,
  not protocol-defined.
- There are **four physical ADP ports** per Active-Pro pod: A, B, C, D. Each
  can connect to a different processor or FPGA. A capture can contain
  concurrent ADP streams from multiple chips on the same timeline.
- ADP carries three kinds of events: text strings (displayed as list entries),
  numeric values (displayed as graphed waveforms), and control commands
  (`?cmd=beep`, `?cmd=stop`, `?cmd=restart`, `?cmd=zoomall`).
- ADP connection modes: `ACTIVE 2-Wire` (sync clock+data, fastest), `ACTIVE 1-Wire`
  (async UART, baud must divide 30 MHz evenly), `ACTIVE SWV` (ARM Cortex-M
  ITM/SWV pin, UART-encoded). These appear as decoder mode indices 1, 2, and
  37 respectively in the device mode table.

When the user mentions ADP, Active Debug Port, `ACTIVEText`, `ACTIVEValue`,
`ACTIVELabel`, or "firmware debug output" — this is what they mean.

---

## What a capture actually contains

Every capture is a time-aligned, nanosecond-resolution recording of:

1. **Active Debug Port events** from up to four firmware targets (A/B/C/D),
   each with up to 64 named channels of text and graphed values.
2. **Logic channels D0–D7** — 8 digital inputs, captured as state transitions
   (not per-sample, only on change).
3. **Analog channels** — CH1–CH3 on Active-Pro; CH1–CH8 on Active-Pro Ultra.
   Voltage or calibrated engineering units.
4. **Hardware protocol decodes** on any device port: I2C, SPI (four polarity
   variants), UART 8N1 and 9N1 (normal and inverted), LIN, RS232, 1-Wire,
   MDIO, CHSI, DS101, EE101.

Capture files are `.active`. Exports for AI analysis are `.aft`
(Active-Pro Firmware Trace).

---

## The `.aft` format — what you'll be reading

When a user exports an AI Snapshot (via `export_ai_snapshot` or by pasting from
the application), they hand you an `.aft` text block. Its structure:

```
Active-Pro Firmware Trace (.aft)

[ANALYSIS CONTEXT]
<user-written briefing: what the firmware does, what they're investigating,
what question they want answered. May be absent if empty.>

[DEVICE SOURCES]
A: <user's label for port A>
B: <user's label for port B>
L: Logic Inputs
AN: Analog Inputs

[DEVICE A CHANNELS]
0: <channel-0 firmware-assigned name>
1: <channel-1 firmware-assigned name>
...

---DATA---
timestamp, source_id, channel_id, value
0.000125100, A, 0, "W 0x28 0x03 0xFF"
0.000125900, B, 0, "speed_loop: target=4500 actual=4498"
0.000201000, L, 3, 1
0.000201050, AN, 1, 3.287
...
```

Field semantics:

- `timestamp` — seconds from start of capture, nanosecond precision.
- `source_id` — `A`/`B`/`C`/`D` (ADP device), `L` (logic input), `AN` (analog input).
- `channel_id` — for ADP, the firmware channel (0–63); for `L`, logic line
  (0–7); for `AN`, analog channel (1–8).
- `value` — decoded string for ADP text and protocol decodes, signed integer
  for ADP numeric values, `0`/`1` for logic transitions, voltage (or engineering
  unit) for analog.
- Values containing commas are quoted (standard CSV).

**Read the `[ANALYSIS CONTEXT]` section first if present.** The user wrote it
specifically to tell you what they're looking for before you see the data.

---

## Available tools (grouped by what they do)

All tools accept an optional `device` parameter (1–8, default 1) that selects
which Active-Pro instance to target when multiple are running (ports 37800,
37801, ...).

**Connection and status**
`hello`, `is_connected`, `is_capturing`, `get_devices_attached`, `select_device`

**Capture control**
`start_capture`, `stop_capture`, `new_capture`, `get_capture_size`, `get_capture_time`

`stop_capture` blocks until decoding finishes, so it's safe to immediately
follow with `save_capture` or `export_ai_snapshot`.

**Live signal reading** (current value at latest captured position)
`get_logic` (returns 0–255 bitmask of D0–D7), `get_analog` (channel → volts)

**Digital output** (stimulus for D0, D1)
`set_digital_output_mode`, `set_digital_pwm`
Modes: 0=tri-state, 1=0V, 2=3.3V, 3=PWM. PWM level 0–255.

**Analog output** (A0 is DC only; A1 adds waveforms)
`set_analog_output_mode`, `set_analog_dc_level`, `set_a1_waveform`
A0 modes 0–6: tri-state, 0V, 1V, 2V, 3V, 3.3V, custom.
A1 modes 0–10: same as A0 plus 7=ramp, 8=sine, 9=square, 10=triangle.
Waveforms take min/max voltage and a step count (each step = 4 µs).

**Cursor control**
`set_cursor` (current/X1/X2, time in seconds), `get_cursor`
The "current cursor" is the starting point for `search`.

**View and navigation**
`zoom_all`, `zoom_from <start> <end>`, `search <string>`
`search` is case-insensitive, searches ADP decoded text forward from the
current cursor. Returns a timestamp in seconds, or `NOTFOUND`.

**Panel and display**
`show_tab` (Inputs/Outputs/List/Settings/Notes/Code/LiveUI/Devices),
`set_device_visibility` (show/hide device A/B/C/D rows), `set_theme`
(dark/light)

**Notes**
`clear_note`, `append_note`, `get_note`
The Notes editor is saved inside the `.active` capture file.

**Analysis Context** (the briefing written into every AI Snapshot)
`clear_analysis_context`, `append_analysis_context`, `get_analysis_context`
Setting this before an automated run means every snapshot exported during
that run carries the test name, firmware version, or whatever context makes
the later analysis useful.

**File operations** (all paths can contain spaces)
`open_capture`, `save_capture`, `save_between_cursors`, `open_configuration`,
`save_configuration`, `export_between_cursors` (CSV between X1/X2),
`export_ai_snapshot` (.aft between X1/X2), `save_screenshot`

**Application**
`exit_app`, `send_raw_command` (escape hatch to the underlying socket API)

---

## How to work with the user effectively

**When they ask for a capture, a typical flow is:**

1. `is_connected` — verify hardware is attached.
2. Either `new_capture` to clear previous data, or `open_capture`, or `open_configuration` to load all channel settings if they give you the filename.
3. If requested special settings, configure them: channel labels, device decoder modes, threshold level,
   analysis context, etc.  Do not call `new_capture` or it will clear all settings.
4. `start_capture`, wait, `stop_capture` (it blocks until decode finishes).
5. `search` for a target string, or set cursors and `export_ai_snapshot` to
   pull data back for analysis.

**When they ask you to analyze something:**

- If they want you to look at captured data, export an AI Snapshot over the
  relevant time range (`set_cursor` X1 and X2, then `export_ai_snapshot`).
  Read the exported `.aft` file. Prefer narrow time windows — snapshot size
  scales with events in range, and big snapshots eat context.
- If they mention a specific event or error, use `search` first to locate it,
  then bracket it with X1/X2 cursors before exporting.
- Only channels that are **enabled and have data in the range** appear in a
  snapshot. If a channel seems missing, check whether it's enabled (there are
  per-channel on/off tools under the `send_raw_command` fallback, or in the
  Inputs tab).

**Things that are easy to get wrong:**

- Ports on the Active-Pro are **A, B, C, D** — four of them. Logic channels
  are D0–D7. Analog channels are CH1–CH8. Don't conflate logic D0 with ADP
  port A — they're different things.
- `ACTIVE 2-Wire` and `ACTIVE 1-Wire` are **ADP decoder modes**, not
  hardware-bus modes like I2C or SPI. The data they decode is firmware-generated
  ADP packets, not arbitrary bus traffic.
- Baud rates for the 1-wire ADP must be exact divisors of 30 MHz
  (e.g., 115200, 250000, 500000, 1000000, 3000000).
- The MCP will not work against `claude.ai` in a browser — it requires
  Claude Desktop (or another MCP-capable host running locally).

**Multiple Active-Pro instances:** If the user has two pods running, the
second one's MCP tools target port 37801. Pass `device=2` on any tool call
to hit it.

---

## Primary documentation

- User's manual index: https://www.activefirmwaretools.com/usersmanual
- MCP Server setup page: https://www.activefirmwaretools.com/usersmanualold/main-window-acm82
- Automation API reference (full command set the MCP wraps):
  https://www.activefirmwaretools.com/usersmanualold/main-window-d9b4x
- AI Snapshot / `.aft` format:
  https://www.activefirmwaretools.com/usersmanualold/main-window-amxnk
- Active Debug Port firmware integration:
  https://www.activefirmwaretools.com/usersmanualold/main-window-stbtg

If the user asks about something this file doesn't cover, those pages have the
full detail. Support: support@activefirmwaretools.com.
