
# Firmware for the Active Debugger

## Overview
This repository hosts the firmware for the Active Debugger, designed to operate with the single Active Debug Port for straightforward debugging of embedded systems. The firmware is crucial for interfacing with the hardware and enabling the core debugging functionalities.

It runs on an Infineon PSoC4 microprocessor (CY8C4248AZI-L475) that is designed into the Active Debugger invented by Active Firmware Tools and released as Open Source Hardware and firmware. You can find the Active Debugger hardware design in the GitHub rapository hardware directory.

## Contents
- Source code
- Compilation instructions
- Configuration files
- Flashing instructions
- Documentation

## License (MIT)

Copyright (c) 2025 Active Firmware Tools

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Contributing
Contributions are welcome! If you have suggestions or improvements, please fork the repository and submit a pull request, or open an issue to discuss the changes.

## Support
For questions and support, please email [support@activefirmwaretools.com].

## Acknowledgments
- Special thanks to all the contributors who have helped improve the firmware.
- Appreciation to the community for testing and feedback.


## Getting Started:
--------------------------------------------------------------------------------
1. Ensure you have the PSoC Creator installed (this project used version 4.2) 
available from Infineon

2. Import the project into your IDE:
   - open the ActiveFirmware.cywrk file located in this directory.

3. Configure your development board settings within the IDE as necessary to 
match your hardware setup.

4. Build the project:
   - Use the 'Build' option to compile the firmware.

5. Flash the firmware onto your PSoC4

