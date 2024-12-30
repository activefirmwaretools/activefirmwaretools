/*******************************************************************************
* File Name: AB_SELECT.c  
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

#include "AB_SELECT.h"

#if !defined(AB_SELECT_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: AB_SELECT_Read
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
uint8 AB_SELECT_Read(void) 
{ 
    return AB_SELECT_Status;
}


/*******************************************************************************
* Function Name: AB_SELECT_InterruptEnable
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
void AB_SELECT_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    AB_SELECT_Status_Aux_Ctrl |= AB_SELECT_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: AB_SELECT_InterruptDisable
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
void AB_SELECT_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    AB_SELECT_Status_Aux_Ctrl &= (uint8)(~AB_SELECT_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: AB_SELECT_WriteMask
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
void AB_SELECT_WriteMask(uint8 mask) 
{
    #if(AB_SELECT_INPUTS < 8u)
    	mask &= ((uint8)(1u << AB_SELECT_INPUTS) - 1u);
	#endif /* End AB_SELECT_INPUTS < 8u */
    AB_SELECT_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: AB_SELECT_ReadMask
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
uint8 AB_SELECT_ReadMask(void) 
{
    return AB_SELECT_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
