/*******************************************************************************
* File Name: COUNTER1A_PM.c
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

#include "COUNTER1A.h"

static COUNTER1A_BACKUP_STRUCT COUNTER1A_backup;


/*******************************************************************************
* Function Name: COUNTER1A_SaveConfig
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
void COUNTER1A_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: COUNTER1A_Sleep
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
void COUNTER1A_Sleep(void)
{
    if(0u != (COUNTER1A_BLOCK_CONTROL_REG & COUNTER1A_MASK))
    {
        COUNTER1A_backup.enableState = 1u;
    }
    else
    {
        COUNTER1A_backup.enableState = 0u;
    }

    COUNTER1A_Stop();
    COUNTER1A_SaveConfig();
}


/*******************************************************************************
* Function Name: COUNTER1A_RestoreConfig
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
void COUNTER1A_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: COUNTER1A_Wakeup
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
void COUNTER1A_Wakeup(void)
{
    COUNTER1A_RestoreConfig();

    if(0u != COUNTER1A_backup.enableState)
    {
        COUNTER1A_Enable();
    }
}


/* [] END OF FILE */
