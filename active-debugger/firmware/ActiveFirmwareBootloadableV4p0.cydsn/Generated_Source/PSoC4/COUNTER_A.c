/*******************************************************************************
* File Name: COUNTER_A.c  
* Version 3.0
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
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

uint8 COUNTER_A_initVar = 0u;


/*******************************************************************************
* Function Name: COUNTER_A_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void COUNTER_A_Init(void) 
{
        #if (!COUNTER_A_UsingFixedFunction && !COUNTER_A_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!COUNTER_A_UsingFixedFunction && !COUNTER_A_ControlRegRemoved) */
        
        #if(!COUNTER_A_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 COUNTER_A_interruptState;
        #endif /* (!COUNTER_A_UsingFixedFunction) */
        
        #if (COUNTER_A_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            COUNTER_A_CONTROL &= COUNTER_A_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                COUNTER_A_CONTROL2 &= ((uint8)(~COUNTER_A_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                COUNTER_A_CONTROL3 &= ((uint8)(~COUNTER_A_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (COUNTER_A_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                COUNTER_A_CONTROL |= COUNTER_A_ONESHOT;
            #endif /* (COUNTER_A_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            COUNTER_A_CONTROL2 |= COUNTER_A_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            COUNTER_A_RT1 &= ((uint8)(~COUNTER_A_RT1_MASK));
            COUNTER_A_RT1 |= COUNTER_A_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            COUNTER_A_RT1 &= ((uint8)(~COUNTER_A_SYNCDSI_MASK));
            COUNTER_A_RT1 |= COUNTER_A_SYNCDSI_EN;

        #else
            #if(!COUNTER_A_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = COUNTER_A_CONTROL & ((uint8)(~COUNTER_A_CTRL_CMPMODE_MASK));
            COUNTER_A_CONTROL = ctrl | COUNTER_A_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = COUNTER_A_CONTROL & ((uint8)(~COUNTER_A_CTRL_CAPMODE_MASK));
            
            #if( 0 != COUNTER_A_CAPTURE_MODE_CONF)
                COUNTER_A_CONTROL = ctrl | COUNTER_A_DEFAULT_CAPTURE_MODE;
            #else
                COUNTER_A_CONTROL = ctrl;
            #endif /* 0 != COUNTER_A_CAPTURE_MODE */ 
            
            #endif /* (!COUNTER_A_ControlRegRemoved) */
        #endif /* (COUNTER_A_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!COUNTER_A_UsingFixedFunction)
            COUNTER_A_ClearFIFO();
        #endif /* (!COUNTER_A_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        COUNTER_A_WritePeriod(COUNTER_A_INIT_PERIOD_VALUE);
        #if (!(COUNTER_A_UsingFixedFunction && (CY_PSOC5A)))
            COUNTER_A_WriteCounter(COUNTER_A_INIT_COUNTER_VALUE);
        #endif /* (!(COUNTER_A_UsingFixedFunction && (CY_PSOC5A))) */
        COUNTER_A_SetInterruptMode(COUNTER_A_INIT_INTERRUPTS_MASK);
        
        #if (!COUNTER_A_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)COUNTER_A_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            COUNTER_A_WriteCompare(COUNTER_A_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            COUNTER_A_interruptState = CyEnterCriticalSection();
            
            COUNTER_A_STATUS_AUX_CTRL |= COUNTER_A_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(COUNTER_A_interruptState);
            
        #endif /* (!COUNTER_A_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_A_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void COUNTER_A_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (COUNTER_A_UsingFixedFunction)
        COUNTER_A_GLOBAL_ENABLE |= COUNTER_A_BLOCK_EN_MASK;
        COUNTER_A_GLOBAL_STBY_ENABLE |= COUNTER_A_BLOCK_STBY_EN_MASK;
    #endif /* (COUNTER_A_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!COUNTER_A_ControlRegRemoved || COUNTER_A_UsingFixedFunction)
        COUNTER_A_CONTROL |= COUNTER_A_CTRL_ENABLE;                
    #endif /* (!COUNTER_A_ControlRegRemoved || COUNTER_A_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: COUNTER_A_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  COUNTER_A_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void COUNTER_A_Start(void) 
{
    if(COUNTER_A_initVar == 0u)
    {
        COUNTER_A_Init();
        
        COUNTER_A_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    COUNTER_A_Enable();        
}


/*******************************************************************************
* Function Name: COUNTER_A_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void COUNTER_A_Stop(void) 
{
    /* Disable Counter */
    #if(!COUNTER_A_ControlRegRemoved || COUNTER_A_UsingFixedFunction)
        COUNTER_A_CONTROL &= ((uint8)(~COUNTER_A_CTRL_ENABLE));        
    #endif /* (!COUNTER_A_ControlRegRemoved || COUNTER_A_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (COUNTER_A_UsingFixedFunction)
        COUNTER_A_GLOBAL_ENABLE &= ((uint8)(~COUNTER_A_BLOCK_EN_MASK));
        COUNTER_A_GLOBAL_STBY_ENABLE &= ((uint8)(~COUNTER_A_BLOCK_STBY_EN_MASK));
    #endif /* (COUNTER_A_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_A_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void COUNTER_A_SetInterruptMode(uint8 interruptsMask) 
{
    COUNTER_A_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: COUNTER_A_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   COUNTER_A_ReadStatusRegister(void) 
{
    return COUNTER_A_STATUS;
}


#if(!COUNTER_A_ControlRegRemoved)
/*******************************************************************************
* Function Name: COUNTER_A_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   COUNTER_A_ReadControlRegister(void) 
{
    return COUNTER_A_CONTROL;
}


/*******************************************************************************
* Function Name: COUNTER_A_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    COUNTER_A_WriteControlRegister(uint8 control) 
{
    COUNTER_A_CONTROL = control;
}

#endif  /* (!COUNTER_A_ControlRegRemoved) */


#if (!(COUNTER_A_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: COUNTER_A_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void COUNTER_A_WriteCounter(uint8 counter) \
                                   
{
    #if(COUNTER_A_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (COUNTER_A_GLOBAL_ENABLE & COUNTER_A_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        COUNTER_A_GLOBAL_ENABLE |= COUNTER_A_BLOCK_EN_MASK;
        CY_SET_REG16(COUNTER_A_COUNTER_LSB_PTR, (uint16)counter);
        COUNTER_A_GLOBAL_ENABLE &= ((uint8)(~COUNTER_A_BLOCK_EN_MASK));
    #else
        CY_SET_REG8(COUNTER_A_COUNTER_LSB_PTR, counter);
    #endif /* (COUNTER_A_UsingFixedFunction) */
}
#endif /* (!(COUNTER_A_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: COUNTER_A_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) The present value of the counter.
*
*******************************************************************************/
uint8 COUNTER_A_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(COUNTER_A_UsingFixedFunction)
		(void)CY_GET_REG16(COUNTER_A_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(COUNTER_A_COUNTER_LSB_PTR_8BIT);
	#endif/* (COUNTER_A_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(COUNTER_A_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(COUNTER_A_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(COUNTER_A_STATICCOUNT_LSB_PTR));
    #endif /* (COUNTER_A_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_A_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) Present Capture value.
*
*******************************************************************************/
uint8 COUNTER_A_ReadCapture(void) 
{
    #if(COUNTER_A_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(COUNTER_A_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(COUNTER_A_STATICCOUNT_LSB_PTR));
    #endif /* (COUNTER_A_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_A_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint8) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void COUNTER_A_WritePeriod(uint8 period) 
{
    #if(COUNTER_A_UsingFixedFunction)
        CY_SET_REG16(COUNTER_A_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(COUNTER_A_PERIOD_LSB_PTR, period);
    #endif /* (COUNTER_A_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_A_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) Present period value.
*
*******************************************************************************/
uint8 COUNTER_A_ReadPeriod(void) 
{
    #if(COUNTER_A_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(COUNTER_A_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(COUNTER_A_PERIOD_LSB_PTR));
    #endif /* (COUNTER_A_UsingFixedFunction) */
}


#if (!COUNTER_A_UsingFixedFunction)
/*******************************************************************************
* Function Name: COUNTER_A_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void COUNTER_A_WriteCompare(uint8 compare) \
                                   
{
    #if(COUNTER_A_UsingFixedFunction)
        CY_SET_REG16(COUNTER_A_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(COUNTER_A_COMPARE_LSB_PTR, compare);
    #endif /* (COUNTER_A_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_A_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint8) Present compare value.
*
*******************************************************************************/
uint8 COUNTER_A_ReadCompare(void) 
{
    return (CY_GET_REG8(COUNTER_A_COMPARE_LSB_PTR));
}


#if (COUNTER_A_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: COUNTER_A_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void COUNTER_A_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    COUNTER_A_CONTROL &= ((uint8)(~COUNTER_A_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    COUNTER_A_CONTROL |= compareMode;
}
#endif  /* (COUNTER_A_COMPARE_MODE_SOFTWARE) */


#if (COUNTER_A_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: COUNTER_A_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void COUNTER_A_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    COUNTER_A_CONTROL &= ((uint8)(~COUNTER_A_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    COUNTER_A_CONTROL |= ((uint8)((uint8)captureMode << COUNTER_A_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (COUNTER_A_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: COUNTER_A_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void COUNTER_A_ClearFIFO(void) 
{

    while(0u != (COUNTER_A_ReadStatusRegister() & COUNTER_A_STATUS_FIFONEMP))
    {
        (void)COUNTER_A_ReadCapture();
    }

}
#endif  /* (!COUNTER_A_UsingFixedFunction) */


/* [] END OF FILE */

