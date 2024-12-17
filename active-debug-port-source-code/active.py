# =============================================================
# ACTIVE-PRO Firmware Debugger
# Debug Output Routines 
# Provided by activefirmwaretools.com
#
# This file is to be included in your embedded firmware project to 
# send debug information to the ACTIVE-PRO Firmware Debugger.
#
# If you have any questions or issues, please email us at
# support@activefirmwaretools.com.
# =============================================================

# Define ACTIVE_DEBUG_ON to enable debug output
ACTIVE_DEBUG_ON = True

if ACTIVE_DEBUG_ON:
    import spidev
    import serial

    # Setup your hardware components for the ACTIVE Debug interface
    def ACTIVEInit():
        global spi, uart
        spi = spidev.SpiDev()
        spi.open(0, 0)  # Adjust bus and device as per your setup
        spi.max_speed_hz = 40000000

        uart = serial.Serial()
        uart.baudrate = 3000000
        uart.port = '/dev/ttyS0'  # Adjust as per your setup
        uart.open()

    # Send an array of bytes to the ACTIVE Debug Interface
    def SendACTIVEPacket(data):
        # Send the packet out the wires

        # 2-wire SPI Interface
        spi.xfer2(data)

        # 1-wire UART Interface
        uart.write(bytearray(data))

    MAX_ACTIVE_LENGTH = 255  # This defines the maximum length of any debug message
    ACTIVETxBuffer = bytearray(MAX_ACTIVE_LENGTH)  # Holds the debug packet as it is being built

    def ACTIVEValue(channel, value):
        length = 0
        done = False
        positive = value >= 0

        # Assemble the ACTIVE Value Packet
        ACTIVETxBuffer[length] = 0x7F
        length += 1
        ACTIVETxBuffer[length] = channel & 0x3F  # Type and Channel
        length += 1

        while not done:
            if (positive and value >= 32) or (not positive and value < -32):
                ACTIVETxBuffer[length] = value & 0x3F
                value >>= 6
                length += 1
            else:
                ACTIVETxBuffer[length] = (value & 0x3F) | 0x40
                done = True
                length += 1

        # Now actually send the packet to the Active-Pro
        SendACTIVEPacket(ACTIVETxBuffer[:length])

    def ACTIVEText(channel, string):
        length = 0

        ACTIVETxBuffer[length] = 0x7F
        length += 1
        ACTIVETxBuffer[length] = 0x40 | (channel & 0x3F)
        length += 1

        for char in string:
            if length >= MAX_ACTIVE_LENGTH - 1:
                break
            ACTIVETxBuffer[length] = ord(char) & 0x7F
            length += 1

        ACTIVETxBuffer[length] = 0  # Send the string termination
        length += 1

        # Now actually send the packet to the Active-Pro
        SendACTIVEPacket(ACTIVETxBuffer[:length])

    def ACTIVEprintf(channel, format_string, *args):
        ACTIVEstr = format_string % args
        ACTIVEText(channel, ACTIVEstr)

else:
    # ACTIVE Debug is turned off, so make empty stubs for all routines
    def ACTIVEInit():
        pass

    def ACTIVEText(channel, string):
        pass

    def ACTIVEValue(channel, value):
        pass

    def ACTIVEprintf(channel, format_string, *args):
        pass
