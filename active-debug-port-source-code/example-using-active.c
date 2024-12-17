/* =============================================================
   Example Project Using ACTIVE-PRO Firmware Debugger
   Provided by activefirmwaretools.com

   This example demonstrates:
   1. Initializing the ACTIVE Debug Interface
   2. Sending debug information: values, text, and formatted messages
   3. Integration of active.c into an embedded firmware project
=============================================================== */

#include "active.h"   // Include the ACTIVE Debug Routines

/* =============================================================
   Hardware Setup: Includes and Definitions
=============================================================== */

// Include your hardware-specific library or headers here
#include "project.h"   // For PSoC Projects, replace with your MCU library

/* =============================================================
   Function: Main Routine
=============================================================== */
int main(void)
{
    // ============================================
    // Step 1: Initialize the Hardware
    // ============================================
    // Initialize the ACTIVE Debug Interface
    ACTIVEInit();
    // This function sets up the communication interface 
    // (e.g., SPI, UART, GPIO) for the ACTIVE-PRO debugger.
    // Modify ACTIVEInit in active.c to suit your hardware.

    // ============================================
    // Step 2: Sending Debug Values
    // ============================================
    int temperature = 25;        // Example integer value (temperature in °C)
    int voltage_level = 3300;    // Example integer value (voltage in millivolts)

    // Send debug values to ACTIVE-PRO
    ACTIVEValue(1, temperature);  // Send temperature on Channel 1
    ACTIVEValue(2, voltage_level); // Send voltage level on Channel 2

    // Explanation:
    // - Channel numbers are user-defined (0-63).
    // - ACTIVEValue encodes the integer into a compact format 
    //   and sends it through the interface.

    // ============================================
    // Step 3: Sending Debug Text Strings
    // ============================================
    // Send a simple text message to ACTIVE-PRO
    ACTIVEText(3, "System Initialization Complete");

    // Explanation:
    // - Channel 3 is used for text messages.
    // - The string "System Initialization Complete" is sent to ACTIVE-PRO.

    // ============================================
    // Step 4: Sending Formatted Messages
    // ============================================
    // Send formatted debug messages using ACTIVEprintf
    ACTIVEprintf(4, "Temperature: %d C, Voltage: %d mV", temperature, voltage_level);

    // Explanation:
    // - Channel 4 is used for formatted debug messages.
    // - ACTIVEprintf works like printf, formatting the message
    //   before sending it to ACTIVE-PRO.

    // ============================================
    // Step 5: Main Program Loop
    // ============================================
    while(1)
    {
        // Example debug updates in the loop
        temperature += 1;   // Simulated temperature increase
        voltage_level -= 10; // Simulated voltage drop

        // Send updated values periodically
        ACTIVEValue(1, temperature);
        ACTIVEValue(2, voltage_level);

        // Simulate some delay (replace with actual hardware delay function)
        CyDelay(1000); // 1-second delay (PSoC-specific, replace as needed)
    }

    // The ACTIVE Debug Interface will continuously send these debug values 
    // to the ACTIVE-PRO Debugger for real-time analysis.
}
