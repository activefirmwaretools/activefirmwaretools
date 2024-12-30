/*******************************************************************************
* File Name: COUNTER_B.c  
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

#include "COUNTER_B.h"

uint8 COUNTER_B_initVar = 0u;


/*******************************************************************************
* Function Name: COUNTER_B_Init
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
void COUNTER_B_Init(void) 
{
        #if (!COUNTER_B_UsingFixedFunction && !COUNTER_B_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!COUNTER_B_UsingFixedFunction && !COUNTER_B_ControlRegRemoved) */
        
        #if(!COUNTER_B_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 COUNTER_B_interruptState;
        #endif /* (!COUNTER_B_UsingFixedFunction) */
        
        #if (COUNTER_B_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            COUNTER_B_CONTROL &= COUNTER_B_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                COUNTER_B_CONTROL2 &= ((uint8)(~COUNTER_B_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                COUNTER_B_CONTROL3 &= ((uint8)(~COUNTER_B_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (COUNTER_B_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                COUNTER_B_CONTROL |= COUNTER_B_ONESHOT;
            #endif /* (COUNTER_B_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            COUNTER_B_CONTROL2 |= COUNTER_B_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            COUNTER_B_RT1 &= ((uint8)(~COUNTER_B_RT1_MASK));
            COUNTER_B_RT1 |= COUNTER_B_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            COUNTER_B_RT1 &= ((uint8)(~COUNTER_B_SYNCDSI_MASK));
            COUNTER_B_RT1 |= COUNTER_B_SYNCDSI_EN;

        #else
            #if(!COUNTER_B_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = COUNTER_B_CONTROL & ((uint8)(~COUNTER_B_CTRL_CMPMODE_MASK));
            COUNTER_B_CONTROL = ctrl | COUNTER_B_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = COUNTER_B_CONTROL & ((uint8)(~COUNTER_B_CTRL_CAPMODE_MASK));
            
            #if( 0 != COUNTER_B_CAPTURE_MODE_CONF)
                COUNTER_B_CONTROL = ctrl | COUNTER_B_DEFAULT_CAPTURE_MODE;
            #else
                COUNTER_B_CONTROL = ctrl;
            #endif /* 0 != COUNTER_B_CAPTURE_MODE */ 
            
            #endif /* (!COUNTER_B_ControlRegRemoved) */
        #endif /* (COUNTER_B_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!COUNTER_B_UsingFixedFunction)
            COUNTER_B_ClearFIFO();
        #endif /* (!COUNTER_B_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        COUNTER_B_WritePeriod(COUNTER_B_INIT_PERIOD_VALUE);
        #if (!(COUNTER_B_UsingFixedFunction && (CY_PSOC5A)))
            COUNTER_B_WriteCounter(COUNTER_B_INIT_COUNTER_VALUE);
        #endif /* (!(COUNTER_B_UsingFixedFunction && (CY_PSOC5A))) */
        COUNTER_B_SetInterruptMode(COUNTER_B_INIT_INTERRUPTS_MASK);
        
        #if (!COUNTER_B_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)COUNTER_B_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            COUNTER_B_WriteCompare(COUNTER_B_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            COUNTER_B_interruptState = CyEnterCriticalSection();
            
            COUNTER_B_STATUS_AUX_CTRL |= COUNTER_B_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(COUNTER_B_interruptState);
            
        #endif /* (!COUNTER_B_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_B_Enable
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
void COUNTER_B_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (COUNTER_B_UsingFixedFunction)
        COUNTER_B_GLOBAL_ENABLE |= COUNTER_B_BLOCK_EN_MASK;
        COUNTER_B_GLOBAL_STBY_ENABLE |= COUNTER_B_BLOCK_STBY_EN_MASK;
    #endif /* (COUNTER_B_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!COUNTER_B_ControlRegRemoved || COUNTER_B_UsingFixedFunction)
        COUNTER_B_CONTROL |= COUNTER_B_CTRL_ENABLE;                
    #endif /* (!COUNTER_B_ControlRegRemoved || COUNTER_B_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: COUNTER_B_Start
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
*  COUNTER_B_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void COUNTER_B_Start(void) 
{
    if(COUNTER_B_initVar == 0u)
    {
        COUNTER_B_Init();
        
        COUNTER_B_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    COUNTER_B_Enable();        
}


/*******************************************************************************
* Function Name: COUNTER_B_Stop
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
void COUNTER_B_Stop(void) 
{
    /* Disable Counter */
    #if(!COUNTER_B_ControlRegRemoved || COUNTER_B_UsingFixedFunction)
        COUNTER_B_CONTROL &= ((uint8)(~COUNTER_B_CTRL_ENABLE));        
    #endif /* (!COUNTER_B_ControlRegRemoved || COUNTER_B_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (COUNTER_B_UsingFixedFunction)
        COUNTER_B_GLOBAL_ENABLE &= ((uint8)(~COUNTER_B_BLOCK_EN_MASK));
        COUNTER_B_GLOBAL_STBY_ENABLE &= ((uint8)(~COUNTER_B_BLOCK_STBY_EN_MASK));
    #endif /* (COUNTER_B_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_B_SetInterruptMode
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
void COUNTER_B_SetInterruptMode(uint8 interruptsMask) 
{
    COUNTER_B_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: COUNTER_B_ReadStatusRegister
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
uint8   COUNTER_B_ReadStatusRegister(void) 
{
    return COUNTER_B_STATUS;
}


#if(!COUNTER_B_ControlRegRemoved)
/*******************************************************************************
* Function Name: COUNTER_B_ReadControlRegister
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
uint8   COUNTER_B_ReadControlRegister(void) 
{
    return COUNTER_B_CONTROL;
}


/*******************************************************************************
* Function Name: COUNTER_B_WriteControlRegister
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
void    COUNTER_B_WriteControlRegister(uint8 control) 
{
    COUNTER_B_CONTROL = control;
}

#endif  /* (!COUNTER_B_ControlRegRemoved) */


#if (!(COUNTER_B_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: COUNTER_B_WriteCounter
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
void COUNTER_B_WriteCounter(uint8 counter) \
                                   
{
    #if(COUNTER_B_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (COUNTER_B_GLOBAL_ENABLE & COUNTER_B_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        COUNTER_B_GLOBAL_ENABLE |= COUNTER_B_BLOCK_EN_MASK;
        CY_SET_REG16(COUNTER_B_COUNTER_LSB_PTR, (uint16)counter);
        COUNTER_B_GLOBAL_ENABLE &= ((uint8)(~COUNTER_B_BLOCK_EN_MASK));
    #else
        CY_SET_REG8(COUNTER_B_COUNTER_LSB_PTR, counter);
    #endif /* (COUNTER_B_UsingFixedFunction) */
}
#endif /* (!(COUNTER_B_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: COUNTER_B_ReadCounter
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
uint8 COUNTER_B_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(COUNTER_B_UsingFixedFunction)
		(void)CY_GET_REG16(COUNTER_B_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(COUNTER_B_COUNTER_LSB_PTR_8BIT);
	#endif/* (COUNTER_B_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(COUNTER_B_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(COUNTER_B_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(COUNTER_B_STATICCOUNT_LSB_PTR));
    #endif /* (COUNTER_B_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_B_ReadCapture
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
uint8 COUNTER_B_ReadCapture(void) 
{
    #if(COUNTER_B_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(COUNTER_B_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(COUNTER_B_STATICCOUNT_LSB_PTR));
    #endif /* (COUNTER_B_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_B_WritePeriod
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
void COUNTER_B_WritePeriod(uint8 period) 
{
    #if(COUNTER_B_UsingFixedFunction)
        CY_SET_REG16(COUNTER_B_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(COUNTER_B_PERIOD_LSB_PTR, period);
    #endif /* (COUNTER_B_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_B_ReadPeriod
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
uint8 COUNTER_B_ReadPeriod(void) 
{
    #if(COUNTER_B_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(COUNTER_B_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(COUNTER_B_PERIOD_LSB_PTR));
    #endif /* (COUNTER_B_UsingFixedFunction) */
}


#if (!COUNTER_B_UsingFixedFunction)
/*******************************************************************************
* Function Name: COUNTER_B_WriteCompare
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
void COUNTER_B_WriteCompare(uint8 compare) \
                                   
{
    #if(COUNTER_B_UsingFixedFunction)
        CY_SET_REG16(COUNTER_B_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(COUNTER_B_COMPARE_LSB_PTR, compare);
    #endif /* (COUNTER_B_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: COUNTER_B_ReadCompare
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
uint8 COUNTER_B_ReadCompare(void) 
{
    return (CY_GET_REG8(COUNTER_B_COMPARE_LSB_PTR));
}


#if (COUNTER_B_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: COUNTER_B_SetCompareMode
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
void COUNTER_B_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    COUNTER_B_CONTROL &= ((uint8)(~COUNTER_B_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    COUNTER_B_CONTROL |= compareMode;
}
#endif  /* (COUNTER_B_COMPARE_MODE_SOFTWARE) */


#if (COUNTER_B_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: COUNTER_B_SetCaptureMode
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
void COUNTER_B_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    COUNTER_B_CONTROL &= ((uint8)(~COUNTER_B_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    COUNTER_B_CONTROL |= ((uint8)((uint8)captureMode << COUNTER_B_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (COUNTER_B_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: COUNTER_B_ClearFIFO
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
void COUNTER_B_ClearFIFO(void) 
{

    while(0u != (COUNTER_B_ReadStatusRegister() & COUNTER_B_STATUS_FIFONEMP))
    {
        (void)COUNTER_B_ReadCapture();
    }

}
#endif  /* (!COUNTER_B_UsingFixedFunction) */


/* [] END OF FILE */

