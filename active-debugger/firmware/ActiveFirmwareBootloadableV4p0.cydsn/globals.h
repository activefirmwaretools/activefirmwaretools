/**
 * @file: globals.h 
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

#ifdef DECLARE_VARIABLES 
#define ACTIVE_DEF	/* */
#else
#define ACTIVE_DEF	extern
#endif

#include <project.h>

#define BUFFER_LEN  256u
ACTIVE_DEF uint8 USBBuffer[BUFFER_LEN];
ACTIVE_DEF uint8 tUSBBuffer[BUFFER_LEN];

#define MAX_COMMAND_LENGTH  256
#define ACTIVE_COMMAND_DEPTH 32

// Make a buffer to hold the data stream to the PC.
#define BUFFERSIZE  0x00004000L
#define BUFFERMASK  (~BUFFERSIZE)
ACTIVE_DEF uint8 BigBuffer[BUFFERSIZE] __attribute__((aligned(64)));
ACTIVE_DEF uint8 *TxHead;
ACTIVE_DEF uint8 *TxTail;
ACTIVE_DEF uint8 *TxStart;      // pointer to start of the buffer
ACTIVE_DEF uint8 *TxEnd;        // pointer to end of the buffer

#define WriteByte(data)  {*TxHead++ = (data); if (TxHead >= TxEnd) TxHead = TxStart;}
#define RoomLeft      ((TxTail > TxHead) ? (uint32)TxTail-(uint32)TxHead-1 : (BUFFERSIZE- 1) - (uint32)TxHead + (uint32)TxTail )
#define CanIWrite(length)  (RoomLeft > length)
#define RoomUsed      (BUFFERSIZE - RoomLeft)
uint8 WriteBytes( uint8 *data, uint32 count );

ACTIVE_DEF uint32 OldTime;

// Buffer of Active Commands, filled by the DMAs

ACTIVE_DEF uint32 ActiveTimestamp[ACTIVE_COMMAND_DEPTH];      
ACTIVE_DEF uint8 ActiveBufferLength[ACTIVE_COMMAND_DEPTH];      
ACTIVE_DEF uint8 ActiveBufferHead;      
ACTIVE_DEF uint8 ActiveBufferTail;      
ACTIVE_DEF uint8 LastActiveBufferTail;      
ACTIVE_DEF uint8 NextActiveBufferHead; 

ACTIVE_DEF uint32 DMA_A_Channel;
ACTIVE_DEF uint32 DMA_B_Channel;

ACTIVE_DEF uint8 ActiveBuffer[ACTIVE_COMMAND_DEPTH][MAX_COMMAND_LENGTH];   
ACTIVE_DEF uint8 OverflowBuffer[500]; 

