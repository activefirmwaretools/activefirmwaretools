/*******************************************************************************
* File Name: COUNTERA.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the COUNTERA
*  component
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

uint8 COUNTERA_initVar = 0u;


/*******************************************************************************
* Function Name: COUNTERA_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default COUNTERA configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (COUNTERA__QUAD == COUNTERA_CONFIG)
        COUNTERA_CONTROL_REG = COUNTERA_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        COUNTERA_TRIG_CONTROL1_REG  = COUNTERA_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        COUNTERA_SetInterruptMode(COUNTERA_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        COUNTERA_SetCounterMode(COUNTERA_COUNT_DOWN);
        COUNTERA_WritePeriod(COUNTERA_QUAD_PERIOD_INIT_VALUE);
        COUNTERA_WriteCounter(COUNTERA_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (COUNTERA__QUAD == COUNTERA_CONFIG) */

    #if (COUNTERA__TIMER == COUNTERA_CONFIG)
        COUNTERA_CONTROL_REG = COUNTERA_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        COUNTERA_TRIG_CONTROL1_REG  = COUNTERA_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        COUNTERA_SetInterruptMode(COUNTERA_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        COUNTERA_WritePeriod(COUNTERA_TC_PERIOD_VALUE );

        #if (COUNTERA__COMPARE == COUNTERA_TC_COMP_CAP_MODE)
            COUNTERA_WriteCompare(COUNTERA_TC_COMPARE_VALUE);

            #if (1u == COUNTERA_TC_COMPARE_SWAP)
                COUNTERA_SetCompareSwap(1u);
                COUNTERA_WriteCompareBuf(COUNTERA_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == COUNTERA_TC_COMPARE_SWAP) */
        #endif  /* (COUNTERA__COMPARE == COUNTERA_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (COUNTERA_CY_TCPWM_V2 && COUNTERA_TIMER_UPDOWN_CNT_USED && !COUNTERA_CY_TCPWM_4000)
            COUNTERA_WriteCounter(1u);
        #elif(COUNTERA__COUNT_DOWN == COUNTERA_TC_COUNTER_MODE)
            COUNTERA_WriteCounter(COUNTERA_TC_PERIOD_VALUE);
        #else
            COUNTERA_WriteCounter(0u);
        #endif /* (COUNTERA_CY_TCPWM_V2 && COUNTERA_TIMER_UPDOWN_CNT_USED && !COUNTERA_CY_TCPWM_4000) */
    #endif  /* (COUNTERA__TIMER == COUNTERA_CONFIG) */

    #if (COUNTERA__PWM_SEL == COUNTERA_CONFIG)
        COUNTERA_CONTROL_REG = COUNTERA_CTRL_PWM_BASE_CONFIG;

        #if (COUNTERA__PWM_PR == COUNTERA_PWM_MODE)
            COUNTERA_CONTROL_REG |= COUNTERA_CTRL_PWM_RUN_MODE;
            COUNTERA_WriteCounter(COUNTERA_PWM_PR_INIT_VALUE);
        #else
            COUNTERA_CONTROL_REG |= COUNTERA_CTRL_PWM_ALIGN | COUNTERA_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (COUNTERA_CY_TCPWM_V2 && COUNTERA_PWM_UPDOWN_CNT_USED && !COUNTERA_CY_TCPWM_4000)
                COUNTERA_WriteCounter(1u);
            #elif (COUNTERA__RIGHT == COUNTERA_PWM_ALIGN)
                COUNTERA_WriteCounter(COUNTERA_PWM_PERIOD_VALUE);
            #else 
                COUNTERA_WriteCounter(0u);
            #endif  /* (COUNTERA_CY_TCPWM_V2 && COUNTERA_PWM_UPDOWN_CNT_USED && !COUNTERA_CY_TCPWM_4000) */
        #endif  /* (COUNTERA__PWM_PR == COUNTERA_PWM_MODE) */

        #if (COUNTERA__PWM_DT == COUNTERA_PWM_MODE)
            COUNTERA_CONTROL_REG |= COUNTERA_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (COUNTERA__PWM_DT == COUNTERA_PWM_MODE) */

        #if (COUNTERA__PWM == COUNTERA_PWM_MODE)
            COUNTERA_CONTROL_REG |= COUNTERA_CTRL_PWM_PRESCALER;
        #endif  /* (COUNTERA__PWM == COUNTERA_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        COUNTERA_TRIG_CONTROL1_REG  = COUNTERA_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        COUNTERA_SetInterruptMode(COUNTERA_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (COUNTERA__PWM_PR == COUNTERA_PWM_MODE)
            COUNTERA_TRIG_CONTROL2_REG =
                    (COUNTERA_CC_MATCH_NO_CHANGE    |
                    COUNTERA_OVERLOW_NO_CHANGE      |
                    COUNTERA_UNDERFLOW_NO_CHANGE);
        #else
            #if (COUNTERA__LEFT == COUNTERA_PWM_ALIGN)
                COUNTERA_TRIG_CONTROL2_REG = COUNTERA_PWM_MODE_LEFT;
            #endif  /* ( COUNTERA_PWM_LEFT == COUNTERA_PWM_ALIGN) */

            #if (COUNTERA__RIGHT == COUNTERA_PWM_ALIGN)
                COUNTERA_TRIG_CONTROL2_REG = COUNTERA_PWM_MODE_RIGHT;
            #endif  /* ( COUNTERA_PWM_RIGHT == COUNTERA_PWM_ALIGN) */

            #if (COUNTERA__CENTER == COUNTERA_PWM_ALIGN)
                COUNTERA_TRIG_CONTROL2_REG = COUNTERA_PWM_MODE_CENTER;
            #endif  /* ( COUNTERA_PWM_CENTER == COUNTERA_PWM_ALIGN) */

            #if (COUNTERA__ASYMMETRIC == COUNTERA_PWM_ALIGN)
                COUNTERA_TRIG_CONTROL2_REG = COUNTERA_PWM_MODE_ASYM;
            #endif  /* (COUNTERA__ASYMMETRIC == COUNTERA_PWM_ALIGN) */
        #endif  /* (COUNTERA__PWM_PR == COUNTERA_PWM_MODE) */

        /* Set other values from customizer */
        COUNTERA_WritePeriod(COUNTERA_PWM_PERIOD_VALUE );
        COUNTERA_WriteCompare(COUNTERA_PWM_COMPARE_VALUE);

        #if (1u == COUNTERA_PWM_COMPARE_SWAP)
            COUNTERA_SetCompareSwap(1u);
            COUNTERA_WriteCompareBuf(COUNTERA_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == COUNTERA_PWM_COMPARE_SWAP) */

        #if (1u == COUNTERA_PWM_PERIOD_SWAP)
            COUNTERA_SetPeriodSwap(1u);
            COUNTERA_WritePeriodBuf(COUNTERA_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == COUNTERA_PWM_PERIOD_SWAP) */
    #endif  /* (COUNTERA__PWM_SEL == COUNTERA_CONFIG) */
    
}


/*******************************************************************************
* Function Name: COUNTERA_Enable
********************************************************************************
*
* Summary:
*  Enables the COUNTERA.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    COUNTERA_BLOCK_CONTROL_REG |= COUNTERA_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (COUNTERA__PWM_SEL == COUNTERA_CONFIG)
        #if (0u == COUNTERA_PWM_START_SIGNAL_PRESENT)
            COUNTERA_TriggerCommand(COUNTERA_MASK, COUNTERA_CMD_START);
        #endif /* (0u == COUNTERA_PWM_START_SIGNAL_PRESENT) */
    #endif /* (COUNTERA__PWM_SEL == COUNTERA_CONFIG) */

    #if (COUNTERA__TIMER == COUNTERA_CONFIG)
        #if (0u == COUNTERA_TC_START_SIGNAL_PRESENT)
            COUNTERA_TriggerCommand(COUNTERA_MASK, COUNTERA_CMD_START);
        #endif /* (0u == COUNTERA_TC_START_SIGNAL_PRESENT) */
    #endif /* (COUNTERA__TIMER == COUNTERA_CONFIG) */
    
    #if (COUNTERA__QUAD == COUNTERA_CONFIG)
        #if (0u != COUNTERA_QUAD_AUTO_START)
            COUNTERA_TriggerCommand(COUNTERA_MASK, COUNTERA_CMD_RELOAD);
        #endif /* (0u != COUNTERA_QUAD_AUTO_START) */
    #endif  /* (COUNTERA__QUAD == COUNTERA_CONFIG) */
}


/*******************************************************************************
* Function Name: COUNTERA_Start
********************************************************************************
*
* Summary:
*  Initializes the COUNTERA with default customizer
*  values when called the first time and enables the COUNTERA.
*  For subsequent calls the configuration is left unchanged and the component is
*  just enabled.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  COUNTERA_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time COUNTERA_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the COUNTERA_Start() routine.
*
*******************************************************************************/
void COUNTERA_Start(void)
{
    if (0u == COUNTERA_initVar)
    {
        COUNTERA_Init();
        COUNTERA_initVar = 1u;
    }

    COUNTERA_Enable();
}


/*******************************************************************************
* Function Name: COUNTERA_Stop
********************************************************************************
*
* Summary:
*  Disables the COUNTERA.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_BLOCK_CONTROL_REG &= (uint32)~COUNTERA_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the COUNTERA. This function is used when
*  configured as a generic COUNTERA and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the COUNTERA to operate in
*   Values:
*   - COUNTERA_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - COUNTERA_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - COUNTERA_MODE_QUAD - Quadrature decoder
*         - COUNTERA_MODE_PWM - PWM
*         - COUNTERA_MODE_PWM_DT - PWM with dead time
*         - COUNTERA_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_CONTROL_REG &= (uint32)~COUNTERA_MODE_MASK;
    COUNTERA_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - COUNTERA_MODE_X1 - Counts on phi 1 rising
*         - COUNTERA_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - COUNTERA_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_CONTROL_REG &= (uint32)~COUNTERA_QUAD_MODE_MASK;
    COUNTERA_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - COUNTERA_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - COUNTERA_PRESCALE_DIVBY2    - Divide by 2
*         - COUNTERA_PRESCALE_DIVBY4    - Divide by 4
*         - COUNTERA_PRESCALE_DIVBY8    - Divide by 8
*         - COUNTERA_PRESCALE_DIVBY16   - Divide by 16
*         - COUNTERA_PRESCALE_DIVBY32   - Divide by 32
*         - COUNTERA_PRESCALE_DIVBY64   - Divide by 64
*         - COUNTERA_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_CONTROL_REG &= (uint32)~COUNTERA_PRESCALER_MASK;
    COUNTERA_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the COUNTERA runs
*  continuously or stops when terminal count is reached.  By default the
*  COUNTERA operates in the continuous mode.
*
* Parameters:
*  oneShotEnable
*   Values:
*     - 0 - Continuous
*     - 1 - Enable One Shot
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_CONTROL_REG &= (uint32)~COUNTERA_ONESHOT_MASK;
    COUNTERA_CONTROL_REG |= ((uint32)((oneShotEnable & COUNTERA_1BIT_MASK) <<
                                                               COUNTERA_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_SetPWMMode
********************************************************************************
*
* Summary:
*  Writes the control register that determines what mode of operation the PWM
*  output lines are driven in.  There is a setting for what to do on a
*  comparison match (CC_MATCH), on an overflow (OVERFLOW) and on an underflow
*  (UNDERFLOW).  The value for each of the three must be ORed together to form
*  the mode.
*
* Parameters:
*  modeMask: A combination of three mode settings.  Mask must include a value
*  for each of the three or use one of the preconfigured PWM settings.
*   Values:
*     - CC_MATCH_SET        - Set on comparison match
*     - CC_MATCH_CLEAR      - Clear on comparison match
*     - CC_MATCH_INVERT     - Invert on comparison match
*     - CC_MATCH_NO_CHANGE  - No change on comparison match
*     - OVERLOW_SET         - Set on overflow
*     - OVERLOW_CLEAR       - Clear on  overflow
*     - OVERLOW_INVERT      - Invert on overflow
*     - OVERLOW_NO_CHANGE   - No change on overflow
*     - UNDERFLOW_SET       - Set on underflow
*     - UNDERFLOW_CLEAR     - Clear on underflow
*     - UNDERFLOW_INVERT    - Invert on underflow
*     - UNDERFLOW_NO_CHANGE - No change on underflow
*     - PWM_MODE_LEFT       - Setting for left aligned PWM.  Should be combined
*                             with up counting mode
*     - PWM_MODE_RIGHT      - Setting for right aligned PWM.  Should be combined
*                             with down counting mode
*     - PWM_MODE_CENTER     - Setting for center aligned PWM.  Should be
*                             combined with up/down 0 mode
*     - PWM_MODE_ASYM       - Setting for asymmetric PWM.  Should be combined
*                             with up/down 1 mode
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetPWMMode(uint32 modeMask)
{
    COUNTERA_TRIG_CONTROL2_REG = (modeMask & COUNTERA_6BIT_MASK);
}



/*******************************************************************************
* Function Name: COUNTERA_SetPWMSyncKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes asynchronous or synchronous kill operation.  By default the kill
*  operation is asynchronous.  This functionality is only applicable to the PWM
*  and PWM with dead time modes.
*
*  For Synchronous mode the kill signal disables both the line and line_n
*  signals until the next terminal count.
*
*  For Asynchronous mode the kill signal disables both the line and line_n
*  signals when the kill signal is present.  This mode should only be used
*  when the kill signal (stop input) is configured in the pass through mode
*  (Level sensitive signal).

*
* Parameters:
*  syncKillEnable
*   Values:
*     - 0 - Asynchronous
*     - 1 - Synchronous
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_CONTROL_REG &= (uint32)~COUNTERA_PWM_SYNC_KILL_MASK;
    COUNTERA_CONTROL_REG |= ((uint32)((syncKillEnable & COUNTERA_1BIT_MASK)  <<
                                               COUNTERA_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_SetPWMStopOnKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes the PWM counter to stop.  By default the kill operation does not stop
*  the counter.  This functionality is only applicable to the three PWM modes.
*
*
* Parameters:
*  stopOnKillEnable
*   Values:
*     - 0 - Don't stop
*     - 1 - Stop
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_CONTROL_REG &= (uint32)~COUNTERA_PWM_STOP_KILL_MASK;
    COUNTERA_CONTROL_REG |= ((uint32)((stopOnKillEnable & COUNTERA_1BIT_MASK)  <<
                                                         COUNTERA_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_SetPWMDeadTime
********************************************************************************
*
* Summary:
*  Writes the dead time control value.  This value delays the rising edge of
*  both the line and line_n signals the designated number of cycles resulting
*  in both signals being inactive for that many cycles.  This functionality is
*  only applicable to the PWM in the dead time mode.

*
* Parameters:
*  Dead time to insert
*   Values: 0 to 255
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_CONTROL_REG &= (uint32)~COUNTERA_PRESCALER_MASK;
    COUNTERA_CONTROL_REG |= ((uint32)((deadTime & COUNTERA_8BIT_MASK) <<
                                                          COUNTERA_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_SetPWMInvert
********************************************************************************
*
* Summary:
*  Writes the bits that control whether the line and line_n outputs are
*  inverted from their normal output values.  This functionality is only
*  applicable to the three PWM modes.
*
* Parameters:
*  mask: Mask of outputs to invert.
*   Values:
*         - COUNTERA_INVERT_LINE   - Inverts the line output
*         - COUNTERA_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_CONTROL_REG &= (uint32)~COUNTERA_INV_OUT_MASK;
    COUNTERA_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: COUNTERA_WriteCounter
********************************************************************************
*
* Summary:
*  Writes a new 16bit counter value directly into the counter register, thus
*  setting the counter (not the period) to the value written. It is not
*  advised to write to this field when the counter is running.
*
* Parameters:
*  count: value to write
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_WriteCounter(uint32 count)
{
    COUNTERA_COUNTER_REG = (count & COUNTERA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTERA_ReadCounter
********************************************************************************
*
* Summary:
*  Reads the current counter value.
*
* Parameters:
*  None
*
* Return:
*  Current counter value
*
*******************************************************************************/
uint32 COUNTERA_ReadCounter(void)
{
    return (COUNTERA_COUNTER_REG & COUNTERA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTERA_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - COUNTERA_COUNT_UP       - Counts up
*     - COUNTERA_COUNT_DOWN     - Counts down
*     - COUNTERA_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - COUNTERA_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_CONTROL_REG &= (uint32)~COUNTERA_UPDOWN_MASK;
    COUNTERA_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_WritePeriod
********************************************************************************
*
* Summary:
*  Writes the 16 bit period register with the new period value.
*  To cause the counter to count for N cycles this register should be written
*  with N-1 (counts from 0 to period inclusive).
*
* Parameters:
*  period: Period value
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_WritePeriod(uint32 period)
{
    COUNTERA_PERIOD_REG = (period & COUNTERA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTERA_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the 16 bit period register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 COUNTERA_ReadPeriod(void)
{
    return (COUNTERA_PERIOD_REG & COUNTERA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTERA_SetCompareSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the compare registers are
*  swapped. When enabled in the Timer/Counter mode(without capture) the swap
*  occurs at a TC event. In the PWM mode the swap occurs at the next TC event
*  following a hardware switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_CONTROL_REG &= (uint32)~COUNTERA_RELOAD_CC_MASK;
    COUNTERA_CONTROL_REG |= (swapEnable & COUNTERA_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_WritePeriodBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit period buf register with the new period value.
*
* Parameters:
*  periodBuf: Period value
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_WritePeriodBuf(uint32 periodBuf)
{
    COUNTERA_PERIOD_BUF_REG = (periodBuf & COUNTERA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTERA_ReadPeriodBuf
********************************************************************************
*
* Summary:
*  Reads the 16 bit period buf register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 COUNTERA_ReadPeriodBuf(void)
{
    return (COUNTERA_PERIOD_BUF_REG & COUNTERA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTERA_SetPeriodSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the period registers are
*  swapped. When enabled in Timer/Counter mode the swap occurs at a TC event.
*  In the PWM mode the swap occurs at the next TC event following a hardware
*  switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_CONTROL_REG &= (uint32)~COUNTERA_RELOAD_PERIOD_MASK;
    COUNTERA_CONTROL_REG |= ((uint32)((swapEnable & COUNTERA_1BIT_MASK) <<
                                                            COUNTERA_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_WriteCompare
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compare: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void COUNTERA_WriteCompare(uint32 compare)
{
    #if (COUNTERA_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (COUNTERA_CY_TCPWM_4000) */

    #if (COUNTERA_CY_TCPWM_4000)
        currentMode = ((COUNTERA_CONTROL_REG & COUNTERA_UPDOWN_MASK) >> COUNTERA_UPDOWN_SHIFT);

        if (((uint32)COUNTERA__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)COUNTERA__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (COUNTERA_CY_TCPWM_4000) */
    
    COUNTERA_COMP_CAP_REG = (compare & COUNTERA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTERA_ReadCompare
********************************************************************************
*
* Summary:
*  Reads the compare register. Not applicable for Timer/Counter with Capture
*  or in Quadrature Decoder modes.
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
* Parameters:
*  None
*
* Return:
*  Compare value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 COUNTERA_ReadCompare(void)
{
    #if (COUNTERA_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (COUNTERA_CY_TCPWM_4000) */

    #if (COUNTERA_CY_TCPWM_4000)
        currentMode = ((COUNTERA_CONTROL_REG & COUNTERA_UPDOWN_MASK) >> COUNTERA_UPDOWN_SHIFT);
        
        regVal = COUNTERA_COMP_CAP_REG;
        
        if (((uint32)COUNTERA__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)COUNTERA__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & COUNTERA_16BIT_MASK);
    #else
        return (COUNTERA_COMP_CAP_REG & COUNTERA_16BIT_MASK);
    #endif /* (COUNTERA_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: COUNTERA_WriteCompareBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare buffer register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compareBuf: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void COUNTERA_WriteCompareBuf(uint32 compareBuf)
{
    #if (COUNTERA_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (COUNTERA_CY_TCPWM_4000) */

    #if (COUNTERA_CY_TCPWM_4000)
        currentMode = ((COUNTERA_CONTROL_REG & COUNTERA_UPDOWN_MASK) >> COUNTERA_UPDOWN_SHIFT);

        if (((uint32)COUNTERA__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)COUNTERA__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (COUNTERA_CY_TCPWM_4000) */
    
    COUNTERA_COMP_CAP_BUF_REG = (compareBuf & COUNTERA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTERA_ReadCompareBuf
********************************************************************************
*
* Summary:
*  Reads the compare buffer register. Not applicable for Timer/Counter with
*  Capture or in Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Compare buffer value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 COUNTERA_ReadCompareBuf(void)
{
    #if (COUNTERA_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (COUNTERA_CY_TCPWM_4000) */

    #if (COUNTERA_CY_TCPWM_4000)
        currentMode = ((COUNTERA_CONTROL_REG & COUNTERA_UPDOWN_MASK) >> COUNTERA_UPDOWN_SHIFT);

        regVal = COUNTERA_COMP_CAP_BUF_REG;
        
        if (((uint32)COUNTERA__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)COUNTERA__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & COUNTERA_16BIT_MASK);
    #else
        return (COUNTERA_COMP_CAP_BUF_REG & COUNTERA_16BIT_MASK);
    #endif /* (COUNTERA_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: COUNTERA_ReadCapture
********************************************************************************
*
* Summary:
*  Reads the captured counter value. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture value
*
*******************************************************************************/
uint32 COUNTERA_ReadCapture(void)
{
    return (COUNTERA_COMP_CAP_REG & COUNTERA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTERA_ReadCaptureBuf
********************************************************************************
*
* Summary:
*  Reads the capture buffer register. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture buffer value
*
*******************************************************************************/
uint32 COUNTERA_ReadCaptureBuf(void)
{
    return (COUNTERA_COMP_CAP_BUF_REG & COUNTERA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTERA_SetCaptureMode
********************************************************************************
*
* Summary:
*  Sets the capture trigger mode. For PWM mode this is the switch input.
*  This input is not applicable to the Timer/Counter without Capture and
*  Quadrature Decoder modes.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - COUNTERA_TRIG_LEVEL     - Level
*     - COUNTERA_TRIG_RISING    - Rising edge
*     - COUNTERA_TRIG_FALLING   - Falling edge
*     - COUNTERA_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_TRIG_CONTROL1_REG &= (uint32)~COUNTERA_CAPTURE_MASK;
    COUNTERA_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - COUNTERA_TRIG_LEVEL     - Level
*     - COUNTERA_TRIG_RISING    - Rising edge
*     - COUNTERA_TRIG_FALLING   - Falling edge
*     - COUNTERA_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_TRIG_CONTROL1_REG &= (uint32)~COUNTERA_RELOAD_MASK;
    COUNTERA_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << COUNTERA_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - COUNTERA_TRIG_LEVEL     - Level
*     - COUNTERA_TRIG_RISING    - Rising edge
*     - COUNTERA_TRIG_FALLING   - Falling edge
*     - COUNTERA_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_TRIG_CONTROL1_REG &= (uint32)~COUNTERA_START_MASK;
    COUNTERA_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << COUNTERA_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - COUNTERA_TRIG_LEVEL     - Level
*     - COUNTERA_TRIG_RISING    - Rising edge
*     - COUNTERA_TRIG_FALLING   - Falling edge
*     - COUNTERA_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_TRIG_CONTROL1_REG &= (uint32)~COUNTERA_STOP_MASK;
    COUNTERA_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << COUNTERA_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - COUNTERA_TRIG_LEVEL     - Level
*     - COUNTERA_TRIG_RISING    - Rising edge
*     - COUNTERA_TRIG_FALLING   - Falling edge
*     - COUNTERA_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_TRIG_CONTROL1_REG &= (uint32)~COUNTERA_COUNT_MASK;
    COUNTERA_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << COUNTERA_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_TriggerCommand
********************************************************************************
*
* Summary:
*  Triggers the designated command to occur on the designated TCPWM instances.
*  The mask can be used to apply this command simultaneously to more than one
*  instance.  This allows multiple TCPWM instances to be synchronized.
*
* Parameters:
*  mask: A combination of mask bits for each instance of the TCPWM that the
*        command should apply to.  This function from one instance can be used
*        to apply the command to any of the instances in the design.
*        The mask value for a specific instance is available with the MASK
*        define.
*  command: Enumerated command values. Capture command only applicable for
*           Timer/Counter with Capture and PWM modes.
*   Values:
*     - COUNTERA_CMD_CAPTURE    - Trigger Capture/Switch command
*     - COUNTERA_CMD_RELOAD     - Trigger Reload/Index command
*     - COUNTERA_CMD_STOP       - Trigger Stop/Kill command
*     - COUNTERA_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTERA_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTERA_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the COUNTERA.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - COUNTERA_STATUS_DOWN    - Set if counting down
*     - COUNTERA_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 COUNTERA_ReadStatus(void)
{
    return ((COUNTERA_STATUS_REG >> COUNTERA_RUNNING_STATUS_SHIFT) |
            (COUNTERA_STATUS_REG & COUNTERA_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: COUNTERA_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - COUNTERA_INTR_MASK_TC       - Terminal count mask
*     - COUNTERA_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetInterruptMode(uint32 interruptMask)
{
    COUNTERA_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: COUNTERA_GetInterruptSourceMasked
********************************************************************************
*
* Summary:
*  Gets the interrupt requests masked by the interrupt mask.
*
* Parameters:
*   None
*
* Return:
*  Masked interrupt source
*   Values:
*     - COUNTERA_INTR_MASK_TC       - Terminal count mask
*     - COUNTERA_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 COUNTERA_GetInterruptSourceMasked(void)
{
    return (COUNTERA_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: COUNTERA_GetInterruptSource
********************************************************************************
*
* Summary:
*  Gets the interrupt requests (without masking).
*
* Parameters:
*  None
*
* Return:
*  Interrupt request value
*   Values:
*     - COUNTERA_INTR_MASK_TC       - Terminal count mask
*     - COUNTERA_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 COUNTERA_GetInterruptSource(void)
{
    return (COUNTERA_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: COUNTERA_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - COUNTERA_INTR_MASK_TC       - Terminal count mask
*     - COUNTERA_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_ClearInterrupt(uint32 interruptMask)
{
    COUNTERA_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: COUNTERA_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - COUNTERA_INTR_MASK_TC       - Terminal count mask
*     - COUNTERA_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void COUNTERA_SetInterrupt(uint32 interruptMask)
{
    COUNTERA_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
