[README.md](https://github.com/user-attachments/files/27071294/README.md)
# Active Firmware Tools

**Record. Reveal. Refine.**

The official repository for the Active-Pro Debugger and Logic Analyzer family — the recorder of embedded system behavior.

[Download](https://activefirmwaretools.com/download) · [User's Manual](https://activefirmwaretools.com/users-manual) · [Store](https://activefirmwaretools.com/store) · [Blog](https://activefirmwaretools.com/blog)

---

## What is Active-Pro?

Active-Pro is a hardware-and-software toolchain that records what your embedded system is actually doing — every signal transition, every protocol event, every stretch of idle time — and lets you replay, inspect, and share that capture later.

Think of it as Wireshark for the wires on your board. Where a logic analyzer shows you waveforms and a debugger shows you breakpoints, Active-Pro records the full behavioral trace of a running system so you can see how the pieces actually fit together over time.

Three products share the same application and capture format:

- **Active Debugger** — entry-level debug port and signal capture
- **Active-Pro** — full debugger and logic analyzer
- **Active-Pro Ultra** — super-speed capture for higher-bandwidth and longer-duration traces

All three work with the free Active-Pro application on Windows, macOS, and Linux. Run the app without hardware attached to demo every feature.

## What's in this repo

| Folder | Contents |
|---|---|
| [`active-debug-port-source-code`](./active-debug-port-source-code) | Reference source for the Active debug port |
| [`active-debugger`](./active-debugger) | Active Debugger product files and examples |
| [`active-pro-application`](./active-pro-application) | Application-side examples and resources |
| [`automation`](./automation) | Automation API command-line tool and scripting examples |
| [`mcp-server`](./mcp-server) | MCP server for connecting Claude and other AI clients to your captures |
| [`packet-presenter-definition-files`](./packet-presenter-definition-files) | Protocol definition files for the Packet Presenter |

## Get started

1. **Buy hardware** at [activefirmwaretools.com/store](https://activefirmwaretools.com/store), or skip ahead and use demo mode.
2. **Download the application** from [activefirmwaretools.com/download](https://activefirmwaretools.com/download) — Windows, macOS, and Linux builds available.
3. **Press Capture.** With hardware attached you'll see your signals live. Without hardware you'll get the full demo capture to explore the UI.
4. **Read the manual** at [activefirmwaretools.com/users-manual](https://activefirmwaretools.com/users-manual) when you're ready to go deeper.

## AI-assisted analysis

Captures export to `.aft` (Active Firmware Trace) — a plain-text format with an embedded AI context block. Copy a capture to your clipboard, paste it into any AI assistant, and ask questions about timing, protocol behavior, or anomalies in plain English.

For tighter integration, the [`mcp-server`](./mcp-server) folder contains an MCP server that lets Claude Desktop and other AI clients query your captures directly.

## Contributing

We accept changes via pull requests against `main`. See [CONTRIBUTING.md](./CONTRIBUTING.md) for the workflow and coding conventions.

For bug reports and feature requests, please open an issue.

## License

Code in this repository is licensed under the MIT License. See [LICENSE](./LICENSE) for details. Hardware, firmware images, and the Active-Pro application are commercial products and are not covered by this license.

---

Made by [Active Firmware Tools](https://activefirmwaretools.com).
