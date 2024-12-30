/*******************************************************************************
* File Name: COUNTUPA_PM.c
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

#include "COUNTUPA.h"

static COUNTUPA_BACKUP_STRUCT COUNTUPA_backup;


/*******************************************************************************
* Function Name: COUNTUPA_SaveConfig
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
void COUNTUPA_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: COUNTUPA_Sleep
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
void COUNTUPA_Sleep(void)
{
    if(0u != (COUNTUPA_BLOCK_CONTROL_REG & COUNTUPA_MASK))
    {
        COUNTUPA_backup.enableState = 1u;
    }
    else
    {
        COUNTUPA_backup.enableState = 0u;
    }

    COUNTUPA_Stop();
    COUNTUPA_SaveConfig();
}


/*******************************************************************************
* Function Name: COUNTUPA_RestoreConfig
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
void COUNTUPA_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: COUNTUPA_Wakeup
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
void COUNTUPA_Wakeup(void)
{
    COUNTUPA_RestoreConfig();

    if(0u != COUNTUPA_backup.enableState)
    {
        COUNTUPA_Enable();
    }
}


/* [] END OF FILE */
