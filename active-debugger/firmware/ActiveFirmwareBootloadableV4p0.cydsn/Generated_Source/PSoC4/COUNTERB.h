/*******************************************************************************
* File Name: COUNTERB.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the COUNTERB
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

#if !defined(CY_TCPWM_COUNTERB_H)
#define CY_TCPWM_COUNTERB_H


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
} COUNTERB_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  COUNTERB_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define COUNTERB_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define COUNTERB_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define COUNTERB_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define COUNTERB_QUAD_ENCODING_MODES            (0lu)
#define COUNTERB_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define COUNTERB_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define COUNTERB_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define COUNTERB_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define COUNTERB_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define COUNTERB_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define COUNTERB_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define COUNTERB_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define COUNTERB_TC_RUN_MODE                    (1lu)
#define COUNTERB_TC_COUNTER_MODE                (0lu)
#define COUNTERB_TC_COMP_CAP_MODE               (0lu)
#define COUNTERB_TC_PRESCALER                   (0lu)

/* Signal modes */
#define COUNTERB_TC_RELOAD_SIGNAL_MODE          (0lu)
#define COUNTERB_TC_COUNT_SIGNAL_MODE           (0lu)
#define COUNTERB_TC_START_SIGNAL_MODE           (0lu)
#define COUNTERB_TC_STOP_SIGNAL_MODE            (0lu)
#define COUNTERB_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define COUNTERB_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define COUNTERB_TC_COUNT_SIGNAL_PRESENT        (1lu)
#define COUNTERB_TC_START_SIGNAL_PRESENT        (0lu)
#define COUNTERB_TC_STOP_SIGNAL_PRESENT         (0lu)
#define COUNTERB_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define COUNTERB_TC_INTERRUPT_MASK              (0lu)

/* PWM Mode */
/* Parameters */
#define COUNTERB_PWM_KILL_EVENT                 (0lu)
#define COUNTERB_PWM_STOP_EVENT                 (0lu)
#define COUNTERB_PWM_MODE                       (4lu)
#define COUNTERB_PWM_OUT_N_INVERT               (0lu)
#define COUNTERB_PWM_OUT_INVERT                 (0lu)
#define COUNTERB_PWM_ALIGN                      (0lu)
#define COUNTERB_PWM_RUN_MODE                   (0lu)
#define COUNTERB_PWM_DEAD_TIME_CYCLE            (0lu)
#define COUNTERB_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define COUNTERB_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define COUNTERB_PWM_COUNT_SIGNAL_MODE          (3lu)
#define COUNTERB_PWM_START_SIGNAL_MODE          (0lu)
#define COUNTERB_PWM_STOP_SIGNAL_MODE           (0lu)
#define COUNTERB_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define COUNTERB_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define COUNTERB_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define COUNTERB_PWM_START_SIGNAL_PRESENT       (0lu)
#define COUNTERB_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define COUNTERB_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define COUNTERB_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define COUNTERB_TC_PERIOD_VALUE                (65535lu)
#define COUNTERB_TC_COMPARE_VALUE               (65535lu)
#define COUNTERB_TC_COMPARE_BUF_VALUE           (65535lu)
#define COUNTERB_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define COUNTERB_PWM_PERIOD_VALUE               (65535lu)
#define COUNTERB_PWM_PERIOD_BUF_VALUE           (65535lu)
#define COUNTERB_PWM_PERIOD_SWAP                (0lu)
#define COUNTERB_PWM_COMPARE_VALUE              (65535lu)
#define COUNTERB_PWM_COMPARE_BUF_VALUE          (65535lu)
#define COUNTERB_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define COUNTERB__LEFT 0
#define COUNTERB__RIGHT 1
#define COUNTERB__CENTER 2
#define COUNTERB__ASYMMETRIC 3

#define COUNTERB__X1 0
#define COUNTERB__X2 1
#define COUNTERB__X4 2

#define COUNTERB__PWM 4
#define COUNTERB__PWM_DT 5
#define COUNTERB__PWM_PR 6

#define COUNTERB__INVERSE 1
#define COUNTERB__DIRECT 0

#define COUNTERB__CAPTURE 2
#define COUNTERB__COMPARE 0

#define COUNTERB__TRIG_LEVEL 3
#define COUNTERB__TRIG_RISING 0
#define COUNTERB__TRIG_FALLING 1
#define COUNTERB__TRIG_BOTH 2

#define COUNTERB__INTR_MASK_TC 1
#define COUNTERB__INTR_MASK_CC_MATCH 2
#define COUNTERB__INTR_MASK_NONE 0
#define COUNTERB__INTR_MASK_TC_CC 3

#define COUNTERB__UNCONFIG 8
#define COUNTERB__TIMER 1
#define COUNTERB__QUAD 3
#define COUNTERB__PWM_SEL 7

#define COUNTERB__COUNT_UP 0
#define COUNTERB__COUNT_DOWN 1
#define COUNTERB__COUNT_UPDOWN0 2
#define COUNTERB__COUNT_UPDOWN1 3


/* Prescaler */
#define COUNTERB_PRESCALE_DIVBY1                ((uint32)(0u << COUNTERB_PRESCALER_SHIFT))
#define COUNTERB_PRESCALE_DIVBY2                ((uint32)(1u << COUNTERB_PRESCALER_SHIFT))
#define COUNTERB_PRESCALE_DIVBY4                ((uint32)(2u << COUNTERB_PRESCALER_SHIFT))
#define COUNTERB_PRESCALE_DIVBY8                ((uint32)(3u << COUNTERB_PRESCALER_SHIFT))
#define COUNTERB_PRESCALE_DIVBY16               ((uint32)(4u << COUNTERB_PRESCALER_SHIFT))
#define COUNTERB_PRESCALE_DIVBY32               ((uint32)(5u << COUNTERB_PRESCALER_SHIFT))
#define COUNTERB_PRESCALE_DIVBY64               ((uint32)(6u << COUNTERB_PRESCALER_SHIFT))
#define COUNTERB_PRESCALE_DIVBY128              ((uint32)(7u << COUNTERB_PRESCALER_SHIFT))

/* TCPWM set modes */
#define COUNTERB_MODE_TIMER_COMPARE             ((uint32)(COUNTERB__COMPARE         <<  \
                                                                  COUNTERB_MODE_SHIFT))
#define COUNTERB_MODE_TIMER_CAPTURE             ((uint32)(COUNTERB__CAPTURE         <<  \
                                                                  COUNTERB_MODE_SHIFT))
#define COUNTERB_MODE_QUAD                      ((uint32)(COUNTERB__QUAD            <<  \
                                                                  COUNTERB_MODE_SHIFT))
#define COUNTERB_MODE_PWM                       ((uint32)(COUNTERB__PWM             <<  \
                                                                  COUNTERB_MODE_SHIFT))
#define COUNTERB_MODE_PWM_DT                    ((uint32)(COUNTERB__PWM_DT          <<  \
                                                                  COUNTERB_MODE_SHIFT))
#define COUNTERB_MODE_PWM_PR                    ((uint32)(COUNTERB__PWM_PR          <<  \
                                                                  COUNTERB_MODE_SHIFT))

/* Quad Modes */
#define COUNTERB_MODE_X1                        ((uint32)(COUNTERB__X1              <<  \
                                                                  COUNTERB_QUAD_MODE_SHIFT))
#define COUNTERB_MODE_X2                        ((uint32)(COUNTERB__X2              <<  \
                                                                  COUNTERB_QUAD_MODE_SHIFT))
#define COUNTERB_MODE_X4                        ((uint32)(COUNTERB__X4              <<  \
                                                                  COUNTERB_QUAD_MODE_SHIFT))

/* Counter modes */
#define COUNTERB_COUNT_UP                       ((uint32)(COUNTERB__COUNT_UP        <<  \
                                                                  COUNTERB_UPDOWN_SHIFT))
#define COUNTERB_COUNT_DOWN                     ((uint32)(COUNTERB__COUNT_DOWN      <<  \
                                                                  COUNTERB_UPDOWN_SHIFT))
#define COUNTERB_COUNT_UPDOWN0                  ((uint32)(COUNTERB__COUNT_UPDOWN0   <<  \
                                                                  COUNTERB_UPDOWN_SHIFT))
#define COUNTERB_COUNT_UPDOWN1                  ((uint32)(COUNTERB__COUNT_UPDOWN1   <<  \
                                                                  COUNTERB_UPDOWN_SHIFT))

/* PWM output invert */
#define COUNTERB_INVERT_LINE                    ((uint32)(COUNTERB__INVERSE         <<  \
                                                                  COUNTERB_INV_OUT_SHIFT))
#define COUNTERB_INVERT_LINE_N                  ((uint32)(COUNTERB__INVERSE         <<  \
                                                                  COUNTERB_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define COUNTERB_TRIG_RISING                    ((uint32)COUNTERB__TRIG_RISING)
#define COUNTERB_TRIG_FALLING                   ((uint32)COUNTERB__TRIG_FALLING)
#define COUNTERB_TRIG_BOTH                      ((uint32)COUNTERB__TRIG_BOTH)
#define COUNTERB_TRIG_LEVEL                     ((uint32)COUNTERB__TRIG_LEVEL)

/* Interrupt mask */
#define COUNTERB_INTR_MASK_TC                   ((uint32)COUNTERB__INTR_MASK_TC)
#define COUNTERB_INTR_MASK_CC_MATCH             ((uint32)COUNTERB__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define COUNTERB_CC_MATCH_SET                   (0x00u)
#define COUNTERB_CC_MATCH_CLEAR                 (0x01u)
#define COUNTERB_CC_MATCH_INVERT                (0x02u)
#define COUNTERB_CC_MATCH_NO_CHANGE             (0x03u)
#define COUNTERB_OVERLOW_SET                    (0x00u)
#define COUNTERB_OVERLOW_CLEAR                  (0x04u)
#define COUNTERB_OVERLOW_INVERT                 (0x08u)
#define COUNTERB_OVERLOW_NO_CHANGE              (0x0Cu)
#define COUNTERB_UNDERFLOW_SET                  (0x00u)
#define COUNTERB_UNDERFLOW_CLEAR                (0x10u)
#define COUNTERB_UNDERFLOW_INVERT               (0x20u)
#define COUNTERB_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define COUNTERB_PWM_MODE_LEFT                  (COUNTERB_CC_MATCH_CLEAR        |   \
                                                         COUNTERB_OVERLOW_SET           |   \
                                                         COUNTERB_UNDERFLOW_NO_CHANGE)
#define COUNTERB_PWM_MODE_RIGHT                 (COUNTERB_CC_MATCH_SET          |   \
                                                         COUNTERB_OVERLOW_NO_CHANGE     |   \
                                                         COUNTERB_UNDERFLOW_CLEAR)
#define COUNTERB_PWM_MODE_ASYM                  (COUNTERB_CC_MATCH_INVERT       |   \
                                                         COUNTERB_OVERLOW_SET           |   \
                                                         COUNTERB_UNDERFLOW_CLEAR)

#if (COUNTERB_CY_TCPWM_V2)
    #if(COUNTERB_CY_TCPWM_4000)
        #define COUNTERB_PWM_MODE_CENTER                (COUNTERB_CC_MATCH_INVERT       |   \
                                                                 COUNTERB_OVERLOW_NO_CHANGE     |   \
                                                                 COUNTERB_UNDERFLOW_CLEAR)
    #else
        #define COUNTERB_PWM_MODE_CENTER                (COUNTERB_CC_MATCH_INVERT       |   \
                                                                 COUNTERB_OVERLOW_SET           |   \
                                                                 COUNTERB_UNDERFLOW_CLEAR)
    #endif /* (COUNTERB_CY_TCPWM_4000) */
#else
    #define COUNTERB_PWM_MODE_CENTER                (COUNTERB_CC_MATCH_INVERT       |   \
                                                             COUNTERB_OVERLOW_NO_CHANGE     |   \
                                                             COUNTERB_UNDERFLOW_CLEAR)
#endif /* (COUNTERB_CY_TCPWM_NEW) */

/* Command operations without condition */
#define COUNTERB_CMD_CAPTURE                    (0u)
#define COUNTERB_CMD_RELOAD                     (8u)
#define COUNTERB_CMD_STOP                       (16u)
#define COUNTERB_CMD_START                      (24u)

/* Status */
#define COUNTERB_STATUS_DOWN                    (1u)
#define COUNTERB_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   COUNTERB_Init(void);
void   COUNTERB_Enable(void);
void   COUNTERB_Start(void);
void   COUNTERB_Stop(void);

void   COUNTERB_SetMode(uint32 mode);
void   COUNTERB_SetCounterMode(uint32 counterMode);
void   COUNTERB_SetPWMMode(uint32 modeMask);
void   COUNTERB_SetQDMode(uint32 qdMode);

void   COUNTERB_SetPrescaler(uint32 prescaler);
void   COUNTERB_TriggerCommand(uint32 mask, uint32 command);
void   COUNTERB_SetOneShot(uint32 oneShotEnable);
uint32 COUNTERB_ReadStatus(void);

void   COUNTERB_SetPWMSyncKill(uint32 syncKillEnable);
void   COUNTERB_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   COUNTERB_SetPWMDeadTime(uint32 deadTime);
void   COUNTERB_SetPWMInvert(uint32 mask);

void   COUNTERB_SetInterruptMode(uint32 interruptMask);
uint32 COUNTERB_GetInterruptSourceMasked(void);
uint32 COUNTERB_GetInterruptSource(void);
void   COUNTERB_ClearInterrupt(uint32 interruptMask);
void   COUNTERB_SetInterrupt(uint32 interruptMask);

void   COUNTERB_WriteCounter(uint32 count);
uint32 COUNTERB_ReadCounter(void);

uint32 COUNTERB_ReadCapture(void);
uint32 COUNTERB_ReadCaptureBuf(void);

void   COUNTERB_WritePeriod(uint32 period);
uint32 COUNTERB_ReadPeriod(void);
void   COUNTERB_WritePeriodBuf(uint32 periodBuf);
uint32 COUNTERB_ReadPeriodBuf(void);

void   COUNTERB_WriteCompare(uint32 compare);
uint32 COUNTERB_ReadCompare(void);
void   COUNTERB_WriteCompareBuf(uint32 compareBuf);
uint32 COUNTERB_ReadCompareBuf(void);

void   COUNTERB_SetPeriodSwap(uint32 swapEnable);
void   COUNTERB_SetCompareSwap(uint32 swapEnable);

void   COUNTERB_SetCaptureMode(uint32 triggerMode);
void   COUNTERB_SetReloadMode(uint32 triggerMode);
void   COUNTERB_SetStartMode(uint32 triggerMode);
void   COUNTERB_SetStopMode(uint32 triggerMode);
void   COUNTERB_SetCountMode(uint32 triggerMode);

void   COUNTERB_SaveConfig(void);
void   COUNTERB_RestoreConfig(void);
void   COUNTERB_Sleep(void);
void   COUNTERB_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define COUNTERB_BLOCK_CONTROL_REG              (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define COUNTERB_BLOCK_CONTROL_PTR              ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define COUNTERB_COMMAND_REG                    (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define COUNTERB_COMMAND_PTR                    ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define COUNTERB_INTRRUPT_CAUSE_REG             (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define COUNTERB_INTRRUPT_CAUSE_PTR             ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define COUNTERB_CONTROL_REG                    (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__CTRL )
#define COUNTERB_CONTROL_PTR                    ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__CTRL )
#define COUNTERB_STATUS_REG                     (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__STATUS )
#define COUNTERB_STATUS_PTR                     ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__STATUS )
#define COUNTERB_COUNTER_REG                    (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__COUNTER )
#define COUNTERB_COUNTER_PTR                    ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__COUNTER )
#define COUNTERB_COMP_CAP_REG                   (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__CC )
#define COUNTERB_COMP_CAP_PTR                   ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__CC )
#define COUNTERB_COMP_CAP_BUF_REG               (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__CC_BUFF )
#define COUNTERB_COMP_CAP_BUF_PTR               ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__CC_BUFF )
#define COUNTERB_PERIOD_REG                     (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__PERIOD )
#define COUNTERB_PERIOD_PTR                     ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__PERIOD )
#define COUNTERB_PERIOD_BUF_REG                 (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define COUNTERB_PERIOD_BUF_PTR                 ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define COUNTERB_TRIG_CONTROL0_REG              (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define COUNTERB_TRIG_CONTROL0_PTR              ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define COUNTERB_TRIG_CONTROL1_REG              (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define COUNTERB_TRIG_CONTROL1_PTR              ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define COUNTERB_TRIG_CONTROL2_REG              (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define COUNTERB_TRIG_CONTROL2_PTR              ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define COUNTERB_INTERRUPT_REQ_REG              (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__INTR )
#define COUNTERB_INTERRUPT_REQ_PTR              ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__INTR )
#define COUNTERB_INTERRUPT_SET_REG              (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__INTR_SET )
#define COUNTERB_INTERRUPT_SET_PTR              ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__INTR_SET )
#define COUNTERB_INTERRUPT_MASK_REG             (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__INTR_MASK )
#define COUNTERB_INTERRUPT_MASK_PTR             ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__INTR_MASK )
#define COUNTERB_INTERRUPT_MASKED_REG           (*(reg32 *) COUNTERB_cy_m0s8_tcpwm_1__INTR_MASKED )
#define COUNTERB_INTERRUPT_MASKED_PTR           ( (reg32 *) COUNTERB_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define COUNTERB_MASK                           ((uint32)COUNTERB_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define COUNTERB_RELOAD_CC_SHIFT                (0u)
#define COUNTERB_RELOAD_PERIOD_SHIFT            (1u)
#define COUNTERB_PWM_SYNC_KILL_SHIFT            (2u)
#define COUNTERB_PWM_STOP_KILL_SHIFT            (3u)
#define COUNTERB_PRESCALER_SHIFT                (8u)
#define COUNTERB_UPDOWN_SHIFT                   (16u)
#define COUNTERB_ONESHOT_SHIFT                  (18u)
#define COUNTERB_QUAD_MODE_SHIFT                (20u)
#define COUNTERB_INV_OUT_SHIFT                  (20u)
#define COUNTERB_INV_COMPL_OUT_SHIFT            (21u)
#define COUNTERB_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define COUNTERB_RELOAD_CC_MASK                 ((uint32)(COUNTERB_1BIT_MASK        <<  \
                                                                            COUNTERB_RELOAD_CC_SHIFT))
#define COUNTERB_RELOAD_PERIOD_MASK             ((uint32)(COUNTERB_1BIT_MASK        <<  \
                                                                            COUNTERB_RELOAD_PERIOD_SHIFT))
#define COUNTERB_PWM_SYNC_KILL_MASK             ((uint32)(COUNTERB_1BIT_MASK        <<  \
                                                                            COUNTERB_PWM_SYNC_KILL_SHIFT))
#define COUNTERB_PWM_STOP_KILL_MASK             ((uint32)(COUNTERB_1BIT_MASK        <<  \
                                                                            COUNTERB_PWM_STOP_KILL_SHIFT))
#define COUNTERB_PRESCALER_MASK                 ((uint32)(COUNTERB_8BIT_MASK        <<  \
                                                                            COUNTERB_PRESCALER_SHIFT))
#define COUNTERB_UPDOWN_MASK                    ((uint32)(COUNTERB_2BIT_MASK        <<  \
                                                                            COUNTERB_UPDOWN_SHIFT))
#define COUNTERB_ONESHOT_MASK                   ((uint32)(COUNTERB_1BIT_MASK        <<  \
                                                                            COUNTERB_ONESHOT_SHIFT))
#define COUNTERB_QUAD_MODE_MASK                 ((uint32)(COUNTERB_3BIT_MASK        <<  \
                                                                            COUNTERB_QUAD_MODE_SHIFT))
#define COUNTERB_INV_OUT_MASK                   ((uint32)(COUNTERB_2BIT_MASK        <<  \
                                                                            COUNTERB_INV_OUT_SHIFT))
#define COUNTERB_MODE_MASK                      ((uint32)(COUNTERB_3BIT_MASK        <<  \
                                                                            COUNTERB_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define COUNTERB_CAPTURE_SHIFT                  (0u)
#define COUNTERB_COUNT_SHIFT                    (2u)
#define COUNTERB_RELOAD_SHIFT                   (4u)
#define COUNTERB_STOP_SHIFT                     (6u)
#define COUNTERB_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define COUNTERB_CAPTURE_MASK                   ((uint32)(COUNTERB_2BIT_MASK        <<  \
                                                                  COUNTERB_CAPTURE_SHIFT))
#define COUNTERB_COUNT_MASK                     ((uint32)(COUNTERB_2BIT_MASK        <<  \
                                                                  COUNTERB_COUNT_SHIFT))
#define COUNTERB_RELOAD_MASK                    ((uint32)(COUNTERB_2BIT_MASK        <<  \
                                                                  COUNTERB_RELOAD_SHIFT))
#define COUNTERB_STOP_MASK                      ((uint32)(COUNTERB_2BIT_MASK        <<  \
                                                                  COUNTERB_STOP_SHIFT))
#define COUNTERB_START_MASK                     ((uint32)(COUNTERB_2BIT_MASK        <<  \
                                                                  COUNTERB_START_SHIFT))

/* MASK */
#define COUNTERB_1BIT_MASK                      ((uint32)0x01u)
#define COUNTERB_2BIT_MASK                      ((uint32)0x03u)
#define COUNTERB_3BIT_MASK                      ((uint32)0x07u)
#define COUNTERB_6BIT_MASK                      ((uint32)0x3Fu)
#define COUNTERB_8BIT_MASK                      ((uint32)0xFFu)
#define COUNTERB_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define COUNTERB_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define COUNTERB_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(COUNTERB_QUAD_ENCODING_MODES     << COUNTERB_QUAD_MODE_SHIFT))       |\
         ((uint32)(COUNTERB_CONFIG                  << COUNTERB_MODE_SHIFT)))

#define COUNTERB_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(COUNTERB_PWM_STOP_EVENT          << COUNTERB_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(COUNTERB_PWM_OUT_INVERT          << COUNTERB_INV_OUT_SHIFT))         |\
         ((uint32)(COUNTERB_PWM_OUT_N_INVERT        << COUNTERB_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(COUNTERB_PWM_MODE                << COUNTERB_MODE_SHIFT)))

#define COUNTERB_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(COUNTERB_PWM_RUN_MODE         << COUNTERB_ONESHOT_SHIFT))
            
#define COUNTERB_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(COUNTERB_PWM_ALIGN            << COUNTERB_UPDOWN_SHIFT))

#define COUNTERB_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(COUNTERB_PWM_KILL_EVENT      << COUNTERB_PWM_SYNC_KILL_SHIFT))

#define COUNTERB_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(COUNTERB_PWM_DEAD_TIME_CYCLE  << COUNTERB_PRESCALER_SHIFT))

#define COUNTERB_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(COUNTERB_PWM_PRESCALER        << COUNTERB_PRESCALER_SHIFT))

#define COUNTERB_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(COUNTERB_TC_PRESCALER            << COUNTERB_PRESCALER_SHIFT))       |\
         ((uint32)(COUNTERB_TC_COUNTER_MODE         << COUNTERB_UPDOWN_SHIFT))          |\
         ((uint32)(COUNTERB_TC_RUN_MODE             << COUNTERB_ONESHOT_SHIFT))         |\
         ((uint32)(COUNTERB_TC_COMP_CAP_MODE        << COUNTERB_MODE_SHIFT)))
        
#define COUNTERB_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(COUNTERB_QUAD_PHIA_SIGNAL_MODE   << COUNTERB_COUNT_SHIFT))           |\
         ((uint32)(COUNTERB_QUAD_INDEX_SIGNAL_MODE  << COUNTERB_RELOAD_SHIFT))          |\
         ((uint32)(COUNTERB_QUAD_STOP_SIGNAL_MODE   << COUNTERB_STOP_SHIFT))            |\
         ((uint32)(COUNTERB_QUAD_PHIB_SIGNAL_MODE   << COUNTERB_START_SHIFT)))

#define COUNTERB_PWM_SIGNALS_MODES                                                              \
        (((uint32)(COUNTERB_PWM_SWITCH_SIGNAL_MODE  << COUNTERB_CAPTURE_SHIFT))         |\
         ((uint32)(COUNTERB_PWM_COUNT_SIGNAL_MODE   << COUNTERB_COUNT_SHIFT))           |\
         ((uint32)(COUNTERB_PWM_RELOAD_SIGNAL_MODE  << COUNTERB_RELOAD_SHIFT))          |\
         ((uint32)(COUNTERB_PWM_STOP_SIGNAL_MODE    << COUNTERB_STOP_SHIFT))            |\
         ((uint32)(COUNTERB_PWM_START_SIGNAL_MODE   << COUNTERB_START_SHIFT)))

#define COUNTERB_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(COUNTERB_TC_CAPTURE_SIGNAL_MODE  << COUNTERB_CAPTURE_SHIFT))         |\
         ((uint32)(COUNTERB_TC_COUNT_SIGNAL_MODE    << COUNTERB_COUNT_SHIFT))           |\
         ((uint32)(COUNTERB_TC_RELOAD_SIGNAL_MODE   << COUNTERB_RELOAD_SHIFT))          |\
         ((uint32)(COUNTERB_TC_STOP_SIGNAL_MODE     << COUNTERB_STOP_SHIFT))            |\
         ((uint32)(COUNTERB_TC_START_SIGNAL_MODE    << COUNTERB_START_SHIFT)))
        
#define COUNTERB_TIMER_UPDOWN_CNT_USED                                                          \
                ((COUNTERB__COUNT_UPDOWN0 == COUNTERB_TC_COUNTER_MODE)                  ||\
                 (COUNTERB__COUNT_UPDOWN1 == COUNTERB_TC_COUNTER_MODE))

#define COUNTERB_PWM_UPDOWN_CNT_USED                                                            \
                ((COUNTERB__CENTER == COUNTERB_PWM_ALIGN)                               ||\
                 (COUNTERB__ASYMMETRIC == COUNTERB_PWM_ALIGN))               
        
#define COUNTERB_PWM_PR_INIT_VALUE              (1u)
#define COUNTERB_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_COUNTERB_H */

/* [] END OF FILE */
