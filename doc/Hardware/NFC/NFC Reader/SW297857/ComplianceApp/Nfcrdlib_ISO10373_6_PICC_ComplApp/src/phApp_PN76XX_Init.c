/*----------------------------------------------------------------------------*/
/* Copyright 2021 NXP                                                         */
/*                                                                            */
/* NXP Confidential. This software is owned or controlled by NXP and may only */
/* be used strictly in accordance with the applicable license terms.          */
/* By expressly accepting such terms or by downloading, installing,           */
/* activating and/or otherwise using the software, you are agreeing that you  */
/* have read, and that you agree to comply with and are bound by, such        */
/* license terms. If you do not agree to be bound by the applicable license   */
/* terms, then you may not retain, install, activate or otherwise use the     */
/* software.                                                                  */
/*----------------------------------------------------------------------------*/

/** \file
* Example Source abstracting component data structure and code initialization and code specific to HW used in the examples
* This file shall be present in all examples. A customer does not need to touch/modify this file. This file
* purely depends on the phNxpBuild_Lpc.h or phNxpBuild_App.h
* The phAppInit.h externs the component data structures initialized here that is in turn included by the core examples.
* The core example shall not use any other variable defined here except the RdLib component data structures(as explained above)
* The RdLib component initialization requires some user defined data and function pointers.
* These are defined in the respective examples and externed here.
*
* Keystore and Crypto initialization needs to be handled by application.
*
* $Author: NXP $
* $Revision: $ (v07.05.00)
* $Date: $
*
*/

#include <ph_Status.h>

#include "phApp_Init.h"

#ifdef NXPBUILD__PHHAL_HW_PN76XX
#include "phOsal.h"
#include "PN76xx.h"
#include "PN76_Eeprom.h"
#include "Clif.h"

/*********************************************************************************************************************/
/*   LOCAL DEFINES                                                                                                   */
/*********************************************************************************************************************/

/*******************************************************************************
**   Function Declarations
*******************************************************************************/
void phApp_CPU_Init(void);

/*******************************************************************************
**   Global Variable Declaration
*******************************************************************************/
phhalHw_Pn76xx_DataParams_t   * pHal;

#ifdef NXPBUILD__PHHAL_HW_TARGET
/* Parameters for L3 activation during Autocoll */
extern uint8_t  sens_res[2]    ;
extern uint8_t  nfc_id1[3]     ;
extern uint8_t  sel_res        ;
extern uint8_t  nfc_id3        ;
extern uint8_t  poll_res[18]   ;
#endif /* NXPBUILD__PHHAL_HW_TARGET */

/*******************************************************************************
**   Function Definitions
*******************************************************************************/
/**
* This function will initialize NXP NFC Controller.
*/
void phApp_CPU_Init(void)
{
#ifdef SEGGER_RTT_ENABLE
    /* SEGGER RTT Init */
    memset((uint8_t *)PN76_SEGGER_RTT_MEMORY_ALLOCATION_ADDRESS, 0, 0x4b8);
    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
#endif
   /* To enable NS RF IRQ */
   NVIC_EnableIRQ(CLIF_RF_IRQn);

   /* To enable NS TIMER IRQ */
   NVIC_EnableIRQ(TIMER_IRQn);

   /* Initialize PMU */
   PMU_Init();

   /* Initialize SMU */
   Smu_Init();

   /* Initialize CLKGEN  */
   CLKGEN_Init();
}

/**
* This function will initialize Hal Target Config
*/
phStatus_t phApp_HALConfigAutoColl(void)
{
#ifdef NXPBUILD__PHHAL_HW_TARGET
   phStatus_t wStatus;
   uint8_t aCmd[1] = {0x01};   /* Enable Random UID feature on PN76XX. */

   /* Set Listen Parameters in HAL Buffer used during Autocoll */
   wStatus = phhalHw_Pn76xx_SetListenParameters(
       pHal,
       &sens_res[0],
       &nfc_id1[0],
       sel_res,
       &poll_res[0],
       nfc_id3);
   CHECK_SUCCESS(wStatus);

   /* Enabling the Random UID in PN76XX EEPROM */
   wStatus = PN76_WriteEeprom(
       aCmd,
       PHHAL_HW_PN76XX_DYNAMIC_UID_CONFG_ADDR,
       0x01
       );

   CHECK_SUCCESS(wStatus);
#endif /* NXPBUILD__PHHAL_HW_TARGET */

    return PH_ERR_SUCCESS;
}

/* Configure LPCD (for PN7462AU) */
phStatus_t phApp_ConfigureLPCD(void)
{
    return PH_ERR_SUCCESS;
}
#endif /* NXPBUILD__PHHAL_HW_PN76XX */

/******************************************************************************
**                            End Of File
******************************************************************************/
