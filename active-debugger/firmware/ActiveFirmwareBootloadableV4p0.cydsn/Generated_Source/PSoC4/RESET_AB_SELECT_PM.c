/*******************************************************************************
* File Name: RESET_AB_SELECT_PM.c
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

#include "RESET_AB_SELECT.h"

/* Check for removal by optimization */
#if !defined(RESET_AB_SELECT_Sync_ctrl_reg__REMOVED)

static RESET_AB_SELECT_BACKUP_STRUCT  RESET_AB_SELECT_backup = {0u};

    
/*******************************************************************************
* Function Name: RESET_AB_SELECT_SaveConfig
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
void RESET_AB_SELECT_SaveConfig(void) 
{
    RESET_AB_SELECT_backup.controlState = RESET_AB_SELECT_Control;
}


/*******************************************************************************
* Function Name: RESET_AB_SELECT_RestoreConfig
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
void RESET_AB_SELECT_RestoreConfig(void) 
{
     RESET_AB_SELECT_Control = RESET_AB_SELECT_backup.controlState;
}


/*******************************************************************************
* Function Name: RESET_AB_SELECT_Sleep
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
void RESET_AB_SELECT_Sleep(void) 
{
    RESET_AB_SELECT_SaveConfig();
}


/*******************************************************************************
* Function Name: RESET_AB_SELECT_Wakeup
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
void RESET_AB_SELECT_Wakeup(void)  
{
    RESET_AB_SELECT_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
