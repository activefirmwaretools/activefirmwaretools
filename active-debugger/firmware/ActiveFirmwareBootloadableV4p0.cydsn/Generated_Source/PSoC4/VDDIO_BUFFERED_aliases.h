/*******************************************************************************
* File Name: VDDIO_BUFFERED.h  
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

#if !defined(CY_PINS_VDDIO_BUFFERED_ALIASES_H) /* Pins VDDIO_BUFFERED_ALIASES_H */
#define CY_PINS_VDDIO_BUFFERED_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define VDDIO_BUFFERED_0			(VDDIO_BUFFERED__0__PC)
#define VDDIO_BUFFERED_0_PS		(VDDIO_BUFFERED__0__PS)
#define VDDIO_BUFFERED_0_PC		(VDDIO_BUFFERED__0__PC)
#define VDDIO_BUFFERED_0_DR		(VDDIO_BUFFERED__0__DR)
#define VDDIO_BUFFERED_0_SHIFT	(VDDIO_BUFFERED__0__SHIFT)
#define VDDIO_BUFFERED_0_INTR	((uint16)((uint16)0x0003u << (VDDIO_BUFFERED__0__SHIFT*2u)))

#define VDDIO_BUFFERED_INTR_ALL	 ((uint16)(VDDIO_BUFFERED_0_INTR))


#endif /* End Pins VDDIO_BUFFERED_ALIASES_H */


/* [] END OF FILE */
