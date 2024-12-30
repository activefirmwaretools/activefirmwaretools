/*******************************************************************************
* File Name: TimestampTimer_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "TimestampTimer.h"

static TimestampTimer_backupStruct TimestampTimer_backup;


/*******************************************************************************
* Function Name: TimestampTimer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  TimestampTimer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void TimestampTimer_SaveConfig(void) 
{
    #if (!TimestampTimer_UsingFixedFunction)
        TimestampTimer_backup.TimerUdb = TimestampTimer_ReadCounter();
        TimestampTimer_backup.InterruptMaskValue = TimestampTimer_STATUS_MASK;
        #if (TimestampTimer_UsingHWCaptureCounter)
            TimestampTimer_backup.TimerCaptureCounter = TimestampTimer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!TimestampTimer_UDB_CONTROL_REG_REMOVED)
            TimestampTimer_backup.TimerControlRegister = TimestampTimer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: TimestampTimer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  TimestampTimer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void TimestampTimer_RestoreConfig(void) 
{   
    #if (!TimestampTimer_UsingFixedFunction)

        TimestampTimer_WriteCounter(TimestampTimer_backup.TimerUdb);
        TimestampTimer_STATUS_MASK =TimestampTimer_backup.InterruptMaskValue;
        #if (TimestampTimer_UsingHWCaptureCounter)
            TimestampTimer_SetCaptureCount(TimestampTimer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!TimestampTimer_UDB_CONTROL_REG_REMOVED)
            TimestampTimer_WriteControlRegister(TimestampTimer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: TimestampTimer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  TimestampTimer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void TimestampTimer_Sleep(void) 
{
    #if(!TimestampTimer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(TimestampTimer_CTRL_ENABLE == (TimestampTimer_CONTROL & TimestampTimer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            TimestampTimer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            TimestampTimer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    TimestampTimer_Stop();
    TimestampTimer_SaveConfig();
}


/*******************************************************************************
* Function Name: TimestampTimer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  TimestampTimer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void TimestampTimer_Wakeup(void) 
{
    TimestampTimer_RestoreConfig();
    #if(!TimestampTimer_UDB_CONTROL_REG_REMOVED)
        if(TimestampTimer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                TimestampTimer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
