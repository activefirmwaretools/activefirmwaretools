/*******************************************************************************
* File Name: COUNTERB_PM.c
* Version 2.10
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "COUNTERB.h"

static COUNTERB_BACKUP_STRUCT COUNTERB_backup;


/*******************************************************************************
* Function Name: COUNTERB_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERB_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: COUNTERB_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERB_Sleep(void)
{
    if(0u != (COUNTERB_BLOCK_CONTROL_REG & COUNTERB_MASK))
    {
        COUNTERB_backup.enableState = 1u;
    }
    else
    {
        COUNTERB_backup.enableState = 0u;
    }

    COUNTERB_Stop();
    COUNTERB_SaveConfig();
}


/*******************************************************************************
* Function Name: COUNTERB_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERB_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: COUNTERB_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERB_Wakeup(void)
{
    COUNTERB_RestoreConfig();

    if(0u != COUNTERB_backup.enableState)
    {
        COUNTERB_Enable();
    }
}


/* [] END OF FILE */
