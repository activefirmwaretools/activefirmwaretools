/*******************************************************************************
* File Name: COUNTER2A_ISR.h
* Version 1.71
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
#if !defined(CY_ISR_COUNTER2A_ISR_H)
#define CY_ISR_COUNTER2A_ISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void COUNTER2A_ISR_Start(void);
void COUNTER2A_ISR_StartEx(cyisraddress address);
void COUNTER2A_ISR_Stop(void);

CY_ISR_PROTO(COUNTER2A_ISR_Interrupt);

void COUNTER2A_ISR_SetVector(cyisraddress address);
cyisraddress COUNTER2A_ISR_GetVector(void);

void COUNTER2A_ISR_SetPriority(uint8 priority);
uint8 COUNTER2A_ISR_GetPriority(void);

void COUNTER2A_ISR_Enable(void);
uint8 COUNTER2A_ISR_GetState(void);
void COUNTER2A_ISR_Disable(void);

void COUNTER2A_ISR_SetPending(void);
void COUNTER2A_ISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the COUNTER2A_ISR ISR. */
#define COUNTER2A_ISR_INTC_VECTOR            ((reg32 *) COUNTER2A_ISR__INTC_VECT)

/* Address of the COUNTER2A_ISR ISR priority. */
#define COUNTER2A_ISR_INTC_PRIOR             ((reg32 *) COUNTER2A_ISR__INTC_PRIOR_REG)

/* Priority of the COUNTER2A_ISR interrupt. */
#define COUNTER2A_ISR_INTC_PRIOR_NUMBER      COUNTER2A_ISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable COUNTER2A_ISR interrupt. */
#define COUNTER2A_ISR_INTC_SET_EN            ((reg32 *) COUNTER2A_ISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the COUNTER2A_ISR interrupt. */
#define COUNTER2A_ISR_INTC_CLR_EN            ((reg32 *) COUNTER2A_ISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the COUNTER2A_ISR interrupt state to pending. */
#define COUNTER2A_ISR_INTC_SET_PD            ((reg32 *) COUNTER2A_ISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the COUNTER2A_ISR interrupt. */
#define COUNTER2A_ISR_INTC_CLR_PD            ((reg32 *) COUNTER2A_ISR__INTC_CLR_PD_REG)



#endif /* CY_ISR_COUNTER2A_ISR_H */


/* [] END OF FILE */
