/*******************************************************************************
* File Name: VDDIO_UNBUFFERED.h  
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

#if !defined(CY_PINS_VDDIO_UNBUFFERED_ALIASES_H) /* Pins VDDIO_UNBUFFERED_ALIASES_H */
#define CY_PINS_VDDIO_UNBUFFERED_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define VDDIO_UNBUFFERED_0			(VDDIO_UNBUFFERED__0__PC)
#define VDDIO_UNBUFFERED_0_PS		(VDDIO_UNBUFFERED__0__PS)
#define VDDIO_UNBUFFERED_0_PC		(VDDIO_UNBUFFERED__0__PC)
#define VDDIO_UNBUFFERED_0_DR		(VDDIO_UNBUFFERED__0__DR)
#define VDDIO_UNBUFFERED_0_SHIFT	(VDDIO_UNBUFFERED__0__SHIFT)
#define VDDIO_UNBUFFERED_0_INTR	((uint16)((uint16)0x0003u << (VDDIO_UNBUFFERED__0__SHIFT*2u)))

#define VDDIO_UNBUFFERED_INTR_ALL	 ((uint16)(VDDIO_UNBUFFERED_0_INTR))


#endif /* End Pins VDDIO_UNBUFFERED_ALIASES_H */


/* [] END OF FILE */
