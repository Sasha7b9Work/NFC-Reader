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


 /** @file
* Implementation of Operating System Abstraction Layer for PN74xx NFC Controllers.
*
* This file contains TickTimer related functionality implementation required
* by the NullOs.
*
* $Author:: NXP $
* $Revision$
* History:
*/

#include "phOsal.h"
#include "phOsal_NullOs_Port.h"

/**
 * NULL OS implementation of PN74XXXX or PN73XXXX controller is just stub.
 * In NULL OS implementation, RdLib with PN74XXXX or PN73XXXX HAL will not use any phOsal functionality.
 */
#if defined(PH_OSAL_NULLOS) && (defined(__PN74XXXX__) || defined (__PN73XXXX__))

#include "phOsal_Cortex_Port.h"

#ifdef __GNUC__
    #define __ENABLE_IRQ() __asm volatile ("cpsie i")
    #define __DISABLE_IRQ() __asm volatile ("cpsid i")
    #define __WFE() __asm volatile ("wfe")
    #define __SEV() __asm volatile ("sev")
#endif /* __GNUC__ */


#ifdef __ARMCC_VERSION
    #define __ENABLE_IRQ __enable_irq
    #define __DISABLE_IRQ __disable_irq
    #define __WFE __wfe
    #define __SEV __sev
#endif /* __ARMCC_VERSION */

#ifdef __ICCARM__
#   include "intrinsics.h"
#   define __NOP             __no_operation
#   define __ENABLE_IRQ      __enable_interrupt
#   define __DISABLE_IRQ     __disable_interrupt
#endif

phStatus_t phOsal_InitTickTimer(pphOsal_TickTimerISRCallBck_t pTickTimerCallback)
{
    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_StartTickTimer(uint32_t dwTimeMilliSecs)
{
    return PH_OSAL_SUCCESS;
}

phStatus_t phOsal_StopTickTimer(void)
{
    return PH_OSAL_SUCCESS;
}

void phOsal_EnterCriticalSection(void)
{
    __DISABLE_IRQ();
}

void phOsal_ExitCriticalSection(void)
{
    __ENABLE_IRQ();
}

void phOsal_Sleep(void)
{
    __WFE();
}

void phOsal_WakeUp(void)
{
    __SEV();
}



#endif /* defined(PH_OSAL_NULLOS) && (defined(__PN74XXXX__) || defined (__PN73XXXX__)) */

