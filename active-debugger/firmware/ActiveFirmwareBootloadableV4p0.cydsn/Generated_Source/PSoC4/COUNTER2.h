/*******************************************************************************
* File Name: COUNTER2.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the COUNTER2
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

#if !defined(CY_TCPWM_COUNTER2_H)
#define CY_TCPWM_COUNTER2_H


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
} COUNTER2_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  COUNTER2_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define COUNTER2_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define COUNTER2_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define COUNTER2_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define COUNTER2_QUAD_ENCODING_MODES            (0lu)
#define COUNTER2_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define COUNTER2_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define COUNTER2_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define COUNTER2_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define COUNTER2_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define COUNTER2_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define COUNTER2_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define COUNTER2_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define COUNTER2_TC_RUN_MODE                    (0lu)
#define COUNTER2_TC_COUNTER_MODE                (0lu)
#define COUNTER2_TC_COMP_CAP_MODE               (2lu)
#define COUNTER2_TC_PRESCALER                   (0lu)

/* Signal modes */
#define COUNTER2_TC_RELOAD_SIGNAL_MODE          (0lu)
#define COUNTER2_TC_COUNT_SIGNAL_MODE           (3lu)
#define COUNTER2_TC_START_SIGNAL_MODE           (0lu)
#define COUNTER2_TC_STOP_SIGNAL_MODE            (0lu)
#define COUNTER2_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define COUNTER2_TC_RELOAD_SIGNAL_PRESENT       (1lu)
#define COUNTER2_TC_COUNT_SIGNAL_PRESENT        (1lu)
#define COUNTER2_TC_START_SIGNAL_PRESENT        (0lu)
#define COUNTER2_TC_STOP_SIGNAL_PRESENT         (0lu)
#define COUNTER2_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define COUNTER2_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define COUNTER2_PWM_KILL_EVENT                 (0lu)
#define COUNTER2_PWM_STOP_EVENT                 (0lu)
#define COUNTER2_PWM_MODE                       (4lu)
#define COUNTER2_PWM_OUT_N_INVERT               (0lu)
#define COUNTER2_PWM_OUT_INVERT                 (0lu)
#define COUNTER2_PWM_ALIGN                      (0lu)
#define COUNTER2_PWM_RUN_MODE                   (0lu)
#define COUNTER2_PWM_DEAD_TIME_CYCLE            (0lu)
#define COUNTER2_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define COUNTER2_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define COUNTER2_PWM_COUNT_SIGNAL_MODE          (3lu)
#define COUNTER2_PWM_START_SIGNAL_MODE          (0lu)
#define COUNTER2_PWM_STOP_SIGNAL_MODE           (0lu)
#define COUNTER2_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define COUNTER2_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define COUNTER2_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define COUNTER2_PWM_START_SIGNAL_PRESENT       (0lu)
#define COUNTER2_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define COUNTER2_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define COUNTER2_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define COUNTER2_TC_PERIOD_VALUE                (8lu)
#define COUNTER2_TC_COMPARE_VALUE               (65535lu)
#define COUNTER2_TC_COMPARE_BUF_VALUE           (65535lu)
#define COUNTER2_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define COUNTER2_PWM_PERIOD_VALUE               (65535lu)
#define COUNTER2_PWM_PERIOD_BUF_VALUE           (65535lu)
#define COUNTER2_PWM_PERIOD_SWAP                (0lu)
#define COUNTER2_PWM_COMPARE_VALUE              (65535lu)
#define COUNTER2_PWM_COMPARE_BUF_VALUE          (65535lu)
#define COUNTER2_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define COUNTER2__LEFT 0
#define COUNTER2__RIGHT 1
#define COUNTER2__CENTER 2
#define COUNTER2__ASYMMETRIC 3

#define COUNTER2__X1 0
#define COUNTER2__X2 1
#define COUNTER2__X4 2

#define COUNTER2__PWM 4
#define COUNTER2__PWM_DT 5
#define COUNTER2__PWM_PR 6

#define COUNTER2__INVERSE 1
#define COUNTER2__DIRECT 0

#define COUNTER2__CAPTURE 2
#define COUNTER2__COMPARE 0

#define COUNTER2__TRIG_LEVEL 3
#define COUNTER2__TRIG_RISING 0
#define COUNTER2__TRIG_FALLING 1
#define COUNTER2__TRIG_BOTH 2

#define COUNTER2__INTR_MASK_TC 1
#define COUNTER2__INTR_MASK_CC_MATCH 2
#define COUNTER2__INTR_MASK_NONE 0
#define COUNTER2__INTR_MASK_TC_CC 3

#define COUNTER2__UNCONFIG 8
#define COUNTER2__TIMER 1
#define COUNTER2__QUAD 3
#define COUNTER2__PWM_SEL 7

#define COUNTER2__COUNT_UP 0
#define COUNTER2__COUNT_DOWN 1
#define COUNTER2__COUNT_UPDOWN0 2
#define COUNTER2__COUNT_UPDOWN1 3


/* Prescaler */
#define COUNTER2_PRESCALE_DIVBY1                ((uint32)(0u << COUNTER2_PRESCALER_SHIFT))
#define COUNTER2_PRESCALE_DIVBY2                ((uint32)(1u << COUNTER2_PRESCALER_SHIFT))
#define COUNTER2_PRESCALE_DIVBY4                ((uint32)(2u << COUNTER2_PRESCALER_SHIFT))
#define COUNTER2_PRESCALE_DIVBY8                ((uint32)(3u << COUNTER2_PRESCALER_SHIFT))
#define COUNTER2_PRESCALE_DIVBY16               ((uint32)(4u << COUNTER2_PRESCALER_SHIFT))
#define COUNTER2_PRESCALE_DIVBY32               ((uint32)(5u << COUNTER2_PRESCALER_SHIFT))
#define COUNTER2_PRESCALE_DIVBY64               ((uint32)(6u << COUNTER2_PRESCALER_SHIFT))
#define COUNTER2_PRESCALE_DIVBY128              ((uint32)(7u << COUNTER2_PRESCALER_SHIFT))

/* TCPWM set modes */
#define COUNTER2_MODE_TIMER_COMPARE             ((uint32)(COUNTER2__COMPARE         <<  \
                                                                  COUNTER2_MODE_SHIFT))
#define COUNTER2_MODE_TIMER_CAPTURE             ((uint32)(COUNTER2__CAPTURE         <<  \
                                                                  COUNTER2_MODE_SHIFT))
#define COUNTER2_MODE_QUAD                      ((uint32)(COUNTER2__QUAD            <<  \
                                                                  COUNTER2_MODE_SHIFT))
#define COUNTER2_MODE_PWM                       ((uint32)(COUNTER2__PWM             <<  \
                                                                  COUNTER2_MODE_SHIFT))
#define COUNTER2_MODE_PWM_DT                    ((uint32)(COUNTER2__PWM_DT          <<  \
                                                                  COUNTER2_MODE_SHIFT))
#define COUNTER2_MODE_PWM_PR                    ((uint32)(COUNTER2__PWM_PR          <<  \
                                                                  COUNTER2_MODE_SHIFT))

/* Quad Modes */
#define COUNTER2_MODE_X1                        ((uint32)(COUNTER2__X1              <<  \
                                                                  COUNTER2_QUAD_MODE_SHIFT))
#define COUNTER2_MODE_X2                        ((uint32)(COUNTER2__X2              <<  \
                                                                  COUNTER2_QUAD_MODE_SHIFT))
#define COUNTER2_MODE_X4                        ((uint32)(COUNTER2__X4              <<  \
                                                                  COUNTER2_QUAD_MODE_SHIFT))

/* Counter modes */
#define COUNTER2_COUNT_UP                       ((uint32)(COUNTER2__COUNT_UP        <<  \
                                                                  COUNTER2_UPDOWN_SHIFT))
#define COUNTER2_COUNT_DOWN                     ((uint32)(COUNTER2__COUNT_DOWN      <<  \
                                                                  COUNTER2_UPDOWN_SHIFT))
#define COUNTER2_COUNT_UPDOWN0                  ((uint32)(COUNTER2__COUNT_UPDOWN0   <<  \
                                                                  COUNTER2_UPDOWN_SHIFT))
#define COUNTER2_COUNT_UPDOWN1                  ((uint32)(COUNTER2__COUNT_UPDOWN1   <<  \
                                                                  COUNTER2_UPDOWN_SHIFT))

/* PWM output invert */
#define COUNTER2_INVERT_LINE                    ((uint32)(COUNTER2__INVERSE         <<  \
                                                                  COUNTER2_INV_OUT_SHIFT))
#define COUNTER2_INVERT_LINE_N                  ((uint32)(COUNTER2__INVERSE         <<  \
                                                                  COUNTER2_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define COUNTER2_TRIG_RISING                    ((uint32)COUNTER2__TRIG_RISING)
#define COUNTER2_TRIG_FALLING                   ((uint32)COUNTER2__TRIG_FALLING)
#define COUNTER2_TRIG_BOTH                      ((uint32)COUNTER2__TRIG_BOTH)
#define COUNTER2_TRIG_LEVEL                     ((uint32)COUNTER2__TRIG_LEVEL)

/* Interrupt mask */
#define COUNTER2_INTR_MASK_TC                   ((uint32)COUNTER2__INTR_MASK_TC)
#define COUNTER2_INTR_MASK_CC_MATCH             ((uint32)COUNTER2__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define COUNTER2_CC_MATCH_SET                   (0x00u)
#define COUNTER2_CC_MATCH_CLEAR                 (0x01u)
#define COUNTER2_CC_MATCH_INVERT                (0x02u)
#define COUNTER2_CC_MATCH_NO_CHANGE             (0x03u)
#define COUNTER2_OVERLOW_SET                    (0x00u)
#define COUNTER2_OVERLOW_CLEAR                  (0x04u)
#define COUNTER2_OVERLOW_INVERT                 (0x08u)
#define COUNTER2_OVERLOW_NO_CHANGE              (0x0Cu)
#define COUNTER2_UNDERFLOW_SET                  (0x00u)
#define COUNTER2_UNDERFLOW_CLEAR                (0x10u)
#define COUNTER2_UNDERFLOW_INVERT               (0x20u)
#define COUNTER2_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define COUNTER2_PWM_MODE_LEFT                  (COUNTER2_CC_MATCH_CLEAR        |   \
                                                         COUNTER2_OVERLOW_SET           |   \
                                                         COUNTER2_UNDERFLOW_NO_CHANGE)
#define COUNTER2_PWM_MODE_RIGHT                 (COUNTER2_CC_MATCH_SET          |   \
                                                         COUNTER2_OVERLOW_NO_CHANGE     |   \
                                                         COUNTER2_UNDERFLOW_CLEAR)
#define COUNTER2_PWM_MODE_ASYM                  (COUNTER2_CC_MATCH_INVERT       |   \
                                                         COUNTER2_OVERLOW_SET           |   \
                                                         COUNTER2_UNDERFLOW_CLEAR)

#if (COUNTER2_CY_TCPWM_V2)
    #if(COUNTER2_CY_TCPWM_4000)
        #define COUNTER2_PWM_MODE_CENTER                (COUNTER2_CC_MATCH_INVERT       |   \
                                                                 COUNTER2_OVERLOW_NO_CHANGE     |   \
                                                                 COUNTER2_UNDERFLOW_CLEAR)
    #else
        #define COUNTER2_PWM_MODE_CENTER                (COUNTER2_CC_MATCH_INVERT       |   \
                                                                 COUNTER2_OVERLOW_SET           |   \
                                                                 COUNTER2_UNDERFLOW_CLEAR)
    #endif /* (COUNTER2_CY_TCPWM_4000) */
#else
    #define COUNTER2_PWM_MODE_CENTER                (COUNTER2_CC_MATCH_INVERT       |   \
                                                             COUNTER2_OVERLOW_NO_CHANGE     |   \
                                                             COUNTER2_UNDERFLOW_CLEAR)
#endif /* (COUNTER2_CY_TCPWM_NEW) */

/* Command operations without condition */
#define COUNTER2_CMD_CAPTURE                    (0u)
#define COUNTER2_CMD_RELOAD                     (8u)
#define COUNTER2_CMD_STOP                       (16u)
#define COUNTER2_CMD_START                      (24u)

/* Status */
#define COUNTER2_STATUS_DOWN                    (1u)
#define COUNTER2_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   COUNTER2_Init(void);
void   COUNTER2_Enable(void);
void   COUNTER2_Start(void);
void   COUNTER2_Stop(void);

void   COUNTER2_SetMode(uint32 mode);
void   COUNTER2_SetCounterMode(uint32 counterMode);
void   COUNTER2_SetPWMMode(uint32 modeMask);
void   COUNTER2_SetQDMode(uint32 qdMode);

void   COUNTER2_SetPrescaler(uint32 prescaler);
void   COUNTER2_TriggerCommand(uint32 mask, uint32 command);
void   COUNTER2_SetOneShot(uint32 oneShotEnable);
uint32 COUNTER2_ReadStatus(void);

void   COUNTER2_SetPWMSyncKill(uint32 syncKillEnable);
void   COUNTER2_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   COUNTER2_SetPWMDeadTime(uint32 deadTime);
void   COUNTER2_SetPWMInvert(uint32 mask);

void   COUNTER2_SetInterruptMode(uint32 interruptMask);
uint32 COUNTER2_GetInterruptSourceMasked(void);
uint32 COUNTER2_GetInterruptSource(void);
void   COUNTER2_ClearInterrupt(uint32 interruptMask);
void   COUNTER2_SetInterrupt(uint32 interruptMask);

void   COUNTER2_WriteCounter(uint32 count);
uint32 COUNTER2_ReadCounter(void);

uint32 COUNTER2_ReadCapture(void);
uint32 COUNTER2_ReadCaptureBuf(void);

void   COUNTER2_WritePeriod(uint32 period);
uint32 COUNTER2_ReadPeriod(void);
void   COUNTER2_WritePeriodBuf(uint32 periodBuf);
uint32 COUNTER2_ReadPeriodBuf(void);

void   COUNTER2_WriteCompare(uint32 compare);
uint32 COUNTER2_ReadCompare(void);
void   COUNTER2_WriteCompareBuf(uint32 compareBuf);
uint32 COUNTER2_ReadCompareBuf(void);

void   COUNTER2_SetPeriodSwap(uint32 swapEnable);
void   COUNTER2_SetCompareSwap(uint32 swapEnable);

void   COUNTER2_SetCaptureMode(uint32 triggerMode);
void   COUNTER2_SetReloadMode(uint32 triggerMode);
void   COUNTER2_SetStartMode(uint32 triggerMode);
void   COUNTER2_SetStopMode(uint32 triggerMode);
void   COUNTER2_SetCountMode(uint32 triggerMode);

void   COUNTER2_SaveConfig(void);
void   COUNTER2_RestoreConfig(void);
void   COUNTER2_Sleep(void);
void   COUNTER2_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define COUNTER2_BLOCK_CONTROL_REG              (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define COUNTER2_BLOCK_CONTROL_PTR              ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define COUNTER2_COMMAND_REG                    (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define COUNTER2_COMMAND_PTR                    ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define COUNTER2_INTRRUPT_CAUSE_REG             (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define COUNTER2_INTRRUPT_CAUSE_PTR             ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define COUNTER2_CONTROL_REG                    (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__CTRL )
#define COUNTER2_CONTROL_PTR                    ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__CTRL )
#define COUNTER2_STATUS_REG                     (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__STATUS )
#define COUNTER2_STATUS_PTR                     ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__STATUS )
#define COUNTER2_COUNTER_REG                    (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__COUNTER )
#define COUNTER2_COUNTER_PTR                    ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__COUNTER )
#define COUNTER2_COMP_CAP_REG                   (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__CC )
#define COUNTER2_COMP_CAP_PTR                   ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__CC )
#define COUNTER2_COMP_CAP_BUF_REG               (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__CC_BUFF )
#define COUNTER2_COMP_CAP_BUF_PTR               ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__CC_BUFF )
#define COUNTER2_PERIOD_REG                     (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__PERIOD )
#define COUNTER2_PERIOD_PTR                     ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__PERIOD )
#define COUNTER2_PERIOD_BUF_REG                 (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define COUNTER2_PERIOD_BUF_PTR                 ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define COUNTER2_TRIG_CONTROL0_REG              (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define COUNTER2_TRIG_CONTROL0_PTR              ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define COUNTER2_TRIG_CONTROL1_REG              (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define COUNTER2_TRIG_CONTROL1_PTR              ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define COUNTER2_TRIG_CONTROL2_REG              (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define COUNTER2_TRIG_CONTROL2_PTR              ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define COUNTER2_INTERRUPT_REQ_REG              (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__INTR )
#define COUNTER2_INTERRUPT_REQ_PTR              ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__INTR )
#define COUNTER2_INTERRUPT_SET_REG              (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__INTR_SET )
#define COUNTER2_INTERRUPT_SET_PTR              ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__INTR_SET )
#define COUNTER2_INTERRUPT_MASK_REG             (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__INTR_MASK )
#define COUNTER2_INTERRUPT_MASK_PTR             ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__INTR_MASK )
#define COUNTER2_INTERRUPT_MASKED_REG           (*(reg32 *) COUNTER2_cy_m0s8_tcpwm_1__INTR_MASKED )
#define COUNTER2_INTERRUPT_MASKED_PTR           ( (reg32 *) COUNTER2_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define COUNTER2_MASK                           ((uint32)COUNTER2_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define COUNTER2_RELOAD_CC_SHIFT                (0u)
#define COUNTER2_RELOAD_PERIOD_SHIFT            (1u)
#define COUNTER2_PWM_SYNC_KILL_SHIFT            (2u)
#define COUNTER2_PWM_STOP_KILL_SHIFT            (3u)
#define COUNTER2_PRESCALER_SHIFT                (8u)
#define COUNTER2_UPDOWN_SHIFT                   (16u)
#define COUNTER2_ONESHOT_SHIFT                  (18u)
#define COUNTER2_QUAD_MODE_SHIFT                (20u)
#define COUNTER2_INV_OUT_SHIFT                  (20u)
#define COUNTER2_INV_COMPL_OUT_SHIFT            (21u)
#define COUNTER2_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define COUNTER2_RELOAD_CC_MASK                 ((uint32)(COUNTER2_1BIT_MASK        <<  \
                                                                            COUNTER2_RELOAD_CC_SHIFT))
#define COUNTER2_RELOAD_PERIOD_MASK             ((uint32)(COUNTER2_1BIT_MASK        <<  \
                                                                            COUNTER2_RELOAD_PERIOD_SHIFT))
#define COUNTER2_PWM_SYNC_KILL_MASK             ((uint32)(COUNTER2_1BIT_MASK        <<  \
                                                                            COUNTER2_PWM_SYNC_KILL_SHIFT))
#define COUNTER2_PWM_STOP_KILL_MASK             ((uint32)(COUNTER2_1BIT_MASK        <<  \
                                                                            COUNTER2_PWM_STOP_KILL_SHIFT))
#define COUNTER2_PRESCALER_MASK                 ((uint32)(COUNTER2_8BIT_MASK        <<  \
                                                                            COUNTER2_PRESCALER_SHIFT))
#define COUNTER2_UPDOWN_MASK                    ((uint32)(COUNTER2_2BIT_MASK        <<  \
                                                                            COUNTER2_UPDOWN_SHIFT))
#define COUNTER2_ONESHOT_MASK                   ((uint32)(COUNTER2_1BIT_MASK        <<  \
                                                                            COUNTER2_ONESHOT_SHIFT))
#define COUNTER2_QUAD_MODE_MASK                 ((uint32)(COUNTER2_3BIT_MASK        <<  \
                                                                            COUNTER2_QUAD_MODE_SHIFT))
#define COUNTER2_INV_OUT_MASK                   ((uint32)(COUNTER2_2BIT_MASK        <<  \
                                                                            COUNTER2_INV_OUT_SHIFT))
#define COUNTER2_MODE_MASK                      ((uint32)(COUNTER2_3BIT_MASK        <<  \
                                                                            COUNTER2_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define COUNTER2_CAPTURE_SHIFT                  (0u)
#define COUNTER2_COUNT_SHIFT                    (2u)
#define COUNTER2_RELOAD_SHIFT                   (4u)
#define COUNTER2_STOP_SHIFT                     (6u)
#define COUNTER2_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define COUNTER2_CAPTURE_MASK                   ((uint32)(COUNTER2_2BIT_MASK        <<  \
                                                                  COUNTER2_CAPTURE_SHIFT))
#define COUNTER2_COUNT_MASK                     ((uint32)(COUNTER2_2BIT_MASK        <<  \
                                                                  COUNTER2_COUNT_SHIFT))
#define COUNTER2_RELOAD_MASK                    ((uint32)(COUNTER2_2BIT_MASK        <<  \
                                                                  COUNTER2_RELOAD_SHIFT))
#define COUNTER2_STOP_MASK                      ((uint32)(COUNTER2_2BIT_MASK        <<  \
                                                                  COUNTER2_STOP_SHIFT))
#define COUNTER2_START_MASK                     ((uint32)(COUNTER2_2BIT_MASK        <<  \
                                                                  COUNTER2_START_SHIFT))

/* MASK */
#define COUNTER2_1BIT_MASK                      ((uint32)0x01u)
#define COUNTER2_2BIT_MASK                      ((uint32)0x03u)
#define COUNTER2_3BIT_MASK                      ((uint32)0x07u)
#define COUNTER2_6BIT_MASK                      ((uint32)0x3Fu)
#define COUNTER2_8BIT_MASK                      ((uint32)0xFFu)
#define COUNTER2_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define COUNTER2_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define COUNTER2_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(COUNTER2_QUAD_ENCODING_MODES     << COUNTER2_QUAD_MODE_SHIFT))       |\
         ((uint32)(COUNTER2_CONFIG                  << COUNTER2_MODE_SHIFT)))

#define COUNTER2_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(COUNTER2_PWM_STOP_EVENT          << COUNTER2_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(COUNTER2_PWM_OUT_INVERT          << COUNTER2_INV_OUT_SHIFT))         |\
         ((uint32)(COUNTER2_PWM_OUT_N_INVERT        << COUNTER2_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(COUNTER2_PWM_MODE                << COUNTER2_MODE_SHIFT)))

#define COUNTER2_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(COUNTER2_PWM_RUN_MODE         << COUNTER2_ONESHOT_SHIFT))
            
#define COUNTER2_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(COUNTER2_PWM_ALIGN            << COUNTER2_UPDOWN_SHIFT))

#define COUNTER2_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(COUNTER2_PWM_KILL_EVENT      << COUNTER2_PWM_SYNC_KILL_SHIFT))

#define COUNTER2_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(COUNTER2_PWM_DEAD_TIME_CYCLE  << COUNTER2_PRESCALER_SHIFT))

#define COUNTER2_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(COUNTER2_PWM_PRESCALER        << COUNTER2_PRESCALER_SHIFT))

#define COUNTER2_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(COUNTER2_TC_PRESCALER            << COUNTER2_PRESCALER_SHIFT))       |\
         ((uint32)(COUNTER2_TC_COUNTER_MODE         << COUNTER2_UPDOWN_SHIFT))          |\
         ((uint32)(COUNTER2_TC_RUN_MODE             << COUNTER2_ONESHOT_SHIFT))         |\
         ((uint32)(COUNTER2_TC_COMP_CAP_MODE        << COUNTER2_MODE_SHIFT)))
        
#define COUNTER2_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(COUNTER2_QUAD_PHIA_SIGNAL_MODE   << COUNTER2_COUNT_SHIFT))           |\
         ((uint32)(COUNTER2_QUAD_INDEX_SIGNAL_MODE  << COUNTER2_RELOAD_SHIFT))          |\
         ((uint32)(COUNTER2_QUAD_STOP_SIGNAL_MODE   << COUNTER2_STOP_SHIFT))            |\
         ((uint32)(COUNTER2_QUAD_PHIB_SIGNAL_MODE   << COUNTER2_START_SHIFT)))

#define COUNTER2_PWM_SIGNALS_MODES                                                              \
        (((uint32)(COUNTER2_PWM_SWITCH_SIGNAL_MODE  << COUNTER2_CAPTURE_SHIFT))         |\
         ((uint32)(COUNTER2_PWM_COUNT_SIGNAL_MODE   << COUNTER2_COUNT_SHIFT))           |\
         ((uint32)(COUNTER2_PWM_RELOAD_SIGNAL_MODE  << COUNTER2_RELOAD_SHIFT))          |\
         ((uint32)(COUNTER2_PWM_STOP_SIGNAL_MODE    << COUNTER2_STOP_SHIFT))            |\
         ((uint32)(COUNTER2_PWM_START_SIGNAL_MODE   << COUNTER2_START_SHIFT)))

#define COUNTER2_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(COUNTER2_TC_CAPTURE_SIGNAL_MODE  << COUNTER2_CAPTURE_SHIFT))         |\
         ((uint32)(COUNTER2_TC_COUNT_SIGNAL_MODE    << COUNTER2_COUNT_SHIFT))           |\
         ((uint32)(COUNTER2_TC_RELOAD_SIGNAL_MODE   << COUNTER2_RELOAD_SHIFT))          |\
         ((uint32)(COUNTER2_TC_STOP_SIGNAL_MODE     << COUNTER2_STOP_SHIFT))            |\
         ((uint32)(COUNTER2_TC_START_SIGNAL_MODE    << COUNTER2_START_SHIFT)))
        
#define COUNTER2_TIMER_UPDOWN_CNT_USED                                                          \
                ((COUNTER2__COUNT_UPDOWN0 == COUNTER2_TC_COUNTER_MODE)                  ||\
                 (COUNTER2__COUNT_UPDOWN1 == COUNTER2_TC_COUNTER_MODE))

#define COUNTER2_PWM_UPDOWN_CNT_USED                                                            \
                ((COUNTER2__CENTER == COUNTER2_PWM_ALIGN)                               ||\
                 (COUNTER2__ASYMMETRIC == COUNTER2_PWM_ALIGN))               
        
#define COUNTER2_PWM_PR_INIT_VALUE              (1u)
#define COUNTER2_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_COUNTER2_H */

/* [] END OF FILE */
