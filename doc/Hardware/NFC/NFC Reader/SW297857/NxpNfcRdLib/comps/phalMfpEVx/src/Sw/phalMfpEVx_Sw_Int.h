/*----------------------------------------------------------------------------*/
/* Copyright 2013-2020 NXP                                                    */
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
* Internal functions of Software implementation for MIFARE Plus EVx contactless IC (Ev1, and future versions) contactless IC application layer.
* $Author: NXP99556 $
* $Revision: 3340 $ (v07.05.00)
* $Date: 2017-04-05 16:07:29 +0530 (Wed, 05 Apr 2017) $
*
* History:
*  Kumar GVS: Generated 15. Apr 2013
*
*/

#ifndef PHALMFPEVX_SW_INT_H
#define PHALMFPEVX_SW_INT_H

#include <ph_Status.h>
#include <phalMfpEVx.h>

#define PHAL_MFPEVX_TAPEOUT_VERSION            30U

/**
* \brief Perform a complete MIFARE Plus contactless IC Authentication for either Security Level.
*
* Refer to the respective Authentication function for description.
* \see phalMfpEVx_AuthenticateSL0
* \see phalMfpEVx_AuthenticateSL1
* \see phalMfpEVx_AuthenticateSL2
* \see phalMfpEVx_AuthenticateSL3
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfpEVx_Sw_AuthenticateGeneral(
    phalMfpEVx_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bLayer4Comm,                        /**< [In] \c 0: use ISO14443-3 protocol; \c 1: use ISO14443-4 protocol; */
    uint8_t bFirstAuth,                         /**< [In] \c 0: Following Authentication; \c 1: First Authentication; */
    uint16_t wBlockNr,                          /**< [In] Key Block number. */
    uint16_t wKeyNumber,                        /**< [In] Key Storage number. */
    uint16_t wKeyVersion,                       /**< [In] Key Storage version. */
    uint8_t bLenDivInput,                       /**< [In] Length of diversification input used to diversify the key. If 0, no diversification is performed. */
    uint8_t * pDivInput,                        /**< [In] Diversification Input used to diversify the key. */
    uint8_t bLenPcdCap2,                        /**< [In] Lengh of the supplied PCDCaps. */
    uint8_t * pPcdCap2In,                       /**< [In] Pointer to PCDCaps (bLenPcdCap2 bytes), ignored if bLenPcdCap2 == 0. */
    uint8_t * pPcdCap2Out,                      /**< [In] Pointer to PCDCaps sent from the card (6 bytes). */
    uint8_t * pPdCap2                           /**< [Out] Pointer to PDCaps sent from the card (6 bytes). */
    );

/**
* \brief Perform a Write command in all it's flavours in ISO14443 Layer 4 activated state.
*
* Refer to the respective Write function for description.
* \see phalMfpEVx_Write
* \see phalMfpEVx_Increment
* \see phalMfpEVx_Decrement
* \see phalMfpEVx_IncrementTransfer
* \see phalMfpEVx_DecrementTransfer
* \see phalMfpEVx_Transfer
* \see phalMfpEVx_Restore
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfpEVx_Sw_WriteExt(
                               phalMfpEVx_Sw_DataParams_t * pDataParams,/**< [In] Pointer to this layer's parameter structure. */
                               uint8_t bCmdCode,                        /**< [In] MIFARE Plus Command Code. */
                               uint16_t wSrcBnr,                        /**< [In] Source Block number. */
                               uint16_t wDstBnr,                        /**< [In] Destination Block number. */
                               uint8_t * pData,                         /**< [In] Payload data. */
                               uint16_t wDataLength,                    /**< [In] Length of payload data. */
                               uint8_t bEncrypted,                      /**< [In] indicates whether the data should be encrypted or not. */
                               uint8_t * pTMC,                          /**< [Out] 4 byte TMAC counter. */
                               uint8_t * pTMV                           /**< [Out] 8 byte TMAC value. */
                               );

/**
* \brief Perform a Read command in all it's flavours in ISO14443 Layer 4 activated state.
*
* Refer to the respective Write function for description.
* \see phalMfpEVx_Read
* \see phalMfpEVx_ReadValue
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfpEVx_Sw_ReadExt(
                               phalMfpEVx_Sw_DataParams_t * pDataParams,/**< [In] Pointer to this layer's parameter structure. */
                               uint8_t bEncrypted,                      /**< [In] \c 0: Plain communication; \c 1: Encrypted communication; */
                               uint8_t bReadMaced,                      /**< [In] \c 0: No MAC on response; \c 1: MAC on response; */
                               uint8_t bMacOnCmd,                       /**< [In] \c 0: No MAC on command; \c 1: MAC on command; */
                               uint16_t wBlockNr,                       /**< [In] MIFARE block number. */
                               uint8_t bNumBlocks,                      /**< [In] Number of blocks to read. */
                               uint8_t * pBlocks                        /**< [Out] Block(s) (16*bNumBlocks bytes).  */
                               );

/**
* \brief Calculate the Init-Vector for encryption from input data.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalMfpEVx_Sw_Int_ComputeIv(
                                phalMfpEVx_Sw_DataParams_t * pDataParams,/**< [In] Pointer to this layer's parameter structure. */
                                uint8_t bIsResponse,                     /**< [In] Whether this is a response or command or not. */
                                uint8_t * pTi,                           /**< [In] Transaction Identifier; uint8_t[4]. */
                                uint16_t wRCtr,                          /**< [In] R_CTR (read counter). */
                                uint16_t wWCtr,                          /**< [In] W_CTR (write counter). */
                                uint8_t * pIv                            /**< [Out] Initvector; uint8_t[16]. */
                                );

/**
* \brief Performs session encryption and session MAC key for EV0 secure messaging.
*
* This function derives the MIFARE Sector Key.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfpEVx_Sw_Int_KDF_EV0(
                                 phalMfpEVx_Sw_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                 uint8_t * pRndA,                           /**< [In] Pointer to RndA Buffer. */
                                 uint8_t * pRndB                            /**< [In] Pointer to RndB Buffer. */
                                 );

/**
* \brief Performs session encryption and session MAC key for EV1 secure messaging.
*
* This function derives the MIFARE Sector Key.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfpEVx_Sw_Int_KDF_EV1(
                                 phalMfpEVx_Sw_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                 uint8_t * pRndA,                           /**< [In] Pointer to RndA Buffer. */
                                 uint8_t * pRndB                            /**< [In] Pointer to RndB Buffer. */
                                 );

/**
* \brief Perform MIFARE Plus specific MAC truncation.
*/
void phalMfpEVx_Sw_Int_TruncateMac(
                                uint8_t * pMac,             /**< [In] MAC; uint8_t[16]. */
                                uint8_t * pTruncatedMac     /**< [Out] Truncated MAC; uint8_t[8]. */
                                );

/**
* \brief Perform a Write command in all it's flavours in ISO14443 Layer 3 activated state.
*
* Refer to the respective Write function for description.
* \see phalMfpEVx_Write
* \see phalMfpEVx_WriteValue
* \see phalMfpEVx_Increment
* \see phalMfpEVx_Decrement
* \see phalMfpEVx_Transfer
* \see phalMfpEVx_Restore
* \see phalMfpEVx_IncrementTransfer
* \see phalMfpEVx_DecrementTransfer
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfpEVx_Sw_WriteExtMfc(
                               phalMfpEVx_Sw_DataParams_t * pDataParams,/**< [In] Pointer to this layer's parameter structure. */
                               uint8_t bCmdCode,                        /**< [In] MIFARE Plus EVx Command Code n Classic mode. */
                               uint8_t bBlockNo,                        /**< [In] Block number. */
                               uint8_t * pData,                         /**< [In] Payload data. */
                               uint16_t wDataLength,                    /**< [In] Length of payload data. */
                               uint8_t * pTMC,                          /**< [Out] 4 byte TMAC counter. */
                               uint8_t * pTMV                           /**< [Out] 8 byte TMAC value. */
                               );
/**
* \brief Perform a Read command in all it's flavours in ISO14443 Layer 3 activated state.
*
* Refer to the respective Write function for description.
* \see phalMfpEVx_Read
* \see phalMfpEVx_ReadValue
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalMfpEVx_Sw_ReadExtMfc(
                               phalMfpEVx_Sw_DataParams_t * pDataParams,/**< [In] Pointer to this layer's parameter structure. */
                               uint8_t bBlockNo,                        /**< [In] Block number. */
                               uint8_t * pData                          /**< [Out] Payload data. */
                               );

#endif /* PHALMFPEVX_SW_INT_H */
