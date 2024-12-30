/**
 * @file: main.c 
 * @brief: Main routines of the Active Debugger firmware project that runs on the 
 *         PSoC4 CY8C4248AZI-L475 processor on the Active Debugger Board.  This
 *         project is built using the PSoC Creator version 4.2 available from Infineon.
 * 
 * Support the developer buy purchasing the ready to use Active Debugger or upgrading
 * to the Active-Pro Debugger & Logic Analyzer at https://www.activefirmwaretools.com
 * 
 * Author: Tim Harvey
 * Company: Active Firmware Tools
 * Copyright (C) 2025 by Active Firmware Tools
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define DECLARE_VARIABLES
#include "globals.h"
#include "project.h"

uint8 SendActiveData( void )
{
    // This routine moves each Active Debug Port message into the outgoing BigBuffer that feeds the USB port
    // First, validate if the Active DMA_A or DMA_B are over the memory buffer and reset everything if so.
    
    if ((DMA_A_GetDstAddress(0) >= (void*)&ActiveBuffer[ACTIVE_COMMAND_DEPTH][0]) ||
        (DMA_B_GetDstAddress(0) >= (void*)&ActiveBuffer[ACTIVE_COMMAND_DEPTH][0]))
    {
        // Someone went rogue! so reset everything
        CyGlobalIntDisable; /* Disable global interrupts */
        
        DMA_A_Start((void*)ACTIVE_DATA_Status_PTR, &ActiveBuffer[0][0]);
        DMA_B_Start((void*)ACTIVE_DATA_Status_PTR, &ActiveBuffer[1][0]);
        
        DMA_A_ChEnable();
        DMA_B_ChEnable();

        ActiveBufferHead = 0;
        ActiveBufferTail = 0;
      
        NextActiveBufferHead = 2;  // Where we will place the next message we setup

        CyGlobalIntEnable; /* Enable global interrupts */

        return(0);
    }
    
    while (ActiveBufferTail != ActiveBufferHead)
    {
        // How many bytes are in this message?
        uint8 Bytes = ActiveBufferLength[ActiveBufferTail];
        
        if (Bytes)
        {
            uint8 *sptr;
            sptr = &ActiveBuffer[ActiveBufferTail][0];
            
            // Each message started with a 0x7F on the Active Debug Port.
            // These 0x7F do not get captured.
            //
            // But, ocassionally, there is a 0x7F that is in ActiveValue data (at the very end).
            // So this logic below detects this extra 0x7F and ignores it.  
            //
            // The logic also determines if the ActiveValue message is missing it's ending 0x7F and adds it.
            
            // The first byte is the channel type and channel number. 
            if ((*sptr & 0x40) == 0)
            {
                // This is a Value message.  See if we are missing the last byte.  In that case it was a 0x7F
                if ((sptr[Bytes-1] & 0x40) == 0)
                {
                    // We are missing the last byte of this message and it is a 0x7F
                    Bytes++;
                    sptr[Bytes-1] = 0x7F;
                }
            }
            
            WriteByte( (uint8)(0x7E) );      
            
            // Timestamp (for this packet is in the next
            uint32 NewTime = ActiveTimestamp[ActiveBufferTail];        
            
            uint32 Delta = OldTime - NewTime;
            
            if (NewTime > OldTime)      // Counter wrap?
            {
                Delta = 0xFFFFFFFF - NewTime + OldTime + 1;
            }

            OldTime = NewTime;
 
            if ( Delta <= 0x7F )
            {
                WriteByte( (uint8)(0x80 | Delta) );      
            }
            else if (Delta < 0x3FFF)
            {
                WriteByte( (uint8)((Delta >>  7) & 0x7F) );      
                WriteByte( (uint8)(0x80 | Delta)); 
            }
            else if (Delta < 0x1FFFFF)
            {
                WriteByte( (uint8)((Delta >> 14) & 0x7F) );      
                WriteByte( (uint8)((Delta >>  7) & 0x7F) );      
                WriteByte( (uint8)(0x80 | Delta));      
            }
            else
            {
                WriteByte( (uint8)((Delta >> 21) & 0x7F) );      
                WriteByte( (uint8)((Delta >> 14) & 0x7F) );      
                WriteByte( (uint8)((Delta >>  7) & 0x7F) );      
                WriteByte( (uint8)(0x80 | Delta)); 
            }
            
            // Length
            WriteByte( (uint8)(Bytes) );
        
            // Active Message
            while (Bytes--)
            {
                WriteByte((uint8)(*sptr++));
            }
            
        }
        
        ActiveBufferLength[ActiveBufferTail] = 0;

        ActiveBufferTail++;
        if (ActiveBufferTail >= ACTIVE_COMMAND_DEPTH)
            ActiveBufferTail = 0;

    }
    
    return(1);
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts */

    // Reset the hardware and get teh DMAs ready
    RESET_AB_SELECT_Write(1);

    Timestamp_ISR_Start();
    Timestamp_ISR_Enable();
    Timestamp_ISR_ClearPending();
    
    COUNTERA_Start();
    COUNTERB_Start();
    
    RESET_TIMESTAMP_Write(1);
    RESET_TIMESTAMP_Write(0);

    TimestampTimer_Start();
 
    DMA_A_Start((void*)ACTIVE_DATA_Status_PTR, &ActiveBuffer[0][0]);
    DMA_B_Start((void*)ACTIVE_DATA_Status_PTR, &ActiveBuffer[1][0]);

    ActiveBufferHead = 0;
    ActiveBufferTail = 0;
  
    NextActiveBufferHead = 2; // Where we will place the next message we setup

    TxHead = &BigBuffer[0];
    TxTail = &BigBuffer[0];
    TxStart = &BigBuffer[0];
    TxEnd = &BigBuffer[BUFFERSIZE];
    
    RESET_AB_SELECT_Write(0);

    // Set the logic threshold
    IDAC_Start();
    IDAC_SetValue(64);
    Opamp_Start();
 
    COUNTER1A_ISR_Start();
    COUNTER1A_ISR_Enable();
    COUNTER1A_ISR_ClearPending();

    COUNTER2A_ISR_Start();
    COUNTER2A_ISR_Enable();
    COUNTER2A_ISR_ClearPending();

    COUNTER1A_Start();
    COUNTER2A_Start();
    
    USBUART_Start(0, USBUART_5V_OPERATION);

    OldTime = TimestampTimer_ReadCounter(); 
    
    for (;;)
    {
        // Send the Active Messages to the BigBuffer
        SendActiveData();
        
        // Flush the BigBuffer to the USB
        if(USBUART_IsConfigurationChanged() != 0u) /* Host could send double SET_INTERFACE request */
        {
            if(USBUART_GetConfiguration() != 0u)   /* Init IN endpoints when device configured */
            {
                /* Enumeration is done, enable OUT endpoint for receive data from Host */
                USBUART_CDC_Init();
            }
        }  
        
        if(USBUART_GetConfiguration() != 0u)    /* Service USB CDC when device configured */
        {
            // USB is fully up, so gather the data
            if(USBUART_DataIsReady() != 0u)               /* Check for input data from PC */
            {   
                USBUART_GetAll(USBBuffer);           /* Read received data and re-enable OUT endpoint */
                IDAC_SetValue(USBBuffer[0]);        // Set the Vthresh
            }  
        
            // Do we have a full packet (64) of data to send to the PC and the PC is ready?  If so send it
            uint32 SendLength = RoomUsed - 1;
            
            if (SendLength >= 64)
                SendLength = 64;
            
            if ( (SendLength > 2) && USBUART_CDCIsReady())
            {

                if (SendLength < 64)        // It'll be a short packet no mater what.
                {
                     // See if we are at the end of the buffer
                    if (TxEnd - TxTail < SendLength)
                        SendLength = TxEnd - TxTail;
                    
                    // Copy the data into the USB transmit buffer
                    USBUART_LoadInEP(2, TxTail, SendLength);
                    
                    TxTail += SendLength;
                    if (TxTail >= TxEnd) 
                        TxTail -= (BUFFERSIZE);
                }
                else    
                {
                    // See if we are at the end of the buffer
                    if (TxEnd - TxTail < SendLength)
                    {
                        uint8 tbuf[64];
                        uint8 length = TxEnd - TxTail;
                        
                        memcpy(tbuf, TxTail, length);
                        TxTail = TxStart;
                        memcpy(&tbuf[length], TxTail, SendLength - length);
                        TxTail += (SendLength - length);
                        
                        // Copy the data into the USB transmit buffer
                        USBUART_LoadInEP(2, tbuf, SendLength);
                    }
                    else
                    {
                        // Copy the data into the USB transmit buffer
                        USBUART_LoadInEP(2, TxTail, SendLength);
                        TxTail += SendLength;
                    }
                }
            }
        }
    }
}


