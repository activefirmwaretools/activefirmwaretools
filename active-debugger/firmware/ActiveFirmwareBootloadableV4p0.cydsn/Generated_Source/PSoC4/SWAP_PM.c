/*******************************************************************************
* File Name: SWAP_PM.c
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

#include "SWAP.h"

/* Check for removal by optimization */
#if !defined(SWAP_Sync_ctrl_reg__REMOVED)

static SWAP_BACKUP_STRUCT  SWAP_backup = {0u};

    
/*******************************************************************************
* Function Name: SWAP_SaveConfig
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
void SWAP_SaveConfig(void) 
{
    SWAP_backup.controlState = SWAP_Control;
}


/*******************************************************************************
* Function Name: SWAP_RestoreConfig
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
void SWAP_RestoreConfig(void) 
{
     SWAP_Control = SWAP_backup.controlState;
}


/*******************************************************************************
* Function Name: SWAP_Sleep
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
void SWAP_Sleep(void) 
{
    SWAP_SaveConfig();
}


/*******************************************************************************
* Function Name: SWAP_Wakeup
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
void SWAP_Wakeup(void)  
{
    SWAP_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
