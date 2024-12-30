/*******************************************************************************
* File Name: COUNTUPA.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the COUNTUPA
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

#if !defined(CY_TCPWM_COUNTUPA_H)
#define CY_TCPWM_COUNTUPA_H


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
} COUNTUPA_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  COUNTUPA_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define COUNTUPA_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define COUNTUPA_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define COUNTUPA_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define COUNTUPA_QUAD_ENCODING_MODES            (0lu)
#define COUNTUPA_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define COUNTUPA_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define COUNTUPA_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define COUNTUPA_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define COUNTUPA_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define COUNTUPA_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define COUNTUPA_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define COUNTUPA_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define COUNTUPA_TC_RUN_MODE                    (1lu)
#define COUNTUPA_TC_COUNTER_MODE                (0lu)
#define COUNTUPA_TC_COMP_CAP_MODE               (2lu)
#define COUNTUPA_TC_PRESCALER                   (0lu)

/* Signal modes */
#define COUNTUPA_TC_RELOAD_SIGNAL_MODE          (0lu)
#define COUNTUPA_TC_COUNT_SIGNAL_MODE           (0lu)
#define COUNTUPA_TC_START_SIGNAL_MODE           (0lu)
#define COUNTUPA_TC_STOP_SIGNAL_MODE            (0lu)
#define COUNTUPA_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define COUNTUPA_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define COUNTUPA_TC_COUNT_SIGNAL_PRESENT        (1lu)
#define COUNTUPA_TC_START_SIGNAL_PRESENT        (0lu)
#define COUNTUPA_TC_STOP_SIGNAL_PRESENT         (0lu)
#define COUNTUPA_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define COUNTUPA_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define COUNTUPA_PWM_KILL_EVENT                 (0lu)
#define COUNTUPA_PWM_STOP_EVENT                 (0lu)
#define COUNTUPA_PWM_MODE                       (4lu)
#define COUNTUPA_PWM_OUT_N_INVERT               (0lu)
#define COUNTUPA_PWM_OUT_INVERT                 (0lu)
#define COUNTUPA_PWM_ALIGN                      (0lu)
#define COUNTUPA_PWM_RUN_MODE                   (0lu)
#define COUNTUPA_PWM_DEAD_TIME_CYCLE            (0lu)
#define COUNTUPA_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define COUNTUPA_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define COUNTUPA_PWM_COUNT_SIGNAL_MODE          (3lu)
#define COUNTUPA_PWM_START_SIGNAL_MODE          (0lu)
#define COUNTUPA_PWM_STOP_SIGNAL_MODE           (0lu)
#define COUNTUPA_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define COUNTUPA_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define COUNTUPA_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define COUNTUPA_PWM_START_SIGNAL_PRESENT       (0lu)
#define COUNTUPA_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define COUNTUPA_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define COUNTUPA_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define COUNTUPA_TC_PERIOD_VALUE                (32lu)
#define COUNTUPA_TC_COMPARE_VALUE               (7lu)
#define COUNTUPA_TC_COMPARE_BUF_VALUE           (65535lu)
#define COUNTUPA_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define COUNTUPA_PWM_PERIOD_VALUE               (65535lu)
#define COUNTUPA_PWM_PERIOD_BUF_VALUE           (65535lu)
#define COUNTUPA_PWM_PERIOD_SWAP                (0lu)
#define COUNTUPA_PWM_COMPARE_VALUE              (65535lu)
#define COUNTUPA_PWM_COMPARE_BUF_VALUE          (65535lu)
#define COUNTUPA_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define COUNTUPA__LEFT 0
#define COUNTUPA__RIGHT 1
#define COUNTUPA__CENTER 2
#define COUNTUPA__ASYMMETRIC 3

#define COUNTUPA__X1 0
#define COUNTUPA__X2 1
#define COUNTUPA__X4 2

#define COUNTUPA__PWM 4
#define COUNTUPA__PWM_DT 5
#define COUNTUPA__PWM_PR 6

#define COUNTUPA__INVERSE 1
#define COUNTUPA__DIRECT 0

#define COUNTUPA__CAPTURE 2
#define COUNTUPA__COMPARE 0

#define COUNTUPA__TRIG_LEVEL 3
#define COUNTUPA__TRIG_RISING 0
#define COUNTUPA__TRIG_FALLING 1
#define COUNTUPA__TRIG_BOTH 2

#define COUNTUPA__INTR_MASK_TC 1
#define COUNTUPA__INTR_MASK_CC_MATCH 2
#define COUNTUPA__INTR_MASK_NONE 0
#define COUNTUPA__INTR_MASK_TC_CC 3

#define COUNTUPA__UNCONFIG 8
#define COUNTUPA__TIMER 1
#define COUNTUPA__QUAD 3
#define COUNTUPA__PWM_SEL 7

#define COUNTUPA__COUNT_UP 0
#define COUNTUPA__COUNT_DOWN 1
#define COUNTUPA__COUNT_UPDOWN0 2
#define COUNTUPA__COUNT_UPDOWN1 3


/* Prescaler */
#define COUNTUPA_PRESCALE_DIVBY1                ((uint32)(0u << COUNTUPA_PRESCALER_SHIFT))
#define COUNTUPA_PRESCALE_DIVBY2                ((uint32)(1u << COUNTUPA_PRESCALER_SHIFT))
#define COUNTUPA_PRESCALE_DIVBY4                ((uint32)(2u << COUNTUPA_PRESCALER_SHIFT))
#define COUNTUPA_PRESCALE_DIVBY8                ((uint32)(3u << COUNTUPA_PRESCALER_SHIFT))
#define COUNTUPA_PRESCALE_DIVBY16               ((uint32)(4u << COUNTUPA_PRESCALER_SHIFT))
#define COUNTUPA_PRESCALE_DIVBY32               ((uint32)(5u << COUNTUPA_PRESCALER_SHIFT))
#define COUNTUPA_PRESCALE_DIVBY64               ((uint32)(6u << COUNTUPA_PRESCALER_SHIFT))
#define COUNTUPA_PRESCALE_DIVBY128              ((uint32)(7u << COUNTUPA_PRESCALER_SHIFT))

/* TCPWM set modes */
#define COUNTUPA_MODE_TIMER_COMPARE             ((uint32)(COUNTUPA__COMPARE         <<  \
                                                                  COUNTUPA_MODE_SHIFT))
#define COUNTUPA_MODE_TIMER_CAPTURE             ((uint32)(COUNTUPA__CAPTURE         <<  \
                                                                  COUNTUPA_MODE_SHIFT))
#define COUNTUPA_MODE_QUAD                      ((uint32)(COUNTUPA__QUAD            <<  \
                                                                  COUNTUPA_MODE_SHIFT))
#define COUNTUPA_MODE_PWM                       ((uint32)(COUNTUPA__PWM             <<  \
                                                                  COUNTUPA_MODE_SHIFT))
#define COUNTUPA_MODE_PWM_DT                    ((uint32)(COUNTUPA__PWM_DT          <<  \
                                                                  COUNTUPA_MODE_SHIFT))
#define COUNTUPA_MODE_PWM_PR                    ((uint32)(COUNTUPA__PWM_PR          <<  \
                                                                  COUNTUPA_MODE_SHIFT))

/* Quad Modes */
#define COUNTUPA_MODE_X1                        ((uint32)(COUNTUPA__X1              <<  \
                                                                  COUNTUPA_QUAD_MODE_SHIFT))
#define COUNTUPA_MODE_X2                        ((uint32)(COUNTUPA__X2              <<  \
                                                                  COUNTUPA_QUAD_MODE_SHIFT))
#define COUNTUPA_MODE_X4                        ((uint32)(COUNTUPA__X4              <<  \
                                                                  COUNTUPA_QUAD_MODE_SHIFT))

/* Counter modes */
#define COUNTUPA_COUNT_UP                       ((uint32)(COUNTUPA__COUNT_UP        <<  \
                                                                  COUNTUPA_UPDOWN_SHIFT))
#define COUNTUPA_COUNT_DOWN                     ((uint32)(COUNTUPA__COUNT_DOWN      <<  \
                                                                  COUNTUPA_UPDOWN_SHIFT))
#define COUNTUPA_COUNT_UPDOWN0                  ((uint32)(COUNTUPA__COUNT_UPDOWN0   <<  \
                                                                  COUNTUPA_UPDOWN_SHIFT))
#define COUNTUPA_COUNT_UPDOWN1                  ((uint32)(COUNTUPA__COUNT_UPDOWN1   <<  \
                                                                  COUNTUPA_UPDOWN_SHIFT))

/* PWM output invert */
#define COUNTUPA_INVERT_LINE                    ((uint32)(COUNTUPA__INVERSE         <<  \
                                                                  COUNTUPA_INV_OUT_SHIFT))
#define COUNTUPA_INVERT_LINE_N                  ((uint32)(COUNTUPA__INVERSE         <<  \
                                                                  COUNTUPA_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define COUNTUPA_TRIG_RISING                    ((uint32)COUNTUPA__TRIG_RISING)
#define COUNTUPA_TRIG_FALLING                   ((uint32)COUNTUPA__TRIG_FALLING)
#define COUNTUPA_TRIG_BOTH                      ((uint32)COUNTUPA__TRIG_BOTH)
#define COUNTUPA_TRIG_LEVEL                     ((uint32)COUNTUPA__TRIG_LEVEL)

/* Interrupt mask */
#define COUNTUPA_INTR_MASK_TC                   ((uint32)COUNTUPA__INTR_MASK_TC)
#define COUNTUPA_INTR_MASK_CC_MATCH             ((uint32)COUNTUPA__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define COUNTUPA_CC_MATCH_SET                   (0x00u)
#define COUNTUPA_CC_MATCH_CLEAR                 (0x01u)
#define COUNTUPA_CC_MATCH_INVERT                (0x02u)
#define COUNTUPA_CC_MATCH_NO_CHANGE             (0x03u)
#define COUNTUPA_OVERLOW_SET                    (0x00u)
#define COUNTUPA_OVERLOW_CLEAR                  (0x04u)
#define COUNTUPA_OVERLOW_INVERT                 (0x08u)
#define COUNTUPA_OVERLOW_NO_CHANGE              (0x0Cu)
#define COUNTUPA_UNDERFLOW_SET                  (0x00u)
#define COUNTUPA_UNDERFLOW_CLEAR                (0x10u)
#define COUNTUPA_UNDERFLOW_INVERT               (0x20u)
#define COUNTUPA_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define COUNTUPA_PWM_MODE_LEFT                  (COUNTUPA_CC_MATCH_CLEAR        |   \
                                                         COUNTUPA_OVERLOW_SET           |   \
                                                         COUNTUPA_UNDERFLOW_NO_CHANGE)
#define COUNTUPA_PWM_MODE_RIGHT                 (COUNTUPA_CC_MATCH_SET          |   \
                                                         COUNTUPA_OVERLOW_NO_CHANGE     |   \
                                                         COUNTUPA_UNDERFLOW_CLEAR)
#define COUNTUPA_PWM_MODE_ASYM                  (COUNTUPA_CC_MATCH_INVERT       |   \
                                                         COUNTUPA_OVERLOW_SET           |   \
                                                         COUNTUPA_UNDERFLOW_CLEAR)

#if (COUNTUPA_CY_TCPWM_V2)
    #if(COUNTUPA_CY_TCPWM_4000)
        #define COUNTUPA_PWM_MODE_CENTER                (COUNTUPA_CC_MATCH_INVERT       |   \
                                                                 COUNTUPA_OVERLOW_NO_CHANGE     |   \
                                                                 COUNTUPA_UNDERFLOW_CLEAR)
    #else
        #define COUNTUPA_PWM_MODE_CENTER                (COUNTUPA_CC_MATCH_INVERT       |   \
                                                                 COUNTUPA_OVERLOW_SET           |   \
                                                                 COUNTUPA_UNDERFLOW_CLEAR)
    #endif /* (COUNTUPA_CY_TCPWM_4000) */
#else
    #define COUNTUPA_PWM_MODE_CENTER                (COUNTUPA_CC_MATCH_INVERT       |   \
                                                             COUNTUPA_OVERLOW_NO_CHANGE     |   \
                                                             COUNTUPA_UNDERFLOW_CLEAR)
#endif /* (COUNTUPA_CY_TCPWM_NEW) */

/* Command operations without condition */
#define COUNTUPA_CMD_CAPTURE                    (0u)
#define COUNTUPA_CMD_RELOAD                     (8u)
#define COUNTUPA_CMD_STOP                       (16u)
#define COUNTUPA_CMD_START                      (24u)

/* Status */
#define COUNTUPA_STATUS_DOWN                    (1u)
#define COUNTUPA_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   COUNTUPA_Init(void);
void   COUNTUPA_Enable(void);
void   COUNTUPA_Start(void);
void   COUNTUPA_Stop(void);

void   COUNTUPA_SetMode(uint32 mode);
void   COUNTUPA_SetCounterMode(uint32 counterMode);
void   COUNTUPA_SetPWMMode(uint32 modeMask);
void   COUNTUPA_SetQDMode(uint32 qdMode);

void   COUNTUPA_SetPrescaler(uint32 prescaler);
void   COUNTUPA_TriggerCommand(uint32 mask, uint32 command);
void   COUNTUPA_SetOneShot(uint32 oneShotEnable);
uint32 COUNTUPA_ReadStatus(void);

void   COUNTUPA_SetPWMSyncKill(uint32 syncKillEnable);
void   COUNTUPA_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   COUNTUPA_SetPWMDeadTime(uint32 deadTime);
void   COUNTUPA_SetPWMInvert(uint32 mask);

void   COUNTUPA_SetInterruptMode(uint32 interruptMask);
uint32 COUNTUPA_GetInterruptSourceMasked(void);
uint32 COUNTUPA_GetInterruptSource(void);
void   COUNTUPA_ClearInterrupt(uint32 interruptMask);
void   COUNTUPA_SetInterrupt(uint32 interruptMask);

void   COUNTUPA_WriteCounter(uint32 count);
uint32 COUNTUPA_ReadCounter(void);

uint32 COUNTUPA_ReadCapture(void);
uint32 COUNTUPA_ReadCaptureBuf(void);

void   COUNTUPA_WritePeriod(uint32 period);
uint32 COUNTUPA_ReadPeriod(void);
void   COUNTUPA_WritePeriodBuf(uint32 periodBuf);
uint32 COUNTUPA_ReadPeriodBuf(void);

void   COUNTUPA_WriteCompare(uint32 compare);
uint32 COUNTUPA_ReadCompare(void);
void   COUNTUPA_WriteCompareBuf(uint32 compareBuf);
uint32 COUNTUPA_ReadCompareBuf(void);

void   COUNTUPA_SetPeriodSwap(uint32 swapEnable);
void   COUNTUPA_SetCompareSwap(uint32 swapEnable);

void   COUNTUPA_SetCaptureMode(uint32 triggerMode);
void   COUNTUPA_SetReloadMode(uint32 triggerMode);
void   COUNTUPA_SetStartMode(uint32 triggerMode);
void   COUNTUPA_SetStopMode(uint32 triggerMode);
void   COUNTUPA_SetCountMode(uint32 triggerMode);

void   COUNTUPA_SaveConfig(void);
void   COUNTUPA_RestoreConfig(void);
void   COUNTUPA_Sleep(void);
void   COUNTUPA_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define COUNTUPA_BLOCK_CONTROL_REG              (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define COUNTUPA_BLOCK_CONTROL_PTR              ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define COUNTUPA_COMMAND_REG                    (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define COUNTUPA_COMMAND_PTR                    ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define COUNTUPA_INTRRUPT_CAUSE_REG             (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define COUNTUPA_INTRRUPT_CAUSE_PTR             ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define COUNTUPA_CONTROL_REG                    (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__CTRL )
#define COUNTUPA_CONTROL_PTR                    ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__CTRL )
#define COUNTUPA_STATUS_REG                     (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__STATUS )
#define COUNTUPA_STATUS_PTR                     ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__STATUS )
#define COUNTUPA_COUNTER_REG                    (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__COUNTER )
#define COUNTUPA_COUNTER_PTR                    ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__COUNTER )
#define COUNTUPA_COMP_CAP_REG                   (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__CC )
#define COUNTUPA_COMP_CAP_PTR                   ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__CC )
#define COUNTUPA_COMP_CAP_BUF_REG               (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__CC_BUFF )
#define COUNTUPA_COMP_CAP_BUF_PTR               ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__CC_BUFF )
#define COUNTUPA_PERIOD_REG                     (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__PERIOD )
#define COUNTUPA_PERIOD_PTR                     ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__PERIOD )
#define COUNTUPA_PERIOD_BUF_REG                 (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define COUNTUPA_PERIOD_BUF_PTR                 ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define COUNTUPA_TRIG_CONTROL0_REG              (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define COUNTUPA_TRIG_CONTROL0_PTR              ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define COUNTUPA_TRIG_CONTROL1_REG              (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define COUNTUPA_TRIG_CONTROL1_PTR              ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define COUNTUPA_TRIG_CONTROL2_REG              (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define COUNTUPA_TRIG_CONTROL2_PTR              ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define COUNTUPA_INTERRUPT_REQ_REG              (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__INTR )
#define COUNTUPA_INTERRUPT_REQ_PTR              ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__INTR )
#define COUNTUPA_INTERRUPT_SET_REG              (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__INTR_SET )
#define COUNTUPA_INTERRUPT_SET_PTR              ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__INTR_SET )
#define COUNTUPA_INTERRUPT_MASK_REG             (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__INTR_MASK )
#define COUNTUPA_INTERRUPT_MASK_PTR             ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__INTR_MASK )
#define COUNTUPA_INTERRUPT_MASKED_REG           (*(reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__INTR_MASKED )
#define COUNTUPA_INTERRUPT_MASKED_PTR           ( (reg32 *) COUNTUPA_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define COUNTUPA_MASK                           ((uint32)COUNTUPA_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define COUNTUPA_RELOAD_CC_SHIFT                (0u)
#define COUNTUPA_RELOAD_PERIOD_SHIFT            (1u)
#define COUNTUPA_PWM_SYNC_KILL_SHIFT            (2u)
#define COUNTUPA_PWM_STOP_KILL_SHIFT            (3u)
#define COUNTUPA_PRESCALER_SHIFT                (8u)
#define COUNTUPA_UPDOWN_SHIFT                   (16u)
#define COUNTUPA_ONESHOT_SHIFT                  (18u)
#define COUNTUPA_QUAD_MODE_SHIFT                (20u)
#define COUNTUPA_INV_OUT_SHIFT                  (20u)
#define COUNTUPA_INV_COMPL_OUT_SHIFT            (21u)
#define COUNTUPA_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define COUNTUPA_RELOAD_CC_MASK                 ((uint32)(COUNTUPA_1BIT_MASK        <<  \
                                                                            COUNTUPA_RELOAD_CC_SHIFT))
#define COUNTUPA_RELOAD_PERIOD_MASK             ((uint32)(COUNTUPA_1BIT_MASK        <<  \
                                                                            COUNTUPA_RELOAD_PERIOD_SHIFT))
#define COUNTUPA_PWM_SYNC_KILL_MASK             ((uint32)(COUNTUPA_1BIT_MASK        <<  \
                                                                            COUNTUPA_PWM_SYNC_KILL_SHIFT))
#define COUNTUPA_PWM_STOP_KILL_MASK             ((uint32)(COUNTUPA_1BIT_MASK        <<  \
                                                                            COUNTUPA_PWM_STOP_KILL_SHIFT))
#define COUNTUPA_PRESCALER_MASK                 ((uint32)(COUNTUPA_8BIT_MASK        <<  \
                                                                            COUNTUPA_PRESCALER_SHIFT))
#define COUNTUPA_UPDOWN_MASK                    ((uint32)(COUNTUPA_2BIT_MASK        <<  \
                                                                            COUNTUPA_UPDOWN_SHIFT))
#define COUNTUPA_ONESHOT_MASK                   ((uint32)(COUNTUPA_1BIT_MASK        <<  \
                                                                            COUNTUPA_ONESHOT_SHIFT))
#define COUNTUPA_QUAD_MODE_MASK                 ((uint32)(COUNTUPA_3BIT_MASK        <<  \
                                                                            COUNTUPA_QUAD_MODE_SHIFT))
#define COUNTUPA_INV_OUT_MASK                   ((uint32)(COUNTUPA_2BIT_MASK        <<  \
                                                                            COUNTUPA_INV_OUT_SHIFT))
#define COUNTUPA_MODE_MASK                      ((uint32)(COUNTUPA_3BIT_MASK        <<  \
                                                                            COUNTUPA_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define COUNTUPA_CAPTURE_SHIFT                  (0u)
#define COUNTUPA_COUNT_SHIFT                    (2u)
#define COUNTUPA_RELOAD_SHIFT                   (4u)
#define COUNTUPA_STOP_SHIFT                     (6u)
#define COUNTUPA_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define COUNTUPA_CAPTURE_MASK                   ((uint32)(COUNTUPA_2BIT_MASK        <<  \
                                                                  COUNTUPA_CAPTURE_SHIFT))
#define COUNTUPA_COUNT_MASK                     ((uint32)(COUNTUPA_2BIT_MASK        <<  \
                                                                  COUNTUPA_COUNT_SHIFT))
#define COUNTUPA_RELOAD_MASK                    ((uint32)(COUNTUPA_2BIT_MASK        <<  \
                                                                  COUNTUPA_RELOAD_SHIFT))
#define COUNTUPA_STOP_MASK                      ((uint32)(COUNTUPA_2BIT_MASK        <<  \
                                                                  COUNTUPA_STOP_SHIFT))
#define COUNTUPA_START_MASK                     ((uint32)(COUNTUPA_2BIT_MASK        <<  \
                                                                  COUNTUPA_START_SHIFT))

/* MASK */
#define COUNTUPA_1BIT_MASK                      ((uint32)0x01u)
#define COUNTUPA_2BIT_MASK                      ((uint32)0x03u)
#define COUNTUPA_3BIT_MASK                      ((uint32)0x07u)
#define COUNTUPA_6BIT_MASK                      ((uint32)0x3Fu)
#define COUNTUPA_8BIT_MASK                      ((uint32)0xFFu)
#define COUNTUPA_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define COUNTUPA_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define COUNTUPA_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(COUNTUPA_QUAD_ENCODING_MODES     << COUNTUPA_QUAD_MODE_SHIFT))       |\
         ((uint32)(COUNTUPA_CONFIG                  << COUNTUPA_MODE_SHIFT)))

#define COUNTUPA_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(COUNTUPA_PWM_STOP_EVENT          << COUNTUPA_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(COUNTUPA_PWM_OUT_INVERT          << COUNTUPA_INV_OUT_SHIFT))         |\
         ((uint32)(COUNTUPA_PWM_OUT_N_INVERT        << COUNTUPA_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(COUNTUPA_PWM_MODE                << COUNTUPA_MODE_SHIFT)))

#define COUNTUPA_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(COUNTUPA_PWM_RUN_MODE         << COUNTUPA_ONESHOT_SHIFT))
            
#define COUNTUPA_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(COUNTUPA_PWM_ALIGN            << COUNTUPA_UPDOWN_SHIFT))

#define COUNTUPA_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(COUNTUPA_PWM_KILL_EVENT      << COUNTUPA_PWM_SYNC_KILL_SHIFT))

#define COUNTUPA_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(COUNTUPA_PWM_DEAD_TIME_CYCLE  << COUNTUPA_PRESCALER_SHIFT))

#define COUNTUPA_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(COUNTUPA_PWM_PRESCALER        << COUNTUPA_PRESCALER_SHIFT))

#define COUNTUPA_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(COUNTUPA_TC_PRESCALER            << COUNTUPA_PRESCALER_SHIFT))       |\
         ((uint32)(COUNTUPA_TC_COUNTER_MODE         << COUNTUPA_UPDOWN_SHIFT))          |\
         ((uint32)(COUNTUPA_TC_RUN_MODE             << COUNTUPA_ONESHOT_SHIFT))         |\
         ((uint32)(COUNTUPA_TC_COMP_CAP_MODE        << COUNTUPA_MODE_SHIFT)))
        
#define COUNTUPA_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(COUNTUPA_QUAD_PHIA_SIGNAL_MODE   << COUNTUPA_COUNT_SHIFT))           |\
         ((uint32)(COUNTUPA_QUAD_INDEX_SIGNAL_MODE  << COUNTUPA_RELOAD_SHIFT))          |\
         ((uint32)(COUNTUPA_QUAD_STOP_SIGNAL_MODE   << COUNTUPA_STOP_SHIFT))            |\
         ((uint32)(COUNTUPA_QUAD_PHIB_SIGNAL_MODE   << COUNTUPA_START_SHIFT)))

#define COUNTUPA_PWM_SIGNALS_MODES                                                              \
        (((uint32)(COUNTUPA_PWM_SWITCH_SIGNAL_MODE  << COUNTUPA_CAPTURE_SHIFT))         |\
         ((uint32)(COUNTUPA_PWM_COUNT_SIGNAL_MODE   << COUNTUPA_COUNT_SHIFT))           |\
         ((uint32)(COUNTUPA_PWM_RELOAD_SIGNAL_MODE  << COUNTUPA_RELOAD_SHIFT))          |\
         ((uint32)(COUNTUPA_PWM_STOP_SIGNAL_MODE    << COUNTUPA_STOP_SHIFT))            |\
         ((uint32)(COUNTUPA_PWM_START_SIGNAL_MODE   << COUNTUPA_START_SHIFT)))

#define COUNTUPA_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(COUNTUPA_TC_CAPTURE_SIGNAL_MODE  << COUNTUPA_CAPTURE_SHIFT))         |\
         ((uint32)(COUNTUPA_TC_COUNT_SIGNAL_MODE    << COUNTUPA_COUNT_SHIFT))           |\
         ((uint32)(COUNTUPA_TC_RELOAD_SIGNAL_MODE   << COUNTUPA_RELOAD_SHIFT))          |\
         ((uint32)(COUNTUPA_TC_STOP_SIGNAL_MODE     << COUNTUPA_STOP_SHIFT))            |\
         ((uint32)(COUNTUPA_TC_START_SIGNAL_MODE    << COUNTUPA_START_SHIFT)))
        
#define COUNTUPA_TIMER_UPDOWN_CNT_USED                                                          \
                ((COUNTUPA__COUNT_UPDOWN0 == COUNTUPA_TC_COUNTER_MODE)                  ||\
                 (COUNTUPA__COUNT_UPDOWN1 == COUNTUPA_TC_COUNTER_MODE))

#define COUNTUPA_PWM_UPDOWN_CNT_USED                                                            \
                ((COUNTUPA__CENTER == COUNTUPA_PWM_ALIGN)                               ||\
                 (COUNTUPA__ASYMMETRIC == COUNTUPA_PWM_ALIGN))               
        
#define COUNTUPA_PWM_PR_INIT_VALUE              (1u)
#define COUNTUPA_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_COUNTUPA_H */

/* [] END OF FILE */
