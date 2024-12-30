/*******************************************************************************
* File Name: ACTIVE_DATA_GPIO.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_ACTIVE_DATA_GPIO_ALIASES_H) /* Pins ACTIVE_DATA_GPIO_ALIASES_H */
#define CY_PINS_ACTIVE_DATA_GPIO_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define ACTIVE_DATA_GPIO_0			(ACTIVE_DATA_GPIO__0__PC)
#define ACTIVE_DATA_GPIO_0_PS		(ACTIVE_DATA_GPIO__0__PS)
#define ACTIVE_DATA_GPIO_0_PC		(ACTIVE_DATA_GPIO__0__PC)
#define ACTIVE_DATA_GPIO_0_DR		(ACTIVE_DATA_GPIO__0__DR)
#define ACTIVE_DATA_GPIO_0_SHIFT	(ACTIVE_DATA_GPIO__0__SHIFT)
#define ACTIVE_DATA_GPIO_0_INTR	((uint16)((uint16)0x0003u << (ACTIVE_DATA_GPIO__0__SHIFT*2u)))

#define ACTIVE_DATA_GPIO_INTR_ALL	 ((uint16)(ACTIVE_DATA_GPIO_0_INTR))


#endif /* End Pins ACTIVE_DATA_GPIO_ALIASES_H */


/* [] END OF FILE */
