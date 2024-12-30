/*******************************************************************************
* File Name: COUNTER2A.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the COUNTER2A
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

#if !defined(CY_TCPWM_COUNTER2A_H)
#define CY_TCPWM_COUNTER2A_H


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
} COUNTER2A_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  COUNTER2A_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define COUNTER2A_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define COUNTER2A_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define COUNTER2A_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define COUNTER2A_QUAD_ENCODING_MODES            (0lu)
#define COUNTER2A_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define COUNTER2A_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define COUNTER2A_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define COUNTER2A_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define COUNTER2A_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define COUNTER2A_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define COUNTER2A_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define COUNTER2A_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define COUNTER2A_TC_RUN_MODE                    (0lu)
#define COUNTER2A_TC_COUNTER_MODE                (0lu)
#define COUNTER2A_TC_COMP_CAP_MODE               (2lu)
#define COUNTER2A_TC_PRESCALER                   (0lu)

/* Signal modes */
#define COUNTER2A_TC_RELOAD_SIGNAL_MODE          (3lu)
#define COUNTER2A_TC_COUNT_SIGNAL_MODE           (0lu)
#define COUNTER2A_TC_START_SIGNAL_MODE           (0lu)
#define COUNTER2A_TC_STOP_SIGNAL_MODE            (0lu)
#define COUNTER2A_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define COUNTER2A_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define COUNTER2A_TC_COUNT_SIGNAL_PRESENT        (1lu)
#define COUNTER2A_TC_START_SIGNAL_PRESENT        (0lu)
#define COUNTER2A_TC_STOP_SIGNAL_PRESENT         (0lu)
#define COUNTER2A_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define COUNTER2A_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define COUNTER2A_PWM_KILL_EVENT                 (0lu)
#define COUNTER2A_PWM_STOP_EVENT                 (0lu)
#define COUNTER2A_PWM_MODE                       (4lu)
#define COUNTER2A_PWM_OUT_N_INVERT               (0lu)
#define COUNTER2A_PWM_OUT_INVERT                 (0lu)
#define COUNTER2A_PWM_ALIGN                      (0lu)
#define COUNTER2A_PWM_RUN_MODE                   (0lu)
#define COUNTER2A_PWM_DEAD_TIME_CYCLE            (0lu)
#define COUNTER2A_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define COUNTER2A_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define COUNTER2A_PWM_COUNT_SIGNAL_MODE          (3lu)
#define COUNTER2A_PWM_START_SIGNAL_MODE          (0lu)
#define COUNTER2A_PWM_STOP_SIGNAL_MODE           (0lu)
#define COUNTER2A_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define COUNTER2A_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define COUNTER2A_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define COUNTER2A_PWM_START_SIGNAL_PRESENT       (0lu)
#define COUNTER2A_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define COUNTER2A_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define COUNTER2A_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define COUNTER2A_TC_PERIOD_VALUE                (48lu)
#define COUNTER2A_TC_COMPARE_VALUE               (16lu)
#define COUNTER2A_TC_COMPARE_BUF_VALUE           (65535lu)
#define COUNTER2A_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define COUNTER2A_PWM_PERIOD_VALUE               (65535lu)
#define COUNTER2A_PWM_PERIOD_BUF_VALUE           (65535lu)
#define COUNTER2A_PWM_PERIOD_SWAP                (0lu)
#define COUNTER2A_PWM_COMPARE_VALUE              (65535lu)
#define COUNTER2A_PWM_COMPARE_BUF_VALUE          (65535lu)
#define COUNTER2A_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define COUNTER2A__LEFT 0
#define COUNTER2A__RIGHT 1
#define COUNTER2A__CENTER 2
#define COUNTER2A__ASYMMETRIC 3

#define COUNTER2A__X1 0
#define COUNTER2A__X2 1
#define COUNTER2A__X4 2

#define COUNTER2A__PWM 4
#define COUNTER2A__PWM_DT 5
#define COUNTER2A__PWM_PR 6

#define COUNTER2A__INVERSE 1
#define COUNTER2A__DIRECT 0

#define COUNTER2A__CAPTURE 2
#define COUNTER2A__COMPARE 0

#define COUNTER2A__TRIG_LEVEL 3
#define COUNTER2A__TRIG_RISING 0
#define COUNTER2A__TRIG_FALLING 1
#define COUNTER2A__TRIG_BOTH 2

#define COUNTER2A__INTR_MASK_TC 1
#define COUNTER2A__INTR_MASK_CC_MATCH 2
#define COUNTER2A__INTR_MASK_NONE 0
#define COUNTER2A__INTR_MASK_TC_CC 3

#define COUNTER2A__UNCONFIG 8
#define COUNTER2A__TIMER 1
#define COUNTER2A__QUAD 3
#define COUNTER2A__PWM_SEL 7

#define COUNTER2A__COUNT_UP 0
#define COUNTER2A__COUNT_DOWN 1
#define COUNTER2A__COUNT_UPDOWN0 2
#define COUNTER2A__COUNT_UPDOWN1 3


/* Prescaler */
#define COUNTER2A_PRESCALE_DIVBY1                ((uint32)(0u << COUNTER2A_PRESCALER_SHIFT))
#define COUNTER2A_PRESCALE_DIVBY2                ((uint32)(1u << COUNTER2A_PRESCALER_SHIFT))
#define COUNTER2A_PRESCALE_DIVBY4                ((uint32)(2u << COUNTER2A_PRESCALER_SHIFT))
#define COUNTER2A_PRESCALE_DIVBY8                ((uint32)(3u << COUNTER2A_PRESCALER_SHIFT))
#define COUNTER2A_PRESCALE_DIVBY16               ((uint32)(4u << COUNTER2A_PRESCALER_SHIFT))
#define COUNTER2A_PRESCALE_DIVBY32               ((uint32)(5u << COUNTER2A_PRESCALER_SHIFT))
#define COUNTER2A_PRESCALE_DIVBY64               ((uint32)(6u << COUNTER2A_PRESCALER_SHIFT))
#define COUNTER2A_PRESCALE_DIVBY128              ((uint32)(7u << COUNTER2A_PRESCALER_SHIFT))

/* TCPWM set modes */
#define COUNTER2A_MODE_TIMER_COMPARE             ((uint32)(COUNTER2A__COMPARE         <<  \
                                                                  COUNTER2A_MODE_SHIFT))
#define COUNTER2A_MODE_TIMER_CAPTURE             ((uint32)(COUNTER2A__CAPTURE         <<  \
                                                                  COUNTER2A_MODE_SHIFT))
#define COUNTER2A_MODE_QUAD                      ((uint32)(COUNTER2A__QUAD            <<  \
                                                                  COUNTER2A_MODE_SHIFT))
#define COUNTER2A_MODE_PWM                       ((uint32)(COUNTER2A__PWM             <<  \
                                                                  COUNTER2A_MODE_SHIFT))
#define COUNTER2A_MODE_PWM_DT                    ((uint32)(COUNTER2A__PWM_DT          <<  \
                                                                  COUNTER2A_MODE_SHIFT))
#define COUNTER2A_MODE_PWM_PR                    ((uint32)(COUNTER2A__PWM_PR          <<  \
                                                                  COUNTER2A_MODE_SHIFT))

/* Quad Modes */
#define COUNTER2A_MODE_X1                        ((uint32)(COUNTER2A__X1              <<  \
                                                                  COUNTER2A_QUAD_MODE_SHIFT))
#define COUNTER2A_MODE_X2                        ((uint32)(COUNTER2A__X2              <<  \
                                                                  COUNTER2A_QUAD_MODE_SHIFT))
#define COUNTER2A_MODE_X4                        ((uint32)(COUNTER2A__X4              <<  \
                                                                  COUNTER2A_QUAD_MODE_SHIFT))

/* Counter modes */
#define COUNTER2A_COUNT_UP                       ((uint32)(COUNTER2A__COUNT_UP        <<  \
                                                                  COUNTER2A_UPDOWN_SHIFT))
#define COUNTER2A_COUNT_DOWN                     ((uint32)(COUNTER2A__COUNT_DOWN      <<  \
                                                                  COUNTER2A_UPDOWN_SHIFT))
#define COUNTER2A_COUNT_UPDOWN0                  ((uint32)(COUNTER2A__COUNT_UPDOWN0   <<  \
                                                                  COUNTER2A_UPDOWN_SHIFT))
#define COUNTER2A_COUNT_UPDOWN1                  ((uint32)(COUNTER2A__COUNT_UPDOWN1   <<  \
                                                                  COUNTER2A_UPDOWN_SHIFT))

/* PWM output invert */
#define COUNTER2A_INVERT_LINE                    ((uint32)(COUNTER2A__INVERSE         <<  \
                                                                  COUNTER2A_INV_OUT_SHIFT))
#define COUNTER2A_INVERT_LINE_N                  ((uint32)(COUNTER2A__INVERSE         <<  \
                                                                  COUNTER2A_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define COUNTER2A_TRIG_RISING                    ((uint32)COUNTER2A__TRIG_RISING)
#define COUNTER2A_TRIG_FALLING                   ((uint32)COUNTER2A__TRIG_FALLING)
#define COUNTER2A_TRIG_BOTH                      ((uint32)COUNTER2A__TRIG_BOTH)
#define COUNTER2A_TRIG_LEVEL                     ((uint32)COUNTER2A__TRIG_LEVEL)

/* Interrupt mask */
#define COUNTER2A_INTR_MASK_TC                   ((uint32)COUNTER2A__INTR_MASK_TC)
#define COUNTER2A_INTR_MASK_CC_MATCH             ((uint32)COUNTER2A__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define COUNTER2A_CC_MATCH_SET                   (0x00u)
#define COUNTER2A_CC_MATCH_CLEAR                 (0x01u)
#define COUNTER2A_CC_MATCH_INVERT                (0x02u)
#define COUNTER2A_CC_MATCH_NO_CHANGE             (0x03u)
#define COUNTER2A_OVERLOW_SET                    (0x00u)
#define COUNTER2A_OVERLOW_CLEAR                  (0x04u)
#define COUNTER2A_OVERLOW_INVERT                 (0x08u)
#define COUNTER2A_OVERLOW_NO_CHANGE              (0x0Cu)
#define COUNTER2A_UNDERFLOW_SET                  (0x00u)
#define COUNTER2A_UNDERFLOW_CLEAR                (0x10u)
#define COUNTER2A_UNDERFLOW_INVERT               (0x20u)
#define COUNTER2A_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define COUNTER2A_PWM_MODE_LEFT                  (COUNTER2A_CC_MATCH_CLEAR        |   \
                                                         COUNTER2A_OVERLOW_SET           |   \
                                                         COUNTER2A_UNDERFLOW_NO_CHANGE)
#define COUNTER2A_PWM_MODE_RIGHT                 (COUNTER2A_CC_MATCH_SET          |   \
                                                         COUNTER2A_OVERLOW_NO_CHANGE     |   \
                                                         COUNTER2A_UNDERFLOW_CLEAR)
#define COUNTER2A_PWM_MODE_ASYM                  (COUNTER2A_CC_MATCH_INVERT       |   \
                                                         COUNTER2A_OVERLOW_SET           |   \
                                                         COUNTER2A_UNDERFLOW_CLEAR)

#if (COUNTER2A_CY_TCPWM_V2)
    #if(COUNTER2A_CY_TCPWM_4000)
        #define COUNTER2A_PWM_MODE_CENTER                (COUNTER2A_CC_MATCH_INVERT       |   \
                                                                 COUNTER2A_OVERLOW_NO_CHANGE     |   \
                                                                 COUNTER2A_UNDERFLOW_CLEAR)
    #else
        #define COUNTER2A_PWM_MODE_CENTER                (COUNTER2A_CC_MATCH_INVERT       |   \
                                                                 COUNTER2A_OVERLOW_SET           |   \
                                                                 COUNTER2A_UNDERFLOW_CLEAR)
    #endif /* (COUNTER2A_CY_TCPWM_4000) */
#else
    #define COUNTER2A_PWM_MODE_CENTER                (COUNTER2A_CC_MATCH_INVERT       |   \
                                                             COUNTER2A_OVERLOW_NO_CHANGE     |   \
                                                             COUNTER2A_UNDERFLOW_CLEAR)
#endif /* (COUNTER2A_CY_TCPWM_NEW) */

/* Command operations without condition */
#define COUNTER2A_CMD_CAPTURE                    (0u)
#define COUNTER2A_CMD_RELOAD                     (8u)
#define COUNTER2A_CMD_STOP                       (16u)
#define COUNTER2A_CMD_START                      (24u)

/* Status */
#define COUNTER2A_STATUS_DOWN                    (1u)
#define COUNTER2A_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   COUNTER2A_Init(void);
void   COUNTER2A_Enable(void);
void   COUNTER2A_Start(void);
void   COUNTER2A_Stop(void);

void   COUNTER2A_SetMode(uint32 mode);
void   COUNTER2A_SetCounterMode(uint32 counterMode);
void   COUNTER2A_SetPWMMode(uint32 modeMask);
void   COUNTER2A_SetQDMode(uint32 qdMode);

void   COUNTER2A_SetPrescaler(uint32 prescaler);
void   COUNTER2A_TriggerCommand(uint32 mask, uint32 command);
void   COUNTER2A_SetOneShot(uint32 oneShotEnable);
uint32 COUNTER2A_ReadStatus(void);

void   COUNTER2A_SetPWMSyncKill(uint32 syncKillEnable);
void   COUNTER2A_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   COUNTER2A_SetPWMDeadTime(uint32 deadTime);
void   COUNTER2A_SetPWMInvert(uint32 mask);

void   COUNTER2A_SetInterruptMode(uint32 interruptMask);
uint32 COUNTER2A_GetInterruptSourceMasked(void);
uint32 COUNTER2A_GetInterruptSource(void);
void   COUNTER2A_ClearInterrupt(uint32 interruptMask);
void   COUNTER2A_SetInterrupt(uint32 interruptMask);

void   COUNTER2A_WriteCounter(uint32 count);
uint32 COUNTER2A_ReadCounter(void);

uint32 COUNTER2A_ReadCapture(void);
uint32 COUNTER2A_ReadCaptureBuf(void);

void   COUNTER2A_WritePeriod(uint32 period);
uint32 COUNTER2A_ReadPeriod(void);
void   COUNTER2A_WritePeriodBuf(uint32 periodBuf);
uint32 COUNTER2A_ReadPeriodBuf(void);

void   COUNTER2A_WriteCompare(uint32 compare);
uint32 COUNTER2A_ReadCompare(void);
void   COUNTER2A_WriteCompareBuf(uint32 compareBuf);
uint32 COUNTER2A_ReadCompareBuf(void);

void   COUNTER2A_SetPeriodSwap(uint32 swapEnable);
void   COUNTER2A_SetCompareSwap(uint32 swapEnable);

void   COUNTER2A_SetCaptureMode(uint32 triggerMode);
void   COUNTER2A_SetReloadMode(uint32 triggerMode);
void   COUNTER2A_SetStartMode(uint32 triggerMode);
void   COUNTER2A_SetStopMode(uint32 triggerMode);
void   COUNTER2A_SetCountMode(uint32 triggerMode);

void   COUNTER2A_SaveConfig(void);
void   COUNTER2A_RestoreConfig(void);
void   COUNTER2A_Sleep(void);
void   COUNTER2A_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define COUNTER2A_BLOCK_CONTROL_REG              (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define COUNTER2A_BLOCK_CONTROL_PTR              ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define COUNTER2A_COMMAND_REG                    (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define COUNTER2A_COMMAND_PTR                    ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define COUNTER2A_INTRRUPT_CAUSE_REG             (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define COUNTER2A_INTRRUPT_CAUSE_PTR             ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define COUNTER2A_CONTROL_REG                    (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__CTRL )
#define COUNTER2A_CONTROL_PTR                    ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__CTRL )
#define COUNTER2A_STATUS_REG                     (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__STATUS )
#define COUNTER2A_STATUS_PTR                     ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__STATUS )
#define COUNTER2A_COUNTER_REG                    (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__COUNTER )
#define COUNTER2A_COUNTER_PTR                    ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__COUNTER )
#define COUNTER2A_COMP_CAP_REG                   (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__CC )
#define COUNTER2A_COMP_CAP_PTR                   ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__CC )
#define COUNTER2A_COMP_CAP_BUF_REG               (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__CC_BUFF )
#define COUNTER2A_COMP_CAP_BUF_PTR               ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__CC_BUFF )
#define COUNTER2A_PERIOD_REG                     (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__PERIOD )
#define COUNTER2A_PERIOD_PTR                     ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__PERIOD )
#define COUNTER2A_PERIOD_BUF_REG                 (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define COUNTER2A_PERIOD_BUF_PTR                 ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define COUNTER2A_TRIG_CONTROL0_REG              (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define COUNTER2A_TRIG_CONTROL0_PTR              ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define COUNTER2A_TRIG_CONTROL1_REG              (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define COUNTER2A_TRIG_CONTROL1_PTR              ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define COUNTER2A_TRIG_CONTROL2_REG              (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define COUNTER2A_TRIG_CONTROL2_PTR              ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define COUNTER2A_INTERRUPT_REQ_REG              (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__INTR )
#define COUNTER2A_INTERRUPT_REQ_PTR              ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__INTR )
#define COUNTER2A_INTERRUPT_SET_REG              (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__INTR_SET )
#define COUNTER2A_INTERRUPT_SET_PTR              ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__INTR_SET )
#define COUNTER2A_INTERRUPT_MASK_REG             (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__INTR_MASK )
#define COUNTER2A_INTERRUPT_MASK_PTR             ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__INTR_MASK )
#define COUNTER2A_INTERRUPT_MASKED_REG           (*(reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__INTR_MASKED )
#define COUNTER2A_INTERRUPT_MASKED_PTR           ( (reg32 *) COUNTER2A_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define COUNTER2A_MASK                           ((uint32)COUNTER2A_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define COUNTER2A_RELOAD_CC_SHIFT                (0u)
#define COUNTER2A_RELOAD_PERIOD_SHIFT            (1u)
#define COUNTER2A_PWM_SYNC_KILL_SHIFT            (2u)
#define COUNTER2A_PWM_STOP_KILL_SHIFT            (3u)
#define COUNTER2A_PRESCALER_SHIFT                (8u)
#define COUNTER2A_UPDOWN_SHIFT                   (16u)
#define COUNTER2A_ONESHOT_SHIFT                  (18u)
#define COUNTER2A_QUAD_MODE_SHIFT                (20u)
#define COUNTER2A_INV_OUT_SHIFT                  (20u)
#define COUNTER2A_INV_COMPL_OUT_SHIFT            (21u)
#define COUNTER2A_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define COUNTER2A_RELOAD_CC_MASK                 ((uint32)(COUNTER2A_1BIT_MASK        <<  \
                                                                            COUNTER2A_RELOAD_CC_SHIFT))
#define COUNTER2A_RELOAD_PERIOD_MASK             ((uint32)(COUNTER2A_1BIT_MASK        <<  \
                                                                            COUNTER2A_RELOAD_PERIOD_SHIFT))
#define COUNTER2A_PWM_SYNC_KILL_MASK             ((uint32)(COUNTER2A_1BIT_MASK        <<  \
                                                                            COUNTER2A_PWM_SYNC_KILL_SHIFT))
#define COUNTER2A_PWM_STOP_KILL_MASK             ((uint32)(COUNTER2A_1BIT_MASK        <<  \
                                                                            COUNTER2A_PWM_STOP_KILL_SHIFT))
#define COUNTER2A_PRESCALER_MASK                 ((uint32)(COUNTER2A_8BIT_MASK        <<  \
                                                                            COUNTER2A_PRESCALER_SHIFT))
#define COUNTER2A_UPDOWN_MASK                    ((uint32)(COUNTER2A_2BIT_MASK        <<  \
                                                                            COUNTER2A_UPDOWN_SHIFT))
#define COUNTER2A_ONESHOT_MASK                   ((uint32)(COUNTER2A_1BIT_MASK        <<  \
                                                                            COUNTER2A_ONESHOT_SHIFT))
#define COUNTER2A_QUAD_MODE_MASK                 ((uint32)(COUNTER2A_3BIT_MASK        <<  \
                                                                            COUNTER2A_QUAD_MODE_SHIFT))
#define COUNTER2A_INV_OUT_MASK                   ((uint32)(COUNTER2A_2BIT_MASK        <<  \
                                                                            COUNTER2A_INV_OUT_SHIFT))
#define COUNTER2A_MODE_MASK                      ((uint32)(COUNTER2A_3BIT_MASK        <<  \
                                                                            COUNTER2A_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define COUNTER2A_CAPTURE_SHIFT                  (0u)
#define COUNTER2A_COUNT_SHIFT                    (2u)
#define COUNTER2A_RELOAD_SHIFT                   (4u)
#define COUNTER2A_STOP_SHIFT                     (6u)
#define COUNTER2A_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define COUNTER2A_CAPTURE_MASK                   ((uint32)(COUNTER2A_2BIT_MASK        <<  \
                                                                  COUNTER2A_CAPTURE_SHIFT))
#define COUNTER2A_COUNT_MASK                     ((uint32)(COUNTER2A_2BIT_MASK        <<  \
                                                                  COUNTER2A_COUNT_SHIFT))
#define COUNTER2A_RELOAD_MASK                    ((uint32)(COUNTER2A_2BIT_MASK        <<  \
                                                                  COUNTER2A_RELOAD_SHIFT))
#define COUNTER2A_STOP_MASK                      ((uint32)(COUNTER2A_2BIT_MASK        <<  \
                                                                  COUNTER2A_STOP_SHIFT))
#define COUNTER2A_START_MASK                     ((uint32)(COUNTER2A_2BIT_MASK        <<  \
                                                                  COUNTER2A_START_SHIFT))

/* MASK */
#define COUNTER2A_1BIT_MASK                      ((uint32)0x01u)
#define COUNTER2A_2BIT_MASK                      ((uint32)0x03u)
#define COUNTER2A_3BIT_MASK                      ((uint32)0x07u)
#define COUNTER2A_6BIT_MASK                      ((uint32)0x3Fu)
#define COUNTER2A_8BIT_MASK                      ((uint32)0xFFu)
#define COUNTER2A_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define COUNTER2A_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define COUNTER2A_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(COUNTER2A_QUAD_ENCODING_MODES     << COUNTER2A_QUAD_MODE_SHIFT))       |\
         ((uint32)(COUNTER2A_CONFIG                  << COUNTER2A_MODE_SHIFT)))

#define COUNTER2A_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(COUNTER2A_PWM_STOP_EVENT          << COUNTER2A_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(COUNTER2A_PWM_OUT_INVERT          << COUNTER2A_INV_OUT_SHIFT))         |\
         ((uint32)(COUNTER2A_PWM_OUT_N_INVERT        << COUNTER2A_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(COUNTER2A_PWM_MODE                << COUNTER2A_MODE_SHIFT)))

#define COUNTER2A_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(COUNTER2A_PWM_RUN_MODE         << COUNTER2A_ONESHOT_SHIFT))
            
#define COUNTER2A_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(COUNTER2A_PWM_ALIGN            << COUNTER2A_UPDOWN_SHIFT))

#define COUNTER2A_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(COUNTER2A_PWM_KILL_EVENT      << COUNTER2A_PWM_SYNC_KILL_SHIFT))

#define COUNTER2A_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(COUNTER2A_PWM_DEAD_TIME_CYCLE  << COUNTER2A_PRESCALER_SHIFT))

#define COUNTER2A_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(COUNTER2A_PWM_PRESCALER        << COUNTER2A_PRESCALER_SHIFT))

#define COUNTER2A_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(COUNTER2A_TC_PRESCALER            << COUNTER2A_PRESCALER_SHIFT))       |\
         ((uint32)(COUNTER2A_TC_COUNTER_MODE         << COUNTER2A_UPDOWN_SHIFT))          |\
         ((uint32)(COUNTER2A_TC_RUN_MODE             << COUNTER2A_ONESHOT_SHIFT))         |\
         ((uint32)(COUNTER2A_TC_COMP_CAP_MODE        << COUNTER2A_MODE_SHIFT)))
        
#define COUNTER2A_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(COUNTER2A_QUAD_PHIA_SIGNAL_MODE   << COUNTER2A_COUNT_SHIFT))           |\
         ((uint32)(COUNTER2A_QUAD_INDEX_SIGNAL_MODE  << COUNTER2A_RELOAD_SHIFT))          |\
         ((uint32)(COUNTER2A_QUAD_STOP_SIGNAL_MODE   << COUNTER2A_STOP_SHIFT))            |\
         ((uint32)(COUNTER2A_QUAD_PHIB_SIGNAL_MODE   << COUNTER2A_START_SHIFT)))

#define COUNTER2A_PWM_SIGNALS_MODES                                                              \
        (((uint32)(COUNTER2A_PWM_SWITCH_SIGNAL_MODE  << COUNTER2A_CAPTURE_SHIFT))         |\
         ((uint32)(COUNTER2A_PWM_COUNT_SIGNAL_MODE   << COUNTER2A_COUNT_SHIFT))           |\
         ((uint32)(COUNTER2A_PWM_RELOAD_SIGNAL_MODE  << COUNTER2A_RELOAD_SHIFT))          |\
         ((uint32)(COUNTER2A_PWM_STOP_SIGNAL_MODE    << COUNTER2A_STOP_SHIFT))            |\
         ((uint32)(COUNTER2A_PWM_START_SIGNAL_MODE   << COUNTER2A_START_SHIFT)))

#define COUNTER2A_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(COUNTER2A_TC_CAPTURE_SIGNAL_MODE  << COUNTER2A_CAPTURE_SHIFT))         |\
         ((uint32)(COUNTER2A_TC_COUNT_SIGNAL_MODE    << COUNTER2A_COUNT_SHIFT))           |\
         ((uint32)(COUNTER2A_TC_RELOAD_SIGNAL_MODE   << COUNTER2A_RELOAD_SHIFT))          |\
         ((uint32)(COUNTER2A_TC_STOP_SIGNAL_MODE     << COUNTER2A_STOP_SHIFT))            |\
         ((uint32)(COUNTER2A_TC_START_SIGNAL_MODE    << COUNTER2A_START_SHIFT)))
        
#define COUNTER2A_TIMER_UPDOWN_CNT_USED                                                          \
                ((COUNTER2A__COUNT_UPDOWN0 == COUNTER2A_TC_COUNTER_MODE)                  ||\
                 (COUNTER2A__COUNT_UPDOWN1 == COUNTER2A_TC_COUNTER_MODE))

#define COUNTER2A_PWM_UPDOWN_CNT_USED                                                            \
                ((COUNTER2A__CENTER == COUNTER2A_PWM_ALIGN)                               ||\
                 (COUNTER2A__ASYMMETRIC == COUNTER2A_PWM_ALIGN))               
        
#define COUNTER2A_PWM_PR_INIT_VALUE              (1u)
#define COUNTER2A_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_COUNTER2A_H */

/* [] END OF FILE */
