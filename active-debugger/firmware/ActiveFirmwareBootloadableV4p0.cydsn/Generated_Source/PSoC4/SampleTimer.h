/*******************************************************************************
* File Name: SampleTimer.h
* Version 2.70
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_Timer_v2_60_SampleTimer_H)
#define CY_Timer_v2_60_SampleTimer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 SampleTimer_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_70 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define SampleTimer_Resolution                 32u
#define SampleTimer_UsingFixedFunction         0u
#define SampleTimer_UsingHWCaptureCounter      0u
#define SampleTimer_SoftwareCaptureMode        0u
#define SampleTimer_SoftwareTriggerMode        0u
#define SampleTimer_UsingHWEnable              0u
#define SampleTimer_EnableTriggerMode          0u
#define SampleTimer_InterruptOnCaptureCount    0u
#define SampleTimer_RunModeUsed                0u
#define SampleTimer_ControlRegRemoved          0u

#if defined(SampleTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG)
    #define SampleTimer_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (SampleTimer_UsingFixedFunction)
    #define SampleTimer_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define SampleTimer_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End SampleTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!SampleTimer_UsingFixedFunction)

        uint32 TimerUdb;
        uint8 InterruptMaskValue;
        #if (SampleTimer_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!SampleTimer_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}SampleTimer_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    SampleTimer_Start(void) ;
void    SampleTimer_Stop(void) ;

void    SampleTimer_SetInterruptMode(uint8 interruptMode) ;
uint8   SampleTimer_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define SampleTimer_GetInterruptSource() SampleTimer_ReadStatusRegister()

#if(!SampleTimer_UDB_CONTROL_REG_REMOVED)
    uint8   SampleTimer_ReadControlRegister(void) ;
    void    SampleTimer_WriteControlRegister(uint8 control) ;
#endif /* (!SampleTimer_UDB_CONTROL_REG_REMOVED) */

uint32  SampleTimer_ReadPeriod(void) ;
void    SampleTimer_WritePeriod(uint32 period) ;
uint32  SampleTimer_ReadCounter(void) ;
void    SampleTimer_WriteCounter(uint32 counter) ;
uint32  SampleTimer_ReadCapture(void) ;
void    SampleTimer_SoftwareCapture(void) ;

#if(!SampleTimer_UsingFixedFunction) /* UDB Prototypes */
    #if (SampleTimer_SoftwareCaptureMode)
        void    SampleTimer_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!SampleTimer_UsingFixedFunction) */

    #if (SampleTimer_SoftwareTriggerMode)
        void    SampleTimer_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (SampleTimer_SoftwareTriggerMode) */

    #if (SampleTimer_EnableTriggerMode)
        void    SampleTimer_EnableTrigger(void) ;
        void    SampleTimer_DisableTrigger(void) ;
    #endif /* (SampleTimer_EnableTriggerMode) */


    #if(SampleTimer_InterruptOnCaptureCount)
        void    SampleTimer_SetInterruptCount(uint8 interruptCount) ;
    #endif /* (SampleTimer_InterruptOnCaptureCount) */

    #if (SampleTimer_UsingHWCaptureCounter)
        void    SampleTimer_SetCaptureCount(uint8 captureCount) ;
        uint8   SampleTimer_ReadCaptureCount(void) ;
    #endif /* (SampleTimer_UsingHWCaptureCounter) */

    void SampleTimer_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void SampleTimer_Init(void)          ;
void SampleTimer_Enable(void)        ;
void SampleTimer_SaveConfig(void)    ;
void SampleTimer_RestoreConfig(void) ;
void SampleTimer_Sleep(void)         ;
void SampleTimer_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define SampleTimer__B_TIMER__CM_NONE 0
#define SampleTimer__B_TIMER__CM_RISINGEDGE 1
#define SampleTimer__B_TIMER__CM_FALLINGEDGE 2
#define SampleTimer__B_TIMER__CM_EITHEREDGE 3
#define SampleTimer__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define SampleTimer__B_TIMER__TM_NONE 0x00u
#define SampleTimer__B_TIMER__TM_RISINGEDGE 0x04u
#define SampleTimer__B_TIMER__TM_FALLINGEDGE 0x08u
#define SampleTimer__B_TIMER__TM_EITHEREDGE 0x0Cu
#define SampleTimer__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define SampleTimer_INIT_PERIOD             4294967295u
#define SampleTimer_INIT_CAPTURE_MODE       ((uint8)((uint8)0u << SampleTimer_CTRL_CAP_MODE_SHIFT))
#define SampleTimer_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << SampleTimer_CTRL_TRIG_MODE_SHIFT))
#if (SampleTimer_UsingFixedFunction)
    #define SampleTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << SampleTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << SampleTimer_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define SampleTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << SampleTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << SampleTimer_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << SampleTimer_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (SampleTimer_UsingFixedFunction) */
#define SampleTimer_INIT_CAPTURE_COUNT      (2u)
#define SampleTimer_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << SampleTimer_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (SampleTimer_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define SampleTimer_STATUS         (*(reg8 *) SampleTimer_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define SampleTimer_STATUS_MASK    (*(reg8 *) SampleTimer_TimerHW__SR0 )
    #define SampleTimer_CONTROL        (*(reg8 *) SampleTimer_TimerHW__CFG0)
    #define SampleTimer_CONTROL2       (*(reg8 *) SampleTimer_TimerHW__CFG1)
    #define SampleTimer_CONTROL2_PTR   ( (reg8 *) SampleTimer_TimerHW__CFG1)
    #define SampleTimer_RT1            (*(reg8 *) SampleTimer_TimerHW__RT1)
    #define SampleTimer_RT1_PTR        ( (reg8 *) SampleTimer_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define SampleTimer_CONTROL3       (*(reg8 *) SampleTimer_TimerHW__CFG2)
        #define SampleTimer_CONTROL3_PTR   ( (reg8 *) SampleTimer_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define SampleTimer_GLOBAL_ENABLE  (*(reg8 *) SampleTimer_TimerHW__PM_ACT_CFG)
    #define SampleTimer_GLOBAL_STBY_ENABLE  (*(reg8 *) SampleTimer_TimerHW__PM_STBY_CFG)

    #define SampleTimer_CAPTURE_LSB         (* (reg16 *) SampleTimer_TimerHW__CAP0 )
    #define SampleTimer_CAPTURE_LSB_PTR       ((reg16 *) SampleTimer_TimerHW__CAP0 )
    #define SampleTimer_PERIOD_LSB          (* (reg16 *) SampleTimer_TimerHW__PER0 )
    #define SampleTimer_PERIOD_LSB_PTR        ((reg16 *) SampleTimer_TimerHW__PER0 )
    #define SampleTimer_COUNTER_LSB         (* (reg16 *) SampleTimer_TimerHW__CNT_CMP0 )
    #define SampleTimer_COUNTER_LSB_PTR       ((reg16 *) SampleTimer_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define SampleTimer_BLOCK_EN_MASK                     SampleTimer_TimerHW__PM_ACT_MSK
    #define SampleTimer_BLOCK_STBY_EN_MASK                SampleTimer_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define SampleTimer_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define SampleTimer_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define SampleTimer_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define SampleTimer_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define SampleTimer_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define SampleTimer_CTRL_ENABLE                        ((uint8)((uint8)0x01u << SampleTimer_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define SampleTimer_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define SampleTimer_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << SampleTimer_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define SampleTimer_CTRL_MODE_SHIFT                 0x01u
        #define SampleTimer_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << SampleTimer_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define SampleTimer_CTRL_RCOD_SHIFT        0x02u
        #define SampleTimer_CTRL_ENBL_SHIFT        0x00u
        #define SampleTimer_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define SampleTimer_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << SampleTimer_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define SampleTimer_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << SampleTimer_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define SampleTimer_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << SampleTimer_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define SampleTimer_CTRL_RCOD       ((uint8)((uint8)0x03u << SampleTimer_CTRL_RCOD_SHIFT))
        #define SampleTimer_CTRL_ENBL       ((uint8)((uint8)0x80u << SampleTimer_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define SampleTimer_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define SampleTimer_RT1_MASK                        ((uint8)((uint8)0x03u << SampleTimer_RT1_SHIFT))
    #define SampleTimer_SYNC                            ((uint8)((uint8)0x03u << SampleTimer_RT1_SHIFT))
    #define SampleTimer_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define SampleTimer_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << SampleTimer_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define SampleTimer_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << SampleTimer_SYNCDSI_SHIFT))

    #define SampleTimer_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << SampleTimer_CTRL_MODE_SHIFT))
    #define SampleTimer_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << SampleTimer_CTRL_MODE_SHIFT))
    #define SampleTimer_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << SampleTimer_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define SampleTimer_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define SampleTimer_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define SampleTimer_STATUS_TC_INT_MASK_SHIFT        (SampleTimer_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define SampleTimer_STATUS_CAPTURE_INT_MASK_SHIFT   (SampleTimer_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define SampleTimer_STATUS_TC                       ((uint8)((uint8)0x01u << SampleTimer_STATUS_TC_SHIFT))
    #define SampleTimer_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << SampleTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define SampleTimer_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << SampleTimer_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define SampleTimer_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << SampleTimer_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define SampleTimer_STATUS              (* (reg8 *) SampleTimer_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define SampleTimer_STATUS_MASK         (* (reg8 *) SampleTimer_TimerUDB_rstSts_stsreg__MASK_REG)
    #define SampleTimer_STATUS_AUX_CTRL     (* (reg8 *) SampleTimer_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define SampleTimer_CONTROL             (* (reg8 *) SampleTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(SampleTimer_Resolution <= 8u) /* 8-bit Timer */
        #define SampleTimer_CAPTURE_LSB         (* (reg8 *) SampleTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define SampleTimer_CAPTURE_LSB_PTR       ((reg8 *) SampleTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define SampleTimer_PERIOD_LSB          (* (reg8 *) SampleTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define SampleTimer_PERIOD_LSB_PTR        ((reg8 *) SampleTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define SampleTimer_COUNTER_LSB         (* (reg8 *) SampleTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
        #define SampleTimer_COUNTER_LSB_PTR       ((reg8 *) SampleTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
    #elif(SampleTimer_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define SampleTimer_CAPTURE_LSB         (* (reg16 *) SampleTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define SampleTimer_CAPTURE_LSB_PTR       ((reg16 *) SampleTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define SampleTimer_PERIOD_LSB          (* (reg16 *) SampleTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define SampleTimer_PERIOD_LSB_PTR        ((reg16 *) SampleTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define SampleTimer_COUNTER_LSB         (* (reg16 *) SampleTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
            #define SampleTimer_COUNTER_LSB_PTR       ((reg16 *) SampleTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define SampleTimer_CAPTURE_LSB         (* (reg16 *) SampleTimer_TimerUDB_sT32_timerdp_u0__16BIT_F0_REG )
            #define SampleTimer_CAPTURE_LSB_PTR       ((reg16 *) SampleTimer_TimerUDB_sT32_timerdp_u0__16BIT_F0_REG )
            #define SampleTimer_PERIOD_LSB          (* (reg16 *) SampleTimer_TimerUDB_sT32_timerdp_u0__16BIT_D0_REG )
            #define SampleTimer_PERIOD_LSB_PTR        ((reg16 *) SampleTimer_TimerUDB_sT32_timerdp_u0__16BIT_D0_REG )
            #define SampleTimer_COUNTER_LSB         (* (reg16 *) SampleTimer_TimerUDB_sT32_timerdp_u0__16BIT_A0_REG )
            #define SampleTimer_COUNTER_LSB_PTR       ((reg16 *) SampleTimer_TimerUDB_sT32_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(SampleTimer_Resolution <= 24u)/* 24-bit Timer */
        #define SampleTimer_CAPTURE_LSB         (* (reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define SampleTimer_CAPTURE_LSB_PTR       ((reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define SampleTimer_PERIOD_LSB          (* (reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define SampleTimer_PERIOD_LSB_PTR        ((reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define SampleTimer_COUNTER_LSB         (* (reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
        #define SampleTimer_COUNTER_LSB_PTR       ((reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define SampleTimer_CAPTURE_LSB         (* (reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define SampleTimer_CAPTURE_LSB_PTR       ((reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define SampleTimer_PERIOD_LSB          (* (reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define SampleTimer_PERIOD_LSB_PTR        ((reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define SampleTimer_COUNTER_LSB         (* (reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
            #define SampleTimer_COUNTER_LSB_PTR       ((reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define SampleTimer_CAPTURE_LSB         (* (reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__32BIT_F0_REG )
            #define SampleTimer_CAPTURE_LSB_PTR       ((reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__32BIT_F0_REG )
            #define SampleTimer_PERIOD_LSB          (* (reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__32BIT_D0_REG )
            #define SampleTimer_PERIOD_LSB_PTR        ((reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__32BIT_D0_REG )
            #define SampleTimer_COUNTER_LSB         (* (reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__32BIT_A0_REG )
            #define SampleTimer_COUNTER_LSB_PTR       ((reg32 *) SampleTimer_TimerUDB_sT32_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define SampleTimer_COUNTER_LSB_PTR_8BIT       ((reg8 *) SampleTimer_TimerUDB_sT32_timerdp_u0__A0_REG )
    
    #if (SampleTimer_UsingHWCaptureCounter)
        #define SampleTimer_CAP_COUNT              (*(reg8 *) SampleTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define SampleTimer_CAP_COUNT_PTR          ( (reg8 *) SampleTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define SampleTimer_CAPTURE_COUNT_CTRL     (*(reg8 *) SampleTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define SampleTimer_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) SampleTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (SampleTimer_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define SampleTimer_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define SampleTimer_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define SampleTimer_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define SampleTimer_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define SampleTimer_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define SampleTimer_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << SampleTimer_CTRL_INTCNT_SHIFT))
    #define SampleTimer_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << SampleTimer_CTRL_TRIG_MODE_SHIFT))
    #define SampleTimer_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << SampleTimer_CTRL_TRIG_EN_SHIFT))
    #define SampleTimer_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << SampleTimer_CTRL_CAP_MODE_SHIFT))
    #define SampleTimer_CTRL_ENABLE                    ((uint8)((uint8)0x01u << SampleTimer_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define SampleTimer_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define SampleTimer_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define SampleTimer_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define SampleTimer_STATUS_TC_INT_MASK_SHIFT       SampleTimer_STATUS_TC_SHIFT
    #define SampleTimer_STATUS_CAPTURE_INT_MASK_SHIFT  SampleTimer_STATUS_CAPTURE_SHIFT
    #define SampleTimer_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define SampleTimer_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define SampleTimer_STATUS_FIFOFULL_INT_MASK_SHIFT SampleTimer_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define SampleTimer_STATUS_TC                      ((uint8)((uint8)0x01u << SampleTimer_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define SampleTimer_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << SampleTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define SampleTimer_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << SampleTimer_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define SampleTimer_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << SampleTimer_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define SampleTimer_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << SampleTimer_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define SampleTimer_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << SampleTimer_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define SampleTimer_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << SampleTimer_STATUS_FIFOFULL_SHIFT))

    #define SampleTimer_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define SampleTimer_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define SampleTimer_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define SampleTimer_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define SampleTimer_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define SampleTimer_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_SampleTimer_H */


/* [] END OF FILE */
