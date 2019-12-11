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

#ifndef _MI_UBI_H_
#define _MI_UBI_H_

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
//  Defines
//-------------------------------------------------------------------------------------------------
#define MAX_VOLUME_NAME         (32)
#define MI_UBI_MAX_VOLUME       (32)
#define MAX_PARTITION_NAME      (32)

//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------
typedef struct MI_UBI_InitParam_s
{
    MI_U32 u32Reserved;
} MI_UBI_InitParam_t;

typedef struct MI_UBI_OpenParam_s
{
    MI_U32 u32DeviceId;
} MI_UBI_OpenParam_t;

typedef struct MI_UBI_VolumeAccess_s
{
    MI_U32 u32VolId;
    MI_U32 u32ByteOffset;
    MI_U8 *pu8Buf;
    MI_U32 u32Length;
} MI_UBI_VolumeAccess_t;

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief UBI initialization
/// @param[in] pstInitParam: Init parameter
/// @return MI_OK: Process success.
/// @return MI_HAS_INITED: Module had inited.
/// @return MI_ERR_FAILED: Processing fail.
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_Init(MI_UBI_InitParam_t *pstInitParam);

//------------------------------------------------------------------------------
/// @brief Finalize UBI module
/// @param[in] None
/// @return MI_OK: Process success.
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_DeInit(void);

//------------------------------------------------------------------------------
/// @brief Attach UBI device
/// @param[in] pu8PartName: Partition name
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_Attach(MI_U8 *pu8PartName);

//------------------------------------------------------------------------------
/// @brief Detach UBI device
/// @param[in] pu8PartName: Partition name
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_Detach(MI_U8 *pu8PartName);

//------------------------------------------------------------------------------
/// @brief Get Device Id by partition name
/// @param[in] pu8PartName: Partition name
/// @param[out] pu32DevId: pointer to get the relative device id
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_GetDevIdByPartName(MI_U8 *pu8PartName, MI_U32 *pu32DevId);

//------------------------------------------------------------------------------
/// @brief UBI device open
/// @param[in] pstOpenParam: open parameters to open
/// @param[out] phUbiDevice: return ubi handle after opening success
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_RESOURCES: No available resource.
/// @return MI_ERR_FAILED: Process failed
/// @return MI_ERR_MEMORY_ALLOCATE: Allocate memory fail
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_Open(MI_UBI_OpenParam_t *pstOpenParam, MI_HANDLE *phUbiDevice);

//------------------------------------------------------------------------------
/// @brief UBI device close
/// @param[in] hUbiDevice: The ubi handle to close
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_Close(MI_HANDLE hUbiDevice);

//------------------------------------------------------------------------------
/// @brief Create a new volume
/// @param[in] hUbiDevice: The ubi handle to process
/// @param[in] volSize Volume size
/// @param[in] u64VolByteSize: Volume size to create
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_CreateVolume(MI_HANDLE hUbiDevice, MI_U8 aszVolName[MAX_VOLUME_NAME], MI_U64 u64VolByteSize);

//------------------------------------------------------------------------------
/// @brief Destroy the volume
/// @param[in] hUbiDevice: The ubi handle to process
/// @param[in] u32VolId: The volume id to destroy
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_DestroyVolume(MI_HANDLE hUbiDevice, MI_U32 u32VolId);

//------------------------------------------------------------------------------
/// @brief Get the Blocksize of the ubi handle
/// @param[in] hUbiDevice: The ubi handle to process
/// @param[out] *pu32BlockSize: pointer to get the block size
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_GetBlockSize(MI_HANDLE hUbiDevice, MI_U32 *pu32BlockSize);

//------------------------------------------------------------------------------
/// @brief Get UBI total size
/// @param[in] hUbiDevice: The ubi handle to process
/// @param[out] *pu32TotalSize: pointer to get the total size
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_U32 MI_UBI_GetTotalSize(MI_HANDLE hUbiDevice, MI_U64 *pu64TotalSize);

//------------------------------------------------------------------------------
/// @brief Get the total volume count of the ubi handle
/// @param[in] hUbiDevice: The ubi handle to process
/// @param[out] *pu32VolumeCount: pointer to get the count result
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_GetVolumeCount(MI_HANDLE hUbiDevice, MI_U32 *pu32VolumeCount);

//------------------------------------------------------------------------------
/// @brief Get UBI volume name by volume id
/// @param[in] hUbiDevice: The ubi handle to process
/// @param[in] u32VolId: The volume id to resize
/// @param[out] aszVolumeName: return the volume name
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_GetVolumeNameById(MI_HANDLE hUbiDevice, MI_U32 u32VolId, MI_U8 aszVolumeName[MAX_VOLUME_NAME]);

//------------------------------------------------------------------------------
/// @brief Get UBI volume id by volume name
/// @param[in] hUbiDevice: The ubi handle to process
/// @param[in] pu8VolumeName: The volume name
/// @param[out] aszVolumeName: return the volume id
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_GetVolumeIdByName(MI_HANDLE hUbiDevice, MI_U8 *pu8VolumeName, MI_U32 *pu32VolId);

//------------------------------------------------------------------------------
/// @brief Get UBI volume size
/// @param[in] hUbiDevice: The ubi handle to process
/// @param[in] u32VolId: The volume id to get size
/// @param[out] pu64VolumeByteSize: Pointer to get the volume size in byte
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_U32 MI_UBI_GetVolumeSize(MI_HANDLE hUbiDevice, MI_U32 u32VolId, MI_U64 *pu64VolumeByteSize);

//------------------------------------------------------------------------------
/// @brief Get UBI volume block count
/// @param[in] hUbiDevice: The ubi handle to process
/// @param[in] u32VolId: The volume id to get block count
/// @param[out] pu64VolumeByteSize: Pointer to get the block count
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_GetVolumeBlockCount(MI_HANDLE hUbiDevice, MI_U32 u32VolId, MI_U32 *pu32BlockCount);

//------------------------------------------------------------------------------
/// @brief Change UBI volume size
/// @param[in] hUbiDevice: The ubi handle to process
/// @param[in] u32VolId: The volume id to resize
/// @param[in] s32Size: The new volume size
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_ResizeVolume(MI_HANDLE hUbiDevice, MI_U32 u32VolId, MI_S32 s32Size);

//------------------------------------------------------------------------------
/// @brief Read data from the head of the specific volume LEB
/// @param[in/out] pstReadBytes: The structure pointer to read data
/// @return -1: Process fail.
/// @return >=0: The length of readed data.
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_S32 MI_UBI_ReadVolumeBytes(MI_HANDLE hUbiDevice, MI_UBI_VolumeAccess_t *pstReadBytes);

//------------------------------------------------------------------------------
/// @brief Write data from the head of the specific volume LEB
/// @param[in] pstWriteBytes: The structure pointer to write data
/// @return -1: Process fail.
/// @return >=0: The length of written data.
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_S32 MI_UBI_WriteVolumeBytes(MI_HANDLE hUbiDevice, MI_UBI_VolumeAccess_t *pstWriteBytes);

//------------------------------------------------------------------------------
/// @brief Check if the volume exist by volume id
/// @param[in] hUbiDevice: The ubi handle to process
/// @param[in] u32VolId: The volume id to check
/// @param[out] pbExist: pointer to get the check result
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process failed
/// @sa
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_CheckVolumeExist(MI_HANDLE hUbiDevice, MI_U32 u32VolId, MI_BOOL *pbExist);

//------------------------------------------------------------------------------
/// @brief Set UBI debug level.
/// @param[in] eDbgLevel.
/// @return MI_OK: Set debug level success.
//------------------------------------------------------------------------------
MI_RESULT MI_UBI_SetDebugLevel(MI_DBG_LEVEL_e eDbgLevel);

#ifdef __cplusplus
}
#endif

#endif///_MI_UBI_H_

