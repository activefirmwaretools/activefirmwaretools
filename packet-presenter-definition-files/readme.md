# Packet Presenter Definition Files

## Overview

Packet Presenter definition files turn raw bytes on a channel into structured, human-readable events: register names, field decodes, frame types, command meanings, anything you can describe with a parser. Drop a definition file into the Active-Pro application and assign it to a channel to decode incoming data on the fly.

For usage instructions and the definition file format, see [PacketPresenter](https://www.activefirmwaretools.com/usersmanualold/main-window-24hr5) in the [user's manual](https://www.activefirmwaretools.com/usersmanual).

## Included definitions

| File                                            | Decodes                                                |
|-------------------------------------------------|--------------------------------------------------------|
| `i2c-generic-packet-presenter.txt`              | Generic I2C frames with address and direction          |
| `spi-generic-packet-presenter.txt`              | Generic SPI transactions                               |
| `bma400-i2c-packet-presenter.txt`               | Bosch BMA400 accelerometer over I2C                    |
| `tmp117-i2c-packet-presenter.txt`               | TI TMP117 temperature sensor over I2C                  |
| `cy3969-spi-packet-presenter.txt`               | Cypress CY3969 over SPI                                |
| `ds101-packet-presenter.txt`                    | DS101 frames                                           |
| `chsi-packet-presenter.txt`                     | CHSI frames                                            |
| `mdio-packet-presenter.txt`                     | MDIO management frames                                 |
| `one-wire-packet-presenter.txt`                 | 1-Wire bus traffic                                     |
| `uart-9bit-packet-presenter.txt`                | UART 9-bit frames                                      |
| `uart-ascii-CR-terminalted-packet-presenter.txt`| UART ASCII text terminated by carriage return         |
| `uart-ascii-null-terminated-packet-presenter.txt`| UART ASCII text terminated by null                    |
| `uart-binary-timeout-terminated-packet-presenter.txt`| UART binary frames terminated by inter-byte timeout |

## Writing your own

A Packet Presenter definition is a text file describing how to parse incoming bytes into named fields. The format supports fixed-width and variable-length fields, enumerated values, bitfield extraction, conditional parsing based on prior field values, and CRC and checksum validation.

The fastest way to start is to copy the closest existing definition and adapt it. The generic I2C, SPI, and UART files are good templates for protocol-level decoders. The BMA400 and TMP117 files show register-level decoding for specific parts.

## Contributing

If you build a definition for a part or protocol that other engineers might use, please consider sharing it back. Pull requests welcome. Include a short description of the part or protocol, sample captures so others can verify the decoder, and reference documentation links where the protocol is publicly specified.

## License

MIT. See [LICENSE](../LICENSE) at the repository root.

## Support

For support, email [support@activefirmwaretools.com](mailto:support@activefirmwaretools.com) or visit [www.activefirmwaretools.com](https://www.activefirmwaretools.com).
