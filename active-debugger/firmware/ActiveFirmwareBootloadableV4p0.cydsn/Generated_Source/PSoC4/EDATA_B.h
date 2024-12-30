/*******************************************************************************
* File Name: EDATA_B.h  
* Version 1.90
*
* Description:
*  This file containts Status Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_STATUS_REG_EDATA_B_H) /* CY_STATUS_REG_EDATA_B_H */
#define CY_STATUS_REG_EDATA_B_H

#include "cyfitter.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))
    #include "cytypes.h"
    #include "CyLib.h"
#else
    #include "syslib/cy_syslib.h"
#endif

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 statusState;

} EDATA_B_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

uint8 EDATA_B_Read(void) ;
void EDATA_B_InterruptEnable(void) ;
void EDATA_B_InterruptDisable(void) ;
void EDATA_B_WriteMask(uint8 mask) ;
uint8 EDATA_B_ReadMask(void) ;


/***************************************
*           API Constants
***************************************/

#define EDATA_B_STATUS_INTR_ENBL    0x10u


/***************************************
*         Parameter Constants
***************************************/

/* Status Register Inputs */
#define EDATA_B_INPUTS              8


/***************************************
*             Registers
***************************************/

/* Status Register */
#define EDATA_B_Status             (* (reg8 *) EDATA_B_sts_sts_reg__STATUS_REG )
#define EDATA_B_Status_PTR         (  (reg8 *) EDATA_B_sts_sts_reg__STATUS_REG )
#define EDATA_B_Status_Mask        (* (reg8 *) EDATA_B_sts_sts_reg__MASK_REG )
#define EDATA_B_Status_Aux_Ctrl    (* (reg8 *) EDATA_B_sts_sts_reg__STATUS_AUX_CTL_REG )

#endif /* End CY_STATUS_REG_EDATA_B_H */


/* [] END OF FILE */