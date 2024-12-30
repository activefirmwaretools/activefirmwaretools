/*******************************************************************************
* File Name: RESET_TIMESTAMP.h  
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

#if !defined(CY_CONTROL_REG_RESET_TIMESTAMP_H) /* CY_CONTROL_REG_RESET_TIMESTAMP_H */
#define CY_CONTROL_REG_RESET_TIMESTAMP_H

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

} RESET_TIMESTAMP_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    RESET_TIMESTAMP_Write(uint8 control) ;
uint8   RESET_TIMESTAMP_Read(void) ;

void RESET_TIMESTAMP_SaveConfig(void) ;
void RESET_TIMESTAMP_RestoreConfig(void) ;
void RESET_TIMESTAMP_Sleep(void) ; 
void RESET_TIMESTAMP_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define RESET_TIMESTAMP_Control        (* (reg8 *) RESET_TIMESTAMP_Sync_ctrl_reg__CONTROL_REG )
#define RESET_TIMESTAMP_Control_PTR    (  (reg8 *) RESET_TIMESTAMP_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_RESET_TIMESTAMP_H */


/* [] END OF FILE */
