/*******************************************************************************
* File Name: COUNTUPA.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the COUNTUPA
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

#include "COUNTUPA.h"

uint8 COUNTUPA_initVar = 0u;


/*******************************************************************************
* Function Name: COUNTUPA_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default COUNTUPA configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (COUNTUPA__QUAD == COUNTUPA_CONFIG)
        COUNTUPA_CONTROL_REG = COUNTUPA_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        COUNTUPA_TRIG_CONTROL1_REG  = COUNTUPA_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        COUNTUPA_SetInterruptMode(COUNTUPA_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        COUNTUPA_SetCounterMode(COUNTUPA_COUNT_DOWN);
        COUNTUPA_WritePeriod(COUNTUPA_QUAD_PERIOD_INIT_VALUE);
        COUNTUPA_WriteCounter(COUNTUPA_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (COUNTUPA__QUAD == COUNTUPA_CONFIG) */

    #if (COUNTUPA__TIMER == COUNTUPA_CONFIG)
        COUNTUPA_CONTROL_REG = COUNTUPA_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        COUNTUPA_TRIG_CONTROL1_REG  = COUNTUPA_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        COUNTUPA_SetInterruptMode(COUNTUPA_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        COUNTUPA_WritePeriod(COUNTUPA_TC_PERIOD_VALUE );

        #if (COUNTUPA__COMPARE == COUNTUPA_TC_COMP_CAP_MODE)
            COUNTUPA_WriteCompare(COUNTUPA_TC_COMPARE_VALUE);

            #if (1u == COUNTUPA_TC_COMPARE_SWAP)
                COUNTUPA_SetCompareSwap(1u);
                COUNTUPA_WriteCompareBuf(COUNTUPA_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == COUNTUPA_TC_COMPARE_SWAP) */
        #endif  /* (COUNTUPA__COMPARE == COUNTUPA_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (COUNTUPA_CY_TCPWM_V2 && COUNTUPA_TIMER_UPDOWN_CNT_USED && !COUNTUPA_CY_TCPWM_4000)
            COUNTUPA_WriteCounter(1u);
        #elif(COUNTUPA__COUNT_DOWN == COUNTUPA_TC_COUNTER_MODE)
            COUNTUPA_WriteCounter(COUNTUPA_TC_PERIOD_VALUE);
        #else
            COUNTUPA_WriteCounter(0u);
        #endif /* (COUNTUPA_CY_TCPWM_V2 && COUNTUPA_TIMER_UPDOWN_CNT_USED && !COUNTUPA_CY_TCPWM_4000) */
    #endif  /* (COUNTUPA__TIMER == COUNTUPA_CONFIG) */

    #if (COUNTUPA__PWM_SEL == COUNTUPA_CONFIG)
        COUNTUPA_CONTROL_REG = COUNTUPA_CTRL_PWM_BASE_CONFIG;

        #if (COUNTUPA__PWM_PR == COUNTUPA_PWM_MODE)
            COUNTUPA_CONTROL_REG |= COUNTUPA_CTRL_PWM_RUN_MODE;
            COUNTUPA_WriteCounter(COUNTUPA_PWM_PR_INIT_VALUE);
        #else
            COUNTUPA_CONTROL_REG |= COUNTUPA_CTRL_PWM_ALIGN | COUNTUPA_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (COUNTUPA_CY_TCPWM_V2 && COUNTUPA_PWM_UPDOWN_CNT_USED && !COUNTUPA_CY_TCPWM_4000)
                COUNTUPA_WriteCounter(1u);
            #elif (COUNTUPA__RIGHT == COUNTUPA_PWM_ALIGN)
                COUNTUPA_WriteCounter(COUNTUPA_PWM_PERIOD_VALUE);
            #else 
                COUNTUPA_WriteCounter(0u);
            #endif  /* (COUNTUPA_CY_TCPWM_V2 && COUNTUPA_PWM_UPDOWN_CNT_USED && !COUNTUPA_CY_TCPWM_4000) */
        #endif  /* (COUNTUPA__PWM_PR == COUNTUPA_PWM_MODE) */

        #if (COUNTUPA__PWM_DT == COUNTUPA_PWM_MODE)
            COUNTUPA_CONTROL_REG |= COUNTUPA_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (COUNTUPA__PWM_DT == COUNTUPA_PWM_MODE) */

        #if (COUNTUPA__PWM == COUNTUPA_PWM_MODE)
            COUNTUPA_CONTROL_REG |= COUNTUPA_CTRL_PWM_PRESCALER;
        #endif  /* (COUNTUPA__PWM == COUNTUPA_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        COUNTUPA_TRIG_CONTROL1_REG  = COUNTUPA_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        COUNTUPA_SetInterruptMode(COUNTUPA_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (COUNTUPA__PWM_PR == COUNTUPA_PWM_MODE)
            COUNTUPA_TRIG_CONTROL2_REG =
                    (COUNTUPA_CC_MATCH_NO_CHANGE    |
                    COUNTUPA_OVERLOW_NO_CHANGE      |
                    COUNTUPA_UNDERFLOW_NO_CHANGE);
        #else
            #if (COUNTUPA__LEFT == COUNTUPA_PWM_ALIGN)
                COUNTUPA_TRIG_CONTROL2_REG = COUNTUPA_PWM_MODE_LEFT;
            #endif  /* ( COUNTUPA_PWM_LEFT == COUNTUPA_PWM_ALIGN) */

            #if (COUNTUPA__RIGHT == COUNTUPA_PWM_ALIGN)
                COUNTUPA_TRIG_CONTROL2_REG = COUNTUPA_PWM_MODE_RIGHT;
            #endif  /* ( COUNTUPA_PWM_RIGHT == COUNTUPA_PWM_ALIGN) */

            #if (COUNTUPA__CENTER == COUNTUPA_PWM_ALIGN)
                COUNTUPA_TRIG_CONTROL2_REG = COUNTUPA_PWM_MODE_CENTER;
            #endif  /* ( COUNTUPA_PWM_CENTER == COUNTUPA_PWM_ALIGN) */

            #if (COUNTUPA__ASYMMETRIC == COUNTUPA_PWM_ALIGN)
                COUNTUPA_TRIG_CONTROL2_REG = COUNTUPA_PWM_MODE_ASYM;
            #endif  /* (COUNTUPA__ASYMMETRIC == COUNTUPA_PWM_ALIGN) */
        #endif  /* (COUNTUPA__PWM_PR == COUNTUPA_PWM_MODE) */

        /* Set other values from customizer */
        COUNTUPA_WritePeriod(COUNTUPA_PWM_PERIOD_VALUE );
        COUNTUPA_WriteCompare(COUNTUPA_PWM_COMPARE_VALUE);

        #if (1u == COUNTUPA_PWM_COMPARE_SWAP)
            COUNTUPA_SetCompareSwap(1u);
            COUNTUPA_WriteCompareBuf(COUNTUPA_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == COUNTUPA_PWM_COMPARE_SWAP) */

        #if (1u == COUNTUPA_PWM_PERIOD_SWAP)
            COUNTUPA_SetPeriodSwap(1u);
            COUNTUPA_WritePeriodBuf(COUNTUPA_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == COUNTUPA_PWM_PERIOD_SWAP) */
    #endif  /* (COUNTUPA__PWM_SEL == COUNTUPA_CONFIG) */
    
}


/*******************************************************************************
* Function Name: COUNTUPA_Enable
********************************************************************************
*
* Summary:
*  Enables the COUNTUPA.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    COUNTUPA_BLOCK_CONTROL_REG |= COUNTUPA_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (COUNTUPA__PWM_SEL == COUNTUPA_CONFIG)
        #if (0u == COUNTUPA_PWM_START_SIGNAL_PRESENT)
            COUNTUPA_TriggerCommand(COUNTUPA_MASK, COUNTUPA_CMD_START);
        #endif /* (0u == COUNTUPA_PWM_START_SIGNAL_PRESENT) */
    #endif /* (COUNTUPA__PWM_SEL == COUNTUPA_CONFIG) */

    #if (COUNTUPA__TIMER == COUNTUPA_CONFIG)
        #if (0u == COUNTUPA_TC_START_SIGNAL_PRESENT)
            COUNTUPA_TriggerCommand(COUNTUPA_MASK, COUNTUPA_CMD_START);
        #endif /* (0u == COUNTUPA_TC_START_SIGNAL_PRESENT) */
    #endif /* (COUNTUPA__TIMER == COUNTUPA_CONFIG) */
    
    #if (COUNTUPA__QUAD == COUNTUPA_CONFIG)
        #if (0u != COUNTUPA_QUAD_AUTO_START)
            COUNTUPA_TriggerCommand(COUNTUPA_MASK, COUNTUPA_CMD_RELOAD);
        #endif /* (0u != COUNTUPA_QUAD_AUTO_START) */
    #endif  /* (COUNTUPA__QUAD == COUNTUPA_CONFIG) */
}


/*******************************************************************************
* Function Name: COUNTUPA_Start
********************************************************************************
*
* Summary:
*  Initializes the COUNTUPA with default customizer
*  values when called the first time and enables the COUNTUPA.
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
*  COUNTUPA_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time COUNTUPA_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the COUNTUPA_Start() routine.
*
*******************************************************************************/
void COUNTUPA_Start(void)
{
    if (0u == COUNTUPA_initVar)
    {
        COUNTUPA_Init();
        COUNTUPA_initVar = 1u;
    }

    COUNTUPA_Enable();
}


/*******************************************************************************
* Function Name: COUNTUPA_Stop
********************************************************************************
*
* Summary:
*  Disables the COUNTUPA.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_BLOCK_CONTROL_REG &= (uint32)~COUNTUPA_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the COUNTUPA. This function is used when
*  configured as a generic COUNTUPA and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the COUNTUPA to operate in
*   Values:
*   - COUNTUPA_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - COUNTUPA_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - COUNTUPA_MODE_QUAD - Quadrature decoder
*         - COUNTUPA_MODE_PWM - PWM
*         - COUNTUPA_MODE_PWM_DT - PWM with dead time
*         - COUNTUPA_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_CONTROL_REG &= (uint32)~COUNTUPA_MODE_MASK;
    COUNTUPA_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - COUNTUPA_MODE_X1 - Counts on phi 1 rising
*         - COUNTUPA_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - COUNTUPA_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_CONTROL_REG &= (uint32)~COUNTUPA_QUAD_MODE_MASK;
    COUNTUPA_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - COUNTUPA_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - COUNTUPA_PRESCALE_DIVBY2    - Divide by 2
*         - COUNTUPA_PRESCALE_DIVBY4    - Divide by 4
*         - COUNTUPA_PRESCALE_DIVBY8    - Divide by 8
*         - COUNTUPA_PRESCALE_DIVBY16   - Divide by 16
*         - COUNTUPA_PRESCALE_DIVBY32   - Divide by 32
*         - COUNTUPA_PRESCALE_DIVBY64   - Divide by 64
*         - COUNTUPA_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_CONTROL_REG &= (uint32)~COUNTUPA_PRESCALER_MASK;
    COUNTUPA_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the COUNTUPA runs
*  continuously or stops when terminal count is reached.  By default the
*  COUNTUPA operates in the continuous mode.
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
void COUNTUPA_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_CONTROL_REG &= (uint32)~COUNTUPA_ONESHOT_MASK;
    COUNTUPA_CONTROL_REG |= ((uint32)((oneShotEnable & COUNTUPA_1BIT_MASK) <<
                                                               COUNTUPA_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetPWMMode
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
void COUNTUPA_SetPWMMode(uint32 modeMask)
{
    COUNTUPA_TRIG_CONTROL2_REG = (modeMask & COUNTUPA_6BIT_MASK);
}



/*******************************************************************************
* Function Name: COUNTUPA_SetPWMSyncKill
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
void COUNTUPA_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_CONTROL_REG &= (uint32)~COUNTUPA_PWM_SYNC_KILL_MASK;
    COUNTUPA_CONTROL_REG |= ((uint32)((syncKillEnable & COUNTUPA_1BIT_MASK)  <<
                                               COUNTUPA_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetPWMStopOnKill
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
void COUNTUPA_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_CONTROL_REG &= (uint32)~COUNTUPA_PWM_STOP_KILL_MASK;
    COUNTUPA_CONTROL_REG |= ((uint32)((stopOnKillEnable & COUNTUPA_1BIT_MASK)  <<
                                                         COUNTUPA_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetPWMDeadTime
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
void COUNTUPA_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_CONTROL_REG &= (uint32)~COUNTUPA_PRESCALER_MASK;
    COUNTUPA_CONTROL_REG |= ((uint32)((deadTime & COUNTUPA_8BIT_MASK) <<
                                                          COUNTUPA_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetPWMInvert
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
*         - COUNTUPA_INVERT_LINE   - Inverts the line output
*         - COUNTUPA_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_CONTROL_REG &= (uint32)~COUNTUPA_INV_OUT_MASK;
    COUNTUPA_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: COUNTUPA_WriteCounter
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
void COUNTUPA_WriteCounter(uint32 count)
{
    COUNTUPA_COUNTER_REG = (count & COUNTUPA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTUPA_ReadCounter
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
uint32 COUNTUPA_ReadCounter(void)
{
    return (COUNTUPA_COUNTER_REG & COUNTUPA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - COUNTUPA_COUNT_UP       - Counts up
*     - COUNTUPA_COUNT_DOWN     - Counts down
*     - COUNTUPA_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - COUNTUPA_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_CONTROL_REG &= (uint32)~COUNTUPA_UPDOWN_MASK;
    COUNTUPA_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_WritePeriod
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
void COUNTUPA_WritePeriod(uint32 period)
{
    COUNTUPA_PERIOD_REG = (period & COUNTUPA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTUPA_ReadPeriod
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
uint32 COUNTUPA_ReadPeriod(void)
{
    return (COUNTUPA_PERIOD_REG & COUNTUPA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetCompareSwap
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
void COUNTUPA_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_CONTROL_REG &= (uint32)~COUNTUPA_RELOAD_CC_MASK;
    COUNTUPA_CONTROL_REG |= (swapEnable & COUNTUPA_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_WritePeriodBuf
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
void COUNTUPA_WritePeriodBuf(uint32 periodBuf)
{
    COUNTUPA_PERIOD_BUF_REG = (periodBuf & COUNTUPA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTUPA_ReadPeriodBuf
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
uint32 COUNTUPA_ReadPeriodBuf(void)
{
    return (COUNTUPA_PERIOD_BUF_REG & COUNTUPA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetPeriodSwap
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
void COUNTUPA_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_CONTROL_REG &= (uint32)~COUNTUPA_RELOAD_PERIOD_MASK;
    COUNTUPA_CONTROL_REG |= ((uint32)((swapEnable & COUNTUPA_1BIT_MASK) <<
                                                            COUNTUPA_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_WriteCompare
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
void COUNTUPA_WriteCompare(uint32 compare)
{
    #if (COUNTUPA_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (COUNTUPA_CY_TCPWM_4000) */

    #if (COUNTUPA_CY_TCPWM_4000)
        currentMode = ((COUNTUPA_CONTROL_REG & COUNTUPA_UPDOWN_MASK) >> COUNTUPA_UPDOWN_SHIFT);

        if (((uint32)COUNTUPA__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)COUNTUPA__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (COUNTUPA_CY_TCPWM_4000) */
    
    COUNTUPA_COMP_CAP_REG = (compare & COUNTUPA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTUPA_ReadCompare
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
uint32 COUNTUPA_ReadCompare(void)
{
    #if (COUNTUPA_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (COUNTUPA_CY_TCPWM_4000) */

    #if (COUNTUPA_CY_TCPWM_4000)
        currentMode = ((COUNTUPA_CONTROL_REG & COUNTUPA_UPDOWN_MASK) >> COUNTUPA_UPDOWN_SHIFT);
        
        regVal = COUNTUPA_COMP_CAP_REG;
        
        if (((uint32)COUNTUPA__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)COUNTUPA__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & COUNTUPA_16BIT_MASK);
    #else
        return (COUNTUPA_COMP_CAP_REG & COUNTUPA_16BIT_MASK);
    #endif /* (COUNTUPA_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: COUNTUPA_WriteCompareBuf
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
void COUNTUPA_WriteCompareBuf(uint32 compareBuf)
{
    #if (COUNTUPA_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (COUNTUPA_CY_TCPWM_4000) */

    #if (COUNTUPA_CY_TCPWM_4000)
        currentMode = ((COUNTUPA_CONTROL_REG & COUNTUPA_UPDOWN_MASK) >> COUNTUPA_UPDOWN_SHIFT);

        if (((uint32)COUNTUPA__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)COUNTUPA__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (COUNTUPA_CY_TCPWM_4000) */
    
    COUNTUPA_COMP_CAP_BUF_REG = (compareBuf & COUNTUPA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTUPA_ReadCompareBuf
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
uint32 COUNTUPA_ReadCompareBuf(void)
{
    #if (COUNTUPA_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (COUNTUPA_CY_TCPWM_4000) */

    #if (COUNTUPA_CY_TCPWM_4000)
        currentMode = ((COUNTUPA_CONTROL_REG & COUNTUPA_UPDOWN_MASK) >> COUNTUPA_UPDOWN_SHIFT);

        regVal = COUNTUPA_COMP_CAP_BUF_REG;
        
        if (((uint32)COUNTUPA__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)COUNTUPA__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & COUNTUPA_16BIT_MASK);
    #else
        return (COUNTUPA_COMP_CAP_BUF_REG & COUNTUPA_16BIT_MASK);
    #endif /* (COUNTUPA_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: COUNTUPA_ReadCapture
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
uint32 COUNTUPA_ReadCapture(void)
{
    return (COUNTUPA_COMP_CAP_REG & COUNTUPA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTUPA_ReadCaptureBuf
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
uint32 COUNTUPA_ReadCaptureBuf(void)
{
    return (COUNTUPA_COMP_CAP_BUF_REG & COUNTUPA_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetCaptureMode
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
*     - COUNTUPA_TRIG_LEVEL     - Level
*     - COUNTUPA_TRIG_RISING    - Rising edge
*     - COUNTUPA_TRIG_FALLING   - Falling edge
*     - COUNTUPA_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_TRIG_CONTROL1_REG &= (uint32)~COUNTUPA_CAPTURE_MASK;
    COUNTUPA_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - COUNTUPA_TRIG_LEVEL     - Level
*     - COUNTUPA_TRIG_RISING    - Rising edge
*     - COUNTUPA_TRIG_FALLING   - Falling edge
*     - COUNTUPA_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_TRIG_CONTROL1_REG &= (uint32)~COUNTUPA_RELOAD_MASK;
    COUNTUPA_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << COUNTUPA_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - COUNTUPA_TRIG_LEVEL     - Level
*     - COUNTUPA_TRIG_RISING    - Rising edge
*     - COUNTUPA_TRIG_FALLING   - Falling edge
*     - COUNTUPA_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_TRIG_CONTROL1_REG &= (uint32)~COUNTUPA_START_MASK;
    COUNTUPA_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << COUNTUPA_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - COUNTUPA_TRIG_LEVEL     - Level
*     - COUNTUPA_TRIG_RISING    - Rising edge
*     - COUNTUPA_TRIG_FALLING   - Falling edge
*     - COUNTUPA_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_TRIG_CONTROL1_REG &= (uint32)~COUNTUPA_STOP_MASK;
    COUNTUPA_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << COUNTUPA_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - COUNTUPA_TRIG_LEVEL     - Level
*     - COUNTUPA_TRIG_RISING    - Rising edge
*     - COUNTUPA_TRIG_FALLING   - Falling edge
*     - COUNTUPA_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_TRIG_CONTROL1_REG &= (uint32)~COUNTUPA_COUNT_MASK;
    COUNTUPA_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << COUNTUPA_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_TriggerCommand
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
*     - COUNTUPA_CMD_CAPTURE    - Trigger Capture/Switch command
*     - COUNTUPA_CMD_RELOAD     - Trigger Reload/Index command
*     - COUNTUPA_CMD_STOP       - Trigger Stop/Kill command
*     - COUNTUPA_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTUPA_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTUPA_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the COUNTUPA.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - COUNTUPA_STATUS_DOWN    - Set if counting down
*     - COUNTUPA_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 COUNTUPA_ReadStatus(void)
{
    return ((COUNTUPA_STATUS_REG >> COUNTUPA_RUNNING_STATUS_SHIFT) |
            (COUNTUPA_STATUS_REG & COUNTUPA_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: COUNTUPA_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - COUNTUPA_INTR_MASK_TC       - Terminal count mask
*     - COUNTUPA_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_SetInterruptMode(uint32 interruptMask)
{
    COUNTUPA_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: COUNTUPA_GetInterruptSourceMasked
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
*     - COUNTUPA_INTR_MASK_TC       - Terminal count mask
*     - COUNTUPA_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 COUNTUPA_GetInterruptSourceMasked(void)
{
    return (COUNTUPA_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: COUNTUPA_GetInterruptSource
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
*     - COUNTUPA_INTR_MASK_TC       - Terminal count mask
*     - COUNTUPA_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 COUNTUPA_GetInterruptSource(void)
{
    return (COUNTUPA_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: COUNTUPA_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - COUNTUPA_INTR_MASK_TC       - Terminal count mask
*     - COUNTUPA_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_ClearInterrupt(uint32 interruptMask)
{
    COUNTUPA_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: COUNTUPA_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - COUNTUPA_INTR_MASK_TC       - Terminal count mask
*     - COUNTUPA_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void COUNTUPA_SetInterrupt(uint32 interruptMask)
{
    COUNTUPA_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
