/*******************************************************************************
* File Name: ACTIVE_DATA_GPIO.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "ACTIVE_DATA_GPIO.h"

static ACTIVE_DATA_GPIO_BACKUP_STRUCT  ACTIVE_DATA_GPIO_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: ACTIVE_DATA_GPIO_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet ACTIVE_DATA_GPIO_SUT.c usage_ACTIVE_DATA_GPIO_Sleep_Wakeup
*******************************************************************************/
void ACTIVE_DATA_GPIO_Sleep(void)
{
    #if defined(ACTIVE_DATA_GPIO__PC)
        ACTIVE_DATA_GPIO_backup.pcState = ACTIVE_DATA_GPIO_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            ACTIVE_DATA_GPIO_backup.usbState = ACTIVE_DATA_GPIO_CR1_REG;
            ACTIVE_DATA_GPIO_USB_POWER_REG |= ACTIVE_DATA_GPIO_USBIO_ENTER_SLEEP;
            ACTIVE_DATA_GPIO_CR1_REG &= ACTIVE_DATA_GPIO_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(ACTIVE_DATA_GPIO__SIO)
        ACTIVE_DATA_GPIO_backup.sioState = ACTIVE_DATA_GPIO_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        ACTIVE_DATA_GPIO_SIO_REG &= (uint32)(~ACTIVE_DATA_GPIO_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: ACTIVE_DATA_GPIO_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to ACTIVE_DATA_GPIO_Sleep() for an example usage.
*******************************************************************************/
void ACTIVE_DATA_GPIO_Wakeup(void)
{
    #if defined(ACTIVE_DATA_GPIO__PC)
        ACTIVE_DATA_GPIO_PC = ACTIVE_DATA_GPIO_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            ACTIVE_DATA_GPIO_USB_POWER_REG &= ACTIVE_DATA_GPIO_USBIO_EXIT_SLEEP_PH1;
            ACTIVE_DATA_GPIO_CR1_REG = ACTIVE_DATA_GPIO_backup.usbState;
            ACTIVE_DATA_GPIO_USB_POWER_REG &= ACTIVE_DATA_GPIO_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(ACTIVE_DATA_GPIO__SIO)
        ACTIVE_DATA_GPIO_SIO_REG = ACTIVE_DATA_GPIO_backup.sioState;
    #endif
}


/* [] END OF FILE */
