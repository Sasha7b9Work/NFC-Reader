/*----------------------------------------------------------------------------*/
/* Copyright 2020-2021 NXP                                                    */
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
* Example Source for NfcrdlibEx6_LPCD.
* This application will configure Reader Library to provide demo on LPCD functionality of NXP Reader IC's.
* Please refer Readme.txt file  for  Hardware Pin Configuration, Software Configuration and steps to build and
* execute the project which is present in the same project directory.
*
* $Author$
* $Revision$ (v07.05.00)
* $Date$
*/


/**
* Reader Library Headers
*/
#include <phApp_Init.h>
#include <phhalHw_Pn5190_Instr.h>

/* Local headers */
#include "NfcrdlibEx6_LPCD.h"

/*******************************************************************************
**   Global Defines
*******************************************************************************/
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
uint32_t aLPCDTaskBuffer[LPCD_TASK_STACK];
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define aLPCDTaskBuffer     NULL
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

/*******************************************************************************
**   Static Defines
*******************************************************************************/


/*******************************************************************************
**   Function Declaration
*******************************************************************************/
void LPCD_Demo(void * pHalParams);
/*******************************************************************************
**   Function Definitions
*******************************************************************************/

/*******************************************************************************
**   Main Function
*******************************************************************************/

int main (void)
{
    do
    {
        phStatus_t wStatus = PH_ERR_INTERNAL_ERROR;
        phNfcLib_Status_t     dwStatus;
#ifdef PH_PLATFORM_HAS_ICFRONTEND
        phNfcLib_AppContext_t AppContext = {0};
#endif /* PH_PLATFORM_HAS_ICFRONTEND */

#ifndef PH_OSAL_NULLOS
        phOsal_ThreadObj_t LPCDTask;
#endif /* PH_OSAL_NULLOS */

        /* Perform Controller specific initialization. */
        phApp_CPU_Init();

        /* Perform OSAL Initialization. */
        (void)phOsal_Init();

        DEBUG_PRINTF("\n LPCD Example: ");

#ifdef PH_PLATFORM_HAS_ICFRONTEND
        wStatus = phbalReg_Init(&sBalParams, sizeof(phbalReg_Type_t));
        CHECK_STATUS(wStatus);

        AppContext.pBalDataparams = &sBalParams;
        dwStatus = phNfcLib_SetContext(&AppContext);
        CHECK_NFCLIB_STATUS(dwStatus);
#endif /* PH_PLATFORM_HAS_ICFRONTEND */
        /* Initialize library */
        dwStatus = phNfcLib_Init();
        CHECK_NFCLIB_STATUS(dwStatus);
        if(dwStatus != PH_NFCLIB_STATUS_SUCCESS) break;

        /* Set the generic pointer */
        pHal = phNfcLib_GetDataParams(PH_COMP_HAL);

        /* Perform Platform Init */
        wStatus = phApp_Configure_IRQ();
        CHECK_STATUS(wStatus);
        if(wStatus != PH_ERR_SUCCESS) break;

#ifndef PH_OSAL_NULLOS

        LPCDTask.pTaskName = (uint8_t *) "LPCDTask";
        LPCDTask.pStackBuffer = aLPCDTaskBuffer;
        LPCDTask.priority = LPCD_TASK_PRIO;
        LPCDTask.stackSizeInNum = LPCD_TASK_STACK;
        phOsal_ThreadCreate(&LPCDTask.ThreadHandle, &LPCDTask, &LPCD_Demo, pHal);

        phOsal_StartScheduler();

        DEBUG_PRINTF("RTOS Error : Scheduler exited. \n");

#else
        LPCD_Demo(pHal);
#endif /* PH_OSAL_NULLOS */
    } while(0);

    while(1); //Comes here if initialization failure or scheduler exit due to error

    return 0;
}

void LPCD_Demo(void * pHalParams)
{
    phStatus_t  wStatus;
    uint32_t   dwLPCDCalibrateOption;
    uint32_t   dwLPCDDemoOption;
    uint32_t   dwLPCDRefValue;
    uint16_t   wLPCDWakeUpTime = LPCD_POWERDOWN_TIME;

    /* Perform LPCD Calibration */
    wStatus = Configure_LPCD(&dwLPCDCalibrateOption, &dwLPCDDemoOption);
    CHECK_STATUS(wStatus);

    /* Perform LPCD Calibration */
    wStatus = Calibrate_LPCD(dwLPCDDemoOption, &dwLPCDRefValue);
    CHECK_STATUS(wStatus);

    do
    {
        /* Perform LPCD Call */
        wStatus = DemoLPCD(dwLPCDDemoOption, dwLPCDRefValue, wLPCDWakeUpTime);
        CHECK_STATUS(wStatus);

        if (dwLPCDCalibrateOption == 1)
        {
            /* Perform LPCD Calibration */
            wStatus = Calibrate_LPCD(dwLPCDDemoOption, &dwLPCDRefValue);
            CHECK_STATUS(wStatus);
        }
    }while(1);
}


/******************************************************************************
**                            End Of File
******************************************************************************/
