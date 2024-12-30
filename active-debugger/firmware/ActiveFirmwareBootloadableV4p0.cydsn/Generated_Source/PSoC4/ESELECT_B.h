/*******************************************************************************
* File Name: ESELECT_B.h  
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

#if !defined(CY_STATUS_REG_ESELECT_B_H) /* CY_STATUS_REG_ESELECT_B_H */
#define CY_STATUS_REG_ESELECT_B_H

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

} ESELECT_B_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

uint8 ESELECT_B_Read(void) ;
void ESELECT_B_InterruptEnable(void) ;
void ESELECT_B_InterruptDisable(void) ;
void ESELECT_B_WriteMask(uint8 mask) ;
uint8 ESELECT_B_ReadMask(void) ;


/***************************************
*           API Constants
***************************************/

#define ESELECT_B_STATUS_INTR_ENBL    0x10u


/***************************************
*         Parameter Constants
***************************************/

/* Status Register Inputs */
#define ESELECT_B_INPUTS              1


/***************************************
*             Registers
***************************************/

/* Status Register */
#define ESELECT_B_Status             (* (reg8 *) ESELECT_B_sts_sts_reg__STATUS_REG )
#define ESELECT_B_Status_PTR         (  (reg8 *) ESELECT_B_sts_sts_reg__STATUS_REG )
#define ESELECT_B_Status_Mask        (* (reg8 *) ESELECT_B_sts_sts_reg__MASK_REG )
#define ESELECT_B_Status_Aux_Ctrl    (* (reg8 *) ESELECT_B_sts_sts_reg__STATUS_AUX_CTL_REG )

#endif /* End CY_STATUS_REG_ESELECT_B_H */


/* [] END OF FILE */
