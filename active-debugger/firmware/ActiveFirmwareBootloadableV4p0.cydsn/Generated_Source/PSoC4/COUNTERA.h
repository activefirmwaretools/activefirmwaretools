/*******************************************************************************
* File Name: COUNTERA.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the COUNTERA
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

#if !defined(CY_TCPWM_COUNTERA_H)
#define CY_TCPWM_COUNTERA_H


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
} COUNTERA_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  COUNTERA_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define COUNTERA_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define COUNTERA_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define COUNTERA_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define COUNTERA_QUAD_ENCODING_MODES            (0lu)
#define COUNTERA_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define COUNTERA_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define COUNTERA_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define COUNTERA_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define COUNTERA_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define COUNTERA_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define COUNTERA_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define COUNTERA_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define COUNTERA_TC_RUN_MODE                    (1lu)
#define COUNTERA_TC_COUNTER_MODE                (0lu)
#define COUNTERA_TC_COMP_CAP_MODE               (0lu)
#define COUNTERA_TC_PRESCALER                   (0lu)

/* Signal modes */
#define COUNTERA_TC_RELOAD_SIGNAL_MODE          (0lu)
#define COUNTERA_TC_COUNT_SIGNAL_MODE           (0lu)
#define COUNTERA_TC_START_SIGNAL_MODE           (0lu)
#define COUNTERA_TC_STOP_SIGNAL_MODE            (0lu)
#define COUNTERA_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define COUNTERA_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define COUNTERA_TC_COUNT_SIGNAL_PRESENT        (1lu)
#define COUNTERA_TC_START_SIGNAL_PRESENT        (0lu)
#define COUNTERA_TC_STOP_SIGNAL_PRESENT         (0lu)
#define COUNTERA_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define COUNTERA_TC_INTERRUPT_MASK              (0lu)

/* PWM Mode */
/* Parameters */
#define COUNTERA_PWM_KILL_EVENT                 (0lu)
#define COUNTERA_PWM_STOP_EVENT                 (0lu)
#define COUNTERA_PWM_MODE                       (4lu)
#define COUNTERA_PWM_OUT_N_INVERT               (0lu)
#define COUNTERA_PWM_OUT_INVERT                 (0lu)
#define COUNTERA_PWM_ALIGN                      (0lu)
#define COUNTERA_PWM_RUN_MODE                   (0lu)
#define COUNTERA_PWM_DEAD_TIME_CYCLE            (0lu)
#define COUNTERA_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define COUNTERA_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define COUNTERA_PWM_COUNT_SIGNAL_MODE          (3lu)
#define COUNTERA_PWM_START_SIGNAL_MODE          (0lu)
#define COUNTERA_PWM_STOP_SIGNAL_MODE           (0lu)
#define COUNTERA_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define COUNTERA_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define COUNTERA_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define COUNTERA_PWM_START_SIGNAL_PRESENT       (0lu)
#define COUNTERA_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define COUNTERA_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define COUNTERA_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define COUNTERA_TC_PERIOD_VALUE                (65535lu)
#define COUNTERA_TC_COMPARE_VALUE               (65535lu)
#define COUNTERA_TC_COMPARE_BUF_VALUE           (65535lu)
#define COUNTERA_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define COUNTERA_PWM_PERIOD_VALUE               (65535lu)
#define COUNTERA_PWM_PERIOD_BUF_VALUE           (65535lu)
#define COUNTERA_PWM_PERIOD_SWAP                (0lu)
#define COUNTERA_PWM_COMPARE_VALUE              (65535lu)
#define COUNTERA_PWM_COMPARE_BUF_VALUE          (65535lu)
#define COUNTERA_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define COUNTERA__LEFT 0
#define COUNTERA__RIGHT 1
#define COUNTERA__CENTER 2
#define COUNTERA__ASYMMETRIC 3

#define COUNTERA__X1 0
#define COUNTERA__X2 1
#define COUNTERA__X4 2

#define COUNTERA__PWM 4
#define COUNTERA__PWM_DT 5
#define COUNTERA__PWM_PR 6

#define COUNTERA__INVERSE 1
#define COUNTERA__DIRECT 0

#define COUNTERA__CAPTURE 2
#define COUNTERA__COMPARE 0

#define COUNTERA__TRIG_LEVEL 3
#define COUNTERA__TRIG_RISING 0
#define COUNTERA__TRIG_FALLING 1
#define COUNTERA__TRIG_BOTH 2

#define COUNTERA__INTR_MASK_TC 1
#define COUNTERA__INTR_MASK_CC_MATCH 2
#define COUNTERA__INTR_MASK_NONE 0
#define COUNTERA__INTR_MASK_TC_CC 3

#define COUNTERA__UNCONFIG 8
#define COUNTERA__TIMER 1
#define COUNTERA__QUAD 3
#define COUNTERA__PWM_SEL 7

#define COUNTERA__COUNT_UP 0
#define COUNTERA__COUNT_DOWN 1
#define COUNTERA__COUNT_UPDOWN0 2
#define COUNTERA__COUNT_UPDOWN1 3


/* Prescaler */
#define COUNTERA_PRESCALE_DIVBY1                ((uint32)(0u << COUNTERA_PRESCALER_SHIFT))
#define COUNTERA_PRESCALE_DIVBY2                ((uint32)(1u << COUNTERA_PRESCALER_SHIFT))
#define COUNTERA_PRESCALE_DIVBY4                ((uint32)(2u << COUNTERA_PRESCALER_SHIFT))
#define COUNTERA_PRESCALE_DIVBY8                ((uint32)(3u << COUNTERA_PRESCALER_SHIFT))
#define COUNTERA_PRESCALE_DIVBY16               ((uint32)(4u << COUNTERA_PRESCALER_SHIFT))
#define COUNTERA_PRESCALE_DIVBY32               ((uint32)(5u << COUNTERA_PRESCALER_SHIFT))
#define COUNTERA_PRESCALE_DIVBY64               ((uint32)(6u << COUNTERA_PRESCALER_SHIFT))
#define COUNTERA_PRESCALE_DIVBY128              ((uint32)(7u << COUNTERA_PRESCALER_SHIFT))

/* TCPWM set modes */
#define COUNTERA_MODE_TIMER_COMPARE             ((uint32)(COUNTERA__COMPARE         <<  \
                                                                  COUNTERA_MODE_SHIFT))
#define COUNTERA_MODE_TIMER_CAPTURE             ((uint32)(COUNTERA__CAPTURE         <<  \
                                                                  COUNTERA_MODE_SHIFT))
#define COUNTERA_MODE_QUAD                      ((uint32)(COUNTERA__QUAD            <<  \
                                                                  COUNTERA_MODE_SHIFT))
#define COUNTERA_MODE_PWM                       ((uint32)(COUNTERA__PWM             <<  \
                                                                  COUNTERA_MODE_SHIFT))
#define COUNTERA_MODE_PWM_DT                    ((uint32)(COUNTERA__PWM_DT          <<  \
                                                                  COUNTERA_MODE_SHIFT))
#define COUNTERA_MODE_PWM_PR                    ((uint32)(COUNTERA__PWM_PR          <<  \
                                                                  COUNTERA_MODE_SHIFT))

/* Quad Modes */
#define COUNTERA_MODE_X1                        ((uint32)(COUNTERA__X1              <<  \
                                                                  COUNTERA_QUAD_MODE_SHIFT))
#define COUNTERA_MODE_X2                        ((uint32)(COUNTERA__X2              <<  \
                                                                  COUNTERA_QUAD_MODE_SHIFT))
#define COUNTERA_MODE_X4                        ((uint32)(COUNTERA__X4              <<  \
                                                                  COUNTERA_QUAD_MODE_SHIFT))

/* Counter modes */
#define COUNTERA_COUNT_UP                       ((uint32)(COUNTERA__COUNT_UP        <<  \
                                                                  COUNTERA_UPDOWN_SHIFT))
#define COUNTERA_COUNT_DOWN                     ((uint32)(COUNTERA__COUNT_DOWN      <<  \
                                                                  COUNTERA_UPDOWN_SHIFT))
#define COUNTERA_COUNT_UPDOWN0                  ((uint32)(COUNTERA__COUNT_UPDOWN0   <<  \
                                                                  COUNTERA_UPDOWN_SHIFT))
#define COUNTERA_COUNT_UPDOWN1                  ((uint32)(COUNTERA__COUNT_UPDOWN1   <<  \
                                                                  COUNTERA_UPDOWN_SHIFT))

/* PWM output invert */
#define COUNTERA_INVERT_LINE                    ((uint32)(COUNTERA__INVERSE         <<  \
                                                                  COUNTERA_INV_OUT_SHIFT))
#define COUNTERA_INVERT_LINE_N                  ((uint32)(COUNTERA__INVERSE         <<  \
                                                                  COUNTERA_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define COUNTERA_TRIG_RISING                    ((uint32)COUNTERA__TRIG_RISING)
#define COUNTERA_TRIG_FALLING                   ((uint32)COUNTERA__TRIG_FALLING)
#define COUNTERA_TRIG_BOTH                      ((uint32)COUNTERA__TRIG_BOTH)
#define COUNTERA_TRIG_LEVEL                     ((uint32)COUNTERA__TRIG_LEVEL)

/* Interrupt mask */
#define COUNTERA_INTR_MASK_TC                   ((uint32)COUNTERA__INTR_MASK_TC)
#define COUNTERA_INTR_MASK_CC_MATCH             ((uint32)COUNTERA__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define COUNTERA_CC_MATCH_SET                   (0x00u)
#define COUNTERA_CC_MATCH_CLEAR                 (0x01u)
#define COUNTERA_CC_MATCH_INVERT                (0x02u)
#define COUNTERA_CC_MATCH_NO_CHANGE             (0x03u)
#define COUNTERA_OVERLOW_SET                    (0x00u)
#define COUNTERA_OVERLOW_CLEAR                  (0x04u)
#define COUNTERA_OVERLOW_INVERT                 (0x08u)
#define COUNTERA_OVERLOW_NO_CHANGE              (0x0Cu)
#define COUNTERA_UNDERFLOW_SET                  (0x00u)
#define COUNTERA_UNDERFLOW_CLEAR                (0x10u)
#define COUNTERA_UNDERFLOW_INVERT               (0x20u)
#define COUNTERA_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define COUNTERA_PWM_MODE_LEFT                  (COUNTERA_CC_MATCH_CLEAR        |   \
                                                         COUNTERA_OVERLOW_SET           |   \
                                                         COUNTERA_UNDERFLOW_NO_CHANGE)
#define COUNTERA_PWM_MODE_RIGHT                 (COUNTERA_CC_MATCH_SET          |   \
                                                         COUNTERA_OVERLOW_NO_CHANGE     |   \
                                                         COUNTERA_UNDERFLOW_CLEAR)
#define COUNTERA_PWM_MODE_ASYM                  (COUNTERA_CC_MATCH_INVERT       |   \
                                                         COUNTERA_OVERLOW_SET           |   \
                                                         COUNTERA_UNDERFLOW_CLEAR)

#if (COUNTERA_CY_TCPWM_V2)
    #if(COUNTERA_CY_TCPWM_4000)
        #define COUNTERA_PWM_MODE_CENTER                (COUNTERA_CC_MATCH_INVERT       |   \
                                                                 COUNTERA_OVERLOW_NO_CHANGE     |   \
                                                                 COUNTERA_UNDERFLOW_CLEAR)
    #else
        #define COUNTERA_PWM_MODE_CENTER                (COUNTERA_CC_MATCH_INVERT       |   \
                                                                 COUNTERA_OVERLOW_SET           |   \
                                                                 COUNTERA_UNDERFLOW_CLEAR)
    #endif /* (COUNTERA_CY_TCPWM_4000) */
#else
    #define COUNTERA_PWM_MODE_CENTER                (COUNTERA_CC_MATCH_INVERT       |   \
                                                             COUNTERA_OVERLOW_NO_CHANGE     |   \
                                                             COUNTERA_UNDERFLOW_CLEAR)
#endif /* (COUNTERA_CY_TCPWM_NEW) */

/* Command operations without condition */
#define COUNTERA_CMD_CAPTURE                    (0u)
#define COUNTERA_CMD_RELOAD                     (8u)
#define COUNTERA_CMD_STOP                       (16u)
#define COUNTERA_CMD_START                      (24u)

/* Status */
#define COUNTERA_STATUS_DOWN                    (1u)
#define COUNTERA_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   COUNTERA_Init(void);
void   COUNTERA_Enable(void);
void   COUNTERA_Start(void);
void   COUNTERA_Stop(void);

void   COUNTERA_SetMode(uint32 mode);
void   COUNTERA_SetCounterMode(uint32 counterMode);
void   COUNTERA_SetPWMMode(uint32 modeMask);
void   COUNTERA_SetQDMode(uint32 qdMode);

void   COUNTERA_SetPrescaler(uint32 prescaler);
void   COUNTERA_TriggerCommand(uint32 mask, uint32 command);
void   COUNTERA_SetOneShot(uint32 oneShotEnable);
uint32 COUNTERA_ReadStatus(void);

void   COUNTERA_SetPWMSyncKill(uint32 syncKillEnable);
void   COUNTERA_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   COUNTERA_SetPWMDeadTime(uint32 deadTime);
void   COUNTERA_SetPWMInvert(uint32 mask);

void   COUNTERA_SetInterruptMode(uint32 interruptMask);
uint32 COUNTERA_GetInterruptSourceMasked(void);
uint32 COUNTERA_GetInterruptSource(void);
void   COUNTERA_ClearInterrupt(uint32 interruptMask);
void   COUNTERA_SetInterrupt(uint32 interruptMask);

void   COUNTERA_WriteCounter(uint32 count);
uint32 COUNTERA_ReadCounter(void);

uint32 COUNTERA_ReadCapture(void);
uint32 COUNTERA_ReadCaptureBuf(void);

void   COUNTERA_WritePeriod(uint32 period);
uint32 COUNTERA_ReadPeriod(void);
void   COUNTERA_WritePeriodBuf(uint32 periodBuf);
uint32 COUNTERA_ReadPeriodBuf(void);

void   COUNTERA_WriteCompare(uint32 compare);
uint32 COUNTERA_ReadCompare(void);
void   COUNTERA_WriteCompareBuf(uint32 compareBuf);
uint32 COUNTERA_ReadCompareBuf(void);

void   COUNTERA_SetPeriodSwap(uint32 swapEnable);
void   COUNTERA_SetCompareSwap(uint32 swapEnable);

void   COUNTERA_SetCaptureMode(uint32 triggerMode);
void   COUNTERA_SetReloadMode(uint32 triggerMode);
void   COUNTERA_SetStartMode(uint32 triggerMode);
void   COUNTERA_SetStopMode(uint32 triggerMode);
void   COUNTERA_SetCountMode(uint32 triggerMode);

void   COUNTERA_SaveConfig(void);
void   COUNTERA_RestoreConfig(void);
void   COUNTERA_Sleep(void);
void   COUNTERA_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define COUNTERA_BLOCK_CONTROL_REG              (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define COUNTERA_BLOCK_CONTROL_PTR              ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define COUNTERA_COMMAND_REG                    (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define COUNTERA_COMMAND_PTR                    ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define COUNTERA_INTRRUPT_CAUSE_REG             (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define COUNTERA_INTRRUPT_CAUSE_PTR             ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define COUNTERA_CONTROL_REG                    (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__CTRL )
#define COUNTERA_CONTROL_PTR                    ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__CTRL )
#define COUNTERA_STATUS_REG                     (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__STATUS )
#define COUNTERA_STATUS_PTR                     ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__STATUS )
#define COUNTERA_COUNTER_REG                    (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__COUNTER )
#define COUNTERA_COUNTER_PTR                    ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__COUNTER )
#define COUNTERA_COMP_CAP_REG                   (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__CC )
#define COUNTERA_COMP_CAP_PTR                   ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__CC )
#define COUNTERA_COMP_CAP_BUF_REG               (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__CC_BUFF )
#define COUNTERA_COMP_CAP_BUF_PTR               ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__CC_BUFF )
#define COUNTERA_PERIOD_REG                     (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__PERIOD )
#define COUNTERA_PERIOD_PTR                     ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__PERIOD )
#define COUNTERA_PERIOD_BUF_REG                 (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define COUNTERA_PERIOD_BUF_PTR                 ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define COUNTERA_TRIG_CONTROL0_REG              (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define COUNTERA_TRIG_CONTROL0_PTR              ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define COUNTERA_TRIG_CONTROL1_REG              (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define COUNTERA_TRIG_CONTROL1_PTR              ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define COUNTERA_TRIG_CONTROL2_REG              (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define COUNTERA_TRIG_CONTROL2_PTR              ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define COUNTERA_INTERRUPT_REQ_REG              (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__INTR )
#define COUNTERA_INTERRUPT_REQ_PTR              ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__INTR )
#define COUNTERA_INTERRUPT_SET_REG              (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__INTR_SET )
#define COUNTERA_INTERRUPT_SET_PTR              ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__INTR_SET )
#define COUNTERA_INTERRUPT_MASK_REG             (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__INTR_MASK )
#define COUNTERA_INTERRUPT_MASK_PTR             ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__INTR_MASK )
#define COUNTERA_INTERRUPT_MASKED_REG           (*(reg32 *) COUNTERA_cy_m0s8_tcpwm_1__INTR_MASKED )
#define COUNTERA_INTERRUPT_MASKED_PTR           ( (reg32 *) COUNTERA_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define COUNTERA_MASK                           ((uint32)COUNTERA_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define COUNTERA_RELOAD_CC_SHIFT                (0u)
#define COUNTERA_RELOAD_PERIOD_SHIFT            (1u)
#define COUNTERA_PWM_SYNC_KILL_SHIFT            (2u)
#define COUNTERA_PWM_STOP_KILL_SHIFT            (3u)
#define COUNTERA_PRESCALER_SHIFT                (8u)
#define COUNTERA_UPDOWN_SHIFT                   (16u)
#define COUNTERA_ONESHOT_SHIFT                  (18u)
#define COUNTERA_QUAD_MODE_SHIFT                (20u)
#define COUNTERA_INV_OUT_SHIFT                  (20u)
#define COUNTERA_INV_COMPL_OUT_SHIFT            (21u)
#define COUNTERA_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define COUNTERA_RELOAD_CC_MASK                 ((uint32)(COUNTERA_1BIT_MASK        <<  \
                                                                            COUNTERA_RELOAD_CC_SHIFT))
#define COUNTERA_RELOAD_PERIOD_MASK             ((uint32)(COUNTERA_1BIT_MASK        <<  \
                                                                            COUNTERA_RELOAD_PERIOD_SHIFT))
#define COUNTERA_PWM_SYNC_KILL_MASK             ((uint32)(COUNTERA_1BIT_MASK        <<  \
                                                                            COUNTERA_PWM_SYNC_KILL_SHIFT))
#define COUNTERA_PWM_STOP_KILL_MASK             ((uint32)(COUNTERA_1BIT_MASK        <<  \
                                                                            COUNTERA_PWM_STOP_KILL_SHIFT))
#define COUNTERA_PRESCALER_MASK                 ((uint32)(COUNTERA_8BIT_MASK        <<  \
                                                                            COUNTERA_PRESCALER_SHIFT))
#define COUNTERA_UPDOWN_MASK                    ((uint32)(COUNTERA_2BIT_MASK        <<  \
                                                                            COUNTERA_UPDOWN_SHIFT))
#define COUNTERA_ONESHOT_MASK                   ((uint32)(COUNTERA_1BIT_MASK        <<  \
                                                                            COUNTERA_ONESHOT_SHIFT))
#define COUNTERA_QUAD_MODE_MASK                 ((uint32)(COUNTERA_3BIT_MASK        <<  \
                                                                            COUNTERA_QUAD_MODE_SHIFT))
#define COUNTERA_INV_OUT_MASK                   ((uint32)(COUNTERA_2BIT_MASK        <<  \
                                                                            COUNTERA_INV_OUT_SHIFT))
#define COUNTERA_MODE_MASK                      ((uint32)(COUNTERA_3BIT_MASK        <<  \
                                                                            COUNTERA_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define COUNTERA_CAPTURE_SHIFT                  (0u)
#define COUNTERA_COUNT_SHIFT                    (2u)
#define COUNTERA_RELOAD_SHIFT                   (4u)
#define COUNTERA_STOP_SHIFT                     (6u)
#define COUNTERA_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define COUNTERA_CAPTURE_MASK                   ((uint32)(COUNTERA_2BIT_MASK        <<  \
                                                                  COUNTERA_CAPTURE_SHIFT))
#define COUNTERA_COUNT_MASK                     ((uint32)(COUNTERA_2BIT_MASK        <<  \
                                                                  COUNTERA_COUNT_SHIFT))
#define COUNTERA_RELOAD_MASK                    ((uint32)(COUNTERA_2BIT_MASK        <<  \
                                                                  COUNTERA_RELOAD_SHIFT))
#define COUNTERA_STOP_MASK                      ((uint32)(COUNTERA_2BIT_MASK        <<  \
                                                                  COUNTERA_STOP_SHIFT))
#define COUNTERA_START_MASK                     ((uint32)(COUNTERA_2BIT_MASK        <<  \
                                                                  COUNTERA_START_SHIFT))

/* MASK */
#define COUNTERA_1BIT_MASK                      ((uint32)0x01u)
#define COUNTERA_2BIT_MASK                      ((uint32)0x03u)
#define COUNTERA_3BIT_MASK                      ((uint32)0x07u)
#define COUNTERA_6BIT_MASK                      ((uint32)0x3Fu)
#define COUNTERA_8BIT_MASK                      ((uint32)0xFFu)
#define COUNTERA_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define COUNTERA_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define COUNTERA_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(COUNTERA_QUAD_ENCODING_MODES     << COUNTERA_QUAD_MODE_SHIFT))       |\
         ((uint32)(COUNTERA_CONFIG                  << COUNTERA_MODE_SHIFT)))

#define COUNTERA_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(COUNTERA_PWM_STOP_EVENT          << COUNTERA_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(COUNTERA_PWM_OUT_INVERT          << COUNTERA_INV_OUT_SHIFT))         |\
         ((uint32)(COUNTERA_PWM_OUT_N_INVERT        << COUNTERA_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(COUNTERA_PWM_MODE                << COUNTERA_MODE_SHIFT)))

#define COUNTERA_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(COUNTERA_PWM_RUN_MODE         << COUNTERA_ONESHOT_SHIFT))
            
#define COUNTERA_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(COUNTERA_PWM_ALIGN            << COUNTERA_UPDOWN_SHIFT))

#define COUNTERA_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(COUNTERA_PWM_KILL_EVENT      << COUNTERA_PWM_SYNC_KILL_SHIFT))

#define COUNTERA_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(COUNTERA_PWM_DEAD_TIME_CYCLE  << COUNTERA_PRESCALER_SHIFT))

#define COUNTERA_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(COUNTERA_PWM_PRESCALER        << COUNTERA_PRESCALER_SHIFT))

#define COUNTERA_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(COUNTERA_TC_PRESCALER            << COUNTERA_PRESCALER_SHIFT))       |\
         ((uint32)(COUNTERA_TC_COUNTER_MODE         << COUNTERA_UPDOWN_SHIFT))          |\
         ((uint32)(COUNTERA_TC_RUN_MODE             << COUNTERA_ONESHOT_SHIFT))         |\
         ((uint32)(COUNTERA_TC_COMP_CAP_MODE        << COUNTERA_MODE_SHIFT)))
        
#define COUNTERA_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(COUNTERA_QUAD_PHIA_SIGNAL_MODE   << COUNTERA_COUNT_SHIFT))           |\
         ((uint32)(COUNTERA_QUAD_INDEX_SIGNAL_MODE  << COUNTERA_RELOAD_SHIFT))          |\
         ((uint32)(COUNTERA_QUAD_STOP_SIGNAL_MODE   << COUNTERA_STOP_SHIFT))            |\
         ((uint32)(COUNTERA_QUAD_PHIB_SIGNAL_MODE   << COUNTERA_START_SHIFT)))

#define COUNTERA_PWM_SIGNALS_MODES                                                              \
        (((uint32)(COUNTERA_PWM_SWITCH_SIGNAL_MODE  << COUNTERA_CAPTURE_SHIFT))         |\
         ((uint32)(COUNTERA_PWM_COUNT_SIGNAL_MODE   << COUNTERA_COUNT_SHIFT))           |\
         ((uint32)(COUNTERA_PWM_RELOAD_SIGNAL_MODE  << COUNTERA_RELOAD_SHIFT))          |\
         ((uint32)(COUNTERA_PWM_STOP_SIGNAL_MODE    << COUNTERA_STOP_SHIFT))            |\
         ((uint32)(COUNTERA_PWM_START_SIGNAL_MODE   << COUNTERA_START_SHIFT)))

#define COUNTERA_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(COUNTERA_TC_CAPTURE_SIGNAL_MODE  << COUNTERA_CAPTURE_SHIFT))         |\
         ((uint32)(COUNTERA_TC_COUNT_SIGNAL_MODE    << COUNTERA_COUNT_SHIFT))           |\
         ((uint32)(COUNTERA_TC_RELOAD_SIGNAL_MODE   << COUNTERA_RELOAD_SHIFT))          |\
         ((uint32)(COUNTERA_TC_STOP_SIGNAL_MODE     << COUNTERA_STOP_SHIFT))            |\
         ((uint32)(COUNTERA_TC_START_SIGNAL_MODE    << COUNTERA_START_SHIFT)))
        
#define COUNTERA_TIMER_UPDOWN_CNT_USED                                                          \
                ((COUNTERA__COUNT_UPDOWN0 == COUNTERA_TC_COUNTER_MODE)                  ||\
                 (COUNTERA__COUNT_UPDOWN1 == COUNTERA_TC_COUNTER_MODE))

#define COUNTERA_PWM_UPDOWN_CNT_USED                                                            \
                ((COUNTERA__CENTER == COUNTERA_PWM_ALIGN)                               ||\
                 (COUNTERA__ASYMMETRIC == COUNTERA_PWM_ALIGN))               
        
#define COUNTERA_PWM_PR_INIT_VALUE              (1u)
#define COUNTERA_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_COUNTERA_H */

/* [] END OF FILE */
