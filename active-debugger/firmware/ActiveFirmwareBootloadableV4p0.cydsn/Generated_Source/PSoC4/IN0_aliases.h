/*******************************************************************************
* File Name: IN0.h  
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

#if !defined(CY_PINS_IN0_ALIASES_H) /* Pins IN0_ALIASES_H */
#define CY_PINS_IN0_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define IN0_0			(IN0__0__PC)
#define IN0_0_PS		(IN0__0__PS)
#define IN0_0_PC		(IN0__0__PC)
#define IN0_0_DR		(IN0__0__DR)
#define IN0_0_SHIFT	(IN0__0__SHIFT)
#define IN0_0_INTR	((uint16)((uint16)0x0003u << (IN0__0__SHIFT*2u)))

#define IN0_INTR_ALL	 ((uint16)(IN0_0_INTR))


#endif /* End Pins IN0_ALIASES_H */


/* [] END OF FILE */
