/*******************************************************************************
* File Name: COUNTER_B_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "COUNTER_B.h"

static COUNTER_B_backupStruct COUNTER_B_backup;


/*******************************************************************************
* Function Name: COUNTER_B_SaveConfig
********************************************************************************
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
*  COUNTER_B_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void COUNTER_B_SaveConfig(void) 
{
    #if (!COUNTER_B_UsingFixedFunction)

        COUNTER_B_backup.CounterUdb = COUNTER_B_ReadCounter();

        #if(!COUNTER_B_ControlRegRemoved)
            COUNTER_B_backup.CounterControlRegister = COUNTER_B_ReadControlRegister();
        #endif /* (!COUNTER_B_ControlRegRemoved) */

    #endif /* (!COUNTER_B_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_B_RestoreConfig
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
*  COUNTER_B_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void COUNTER_B_RestoreConfig(void) 
{      
    #if (!COUNTER_B_UsingFixedFunction)

       COUNTER_B_WriteCounter(COUNTER_B_backup.CounterUdb);

        #if(!COUNTER_B_ControlRegRemoved)
            COUNTER_B_WriteControlRegister(COUNTER_B_backup.CounterControlRegister);
        #endif /* (!COUNTER_B_ControlRegRemoved) */

    #endif /* (!COUNTER_B_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_B_Sleep
********************************************************************************
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
*  COUNTER_B_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void COUNTER_B_Sleep(void) 
{
    #if(!COUNTER_B_ControlRegRemoved)
        /* Save Counter's enable state */
        if(COUNTER_B_CTRL_ENABLE == (COUNTER_B_CONTROL & COUNTER_B_CTRL_ENABLE))
        {
            /* Counter is enabled */
            COUNTER_B_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            COUNTER_B_backup.CounterEnableState = 0u;
        }
    #else
        COUNTER_B_backup.CounterEnableState = 1u;
        if(COUNTER_B_backup.CounterEnableState != 0u)
        {
            COUNTER_B_backup.CounterEnableState = 0u;
        }
    #endif /* (!COUNTER_B_ControlRegRemoved) */
    
    COUNTER_B_Stop();
    COUNTER_B_SaveConfig();
}


/*******************************************************************************
* Function Name: COUNTER_B_Wakeup
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
*  COUNTER_B_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void COUNTER_B_Wakeup(void) 
{
    COUNTER_B_RestoreConfig();
    #if(!COUNTER_B_ControlRegRemoved)
        if(COUNTER_B_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            COUNTER_B_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!COUNTER_B_ControlRegRemoved) */
    
}


/* [] END OF FILE */
