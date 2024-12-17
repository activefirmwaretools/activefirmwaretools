/* =============================================================
ACTIVE-PRO Firmware Debugger
Bare-Metal Rust Version with Direct Hardware GPIO Control
===============================================================*/

#![no_std]
#![no_main]

use core::ptr;

// Define GPIO peripheral base address (e.g., for ARM Cortex-M, STM32)
// Replace this with your specific microcontroller's base addresses
const GPIO_BASE: u32 = 0x4002_0800; // Example GPIO base address (STM32 GPIOB)

// GPIO register offsets
const MODER_OFFSET: u32 = 0x00;    // GPIO Mode Register
const BSRR_OFFSET: u32 = 0x18;     // GPIO Bit Set/Reset Register

// Pin masks for GPIO pins (e.g., DATA = Pin 0, CLOCK = Pin 1)
const DATA_PIN: u16 = 1 << 0;  // Pin 0 for DATA
const CLOCK_PIN: u16 = 1 << 1; // Pin 1 for CLOCK

// =======================================================================
// Hardware Initialization
// =======================================================================

// Initialize GPIO pins for DATA and CLOCK
fn active_init() {
    unsafe {
        let gpio_moder = (GPIO_BASE + MODER_OFFSET) as *mut u32;

        // Configure DATA and CLOCK pins as outputs (00: Input, 01: Output)
        let moder_val = ptr::read_volatile(gpio_moder);
        ptr::write_volatile(gpio_moder, moder_val | (0b01 << (0 * 2)) | (0b01 << (1 * 2)));

        // Initialize pins LOW
        set_pin_low(DATA_PIN);
        set_pin_low(CLOCK_PIN);
    }
}

// =======================================================================
// GPIO Pin Control Functions
// =======================================================================

// Set a GPIO pin HIGH
fn set_pin_high(pin: u16) {
    unsafe {
        let gpio_bsrr = (GPIO_BASE + BSRR_OFFSET) as *mut u32;
        ptr::write_volatile(gpio_bsrr, pin as u32);
    }
}

// Set a GPIO pin LOW
fn set_pin_low(pin: u16) {
    unsafe {
        let gpio_bsrr = (GPIO_BASE + BSRR_OFFSET) as *mut u32;
        ptr::write_volatile(gpio_bsrr, (pin as u32) << 16);
    }
}

// Toggle the CLOCK pin without delay
fn toggle_clock() {
    set_pin_high(CLOCK_PIN);
    set_pin_low(CLOCK_PIN);
}

// =======================================================================
// Packet Sending Routine: Bit-Banging GPIO
// =======================================================================

// Send a single byte via bit-banging
fn send_byte(byte: u8) {
    let mut value = byte;

    for _ in 0..8 {
        if value & 0x80 != 0 {
            set_pin_high(DATA_PIN);
        } else {
            set_pin_low(DATA_PIN);
        }

        toggle_clock();
        value <<= 1;
    }
}

// Send a packet of bytes
fn send_active_packet(data: &[u8]) {
    for &byte in data {
        send_byte(byte);
    }
}

// =======================================================================
// ACTIVE Debug Routines
// =======================================================================

// Send an integer value to the ACTIVE-PRO Debug Interface
fn active_value(channel: u8, mut value: i32) {
    let mut buffer = [0u8; 255];
    let mut length = 0;

    buffer[length] = 0x7F; // Start of packet
    length += 1;
    buffer[length] = channel & 0x3F;
    length += 1;

    loop {
        if value.abs() >= 32 {
            buffer[length] = (value & 0x3F) as u8;
            length += 1;
            value >>= 6;
        } else {
            buffer[length] = ((value & 0x3F) | 0x40) as u8;
            length += 1;
            break;
        }
    }

    send_active_packet(&buffer[0..length]);
}

// Send a text string to the ACTIVE-PRO Debug Interface
fn active_text(channel: u8, string: &str) {
    let mut buffer = [0u8; 255];
    let mut length = 0;

    buffer[length] = 0x7F; // Start of packet
    length += 1;
    buffer[length] = 0x40 | (channel & 0x3F);
    length += 1;

    for c in string.bytes() {
        if length >= buffer.len() - 1 {
            break;
        }
        buffer[length] = c & 0x7F;
        length += 1;
    }

    buffer[length] = 0; // Null terminator
    length += 1;

    send_active_packet(&buffer[0..length]);
}
