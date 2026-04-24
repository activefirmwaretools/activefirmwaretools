# Active-Pro Application

## Overview

The Active-Pro application is the desktop software that drives the Active Debugger and Active-Pro Debugger & Logic Analyzer. It records, displays, and analyzes streaming data from your embedded target.

The same application supports all hardware in the family. It detects the connected device and adjusts available features automatically.

## Downloads

| File                       | Platform                                  |
|----------------------------|-------------------------------------------|
| `ACTIVEwin*.zip`           | Windows 10 and 11, 64-bit                 |
| `ACTIVEmac*.dmg`           | macOS, Universal binary                   |
| `ACTIVELinux-readme.txt`   | Linux installation notes                  |
| `license-agreement.txt`    | End user license agreement                |

For step-by-step install instructions, see [Getting Started](https://www.activefirmwaretools.com/usersmanualold/getting-started) in the [user's manual](https://www.activefirmwaretools.com/usersmanual).

## Hardware support

- **Active Debugger** records streaming data through a single Active Debug Port over USB 2.0.
- **Active-Pro** records through up to four Active Debug Ports with higher sample rates and integrated logic analyzer, oscilloscope, and signal generator features.
- **Active-Pro Ultra** adds super-speed USB 3.0 streaming for the highest sample rates and longest captures.

## Linux notes

Read `ACTIVELinux-readme.txt` for distribution-specific install steps and USB permission setup. The application installs a udev rule on first launch so non-root users can access the USB device. If you run into issues, see [Troubleshooting](https://www.activefirmwaretools.com/usersmanualold/main-window-cy4gh) in the user's manual.

## License

The Active-Pro application is proprietary software. See `license-agreement.txt` in this folder for the full end user license. The hardware and the firmware in the [active-debug-port-source-code](../active-debug-port-source-code) folder are open source under the MIT License.

## Support

For support, email [support@activefirmwaretools.com](mailto:support@activefirmwaretools.com) or visit [www.activefirmwaretools.com](https://www.activefirmwaretools.com).
