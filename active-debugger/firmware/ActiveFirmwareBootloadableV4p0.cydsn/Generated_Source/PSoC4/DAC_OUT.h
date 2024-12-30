/*******************************************************************************
* File Name: DAC_OUT.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_DAC_OUT_H) /* Pins DAC_OUT_H */
#define CY_PINS_DAC_OUT_H

#include "cytypes.h"
#include "cyfitter.h"
#include "DAC_OUT_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} DAC_OUT_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   DAC_OUT_Read(void);
void    DAC_OUT_Write(uint8 value);
uint8   DAC_OUT_ReadDataReg(void);
#if defined(DAC_OUT__PC) || (CY_PSOC4_4200L) 
    void    DAC_OUT_SetDriveMode(uint8 mode);
#endif
void    DAC_OUT_SetInterruptMode(uint16 position, uint16 mode);
uint8   DAC_OUT_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void DAC_OUT_Sleep(void); 
void DAC_OUT_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(DAC_OUT__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define DAC_OUT_DRIVE_MODE_BITS        (3)
    #define DAC_OUT_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - DAC_OUT_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the DAC_OUT_SetDriveMode() function.
         *  @{
         */
        #define DAC_OUT_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define DAC_OUT_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define DAC_OUT_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define DAC_OUT_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define DAC_OUT_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define DAC_OUT_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define DAC_OUT_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define DAC_OUT_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define DAC_OUT_MASK               DAC_OUT__MASK
#define DAC_OUT_SHIFT              DAC_OUT__SHIFT
#define DAC_OUT_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DAC_OUT_SetInterruptMode() function.
     *  @{
     */
        #define DAC_OUT_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define DAC_OUT_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define DAC_OUT_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define DAC_OUT_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(DAC_OUT__SIO)
    #define DAC_OUT_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(DAC_OUT__PC) && (CY_PSOC4_4200L)
    #define DAC_OUT_USBIO_ENABLE               ((uint32)0x80000000u)
    #define DAC_OUT_USBIO_DISABLE              ((uint32)(~DAC_OUT_USBIO_ENABLE))
    #define DAC_OUT_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define DAC_OUT_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define DAC_OUT_USBIO_ENTER_SLEEP          ((uint32)((1u << DAC_OUT_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << DAC_OUT_USBIO_SUSPEND_DEL_SHIFT)))
    #define DAC_OUT_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << DAC_OUT_USBIO_SUSPEND_SHIFT)))
    #define DAC_OUT_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << DAC_OUT_USBIO_SUSPEND_DEL_SHIFT)))
    #define DAC_OUT_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(DAC_OUT__PC)
    /* Port Configuration */
    #define DAC_OUT_PC                 (* (reg32 *) DAC_OUT__PC)
#endif
/* Pin State */
#define DAC_OUT_PS                     (* (reg32 *) DAC_OUT__PS)
/* Data Register */
#define DAC_OUT_DR                     (* (reg32 *) DAC_OUT__DR)
/* Input Buffer Disable Override */
#define DAC_OUT_INP_DIS                (* (reg32 *) DAC_OUT__PC2)

/* Interrupt configuration Registers */
#define DAC_OUT_INTCFG                 (* (reg32 *) DAC_OUT__INTCFG)
#define DAC_OUT_INTSTAT                (* (reg32 *) DAC_OUT__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define DAC_OUT_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(DAC_OUT__SIO)
    #define DAC_OUT_SIO_REG            (* (reg32 *) DAC_OUT__SIO)
#endif /* (DAC_OUT__SIO_CFG) */

/* USBIO registers */
#if !defined(DAC_OUT__PC) && (CY_PSOC4_4200L)
    #define DAC_OUT_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define DAC_OUT_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define DAC_OUT_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define DAC_OUT_DRIVE_MODE_SHIFT       (0x00u)
#define DAC_OUT_DRIVE_MODE_MASK        (0x07u << DAC_OUT_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins DAC_OUT_H */


/* [] END OF FILE */
