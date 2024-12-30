/*******************************************************************************
* File Name: DMA_BufferFillA1.h
* Version 1.0
*
* Description:
*  This file contains all of the channel DMA API functions.
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "DMA_BufferFillA1.h"

uint8 DMA_BufferFillA1_initVar = 0u;


/*******************************************************************************
* Function Name: DMA_BufferFillA1_Start
********************************************************************************
*
* Summary:
*  Calls Init() to configure the DMA channel based on the customizer settings if
*  the channel has not been initialized before.
*  Sets source and destination address, validates descriptor 0 and enables the
*  channel. After calling this function the DMA channel is active and waiting
*  for a trigger.
*
* Parameters:
*  srcAddress: Address of DMA transfer source for descriptor 0.
*  dstAddress: Address of DMA transfer destination for descriptor 0.
*                   
* Return:
*  None.
*
*******************************************************************************/
void DMA_BufferFillA1_Start(void * srcAddress, void * dstAddress)
{
    if(0u == DMA_BufferFillA1_initVar)
    {
        DMA_BufferFillA1_Init();
        DMA_BufferFillA1_initVar = 1u;
    }
    CyDmaEnable();
    DMA_BufferFillA1_SetSrcAddress(0, srcAddress);
    DMA_BufferFillA1_SetDstAddress(0, dstAddress);
    DMA_BufferFillA1_ValidateDescriptor(0);
    DMA_BufferFillA1_ChEnable();
}


/*******************************************************************************
* Function Name: DMA_BufferFillA1_Init
********************************************************************************
*
* Summary:
*  Initializes the DMA channel based on the parameters set in the component
*  customizer. It is not necessary to call DMA_BufferFillA1_Init() because the
*  DMA_BufferFillA1_Start() API calls this function, which is the preferred method
*  to begin component operation.
*
* Parameters:
*  None.
*                   
* Return:
*  None.
*
* Side effect:
*  The first descriptor is set to descriptor 0. This function should not be
*  called while the channel is enabled. All settings will be reset to their
*  initial values.
*
*******************************************************************************/
void DMA_BufferFillA1_Init(void)
{
    CYDMA_CH_CTL_BASE.ctl[DMA_BufferFillA1_CHANNEL] = DMA_BufferFillA1_CHANNEL_CFG;
    CYDMA_DESCR_BASE.descriptor[DMA_BufferFillA1_CHANNEL][0].ctl = DMA_BufferFillA1_DESCR0_CFG;
    CYDMA_DESCR_BASE.descriptor[DMA_BufferFillA1_CHANNEL][1].ctl = DMA_BufferFillA1_DESCR1_CFG;

    /* If the Generate IRQ option is selected for descriptor 0 or 1, set the
    * channel bit field in the DMA interrupt mask register.
    */
    #if (0u != ((DMA_BufferFillA1_DESCR0_CFG | DMA_BufferFillA1_DESCR1_CFG) & CYDMA_GENERATE_IRQ))
        CYDMA_INTR_MASK_REG |= DMA_BufferFillA1_CHANNEL_MASK;
    #endif /* (0u != ((DMA_BufferFillA1_DESCR0_CFG | DMA_BufferFillA1_DESCR1_CFG) & CYDMA_GENERATE_IRQ)) */
}


/* [] END OF FILE */
