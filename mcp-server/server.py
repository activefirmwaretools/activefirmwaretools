#!/usr/bin/env python3
"""ACTIVE-PRO MCP Server — exposes the ACTIVE-PRO Automation API as MCP tools."""

import asyncio
import socket
import mcp.types as types
from mcp.server import Server
from mcp.server.stdio import stdio_server

HOST = "127.0.0.1"
BASE_PORT = 37800
TIMEOUT = 15  # seconds


def send_command(command: str, device: int = 1) -> str:
    port = BASE_PORT + (device - 1)
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.settimeout(TIMEOUT)
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
        tool("stop_capture", "Stop the current capture. Blocks until decode is complete — safe to follow immediately with export commands."),
        tool("new_capture", "Clear all current capture data (equivalent to File > New Capture)."),
        tool("get_capture_size", "Return total bytes captured and stored on disk so far."),
        tool("get_capture_time", "Return the duration of the current capture in seconds."),

        # --- Signal Reading (requires active capture) ---
        tool("get_logic", "Return the current state of the 8 digital logic inputs as a bitmask (0–255). Bit 0 = D0, bit 7 = D7. Requires an active capture."),
        tool("get_analog",
             "Return the current voltage on an analog input channel (in volts). Requires an active capture. Channels 4–6 are ACTIVE-PRO Ultra only.",
             {"channel": {"type": "integer", "description": "Channel number 1–6"}},
             required=["channel"]),

        # --- Digital Output Control ---
        tool("set_digital_output_mode",
             "Set a digital output pin mode. pin: 0 (D0) or 1 (D1). mode: 0=tri-state, 1=0V (low), 2=3.3V (high), 3=PWM.",
             {"pin": {"type": "integer", "description": "0 = D0, 1 = D1"},
              "mode": {"type": "integer", "description": "0=tri-state, 1=low, 2=high, 3=PWM"}},
             required=["pin", "mode"]),
        tool("set_digital_pwm",
             "Set the PWM duty cycle for D0 or D1. 0 = 0%, 255 = 100%. Pin must already be in PWM mode.",
             {"pin": {"type": "integer", "description": "0 = D0, 1 = D1"},
              "level": {"type": "integer", "description": "0–255"}},
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
              "steps": {"type": "integer", "description": "Step count — higher = slower frequency"}},
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
             "tab values: inputs, outputs, list, settings, notes, code, liveui, devices, close.",
             {"tab": {"type": "string",
                      "enum": ["inputs", "outputs", "list", "settings", "notes", "code", "liveui", "devices", "close"]}},
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
             "Append text as a new line to the Analysis Context — a freeform description embedded in every AI Snapshot export. "
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
             "Export the AI Snapshot for the X1–X2 range to an .aft file. "
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

        case "zoom_all":            result = send_command("ZoomAll", d)
        case "zoom_from":           result = send_command(f"ZoomFrom {arguments['start']} {arguments['end']}", d)
        case "search":              result = send_command(f"Search {arguments['text']}", d)

        case "show_tab":
            tab_map = {
                "inputs": "ShowInputs", "outputs": "ShowOutputs", "list": "ShowList",
                "settings": "ShowSettings", "notes": "ShowNotes", "code": "ShowCode",
                "liveui": "ShowLiveUI", "devices": "ShowDevices", "close": "CloseTabs",
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
