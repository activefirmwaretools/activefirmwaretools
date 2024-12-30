/*******************************************************************************
* File Name: FREEZE_PM.c
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

#include "FREEZE.h"

/* Check for removal by optimization */
#if !defined(FREEZE_Sync_ctrl_reg__REMOVED)

static FREEZE_BACKUP_STRUCT  FREEZE_backup = {0u};

    
/*******************************************************************************
* Function Name: FREEZE_SaveConfig
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
void FREEZE_SaveConfig(void) 
{
    FREEZE_backup.controlState = FREEZE_Control;
}


/*******************************************************************************
* Function Name: FREEZE_RestoreConfig
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
void FREEZE_RestoreConfig(void) 
{
     FREEZE_Control = FREEZE_backup.controlState;
}


/*******************************************************************************
* Function Name: FREEZE_Sleep
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
void FREEZE_Sleep(void) 
{
    FREEZE_SaveConfig();
}


/*******************************************************************************
* Function Name: FREEZE_Wakeup
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
void FREEZE_Wakeup(void)  
{
    FREEZE_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
