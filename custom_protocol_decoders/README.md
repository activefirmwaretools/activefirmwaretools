# ACTIVE-PRO Custom Decoders

Python protocol decoders for the **ACTIVE-PRO Debugger** app.

Each `*_Decoder.py` file in this folder is a self-contained protocol decoder. The app discovers it on startup, lists it in the Custom Python Decoder picker, and runs it against captured digital and analog samples after capture stops. Decoders are plain Python, fully editable. The picker rescans the decoder folder every time it opens, so dropping a new `.py` file in and reopening the picker is enough to see it appear.

## Bundled decoders

**Serial / asynchronous**

| Decoder | Signals | Notes |
| --- | --- | --- |
| `UART_Decoder.py` | 1 digital | Standard async serial. Configurable baud, data bits, parity, stop bits. |
| `RS232_Decoder.py` | 1 analog | Probe directly on a DB9 / DB25 wire. Thresholds +/-12 V at 0 V. |
| `RS485_Decoder.py` | 2 analog | Differential. Computes A,B per sample. |
| `MIDI_Decoder.py` | 1 digital | 31250 baud. Decodes Note On/Off, CC, SysEx, and friends. |
| `DMX512_Decoder.py` | 1 digital | Lighting protocol on the TTL side (use RS-485 for the wire). |
| `Modbus_RTU_Decoder.py` | 1 digital | UART framing plus Modbus PDU parsing with >=3.5 char idle detection. |

**Synchronous serial**

| Decoder | Signals | Notes |
| --- | --- | --- |
| `SPI_Decoder.py` | 4 digital | CS / SCK / MOSI / MISO. Configurable mode, bit order, word size. |
| `QSPI_Decoder.py` | 6 digital | CLK / CS / IO0,IO3. Supports 1-1-1, 1-1-2, 1-1-4, 1-4-4, 4-4-4 lane widths. |
| `I2C_Decoder.py` | 2 digital | SCL / SDA with START/STOP/repeated-start, ACK/NACK, 7 and 10 bit addressing. |
| `SMBus_Decoder.py` | 2 digital | Strict I2C superset. Reports command codes and PEC. |
| `PMBus_Decoder.py` | 2 digital | PMBus 1.x on SMBus. Named commands plus LINEAR11 / LINEAR16 decoding. |
| `I3C_Decoder.py` | 2 digital | MIPI I3C v1.1 SDR. 0x7E preamble, full CCC table, ENTDAA, IBI, HDR entry. |
| `I2S_Decoder.py` | 3 digital | BCLK / LRCLK / SD audio. Configurable bits per word. |
| `PS2_Decoder.py` | 2 digital | CLK / DATA. Keyboard or mouse byte streams. |

**Single-wire**

| Decoder | Signals | Notes |
| --- | --- | --- |
| `OneWire_Decoder.py` | 1 digital | Dallas / Maxim 1-Wire. RESET / PRESENCE plus ROM and function commands. |
| `DHT_Decoder.py` | 1 digital | DHT11 / DHT22 (AM2302) humidity and temperature frames. |
| `WS2812_Decoder.py` | 1 digital | WS2812 / WS2812B / SK6812 pulse-coded RGB pixels. |
| `Manchester_Decoder.py` | 1 digital | Manchester encoding (Thomas or IEEE convention). Configurable bit rate and word size. |

**Automotive / industrial**

| Decoder | Signals | Notes |
| --- | --- | --- |
| `CAN_Decoder.py` | 2 analog | ISO 11898 CAN 2.0A/B on the CANH/CANL differential pair. |
| `LIN_Decoder.py` | 1 analog | ISO 17987 LIN. Single-wire 12 V automotive bus, UART stream framed by long BREAK. |
| `HART_Decoder.py` | 1 analog | Bell 202 FSK riding on a 4-20 mA loop. 1200 baud, mark 1200 Hz / space 2200 Hz. |
| `Loop4_20mA_Decoder.py` | 1 analog | 4-20 mA current loop across a shunt. Linear engineering-unit mapping. |

**Embedded / display / debug**

| Decoder | Signals | Notes |
| --- | --- | --- |
| `JTAG_Decoder.py` | 4 digital | IEEE 1149.1. TCK / TMS / TDI / TDO state-machine and IR/DR tracking. |
| `SWD_Decoder.py` | 2 digital | ARM Serial Wire Debug. SWCLK / SWDIO with packet framing. |
| `HD44780_Decoder.py` | 6 digital | Character LCD, 4-bit mode (RS, E, D4,D7). Common Arduino wiring. |
| `RGMII_Decoder.py` | 6 digital | One direction of an RGMII PHY (RX or TX). Use two decoder slots for both directions. |

**Timing / measurement**

| Decoder | Signals | Notes |
| --- | --- | --- |
| `PWM_Decoder.py` | 1 digital | Per-cycle duty (%) and frequency (Hz). |
| `Quadrature_Decoder.py` | 2-3 digital | A/B encoder with optional Z index. x1/x2/x4 decode modes. |

**Template and helpers**

| File | Purpose |
| --- | --- |
| `decoder_template.py` | Starter template. Documents every PARAM type and every condition factory. Copy and edit. |
| `active_pro.py` | Runtime library that decoders import. Provides `wait_for`, `wait_time`, `append`, and all condition factories. |
| `test_decoder_digital.py` | Smoke test exercising digital edge / level conditions. |
| `test_decoder_analog.py` | Smoke test exercising analog threshold / hysteresis conditions. |
| `test_decoder_mixed.py` | Smoke test combining digital and analog with `all_of` / `any_of` / `not_`. |

## Using a decoder

The decoders ship with the app, no install needed. Open the decoder picker, choose a protocol, assign its channels, set its parameters, and run a capture. Decoded output appears on the protocol's row beneath the waveform.

## Writing your own

Copy `decoder_template.py`, rename it to `MyProtocol_Decoder.py`, drop it in your decoders folder, and hit Reload in the picker. The template doubles as the API reference: every parameter type and every condition factory is documented inline.

The shape of a decoder:

```python
# DECODER_MYNAME = "My Protocol"
#
# One-line description shown in the picker.
#
# PARAM: channel | Logic Channel | digital_channel |  |
# PARAM: baud    | Baud Rate     | select | 9600,115200 | 115200

from active_pro import (
    append, wait_for, wait_time,
    rising_edge, falling_edge, high, low,
    voltage_rises, voltage_falls, voltage_above, voltage_below, voltage_between,
    all_of, any_of, not_,
)

def decode(params):
    ch   = int(params["channel"])
    baud = int(params["baud"])

    while True:
        m = yield from wait_for(falling_edge(ch))
        if m is None:
            return
        append(m.t, m.t, 0, text="start bit")
```

Key concepts:

- **One notion of time.** Two and only two ways to advance it: `yield from wait_for(cond)` and `yield from wait_time(seconds)`. Both return a `Moment` snapshot (`m.t`, `m.d[0..7]`, `m.bits`, `m.a[0..7]`) or `None` at end-of-capture.
- **Composable conditions.** Edge and level factories for both digital and analog, combined with `all_of`, `any_of`, `not_`. Analog conditions take optional hysteresis for Schmitt-style debouncing.
- **Output via `append`.** Each decoder slot owns 8 output rows (channels 0,7). `sample_type` markers (`SAMPLE_PACKET_START` / `SAMPLE_PACKET_END`) let PacketPresenter scripts group bytes into frames downstream.
- **Parameters.** Each `PARAM:` comment becomes a UI control in the decoder picker. Values arrive in `params` as strings; cast at the top of `decode()`.

## Contributing

Pull requests for new protocol decoders or fixes to existing ones are welcome. A good decoder PR:

1. Lives in a single `<Protocol>_Decoder.py` file.
2. Starts with a `DECODER_MYNAME` line plus a one-sentence description.
3. Declares every input channel via `PARAM: ... | digital_channel |` or `analog_channel`, so the app can auto-enable the right channels and warn on collisions.
4. Documents its output rows in the header comment (channel 0 does X, channel 1 does Y, and so on).
5. Handles `m is None` after every wait. Decoders that ignore end-of-capture will hang.
6. Avoids busy loops: if you find yourself polling, you almost certainly want `wait_for(...)` with a compound condition instead.

## License

These decoders ship with ACTIVE-PRO Debugger and are free to modify, redistribute, and use as the basis for your own protocol work.
