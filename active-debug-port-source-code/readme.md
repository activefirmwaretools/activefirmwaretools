# Active Debug Port Source Code

## Overview

The Active Debug Port is a lightweight firmware module that lets your embedded target stream events, variables, and trace data into the Active Debugger or Active-Pro Debugger & Logic Analyzer. This folder contains the driver source in several languages along with example integrations.

The driver is small, non-blocking, and has no OS dependency. It runs on Cortex-M, AVR, MSP430, RISC-V, and similar 8/16/32-bit microcontrollers, as well as inside FPGA fabric.

For a step-by-step integration walkthrough, see [Active Debug Port - Firmware Integration](https://www.activefirmwaretools.com/usersmanualold/main-window-stbtg) and [Source Code Integration](https://www.activefirmwaretools.com/usersmanualold/main-window-nr5t4) in the [user's manual](https://www.activefirmwaretools.com/usersmanual).

## Files

| File                       | Purpose                                          |
|----------------------------|--------------------------------------------------|
| `active.c`                 | C driver implementation                          |
| `active.h`                 | C driver header                                  |
| `active.py`                | Python driver implementation                     |
| `active.rs`                | Rust driver implementation                       |
| `active.v`                 | Verilog driver for FPGA targets                  |
| `active.vhd`               | VHDL driver for FPGA targets                     |
| `example-using-active.c`   | Reference C integration                          |
| `example-using-active.v`   | Reference Verilog integration                    |

## Adding the driver to your project

For C and Rust targets, copy the driver file and matching header into your project, implement the small platform shim (transport pins and a microsecond timestamp source), and call the init function once at startup. The example files show a minimal working integration.

For Verilog and VHDL targets, instantiate the module inside your top-level design and connect it to the chosen output pins. The example shows typical wiring.

For Python targets, the driver is suitable for systems running MicroPython or CPython on a board with GPIO access.

## Channels and events

The driver groups data into channels (logical streams) carrying timestamped events. Channel and event definitions are declared once in your firmware and decoded automatically by the Active-Pro application.

## License

MIT. See [LICENSE](../LICENSE) at the repository root.

## Support

For support, email [support@activefirmwaretools.com](mailto:support@activefirmwaretools.com) or visit [www.activefirmwaretools.com](https://www.activefirmwaretools.com).
