/*******************************************************************************
* File Name: COUNTER2A_PM.c
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

#include "COUNTER2A.h"

static COUNTER2A_BACKUP_STRUCT COUNTER2A_backup;


/*******************************************************************************
* Function Name: COUNTER2A_SaveConfig
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
void COUNTER2A_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: COUNTER2A_Sleep
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
void COUNTER2A_Sleep(void)
{
    if(0u != (COUNTER2A_BLOCK_CONTROL_REG & COUNTER2A_MASK))
    {
        COUNTER2A_backup.enableState = 1u;
    }
    else
    {
        COUNTER2A_backup.enableState = 0u;
    }

    COUNTER2A_Stop();
    COUNTER2A_SaveConfig();
}


/*******************************************************************************
* Function Name: COUNTER2A_RestoreConfig
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
void COUNTER2A_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: COUNTER2A_Wakeup
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
void COUNTER2A_Wakeup(void)
{
    COUNTER2A_RestoreConfig();

    if(0u != COUNTER2A_backup.enableState)
    {
        COUNTER2A_Enable();
    }
}


/* [] END OF FILE */
