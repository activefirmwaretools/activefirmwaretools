/*******************************************************************************
* File Name: ACTIVE_DATA.c  
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

#include "ACTIVE_DATA.h"

#if !defined(ACTIVE_DATA_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: ACTIVE_DATA_Read
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
uint8 ACTIVE_DATA_Read(void) 
{ 
    return ACTIVE_DATA_Status;
}


/*******************************************************************************
* Function Name: ACTIVE_DATA_InterruptEnable
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
void ACTIVE_DATA_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    ACTIVE_DATA_Status_Aux_Ctrl |= ACTIVE_DATA_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: ACTIVE_DATA_InterruptDisable
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
void ACTIVE_DATA_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    ACTIVE_DATA_Status_Aux_Ctrl &= (uint8)(~ACTIVE_DATA_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: ACTIVE_DATA_WriteMask
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
void ACTIVE_DATA_WriteMask(uint8 mask) 
{
    #if(ACTIVE_DATA_INPUTS < 8u)
    	mask &= ((uint8)(1u << ACTIVE_DATA_INPUTS) - 1u);
	#endif /* End ACTIVE_DATA_INPUTS < 8u */
    ACTIVE_DATA_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: ACTIVE_DATA_ReadMask
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
uint8 ACTIVE_DATA_ReadMask(void) 
{
    return ACTIVE_DATA_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
