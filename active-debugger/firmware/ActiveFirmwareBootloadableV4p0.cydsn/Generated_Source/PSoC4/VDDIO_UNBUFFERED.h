/*******************************************************************************
* File Name: VDDIO_UNBUFFERED.h  
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

#if !defined(CY_PINS_VDDIO_UNBUFFERED_H) /* Pins VDDIO_UNBUFFERED_H */
#define CY_PINS_VDDIO_UNBUFFERED_H

#include "cytypes.h"
#include "cyfitter.h"
#include "VDDIO_UNBUFFERED_aliases.h"


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
} VDDIO_UNBUFFERED_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   VDDIO_UNBUFFERED_Read(void);
void    VDDIO_UNBUFFERED_Write(uint8 value);
uint8   VDDIO_UNBUFFERED_ReadDataReg(void);
#if defined(VDDIO_UNBUFFERED__PC) || (CY_PSOC4_4200L) 
    void    VDDIO_UNBUFFERED_SetDriveMode(uint8 mode);
#endif
void    VDDIO_UNBUFFERED_SetInterruptMode(uint16 position, uint16 mode);
uint8   VDDIO_UNBUFFERED_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void VDDIO_UNBUFFERED_Sleep(void); 
void VDDIO_UNBUFFERED_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(VDDIO_UNBUFFERED__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define VDDIO_UNBUFFERED_DRIVE_MODE_BITS        (3)
    #define VDDIO_UNBUFFERED_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - VDDIO_UNBUFFERED_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the VDDIO_UNBUFFERED_SetDriveMode() function.
         *  @{
         */
        #define VDDIO_UNBUFFERED_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define VDDIO_UNBUFFERED_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define VDDIO_UNBUFFERED_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define VDDIO_UNBUFFERED_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define VDDIO_UNBUFFERED_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define VDDIO_UNBUFFERED_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define VDDIO_UNBUFFERED_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define VDDIO_UNBUFFERED_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define VDDIO_UNBUFFERED_MASK               VDDIO_UNBUFFERED__MASK
#define VDDIO_UNBUFFERED_SHIFT              VDDIO_UNBUFFERED__SHIFT
#define VDDIO_UNBUFFERED_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in VDDIO_UNBUFFERED_SetInterruptMode() function.
     *  @{
     */
        #define VDDIO_UNBUFFERED_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define VDDIO_UNBUFFERED_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define VDDIO_UNBUFFERED_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define VDDIO_UNBUFFERED_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(VDDIO_UNBUFFERED__SIO)
    #define VDDIO_UNBUFFERED_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(VDDIO_UNBUFFERED__PC) && (CY_PSOC4_4200L)
    #define VDDIO_UNBUFFERED_USBIO_ENABLE               ((uint32)0x80000000u)
    #define VDDIO_UNBUFFERED_USBIO_DISABLE              ((uint32)(~VDDIO_UNBUFFERED_USBIO_ENABLE))
    #define VDDIO_UNBUFFERED_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define VDDIO_UNBUFFERED_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define VDDIO_UNBUFFERED_USBIO_ENTER_SLEEP          ((uint32)((1u << VDDIO_UNBUFFERED_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << VDDIO_UNBUFFERED_USBIO_SUSPEND_DEL_SHIFT)))
    #define VDDIO_UNBUFFERED_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << VDDIO_UNBUFFERED_USBIO_SUSPEND_SHIFT)))
    #define VDDIO_UNBUFFERED_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << VDDIO_UNBUFFERED_USBIO_SUSPEND_DEL_SHIFT)))
    #define VDDIO_UNBUFFERED_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(VDDIO_UNBUFFERED__PC)
    /* Port Configuration */
    #define VDDIO_UNBUFFERED_PC                 (* (reg32 *) VDDIO_UNBUFFERED__PC)
#endif
/* Pin State */
#define VDDIO_UNBUFFERED_PS                     (* (reg32 *) VDDIO_UNBUFFERED__PS)
/* Data Register */
#define VDDIO_UNBUFFERED_DR                     (* (reg32 *) VDDIO_UNBUFFERED__DR)
/* Input Buffer Disable Override */
#define VDDIO_UNBUFFERED_INP_DIS                (* (reg32 *) VDDIO_UNBUFFERED__PC2)

/* Interrupt configuration Registers */
#define VDDIO_UNBUFFERED_INTCFG                 (* (reg32 *) VDDIO_UNBUFFERED__INTCFG)
#define VDDIO_UNBUFFERED_INTSTAT                (* (reg32 *) VDDIO_UNBUFFERED__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define VDDIO_UNBUFFERED_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(VDDIO_UNBUFFERED__SIO)
    #define VDDIO_UNBUFFERED_SIO_REG            (* (reg32 *) VDDIO_UNBUFFERED__SIO)
#endif /* (VDDIO_UNBUFFERED__SIO_CFG) */

/* USBIO registers */
#if !defined(VDDIO_UNBUFFERED__PC) && (CY_PSOC4_4200L)
    #define VDDIO_UNBUFFERED_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define VDDIO_UNBUFFERED_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define VDDIO_UNBUFFERED_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define VDDIO_UNBUFFERED_DRIVE_MODE_SHIFT       (0x00u)
#define VDDIO_UNBUFFERED_DRIVE_MODE_MASK        (0x07u << VDDIO_UNBUFFERED_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins VDDIO_UNBUFFERED_H */


/* [] END OF FILE */
