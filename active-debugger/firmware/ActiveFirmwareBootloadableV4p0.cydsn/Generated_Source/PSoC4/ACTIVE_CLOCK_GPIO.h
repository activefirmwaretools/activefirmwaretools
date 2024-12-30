/*******************************************************************************
* File Name: ACTIVE_CLOCK_GPIO.h  
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

#if !defined(CY_PINS_ACTIVE_CLOCK_GPIO_H) /* Pins ACTIVE_CLOCK_GPIO_H */
#define CY_PINS_ACTIVE_CLOCK_GPIO_H

#include "cytypes.h"
#include "cyfitter.h"
#include "ACTIVE_CLOCK_GPIO_aliases.h"


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
} ACTIVE_CLOCK_GPIO_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   ACTIVE_CLOCK_GPIO_Read(void);
void    ACTIVE_CLOCK_GPIO_Write(uint8 value);
uint8   ACTIVE_CLOCK_GPIO_ReadDataReg(void);
#if defined(ACTIVE_CLOCK_GPIO__PC) || (CY_PSOC4_4200L) 
    void    ACTIVE_CLOCK_GPIO_SetDriveMode(uint8 mode);
#endif
void    ACTIVE_CLOCK_GPIO_SetInterruptMode(uint16 position, uint16 mode);
uint8   ACTIVE_CLOCK_GPIO_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void ACTIVE_CLOCK_GPIO_Sleep(void); 
void ACTIVE_CLOCK_GPIO_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(ACTIVE_CLOCK_GPIO__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define ACTIVE_CLOCK_GPIO_DRIVE_MODE_BITS        (3)
    #define ACTIVE_CLOCK_GPIO_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - ACTIVE_CLOCK_GPIO_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the ACTIVE_CLOCK_GPIO_SetDriveMode() function.
         *  @{
         */
        #define ACTIVE_CLOCK_GPIO_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define ACTIVE_CLOCK_GPIO_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define ACTIVE_CLOCK_GPIO_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define ACTIVE_CLOCK_GPIO_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define ACTIVE_CLOCK_GPIO_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define ACTIVE_CLOCK_GPIO_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define ACTIVE_CLOCK_GPIO_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define ACTIVE_CLOCK_GPIO_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define ACTIVE_CLOCK_GPIO_MASK               ACTIVE_CLOCK_GPIO__MASK
#define ACTIVE_CLOCK_GPIO_SHIFT              ACTIVE_CLOCK_GPIO__SHIFT
#define ACTIVE_CLOCK_GPIO_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ACTIVE_CLOCK_GPIO_SetInterruptMode() function.
     *  @{
     */
        #define ACTIVE_CLOCK_GPIO_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define ACTIVE_CLOCK_GPIO_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define ACTIVE_CLOCK_GPIO_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define ACTIVE_CLOCK_GPIO_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(ACTIVE_CLOCK_GPIO__SIO)
    #define ACTIVE_CLOCK_GPIO_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(ACTIVE_CLOCK_GPIO__PC) && (CY_PSOC4_4200L)
    #define ACTIVE_CLOCK_GPIO_USBIO_ENABLE               ((uint32)0x80000000u)
    #define ACTIVE_CLOCK_GPIO_USBIO_DISABLE              ((uint32)(~ACTIVE_CLOCK_GPIO_USBIO_ENABLE))
    #define ACTIVE_CLOCK_GPIO_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define ACTIVE_CLOCK_GPIO_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define ACTIVE_CLOCK_GPIO_USBIO_ENTER_SLEEP          ((uint32)((1u << ACTIVE_CLOCK_GPIO_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << ACTIVE_CLOCK_GPIO_USBIO_SUSPEND_DEL_SHIFT)))
    #define ACTIVE_CLOCK_GPIO_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << ACTIVE_CLOCK_GPIO_USBIO_SUSPEND_SHIFT)))
    #define ACTIVE_CLOCK_GPIO_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << ACTIVE_CLOCK_GPIO_USBIO_SUSPEND_DEL_SHIFT)))
    #define ACTIVE_CLOCK_GPIO_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(ACTIVE_CLOCK_GPIO__PC)
    /* Port Configuration */
    #define ACTIVE_CLOCK_GPIO_PC                 (* (reg32 *) ACTIVE_CLOCK_GPIO__PC)
#endif
/* Pin State */
#define ACTIVE_CLOCK_GPIO_PS                     (* (reg32 *) ACTIVE_CLOCK_GPIO__PS)
/* Data Register */
#define ACTIVE_CLOCK_GPIO_DR                     (* (reg32 *) ACTIVE_CLOCK_GPIO__DR)
/* Input Buffer Disable Override */
#define ACTIVE_CLOCK_GPIO_INP_DIS                (* (reg32 *) ACTIVE_CLOCK_GPIO__PC2)

/* Interrupt configuration Registers */
#define ACTIVE_CLOCK_GPIO_INTCFG                 (* (reg32 *) ACTIVE_CLOCK_GPIO__INTCFG)
#define ACTIVE_CLOCK_GPIO_INTSTAT                (* (reg32 *) ACTIVE_CLOCK_GPIO__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define ACTIVE_CLOCK_GPIO_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(ACTIVE_CLOCK_GPIO__SIO)
    #define ACTIVE_CLOCK_GPIO_SIO_REG            (* (reg32 *) ACTIVE_CLOCK_GPIO__SIO)
#endif /* (ACTIVE_CLOCK_GPIO__SIO_CFG) */

/* USBIO registers */
#if !defined(ACTIVE_CLOCK_GPIO__PC) && (CY_PSOC4_4200L)
    #define ACTIVE_CLOCK_GPIO_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define ACTIVE_CLOCK_GPIO_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define ACTIVE_CLOCK_GPIO_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define ACTIVE_CLOCK_GPIO_DRIVE_MODE_SHIFT       (0x00u)
#define ACTIVE_CLOCK_GPIO_DRIVE_MODE_MASK        (0x07u << ACTIVE_CLOCK_GPIO_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins ACTIVE_CLOCK_GPIO_H */


/* [] END OF FILE */
