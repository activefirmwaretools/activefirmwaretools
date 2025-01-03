/*******************************************************************************
* File Name: TIME_0.c  
* Version 1.90
*
* Description:
*  This file contains API to enable firmware to read the value of a Status 
*  Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "TIME_0.h"

#if !defined(TIME_0_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: TIME_0_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The current value in the Status Register.
*
*******************************************************************************/
uint8 TIME_0_Read(void) 
{ 
    return TIME_0_Status;
}


/*******************************************************************************
* Function Name: TIME_0_InterruptEnable
********************************************************************************
*
* Summary:
*  Enables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void TIME_0_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    TIME_0_Status_Aux_Ctrl |= TIME_0_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: TIME_0_InterruptDisable
********************************************************************************
*
* Summary:
*  Disables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void TIME_0_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    TIME_0_Status_Aux_Ctrl &= (uint8)(~TIME_0_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: TIME_0_WriteMask
********************************************************************************
*
* Summary:
*  Writes the current mask value assigned to the Status Register.
*
* Parameters:
*  mask:  Value to write into the mask register.
*
* Return:
*  None.
*
*******************************************************************************/
void TIME_0_WriteMask(uint8 mask) 
{
    #if(TIME_0_INPUTS < 8u)
    	mask &= ((uint8)(1u << TIME_0_INPUTS) - 1u);
	#endif /* End TIME_0_INPUTS < 8u */
    TIME_0_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: TIME_0_ReadMask
********************************************************************************
*
* Summary:
*  Reads the current interrupt mask assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The value of the interrupt mask of the Status Register.
*
*******************************************************************************/
uint8 TIME_0_ReadMask(void) 
{
    return TIME_0_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
