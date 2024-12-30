/*******************************************************************************
* File Name: Timestamp_ISR.c  
* Version 1.70
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <Timestamp_ISR.h>


#if !defined(Timestamp_ISR__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START Timestamp_ISR_intc` */
    
/**
 * @file: Timestamp_ISR.c 
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
    
#include "globals.h"
    uint32 timestamp;
    uint8 bytesA;
    uint8 bytesB;

/* `#END` */

extern cyisraddress CyRamVectors[CYINT_IRQ_BASE + CY_NUM_INTERRUPTS];

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);


/*******************************************************************************
* Function Name: Timestamp_ISR_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it. This function disables the interrupt, 
*  sets the default interrupt vector, sets the priority from the value in the
*  Design Wide Resources Interrupt Editor, then enables the interrupt to the 
*  interrupt controller.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Timestamp_ISR_Start(void)
{
    /* For all we know the interrupt is active. */
    Timestamp_ISR_Disable();

    /* Set the ISR to point to the Timestamp_ISR Interrupt. */
    Timestamp_ISR_SetVector(&Timestamp_ISR_Interrupt);

    /* Set the priority. */
    Timestamp_ISR_SetPriority((uint8)Timestamp_ISR_INTC_PRIOR_NUMBER);

    /* Enable it. */
    Timestamp_ISR_Enable();
}


/*******************************************************************************
* Function Name: Timestamp_ISR_StartEx
********************************************************************************
*
* Summary:
*  Sets up the interrupt and enables it. This function disables the interrupt,
*  sets the interrupt vector based on the address passed in, sets the priority 
*  from the value in the Design Wide Resources Interrupt Editor, then enables 
*  the interrupt to the interrupt controller.
*  
*  When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*  used to provide consistent definition across compilers:
*  
*  Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*   Function prototype example:
*   CY_ISR_PROTO(MyISR);
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void Timestamp_ISR_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    Timestamp_ISR_Disable();

    /* Set the ISR to point to the Timestamp_ISR Interrupt. */
    Timestamp_ISR_SetVector(address);

    /* Set the priority. */
    Timestamp_ISR_SetPriority((uint8)Timestamp_ISR_INTC_PRIOR_NUMBER);

    /* Enable it. */
    Timestamp_ISR_Enable();
}


/*******************************************************************************
* Function Name: Timestamp_ISR_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Timestamp_ISR_Stop(void)
{
    /* Disable this interrupt. */
    Timestamp_ISR_Disable();

    /* Set the ISR to point to the passive one. */
    Timestamp_ISR_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: Timestamp_ISR_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for Timestamp_ISR.
*
*   Add custom code between the START and END comments to keep the next version
*   of this file from over-writing your code.
*
*   Note You may use either the default ISR by using this API, or you may define
*   your own separate ISR through ISR_StartEx().
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(Timestamp_ISR_Interrupt)
{
    #ifdef Timestamp_ISR_INTERRUPT_INTERRUPT_CALLBACK
        Timestamp_ISR_Interrupt_InterruptCallback();
    #endif /* Timestamp_ISR_INTERRUPT_INTERRUPT_CALLBACK */ 

    /*  Place your Interrupt code here. */
    /* `#START Timestamp_ISR_Interrupt` */
    
    if (AB_SELECT_Status)       // Data is now being DMAed and Counted using channel B, so capture the number of bytes we got from the previous
                                // message on channel A.  Also reset the Channel A DMA to point to then next location in the buffer for when that 
                                // DMA starts
    {
        // Catch the next interrupt
        *Timestamp_ISR_INTC_CLR_PD = Timestamp_ISR__INTC_MASK;
 
        bytesA = COUNTERA_COUNTER_REG;
        COUNTERA_COUNTER_REG = 0;
        
        // Set the next DMA up to the new location in the buffer
        CYDMA_DESCR_BASE.descriptor[DMA_A_CHANNEL][0].dst = (void*)&ActiveBuffer[NextActiveBufferHead][0];
        CYDMA_DESCR_BASE.descriptor[DMA_A_CHANNEL][0].status = CYDMA_VALID;
    
        // Read the timestamp
        (void)CY_GET_REG8(TimestampTimer_COUNTER_LSB_PTR_8BIT);
        timestamp = (CY_GET_REG32(TimestampTimer_CAPTURE_LSB_PTR));


        // Save the length and timestamp
        ActiveBufferLength[ActiveBufferHead] = bytesA; 
        ActiveBufferHead++;
        ActiveBufferHead &= 0x1F;   // Wrap
        ActiveTimestamp[ActiveBufferHead] = timestamp;      // This timestamp is for the DMA data that is coming in for this new message
        NextActiveBufferHead++;
        NextActiveBufferHead &= 0x1F;   // Wrap
    }
    else
    {
        // Catch the next interrupt
        *Timestamp_ISR_INTC_CLR_PD = Timestamp_ISR__INTC_MASK;

        bytesB = COUNTERB_COUNTER_REG;
        COUNTERB_COUNTER_REG = 0;
        
        // Set the DMA up to the new location in the buffer
        CYDMA_DESCR_BASE.descriptor[DMA_B_CHANNEL][0].dst = (void*)&ActiveBuffer[NextActiveBufferHead][0];
        CYDMA_DESCR_BASE.descriptor[DMA_B_CHANNEL][0].status = CYDMA_VALID;

        // Read the timestamp
        (void)CY_GET_REG8(TimestampTimer_COUNTER_LSB_PTR_8BIT);
        timestamp = (CY_GET_REG32(TimestampTimer_CAPTURE_LSB_PTR));

        // Save the length and timestamp
        ActiveBufferLength[ActiveBufferHead] = bytesB; 
        ActiveBufferHead++;
        ActiveBufferHead &= 0x1F;   // Wrap
        ActiveTimestamp[ActiveBufferHead] = timestamp;     // This timestamp is for the DMA data that is coming in for this new message
        NextActiveBufferHead++;
        NextActiveBufferHead &= 0x1F;   // Wrap
    }

    /* `#END` */
}


/*******************************************************************************
* Function Name: Timestamp_ISR_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling Timestamp_ISR_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use Timestamp_ISR_StartEx instead.
* 
*   When defining ISR functions, the CY_ISR and CY_ISR_PROTO macros should be 
*   used to provide consistent definition across compilers:
*
*   Function definition example:
*   CY_ISR(MyISR)
*   {
*   }
*
*   Function prototype example:
*     CY_ISR_PROTO(MyISR);
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void Timestamp_ISR_SetVector(cyisraddress address)
{
    CyRamVectors[CYINT_IRQ_BASE + Timestamp_ISR__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: Timestamp_ISR_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress Timestamp_ISR_GetVector(void)
{
    return CyRamVectors[CYINT_IRQ_BASE + Timestamp_ISR__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: Timestamp_ISR_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. 
*
*   Note calling Timestamp_ISR_Start or Timestamp_ISR_StartEx will 
*   override any effect this API would have had. This API should only be called
*   after Timestamp_ISR_Start or Timestamp_ISR_StartEx has been called. 
*   To set the initial priority for the component, use the Design-Wide Resources
*   Interrupt Editor.
*
*   Note This API has no effect on Non-maskable interrupt NMI).
*
* Parameters:
*   priority: Priority of the interrupt, 0 being the highest priority
*             PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*             PSoC 4: Priority is from 0 to 3.
*
* Return:
*   None
*
*******************************************************************************/
void Timestamp_ISR_SetPriority(uint8 priority)
{
	uint8 interruptState;
    uint32 priorityOffset = ((Timestamp_ISR__INTC_NUMBER % 4u) * 8u) + 6u;
    
	interruptState = CyEnterCriticalSection();
    *Timestamp_ISR_INTC_PRIOR = (*Timestamp_ISR_INTC_PRIOR & (uint32)(~Timestamp_ISR__INTC_PRIOR_MASK)) |
                                    ((uint32)priority << priorityOffset);
	CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Timestamp_ISR_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt, 0 being the highest priority
*    PSoC 3 and PSoC 5LP: Priority is from 0 to 7.
*    PSoC 4: Priority is from 0 to 3.
*
*******************************************************************************/
uint8 Timestamp_ISR_GetPriority(void)
{
    uint32 priority;
	uint32 priorityOffset = ((Timestamp_ISR__INTC_NUMBER % 4u) * 8u) + 6u;

    priority = (*Timestamp_ISR_INTC_PRIOR & Timestamp_ISR__INTC_PRIOR_MASK) >> priorityOffset;

    return (uint8)priority;
}


/*******************************************************************************
* Function Name: Timestamp_ISR_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt to the interrupt controller. Do not call this function
*   unless ISR_Start() has been called or the functionality of the ISR_Start() 
*   function, which sets the vector and the priority, has been called.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Timestamp_ISR_Enable(void)
{
    /* Enable the general interrupt. */
    *Timestamp_ISR_INTC_SET_EN = Timestamp_ISR__INTC_MASK;
}


/*******************************************************************************
* Function Name: Timestamp_ISR_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 Timestamp_ISR_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*Timestamp_ISR_INTC_SET_EN & (uint32)Timestamp_ISR__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: Timestamp_ISR_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt in the interrupt controller.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Timestamp_ISR_Disable(void)
{
    /* Disable the general interrupt. */
    *Timestamp_ISR_INTC_CLR_EN = Timestamp_ISR__INTC_MASK;
}


/*******************************************************************************
* Function Name: Timestamp_ISR_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
* Side Effects:
*   If interrupts are enabled and the interrupt is set up properly, the ISR is
*   entered (depending on the priority of this interrupt and other pending 
*   interrupts).
*
*******************************************************************************/
void Timestamp_ISR_SetPending(void)
{
    *Timestamp_ISR_INTC_SET_PD = Timestamp_ISR__INTC_MASK;
}


/*******************************************************************************
* Function Name: Timestamp_ISR_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt in the interrupt controller.
*
*   Note Some interrupt sources are clear-on-read and require the block 
*   interrupt/status register to be read/cleared with the appropriate block API 
*   (GPIO, UART, and so on). Otherwise the ISR will continue to remain in 
*   pending state even though the interrupt itself is cleared using this API.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void Timestamp_ISR_ClearPending(void)
{
    *Timestamp_ISR_INTC_CLR_PD = Timestamp_ISR__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
