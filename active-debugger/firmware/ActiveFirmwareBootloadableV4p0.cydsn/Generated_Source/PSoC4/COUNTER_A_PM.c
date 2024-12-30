/*******************************************************************************
* File Name: COUNTER_A_PM.c  
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

#include "COUNTER_A.h"

static COUNTER_A_backupStruct COUNTER_A_backup;


/*******************************************************************************
* Function Name: COUNTER_A_SaveConfig
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
*  COUNTER_A_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void COUNTER_A_SaveConfig(void) 
{
    #if (!COUNTER_A_UsingFixedFunction)

        COUNTER_A_backup.CounterUdb = COUNTER_A_ReadCounter();

        #if(!COUNTER_A_ControlRegRemoved)
            COUNTER_A_backup.CounterControlRegister = COUNTER_A_ReadControlRegister();
        #endif /* (!COUNTER_A_ControlRegRemoved) */

    #endif /* (!COUNTER_A_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_A_RestoreConfig
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
*  COUNTER_A_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void COUNTER_A_RestoreConfig(void) 
{      
    #if (!COUNTER_A_UsingFixedFunction)

       COUNTER_A_WriteCounter(COUNTER_A_backup.CounterUdb);

        #if(!COUNTER_A_ControlRegRemoved)
            COUNTER_A_WriteControlRegister(COUNTER_A_backup.CounterControlRegister);
        #endif /* (!COUNTER_A_ControlRegRemoved) */

    #endif /* (!COUNTER_A_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_A_Sleep
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
*  COUNTER_A_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void COUNTER_A_Sleep(void) 
{
    #if(!COUNTER_A_ControlRegRemoved)
        /* Save Counter's enable state */
        if(COUNTER_A_CTRL_ENABLE == (COUNTER_A_CONTROL & COUNTER_A_CTRL_ENABLE))
        {
            /* Counter is enabled */
            COUNTER_A_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            COUNTER_A_backup.CounterEnableState = 0u;
        }
    #else
        COUNTER_A_backup.CounterEnableState = 1u;
        if(COUNTER_A_backup.CounterEnableState != 0u)
        {
            COUNTER_A_backup.CounterEnableState = 0u;
        }
    #endif /* (!COUNTER_A_ControlRegRemoved) */
    
    COUNTER_A_Stop();
    COUNTER_A_SaveConfig();
}


/*******************************************************************************
* Function Name: COUNTER_A_Wakeup
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
*  COUNTER_A_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void COUNTER_A_Wakeup(void) 
{
    COUNTER_A_RestoreConfig();
    #if(!COUNTER_A_ControlRegRemoved)
        if(COUNTER_A_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            COUNTER_A_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!COUNTER_A_ControlRegRemoved) */
    
}


/* [] END OF FILE */
