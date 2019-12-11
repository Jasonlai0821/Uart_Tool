//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>

#ifndef _MI_FLASH_H_
#define _MI_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
//  Defines
//-------------------------------------------------------------------------------------------------
#define MI_FLASH_ENV_SIZE (0x20000)
#define MI_FLASH_ENV_DATA_SIZE (MI_FLASH_ENV_SIZE - sizeof(MI_U32))

#define MI_FLASH_ENV_NAME_LEN_MAX    256
#define MI_FLASH_ENV_VALUE_LEN_MAX  512
#define MI_MTD_MAX_NUM                  (16)

typedef enum
{
    E_MI_FLASH_TYPE_SPI =0,
    E_MI_FLASH_TYPE_NAND,
    E_MI_FLASH_TYPE_MAX,
} MI_FLASH_Type_e;


typedef enum
{
    E_MI_FLASH_NAND_RW_ADDR,
    E_MI_FLASH_NAND_RW_PARTITION,
    E_MI_FLASH_NAND_ACCESS_MAX,
} MI_FLASH_NandAccessType_e;

typedef enum
{
    E_MI_FLASH_NAND_ERASE_ADDR,
    E_MI_FLASH_NAND_ERASE_PARTITION,
    E_MI_FLASH_NAND_ERASE_PHYSICAL_BLOCK,
    E_MI_FLASH_NAND_ERASE_MAX,
} MI_FLASH_NandEraseType_e;
//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------

typedef struct MI_FLASH_EnvInfo_s
{
    MI_U32 u32Crc;//CRC32 over data bytes
    MI_U8 u8Data[MI_FLASH_ENV_DATA_SIZE];
} MI_FLASH_EnvInfo_t;

typedef struct MtdPartition MtdPartition;


typedef struct MI_FLASH_UnfdParam_s
{    
	MI_U8 *pu8NandInfo;    
	MI_U8 *pu8PartitionInfo;
} MI_FLASH_UnfdParam_t;

typedef struct MI_FLASH_InitParam_s
{
    MI_FLASH_UnfdParam_t stUnfdParam;
} MI_FLASH_InitParam_t;

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
///// @brief Init flash module
///// @param[in] : pstInitParam
///// @return MI_OK: Process success.
///// @return MI_HAS_INITED: Module had inited.
///// @return MI_ERR_RESOURCES: No available resource.
///// @sa
///// @note
////-----------------------------------------------------------------------------
MI_RESULT MI_FLASH_Init(MI_FLASH_InitParam_t *pstInitParam);

//------------------------------------------------------------------------------
///// @brief DeInit the SPI flash
///// @param[in] : none
///// @return MI_OK: Process success
///// @sa
///// @note
////-----------------------------------------------------------------------------
MI_RESULT MI_FLASH_DeInit(void);

//------------------------------------------------------------------------------
///// @brief Load current env data from SPI flash
///// @param[in] eType: The flash Type to access
///// @param[out] pstGetCurEnv: pointer to get current env
///// @return MI_OK: Process success
///// @return MI_ERR_FAILED: Process failure
///// @return MI_ERR_INVALID_PARAMETER: Parameter invalid
///// @return MI_ERR_NOT_INITED: Module not inited
///// @sa
///// @note : Load env info from SPI first.
////-----------------------------------------------------------------------------
MI_RESULT MI_FLASH_LoadEnv(MI_FLASH_Type_e eType, MI_FLASH_EnvInfo_t *pstGetCurEnv);

//------------------------------------------------------------------------------
///// @brief MI_FLASH_StoreEnv : Store env data into SPI flash
///// @param[in] eType: The flash Type to access
///// @param[in] pstPutNewEnv: the env info to be stored
///// @return MI_OK: Process success
///// @return MI_ERR_FAILED: Process failure
///// @return MI_ERR_INVALID_PARAMETER: Parameter invalid
///// @return MI_ERR_NOT_INITED: Module not inited
///// @sa
///// @note : Store env info into SPI.
////-----------------------------------------------------------------------------
MI_RESULT MI_FLASH_StoreEnv(MI_FLASH_Type_e eType, MI_FLASH_EnvInfo_t *pstPutNewEnv);

//------------------------------------------------------------------------------
///// @brief Get an environment variable from current env info
///// @param[in] pstEnvInfo: Current env info.
///// @param[in] pszName : Ptr to name str.
///// @param[out] aszValue : Ptr get the env value of the name.
///// @return MI_OK: Process success
///// @return MI_ERR_FAILED: Process failure
///// @return MI_ERR_INVALID_PARAMETER: Parameter invalid
///// @return MI_ERR_NOT_INITED: Module not inited
///// @sa
///// @note : Load env info from SPI first, then get an env variable from env info.
////-----------------------------------------------------------------------------
MI_RESULT MI_FLASH_GetEnvVar(MI_FLASH_EnvInfo_t *pstEnvInfo, MI_U8 *pszName, MI_U8 aszValue[MI_FLASH_ENV_VALUE_LEN_MAX]);

//------------------------------------------------------------------------------
///// @brief Add/Modify an environment variable.
///// @param[in] pstCurEnv: Set new env into  pstEnvInfo structure
///// @param[in] pszName : Ptr to name str.
///// @param[in] aszValue : Pointer to value str.
///// @return MI_OK: Process success
///// @return MI_ERR_FAILED: Process failure
///// @return MI_ERR_NOT_INITED: Module not inited
///// @sa
///// @note : Load env info from SPI first, then modify an env variable in env info .
////-----------------------------------------------------------------------------
MI_RESULT MI_FLASH_SetEnvVar(MI_FLASH_EnvInfo_t *pstCurEnv, MI_U8 *pszName, MI_U8 aszValue[MI_FLASH_ENV_VALUE_LEN_MAX]);

MI_U32 MI_FLASH_MTD_READ(MI_U8 * partitionName, MI_U8 * pu8Buf ,MI_U32 u32StartOffset,MI_U32 u32Length);

MI_U32 MI_FLASH_MTD_WRITE(MI_U8 * partitionName, MI_U8 * pu8Buf ,MI_U32 u32StartOffset,MI_U32 u32Length);

MI_U32 MI_FLASH_CRC32 (MI_U32 u32Crc, const MI_U8 *pu8Buf, MI_U32 u32Len);

//------------------------------------------------------------------------------
///// @brief Set Debug level for flash module
///// @param[in] : eDbgLevel
///// @return MI_OK: Process success.
///// @sa
///// @note
////-----------------------------------------------------------------------------
MI_RESULT MI_FLASH_SetDebugLevel(MI_DBG_LEVEL_e eDbgLevel);

#ifdef __cplusplus
}
#endif

#endif///_MI_FLASH_H_

