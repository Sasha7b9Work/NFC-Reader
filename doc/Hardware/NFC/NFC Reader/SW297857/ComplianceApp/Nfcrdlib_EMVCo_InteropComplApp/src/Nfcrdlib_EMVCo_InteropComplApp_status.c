/*----------------------------------------------------------------------------*/
/* Copyright 2017-2020,2022 NXP                                               */
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
* Example Source file for EMVCo Interoperability test status.
* This file performs initialization of GPOI's and LED indication for EMVCo Interoperability loopback test status.
* Please refer Readme.txt file for Hardware Pin Configuration, Software Configuration and steps to build and
* execute the project which is present in the same project directory.
* $Author: NXP $
* $Revision: $ (v07.05.00)
* $Date: $
*
*/
#include "phApp_Init.h"
#ifndef NXPBUILD__PHHAL_HW_PN7462AU
#include "BoardSelection.h"
#else
#include "phhalGpio.h"
#include "ph_Reg.h"
#endif

#if defined(PHDRIVER_PIPN5180_BOARD) || defined(PHDRIVER_PIRC663_BOARD) || defined(PHDRIVER_PIPN5190_BOARD)
#include <time.h>
#include <stdlib.h>
#endif

#include "Nfcrdlib_EMVCo_InteropComplApp.h"
#include "Nfcrdlib_EMVCo_InteropComplApp_status.h"

/***********************************************************************************************
 * \brief   This function performs initialization of LED pins and GPIO's to indicate Interoperability
 *          test status.
 * \param   none
 * \return  status  Returns the function status
 **********************************************************************************************/
phStatus_t Emvco_Interop_Init(void)
{
#if !defined(PHDRIVER_PIPN5180_BOARD) && !defined(PHDRIVER_PIRC663_BOARD) && !defined(PHDRIVER_PIPN5190_BOARD)

#ifndef NXPBUILD__PHHAL_HW_PN7462AU
    phStatus_t statusTmp;
    phDriver_Pin_Config_t pinCfg;

    pinCfg.bPullSelect = PH_DRIVER_PULL_DOWN;
    pinCfg.bOutputLogic = EMVCO_INTEROP_LED_OFF;
    PH_CHECK_SUCCESS_FCT(statusTmp, phDriver_PinConfig(PHDRIVER_PIN_RLED, PH_DRIVER_PINFUNC_OUTPUT, &pinCfg));

    pinCfg.bPullSelect = PH_DRIVER_PULL_DOWN;
    pinCfg.bOutputLogic = EMVCO_INTEROP_LED_OFF;
    PH_CHECK_SUCCESS_FCT(statusTmp, phDriver_PinConfig(PHDRIVER_PIN_GLED, PH_DRIVER_PINFUNC_OUTPUT, &pinCfg));

    pinCfg.bPullSelect = PH_DRIVER_PULL_DOWN;
    pinCfg.bOutputLogic = PH_DRIVER_SET_LOW;
    PH_CHECK_SUCCESS_FCT(statusTmp, phDriver_PinConfig(PHDRIVER_PIN_SUCCESS, PH_DRIVER_PINFUNC_OUTPUT, &pinCfg));

    pinCfg.bPullSelect = PH_DRIVER_PULL_DOWN;
    pinCfg.bOutputLogic = PH_DRIVER_SET_LOW;
    PH_CHECK_SUCCESS_FCT(statusTmp, phDriver_PinConfig(PHDRIVER_PIN_FAIL, PH_DRIVER_PINFUNC_OUTPUT, &pinCfg));
#else
    phhalPcr_ConfigOutput(EMVCO_INTEROP_GREEN_LED, true, false);
    phhalPcr_ConfigOutput(EMVCO_INTEROP_RED_LED, true, false);
    phhalPcr_ConfigOutput(EMVCO_INTEROP_STAT_SUCCESS, true, false);
    phhalPcr_ConfigOutput(EMVCO_INTEROP_STAT_FAIL, true, false);
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#else
    system("echo none | sudo tee /sys/class/leds/led0/trigger >/dev/null");
    system("echo 0 | sudo tee /sys/class/leds/led0/brightness >/dev/null");
#endif /* !defined(PHDRIVER_PIPN5180_BOARD) && !defined(PHDRIVER_PIRC663_BOARD) && !defined(PHDRIVER_PIPN5190_BOARD) */

    return PH_ERR_SUCCESS;
}

/*************************************************************************************************
 * \brief   This function performs LED's and GPIO's indication for Interoperability test status.
 *          CLEV6630B 2.0, PNEV5180B v2.0, PNEV7462C and FRDM-K82F Board:
 *              Success: Green LED On, GPIO High, 500 milli sec delay, GPIO Low and LED Off.
 *              Fail   : RED LED On, GPIO High, 500 milli sec delay, GPIO Low and LED Off.
 *          Raspberry Pi:
 *              Success: Green LED On, 2 sec delay and LED Off.
 *              Fail   : Green LED Toggles every 250 milli sec for 2 sec and LED Off.
 * \param   Error_Status EMVCO_INTEROP_ERR_SUCCESS or EMVCO_INTEROP_ERR_FAIL
 * \return  none
 *
 * Note: - CLEV6630B 2.0 or PNEV5180B v2.0 Customer Evaluation Board LED and GPIO's indications:
 *         Red LED - LD201 and Green LED - LD203
 *         Success indication - TX(Port0, pin2) and Fail indication - RX(Port0, pin3)
 *       - PNEV7462C Board LED and GPIO's indications:
 *         Red LED - LED12 and Green LED - LED10
 *         Success indication - GPIO1 and Fail indication - GPIO2
 *       - FRDM-K82F Board LED and GPIO's indications:
 *         Red LED - D3 RGB and Green LED - D3 RGB
 *         Success indication - J4-Pin6(Port C, Pin1) and Fail indication - J4-Pin8(Port C, Pin2)
 *       - PNEV5190B V1.0 Board LED and GPIO's indications:
 *         Red LED - LEDRR and Green LED - LEDGR
 *         Success indication - TP8(PortD, pin12) and Fail indication - TP6(PortD, pin13)
 *       - Raspberry Pi:
 *         ACT LED
 ************************************************************************************************/
void Emvco_Interop_Status(uint8_t Error_Status)
{
#if !defined(PHDRIVER_PIPN5180_BOARD) && !defined(PHDRIVER_PIRC663_BOARD)  && !defined(PHDRIVER_PIPN5190_BOARD)

    if(Error_Status != EMVCO_INTEROP_ERR_FAIL)
    {
        /* Success - Green LED On and GPIO High
         *           500 milli sec delay
         *           GPIO Low and Green LED Off */
        EMVCO_INTEROP_STAT_GLED_ON();
        EMVCO_INTEROP_STAT_SUCCESS_GPIO_ON();
        CHECK_NFCLIB_STATUS(phhalHw_Wait(pHal, PHHAL_HW_TIME_MILLISECONDS, 500U));
        EMVCO_INTEROP_STAT_SUCCESS_GPIO_OFF();
        EMVCO_INTEROP_STAT_GLED_OFF();
    }
    else
    {
        /* Fail - Red LED On and GPIO High
         *        500 milli sec delay
         *        GPIO Low and Red LED Off */
        EMVCO_INTEROP_STAT_RLED_ON();
        EMVCO_INTEROP_STAT_FAIL_GPIO_ON();
        CHECK_NFCLIB_STATUS(phhalHw_Wait(pHal, PHHAL_HW_TIME_MILLISECONDS, 500U));
        EMVCO_INTEROP_STAT_FAIL_GPIO_OFF();
        EMVCO_INTEROP_STAT_RLED_OFF();
    }

#else
    struct timespec sTimeSpecReq;
    struct timespec sTimeSpecRem;
    uint8_t bCnt;

    if(Error_Status != EMVCO_INTEROP_ERR_FAIL)
    {
        /* SUCCESS: ON for 2 sec. */
        system("echo 1 | sudo tee /sys/class/leds/led0/brightness >/dev/null");
        sTimeSpecReq.tv_sec = PHDRIVER_LED_SUCCESS_DELAY;
        sTimeSpecReq.tv_nsec = 0;
        nanosleep(&sTimeSpecReq, &sTimeSpecRem);

        system("echo 0 | sudo tee /sys/class/leds/led0/brightness >/dev/null");
    }
    else
    {
        /* FAILURE: Toggle every 250ms for 2 sec. */
        sTimeSpecReq.tv_sec = 0;
        sTimeSpecReq.tv_nsec = PHDRIVER_LED_FAILURE_DELAY_MS * 1000 * 1000;
        for(bCnt = 0; bCnt < PHDRIVER_LED_FAILURE_FLICKER; bCnt++)
        {
            system("echo 1 | sudo tee /sys/class/leds/led0/brightness >/dev/null");
            nanosleep(&sTimeSpecReq, &sTimeSpecRem);
            system("echo 0 | sudo tee /sys/class/leds/led0/brightness >/dev/null");
            nanosleep(&sTimeSpecReq, &sTimeSpecRem);
        }
    }
#endif /* !defined(PHDRIVER_PIPN5180_BOARD) && !defined(PHDRIVER_PIRC663_BOARD) */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
