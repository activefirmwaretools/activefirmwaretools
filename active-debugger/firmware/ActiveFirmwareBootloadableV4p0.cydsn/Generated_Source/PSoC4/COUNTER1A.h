/*******************************************************************************
* File Name: COUNTER1A.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the COUNTER1A
*  component.
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

#if !defined(CY_TCPWM_COUNTER1A_H)
#define CY_TCPWM_COUNTER1A_H


#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/*******************************************************************************
* Internal Type defines
*******************************************************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} COUNTER1A_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  COUNTER1A_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define COUNTER1A_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define COUNTER1A_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define COUNTER1A_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define COUNTER1A_QUAD_ENCODING_MODES            (0lu)
#define COUNTER1A_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define COUNTER1A_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define COUNTER1A_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define COUNTER1A_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define COUNTER1A_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define COUNTER1A_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define COUNTER1A_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define COUNTER1A_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define COUNTER1A_TC_RUN_MODE                    (0lu)
#define COUNTER1A_TC_COUNTER_MODE                (0lu)
#define COUNTER1A_TC_COMP_CAP_MODE               (2lu)
#define COUNTER1A_TC_PRESCALER                   (0lu)

/* Signal modes */
#define COUNTER1A_TC_RELOAD_SIGNAL_MODE          (3lu)
#define COUNTER1A_TC_COUNT_SIGNAL_MODE           (0lu)
#define COUNTER1A_TC_START_SIGNAL_MODE           (0lu)
#define COUNTER1A_TC_STOP_SIGNAL_MODE            (0lu)
#define COUNTER1A_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define COUNTER1A_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define COUNTER1A_TC_COUNT_SIGNAL_PRESENT        (1lu)
#define COUNTER1A_TC_START_SIGNAL_PRESENT        (0lu)
#define COUNTER1A_TC_STOP_SIGNAL_PRESENT         (0lu)
#define COUNTER1A_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define COUNTER1A_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define COUNTER1A_PWM_KILL_EVENT                 (0lu)
#define COUNTER1A_PWM_STOP_EVENT                 (0lu)
#define COUNTER1A_PWM_MODE                       (4lu)
#define COUNTER1A_PWM_OUT_N_INVERT               (0lu)
#define COUNTER1A_PWM_OUT_INVERT                 (0lu)
#define COUNTER1A_PWM_ALIGN                      (0lu)
#define COUNTER1A_PWM_RUN_MODE                   (0lu)
#define COUNTER1A_PWM_DEAD_TIME_CYCLE            (0lu)
#define COUNTER1A_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define COUNTER1A_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define COUNTER1A_PWM_COUNT_SIGNAL_MODE          (3lu)
#define COUNTER1A_PWM_START_SIGNAL_MODE          (0lu)
#define COUNTER1A_PWM_STOP_SIGNAL_MODE           (0lu)
#define COUNTER1A_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define COUNTER1A_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define COUNTER1A_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define COUNTER1A_PWM_START_SIGNAL_PRESENT       (0lu)
#define COUNTER1A_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define COUNTER1A_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define COUNTER1A_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define COUNTER1A_TC_PERIOD_VALUE                (48lu)
#define COUNTER1A_TC_COMPARE_VALUE               (16lu)
#define COUNTER1A_TC_COMPARE_BUF_VALUE           (65535lu)
#define COUNTER1A_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define COUNTER1A_PWM_PERIOD_VALUE               (65535lu)
#define COUNTER1A_PWM_PERIOD_BUF_VALUE           (65535lu)
#define COUNTER1A_PWM_PERIOD_SWAP                (0lu)
#define COUNTER1A_PWM_COMPARE_VALUE              (65535lu)
#define COUNTER1A_PWM_COMPARE_BUF_VALUE          (65535lu)
#define COUNTER1A_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define COUNTER1A__LEFT 0
#define COUNTER1A__RIGHT 1
#define COUNTER1A__CENTER 2
#define COUNTER1A__ASYMMETRIC 3

#define COUNTER1A__X1 0
#define COUNTER1A__X2 1
#define COUNTER1A__X4 2

#define COUNTER1A__PWM 4
#define COUNTER1A__PWM_DT 5
#define COUNTER1A__PWM_PR 6

#define COUNTER1A__INVERSE 1
#define COUNTER1A__DIRECT 0

#define COUNTER1A__CAPTURE 2
#define COUNTER1A__COMPARE 0

#define COUNTER1A__TRIG_LEVEL 3
#define COUNTER1A__TRIG_RISING 0
#define COUNTER1A__TRIG_FALLING 1
#define COUNTER1A__TRIG_BOTH 2

#define COUNTER1A__INTR_MASK_TC 1
#define COUNTER1A__INTR_MASK_CC_MATCH 2
#define COUNTER1A__INTR_MASK_NONE 0
#define COUNTER1A__INTR_MASK_TC_CC 3

#define COUNTER1A__UNCONFIG 8
#define COUNTER1A__TIMER 1
#define COUNTER1A__QUAD 3
#define COUNTER1A__PWM_SEL 7

#define COUNTER1A__COUNT_UP 0
#define COUNTER1A__COUNT_DOWN 1
#define COUNTER1A__COUNT_UPDOWN0 2
#define COUNTER1A__COUNT_UPDOWN1 3


/* Prescaler */
#define COUNTER1A_PRESCALE_DIVBY1                ((uint32)(0u << COUNTER1A_PRESCALER_SHIFT))
#define COUNTER1A_PRESCALE_DIVBY2                ((uint32)(1u << COUNTER1A_PRESCALER_SHIFT))
#define COUNTER1A_PRESCALE_DIVBY4                ((uint32)(2u << COUNTER1A_PRESCALER_SHIFT))
#define COUNTER1A_PRESCALE_DIVBY8                ((uint32)(3u << COUNTER1A_PRESCALER_SHIFT))
#define COUNTER1A_PRESCALE_DIVBY16               ((uint32)(4u << COUNTER1A_PRESCALER_SHIFT))
#define COUNTER1A_PRESCALE_DIVBY32               ((uint32)(5u << COUNTER1A_PRESCALER_SHIFT))
#define COUNTER1A_PRESCALE_DIVBY64               ((uint32)(6u << COUNTER1A_PRESCALER_SHIFT))
#define COUNTER1A_PRESCALE_DIVBY128              ((uint32)(7u << COUNTER1A_PRESCALER_SHIFT))

/* TCPWM set modes */
#define COUNTER1A_MODE_TIMER_COMPARE             ((uint32)(COUNTER1A__COMPARE         <<  \
                                                                  COUNTER1A_MODE_SHIFT))
#define COUNTER1A_MODE_TIMER_CAPTURE             ((uint32)(COUNTER1A__CAPTURE         <<  \
                                                                  COUNTER1A_MODE_SHIFT))
#define COUNTER1A_MODE_QUAD                      ((uint32)(COUNTER1A__QUAD            <<  \
                                                                  COUNTER1A_MODE_SHIFT))
#define COUNTER1A_MODE_PWM                       ((uint32)(COUNTER1A__PWM             <<  \
                                                                  COUNTER1A_MODE_SHIFT))
#define COUNTER1A_MODE_PWM_DT                    ((uint32)(COUNTER1A__PWM_DT          <<  \
                                                                  COUNTER1A_MODE_SHIFT))
#define COUNTER1A_MODE_PWM_PR                    ((uint32)(COUNTER1A__PWM_PR          <<  \
                                                                  COUNTER1A_MODE_SHIFT))

/* Quad Modes */
#define COUNTER1A_MODE_X1                        ((uint32)(COUNTER1A__X1              <<  \
                                                                  COUNTER1A_QUAD_MODE_SHIFT))
#define COUNTER1A_MODE_X2                        ((uint32)(COUNTER1A__X2              <<  \
                                                                  COUNTER1A_QUAD_MODE_SHIFT))
#define COUNTER1A_MODE_X4                        ((uint32)(COUNTER1A__X4              <<  \
                                                                  COUNTER1A_QUAD_MODE_SHIFT))

/* Counter modes */
#define COUNTER1A_COUNT_UP                       ((uint32)(COUNTER1A__COUNT_UP        <<  \
                                                                  COUNTER1A_UPDOWN_SHIFT))
#define COUNTER1A_COUNT_DOWN                     ((uint32)(COUNTER1A__COUNT_DOWN      <<  \
                                                                  COUNTER1A_UPDOWN_SHIFT))
#define COUNTER1A_COUNT_UPDOWN0                  ((uint32)(COUNTER1A__COUNT_UPDOWN0   <<  \
                                                                  COUNTER1A_UPDOWN_SHIFT))
#define COUNTER1A_COUNT_UPDOWN1                  ((uint32)(COUNTER1A__COUNT_UPDOWN1   <<  \
                                                                  COUNTER1A_UPDOWN_SHIFT))

/* PWM output invert */
#define COUNTER1A_INVERT_LINE                    ((uint32)(COUNTER1A__INVERSE         <<  \
                                                                  COUNTER1A_INV_OUT_SHIFT))
#define COUNTER1A_INVERT_LINE_N                  ((uint32)(COUNTER1A__INVERSE         <<  \
                                                                  COUNTER1A_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define COUNTER1A_TRIG_RISING                    ((uint32)COUNTER1A__TRIG_RISING)
#define COUNTER1A_TRIG_FALLING                   ((uint32)COUNTER1A__TRIG_FALLING)
#define COUNTER1A_TRIG_BOTH                      ((uint32)COUNTER1A__TRIG_BOTH)
#define COUNTER1A_TRIG_LEVEL                     ((uint32)COUNTER1A__TRIG_LEVEL)

/* Interrupt mask */
#define COUNTER1A_INTR_MASK_TC                   ((uint32)COUNTER1A__INTR_MASK_TC)
#define COUNTER1A_INTR_MASK_CC_MATCH             ((uint32)COUNTER1A__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define COUNTER1A_CC_MATCH_SET                   (0x00u)
#define COUNTER1A_CC_MATCH_CLEAR                 (0x01u)
#define COUNTER1A_CC_MATCH_INVERT                (0x02u)
#define COUNTER1A_CC_MATCH_NO_CHANGE             (0x03u)
#define COUNTER1A_OVERLOW_SET                    (0x00u)
#define COUNTER1A_OVERLOW_CLEAR                  (0x04u)
#define COUNTER1A_OVERLOW_INVERT                 (0x08u)
#define COUNTER1A_OVERLOW_NO_CHANGE              (0x0Cu)
#define COUNTER1A_UNDERFLOW_SET                  (0x00u)
#define COUNTER1A_UNDERFLOW_CLEAR                (0x10u)
#define COUNTER1A_UNDERFLOW_INVERT               (0x20u)
#define COUNTER1A_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define COUNTER1A_PWM_MODE_LEFT                  (COUNTER1A_CC_MATCH_CLEAR        |   \
                                                         COUNTER1A_OVERLOW_SET           |   \
                                                         COUNTER1A_UNDERFLOW_NO_CHANGE)
#define COUNTER1A_PWM_MODE_RIGHT                 (COUNTER1A_CC_MATCH_SET          |   \
                                                         COUNTER1A_OVERLOW_NO_CHANGE     |   \
                                                         COUNTER1A_UNDERFLOW_CLEAR)
#define COUNTER1A_PWM_MODE_ASYM                  (COUNTER1A_CC_MATCH_INVERT       |   \
                                                         COUNTER1A_OVERLOW_SET           |   \
                                                         COUNTER1A_UNDERFLOW_CLEAR)

#if (COUNTER1A_CY_TCPWM_V2)
    #if(COUNTER1A_CY_TCPWM_4000)
        #define COUNTER1A_PWM_MODE_CENTER                (COUNTER1A_CC_MATCH_INVERT       |   \
                                                                 COUNTER1A_OVERLOW_NO_CHANGE     |   \
                                                                 COUNTER1A_UNDERFLOW_CLEAR)
    #else
        #define COUNTER1A_PWM_MODE_CENTER                (COUNTER1A_CC_MATCH_INVERT       |   \
                                                                 COUNTER1A_OVERLOW_SET           |   \
                                                                 COUNTER1A_UNDERFLOW_CLEAR)
    #endif /* (COUNTER1A_CY_TCPWM_4000) */
#else
    #define COUNTER1A_PWM_MODE_CENTER                (COUNTER1A_CC_MATCH_INVERT       |   \
                                                             COUNTER1A_OVERLOW_NO_CHANGE     |   \
                                                             COUNTER1A_UNDERFLOW_CLEAR)
#endif /* (COUNTER1A_CY_TCPWM_NEW) */

/* Command operations without condition */
#define COUNTER1A_CMD_CAPTURE                    (0u)
#define COUNTER1A_CMD_RELOAD                     (8u)
#define COUNTER1A_CMD_STOP                       (16u)
#define COUNTER1A_CMD_START                      (24u)

/* Status */
#define COUNTER1A_STATUS_DOWN                    (1u)
#define COUNTER1A_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   COUNTER1A_Init(void);
void   COUNTER1A_Enable(void);
void   COUNTER1A_Start(void);
void   COUNTER1A_Stop(void);

void   COUNTER1A_SetMode(uint32 mode);
void   COUNTER1A_SetCounterMode(uint32 counterMode);
void   COUNTER1A_SetPWMMode(uint32 modeMask);
void   COUNTER1A_SetQDMode(uint32 qdMode);

void   COUNTER1A_SetPrescaler(uint32 prescaler);
void   COUNTER1A_TriggerCommand(uint32 mask, uint32 command);
void   COUNTER1A_SetOneShot(uint32 oneShotEnable);
uint32 COUNTER1A_ReadStatus(void);

void   COUNTER1A_SetPWMSyncKill(uint32 syncKillEnable);
void   COUNTER1A_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   COUNTER1A_SetPWMDeadTime(uint32 deadTime);
void   COUNTER1A_SetPWMInvert(uint32 mask);

void   COUNTER1A_SetInterruptMode(uint32 interruptMask);
uint32 COUNTER1A_GetInterruptSourceMasked(void);
uint32 COUNTER1A_GetInterruptSource(void);
void   COUNTER1A_ClearInterrupt(uint32 interruptMask);
void   COUNTER1A_SetInterrupt(uint32 interruptMask);

void   COUNTER1A_WriteCounter(uint32 count);
uint32 COUNTER1A_ReadCounter(void);

uint32 COUNTER1A_ReadCapture(void);
uint32 COUNTER1A_ReadCaptureBuf(void);

void   COUNTER1A_WritePeriod(uint32 period);
uint32 COUNTER1A_ReadPeriod(void);
void   COUNTER1A_WritePeriodBuf(uint32 periodBuf);
uint32 COUNTER1A_ReadPeriodBuf(void);

void   COUNTER1A_WriteCompare(uint32 compare);
uint32 COUNTER1A_ReadCompare(void);
void   COUNTER1A_WriteCompareBuf(uint32 compareBuf);
uint32 COUNTER1A_ReadCompareBuf(void);

void   COUNTER1A_SetPeriodSwap(uint32 swapEnable);
void   COUNTER1A_SetCompareSwap(uint32 swapEnable);

void   COUNTER1A_SetCaptureMode(uint32 triggerMode);
void   COUNTER1A_SetReloadMode(uint32 triggerMode);
void   COUNTER1A_SetStartMode(uint32 triggerMode);
void   COUNTER1A_SetStopMode(uint32 triggerMode);
void   COUNTER1A_SetCountMode(uint32 triggerMode);

void   COUNTER1A_SaveConfig(void);
void   COUNTER1A_RestoreConfig(void);
void   COUNTER1A_Sleep(void);
void   COUNTER1A_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define COUNTER1A_BLOCK_CONTROL_REG              (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define COUNTER1A_BLOCK_CONTROL_PTR              ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define COUNTER1A_COMMAND_REG                    (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define COUNTER1A_COMMAND_PTR                    ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define COUNTER1A_INTRRUPT_CAUSE_REG             (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define COUNTER1A_INTRRUPT_CAUSE_PTR             ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define COUNTER1A_CONTROL_REG                    (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__CTRL )
#define COUNTER1A_CONTROL_PTR                    ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__CTRL )
#define COUNTER1A_STATUS_REG                     (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__STATUS )
#define COUNTER1A_STATUS_PTR                     ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__STATUS )
#define COUNTER1A_COUNTER_REG                    (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__COUNTER )
#define COUNTER1A_COUNTER_PTR                    ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__COUNTER )
#define COUNTER1A_COMP_CAP_REG                   (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__CC )
#define COUNTER1A_COMP_CAP_PTR                   ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__CC )
#define COUNTER1A_COMP_CAP_BUF_REG               (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__CC_BUFF )
#define COUNTER1A_COMP_CAP_BUF_PTR               ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__CC_BUFF )
#define COUNTER1A_PERIOD_REG                     (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__PERIOD )
#define COUNTER1A_PERIOD_PTR                     ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__PERIOD )
#define COUNTER1A_PERIOD_BUF_REG                 (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define COUNTER1A_PERIOD_BUF_PTR                 ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define COUNTER1A_TRIG_CONTROL0_REG              (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define COUNTER1A_TRIG_CONTROL0_PTR              ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define COUNTER1A_TRIG_CONTROL1_REG              (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define COUNTER1A_TRIG_CONTROL1_PTR              ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define COUNTER1A_TRIG_CONTROL2_REG              (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define COUNTER1A_TRIG_CONTROL2_PTR              ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define COUNTER1A_INTERRUPT_REQ_REG              (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__INTR )
#define COUNTER1A_INTERRUPT_REQ_PTR              ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__INTR )
#define COUNTER1A_INTERRUPT_SET_REG              (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__INTR_SET )
#define COUNTER1A_INTERRUPT_SET_PTR              ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__INTR_SET )
#define COUNTER1A_INTERRUPT_MASK_REG             (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__INTR_MASK )
#define COUNTER1A_INTERRUPT_MASK_PTR             ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__INTR_MASK )
#define COUNTER1A_INTERRUPT_MASKED_REG           (*(reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__INTR_MASKED )
#define COUNTER1A_INTERRUPT_MASKED_PTR           ( (reg32 *) COUNTER1A_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define COUNTER1A_MASK                           ((uint32)COUNTER1A_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define COUNTER1A_RELOAD_CC_SHIFT                (0u)
#define COUNTER1A_RELOAD_PERIOD_SHIFT            (1u)
#define COUNTER1A_PWM_SYNC_KILL_SHIFT            (2u)
#define COUNTER1A_PWM_STOP_KILL_SHIFT            (3u)
#define COUNTER1A_PRESCALER_SHIFT                (8u)
#define COUNTER1A_UPDOWN_SHIFT                   (16u)
#define COUNTER1A_ONESHOT_SHIFT                  (18u)
#define COUNTER1A_QUAD_MODE_SHIFT                (20u)
#define COUNTER1A_INV_OUT_SHIFT                  (20u)
#define COUNTER1A_INV_COMPL_OUT_SHIFT            (21u)
#define COUNTER1A_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define COUNTER1A_RELOAD_CC_MASK                 ((uint32)(COUNTER1A_1BIT_MASK        <<  \
                                                                            COUNTER1A_RELOAD_CC_SHIFT))
#define COUNTER1A_RELOAD_PERIOD_MASK             ((uint32)(COUNTER1A_1BIT_MASK        <<  \
                                                                            COUNTER1A_RELOAD_PERIOD_SHIFT))
#define COUNTER1A_PWM_SYNC_KILL_MASK             ((uint32)(COUNTER1A_1BIT_MASK        <<  \
                                                                            COUNTER1A_PWM_SYNC_KILL_SHIFT))
#define COUNTER1A_PWM_STOP_KILL_MASK             ((uint32)(COUNTER1A_1BIT_MASK        <<  \
                                                                            COUNTER1A_PWM_STOP_KILL_SHIFT))
#define COUNTER1A_PRESCALER_MASK                 ((uint32)(COUNTER1A_8BIT_MASK        <<  \
                                                                            COUNTER1A_PRESCALER_SHIFT))
#define COUNTER1A_UPDOWN_MASK                    ((uint32)(COUNTER1A_2BIT_MASK        <<  \
                                                                            COUNTER1A_UPDOWN_SHIFT))
#define COUNTER1A_ONESHOT_MASK                   ((uint32)(COUNTER1A_1BIT_MASK        <<  \
                                                                            COUNTER1A_ONESHOT_SHIFT))
#define COUNTER1A_QUAD_MODE_MASK                 ((uint32)(COUNTER1A_3BIT_MASK        <<  \
                                                                            COUNTER1A_QUAD_MODE_SHIFT))
#define COUNTER1A_INV_OUT_MASK                   ((uint32)(COUNTER1A_2BIT_MASK        <<  \
                                                                            COUNTER1A_INV_OUT_SHIFT))
#define COUNTER1A_MODE_MASK                      ((uint32)(COUNTER1A_3BIT_MASK        <<  \
                                                                            COUNTER1A_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define COUNTER1A_CAPTURE_SHIFT                  (0u)
#define COUNTER1A_COUNT_SHIFT                    (2u)
#define COUNTER1A_RELOAD_SHIFT                   (4u)
#define COUNTER1A_STOP_SHIFT                     (6u)
#define COUNTER1A_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define COUNTER1A_CAPTURE_MASK                   ((uint32)(COUNTER1A_2BIT_MASK        <<  \
                                                                  COUNTER1A_CAPTURE_SHIFT))
#define COUNTER1A_COUNT_MASK                     ((uint32)(COUNTER1A_2BIT_MASK        <<  \
                                                                  COUNTER1A_COUNT_SHIFT))
#define COUNTER1A_RELOAD_MASK                    ((uint32)(COUNTER1A_2BIT_MASK        <<  \
                                                                  COUNTER1A_RELOAD_SHIFT))
#define COUNTER1A_STOP_MASK                      ((uint32)(COUNTER1A_2BIT_MASK        <<  \
                                                                  COUNTER1A_STOP_SHIFT))
#define COUNTER1A_START_MASK                     ((uint32)(COUNTER1A_2BIT_MASK        <<  \
                                                                  COUNTER1A_START_SHIFT))

/* MASK */
#define COUNTER1A_1BIT_MASK                      ((uint32)0x01u)
#define COUNTER1A_2BIT_MASK                      ((uint32)0x03u)
#define COUNTER1A_3BIT_MASK                      ((uint32)0x07u)
#define COUNTER1A_6BIT_MASK                      ((uint32)0x3Fu)
#define COUNTER1A_8BIT_MASK                      ((uint32)0xFFu)
#define COUNTER1A_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define COUNTER1A_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define COUNTER1A_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(COUNTER1A_QUAD_ENCODING_MODES     << COUNTER1A_QUAD_MODE_SHIFT))       |\
         ((uint32)(COUNTER1A_CONFIG                  << COUNTER1A_MODE_SHIFT)))

#define COUNTER1A_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(COUNTER1A_PWM_STOP_EVENT          << COUNTER1A_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(COUNTER1A_PWM_OUT_INVERT          << COUNTER1A_INV_OUT_SHIFT))         |\
         ((uint32)(COUNTER1A_PWM_OUT_N_INVERT        << COUNTER1A_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(COUNTER1A_PWM_MODE                << COUNTER1A_MODE_SHIFT)))

#define COUNTER1A_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(COUNTER1A_PWM_RUN_MODE         << COUNTER1A_ONESHOT_SHIFT))
            
#define COUNTER1A_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(COUNTER1A_PWM_ALIGN            << COUNTER1A_UPDOWN_SHIFT))

#define COUNTER1A_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(COUNTER1A_PWM_KILL_EVENT      << COUNTER1A_PWM_SYNC_KILL_SHIFT))

#define COUNTER1A_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(COUNTER1A_PWM_DEAD_TIME_CYCLE  << COUNTER1A_PRESCALER_SHIFT))

#define COUNTER1A_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(COUNTER1A_PWM_PRESCALER        << COUNTER1A_PRESCALER_SHIFT))

#define COUNTER1A_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(COUNTER1A_TC_PRESCALER            << COUNTER1A_PRESCALER_SHIFT))       |\
         ((uint32)(COUNTER1A_TC_COUNTER_MODE         << COUNTER1A_UPDOWN_SHIFT))          |\
         ((uint32)(COUNTER1A_TC_RUN_MODE             << COUNTER1A_ONESHOT_SHIFT))         |\
         ((uint32)(COUNTER1A_TC_COMP_CAP_MODE        << COUNTER1A_MODE_SHIFT)))
        
#define COUNTER1A_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(COUNTER1A_QUAD_PHIA_SIGNAL_MODE   << COUNTER1A_COUNT_SHIFT))           |\
         ((uint32)(COUNTER1A_QUAD_INDEX_SIGNAL_MODE  << COUNTER1A_RELOAD_SHIFT))          |\
         ((uint32)(COUNTER1A_QUAD_STOP_SIGNAL_MODE   << COUNTER1A_STOP_SHIFT))            |\
         ((uint32)(COUNTER1A_QUAD_PHIB_SIGNAL_MODE   << COUNTER1A_START_SHIFT)))

#define COUNTER1A_PWM_SIGNALS_MODES                                                              \
        (((uint32)(COUNTER1A_PWM_SWITCH_SIGNAL_MODE  << COUNTER1A_CAPTURE_SHIFT))         |\
         ((uint32)(COUNTER1A_PWM_COUNT_SIGNAL_MODE   << COUNTER1A_COUNT_SHIFT))           |\
         ((uint32)(COUNTER1A_PWM_RELOAD_SIGNAL_MODE  << COUNTER1A_RELOAD_SHIFT))          |\
         ((uint32)(COUNTER1A_PWM_STOP_SIGNAL_MODE    << COUNTER1A_STOP_SHIFT))            |\
         ((uint32)(COUNTER1A_PWM_START_SIGNAL_MODE   << COUNTER1A_START_SHIFT)))

#define COUNTER1A_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(COUNTER1A_TC_CAPTURE_SIGNAL_MODE  << COUNTER1A_CAPTURE_SHIFT))         |\
         ((uint32)(COUNTER1A_TC_COUNT_SIGNAL_MODE    << COUNTER1A_COUNT_SHIFT))           |\
         ((uint32)(COUNTER1A_TC_RELOAD_SIGNAL_MODE   << COUNTER1A_RELOAD_SHIFT))          |\
         ((uint32)(COUNTER1A_TC_STOP_SIGNAL_MODE     << COUNTER1A_STOP_SHIFT))            |\
         ((uint32)(COUNTER1A_TC_START_SIGNAL_MODE    << COUNTER1A_START_SHIFT)))
        
#define COUNTER1A_TIMER_UPDOWN_CNT_USED                                                          \
                ((COUNTER1A__COUNT_UPDOWN0 == COUNTER1A_TC_COUNTER_MODE)                  ||\
                 (COUNTER1A__COUNT_UPDOWN1 == COUNTER1A_TC_COUNTER_MODE))

#define COUNTER1A_PWM_UPDOWN_CNT_USED                                                            \
                ((COUNTER1A__CENTER == COUNTER1A_PWM_ALIGN)                               ||\
                 (COUNTER1A__ASYMMETRIC == COUNTER1A_PWM_ALIGN))               
        
#define COUNTER1A_PWM_PR_INIT_VALUE              (1u)
#define COUNTER1A_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_COUNTER1A_H */

/* [] END OF FILE */
