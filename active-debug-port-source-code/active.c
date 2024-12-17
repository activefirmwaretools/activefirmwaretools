/* =============================================================
ACTIVE-PRO Firmware Debugger
Debug Output Routines 
Provided by activefirmwaretools.com

This file is to be included in your embedded firmware project to 
send debug information to the ACTIVE-PRO Firmware Debugger.

If you have any questions or issues, please email us at
support@activefirmwaretools.com.
===============================================================*/

#define ACTIVE_DEBUG_ON // Comment this line out to remove all ACTIVE Debug output from your project

//===============================================================================================
// MAKE MODIFICATIONS FOR YOUR HARDWARE SETUP BELOW THIS LINE
//===============================================================================================
// CHANGE #1:  Add any includes that you need for the interface to your hardware to the ACTIVE bus
#include "project.h"    // for PSoC Projects

// CHANGE #2:  Modify this routine to set up the interface from this processor to the ACTIVE bus
void ACTIVEInit( void )
{
    // Setup your hardware components for the ACTIVE Debug interface.  Uncomment the mode you want to use or create your own
    // based on the processor you are using.

    // 1-Wire UART Interface
    // Serial.begin(3000000, SERIAL_8N1, -1, 1);    // Use IO1 as the output signal at 3MBaud, No parity, 8 data bits, 1 stop bit  (Arduino Style)
    // UART_Start();                // Setup the UART Hardware Block (PSoC Style)
    
    // 2-wire SPI Interface
    // SPI.begin(18, -1, 19);       // Initialize the SPI block and assign pins
    // SPI.setFrequency(40000000);  // Set the SPI SCK frequency to 40MHz
    SPI_Start();                 // Setup the SPI hardware block (PSoC Style)

    // 2-wire GPIO Interface
    // pinMode(CLOCK_PIN, OUTPUT);   // Set the CLOCK to an Output (Arduino Style)
    // pinMode(DATA_PIN, OUTPUT);    // Set the DATA to an Output (Arduino Style)
    //ACTIVE_CLOCK_GPIO_SetDriveMode( PIN_DM_STRONG );  // Set the CLOCK to an Output (PSoC Style)
    //ACTIVE_DATA_GPIO_SetDriveMode( PIN_DM_STRONG );   // Set the DATA to an Output (PSoC Style)
    
    // SWV Interface (UART Mode Only 8,N,1)
    // SWV_Start();    // Setup the SWV hardware (PSoC Style).  Replace this with your processors SWV setup routine.
    
}

// CHANGE #3:  Modify this routine to send an array of bytes to the the ACTIVE Debug Interface
void SendACTIVEPacket( unsigned char *data, int length )   // This routine is used to send a packet to the ACTIVE Debug Interface
{
    // Send the packet out the wires.  Choose the one type you have selected for this processor.

    // 2-wire SPI Interface
    // SPI.writeBytes( data, length );   // Send the packet directly to the SPI hardware (ESP32 Arduino version)
    SPI_PutArray( data, length );
    
    // 1-wire UART Interface
    // Serial.write( data, length );   // Send the packet directly to the UART hardware (ESP32 Arduino version)
    // UART_PutArray( data, length );  // Send the packet directly to the UART hardware (PSoC version)
    
    // 2-wire GPIO Interface
    // unsigned char value;
    // while(length--)
    // {
    //      // Send the next byte of data
    //      value = *data++;
    //      // Set the DATA line, and toggle the CLOCK for each bit, MSbit first  (PSoC Style)  
    //      //  Replace the calls here for your calls to set the GPIO output level of the DATA and CLOCK lines.
    //    
    //      if (value & 0x80) ACTIVE_DATA_GPIO_Write(1); else  ACTIVE_DATA_GPIO_Write(0); ACTIVE_CLOCK_GPIO_Write(1); ACTIVE_CLOCK_GPIO_Write(0);  
    //      if (value & 0x40) ACTIVE_DATA_GPIO_Write(1); else  ACTIVE_DATA_GPIO_Write(0); ACTIVE_CLOCK_GPIO_Write(1); ACTIVE_CLOCK_GPIO_Write(0);  
    //      if (value & 0x20) ACTIVE_DATA_GPIO_Write(1); else  ACTIVE_DATA_GPIO_Write(0); ACTIVE_CLOCK_GPIO_Write(1); ACTIVE_CLOCK_GPIO_Write(0);  
    //      if (value & 0x10) ACTIVE_DATA_GPIO_Write(1); else  ACTIVE_DATA_GPIO_Write(0); ACTIVE_CLOCK_GPIO_Write(1); ACTIVE_CLOCK_GPIO_Write(0);  
    //      if (value & 0x08) ACTIVE_DATA_GPIO_Write(1); else  ACTIVE_DATA_GPIO_Write(0); ACTIVE_CLOCK_GPIO_Write(1); ACTIVE_CLOCK_GPIO_Write(0);  
    //      if (value & 0x04) ACTIVE_DATA_GPIO_Write(1); else  ACTIVE_DATA_GPIO_Write(0); ACTIVE_CLOCK_GPIO_Write(1); ACTIVE_CLOCK_GPIO_Write(0);  
    //      If (value & 0x02) ACTIVE_DATA_GPIO_Write(1); else  ACTIVE_DATA_GPIO_Write(0); ACTIVE_CLOCK_GPIO_Write(1); ACTIVE_CLOCK_GPIO_Write(0);  
    //      if (value & 0x01) ACTIVE_DATA_GPIO_Write(1); else  ACTIVE_DATA_GPIO_Write(0); ACTIVE_CLOCK_GPIO_Write(1); ACTIVE_CLOCK_GPIO_Write(0);  
    // }
    
    // SWV Interface (UART Mode Only 8,N,1)
    // while(length--) ITM_SendChar( *data++ );   // Send the packet directly to the SWV Hardware Block (PSoC version)

} 
//===============================================================================================
// MAKE MODIFICATIONS FOR YOUR HARDWARE SETUP ABOVE THIS LINE ONLY
//===============================================================================================

//===============================================================================================
// Define the basic Value and Text ACTIVE Message transmit routines
//===============================================================================================
#ifdef ACTIVE_DEBUG_ON

#define MAX_ACTIVE_LENGTH 255 // This defines the maximum length of any debug message
unsigned char ACTIVETxBuffer[MAX_ACTIVE_LENGTH];    // Holds the debug packet as it is being built

void ACTIVEValue( int channel, int value )
{
    int length = 0;
    char done = 0;
    char positive = (value >= 0);
    
    // Assemble the ACTIVE Value Packet

    ACTIVETxBuffer[length++] = 0x7F;           // Every ACTIVE packet starts with a 0x7F
    ACTIVETxBuffer[length++] = channel & 0x3F; // Type and Channel: Bit 7=0, Bit 6=0 Means Value, Bits 5:0 means the channel (0-63)

    while(!done)
    {
        if ((positive && (value >= 32)) || (!positive && (value < -32)))
        {
            ACTIVETxBuffer[length++] = value & 0x3F;
            value = value >> 6;
        }
        else
        {
            ACTIVETxBuffer[length++] =  (value & 0x3F ) | 0x40;
            done = 1;
        }
    }

    // Now actually send the packet to the Active-Pro
    SendACTIVEPacket(ACTIVETxBuffer, length);
}

void ACTIVEText( int channel, char *string )
{
    int length = 0;

    ACTIVETxBuffer[length++] = 0x7F;   // Every ACTIVE packet starts with a 0x7F
    ACTIVETxBuffer[length++] = 0x40 | (channel & 0x3F); // Type and Channel: Bit 7=0, Bit 6=1 Means Text, Bits 5:0 means the channel (0-63)
    
    while(*string)
    {
        if (length >= MAX_ACTIVE_LENGTH-1)
            break;
        ACTIVETxBuffer[length++] = *string & 0x7F;     // Send the ascii characters
        string++;
        
    }
    ACTIVETxBuffer[length++] = 0;     // Send the string termination
        
    // Now actually send the packet to the Active-Pro
    SendACTIVEPacket(ACTIVETxBuffer, length);

}

//===============================================================================================
// Define the printf-like ACTIVE message routine
//===============================================================================================
#include "stdio.h"
#include <stdarg.h>   // va_list, va_start, and va_end
char ACTIVEstr[MAX_ACTIVE_LENGTH];
void ACTIVEprintf( int channel, char *format, ... )
{
    va_list arglist;
    va_start( arglist, format );
    vsprintf( ACTIVEstr, format, arglist );
    va_end( arglist );
    ACTIVEText( channel, ACTIVEstr );
};

#else       // ACTIVE Debug is turned off, so make empty stubs for all routines
void ACTIVEInit( void ) {};
void ACTIVEText( int channel, char *string ) {};
void ACTIVEValue( int channel, int value ) {};
void ACTIVEprintf( int channel, char *format, ... ) {};
#endif
