/*******************************************************************************
* File Name: RESET_COUNT_A_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "RESET_COUNT_A.h"

/* Check for removal by optimization */
#if !defined(RESET_COUNT_A_Sync_ctrl_reg__REMOVED)

static RESET_COUNT_A_BACKUP_STRUCT  RESET_COUNT_A_backup = {0u};

    
/*******************************************************************************
* Function Name: RESET_COUNT_A_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void RESET_COUNT_A_SaveConfig(void) 
{
    RESET_COUNT_A_backup.controlState = RESET_COUNT_A_Control;
}


/*******************************************************************************
* Function Name: RESET_COUNT_A_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void RESET_COUNT_A_RestoreConfig(void) 
{
     RESET_COUNT_A_Control = RESET_COUNT_A_backup.controlState;
}


/*******************************************************************************
* Function Name: RESET_COUNT_A_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void RESET_COUNT_A_Sleep(void) 
{
    RESET_COUNT_A_SaveConfig();
}


/*******************************************************************************
* Function Name: RESET_COUNT_A_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void RESET_COUNT_A_Wakeup(void)  
{
    RESET_COUNT_A_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
