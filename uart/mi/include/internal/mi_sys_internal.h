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

#ifndef _MI_SYS_INTERNAL_H_
#define _MI_SYS_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
//  Defines
//-------------------------------------------------------------------------------------------------
#define MI_SYS_CUSTOMER_INFO_SIZE   49
#define MI_SYS_CUSTOMER_ID_SIZE         32
#define MI_SYS_CUSTOMER_ID_SIZE_2       34
#define MI_SYS_CUSTOMER_HASH_SIZE   16
#define MI_SYS_IP_CTRL_MAPPING_SIZE     8

#define MI_SYS_MMAP_INFO_MAX    8

#define MI_SYS_SDK_MMAP_LAYOUT_NUM          (E_MI_SYS_MMAP_ID_MAX - E_MI_SYS_MMAP_ID_MIN)
#define MI_SYS_CUST_MMAP_LAYOUT_NUM         (E_MI_SYS_MMAP_ID_CUSTOM_MAX - E_MI_SYS_MMAP_ID_CUSTOM_MIN)
#define MI_SYS_CA_POOL_MMAP_LAYOUT_NUM      (E_MI_SYS_MMAP_ID_CA_POOL_BUF_MAX - E_MI_SYS_MMAP_ID_CA_POOL_BUF_MIN)
#define MI_SYS_MMAP_LAYOUT_NUM              (MI_SYS_SDK_MMAP_LAYOUT_NUM + MI_SYS_CUST_MMAP_LAYOUT_NUM + MI_SYS_CA_POOL_MMAP_LAYOUT_NUM)

//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------
typedef struct mi_sys_MmapPoolInfo_s
{
    MI_BOOL bIsUsed;
    MI_U8 u8MiuSel;
    MI_BOOL bIsCached;
    MI_U32 u32MmapSize;
    MI_U32 u32VirAddr;
    MI_U32 u32PhyAddr;

}mi_sys_MmapPoolInfo_t;

typedef struct mi_sys_MmapLayout_s
{
    MI_SYS_MmapId_e eMmapId;                    ///< enum type of mmap ID
    MI_PHY phyMemAddr;                          ///< start address of physical memory
    MI_U32 u32MemLen;                           ///< size of memory
    MI_SYS_MiuType_e eMiuType;                  ///< MIU type
    MI_SYS_CoBufLayer_e    eCoBufLayer;         ///< Co_Buffer Layer

} mi_sys_MmapLayout_t;

typedef struct mi_sys_TimerMeasureInfo_s
{
    MI_BOOL bEnable;        // enable or not
    MI_U32 u32Start;         // current time of measurement

    MI_U32 u32Times;        // times of measurement
    MI_U32 u32Total;        // total time of all measurement
    MI_U32 u32Max;          // max time of all measurement
    MI_U32 u32Min;          // min time of all measurement
    MI_U32 u32Avg;          // average time of all measurement
}mi_sys_TimerMeasureInfo_t;

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief Get initialization status.
/// @return MI_HAS_INITED: MI_SYS has been inited.
/// @return MI_ERR_NOT_INITED: MI_SYS hasn't been inited.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_CheckInited(void);

//------------------------------------------------------------------------------
/// @brief Get Customer Info.
/// @param[out] pu8CustomerInfo: Pointer to copy the Customer_Info.
/// @param[in] u32CustomerInfoSize: Size to copy.
/// @param[out] pu8CustomerHash: Pointer to copy the Customer_hash.
/// @param[in] u32CustomerHashSize: Size to copy.
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: MI_SYS hasn't been inited.
/// @return MI_ERR_INVALID_PARAMETER: Parameters is invalid.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_GetCustomerInfo(MI_U8 *pu8CustomerInfo, MI_U32 u32CustomerInfoSize, MI_U8 *pu8CustomerHash, MI_U32 u32CustomerHashSize);

//------------------------------------------------------------------------------
/// @brief Set Customer Info.
/// @param[in] pu8CustomerInfo: Pointer to copy the Customer_Info.
/// @param[in] pu8CustomerHash: Pointer to copy the Customer_hash.
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: MI_SYS hasn't been inited.
/// @return MI_ERR_INVALID_PARAMETER: Parameters is invalid.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_SetCustomerHashInfo(MI_U8 *pu8CustomerInfo, MI_U8 *pu8CustomerHash);

//------------------------------------------------------------------------------
/// @brief Dump the current memory layout information.
/// @param[in] bForcePrint: TRUE for force print out mmap layout, FALSE for print out depends on MI_SYS debug level.
/// @return MI_OK: Process success.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_DumpMemLayout(MI_BOOL bForcePrint);

//------------------------------------------------------------------------------
/// @brief Allocate a cached memory block with 16-Byte aligned starting address from system cached memory pool
/// @param[in] u32Size Request size
/// @param[Out]  pMemAddr : pointer to the allocated memory block
/// @return NULL: not enough available memory
/// @return Otherwise: pointer to the allocated cached memory block
//------------------------------------------------------------------------------
void* mi_sys_AllocateCachedMemory(MI_U32 u32Size);

//------------------------------------------------------------------------------
/// @brief Allocate and return physical address from user cache memory in kernel space
/// @param[in] u32Size Request size
/// @return NULL: not enough available memory
/// @return Otherwise: pointer to the allocated cached memory block
//------------------------------------------------------------------------------
void* mi_sys_AllocatePhyCachedMemory(MI_U32 u32Size);

//------------------------------------------------------------------------------
/// @brief Free a cached memory block from cached memory pool
/// @param[in] pAddress Pointer to previously allocated cached memory block
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_FreeCachedMemory(void* pAddr);

//------------------------------------------------------------------------------
/// @brief Free physical address to user cache memory in kernel space
/// @param[in] pAddress Pointer to previously allocated cached memory block
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_FreePhyCachedMemory(void* pAddr);

//------------------------------------------------------------------------------
/// @brief Allocate a non-cached memory block with 16-Byte aligned starting address from system non-cached memory pool
/// @param[in] u32Size Request size
/// @param[Out]  pMemAddr : pointer to the allocated memory block
/// @return NULL: not enough available memory
/// @return Otherwise: pointer to the allocated non-cached memory block
//------------------------------------------------------------------------------
void* mi_sys_AllocateNonCachedMemory(MI_U32 u32Size);

//------------------------------------------------------------------------------
/// @brief Allocate and return physical address from user noncache memory in kernel space
/// @param[in] u32Size Request size
/// @return NULL: not enough available memory
/// @return Otherwise: pointer to the allocated non-cached memory block
//------------------------------------------------------------------------------
void* mi_sys_AllocatePhyNonCachedMemory(MI_U32 u32Size);

//------------------------------------------------------------------------------
/// @brief Free a non-cached memory block from non-cached memory pool
/// @param[in] pAddress Pointer to previously allocated non-cached memory block
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_FreeNonCachedMemory(void* pAddr);

//------------------------------------------------------------------------------
/// @brief Free physical address to user noncache memory in kernel space
/// @param[in] pAddress Pointer to previously allocated non-cached memory block
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_FreePhyNonCachedMemory(void* pAddr);

//------------------------------------------------------------------------------
/// @brief Create user Memory Pool
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_CreateUserMemoryPool(void);
//------------------------------------------------------------------------------
/// @brief Get user memory pool id
/// @param[out] ps32CachedPoolId Pointer to previously Cached Pool Id
/// @param[out] ps32NonCachedPoolId Pointer to previously NonCached Pool Id
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_GetUserMemPoolId(MI_S32 *ps32CachedPoolId, MI_S32 *ps32NonCachedPoolId);
//------------------------------------------------------------------------------
/// @brief Get Board information by board information ID
/// @param[in] eBoardInfoId : Board information ID
/// @param[Out]  pstBoardInfo : pointer to the board information
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_GetBoardInfo(MI_SYS_BoardInfoId_e eBoardInfoId, MI_SYS_BoardInfo_t *pstBoardInfo);

//------------------------------------------------------------------------------
/// @brief Read register value by bank and offset address.
/// @param[in] u32Bank : register bank.
/// @param[in] u32Offset : register address offset, this register address offset with 16bit.
/// @param[Out]  pu32RegValue : pointer to value of read register.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_ReadReg(MI_U32 u32Bank, MI_U32 u32Offset, MI_U32 *pu32RegValue);

//------------------------------------------------------------------------------
/// @brief write register value by bank and offset address.
/// @param[in] u32Bank : register bank.
/// @param[in] u32Offset : register address offset, this register address offset with 16bit.
/// @param[in]  u32RegValue : value of wirte register.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_WriteReg(MI_U32 u32Bank, MI_U32 u32Offset, MI_U32 u32RegValue);
//------------------------------------------------------------------------------
/// @brief Get the Mmap Pool Info Table
/// @param[Out] ppastMmapPoolInfo : Double pointer to mi_sys_MmapPoolInfo_t struct to retrieve The table of Mmap Pool Info.
/// @param[Out]  pu32MmapPoolInfoCnt : Pointer to MI_U32 to retrieve the max count of Mmap Pool Info Table
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_GetMmapPoolInfoTable(mi_sys_MmapPoolInfo_t **ppastMmapPoolInfo, MI_U32 *pu32MmapPoolInfoCnt);

//------------------------------------------------------------------------------
/// @brief Get the hardware memory layout current stored.
/// @param[in] eMmapId ID of memory map
/// @param[out] pstMmapLayout Pointer to struct MI_SYS_MmapLayout_t to retrieve the memory map of the specified mmap id.
/// @param[out] pu32MiuInterval Pointer to MI_U32 to retrieve the MIU_INTERVAL
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module hasn't be inited.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_GetMmapLayout(MI_SYS_MmapId_e eMmapId, mi_sys_MmapLayout_t *pstMmapLayout, MI_U32 *pu32MiuInterval);

/// @brief Launch the system (Linux) command by system call
/// @param[in] pszCmd : The system (Linux) command declared with MI_U8 string launched by system call.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_LaunchSystemCmd(MI_U8 *pszCmd);

//------------------------------------------------------------------------------
/// @brief suspend BDMA before PM STR power down.
/// @return MI_OK: success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_PmStrSuspend(void);

//------------------------------------------------------------------------------
/// @brief resume BDMA after PM STR power up.
/// @return MI_OK: success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_PmStrResume(void);

//------------------------------------------------------------------------------
/// @brief Reset measurement information
/// @param[in] pstMeasureInfo : Pointer to struct mi_sys_TimerMeasureInfo_t for reseting
/// @param[in] bEnable : Boolean value for enable or not
/// @return MI_OK: success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_TimerMeasureReset(mi_sys_TimerMeasureInfo_t *pstMeasureInfo, MI_BOOL bEnable);

//------------------------------------------------------------------------------
/// @brief Start to measure time
/// @param[in] pstMeasureInfo : Pointer to struct mi_sys_TimerMeasureInfo_t for recording measurement
/// @return MI_OK: success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_TimerMeasureStart(mi_sys_TimerMeasureInfo_t *pstMeasureInfo);

//------------------------------------------------------------------------------
/// @brief Stop measuring time
/// @param[in] pstMeasureInfo : Pointer to struct mi_sys_TimerMeasureInfo_t for stoping measurement and calculating result
/// @return MI_OK: success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_TimerMeasureStop(mi_sys_TimerMeasureInfo_t *pstMeasureInfo);

//------------------------------------------------------------------------------
/// @brief Report result of measurement
/// @param[in] pstMeasureInfo : Pointer to struct mi_sys_TimerMeasureInfo_t for reporting measurement
/// @param[in] pszDescription : User description for this report.
/// @return MI_OK: success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_TimerMeasureReport(mi_sys_TimerMeasureInfo_t *pstMeasureInfo, char *pszDescription);

//------------------------------------------------------------------------------
/// @brief get MMAP layout string
/// @param[in] eMmapId : eMmapId ID of memory map
/// @param[out] ppszMmapLayoutString : pointer to the MMAP layout string.
/// @return MI_OK: success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_GetMmapLayoutString(MI_SYS_MmapId_e eMmapId, MI_U8 **ppszMmapLayoutString);

//------------------------------------------------------------------------------
/// @brief Set debug level
/// @param[in] eDgbLevel : debug level to be set
/// @return MI_OK: success.
//------------------------------------------------------------------------------
MI_RESULT mi_sys_SetDebugLevel(MI_DBG_LEVEL_e eDgbLevel);

//------------------------------------------------------------------------------
/// @brief resume Get Low power mode(MI_SYS_LOW_POWER_CONSUMPTION_IN_SD).
/// @return TRUE: Low power mode.
/// @return FALSE: NOT Low power mode.
//------------------------------------------------------------------------------
MI_BOOL mi_sys_IsLowPowerMode(void);

#ifdef __cplusplus
}
#endif

#endif///_MI_SYS_INTERNAL_H_

