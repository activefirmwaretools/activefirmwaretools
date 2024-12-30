/*******************************************************************************
* File Name: VDDIO_BUFFERED.c  
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
#include "VDDIO_BUFFERED.h"

static VDDIO_BUFFERED_BACKUP_STRUCT  VDDIO_BUFFERED_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: VDDIO_BUFFERED_Sleep
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
*  \snippet VDDIO_BUFFERED_SUT.c usage_VDDIO_BUFFERED_Sleep_Wakeup
*******************************************************************************/
void VDDIO_BUFFERED_Sleep(void)
{
    #if defined(VDDIO_BUFFERED__PC)
        VDDIO_BUFFERED_backup.pcState = VDDIO_BUFFERED_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            VDDIO_BUFFERED_backup.usbState = VDDIO_BUFFERED_CR1_REG;
            VDDIO_BUFFERED_USB_POWER_REG |= VDDIO_BUFFERED_USBIO_ENTER_SLEEP;
            VDDIO_BUFFERED_CR1_REG &= VDDIO_BUFFERED_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(VDDIO_BUFFERED__SIO)
        VDDIO_BUFFERED_backup.sioState = VDDIO_BUFFERED_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        VDDIO_BUFFERED_SIO_REG &= (uint32)(~VDDIO_BUFFERED_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: VDDIO_BUFFERED_Wakeup
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
*  Refer to VDDIO_BUFFERED_Sleep() for an example usage.
*******************************************************************************/
void VDDIO_BUFFERED_Wakeup(void)
{
    #if defined(VDDIO_BUFFERED__PC)
        VDDIO_BUFFERED_PC = VDDIO_BUFFERED_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            VDDIO_BUFFERED_USB_POWER_REG &= VDDIO_BUFFERED_USBIO_EXIT_SLEEP_PH1;
            VDDIO_BUFFERED_CR1_REG = VDDIO_BUFFERED_backup.usbState;
            VDDIO_BUFFERED_USB_POWER_REG &= VDDIO_BUFFERED_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(VDDIO_BUFFERED__SIO)
        VDDIO_BUFFERED_SIO_REG = VDDIO_BUFFERED_backup.sioState;
    #endif
}


/* [] END OF FILE */
