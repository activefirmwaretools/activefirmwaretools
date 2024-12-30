/*******************************************************************************
* File Name: Timestamp_ISR_B.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_Timestamp_ISR_B_H)
#define CY_ISR_Timestamp_ISR_B_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Timestamp_ISR_B_Start(void);
void Timestamp_ISR_B_StartEx(cyisraddress address);
void Timestamp_ISR_B_Stop(void);

CY_ISR_PROTO(Timestamp_ISR_B_Interrupt);

void Timestamp_ISR_B_SetVector(cyisraddress address);
cyisraddress Timestamp_ISR_B_GetVector(void);

void Timestamp_ISR_B_SetPriority(uint8 priority);
uint8 Timestamp_ISR_B_GetPriority(void);

void Timestamp_ISR_B_Enable(void);
uint8 Timestamp_ISR_B_GetState(void);
void Timestamp_ISR_B_Disable(void);

void Timestamp_ISR_B_SetPending(void);
void Timestamp_ISR_B_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Timestamp_ISR_B ISR. */
#define Timestamp_ISR_B_INTC_VECTOR            ((reg32 *) Timestamp_ISR_B__INTC_VECT)

/* Address of the Timestamp_ISR_B ISR priority. */
#define Timestamp_ISR_B_INTC_PRIOR             ((reg32 *) Timestamp_ISR_B__INTC_PRIOR_REG)

/* Priority of the Timestamp_ISR_B interrupt. */
#define Timestamp_ISR_B_INTC_PRIOR_NUMBER      Timestamp_ISR_B__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Timestamp_ISR_B interrupt. */
#define Timestamp_ISR_B_INTC_SET_EN            ((reg32 *) Timestamp_ISR_B__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Timestamp_ISR_B interrupt. */
#define Timestamp_ISR_B_INTC_CLR_EN            ((reg32 *) Timestamp_ISR_B__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Timestamp_ISR_B interrupt state to pending. */
#define Timestamp_ISR_B_INTC_SET_PD            ((reg32 *) Timestamp_ISR_B__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Timestamp_ISR_B interrupt. */
#define Timestamp_ISR_B_INTC_CLR_PD            ((reg32 *) Timestamp_ISR_B__INTC_CLR_PD_REG)



#endif /* CY_ISR_Timestamp_ISR_B_H */


/* [] END OF FILE */
