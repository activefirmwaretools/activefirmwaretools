/*******************************************************************************
* File Name: RESET_COUNT_A.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_RESET_COUNT_A_H) /* CY_CONTROL_REG_RESET_COUNT_A_H */
#define CY_CONTROL_REG_RESET_COUNT_A_H

#include "cyfitter.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} RESET_COUNT_A_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    RESET_COUNT_A_Write(uint8 control) ;
uint8   RESET_COUNT_A_Read(void) ;

void RESET_COUNT_A_SaveConfig(void) ;
void RESET_COUNT_A_RestoreConfig(void) ;
void RESET_COUNT_A_Sleep(void) ; 
void RESET_COUNT_A_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define RESET_COUNT_A_Control        (* (reg8 *) RESET_COUNT_A_Sync_ctrl_reg__CONTROL_REG )
#define RESET_COUNT_A_Control_PTR    (  (reg8 *) RESET_COUNT_A_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_RESET_COUNT_A_H */


/* [] END OF FILE */
