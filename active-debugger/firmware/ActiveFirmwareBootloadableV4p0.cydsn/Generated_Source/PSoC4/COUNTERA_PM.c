/*******************************************************************************
* File Name: COUNTERA_PM.c
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

#include "COUNTERA.h"

static COUNTERA_BACKUP_STRUCT COUNTERA_backup;


/*******************************************************************************
* Function Name: COUNTERA_SaveConfig
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
void COUNTERA_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: COUNTERA_Sleep
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
void COUNTERA_Sleep(void)
{
    if(0u != (COUNTERA_BLOCK_CONTROL_REG & COUNTERA_MASK))
    {
        COUNTERA_backup.enableState = 1u;
    }
    else
    {
        COUNTERA_backup.enableState = 0u;
    }

    COUNTERA_Stop();
    COUNTERA_SaveConfig();
}


/*******************************************************************************
* Function Name: COUNTERA_RestoreConfig
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
void COUNTERA_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: COUNTERA_Wakeup
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
void COUNTERA_Wakeup(void)
{
    COUNTERA_RestoreConfig();

    if(0u != COUNTERA_backup.enableState)
    {
        COUNTERA_Enable();
    }
}


/* [] END OF FILE */
