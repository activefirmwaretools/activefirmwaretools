/*******************************************************************************
* File Name: COUNTER1_PM.c
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

#include "COUNTER1.h"

static COUNTER1_BACKUP_STRUCT COUNTER1_backup;


/*******************************************************************************
* Function Name: COUNTER1_SaveConfig
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
void COUNTER1_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: COUNTER1_Sleep
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
void COUNTER1_Sleep(void)
{
    if(0u != (COUNTER1_BLOCK_CONTROL_REG & COUNTER1_MASK))
    {
        COUNTER1_backup.enableState = 1u;
    }
    else
    {
        COUNTER1_backup.enableState = 0u;
    }

    COUNTER1_Stop();
    COUNTER1_SaveConfig();
}


/*******************************************************************************
* Function Name: COUNTER1_RestoreConfig
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
void COUNTER1_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: COUNTER1_Wakeup
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
void COUNTER1_Wakeup(void)
{
    COUNTER1_RestoreConfig();

    if(0u != COUNTER1_backup.enableState)
    {
        COUNTER1_Enable();
    }
}


/* [] END OF FILE */
