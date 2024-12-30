/*******************************************************************************
* File Name: VDDIO_BUFFERED.h  
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

#if !defined(CY_PINS_VDDIO_BUFFERED_H) /* Pins VDDIO_BUFFERED_H */
#define CY_PINS_VDDIO_BUFFERED_H

#include "cytypes.h"
#include "cyfitter.h"
#include "VDDIO_BUFFERED_aliases.h"


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
} VDDIO_BUFFERED_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   VDDIO_BUFFERED_Read(void);
void    VDDIO_BUFFERED_Write(uint8 value);
uint8   VDDIO_BUFFERED_ReadDataReg(void);
#if defined(VDDIO_BUFFERED__PC) || (CY_PSOC4_4200L) 
    void    VDDIO_BUFFERED_SetDriveMode(uint8 mode);
#endif
void    VDDIO_BUFFERED_SetInterruptMode(uint16 position, uint16 mode);
uint8   VDDIO_BUFFERED_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void VDDIO_BUFFERED_Sleep(void); 
void VDDIO_BUFFERED_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(VDDIO_BUFFERED__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define VDDIO_BUFFERED_DRIVE_MODE_BITS        (3)
    #define VDDIO_BUFFERED_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - VDDIO_BUFFERED_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the VDDIO_BUFFERED_SetDriveMode() function.
         *  @{
         */
        #define VDDIO_BUFFERED_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define VDDIO_BUFFERED_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define VDDIO_BUFFERED_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define VDDIO_BUFFERED_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define VDDIO_BUFFERED_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define VDDIO_BUFFERED_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define VDDIO_BUFFERED_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define VDDIO_BUFFERED_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define VDDIO_BUFFERED_MASK               VDDIO_BUFFERED__MASK
#define VDDIO_BUFFERED_SHIFT              VDDIO_BUFFERED__SHIFT
#define VDDIO_BUFFERED_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in VDDIO_BUFFERED_SetInterruptMode() function.
     *  @{
     */
        #define VDDIO_BUFFERED_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define VDDIO_BUFFERED_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define VDDIO_BUFFERED_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define VDDIO_BUFFERED_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(VDDIO_BUFFERED__SIO)
    #define VDDIO_BUFFERED_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(VDDIO_BUFFERED__PC) && (CY_PSOC4_4200L)
    #define VDDIO_BUFFERED_USBIO_ENABLE               ((uint32)0x80000000u)
    #define VDDIO_BUFFERED_USBIO_DISABLE              ((uint32)(~VDDIO_BUFFERED_USBIO_ENABLE))
    #define VDDIO_BUFFERED_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define VDDIO_BUFFERED_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define VDDIO_BUFFERED_USBIO_ENTER_SLEEP          ((uint32)((1u << VDDIO_BUFFERED_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << VDDIO_BUFFERED_USBIO_SUSPEND_DEL_SHIFT)))
    #define VDDIO_BUFFERED_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << VDDIO_BUFFERED_USBIO_SUSPEND_SHIFT)))
    #define VDDIO_BUFFERED_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << VDDIO_BUFFERED_USBIO_SUSPEND_DEL_SHIFT)))
    #define VDDIO_BUFFERED_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(VDDIO_BUFFERED__PC)
    /* Port Configuration */
    #define VDDIO_BUFFERED_PC                 (* (reg32 *) VDDIO_BUFFERED__PC)
#endif
/* Pin State */
#define VDDIO_BUFFERED_PS                     (* (reg32 *) VDDIO_BUFFERED__PS)
/* Data Register */
#define VDDIO_BUFFERED_DR                     (* (reg32 *) VDDIO_BUFFERED__DR)
/* Input Buffer Disable Override */
#define VDDIO_BUFFERED_INP_DIS                (* (reg32 *) VDDIO_BUFFERED__PC2)

/* Interrupt configuration Registers */
#define VDDIO_BUFFERED_INTCFG                 (* (reg32 *) VDDIO_BUFFERED__INTCFG)
#define VDDIO_BUFFERED_INTSTAT                (* (reg32 *) VDDIO_BUFFERED__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define VDDIO_BUFFERED_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(VDDIO_BUFFERED__SIO)
    #define VDDIO_BUFFERED_SIO_REG            (* (reg32 *) VDDIO_BUFFERED__SIO)
#endif /* (VDDIO_BUFFERED__SIO_CFG) */

/* USBIO registers */
#if !defined(VDDIO_BUFFERED__PC) && (CY_PSOC4_4200L)
    #define VDDIO_BUFFERED_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define VDDIO_BUFFERED_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define VDDIO_BUFFERED_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define VDDIO_BUFFERED_DRIVE_MODE_SHIFT       (0x00u)
#define VDDIO_BUFFERED_DRIVE_MODE_MASK        (0x07u << VDDIO_BUFFERED_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins VDDIO_BUFFERED_H */


/* [] END OF FILE */
