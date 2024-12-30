/*******************************************************************************
* File Name: COUNTER2.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the COUNTER2
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

#include "COUNTER2.h"

uint8 COUNTER2_initVar = 0u;


/*******************************************************************************
* Function Name: COUNTER2_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default COUNTER2 configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (COUNTER2__QUAD == COUNTER2_CONFIG)
        COUNTER2_CONTROL_REG = COUNTER2_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        COUNTER2_TRIG_CONTROL1_REG  = COUNTER2_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        COUNTER2_SetInterruptMode(COUNTER2_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        COUNTER2_SetCounterMode(COUNTER2_COUNT_DOWN);
        COUNTER2_WritePeriod(COUNTER2_QUAD_PERIOD_INIT_VALUE);
        COUNTER2_WriteCounter(COUNTER2_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (COUNTER2__QUAD == COUNTER2_CONFIG) */

    #if (COUNTER2__TIMER == COUNTER2_CONFIG)
        COUNTER2_CONTROL_REG = COUNTER2_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        COUNTER2_TRIG_CONTROL1_REG  = COUNTER2_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        COUNTER2_SetInterruptMode(COUNTER2_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        COUNTER2_WritePeriod(COUNTER2_TC_PERIOD_VALUE );

        #if (COUNTER2__COMPARE == COUNTER2_TC_COMP_CAP_MODE)
            COUNTER2_WriteCompare(COUNTER2_TC_COMPARE_VALUE);

            #if (1u == COUNTER2_TC_COMPARE_SWAP)
                COUNTER2_SetCompareSwap(1u);
                COUNTER2_WriteCompareBuf(COUNTER2_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == COUNTER2_TC_COMPARE_SWAP) */
        #endif  /* (COUNTER2__COMPARE == COUNTER2_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (COUNTER2_CY_TCPWM_V2 && COUNTER2_TIMER_UPDOWN_CNT_USED && !COUNTER2_CY_TCPWM_4000)
            COUNTER2_WriteCounter(1u);
        #elif(COUNTER2__COUNT_DOWN == COUNTER2_TC_COUNTER_MODE)
            COUNTER2_WriteCounter(COUNTER2_TC_PERIOD_VALUE);
        #else
            COUNTER2_WriteCounter(0u);
        #endif /* (COUNTER2_CY_TCPWM_V2 && COUNTER2_TIMER_UPDOWN_CNT_USED && !COUNTER2_CY_TCPWM_4000) */
    #endif  /* (COUNTER2__TIMER == COUNTER2_CONFIG) */

    #if (COUNTER2__PWM_SEL == COUNTER2_CONFIG)
        COUNTER2_CONTROL_REG = COUNTER2_CTRL_PWM_BASE_CONFIG;

        #if (COUNTER2__PWM_PR == COUNTER2_PWM_MODE)
            COUNTER2_CONTROL_REG |= COUNTER2_CTRL_PWM_RUN_MODE;
            COUNTER2_WriteCounter(COUNTER2_PWM_PR_INIT_VALUE);
        #else
            COUNTER2_CONTROL_REG |= COUNTER2_CTRL_PWM_ALIGN | COUNTER2_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (COUNTER2_CY_TCPWM_V2 && COUNTER2_PWM_UPDOWN_CNT_USED && !COUNTER2_CY_TCPWM_4000)
                COUNTER2_WriteCounter(1u);
            #elif (COUNTER2__RIGHT == COUNTER2_PWM_ALIGN)
                COUNTER2_WriteCounter(COUNTER2_PWM_PERIOD_VALUE);
            #else 
                COUNTER2_WriteCounter(0u);
            #endif  /* (COUNTER2_CY_TCPWM_V2 && COUNTER2_PWM_UPDOWN_CNT_USED && !COUNTER2_CY_TCPWM_4000) */
        #endif  /* (COUNTER2__PWM_PR == COUNTER2_PWM_MODE) */

        #if (COUNTER2__PWM_DT == COUNTER2_PWM_MODE)
            COUNTER2_CONTROL_REG |= COUNTER2_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (COUNTER2__PWM_DT == COUNTER2_PWM_MODE) */

        #if (COUNTER2__PWM == COUNTER2_PWM_MODE)
            COUNTER2_CONTROL_REG |= COUNTER2_CTRL_PWM_PRESCALER;
        #endif  /* (COUNTER2__PWM == COUNTER2_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        COUNTER2_TRIG_CONTROL1_REG  = COUNTER2_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        COUNTER2_SetInterruptMode(COUNTER2_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (COUNTER2__PWM_PR == COUNTER2_PWM_MODE)
            COUNTER2_TRIG_CONTROL2_REG =
                    (COUNTER2_CC_MATCH_NO_CHANGE    |
                    COUNTER2_OVERLOW_NO_CHANGE      |
                    COUNTER2_UNDERFLOW_NO_CHANGE);
        #else
            #if (COUNTER2__LEFT == COUNTER2_PWM_ALIGN)
                COUNTER2_TRIG_CONTROL2_REG = COUNTER2_PWM_MODE_LEFT;
            #endif  /* ( COUNTER2_PWM_LEFT == COUNTER2_PWM_ALIGN) */

            #if (COUNTER2__RIGHT == COUNTER2_PWM_ALIGN)
                COUNTER2_TRIG_CONTROL2_REG = COUNTER2_PWM_MODE_RIGHT;
            #endif  /* ( COUNTER2_PWM_RIGHT == COUNTER2_PWM_ALIGN) */

            #if (COUNTER2__CENTER == COUNTER2_PWM_ALIGN)
                COUNTER2_TRIG_CONTROL2_REG = COUNTER2_PWM_MODE_CENTER;
            #endif  /* ( COUNTER2_PWM_CENTER == COUNTER2_PWM_ALIGN) */

            #if (COUNTER2__ASYMMETRIC == COUNTER2_PWM_ALIGN)
                COUNTER2_TRIG_CONTROL2_REG = COUNTER2_PWM_MODE_ASYM;
            #endif  /* (COUNTER2__ASYMMETRIC == COUNTER2_PWM_ALIGN) */
        #endif  /* (COUNTER2__PWM_PR == COUNTER2_PWM_MODE) */

        /* Set other values from customizer */
        COUNTER2_WritePeriod(COUNTER2_PWM_PERIOD_VALUE );
        COUNTER2_WriteCompare(COUNTER2_PWM_COMPARE_VALUE);

        #if (1u == COUNTER2_PWM_COMPARE_SWAP)
            COUNTER2_SetCompareSwap(1u);
            COUNTER2_WriteCompareBuf(COUNTER2_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == COUNTER2_PWM_COMPARE_SWAP) */

        #if (1u == COUNTER2_PWM_PERIOD_SWAP)
            COUNTER2_SetPeriodSwap(1u);
            COUNTER2_WritePeriodBuf(COUNTER2_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == COUNTER2_PWM_PERIOD_SWAP) */
    #endif  /* (COUNTER2__PWM_SEL == COUNTER2_CONFIG) */
    
}


/*******************************************************************************
* Function Name: COUNTER2_Enable
********************************************************************************
*
* Summary:
*  Enables the COUNTER2.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    COUNTER2_BLOCK_CONTROL_REG |= COUNTER2_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (COUNTER2__PWM_SEL == COUNTER2_CONFIG)
        #if (0u == COUNTER2_PWM_START_SIGNAL_PRESENT)
            COUNTER2_TriggerCommand(COUNTER2_MASK, COUNTER2_CMD_START);
        #endif /* (0u == COUNTER2_PWM_START_SIGNAL_PRESENT) */
    #endif /* (COUNTER2__PWM_SEL == COUNTER2_CONFIG) */

    #if (COUNTER2__TIMER == COUNTER2_CONFIG)
        #if (0u == COUNTER2_TC_START_SIGNAL_PRESENT)
            COUNTER2_TriggerCommand(COUNTER2_MASK, COUNTER2_CMD_START);
        #endif /* (0u == COUNTER2_TC_START_SIGNAL_PRESENT) */
    #endif /* (COUNTER2__TIMER == COUNTER2_CONFIG) */
    
    #if (COUNTER2__QUAD == COUNTER2_CONFIG)
        #if (0u != COUNTER2_QUAD_AUTO_START)
            COUNTER2_TriggerCommand(COUNTER2_MASK, COUNTER2_CMD_RELOAD);
        #endif /* (0u != COUNTER2_QUAD_AUTO_START) */
    #endif  /* (COUNTER2__QUAD == COUNTER2_CONFIG) */
}


/*******************************************************************************
* Function Name: COUNTER2_Start
********************************************************************************
*
* Summary:
*  Initializes the COUNTER2 with default customizer
*  values when called the first time and enables the COUNTER2.
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
*  COUNTER2_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time COUNTER2_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the COUNTER2_Start() routine.
*
*******************************************************************************/
void COUNTER2_Start(void)
{
    if (0u == COUNTER2_initVar)
    {
        COUNTER2_Init();
        COUNTER2_initVar = 1u;
    }

    COUNTER2_Enable();
}


/*******************************************************************************
* Function Name: COUNTER2_Stop
********************************************************************************
*
* Summary:
*  Disables the COUNTER2.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_BLOCK_CONTROL_REG &= (uint32)~COUNTER2_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the COUNTER2. This function is used when
*  configured as a generic COUNTER2 and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the COUNTER2 to operate in
*   Values:
*   - COUNTER2_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - COUNTER2_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - COUNTER2_MODE_QUAD - Quadrature decoder
*         - COUNTER2_MODE_PWM - PWM
*         - COUNTER2_MODE_PWM_DT - PWM with dead time
*         - COUNTER2_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_CONTROL_REG &= (uint32)~COUNTER2_MODE_MASK;
    COUNTER2_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - COUNTER2_MODE_X1 - Counts on phi 1 rising
*         - COUNTER2_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - COUNTER2_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_CONTROL_REG &= (uint32)~COUNTER2_QUAD_MODE_MASK;
    COUNTER2_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - COUNTER2_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - COUNTER2_PRESCALE_DIVBY2    - Divide by 2
*         - COUNTER2_PRESCALE_DIVBY4    - Divide by 4
*         - COUNTER2_PRESCALE_DIVBY8    - Divide by 8
*         - COUNTER2_PRESCALE_DIVBY16   - Divide by 16
*         - COUNTER2_PRESCALE_DIVBY32   - Divide by 32
*         - COUNTER2_PRESCALE_DIVBY64   - Divide by 64
*         - COUNTER2_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_CONTROL_REG &= (uint32)~COUNTER2_PRESCALER_MASK;
    COUNTER2_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the COUNTER2 runs
*  continuously or stops when terminal count is reached.  By default the
*  COUNTER2 operates in the continuous mode.
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
void COUNTER2_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_CONTROL_REG &= (uint32)~COUNTER2_ONESHOT_MASK;
    COUNTER2_CONTROL_REG |= ((uint32)((oneShotEnable & COUNTER2_1BIT_MASK) <<
                                                               COUNTER2_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_SetPWMMode
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
void COUNTER2_SetPWMMode(uint32 modeMask)
{
    COUNTER2_TRIG_CONTROL2_REG = (modeMask & COUNTER2_6BIT_MASK);
}



/*******************************************************************************
* Function Name: COUNTER2_SetPWMSyncKill
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
void COUNTER2_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_CONTROL_REG &= (uint32)~COUNTER2_PWM_SYNC_KILL_MASK;
    COUNTER2_CONTROL_REG |= ((uint32)((syncKillEnable & COUNTER2_1BIT_MASK)  <<
                                               COUNTER2_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_SetPWMStopOnKill
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
void COUNTER2_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_CONTROL_REG &= (uint32)~COUNTER2_PWM_STOP_KILL_MASK;
    COUNTER2_CONTROL_REG |= ((uint32)((stopOnKillEnable & COUNTER2_1BIT_MASK)  <<
                                                         COUNTER2_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_SetPWMDeadTime
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
void COUNTER2_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_CONTROL_REG &= (uint32)~COUNTER2_PRESCALER_MASK;
    COUNTER2_CONTROL_REG |= ((uint32)((deadTime & COUNTER2_8BIT_MASK) <<
                                                          COUNTER2_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_SetPWMInvert
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
*         - COUNTER2_INVERT_LINE   - Inverts the line output
*         - COUNTER2_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_CONTROL_REG &= (uint32)~COUNTER2_INV_OUT_MASK;
    COUNTER2_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: COUNTER2_WriteCounter
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
void COUNTER2_WriteCounter(uint32 count)
{
    COUNTER2_COUNTER_REG = (count & COUNTER2_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTER2_ReadCounter
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
uint32 COUNTER2_ReadCounter(void)
{
    return (COUNTER2_COUNTER_REG & COUNTER2_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTER2_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - COUNTER2_COUNT_UP       - Counts up
*     - COUNTER2_COUNT_DOWN     - Counts down
*     - COUNTER2_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - COUNTER2_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_CONTROL_REG &= (uint32)~COUNTER2_UPDOWN_MASK;
    COUNTER2_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_WritePeriod
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
void COUNTER2_WritePeriod(uint32 period)
{
    COUNTER2_PERIOD_REG = (period & COUNTER2_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTER2_ReadPeriod
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
uint32 COUNTER2_ReadPeriod(void)
{
    return (COUNTER2_PERIOD_REG & COUNTER2_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTER2_SetCompareSwap
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
void COUNTER2_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_CONTROL_REG &= (uint32)~COUNTER2_RELOAD_CC_MASK;
    COUNTER2_CONTROL_REG |= (swapEnable & COUNTER2_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_WritePeriodBuf
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
void COUNTER2_WritePeriodBuf(uint32 periodBuf)
{
    COUNTER2_PERIOD_BUF_REG = (periodBuf & COUNTER2_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTER2_ReadPeriodBuf
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
uint32 COUNTER2_ReadPeriodBuf(void)
{
    return (COUNTER2_PERIOD_BUF_REG & COUNTER2_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTER2_SetPeriodSwap
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
void COUNTER2_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_CONTROL_REG &= (uint32)~COUNTER2_RELOAD_PERIOD_MASK;
    COUNTER2_CONTROL_REG |= ((uint32)((swapEnable & COUNTER2_1BIT_MASK) <<
                                                            COUNTER2_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_WriteCompare
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
void COUNTER2_WriteCompare(uint32 compare)
{
    #if (COUNTER2_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (COUNTER2_CY_TCPWM_4000) */

    #if (COUNTER2_CY_TCPWM_4000)
        currentMode = ((COUNTER2_CONTROL_REG & COUNTER2_UPDOWN_MASK) >> COUNTER2_UPDOWN_SHIFT);

        if (((uint32)COUNTER2__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)COUNTER2__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (COUNTER2_CY_TCPWM_4000) */
    
    COUNTER2_COMP_CAP_REG = (compare & COUNTER2_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTER2_ReadCompare
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
uint32 COUNTER2_ReadCompare(void)
{
    #if (COUNTER2_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (COUNTER2_CY_TCPWM_4000) */

    #if (COUNTER2_CY_TCPWM_4000)
        currentMode = ((COUNTER2_CONTROL_REG & COUNTER2_UPDOWN_MASK) >> COUNTER2_UPDOWN_SHIFT);
        
        regVal = COUNTER2_COMP_CAP_REG;
        
        if (((uint32)COUNTER2__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)COUNTER2__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & COUNTER2_16BIT_MASK);
    #else
        return (COUNTER2_COMP_CAP_REG & COUNTER2_16BIT_MASK);
    #endif /* (COUNTER2_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: COUNTER2_WriteCompareBuf
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
void COUNTER2_WriteCompareBuf(uint32 compareBuf)
{
    #if (COUNTER2_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (COUNTER2_CY_TCPWM_4000) */

    #if (COUNTER2_CY_TCPWM_4000)
        currentMode = ((COUNTER2_CONTROL_REG & COUNTER2_UPDOWN_MASK) >> COUNTER2_UPDOWN_SHIFT);

        if (((uint32)COUNTER2__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)COUNTER2__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (COUNTER2_CY_TCPWM_4000) */
    
    COUNTER2_COMP_CAP_BUF_REG = (compareBuf & COUNTER2_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTER2_ReadCompareBuf
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
uint32 COUNTER2_ReadCompareBuf(void)
{
    #if (COUNTER2_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (COUNTER2_CY_TCPWM_4000) */

    #if (COUNTER2_CY_TCPWM_4000)
        currentMode = ((COUNTER2_CONTROL_REG & COUNTER2_UPDOWN_MASK) >> COUNTER2_UPDOWN_SHIFT);

        regVal = COUNTER2_COMP_CAP_BUF_REG;
        
        if (((uint32)COUNTER2__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)COUNTER2__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & COUNTER2_16BIT_MASK);
    #else
        return (COUNTER2_COMP_CAP_BUF_REG & COUNTER2_16BIT_MASK);
    #endif /* (COUNTER2_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: COUNTER2_ReadCapture
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
uint32 COUNTER2_ReadCapture(void)
{
    return (COUNTER2_COMP_CAP_REG & COUNTER2_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTER2_ReadCaptureBuf
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
uint32 COUNTER2_ReadCaptureBuf(void)
{
    return (COUNTER2_COMP_CAP_BUF_REG & COUNTER2_16BIT_MASK);
}


/*******************************************************************************
* Function Name: COUNTER2_SetCaptureMode
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
*     - COUNTER2_TRIG_LEVEL     - Level
*     - COUNTER2_TRIG_RISING    - Rising edge
*     - COUNTER2_TRIG_FALLING   - Falling edge
*     - COUNTER2_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_TRIG_CONTROL1_REG &= (uint32)~COUNTER2_CAPTURE_MASK;
    COUNTER2_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - COUNTER2_TRIG_LEVEL     - Level
*     - COUNTER2_TRIG_RISING    - Rising edge
*     - COUNTER2_TRIG_FALLING   - Falling edge
*     - COUNTER2_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_TRIG_CONTROL1_REG &= (uint32)~COUNTER2_RELOAD_MASK;
    COUNTER2_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << COUNTER2_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - COUNTER2_TRIG_LEVEL     - Level
*     - COUNTER2_TRIG_RISING    - Rising edge
*     - COUNTER2_TRIG_FALLING   - Falling edge
*     - COUNTER2_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_TRIG_CONTROL1_REG &= (uint32)~COUNTER2_START_MASK;
    COUNTER2_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << COUNTER2_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - COUNTER2_TRIG_LEVEL     - Level
*     - COUNTER2_TRIG_RISING    - Rising edge
*     - COUNTER2_TRIG_FALLING   - Falling edge
*     - COUNTER2_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_TRIG_CONTROL1_REG &= (uint32)~COUNTER2_STOP_MASK;
    COUNTER2_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << COUNTER2_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - COUNTER2_TRIG_LEVEL     - Level
*     - COUNTER2_TRIG_RISING    - Rising edge
*     - COUNTER2_TRIG_FALLING   - Falling edge
*     - COUNTER2_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_TRIG_CONTROL1_REG &= (uint32)~COUNTER2_COUNT_MASK;
    COUNTER2_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << COUNTER2_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_TriggerCommand
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
*     - COUNTER2_CMD_CAPTURE    - Trigger Capture/Switch command
*     - COUNTER2_CMD_RELOAD     - Trigger Reload/Index command
*     - COUNTER2_CMD_STOP       - Trigger Stop/Kill command
*     - COUNTER2_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    COUNTER2_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: COUNTER2_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the COUNTER2.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - COUNTER2_STATUS_DOWN    - Set if counting down
*     - COUNTER2_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 COUNTER2_ReadStatus(void)
{
    return ((COUNTER2_STATUS_REG >> COUNTER2_RUNNING_STATUS_SHIFT) |
            (COUNTER2_STATUS_REG & COUNTER2_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: COUNTER2_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - COUNTER2_INTR_MASK_TC       - Terminal count mask
*     - COUNTER2_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_SetInterruptMode(uint32 interruptMask)
{
    COUNTER2_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: COUNTER2_GetInterruptSourceMasked
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
*     - COUNTER2_INTR_MASK_TC       - Terminal count mask
*     - COUNTER2_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 COUNTER2_GetInterruptSourceMasked(void)
{
    return (COUNTER2_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: COUNTER2_GetInterruptSource
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
*     - COUNTER2_INTR_MASK_TC       - Terminal count mask
*     - COUNTER2_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 COUNTER2_GetInterruptSource(void)
{
    return (COUNTER2_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: COUNTER2_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - COUNTER2_INTR_MASK_TC       - Terminal count mask
*     - COUNTER2_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_ClearInterrupt(uint32 interruptMask)
{
    COUNTER2_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: COUNTER2_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - COUNTER2_INTR_MASK_TC       - Terminal count mask
*     - COUNTER2_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void COUNTER2_SetInterrupt(uint32 interruptMask)
{
    COUNTER2_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
