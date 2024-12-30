/*******************************************************************************
* File Name: IN0.h  
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

#if !defined(CY_PINS_IN0_H) /* Pins IN0_H */
#define CY_PINS_IN0_H

#include "cytypes.h"
#include "cyfitter.h"
#include "IN0_aliases.h"


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
} IN0_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   IN0_Read(void);
void    IN0_Write(uint8 value);
uint8   IN0_ReadDataReg(void);
#if defined(IN0__PC) || (CY_PSOC4_4200L) 
    void    IN0_SetDriveMode(uint8 mode);
#endif
void    IN0_SetInterruptMode(uint16 position, uint16 mode);
uint8   IN0_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void IN0_Sleep(void); 
void IN0_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(IN0__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define IN0_DRIVE_MODE_BITS        (3)
    #define IN0_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - IN0_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the IN0_SetDriveMode() function.
         *  @{
         */
        #define IN0_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define IN0_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define IN0_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define IN0_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define IN0_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define IN0_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define IN0_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define IN0_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define IN0_MASK               IN0__MASK
#define IN0_SHIFT              IN0__SHIFT
#define IN0_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in IN0_SetInterruptMode() function.
     *  @{
     */
        #define IN0_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define IN0_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define IN0_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define IN0_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(IN0__SIO)
    #define IN0_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(IN0__PC) && (CY_PSOC4_4200L)
    #define IN0_USBIO_ENABLE               ((uint32)0x80000000u)
    #define IN0_USBIO_DISABLE              ((uint32)(~IN0_USBIO_ENABLE))
    #define IN0_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define IN0_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define IN0_USBIO_ENTER_SLEEP          ((uint32)((1u << IN0_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << IN0_USBIO_SUSPEND_DEL_SHIFT)))
    #define IN0_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << IN0_USBIO_SUSPEND_SHIFT)))
    #define IN0_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << IN0_USBIO_SUSPEND_DEL_SHIFT)))
    #define IN0_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(IN0__PC)
    /* Port Configuration */
    #define IN0_PC                 (* (reg32 *) IN0__PC)
#endif
/* Pin State */
#define IN0_PS                     (* (reg32 *) IN0__PS)
/* Data Register */
#define IN0_DR                     (* (reg32 *) IN0__DR)
/* Input Buffer Disable Override */
#define IN0_INP_DIS                (* (reg32 *) IN0__PC2)

/* Interrupt configuration Registers */
#define IN0_INTCFG                 (* (reg32 *) IN0__INTCFG)
#define IN0_INTSTAT                (* (reg32 *) IN0__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define IN0_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(IN0__SIO)
    #define IN0_SIO_REG            (* (reg32 *) IN0__SIO)
#endif /* (IN0__SIO_CFG) */

/* USBIO registers */
#if !defined(IN0__PC) && (CY_PSOC4_4200L)
    #define IN0_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define IN0_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define IN0_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define IN0_DRIVE_MODE_SHIFT       (0x00u)
#define IN0_DRIVE_MODE_MASK        (0x07u << IN0_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins IN0_H */


/* [] END OF FILE */
