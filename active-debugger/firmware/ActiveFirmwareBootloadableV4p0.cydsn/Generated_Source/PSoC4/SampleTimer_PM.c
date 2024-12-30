/*******************************************************************************
* File Name: SampleTimer_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "SampleTimer.h"

static SampleTimer_backupStruct SampleTimer_backup;


/*******************************************************************************
* Function Name: SampleTimer_SaveConfig
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
*  SampleTimer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void SampleTimer_SaveConfig(void) 
{
    #if (!SampleTimer_UsingFixedFunction)
        SampleTimer_backup.TimerUdb = SampleTimer_ReadCounter();
        SampleTimer_backup.InterruptMaskValue = SampleTimer_STATUS_MASK;
        #if (SampleTimer_UsingHWCaptureCounter)
            SampleTimer_backup.TimerCaptureCounter = SampleTimer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!SampleTimer_UDB_CONTROL_REG_REMOVED)
            SampleTimer_backup.TimerControlRegister = SampleTimer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: SampleTimer_RestoreConfig
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
*  SampleTimer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void SampleTimer_RestoreConfig(void) 
{   
    #if (!SampleTimer_UsingFixedFunction)

        SampleTimer_WriteCounter(SampleTimer_backup.TimerUdb);
        SampleTimer_STATUS_MASK =SampleTimer_backup.InterruptMaskValue;
        #if (SampleTimer_UsingHWCaptureCounter)
            SampleTimer_SetCaptureCount(SampleTimer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!SampleTimer_UDB_CONTROL_REG_REMOVED)
            SampleTimer_WriteControlRegister(SampleTimer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: SampleTimer_Sleep
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
*  SampleTimer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void SampleTimer_Sleep(void) 
{
    #if(!SampleTimer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(SampleTimer_CTRL_ENABLE == (SampleTimer_CONTROL & SampleTimer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            SampleTimer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            SampleTimer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    SampleTimer_Stop();
    SampleTimer_SaveConfig();
}


/*******************************************************************************
* Function Name: SampleTimer_Wakeup
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
*  SampleTimer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void SampleTimer_Wakeup(void) 
{
    SampleTimer_RestoreConfig();
    #if(!SampleTimer_UDB_CONTROL_REG_REMOVED)
        if(SampleTimer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                SampleTimer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
