/* =============================================================
ACTIVE-PRO Firmware Debugger
Debug Output Routines
Converted to Rust by activefirmwaretools.com
===============================================================*/

#![allow(dead_code)] // For unused placeholders

use core::fmt::Write;

// Enable or disable ACTIVE Debug output
const ACTIVE_DEBUG_ON: bool = true;

// ===============================================================================================
// Modify these functions for your hardware interface setup
// ===============================================================================================

// Hardware initialization function
pub fn active_init() {
    // Replace these placeholders with your hardware initialization
    // Examples below assume usage of embedded-hal compatible crates
    
    #[cfg(feature = "spi")]
    {
        // SPI interface example setup (embedded-hal crate)
        println!("SPI hardware initialized");
    }

    #[cfg(feature = "uart")]
    {
        // UART interface example setup
        println!("UART hardware initialized");
    }

    #[cfg(feature = "gpio")]
    {
        // GPIO interface example setup
        println!("GPIO hardware initialized");
    }
}

// Function to send a packet to the ACTIVE Debug Interface
pub fn send_active_packet(data: &[u8]) {
    #[cfg(feature = "spi")]
    {
        // SPI transmission example
        println!("Sending packet via SPI: {:?}", data);
    }

    #[cfg(feature = "uart")]
    {
        // UART transmission example
        println!("Sending packet via UART: {:?}", data);
    }

    #[cfg(feature = "gpio")]
    {
        // GPIO-based bit-banging example
        for &byte in data {
            println!("Sending byte via GPIO: {:08b}", byte);
        }
    }
}

// ===============================================================================================
// ACTIVE Debug Routines (Enabled if ACTIVE_DEBUG_ON is true)
// ===============================================================================================

#[cfg(feature = "debug")]
pub mod active_debug {
    use super::*;

    const MAX_ACTIVE_LENGTH: usize = 255; // Maximum debug message length

    pub fn active_value(channel: u8, mut value: i32) {
        let mut buffer = Vec::with_capacity(MAX_ACTIVE_LENGTH);
        buffer.push(0x7F); // Start packet
        buffer.push(channel & 0x3F); // Type and channel

        let positive = value >= 0;

        // Encode value
        while value != 0 {
            if (positive && value >= 32) || (!positive && value < -32) {
                buffer.push((value & 0x3F) as u8);
                value >>= 6;
            } else {
                buffer.push(((value & 0x3F) | 0x40) as u8);
                break;
            }
        }

        // Send packet
        send_active_packet(&buffer);
    }

    pub fn active_text(channel: u8, string: &str) {
        let mut buffer = Vec::with_capacity(MAX_ACTIVE_LENGTH);
        buffer.push(0x7F); // Start packet
        buffer.push(0x40 | (channel & 0x3F)); // Type and channel

        for c in string.chars() {
            if buffer.len() >= MAX_ACTIVE_LENGTH - 1 {
                break;
            }
            buffer.push(c as u8 & 0x7F);
        }

        buffer.push(0); // Null-terminate
        send_active_packet(&buffer);
    }

    use core::fmt::Write;
    pub fn active_printf(channel: u8, format: core::fmt::Arguments) {
        let mut string_buffer = String::with_capacity(MAX_ACTIVE_LENGTH);
        string_buffer.write_fmt(format).unwrap();
        active_text(channel, &string_buffer);
    }
}

// ===============================================================================================
// Empty Debug Stubs if ACTIVE_DEBUG_ON is disabled
// ===============================================================================================

#[cfg(not(feature = "debug"))]
pub mod active_debug {
    pub fn active_value(_channel: u8, _value: i32) {}
    pub fn active_text(_channel: u8, _string: &str) {}
    pub fn active_printf(_channel: u8, _format: core::fmt::Arguments) {}
}
