[readme.md](https://github.com/user-attachments/files/27069285/readme.md)
# MCP Server

## Overview

This folder contains a Model Context Protocol server that lets AI assistants interface with the Active-Pro application to drive an Active-Pro, Active Debugger, or Active-Pro Ultra. Connect Claude Desktop or any other MCP-compatible client and the AI can run captures, query channels, and analyze results on your behalf, all in plain English.

For a complete beginner's walkthrough including Claude Desktop setup, Python installation, and JSON config editing, see [MCP Server for AI Assistants](https://www.activefirmwaretools.com/usersmanualold/main-window-acm82) in the [user's manual](https://www.activefirmwaretools.com/usersmanual).

## What it does

The server exposes a set of tools that an AI client can call against a running Active-Pro application:

- Start, stop, and configure captures on connected hardware
- List channels, signals, and connected devices
- Query events by time range, channel, or content
- Compute statistics such as event counts, intervals, and duty cycles
- Export captures to the `.aft` trace format for the AI to analyze in depth
- Read previously saved `.aft` files

Combined, these let you ask questions like *"capture for ten seconds and tell me why channel 3 stopped firing after the reset event"* and have the model actually drive the hardware and look at the data.

## How it fits together

```
Your hardware  ──USB──>  Active-Pro app  <──TCP──>  MCP server  <──MCP──>  AI client
```

The MCP server talks to the Active-Pro application over the same local TCP interface used by the [automation tools](../automation), then exposes those capabilities as MCP tools for the AI client to call.

## The .aft format

The Active-Pro application exports captures as `.aft` (Active Firmware Trace) files. This is the structured format the AI uses for deep analysis: channel definitions, timestamped events, and metadata in one self-contained package. The MCP server can both produce `.aft` files from live captures and read existing ones.

For format details, see [The .aft Format](https://www.activefirmwaretools.com/usersmanualold/aft-format) in the user's manual.

## Quick start

1. Install Python 3.10 or newer.
2. Install the server:

   ```
   pip install activepro-mcp
   ```

3. Add the server to your Claude Desktop configuration file (`claude_desktop_config.json`):

   ```json
   {
     "mcpServers": {
       "activepro": {
         "command": "activepro-mcp",
         "args": []
       }
     }
   }
   ```

4. Make sure the Active-Pro application is running with the automation interface enabled.
5. Restart Claude Desktop. The Active-Pro tools appear in the tool menu.

Detailed install instructions for Windows, macOS, and Linux are in the manual section linked above.

## The simpler alternative

If you just want to share a single capture with an AI, you do not need the MCP server. The application's [Copy AI Snapshot](https://www.activefirmwaretools.com/usersmanualold/main-window-amxnk) button copies a structured representation of the current view to your clipboard. Paste it into any AI chat and ask your question.

The MCP server is for ongoing, hands-off workflows where the model can drive the hardware and pull data on its own.

## Privacy

The server runs locally. Capture data is read from your application and passed only to the AI client you have configured. Nothing is sent to Active Firmware Tools.

## License

MIT. See [LICENSE](../LICENSE) at the repository root.

## Support

For support, email [support@activefirmwaretools.com](mailto:support@activefirmwaretools.com) or visit [www.activefirmwaretools.com](https://www.activefirmwaretools.com).
