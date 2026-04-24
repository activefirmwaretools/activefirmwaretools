# MCP Server

## Overview

This folder contains a Model Context Protocol server that lets AI assistants read and analyze your captured traces. Point Claude Desktop or any other MCP-compatible client at this server and ask questions about your `.aft` trace files in plain English.

For a complete beginner's walkthrough including Claude Desktop setup, Python installation, and JSON config editing, see [MCP Server for AI Assistants](https://www.activefirmwaretools.com/usersmanualold/main-window-acm82) in the [user's manual](https://www.activefirmwaretools.com/usersmanual).

## What it does

The server exposes a small set of tools that an AI client can call:

- Open an `.aft` trace file
- List channels and event types
- Query events by time range, channel, or content
- Compute basic statistics such as event counts, intervals, and duty cycles
- Pull out specific event records for the model to reason about

Combined, these let you ask questions like *"why did channel 3 stop firing after the reset event?"* and have the model actually look at your data.

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

4. Restart Claude Desktop. The Active-Pro tools appear in the tool menu.

Detailed install instructions for Windows, macOS, and Linux are in the manual section linked above.

## The simpler alternative

If you just want to share a trace with an AI once, you do not need the MCP server. The Active-Pro application's [Copy AI Snapshot](https://www.activefirmwaretools.com/usersmanualold/main-window-amxnk) button copies a structured representation of the current view to your clipboard. Paste it into any AI chat and ask your question.

The MCP server is for ongoing, hands-off analysis where the model can pull data on its own.

## Privacy

The server runs locally. Your trace data is read from local files and passed only to the AI client you have configured. Nothing is sent to Active Firmware Tools.

## License

MIT. See [LICENSE](../LICENSE) at the repository root.

## Support

For support, email [support@activefirmwaretools.com](mailto:support@activefirmwaretools.com) or visit [www.activefirmwaretools.com](https://www.activefirmwaretools.com).
