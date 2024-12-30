/*******************************************************************************
* File Name: EDATA_B.c  
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

#include "EDATA_B.h"

#if !defined(EDATA_B_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: EDATA_B_Read
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
uint8 EDATA_B_Read(void) 
{ 
    return EDATA_B_Status;
}


/*******************************************************************************
* Function Name: EDATA_B_InterruptEnable
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
void EDATA_B_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    EDATA_B_Status_Aux_Ctrl |= EDATA_B_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: EDATA_B_InterruptDisable
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
void EDATA_B_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    EDATA_B_Status_Aux_Ctrl &= (uint8)(~EDATA_B_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: EDATA_B_WriteMask
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
void EDATA_B_WriteMask(uint8 mask) 
{
    #if(EDATA_B_INPUTS < 8u)
    	mask &= ((uint8)(1u << EDATA_B_INPUTS) - 1u);
	#endif /* End EDATA_B_INPUTS < 8u */
    EDATA_B_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: EDATA_B_ReadMask
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
uint8 EDATA_B_ReadMask(void) 
{
    return EDATA_B_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
