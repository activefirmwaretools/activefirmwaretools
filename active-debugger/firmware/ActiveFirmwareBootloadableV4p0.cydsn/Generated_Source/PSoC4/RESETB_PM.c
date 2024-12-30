/*******************************************************************************
* File Name: RESETB_PM.c
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

#include "RESETB.h"

/* Check for removal by optimization */
#if !defined(RESETB_Sync_ctrl_reg__REMOVED)

static RESETB_BACKUP_STRUCT  RESETB_backup = {0u};

    
/*******************************************************************************
* Function Name: RESETB_SaveConfig
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
void RESETB_SaveConfig(void) 
{
    RESETB_backup.controlState = RESETB_Control;
}


/*******************************************************************************
* Function Name: RESETB_RestoreConfig
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
void RESETB_RestoreConfig(void) 
{
     RESETB_Control = RESETB_backup.controlState;
}


/*******************************************************************************
* Function Name: RESETB_Sleep
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
void RESETB_Sleep(void) 
{
    RESETB_SaveConfig();
}


/*******************************************************************************
* Function Name: RESETB_Wakeup
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
void RESETB_Wakeup(void)  
{
    RESETB_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
