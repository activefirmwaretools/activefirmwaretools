/*******************************************************************************
* File Name: timer_clock_2.h
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

#if !defined(CY_CLOCK_timer_clock_2_H)
#define CY_CLOCK_timer_clock_2_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void timer_clock_2_StartEx(uint32 alignClkDiv);
#define timer_clock_2_Start() \
    timer_clock_2_StartEx(timer_clock_2__PA_DIV_ID)

#else

void timer_clock_2_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void timer_clock_2_Stop(void);

void timer_clock_2_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 timer_clock_2_GetDividerRegister(void);
uint8  timer_clock_2_GetFractionalDividerRegister(void);

#define timer_clock_2_Enable()                         timer_clock_2_Start()
#define timer_clock_2_Disable()                        timer_clock_2_Stop()
#define timer_clock_2_SetDividerRegister(clkDivider, reset)  \
    timer_clock_2_SetFractionalDividerRegister((clkDivider), 0u)
#define timer_clock_2_SetDivider(clkDivider)           timer_clock_2_SetDividerRegister((clkDivider), 1u)
#define timer_clock_2_SetDividerValue(clkDivider)      timer_clock_2_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define timer_clock_2_DIV_ID     timer_clock_2__DIV_ID

#define timer_clock_2_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define timer_clock_2_CTRL_REG   (*(reg32 *)timer_clock_2__CTRL_REGISTER)
#define timer_clock_2_DIV_REG    (*(reg32 *)timer_clock_2__DIV_REGISTER)

#define timer_clock_2_CMD_DIV_SHIFT          (0u)
#define timer_clock_2_CMD_PA_DIV_SHIFT       (8u)
#define timer_clock_2_CMD_DISABLE_SHIFT      (30u)
#define timer_clock_2_CMD_ENABLE_SHIFT       (31u)

#define timer_clock_2_CMD_DISABLE_MASK       ((uint32)((uint32)1u << timer_clock_2_CMD_DISABLE_SHIFT))
#define timer_clock_2_CMD_ENABLE_MASK        ((uint32)((uint32)1u << timer_clock_2_CMD_ENABLE_SHIFT))

#define timer_clock_2_DIV_FRAC_MASK  (0x000000F8u)
#define timer_clock_2_DIV_FRAC_SHIFT (3u)
#define timer_clock_2_DIV_INT_MASK   (0xFFFFFF00u)
#define timer_clock_2_DIV_INT_SHIFT  (8u)

#else 

#define timer_clock_2_DIV_REG        (*(reg32 *)timer_clock_2__REGISTER)
#define timer_clock_2_ENABLE_REG     timer_clock_2_DIV_REG
#define timer_clock_2_DIV_FRAC_MASK  timer_clock_2__FRAC_MASK
#define timer_clock_2_DIV_FRAC_SHIFT (16u)
#define timer_clock_2_DIV_INT_MASK   timer_clock_2__DIVIDER_MASK
#define timer_clock_2_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_timer_clock_2_H) */

/* [] END OF FILE */
