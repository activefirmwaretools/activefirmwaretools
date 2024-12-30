/*******************************************************************************
* File Name: TimerClock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_TimerClock_H)
#define CY_CLOCK_TimerClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void TimerClock_StartEx(uint32 alignClkDiv);
#define TimerClock_Start() \
    TimerClock_StartEx(TimerClock__PA_DIV_ID)

#else

void TimerClock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void TimerClock_Stop(void);

void TimerClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 TimerClock_GetDividerRegister(void);
uint8  TimerClock_GetFractionalDividerRegister(void);

#define TimerClock_Enable()                         TimerClock_Start()
#define TimerClock_Disable()                        TimerClock_Stop()
#define TimerClock_SetDividerRegister(clkDivider, reset)  \
    TimerClock_SetFractionalDividerRegister((clkDivider), 0u)
#define TimerClock_SetDivider(clkDivider)           TimerClock_SetDividerRegister((clkDivider), 1u)
#define TimerClock_SetDividerValue(clkDivider)      TimerClock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define TimerClock_DIV_ID     TimerClock__DIV_ID

#define TimerClock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define TimerClock_CTRL_REG   (*(reg32 *)TimerClock__CTRL_REGISTER)
#define TimerClock_DIV_REG    (*(reg32 *)TimerClock__DIV_REGISTER)

#define TimerClock_CMD_DIV_SHIFT          (0u)
#define TimerClock_CMD_PA_DIV_SHIFT       (8u)
#define TimerClock_CMD_DISABLE_SHIFT      (30u)
#define TimerClock_CMD_ENABLE_SHIFT       (31u)

#define TimerClock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << TimerClock_CMD_DISABLE_SHIFT))
#define TimerClock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << TimerClock_CMD_ENABLE_SHIFT))

#define TimerClock_DIV_FRAC_MASK  (0x000000F8u)
#define TimerClock_DIV_FRAC_SHIFT (3u)
#define TimerClock_DIV_INT_MASK   (0xFFFFFF00u)
#define TimerClock_DIV_INT_SHIFT  (8u)

#else 

#define TimerClock_DIV_REG        (*(reg32 *)TimerClock__REGISTER)
#define TimerClock_ENABLE_REG     TimerClock_DIV_REG
#define TimerClock_DIV_FRAC_MASK  TimerClock__FRAC_MASK
#define TimerClock_DIV_FRAC_SHIFT (16u)
#define TimerClock_DIV_INT_MASK   TimerClock__DIVIDER_MASK
#define TimerClock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_TimerClock_H) */

/* [] END OF FILE */
