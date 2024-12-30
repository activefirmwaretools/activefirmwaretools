/*******************************************************************************
* File Name: TimestampTimer.h
* Version 2.80
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_TIMER_TimestampTimer_H)
#define CY_TIMER_TimestampTimer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 TimestampTimer_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_80 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define TimestampTimer_Resolution                 32u
#define TimestampTimer_UsingFixedFunction         0u
#define TimestampTimer_UsingHWCaptureCounter      0u
#define TimestampTimer_SoftwareCaptureMode        0u
#define TimestampTimer_SoftwareTriggerMode        0u
#define TimestampTimer_UsingHWEnable              0u
#define TimestampTimer_EnableTriggerMode          0u
#define TimestampTimer_InterruptOnCaptureCount    0u
#define TimestampTimer_RunModeUsed                0u
#define TimestampTimer_ControlRegRemoved          0u

#if defined(TimestampTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define TimestampTimer_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (TimestampTimer_UsingFixedFunction)
    #define TimestampTimer_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define TimestampTimer_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End TimestampTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!TimestampTimer_UsingFixedFunction)

        uint32 TimerUdb;
        uint8 InterruptMaskValue;
        #if (TimestampTimer_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!TimestampTimer_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}TimestampTimer_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    TimestampTimer_Start(void) ;
void    TimestampTimer_Stop(void) ;

void    TimestampTimer_SetInterruptMode(uint8 interruptMode) ;
uint8   TimestampTimer_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define TimestampTimer_GetInterruptSource() TimestampTimer_ReadStatusRegister()

#if(!TimestampTimer_UDB_CONTROL_REG_REMOVED)
    uint8   TimestampTimer_ReadControlRegister(void) ;
    void    TimestampTimer_WriteControlRegister(uint8 control) ;
#endif /* (!TimestampTimer_UDB_CONTROL_REG_REMOVED) */

uint32  TimestampTimer_ReadPeriod(void) ;
void    TimestampTimer_WritePeriod(uint32 period) ;
uint32  TimestampTimer_ReadCounter(void) ;
void    TimestampTimer_WriteCounter(uint32 counter) ;
uint32  TimestampTimer_ReadCapture(void) ;
void    TimestampTimer_SoftwareCapture(void) ;

#if(!TimestampTimer_UsingFixedFunction) /* UDB Prototypes */
    #if (TimestampTimer_SoftwareCaptureMode)
        void    TimestampTimer_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!TimestampTimer_UsingFixedFunction) */

    #if (TimestampTimer_SoftwareTriggerMode)
        void    TimestampTimer_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (TimestampTimer_SoftwareTriggerMode) */

    #if (TimestampTimer_EnableTriggerMode)
        void    TimestampTimer_EnableTrigger(void) ;
        void    TimestampTimer_DisableTrigger(void) ;
    #endif /* (TimestampTimer_EnableTriggerMode) */


    #if(TimestampTimer_InterruptOnCaptureCount)
        void    TimestampTimer_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (TimestampTimer_InterruptOnCaptureCount) */

    #if (TimestampTimer_UsingHWCaptureCounter)
        void    TimestampTimer_SetCaptureCount(uint8 captureCount) ;
        uint8   TimestampTimer_ReadCaptureCount(void) ;
    #endif /* (TimestampTimer_UsingHWCaptureCounter) */

    void TimestampTimer_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void TimestampTimer_Init(void)          ;
void TimestampTimer_Enable(void)        ;
void TimestampTimer_SaveConfig(void)    ;
void TimestampTimer_RestoreConfig(void) ;
void TimestampTimer_Sleep(void)         ;
void TimestampTimer_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define TimestampTimer__B_TIMER__CM_NONE 0
#define TimestampTimer__B_TIMER__CM_RISINGEDGE 1
#define TimestampTimer__B_TIMER__CM_FALLINGEDGE 2
#define TimestampTimer__B_TIMER__CM_EITHEREDGE 3
#define TimestampTimer__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define TimestampTimer__B_TIMER__TM_NONE 0x00u
#define TimestampTimer__B_TIMER__TM_RISINGEDGE 0x04u
#define TimestampTimer__B_TIMER__TM_FALLINGEDGE 0x08u
#define TimestampTimer__B_TIMER__TM_EITHEREDGE 0x0Cu
#define TimestampTimer__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define TimestampTimer_INIT_PERIOD             4294967295u
#define TimestampTimer_INIT_CAPTURE_MODE       ((uint8)((uint8)0u << TimestampTimer_CTRL_CAP_MODE_SHIFT))
#define TimestampTimer_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << TimestampTimer_CTRL_TRIG_MODE_SHIFT))
#if (TimestampTimer_UsingFixedFunction)
    #define TimestampTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << TimestampTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << TimestampTimer_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define TimestampTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << TimestampTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << TimestampTimer_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << TimestampTimer_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (TimestampTimer_UsingFixedFunction) */
#define TimestampTimer_INIT_CAPTURE_COUNT      (2u)
#define TimestampTimer_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << TimestampTimer_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (TimestampTimer_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define TimestampTimer_STATUS         (*(reg8 *) TimestampTimer_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define TimestampTimer_STATUS_MASK    (*(reg8 *) TimestampTimer_TimerHW__SR0 )
    #define TimestampTimer_CONTROL        (*(reg8 *) TimestampTimer_TimerHW__CFG0)
    #define TimestampTimer_CONTROL2       (*(reg8 *) TimestampTimer_TimerHW__CFG1)
    #define TimestampTimer_CONTROL2_PTR   ( (reg8 *) TimestampTimer_TimerHW__CFG1)
    #define TimestampTimer_RT1            (*(reg8 *) TimestampTimer_TimerHW__RT1)
    #define TimestampTimer_RT1_PTR        ( (reg8 *) TimestampTimer_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define TimestampTimer_CONTROL3       (*(reg8 *) TimestampTimer_TimerHW__CFG2)
        #define TimestampTimer_CONTROL3_PTR   ( (reg8 *) TimestampTimer_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define TimestampTimer_GLOBAL_ENABLE  (*(reg8 *) TimestampTimer_TimerHW__PM_ACT_CFG)
    #define TimestampTimer_GLOBAL_STBY_ENABLE  (*(reg8 *) TimestampTimer_TimerHW__PM_STBY_CFG)

    #define TimestampTimer_CAPTURE_LSB         (* (reg16 *) TimestampTimer_TimerHW__CAP0 )
    #define TimestampTimer_CAPTURE_LSB_PTR       ((reg16 *) TimestampTimer_TimerHW__CAP0 )
    #define TimestampTimer_PERIOD_LSB          (* (reg16 *) TimestampTimer_TimerHW__PER0 )
    #define TimestampTimer_PERIOD_LSB_PTR        ((reg16 *) TimestampTimer_TimerHW__PER0 )
    #define TimestampTimer_COUNTER_LSB         (* (reg16 *) TimestampTimer_TimerHW__CNT_CMP0 )
    #define TimestampTimer_COUNTER_LSB_PTR       ((reg16 *) TimestampTimer_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define TimestampTimer_BLOCK_EN_MASK                     TimestampTimer_TimerHW__PM_ACT_MSK
    #define TimestampTimer_BLOCK_STBY_EN_MASK                TimestampTimer_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define TimestampTimer_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define TimestampTimer_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define TimestampTimer_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define TimestampTimer_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define TimestampTimer_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define TimestampTimer_CTRL_ENABLE                        ((uint8)((uint8)0x01u << TimestampTimer_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define TimestampTimer_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define TimestampTimer_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << TimestampTimer_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define TimestampTimer_CTRL_MODE_SHIFT                 0x01u
        #define TimestampTimer_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << TimestampTimer_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define TimestampTimer_CTRL_RCOD_SHIFT        0x02u
        #define TimestampTimer_CTRL_ENBL_SHIFT        0x00u
        #define TimestampTimer_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define TimestampTimer_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << TimestampTimer_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define TimestampTimer_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << TimestampTimer_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define TimestampTimer_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << TimestampTimer_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define TimestampTimer_CTRL_RCOD       ((uint8)((uint8)0x03u << TimestampTimer_CTRL_RCOD_SHIFT))
        #define TimestampTimer_CTRL_ENBL       ((uint8)((uint8)0x80u << TimestampTimer_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define TimestampTimer_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define TimestampTimer_RT1_MASK                        ((uint8)((uint8)0x03u << TimestampTimer_RT1_SHIFT))
    #define TimestampTimer_SYNC                            ((uint8)((uint8)0x03u << TimestampTimer_RT1_SHIFT))
    #define TimestampTimer_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define TimestampTimer_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << TimestampTimer_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define TimestampTimer_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << TimestampTimer_SYNCDSI_SHIFT))

    #define TimestampTimer_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << TimestampTimer_CTRL_MODE_SHIFT))
    #define TimestampTimer_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << TimestampTimer_CTRL_MODE_SHIFT))
    #define TimestampTimer_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << TimestampTimer_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define TimestampTimer_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define TimestampTimer_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define TimestampTimer_STATUS_TC_INT_MASK_SHIFT        (TimestampTimer_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define TimestampTimer_STATUS_CAPTURE_INT_MASK_SHIFT   (TimestampTimer_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define TimestampTimer_STATUS_TC                       ((uint8)((uint8)0x01u << TimestampTimer_STATUS_TC_SHIFT))
    #define TimestampTimer_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << TimestampTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define TimestampTimer_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << TimestampTimer_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define TimestampTimer_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << TimestampTimer_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define TimestampTimer_STATUS              (* (reg8 *) TimestampTimer_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define TimestampTimer_STATUS_MASK         (* (reg8 *) TimestampTimer_TimerUDB_rstSts_stsreg__MASK_REG)
    #define TimestampTimer_STATUS_AUX_CTRL     (* (reg8 *) TimestampTimer_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define TimestampTimer_CONTROL             (* (reg8 *) TimestampTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(TimestampTimer_Resolution <= 8u) /* 8-bit Timer */
        #define TimestampTimer_CAPTURE_LSB         (* (reg8 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define TimestampTimer_CAPTURE_LSB_PTR       ((reg8 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define TimestampTimer_PERIOD_LSB          (* (reg8 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define TimestampTimer_PERIOD_LSB_PTR        ((reg8 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define TimestampTimer_COUNTER_LSB         (* (reg8 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
        #define TimestampTimer_COUNTER_LSB_PTR       ((reg8 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
    #elif(TimestampTimer_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define TimestampTimer_CAPTURE_LSB         (* (reg16 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define TimestampTimer_CAPTURE_LSB_PTR       ((reg16 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define TimestampTimer_PERIOD_LSB          (* (reg16 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define TimestampTimer_PERIOD_LSB_PTR        ((reg16 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define TimestampTimer_COUNTER_LSB         (* (reg16 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
            #define TimestampTimer_COUNTER_LSB_PTR       ((reg16 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define TimestampTimer_CAPTURE_LSB         (* (reg16 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__16BIT_F0_REG )
            #define TimestampTimer_CAPTURE_LSB_PTR       ((reg16 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__16BIT_F0_REG )
            #define TimestampTimer_PERIOD_LSB          (* (reg16 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__16BIT_D0_REG )
            #define TimestampTimer_PERIOD_LSB_PTR        ((reg16 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__16BIT_D0_REG )
            #define TimestampTimer_COUNTER_LSB         (* (reg16 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__16BIT_A0_REG )
            #define TimestampTimer_COUNTER_LSB_PTR       ((reg16 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(TimestampTimer_Resolution <= 24u)/* 24-bit Timer */
        #define TimestampTimer_CAPTURE_LSB         (* (reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define TimestampTimer_CAPTURE_LSB_PTR       ((reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define TimestampTimer_PERIOD_LSB          (* (reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define TimestampTimer_PERIOD_LSB_PTR        ((reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define TimestampTimer_COUNTER_LSB         (* (reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
        #define TimestampTimer_COUNTER_LSB_PTR       ((reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define TimestampTimer_CAPTURE_LSB         (* (reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define TimestampTimer_CAPTURE_LSB_PTR       ((reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define TimestampTimer_PERIOD_LSB          (* (reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define TimestampTimer_PERIOD_LSB_PTR        ((reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define TimestampTimer_COUNTER_LSB         (* (reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
            #define TimestampTimer_COUNTER_LSB_PTR       ((reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define TimestampTimer_CAPTURE_LSB         (* (reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__32BIT_F0_REG )
            #define TimestampTimer_CAPTURE_LSB_PTR       ((reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__32BIT_F0_REG )
            #define TimestampTimer_PERIOD_LSB          (* (reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__32BIT_D0_REG )
            #define TimestampTimer_PERIOD_LSB_PTR        ((reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__32BIT_D0_REG )
            #define TimestampTimer_COUNTER_LSB         (* (reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__32BIT_A0_REG )
            #define TimestampTimer_COUNTER_LSB_PTR       ((reg32 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define TimestampTimer_COUNTER_LSB_PTR_8BIT       ((reg8 *) TimestampTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
    
    #if (TimestampTimer_UsingHWCaptureCounter)
        #define TimestampTimer_CAP_COUNT              (*(reg8 *) TimestampTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define TimestampTimer_CAP_COUNT_PTR          ( (reg8 *) TimestampTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define TimestampTimer_CAPTURE_COUNT_CTRL     (*(reg8 *) TimestampTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define TimestampTimer_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) TimestampTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (TimestampTimer_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define TimestampTimer_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define TimestampTimer_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define TimestampTimer_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define TimestampTimer_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define TimestampTimer_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define TimestampTimer_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << TimestampTimer_CTRL_INTCNT_SHIFT))
    #define TimestampTimer_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << TimestampTimer_CTRL_TRIG_MODE_SHIFT))
    #define TimestampTimer_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << TimestampTimer_CTRL_TRIG_EN_SHIFT))
    #define TimestampTimer_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << TimestampTimer_CTRL_CAP_MODE_SHIFT))
    #define TimestampTimer_CTRL_ENABLE                    ((uint8)((uint8)0x01u << TimestampTimer_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define TimestampTimer_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define TimestampTimer_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define TimestampTimer_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define TimestampTimer_STATUS_TC_INT_MASK_SHIFT       TimestampTimer_STATUS_TC_SHIFT
    #define TimestampTimer_STATUS_CAPTURE_INT_MASK_SHIFT  TimestampTimer_STATUS_CAPTURE_SHIFT
    #define TimestampTimer_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define TimestampTimer_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define TimestampTimer_STATUS_FIFOFULL_INT_MASK_SHIFT TimestampTimer_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define TimestampTimer_STATUS_TC                      ((uint8)((uint8)0x01u << TimestampTimer_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define TimestampTimer_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << TimestampTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TimestampTimer_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << TimestampTimer_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TimestampTimer_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << TimestampTimer_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define TimestampTimer_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << TimestampTimer_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define TimestampTimer_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << TimestampTimer_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define TimestampTimer_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << TimestampTimer_STATUS_FIFOFULL_SHIFT))

    #define TimestampTimer_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define TimestampTimer_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define TimestampTimer_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define TimestampTimer_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define TimestampTimer_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define TimestampTimer_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_TIMER_TimestampTimer_H */


/* [] END OF FILE */
