#!/usr/bin/env python3
"""ACTIVE-PRO MCP Server, exposes the ACTIVE-PRO Automation API as MCP tools."""

import asyncio
import socket
import mcp.types as types
from mcp.server import Server
from mcp.server.stdio import stdio_server

HOST = "127.0.0.1"
BASE_PORT = 37800
TIMEOUT = 15  # seconds, default
TIMEOUT_LONG = 600  # seconds, for blocking pipeline commands (ApplyChanges)


def send_command(command: str, device: int = 1, timeout: float | None = None) -> str:
    port = BASE_PORT + (device - 1)
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.settimeout(timeout if timeout is not None else TIMEOUT)
        try:
            s.connect((HOST, port))
            s.sendall((command + "\n").encode("ascii"))
            response = b""
            while b"\n" not in response:
                chunk = s.recv(4096)
                if not chunk:
                    break
                response += chunk
            return response.decode("ascii").strip()
        except ConnectionRefusedError:
            return f"ERROR: Cannot connect to ACTIVE-PRO on port {port}. Is the app running?"
        except socket.timeout:
            return "ERROR: Timed out waiting for response"
        except Exception as e:
            return f"ERROR: {e}"


app = Server("active-pro")


@app.list_tools()
async def list_tools() -> list[types.Tool]:
    def tool(name, description, properties=None, required=None):
        props = {"device": {"type": "integer", "description": "ACTIVE-PRO instance 1-8 (default 1)", "default": 1}}
        if properties:
            props.update(properties)
        return types.Tool(
            name=name,
            description=description,
            inputSchema={"type": "object", "properties": props, "required": required or []},
        )

    return [
        # --- Connection and Status ---
        tool("hello", "Verify the connection to ACTIVE-PRO is live. Returns HELLO."),
        tool("is_connected", "Check whether a hardware device is currently connected. Returns YES or NO."),
        tool("is_capturing", "Check whether a capture is currently in progress. Returns YES or NO."),
        tool("get_devices_attached", "Return a newline-separated list of all attached hardware devices. Returns NONE if no devices detected."),
        tool("select_device",
             "Switch to the hardware device at the given 1-based index from get_devices_attached. Clears capture data and resets settings.",
             {"index": {"type": "integer", "description": "1-based device index"}},
             required=["index"]),

        # --- Capture Control ---
        tool("start_capture", "Start a capture on the connected hardware device."),
        tool("stop_capture", "Stop the current capture. Blocks until decode is complete, safe to follow immediately with export commands."),
        tool("new_capture", "Clear all current capture data (equivalent to File > New Capture)."),
        tool("get_capture_size", "Return total bytes captured and stored on disk so far."),
        tool("get_capture_time", "Return the duration of the current capture in seconds."),

        # --- Signal Reading (requires active capture) ---
        tool("get_logic", "Return the current state of the 8 digital logic inputs as a bitmask (0-255). Bit 0 = D0, bit 7 = D7. Requires an active capture."),
        tool("get_analog",
             "Return the current voltage on an analog input channel (in volts). Requires an active capture. Channels 4-6 are ACTIVE-PRO Ultra only.",
             {"channel": {"type": "integer", "description": "Channel number 1-6"}},
             required=["channel"]),

        # --- Analog Channel Settings (read-only introspection) ---
        # These tools let the AI interpret captured analog numbers (units, scale, offset)
        # by reading the values configured in the GUI's Analog Channel Settings dialog.
        # There are no setter equivalents: the user configures via the GUI and saves
        # a configuration file; the API loads it via open_configuration.
        tool("get_channel_offset",
             "Return the per-channel Offset value (applied to raw voltage before display). Channel 1-8.",
             {"channel": {"type": "integer", "description": "Analog channel number 1-8"}},
             required=["channel"]),
        tool("get_channel_scale",
             "Return the per-channel Scale value (multiplier applied after Offset). Channel 1-8.",
             {"channel": {"type": "integer", "description": "Analog channel number 1-8"}},
             required=["channel"]),
        tool("get_channel_units",
             "Return the per-channel Units label, e.g. 'V', 'mA'. Channel 1-8.",
             {"channel": {"type": "integer", "description": "Analog channel number 1-8"}},
             required=["channel"]),
        tool("get_current_resistor",
             "Return the Current Measuring Resistor Value in ohms. 0.0 = current conversion not configured."),
        tool("get_battery_capacity",
             "Return the Battery Capacity in mAh used for battery-life estimation. 0 = not configured."),
        tool("get_analog_range",
             "Return the analog input voltage range. Either '0_20' (0 V to +20 V) or 'MINUS10_PLUS10' (-10 V to +10 V)."),
        tool("get_analog_sample_rate",
             "Return the configured analog sample rate in samples-per-second. "
             "Effective hardware max is 1 Msps on ACTIVE-PRO; up to 50 Msps on ACTIVE-PRO Ultra (auto-degraded by enabled-channel count)."),

        # --- Digital Output Control ---
        tool("set_digital_output_mode",
             "Set a digital output pin mode. pin: 0 (D0) or 1 (D1). mode: 0=tri-state, 1=0V (low), 2=3.3V (high), 3=PWM.",
             {"pin": {"type": "integer", "description": "0 = D0, 1 = D1"},
              "mode": {"type": "integer", "description": "0=tri-state, 1=low, 2=high, 3=PWM"}},
             required=["pin", "mode"]),
        tool("set_digital_pwm",
             "Set the PWM duty cycle for D0 or D1. 0 = 0%, 255 = 100%. Pin must already be in PWM mode.",
             {"pin": {"type": "integer", "description": "0 = D0, 1 = D1"},
              "level": {"type": "integer", "description": "0-255"}},
             required=["pin", "level"]),

        # --- Analog Output Control ---
        tool("set_analog_output_mode",
             "Set analog output mode. channel: 0=A0, 1=A1. "
             "A0 modes: 0=tri-state, 1=0V, 2=1V, 3=2V, 4=3V, 5=3.3V, 6=custom DC. "
             "A1 adds: 7=ramp, 8=sine, 9=square, 10=triangle.",
             {"channel": {"type": "integer", "description": "0 = A0, 1 = A1"},
              "mode": {"type": "integer"}},
             required=["channel", "mode"]),
        tool("set_analog_dc_level",
             "Set the custom DC output voltage for A0 or A1 (used when mode = 6). Max 3.3V.",
             {"channel": {"type": "integer", "description": "0 = A0, 1 = A1"},
              "voltage": {"type": "number", "description": "Voltage in volts, e.g. 1.75"}},
             required=["channel", "voltage"]),
        tool("set_a1_waveform",
             "Configure A1 waveform output parameters: minimum voltage, maximum voltage, and step count (controls frequency). "
             "Set mode to 7 (ramp), 8 (sine), 9 (square), or 10 (triangle) first.",
             {"minimum": {"type": "number", "description": "Minimum voltage"},
              "maximum": {"type": "number", "description": "Maximum voltage (max 3.3V)"},
              "steps": {"type": "integer", "description": "Step count, higher = slower frequency"}},
             required=["minimum", "maximum", "steps"]),

        # --- Cursor Control ---
        tool("set_cursor",
             "Move a cursor to a time position in seconds. cursor: 'current' (C), 'x1', or 'x2'.",
             {"cursor": {"type": "string", "enum": ["current", "x1", "x2"]},
              "time": {"type": "number", "description": "Time in seconds (e.g. 0.0015 = 1.5 ms)"}},
             required=["cursor", "time"]),
        tool("get_cursor",
             "Return the position of a cursor in seconds. cursor: 'current' (C), 'x1', or 'x2'.",
             {"cursor": {"type": "string", "enum": ["current", "x1", "x2"]}},
             required=["cursor"]),

        # --- Post-capture Pipeline ---
        # After loading a configuration (or after editing decoder/PP/trigger settings via the GUI),
        # apply_changes triggers a full reprocess of the existing capture buffer through the
        # Custom Decoders → Packet Presenters → Trigger Search pipeline. This is the same action
        # as clicking the "APPLY CHANGES" button in the status bar. The TCP socket blocks until
        # the pipeline completes or the user clicks the Cancel button.
        tool("apply_changes",
             "Reprocess the current capture through the decoder/PP/trigger pipeline. "
             "Blocks until the pipeline finishes or the user cancels via the GUI Cancel button. "
             "Returns 'OK' on completion, or an error if a capture is running or no changes are pending."),
        tool("is_processing",
             "Return 'Yes' if the post-capture pipeline (Custom Decoder, PacketPresenter, or trigger search) is running, otherwise 'No'. "
             "Use this for non-blocking polling instead of waiting on apply_changes."),
        tool("cancel_processing",
             "Request cancellation of the currently running post-capture pipeline. "
             "Equivalent to clicking the GUI Cancel button. Returns 'OK' or 'NotRunning'."),

        # --- Trigger Status and Navigation ---
        tool("is_triggered",
             "Return 'YES' if any triggers have fired in the current capture, otherwise 'NO'."),
        tool("get_trigger_count",
             "Return the total number of triggers detected in the current capture."),
        tool("get_trigger_index",
             "Return the 1-based index of the currently viewed trigger (0 if none)."),
        tool("get_trigger_cursor",
             "Return the position of the trigger cursor in seconds (0 if no triggers)."),
        tool("set_trigger_cursor",
             "Move the trigger cursor to a specific time (seconds).",
             {"time": {"type": "number", "description": "Time in seconds"}},
             required=["time"]),
        tool("next_trigger",
             "Advance to the next trigger. Returns the trigger time in seconds, or NOTFOUND."),
        tool("prev_trigger",
             "Step back to the previous trigger. Returns the trigger time in seconds, or NOTFOUND."),
        tool("first_trigger",
             "Jump to the oldest available trigger. Returns the trigger time in seconds, or NOTFOUND."),
        tool("last_trigger",
             "Jump to the most recent trigger. Returns the trigger time in seconds, or NOTFOUND."),
        tool("goto_trigger",
             "Jump to a specific trigger by 1-based absolute index. Returns the trigger time in seconds, or ERROR INVALIDINDEX.",
             {"index": {"type": "integer", "description": "1-based trigger index"}},
             required=["index"]),
        tool("center_trigger",
             "Re-center the current trigger in the waveform window. Same as the CENTER TRIGGER button."),
        tool("reprocess_triggers",
             "Re-scan the captured data with the current trigger settings only. "
             "Back-compat shortcut, prefer apply_changes for new scripts (runs the full pipeline)."),

        # --- Trigger Configuration ---
        tool("set_trigger_mode",
             "Set the trigger mode. off=trigger disabled, normal=fire only when condition met, auto=fire on timeout if no condition.",
             {"mode": {"type": "string", "enum": ["off", "normal", "auto"]}},
             required=["mode"]),
        tool("get_trigger_mode",
             "Return the current trigger mode token: OFF, NORMAL, or AUTO."),
        tool("set_trigger_source_type",
             "Select the trigger source category. Source types are: "
             "none, analog, digital, atext, avalues, btext, bvalues, ctext, cvalues, dtext, dvalues, "
             "appgraph, bppgraph, cppgraph, dppgraph. "
             "Changing the source repopulates the channel and condition lists.",
             {"source": {"type": "string",
                         "enum": ["none", "analog", "digital",
                                  "atext", "avalues", "btext", "bvalues",
                                  "ctext", "cvalues", "dtext", "dvalues",
                                  "appgraph", "bppgraph", "cppgraph", "dppgraph"]}},
             required=["source"]),
        tool("get_trigger_source_type",
             "Return the current trigger source token (NONE, ANALOG, DIGITAL, ATEXT, ..., DPPGRAPH)."),
        tool("set_trigger_channel",
             "Select a channel by 1-based index in the channel combo (the list is source-dependent, use get_trigger_channel_list to discover what's available).",
             {"channel": {"type": "integer", "description": "1-based channel index"}},
             required=["channel"]),
        tool("get_trigger_channel",
             "Return the display text of the currently selected channel (e.g. 'CH1 (VBUS)', 'D3'), or NONE if no channel is selected."),
        tool("get_trigger_channel_list",
             "Return the available channels for the current source type as a newline-separated list, or NONE if no source is active."),
        tool("set_trigger_condition",
             "Select the condition. Valid tokens depend on the source type: "
             "analog: rising, falling; digital: rising, falling, pulsehighgreater, pulsehighless, pulselowgreater, pulselowless; "
             "device text: contains, equals; device values: rising, falling, equals.",
             {"condition": {"type": "string",
                            "enum": ["rising", "falling",
                                     "pulsehighgreater", "pulsehighless",
                                     "pulselowgreater", "pulselowless",
                                     "contains", "equals"]}},
             required=["condition"]),
        tool("get_trigger_condition",
             "Return the current condition token, or NONE."),
        tool("set_trigger_threshold",
             "Set the trigger threshold value (volts for analog conditions, integer for Device Values). Range -100.0 to 100.0.",
             {"value": {"type": "number", "description": "Threshold value"}},
             required=["value"]),
        tool("get_trigger_threshold",
             "Return the current threshold value."),
        tool("set_trigger_pulse_width",
             "Set the pulse-width value and unit for digital pulse conditions.",
             {"value": {"type": "number", "description": "Pulse width value"},
              "unit":  {"type": "string", "enum": ["ns", "us", "ms", "s"]}},
             required=["value", "unit"]),
        tool("get_trigger_pulse_width",
             "Return the pulse width as '<value> <unit>' using the currently displayed unit."),
        tool("set_trigger_text",
             "Set the text pattern for Contains / Equals conditions. The string can contain spaces.",
             {"text": {"type": "string"}},
             required=["text"]),
        tool("get_trigger_text",
             "Return the current Contains / Equals text pattern (empty string if unset)."),

        # --- Buffer Size and Pre/Post-Trigger ---
        tool("set_buffer_size",
             "Set the capture buffer size in megabytes. Selects the smallest available entry that is >= the requested size. "
             "Combo entries are 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 100000, 1000000.",
             {"megabytes": {"type": "integer", "description": "Buffer size in MB"}},
             required=["megabytes"]),
        tool("get_buffer_size",
             "Return the current capture buffer size in MB."),
        tool("set_pre_trigger_mode",
             "Set the pre-trigger retention mode. keepall=retain everything captured before the trigger, keeplast=retain only the last N seconds.",
             {"mode": {"type": "string", "enum": ["keepall", "keeplast"]}},
             required=["mode"]),
        tool("get_pre_trigger_mode",
             "Return the current pre-trigger mode: KEEPALL or KEEPLAST."),
        tool("set_pre_trigger_seconds",
             "Set the pre-trigger window depth in seconds for the keeplast mode (range 1-60).",
             {"seconds": {"type": "integer", "description": "Seconds, 1-60"}},
             required=["seconds"]),
        tool("get_pre_trigger_seconds",
             "Return the current pre-trigger keep-last window depth in seconds."),
        tool("set_post_trigger_mode",
             "Set the post-trigger stop policy. untilfull=capture until buffer fills, stopafterseconds=stop N seconds after first trigger, stopaftertriggers=stop after N triggers fired.",
             {"mode": {"type": "string", "enum": ["untilfull", "stopafterseconds", "stopaftertriggers"]}},
             required=["mode"]),
        tool("get_post_trigger_mode",
             "Return the current post-trigger mode token: UNTILFULL, STOPAFTERSECONDS, or STOPAFTERTRIGGERS."),
        tool("set_post_trigger_seconds",
             "Set how many seconds to keep capturing after the first trigger fires. Promotes the StopAfterSeconds mode if not already selected.",
             {"seconds": {"type": "integer", "description": "Seconds, >= 1"}},
             required=["seconds"]),
        tool("get_post_trigger_seconds",
             "Return the post-trigger seconds value."),
        tool("set_post_trigger_triggers",
             "Set how many triggers to capture before stopping. Promotes the StopAfterTriggers mode if not already selected.",
             {"count": {"type": "integer", "description": "Trigger count, >= 1"}},
             required=["count"]),
        tool("get_post_trigger_triggers",
             "Return the post-trigger trigger count."),

        # --- Input Channel Settings ---
        tool("logic_channel_on_off",
             "Enable or disable a logic channel (D0-D7) for capture.",
             {"channel": {"type": "integer", "description": "Logic channel 0-7"},
              "on": {"type": "boolean", "description": "true to enable, false to disable"}},
             required=["channel", "on"]),
        tool("analog_channel_on_off",
             "Enable or disable an analog channel for capture. Channels 5-8 are ACTIVE-PRO Ultra only.",
             {"channel": {"type": "integer", "description": "Analog channel 1-8"},
              "on": {"type": "boolean", "description": "true to enable, false to disable"}},
             required=["channel", "on"]),
        tool("set_device_mode",
             "Set the decoder mode for a device port (A, B, C, or D). Mode is the integer DECODER_MODES index (see the Device Mode Index Table in the Automation API chapter). "
             "Changes reset the device label and bit/baud rate to protocol defaults.",
             {"device": {"type": "string", "enum": ["A", "B", "C", "D"]},
              "mode": {"type": "integer", "description": "DECODER_MODES index"}},
             required=["device", "mode"]),
        tool("set_threshold_level",
             "Set the digital logic threshold voltage. Accepts 1.0, 1.8, 2.5, 3.3, or 5.0 (any other value snaps to nearest).",
             {"voltage": {"type": "number", "description": "Threshold logic level in volts"}},
             required=["voltage"]),
        tool("set_label",
             "Set the display label for a logic channel, analog channel, or device port. "
             "target='logic' with index 0-7, target='analog' with index 1-8, target='device' with index='A'/'B'/'C'/'D'.",
             {"target": {"type": "string", "enum": ["logic", "analog", "device"]},
              "index":  {"type": "string", "description": "Numeric index as string (logic/analog) or letter A-D (device)"},
              "text":   {"type": "string"}},
             required=["target", "index", "text"]),
        tool("set_device_setting",
             "Set the numeric setting (baud rate, bit rate) for a device port. "
             "Use after set_device_mode, since changing the mode resets the setting to the protocol default.",
             {"device": {"type": "string", "enum": ["A", "B", "C", "D"]},
              "value":  {"type": "number"}},
             required=["device", "value"]),

        # --- View and Navigation ---
        tool("zoom_all", "Fit the entire capture on screen."),
        tool("zoom_from",
             "Zoom to show a specific time range. Both times are in seconds.",
             {"start": {"type": "number"}, "end": {"type": "number"}},
             required=["start", "end"]),
        tool("search",
             "Search forward from the current cursor for a text string in decoded Active Debug Port data. "
             "Returns the timestamp in seconds where found, or NOTFOUND.",
             {"text": {"type": "string", "description": "Text to search for (case-insensitive)"}},
             required=["text"]),

        # --- Tab Visibility ---
        tool("show_tab",
             "Open a tab panel or close the current one. "
             "tab values: inputs, triggers, outputs, list, settings, notes, code, liveui, devices, close.",
             {"tab": {"type": "string",
                      "enum": ["inputs", "triggers", "outputs", "list", "settings", "notes", "code", "liveui", "devices", "close"]}},
             required=["tab"]),

        # --- Waveform Device Visibility ---
        tool("set_device_visibility",
             "Show or hide a device's rows in the waveform view. device_label: A, B, C, or D.",
             {"device_label": {"type": "string", "enum": ["A", "B", "C", "D"]},
              "visible": {"type": "boolean"}},
             required=["device_label", "visible"]),

        # --- Notes ---
        tool("clear_note", "Clear all content from the Notes editor."),
        tool("append_note",
             "Append text to the Notes editor. Text can contain spaces.",
             {"text": {"type": "string"}},
             required=["text"]),
        tool("get_note", "Return the full current content of the Notes editor."),

        # --- AI Snapshot Analysis Context ---
        tool("clear_analysis_context", "Clear all content from the Analysis Context."),
        tool("append_analysis_context",
             "Append text as a new line to the Analysis Context, a freeform description embedded in every AI Snapshot export. "
             "Use this to tell the AI what the capture represents before exporting.",
             {"text": {"type": "string"}},
             required=["text"]),
        tool("get_analysis_context", "Return the current Analysis Context text."),

        # --- File Operations ---
        tool("open_capture",
             "Open a capture file from the given path.",
             {"path": {"type": "string"}},
             required=["path"]),
        tool("save_capture",
             "Save the current capture. If path is omitted, saves to the current filename.",
             {"path": {"type": "string", "description": "File path (optional)"}}),
        tool("save_between_cursors",
             "Save the X1-to-X2 region to a new capture file.",
             {"path": {"type": "string"}},
             required=["path"]),
        tool("open_configuration",
             "Load a configuration (.ini) file from the given path.",
             {"path": {"type": "string"}},
             required=["path"]),
        tool("save_configuration",
             "Save the current settings to a configuration file.",
             {"path": {"type": "string"}},
             required=["path"]),
        tool("export_between_cursors",
             "Export decoded data between X1 and X2 cursors to a CSV file.",
             {"path": {"type": "string"}},
             required=["path"]),
        tool("export_ai_snapshot",
             "Export the AI Snapshot for the X1-X2 range to an .aft file. "
             "The .aft file contains all decoded data and the Analysis Context, ready for AI analysis. "
             "Read the file back after exporting to analyze its contents.",
             {"path": {"type": "string"}},
             required=["path"]),
        tool("save_screenshot",
             "Save a screenshot of the full application window to an image file (.png recommended).",
             {"path": {"type": "string"}},
             required=["path"]),

        # --- Display ---
        tool("set_theme",
             "Switch the application theme.",
             {"theme": {"type": "string", "enum": ["dark", "light"]}},
             required=["theme"]),

        # --- Application Control ---
        tool("exit_app", "Close the ACTIVE-PRO application."),

        # --- Raw fallback ---
        tool("send_raw_command",
             "Send any raw command string directly to the ACTIVE-PRO TCP API. "
             "Use this for commands not covered by the other tools.",
             {"command": {"type": "string", "description": "Command string without trailing newline"}},
             required=["command"]),
    ]


@app.call_tool()
async def call_tool(name: str, arguments: dict) -> list[types.TextContent]:
    d = arguments.get("device", 1)

    match name:
        case "hello":               result = send_command("Hello", d)
        case "is_connected":        result = send_command("isConnected", d)
        case "is_capturing":        result = send_command("isCapturing", d)
        case "get_devices_attached": result = send_command("GetDevicesAttached", d)
        case "select_device":       result = send_command(f"SelectDevice {arguments['index']}", d)

        case "start_capture":       result = send_command("StartCapture", d)
        case "stop_capture":        result = send_command("StopCapture", d)
        case "new_capture":         result = send_command("NewCapture", d)
        case "get_capture_size":    result = send_command("GetCaptureSize", d)
        case "get_capture_time":    result = send_command("GetCaptureTime", d)

        case "get_logic":           result = send_command("GetLogic", d)
        case "get_analog":          result = send_command(f"GetCH{arguments['channel']}", d)

        case "get_channel_offset":  result = send_command(f"GetCH{arguments['channel']}Offset", d)
        case "get_channel_scale":   result = send_command(f"GetCH{arguments['channel']}Scale", d)
        case "get_channel_units":   result = send_command(f"GetCH{arguments['channel']}Units", d)
        case "get_current_resistor":    result = send_command("GetCurrentResistor", d)
        case "get_battery_capacity":    result = send_command("GetBatteryCapacity", d)
        case "get_analog_range":        result = send_command("GetAnalogRange", d)
        case "get_analog_sample_rate":  result = send_command("GetAnalogSampleRate", d)

        case "set_digital_output_mode":
            result = send_command(f"SetD{arguments['pin']}Mode {arguments['mode']}", d)
        case "set_digital_pwm":
            result = send_command(f"SetD{arguments['pin']}PWM {arguments['level']}", d)

        case "set_analog_output_mode":
            result = send_command(f"SetA{arguments['channel']}Mode {arguments['mode']}", d)
        case "set_analog_dc_level":
            result = send_command(f"SetA{arguments['channel']}DCLevel {arguments['voltage']}", d)
        case "set_a1_waveform":
            r1 = send_command(f"SetA1Minimum {arguments['minimum']}", d)
            r2 = send_command(f"SetA1Maximum {arguments['maximum']}", d)
            r3 = send_command(f"SetA1Steps {arguments['steps']}", d)
            result = f"Minimum={r1}  Maximum={r2}  Steps={r3}"

        case "set_cursor":
            cmd_map = {"current": "SetCursorCurrent", "x1": "SetCursorX1", "x2": "SetCursorX2"}
            result = send_command(f"{cmd_map[arguments['cursor']]} {arguments['time']}", d)
        case "get_cursor":
            cmd_map = {"current": "GetCursorCurrent", "x1": "GetCursorX1", "x2": "GetCursorX2"}
            result = send_command(cmd_map[arguments["cursor"]], d)

        case "apply_changes":       result = send_command("ApplyChanges", d, timeout=TIMEOUT_LONG)
        case "is_processing":       result = send_command("IsProcessing", d)
        case "cancel_processing":   result = send_command("CancelProcessing", d)

        case "is_triggered":        result = send_command("IsTriggered", d)
        case "get_trigger_count":   result = send_command("GetTriggerCount", d)
        case "get_trigger_index":   result = send_command("GetTriggerIndex", d)
        case "get_trigger_cursor":  result = send_command("GetTriggerCursor", d)
        case "set_trigger_cursor":  result = send_command(f"SetTriggerCursor {arguments['time']}", d)
        case "next_trigger":        result = send_command("NextTrigger", d)
        case "prev_trigger":        result = send_command("PrevTrigger", d)
        case "first_trigger":       result = send_command("FirstTrigger", d)
        case "last_trigger":        result = send_command("LastTrigger", d)
        case "goto_trigger":        result = send_command(f"GoToTrigger {arguments['index']}", d)
        case "center_trigger":      result = send_command("CenterTrigger", d)
        case "reprocess_triggers":  result = send_command("ReprocessTriggers", d, timeout=TIMEOUT_LONG)

        case "set_trigger_mode":         result = send_command(f"SetTriggerMode {arguments['mode']}", d)
        case "get_trigger_mode":         result = send_command("GetTriggerMode", d)
        case "set_trigger_source_type":  result = send_command(f"SetTriggerSourceType {arguments['source']}", d)
        case "get_trigger_source_type":  result = send_command("GetTriggerSourceType", d)
        case "set_trigger_channel":      result = send_command(f"SetTriggerChannel {arguments['channel']}", d)
        case "get_trigger_channel":      result = send_command("GetTriggerChannel", d)
        case "get_trigger_channel_list": result = send_command("GetTriggerChannelList", d)
        case "set_trigger_condition":    result = send_command(f"SetTriggerCondition {arguments['condition']}", d)
        case "get_trigger_condition":    result = send_command("GetTriggerCondition", d)
        case "set_trigger_threshold":    result = send_command(f"SetTriggerThreshold {arguments['value']}", d)
        case "get_trigger_threshold":    result = send_command("GetTriggerThreshold", d)
        case "set_trigger_pulse_width":  result = send_command(f"SetTriggerPulseWidth {arguments['value']} {arguments['unit']}", d)
        case "get_trigger_pulse_width":  result = send_command("GetTriggerPulseWidth", d)
        case "set_trigger_text":         result = send_command(f"SetTriggerText {arguments['text']}", d)
        case "get_trigger_text":         result = send_command("GetTriggerText", d)

        case "set_buffer_size":              result = send_command(f"SetBufferSize {arguments['megabytes']}", d)
        case "get_buffer_size":              result = send_command("GetBufferSize", d)
        case "set_pre_trigger_mode":         result = send_command(f"SetPreTriggerMode {arguments['mode']}", d)
        case "get_pre_trigger_mode":         result = send_command("GetPreTriggerMode", d)
        case "set_pre_trigger_seconds":      result = send_command(f"SetPreTriggerSeconds {arguments['seconds']}", d)
        case "get_pre_trigger_seconds":      result = send_command("GetPreTriggerSeconds", d)
        case "set_post_trigger_mode":        result = send_command(f"SetPostTriggerMode {arguments['mode']}", d)
        case "get_post_trigger_mode":        result = send_command("GetPostTriggerMode", d)
        case "set_post_trigger_seconds":     result = send_command(f"SetPostTriggerSeconds {arguments['seconds']}", d)
        case "get_post_trigger_seconds":     result = send_command("GetPostTriggerSeconds", d)
        case "set_post_trigger_triggers":    result = send_command(f"SetPostTriggerTriggers {arguments['count']}", d)
        case "get_post_trigger_triggers":    result = send_command("GetPostTriggerTriggers", d)

        case "logic_channel_on_off":
            state = "On" if arguments["on"] else "Off"
            result = send_command(f"LogicCH{arguments['channel']}{state}", d)
        case "analog_channel_on_off":
            state = "On" if arguments["on"] else "Off"
            result = send_command(f"AnalogCH{arguments['channel']}{state}", d)
        case "set_device_mode":
            result = send_command(f"Device{arguments['device'].upper()}Mode {arguments['mode']}", d)
        case "set_threshold_level":
            result = send_command(f"ThresholdLevel {arguments['voltage']}", d)
        case "set_label":
            target = arguments["target"]
            idx = arguments["index"]
            text = arguments["text"]
            if target == "logic":
                cmd = f"LogicCH{idx}Label {text}"
            elif target == "analog":
                cmd = f"AnalogCH{idx}Label {text}"
            else:
                cmd = f"Device{str(idx).upper()}Label {text}"
            result = send_command(cmd, d)
        case "set_device_setting":
            result = send_command(f"Device{arguments['device'].upper()}Setting {arguments['value']}", d)

        case "zoom_all":            result = send_command("ZoomAll", d)
        case "zoom_from":           result = send_command(f"ZoomFrom {arguments['start']} {arguments['end']}", d)
        case "search":              result = send_command(f"Search {arguments['text']}", d)

        case "show_tab":
            tab_map = {
                "inputs": "ShowInputs", "triggers": "ShowTriggers", "outputs": "ShowOutputs",
                "list": "ShowList", "settings": "ShowSettings", "notes": "ShowNotes",
                "code": "ShowCode", "liveui": "ShowLiveUI", "devices": "ShowDevices",
                "close": "CloseTabs",
            }
            result = send_command(tab_map[arguments["tab"]], d)

        case "set_device_visibility":
            action = "Show" if arguments["visible"] else "Hide"
            result = send_command(f"{action}Device{arguments['device_label'].upper()}", d)

        case "clear_note":          result = send_command("ClearNote", d)
        case "append_note":         result = send_command(f"AppendNote {arguments['text']}", d)
        case "get_note":            result = send_command("GetNote", d)

        case "clear_analysis_context":  result = send_command("ClearAnalysisContext", d)
        case "append_analysis_context": result = send_command(f"AppendAnalysisContext {arguments['text']}", d)
        case "get_analysis_context":    result = send_command("GetAnalysisContext", d)

        case "open_capture":        result = send_command(f"OpenCapture {arguments['path']}", d)
        case "save_capture":
            path = arguments.get("path", "")
            result = send_command(f"SaveCapture {path}".strip(), d)
        case "save_between_cursors":    result = send_command(f"SaveBetweenCursors {arguments['path']}", d)
        case "open_configuration":      result = send_command(f"OpenConfiguration {arguments['path']}", d)
        case "save_configuration":      result = send_command(f"SaveConfiguration {arguments['path']}", d)
        case "export_between_cursors":  result = send_command(f"ExportBetweenCursors {arguments['path']}", d)
        case "export_ai_snapshot":      result = send_command(f"ExportAISnapshotBetweenCursors {arguments['path']}", d)
        case "save_screenshot":         result = send_command(f"SaveScreenshot {arguments['path']}", d)

        case "set_theme":
            result = send_command("SetDarkMode" if arguments["theme"] == "dark" else "SetLightMode", d)
        case "exit_app":            result = send_command("Exit", d)
        case "send_raw_command":    result = send_command(arguments["command"], d)
        case _:                     result = f"ERROR: Unknown tool '{name}'"

    return [types.TextContent(type="text", text=result)]


async def main():
    async with stdio_server() as (read_stream, write_stream):
        await app.run(read_stream, write_stream, app.create_initialization_options())


if __name__ == "__main__":
    asyncio.run(main())
