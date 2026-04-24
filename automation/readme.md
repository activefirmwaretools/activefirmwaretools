# Automation

## Overview

This folder contains everything needed to drive the Active-Pro application from your own scripts and tools. Use these files to start and stop captures, configure channels, export traces, and integrate the debugger into automated test workflows without writing a TCP client from scratch.

The automation interface is available in Active-Pro application version 4.0 and above. For full reference documentation, see [Automation API](https://www.activefirmwaretools.com/usersmanualold/main-window-d9b4x) in the [user's manual](https://www.activefirmwaretools.com/usersmanual).

## Contents

| File or folder                                        | Purpose                                                |
|-------------------------------------------------------|--------------------------------------------------------|
| `ActiveCommandLine/`                                  | Prebuilt Windows command-line tool, ready to run       |
| `automation-command-line-application-source-code.c`   | Source for the command-line tool, build for any OS     |
| `example-automation-using-qt-in-c.c`                  | Qt/C example showing how to embed automation in your application |

The two source files use Qt's `QTcpSocket` to talk to the Active-Pro application over a local TCP connection.

## ActiveCommandLine folder

The prebuilt tool for Windows users who want to start automating immediately:

| File                          | Purpose                                                |
|-------------------------------|--------------------------------------------------------|
| `ActiveAPICommandLine.exe`    | Compiled command-line client for Windows               |
| `readme_automationapi.txt`    | Quick reference for the command set                    |
| `test_all_commands.bat`       | Batch script that exercises every API command          |

Run `test_all_commands.bat` against a running Active-Pro application to verify the automation interface is working and to see every command in action.

## Two ways to integrate

**Command line.** Use `ActiveAPICommandLine.exe` directly on Windows, or build `automation-command-line-application-source-code.c` to get the same tool on macOS or Linux. Send a command, get the response on stdout, exit with a status code. Suitable for shell scripts, batch files, `make`, `ctest`, GitHub Actions, and similar runners.

**Library.** `example-automation-using-qt-in-c.c` shows how to embed the same logic in your own Qt application for tighter control and live data access. Use it as a starting point.

## TCP ports

The Active-Pro application listens on the following local ports by default:

| Port  | Macro                       | Purpose                |
|-------|-----------------------------|------------------------|
| 37800 | `ACTIVE_TCP_PORT_DEVICE_1`  | First connected device |
| 37801 | `ACTIVE_TCP_PORT_DEVICE_2`  | Second connected device |

Enable the automation interface in the Active-Pro application before connecting.

## Protocol

Each interaction is a single command followed by a single response, both newline-terminated:

1. Open a TCP connection to the appropriate port
2. Write the command (must end with `\n`)
3. Read until you receive a newline in the response
4. Close the connection or reuse it for the next command

The full command reference is in the [Automation API](https://www.activefirmwaretools.com/usersmanualold/main-window-d9b4x) section of the user's manual, and `ActiveCommandLine/readme_automationapi.txt` has a quick reference.

## Common use cases

- Bring-up scripts that run a fixed regression after every firmware build
- Long-soak tests that record continuously and rotate trace files on a schedule
- Hardware-in-the-loop benches that correlate debugger traces with stimulus generators
- Field diagnostic kits that capture a trace on demand and email it to engineering

## Building from source

The two `.c` files require Qt (Core and Network modules). With Qt installed:

```
qmake -project "QT += core network" "CONFIG += console"
qmake
make
```

## License

MIT. See [LICENSE](../LICENSE) at the repository root.

## Support

For support, email [support@activefirmwaretools.com](mailto:support@activefirmwaretools.com) or visit [www.activefirmwaretools.com](https://www.activefirmwaretools.com).
