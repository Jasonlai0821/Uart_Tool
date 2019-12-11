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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   mi_ubi.c
/// @brief ubi module
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include "mi_common.h"
#include "mi_ubi.h"
#include "mi_sys.h"
#include "internal/mi_sys_internal.h"


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Defines
//-------------------------------------------------------------------------------------------------
#define DBG_LEVEL                       MI_DBG_FW

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MI_UBI_SLOT_MAX                 (16)
#define MI_UBI_MAGIC_NUM                (0x55424930)
#define MI_UBI_MAX_PART_NUM             (MI_MTD_MAX_NUM)

#define MI_MTD_MAX_NUM                  (16)
#define MI_MTD_NAME_PATH                "/sys/class/mtd/mtd%lu/name"

#define UBIDEV_INFO_VOLCNT              "/sys/devices/virtual/ubi/ubi%lu/volumes_count"
#define UBIDEV_INFO_EB_SIZE             "/sys/devices/virtual/ubi/ubi%lu/eraseblock_size"
#define UBIDEV_INFO_TOTAL_SIZE          "/sys/devices/virtual/ubi/ubi%lu/total_eraseblocks"
#define UBIDEV_INFO_MIN_IO_SIZE         "/sys/devices/virtual/ubi/ubi%lu/min_io_size"
#define UBIDEV_INFO_MTD_NUM             "/sys/devices/virtual/ubi/ubi%lu/mtd_num"

#define UBIVOL_LEB_SIZE_PATT            "/sys/devices/virtual/ubi/ubi%lu/ubi%lu_%lu/usable_eb_size"
#define UBIVOL_BLOCKCNT_PATT            "/sys/devices/virtual/ubi/ubi%lu/ubi%lu_%lu/reserved_ebs"
#define UBIVOL_TOTAL_SIZE_PATT          "/sys/devices/virtual/ubi/ubi%lu/ubi%lu_%lu/data_bytes"
#define UBIVOL_NAME_PATT                "/sys/devices/virtual/ubi/ubi%lu/ubi%lu_%lu/name"
#define UBIVOL_DEV_PATT                 "/sys/devices/virtual/ubi/ubi%lu/ubi%lu_%lu/dev"

#define UBI_DEVICE_NODE_PATH            "/dev/shm/ubi%lu"
#define UBI_VOLUME_NODE_PATH            "/dev/shm/ubi%lu_%lu"

#define MI_UBI_VOL_NUM_AUTO (-1)
#define MI_UBI_MAX_VOLUME_NAME 127
#define MI_UBI_VOL_IOC_MAGIC 'O'
#define MI_UBI_IOC_MAGIC 'o'

#define MI_UBI_IOC_EBCH _IOW(MI_UBI_VOL_IOC_MAGIC, 2, MI_S32)
#define MI_UBI_IOC_MKVOL _IOW(MI_UBI_IOC_MAGIC, 0, MI_UBI_MakeVolReq_t)
#define MI_UBI_IOC_RMVOL _IOW(MI_UBI_IOC_MAGIC, 1, MI_S32)
#define MI_UBI_IOC_RSVOL _IOW(MI_UBI_IOC_MAGIC, 2, MI_UBI_ResizeVolReq_t)

static int _MsOS_CreateMutex(void);
static int _MsOS_DeleteMutex(MI_S32 s32UbiMutex);
static int _MsOS_ObtainMutex(MI_S32 s32MutexId);
static int _MsOS_ReleaseMutex(MI_S32 s32MutexId);
#define UBI_LOCK()    _MsOS_ObtainMutex(_s32UbiMutex)
#define UBI_UNLOCK()  _MsOS_ReleaseMutex(_s32UbiMutex)
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct MI_UBI_LebChangeReq_s {
    MI_S32 s32Num;
    MI_S32 s32Bytes;
    MI_S8  s8Type;
    MI_S8  as8Padding[7];
} __attribute__ ((packed)) MI_UBI_LebChangeReq_t;

typedef struct MI_UBI_MakeVolReq_s {
    MI_S32 s32VolId;
    MI_S32 s32Alignment;
    MI_S64 s64Bytes;
    MI_S8 s8VolType;
    MI_S8 s8Padding1;
    MI_S16 s16NameLen;
    MI_S8 as8Padding2[4];
    MI_U8 szName[MI_UBI_MAX_VOLUME_NAME + 1];
} __attribute__ ((packed)) MI_UBI_MakeVolReq_t;

typedef struct MI_UBI_ResizeVolReq_s {
    MI_S64 s64Bytes;
    MI_S32 s32VolId;
} __attribute__ ((packed)) MI_UBI_ResizeVolReq_t;

typedef struct MI_UBI_VolInfo_s
{
    MI_S32 s32VolFd;
    MI_U32 u32VolId;
    MI_U32 u64VolByteSize;
    MI_U32 u32BlockSize;
    MI_U32 u32BlockCnt;
    MI_U8 u8VolName[MAX_VOLUME_NAME];
} MI_UBI_VolInfo_t;

typedef struct MI_MTD_PartInfo_s
{
    MI_U32 u32Id;
    MI_U8 aszName[MAX_PARTITION_NAME];
    MI_BOOL bExist;
} MI_MTD_PartInfo_t;//partition info

typedef struct MI_UBI_DevInfo_s
{
    MI_U32 u32DeviceId;
    MI_S32 s32DevFd;
    MI_S32 s32VolCnt;
    MI_U32 u32BlockSize;
    MI_U64 u64TotalSize;
    MI_U32 u32MinIoSize;
    MI_U32 u32MtdNum;
} MI_UBI_DevInfo_t;

typedef struct MI_UBI_Slot_s
{
    MI_BOOL bUsed;
    MI_U32 u32MagicNum;
    MI_UBI_DevInfo_t stUbiDevInfo;
    MI_UBI_VolInfo_t *pstUbiVolInfoTbl[MI_UBI_MAX_VOLUME];
} MI_UBI_Slot_t;

enum {
    E_MI_UBI_DATATYPE_LONGTERM  = 1,
    E_MI_UBI_DATATYPE_SHORTTERM = 2,
    E_MI_UBI_DATATYPE_UNKNOWN   = 3,
}MI_UBI_DataType_e;

enum {
    E_MI_UBI_VOLUME_TYPE_DYNAMIC = 3,
    E_MI_UBI_VOLUME_TYPE_STATIC = 4,
}MI_UBI_Volume_Type_e;
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MI_U8 _u8UbiDbgLevel = (MI_U8)MI_DBG_DRV_ALL;
static MI_BOOL _bIsUBIModuleInit = FALSE;
static MI_BOOL _bMtdInfoInited = FALSE;
static MI_UBI_Slot_t _astUbiSlot[MI_UBI_SLOT_MAX];

static MI_MTD_PartInfo_t _astMtdPartInfo[MI_MTD_MAX_NUM];
MI_S32 _s32UbiMutex = -1;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#if (MI_UCLIBC_TOOLCHAIN==1)
static char *_index(const char *s, int c)
{
    int size=strlen(s);
    char *ptr=(char *)s;
    char *end=(char *)s;

    end+=size;

    while(ptr<end)
    {
        if(*ptr==c)
            return ptr;
        ptr++;
    }
    return NULL;
}
#else
#define _index(s,c) index(s,c)
#endif
static MI_RESULT _MI_UBI_Handle2Slot(MI_HANDLE hHandle, MI_UBI_Slot_t **ppstSlot)
{
    MI_UBI_Slot_t *pstUbiSlot = NULL;
    MI_U8 u8Index;

    if(hHandle == MI_HANDLE_NULL || hHandle == NULL)
    {
        MI_ERR("Invalid handle!\n");
        return MI_ERR_INVALID_HANDLE;
    }

    for(u8Index = 0; u8Index < MI_UBI_SLOT_MAX; u8Index++)
    {
        if(hHandle == (MI_HANDLE)(&_astUbiSlot[u8Index]))
            break;
    }
    if(u8Index >= MI_UBI_SLOT_MAX)
    {
        MI_ERR("No match UBI slot\n");
        return MI_ERR_INVALID_HANDLE;
    }

    pstUbiSlot = (MI_UBI_Slot_t *)hHandle;
    if(pstUbiSlot->bUsed == FALSE)
    {
        MI_ERR("Invalid handle which is not opened!\n");
        return MI_ERR_INVALID_HANDLE;
    }

    if(pstUbiSlot->u32MagicNum != MI_UBI_MAGIC_NUM)
    {
        MI_ERR("Invalid magic number = 0x%lX!\n", pstUbiSlot->u32MagicNum);
        return MI_ERR_INVALID_HANDLE;
    }

    if(pstUbiSlot->stUbiDevInfo.s32DevFd== -1)
    {
        MI_ERR("Device not open!\n");
        return MI_ERR_FAILED;
    }

    *ppstSlot = pstUbiSlot;
    return MI_OK;
}

static MI_BOOL _MI_UBI_ReadInfo(MI_U8 *pu8FilePath, MI_U8 *pu8Buff, MI_U32 u32Size)
{
    FILE *File = NULL;

    if(pu8FilePath==NULL || pu8Buff==NULL)
    {
        MI_ERR("Invalid parameter\n");
        return FALSE;
    }

    memset(pu8Buff, 0, sizeof(u32Size));
    File = fopen((char *)pu8FilePath, "r");
    if (File==NULL)
    {
        MI_INFO("Open %s Error: %s\n", pu8FilePath, strerror(errno));
        return FALSE;
    }

    if(fgets((char *)pu8Buff, u32Size, File)<0)
    {
        MI_INFO("Read %s fail: %s\n", pu8FilePath, strerror(errno));
        fclose(File);
        return FALSE;
    }

    fclose(File);
    return TRUE;
}

static MI_RESULT _MI_UBI_SetupDeviceInfo(MI_UBI_Slot_t *pstUbiSlot)
{
    MI_U32 u32DeviceIndex = pstUbiSlot->stUbiDevInfo.u32DeviceId;
    MI_U8 u8Buff[32];
    MI_U8 aszPath[64]= {0};

    memset(aszPath, 0, sizeof(aszPath));
    snprintf((char *)aszPath, sizeof(aszPath), UBIDEV_INFO_VOLCNT, u32DeviceIndex);
    if(_MI_UBI_ReadInfo(aszPath, u8Buff, sizeof(u8Buff))==FALSE)
        return MI_ERR_FAILED;
    pstUbiSlot->stUbiDevInfo.s32VolCnt = atoi((const char*)u8Buff);

    memset(aszPath, 0, sizeof(aszPath));
    snprintf((char *)aszPath, sizeof(aszPath), UBIDEV_INFO_EB_SIZE, u32DeviceIndex);
    if(_MI_UBI_ReadInfo(aszPath, u8Buff, sizeof(u8Buff))==FALSE)
        return MI_ERR_FAILED;
    pstUbiSlot->stUbiDevInfo.u32BlockSize = atoi((const char*)u8Buff);

    memset(aszPath, 0, sizeof(aszPath));
    snprintf((char *)aszPath, sizeof(aszPath), UBIDEV_INFO_TOTAL_SIZE, u32DeviceIndex);
    if(_MI_UBI_ReadInfo(aszPath, u8Buff, sizeof(u8Buff))==FALSE)
        return MI_ERR_FAILED;
    pstUbiSlot->stUbiDevInfo.u64TotalSize = atoi((const char*)u8Buff);
    pstUbiSlot->stUbiDevInfo.u64TotalSize = pstUbiSlot->stUbiDevInfo.u64TotalSize * pstUbiSlot->stUbiDevInfo.u32BlockSize;

    memset(aszPath, 0, sizeof(aszPath));
    snprintf((char *)aszPath, sizeof(aszPath), UBIDEV_INFO_MIN_IO_SIZE, u32DeviceIndex);
    if(_MI_UBI_ReadInfo(aszPath, u8Buff, sizeof(u8Buff))==FALSE)
        return MI_ERR_FAILED;
    pstUbiSlot->stUbiDevInfo.u32MinIoSize = atoi((const char*)u8Buff);

    memset(aszPath, 0, sizeof(aszPath));
    snprintf((char *)aszPath, sizeof(aszPath), UBIDEV_INFO_MTD_NUM, u32DeviceIndex);
    if(_MI_UBI_ReadInfo(aszPath, u8Buff, sizeof(u8Buff))==FALSE)
        return MI_ERR_FAILED;
    pstUbiSlot->stUbiDevInfo.u32MtdNum= atoi((const char*)u8Buff);

    MI_INFO("_stUbiDevInfo.s32VolCnt=%lu\n", pstUbiSlot->stUbiDevInfo.s32VolCnt);
    MI_INFO("_stUbiDevInfo.u32BlockSize=%lu\n", pstUbiSlot->stUbiDevInfo.u32BlockSize);
    MI_INFO("_stUbiDevInfo.u64TotalSize=%llu\n", pstUbiSlot->stUbiDevInfo.u64TotalSize);
    MI_INFO("_stUbiDevInfo.u32MinIoSize=%lu\n", pstUbiSlot->stUbiDevInfo.u32MinIoSize);
    MI_INFO("_stUbiDevInfo.u32MtdNum=%lu\n", pstUbiSlot->stUbiDevInfo.u32MtdNum);
    return MI_OK;
}

static MI_RESULT _MI_UBI_MakeVolumeNode(MI_UBI_Slot_t *pstUbiSlot, MI_U32 u32VolId)
{
    struct stat sts;

    MI_U8 au8Path[128]= {0};
    MI_U8 au8Info[64]= {0};
    MI_U8 u8Cmd[128]= {0};
    MI_U32 u32DevMajor=0;
    MI_U32 u32DevMinor=0;
    MI_U32 u32DeviceId = pstUbiSlot->stUbiDevInfo.u32DeviceId;

    if(pstUbiSlot==NULL)
    {
        MI_ERR("Inavalid Parameter!\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    memset(au8Path, 0, sizeof(au8Path));
    snprintf((char *)au8Path, sizeof(au8Path), UBIVOL_DEV_PATT, u32DeviceId, u32DeviceId, u32VolId);
    if(_MI_UBI_ReadInfo(au8Path, au8Info, sizeof(au8Info))==FALSE)
        return MI_ERR_FAILED;

    memset(au8Path, 0, sizeof(au8Path));
    snprintf((char *)au8Path, sizeof(au8Path), UBI_VOLUME_NODE_PATH, u32DeviceId, u32VolId);
    if ((stat ((char *)au8Path, &sts)) == -1)
    {
        u32DevMajor = atoi((char *)au8Info);
        u32DevMinor = atoi((char*)(_index((char *)au8Info,':')+1));
        snprintf((char *)u8Cmd, sizeof(u8Cmd), "mknod %s c %lu %lu", (char *)au8Path, u32DevMajor, u32DevMinor);
        MI_INFO("CMD:%s\n", (char *)u8Cmd);
        if(mi_sys_LaunchSystemCmd(u8Cmd)==MI_ERR_FAILED)
        {
            MI_ERR("%s fail\n", u8Cmd);
            return MI_ERR_FAILED;
        }
        else
        {
            MI_INFO("create node success\n");
            return MI_OK;
        }
    }
    else
    {
        MI_INFO("Node exist already\n");
        return MI_OK;
    }
}

static MI_RESULT _MI_UBI_SetupVolumeInfo(MI_UBI_Slot_t *pstUbiSlot, MI_U32 u32VolId)
{
    MI_U8 au8Path[100];
    MI_U8 au8Info[128];
    MI_U32 u32DeviceId = pstUbiSlot->stUbiDevInfo.u32DeviceId;
    MI_UBI_VolInfo_t *pstVolnfo=NULL;

    if(u32VolId>=MI_UBI_MAX_VOLUME)
    {
        MI_ERR("Invalid  volume Index=%lu\n",u32VolId);
        return MI_ERR_FAILED;
    }

    pstVolnfo = pstUbiSlot->pstUbiVolInfoTbl[u32VolId];

    memset(au8Path, 0, sizeof(au8Path));
    snprintf((char *)au8Path, sizeof(au8Path), UBIVOL_LEB_SIZE_PATT, u32DeviceId, u32DeviceId, u32VolId);
    if(_MI_UBI_ReadInfo(au8Path, au8Info, sizeof(au8Info))==FALSE)
        return MI_ERR_FAILED;
    pstVolnfo->u32BlockSize = atoi((const char*)au8Info);

    memset(au8Path, 0, sizeof(au8Path));
    snprintf((char *)au8Path, sizeof(au8Path), UBIVOL_BLOCKCNT_PATT, u32DeviceId, u32DeviceId, u32VolId);
    if(_MI_UBI_ReadInfo(au8Path, au8Info, sizeof(au8Info))==FALSE)
        return MI_ERR_FAILED;
    pstVolnfo->u32BlockCnt = atoi((const char*)au8Info);

    memset(au8Path, 0, sizeof(au8Path));
    snprintf((char *)au8Path, sizeof(au8Path), UBIVOL_TOTAL_SIZE_PATT, u32DeviceId, u32DeviceId, u32VolId);
    if(_MI_UBI_ReadInfo(au8Path, au8Info, sizeof(au8Info))==FALSE)
        return MI_ERR_FAILED;
    pstVolnfo->u64VolByteSize = atoi((char*)au8Info);

    memset(au8Path, 0, sizeof(au8Path));
    snprintf((char *)au8Path, sizeof(au8Path), UBIVOL_NAME_PATT, u32DeviceId, u32DeviceId, u32VolId);
    if(_MI_UBI_ReadInfo(au8Path, au8Info, sizeof(au8Info))==FALSE)
        return MI_ERR_FAILED;
    strncpy((char *)pstVolnfo->u8VolName, (char *)au8Info, MAX_VOLUME_NAME);
    if(pstVolnfo->u8VolName[strlen((char *)pstVolnfo->u8VolName)-1]==10)
        pstVolnfo->u8VolName[strlen((char *)pstVolnfo->u8VolName)-1]=0;

    _MI_UBI_MakeVolumeNode(pstUbiSlot, u32VolId);
    pstVolnfo->u32VolId=u32VolId;
    MI_INFO("pstVolnfo->u8VolName=%s\n", pstVolnfo->u8VolName);
    MI_INFO("pstVolnfo->u32BlockSize=%lu\n", pstVolnfo->u32BlockSize);
    MI_INFO("pstVolnfo->u32BlockCnt=%lu\n", pstVolnfo->u32BlockCnt);
    MI_INFO("pstVolnfo->u64VolByteSize=%lu\n", pstVolnfo->u64VolByteSize);
    MI_INFO("pstVolnfo->u32VolId=%lu\n", pstVolnfo->u32VolId);
    return 0;

}

static MI_RESULT _MI_UBI_Setup(MI_UBI_Slot_t *pstUbiSlot)
{
    MI_U8 u8VolIdx=0;
    MI_U8 u8VolCount=0;

    if(_MI_UBI_SetupDeviceInfo(pstUbiSlot) != MI_OK)
    {
        MI_ERR("Setup Device info fail!!!\n");
        return MI_ERR_FAILED;
    }

    for(u8VolIdx=0; u8VolIdx<MI_UBI_MAX_VOLUME; u8VolIdx++)
    {
        if(_MI_UBI_SetupVolumeInfo(pstUbiSlot, u8VolIdx)!=MI_OK)
            continue;

        u8VolCount++;
    }

    if(u8VolCount!=pstUbiSlot->stUbiDevInfo.s32VolCnt)
    {
        MI_ERR("Volume count not match\n");
        return MI_ERR_FAILED;
    }

    return MI_OK;
}

static MI_RESULT _MI_UBI_EnableVolumeLebChange(MI_HANDLE hUbiDevice, MI_U32 u32VolId,  MI_S32 s32Num, MI_S32 s32Length)
{
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;

    MI_UBI_LebChangeReq_t stLebChgReq;

    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    if(u32VolId>=MI_UBI_MAX_VOLUME)
    {
        MI_ERR("Invalid  volId=%lu\n",u32VolId);
        return MI_ERR_FAILED;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        memset(&stLebChgReq, 0, sizeof(stLebChgReq));
        stLebChgReq.s32Num = s32Num;
        stLebChgReq.s32Bytes = s32Length;
        stLebChgReq.s8Type = E_MI_UBI_DATATYPE_UNKNOWN;

        if(ioctl(pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd, MI_UBI_IOC_EBCH, &stLebChgReq) < 0)
        {
            MI_ERR("ioctl failed: %s, Volume Fd=%ld\n", strerror(errno), pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd);
            return MI_ERR_FAILED;
        }
        MI_EXIT_OK();
        return MI_OK;
    }
}

static MI_RESULT _MI_UBI_OpenVolume(MI_UBI_Slot_t *pstUbiSlot, MI_U32 u32VolId)
{
    MI_U8 aszNodePath[256]= {0};
    MI_S32 s32VolumeFd=-1;

    if(u32VolId >= MI_UBI_MAX_VOLUME)
    {
        MI_ERR("Error Volume id: %lu\n",u32VolId);
        return MI_ERR_FAILED;
    }

    if (pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd > 0)
    {
//        MI_INFO("ReOpen, pstUbiSlot->pstUbiVolInfoTbl[u32VolId].s32VolFd = %ld\n", pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd);
        return MI_OK;
    }

    UBI_LOCK();
    memset(aszNodePath, 0, 256);
    snprintf((char *)aszNodePath, sizeof(aszNodePath), UBI_VOLUME_NODE_PATH, pstUbiSlot->stUbiDevInfo.u32DeviceId, u32VolId);
    s32VolumeFd = open((char *)aszNodePath, O_RDWR);
    if (s32VolumeFd < 0)
    {
        MI_ERR("Open %s failed: %s!!\n", aszNodePath, strerror(errno));
        UBI_UNLOCK();
        return MI_ERR_FAILED;
    }

    MI_INFO("Open volume: %s ok\n", aszNodePath);
    pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd = s32VolumeFd;
    UBI_UNLOCK();
    return MI_OK;
}

static MI_RESULT _MI_UBI_CloseVolume(MI_UBI_Slot_t *pstUbiSlot, MI_U32 u32VolId)
{
    UBI_LOCK();
    if(u32VolId>=MI_UBI_MAX_VOLUME)
    {
        MI_ERR("Invalid  volId=%lu\n",u32VolId);
        UBI_UNLOCK();
        return MI_ERR_FAILED;
    }

    if(pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd != -1)
    {
        close(pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd);
    }
    memset(pstUbiSlot->pstUbiVolInfoTbl[u32VolId], 0, sizeof(MI_UBI_VolInfo_t));
    pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd = -1;
    MI_INFO("_MI_UBI_CloseVolume(%lu)\n", u32VolId);
    UBI_UNLOCK();
    return MI_OK;
}


static MI_S32 _MI_UBI_Offset2VolumeLebNum(MI_HANDLE hUbiDevice, MI_U32 u32Offset, MI_U32 *pu32LebNum)
{
    MI_U32 u32LebSize;
    MI_U32 u32Remainder;

    if(MI_UBI_GetBlockSize(hUbiDevice, &u32LebSize)!=MI_OK)
        return -1;

    if(u32LebSize==0)
        return -1;

    *pu32LebNum = u32Offset / u32LebSize;
    u32Remainder = u32Offset % u32LebSize;
    MI_INFO("Offset %lu to Volume Leb number[%lu], remainder=%ld\n", u32Offset, *pu32LebNum, u32Remainder);
    return u32Remainder;
}

static MI_S32 _MI_UBI_ReadVolume(MI_HANDLE hUbiDevice, MI_U32 u32VolId, MI_S32 s32StartLeb, MI_U8 *pu8Buf, MI_U32 u32Length)
{
    MI_S32 retlen=-1;
    MI_S32 s32Offset;
    MI_S32 s32LEBSize;
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;

    MI_ENTER();
    if(_bIsUBIModuleInit == FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return -1;
    }

    if(pu8Buf==NULL)
    {
        MI_ERR("Invalid Parameter\n");
        return -1;
    }

    if(u32VolId>=MI_UBI_MAX_VOLUME)
    {
        MI_ERR("Invalid volId=%lu\n",u32VolId);
        return -1;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return -1;
    }
    else
    {
        MI_U32 u32BlockSize=0;
        MI_U32 u32BlockCount=0;

        if(MI_UBI_GetBlockSize(hUbiDevice, &u32BlockSize)!=MI_OK)
        {
            MI_ERR("Get blocksize err\n");
            return -1;
        }

        if(_MI_UBI_OpenVolume(pstUbiSlot, u32VolId)!=MI_OK)
        {
            MI_ERR("open volume fail\n");
            return -1;
        }

        if(MI_UBI_GetVolumeBlockCount(hUbiDevice, u32VolId, &u32BlockCount)==MI_OK && s32StartLeb>=u32BlockCount)
        {
            MI_ERR("Volume[%lu] total Leb %lu but want to access leb %lu\n", u32VolId, u32BlockCount, s32StartLeb);
            return -1;
        }

        UBI_LOCK();
        s32LEBSize = u32BlockSize;
        s32Offset = s32StartLeb * s32LEBSize;
        lseek(pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd, s32Offset, SEEK_SET);
        retlen = read(pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd, pu8Buf, u32Length);
        if(retlen==-1)
        {
            MI_ERR("Read data failed:%s\n", strerror(errno));
            UBI_UNLOCK();
            return -1;
        }
        else if (retlen>0 && retlen != u32Length)
        {
            MI_WRN("ubi volume[%lu] want to read length %lu, but only %lu readed.\n", u32VolId, u32Length, retlen);
        }
        UBI_UNLOCK();
        MI_EXIT_OK();
        return retlen;
    }
}

static void _MI_UBI_ReleaseMemory(MI_U8 *pu8Addr)
{
    if(pu8Addr)
    {
        free(pu8Addr);
        pu8Addr=NULL;
    }
}

//write single Leb
static MI_S32 _MI_UBI_WriteVolumeLeb(MI_HANDLE hUbiDevice, MI_U32 u32VolId, MI_U32 u32LebNum, MI_S32 s32LebOffset, MI_U8 *pu8Buf, MI_U32 u32WriteLen)
{
    MI_U32 u32VolumeOffset=0;
    MI_U32 u32LEBSize=0;
    MI_S32 s32BytesWritten=-1;
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;
    MI_U8 *pu8LebBuf=NULL;
    MI_S32 s32Ret=-1;

    MI_ENTER();
//    MI_INFO("Write VolId=%lu, u32LebNum=%lu, s32LebOffset=%ld\n", u32VolId, u32LebNum, s32LebOffset);
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return -1;
    }

    if(u32VolId>=MI_UBI_MAX_VOLUME)
    {
        MI_ERR("Invalid  volId=%lu\n",u32VolId);
        return -1;
    }

    if(pu8Buf==NULL)
    {
        MI_ERR("Invalid Parameter\n");
        return -1;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return -1;
    }
    else
    {
        MI_U32 u32BlockCount=0;

        if(_MI_UBI_OpenVolume(pstUbiSlot, u32VolId)!=MI_OK)
        {
            MI_ERR("open volume fail\n");
            return -1;
        }

        if(MI_UBI_GetBlockSize(hUbiDevice, &u32LEBSize)!=MI_OK || s32LebOffset+u32WriteLen>u32LEBSize)
        {
            MI_ERR("Get block size fail!\n");
            return -1;
        }

        if(MI_UBI_GetVolumeBlockCount(hUbiDevice, u32VolId, &u32BlockCount)==MI_OK && u32LebNum>=u32BlockCount)
        {
            MI_ERR("Volume[%lu] total Leb %lu but want to access leb %lu\n", u32VolId, u32BlockCount, u32LebNum);
            return -1;
        }

        if((pu8LebBuf=malloc(u32LEBSize))==NULL)
        {
            MI_ERR("malloc readbuf fail\n");
            return -1;
        }

        if(_MI_UBI_ReadVolume(hUbiDevice, u32VolId, u32LebNum, pu8LebBuf, u32LEBSize) != u32LEBSize)
        {
            MI_ERR("Read data not complete\n");
            _MI_UBI_ReleaseMemory(pu8LebBuf);
            return -1;
        }

        memcpy(pu8LebBuf+s32LebOffset, pu8Buf, u32WriteLen);
        UBI_LOCK();
        if(_MI_UBI_EnableVolumeLebChange(hUbiDevice, u32VolId, u32LebNum, u32LEBSize)!=MI_OK)
        {
            MI_ERR("Enable Leb Change err\n");
            _MI_UBI_ReleaseMemory(pu8LebBuf);
            UBI_UNLOCK();
            return -1;
        }

        u32VolumeOffset = u32LebNum*u32LEBSize;//offset to leb start
        if(lseek(pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd, u32VolumeOffset, SEEK_SET) == -1)
        {
            MI_ERR("Seek fail:%s\n", strerror(errno));
            _MI_UBI_ReleaseMemory(pu8LebBuf);
            UBI_UNLOCK();
            return -1;
        }

        //write leb
        s32BytesWritten = write(pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd, pu8LebBuf, u32LEBSize);
        if(s32BytesWritten==-1)
        {
            MI_ERR("Write data failed:%s\n", strerror(errno));
            _MI_UBI_ReleaseMemory(pu8LebBuf);
            UBI_UNLOCK();
            return -1;
        }
        else if(s32BytesWritten == u32LEBSize)
        {
            s32Ret = u32WriteLen;
        }
        else//write all leb fail
        {
            MI_WRN("ubi volume[%lu] want to write bytes=%lu, but only %ld writted.\n", u32VolId, u32LEBSize, s32BytesWritten);
            if(s32BytesWritten >= (s32LebOffset+u32WriteLen))
            {
                s32Ret = u32WriteLen;
            }
            else if(s32BytesWritten>=s32LebOffset && s32BytesWritten< s32LebOffset+u32WriteLen)
            {
                s32Ret = s32BytesWritten-s32LebOffset;
            }
            else if(s32BytesWritten< s32LebOffset)
            {
                s32Ret=0;
            }
        }
        UBI_UNLOCK();
        MI_EXIT_OK();
        _MI_UBI_ReleaseMemory(pu8LebBuf);
        return s32Ret;
    }
}

static MI_RESULT _MI_UBI_FindMtdNumByName(MI_U8 *pu8PartName, MI_U32 *pu32MtdNo)
{
    MI_RESULT eRet = MI_ERR_FAILED;
    MI_U32 u32Index=0;

    MI_ENTER();
    if(pu8PartName==NULL)
    {
        MI_ERR("Invalid parameter\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    MI_INFO("pu8PartName=%s\n", pu8PartName);
    for(u32Index=0; u32Index< MI_MTD_MAX_NUM; u32Index++)
    {
        MI_INFO("[%lu] _astMtdPartInfo[u32Index].aszName=%s\n", u32Index, _astMtdPartInfo[u32Index].aszName);
        if(_astMtdPartInfo[u32Index].bExist==TRUE && strncmp((char *)pu8PartName, (char *)_astMtdPartInfo[u32Index].aszName, strlen((char *)pu8PartName))==0 && strlen((char *)_astMtdPartInfo[u32Index].aszName)==strlen((char *)pu8PartName))
        {
            *pu32MtdNo = _astMtdPartInfo[u32Index].u32Id;
            eRet= MI_OK;
            MI_INFO("Find mtd id=%lu has name %s\n", u32Index, pu8PartName);
            break;
        }
    }

    return eRet;
}

static void _MI_UBI_InitMtdInfo(void)
{
    MI_U32 u32Index=0;
    MI_U8 aszMtdPath[64];
    MI_U8 u8MtdName[MAX_PARTITION_NAME];
    MI_U8 u8NameLen;

    MI_ENTER();
    if(_bMtdInfoInited==TRUE)
    {
        return;
    }

    memset(_astMtdPartInfo, 0, sizeof(_astMtdPartInfo));
    for(u32Index=0; u32Index< MI_MTD_MAX_NUM; u32Index++)
    {
        memset(aszMtdPath, 0, sizeof(aszMtdPath));
        memset(u8MtdName, 0, sizeof(u8MtdName));
        snprintf((char *)aszMtdPath, sizeof(aszMtdPath), MI_MTD_NAME_PATH, u32Index);
        if(_MI_UBI_ReadInfo(aszMtdPath, u8MtdName, sizeof(u8MtdName))==FALSE)
        {
            break;
        }

        u8NameLen = strlen((char *)u8MtdName);
        _astMtdPartInfo[u32Index].u32Id = u32Index;
        if(u8MtdName[u8NameLen-1]==10)
        {
            strncpy((char *)_astMtdPartInfo[u32Index].aszName, (char *)u8MtdName, u8NameLen-1);//skip new line character at last character
        }
        else
        {
            strncpy((char *)_astMtdPartInfo[u32Index].aszName, (char *)u8MtdName, u8NameLen);
        }

        _astMtdPartInfo[u32Index].bExist = TRUE;
    }

    _bMtdInfoInited = TRUE;
    return;
}

static MI_RESULT _MI_UBI_MakeNode(void)
{
    MI_U8 u8Cmd[64];

    MI_ENTER();
    memset(u8Cmd, 0, sizeof(u8Cmd));
    sprintf((char *)u8Cmd,"sh /application/ubiMknod.sh");
    if(mi_sys_LaunchSystemCmd(u8Cmd)==MI_ERR_FAILED)
    {
        MI_ERR("make node fail");
        return MI_ERR_FAILED;
    }
    else
    {
        MI_INFO("make node success\n");
        return MI_OK;
    }
}

static MI_RESULT _MI_UBI_Attach(MI_U8 *pu8PartName)
{
    MI_U8 u8Cmd[64];
    MI_U32 u32MtdNum=0;

    MI_ENTER();
    if(pu8PartName==NULL)
    {
        MI_ERR("Invalid parameter\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    if(_MI_UBI_FindMtdNumByName(pu8PartName, &u32MtdNum)!=MI_OK)
    {
        MI_ERR("Can't find relative MTD\n");
        return MI_ERR_FAILED;
    }

    memset(u8Cmd, 0, sizeof(u8Cmd));
    sprintf((char *)u8Cmd,"ubiattach -m %lu /dev/ubi_ctrl", u32MtdNum);
    if(mi_sys_LaunchSystemCmd(u8Cmd)==MI_ERR_FAILED)
    {
        MI_ERR("attach MTD[%lu]:%s fail\n", u32MtdNum, pu8PartName);
        return MI_ERR_FAILED;
    }
    else
    {
        MI_INFO("attch MTD[%lu]:%s success\n", u32MtdNum, pu8PartName);
        if(_MI_UBI_MakeNode()!=MI_OK)
        {
            return MI_ERR_FAILED;
        }
        return MI_OK;
    }
}

#if 0
MI_RESULT MI_UBI_EraseVolumeLeb(MI_HANDLE hUbiDevice, MI_U32 u32VolId, MI_U32 u32LebNum)
{
    MI_RESULT eErrCode;
    MI_UBI_Slot_t *pstUbiSlot;

    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        if(_MI_UBI_OpenVolume(pstUbiSlot, u32VolId)!=MI_OK)
        {
            MI_ERR("open volume fail\n");
            return MI_ERR_FAILED;
        }

        UBI_LOCK();
        if(ioctl(pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd, UBI_IOCEBER, &u32LebNum) < 0)
        {
            MI_ERR("ioctl failed: %s\n", strerror(errno));
            UBI_UNLOCK();
            return MI_ERR_FAILED;
        }
        UBI_UNLOCK();
        MI_EXIT_OK();
        return MI_OK;
    }
}

MI_RESULT MI_UBI_EnableVolumeUpdate(MI_HANDLE hUbiDevice, MI_U32 u32VolId,  MI_S32 s32Bytes2Save)
{
    MI_RESULT eErrCode;
    MI_UBI_Slot_t *pstUbiSlot;

    MI_ENTER();

    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        if(ioctl(pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd, UBI_IOCVOLUP, &s32Bytes2Save) < 0)
        {
            MI_ERR("ioctl failed: %s, Volume Fd=%ld\n", strerror(errno), pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd);
            return MI_ERR_FAILED;
        }
        MI_EXIT_OK();
        return MI_OK;
    }
}

MI_RESULT MI_UBI_EnableVolumeDirectWrite(MI_HANDLE hUbiDevice, MI_U32 u32VolId)
{
    MI_RESULT eErrCode;
    MI_UBI_Slot_t *pstUbiSlot;
    struct ubi_set_prop_req stSetProReq;

    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        memset(&stSetProReq, 0, sizeof(stSetProReq));
        stSetProReq.property = UBI_PROP_DIRECT_WRITE;
        stSetProReq.value = 1;

        if(ioctl(pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd, UBI_IOCSETPROP, &stSetProReq) < 0)
        {
            MI_ERR("ioctl failed: %s, Volume Fd=%ld\n", strerror(errno), pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->s32VolFd);
            return MI_ERR_FAILED;
        }
        MI_EXIT_OK();
        return MI_OK;
    }
}
#endif
static void _MI_UBI_ReleaseResource(MI_UBI_Slot_t *pstUbiSlot)
{
    MI_U8 u8VolIndex=0;

    if(pstUbiSlot->stUbiDevInfo.s32DevFd != -1)
    {
        close(pstUbiSlot->stUbiDevInfo.s32DevFd);
        pstUbiSlot->stUbiDevInfo.s32DevFd=-1;
    }

    for(u8VolIndex=0; u8VolIndex<MI_UBI_MAX_VOLUME; u8VolIndex++)
    {
        if(pstUbiSlot->pstUbiVolInfoTbl[u8VolIndex])
        {
            if(pstUbiSlot->pstUbiVolInfoTbl[u8VolIndex]->s32VolFd!=-1)
            {
                close(pstUbiSlot->pstUbiVolInfoTbl[u8VolIndex]->s32VolFd);
                pstUbiSlot->pstUbiVolInfoTbl[u8VolIndex]->s32VolFd=-1;
            }

            if(pstUbiSlot->pstUbiVolInfoTbl[u8VolIndex])
            {
                free(pstUbiSlot->pstUbiVolInfoTbl[u8VolIndex]);
                pstUbiSlot->pstUbiVolInfoTbl[u8VolIndex]=0;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions for internal MI mouldes using.
//-------------------------------------------------------------------------------------------------
MI_RESULT mi_ubi_AttachEnvPart(MI_U8 *pu8PartName)
{
#if (MI_LITE == 0)
    _MI_UBI_InitMtdInfo();
    return _MI_UBI_Attach(pu8PartName);
#else
    MI_ERR("Not support\n");
    return MI_ERR_NOT_SUPPORT;
#endif
}


pthread_mutexattr_t _MsOS_Mutex_Attr;
pthread_mutex_t     stMutex;

static int _MsOS_CreateMutex(void)
{
    pthread_mutexattr_init(&_MsOS_Mutex_Attr);
    pthread_mutexattr_setpshared(&_MsOS_Mutex_Attr, PTHREAD_PROCESS_SHARED);
    pthread_mutexattr_settype(&_MsOS_Mutex_Attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&stMutex, &_MsOS_Mutex_Attr);
    return 0;
}


static int _MsOS_DeleteMutex(MI_S32 s32UbiMutex)
{
    pthread_mutex_destroy(&stMutex);
    return 0;
}

static int _MsOS_ObtainMutex(MI_S32 s32MutexId)
{
    pthread_mutex_lock(&stMutex);
    return 0;
}

static int _MsOS_ReleaseMutex(MI_S32 s32MutexId)
{
    pthread_mutex_unlock(&stMutex);
    return 0;
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MI_RESULT MI_UBI_Init(MI_UBI_InitParam_t *pstInitParam)
{
    MI_U8 u8Idx;
    MI_ENTER();

    if(_bIsUBIModuleInit)
    {
        MI_WRN("Module had inited!!!\n");
        return MI_HAS_INITED;
    }

    _s32UbiMutex= -1;
    memset(_astUbiSlot, 0, sizeof(_astUbiSlot));
    //memset(_stUbiVolInfoTbl, 0, sizeof(_stUbiVolInfoTbl));
    //memset(&_stUbiDevInfo, 0, sizeof(_stUbiDevInfo));

    for(u8Idx=0; u8Idx < MI_UBI_SLOT_MAX; u8Idx++)
    {
        _astUbiSlot[u8Idx].u32MagicNum = MI_UBI_MAGIC_NUM;
        _astUbiSlot[u8Idx].stUbiDevInfo.s32DevFd = -1;
    }

    _s32UbiMutex= _MsOS_CreateMutex();
    if (_s32UbiMutex < 0)
    {
        MI_ERR("Create mutex error!!\n");
        return MI_ERR_FAILED;
    }

    _MI_UBI_InitMtdInfo();
    _bIsUBIModuleInit = TRUE;
    MI_EXIT_OK();
    return MI_OK;
}

MI_RESULT MI_UBI_DeInit(void)
{
    MI_ENTER();
    _bIsUBIModuleInit = FALSE;
    _bMtdInfoInited = FALSE;
    if(_s32UbiMutex != -1)
    {
        _MsOS_DeleteMutex(_s32UbiMutex);
        _s32UbiMutex = -1;
    }
    MI_EXIT_OK();
    return MI_OK;
}

MI_RESULT MI_UBI_Attach(MI_U8 *pu8PartName)
{
    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    return _MI_UBI_Attach(pu8PartName);
}

MI_RESULT MI_UBI_Detach(MI_U8 *pu8PartName)
{
    MI_U8 u8Cmd[64];
    MI_U32 u32DeviceId=0;

    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    if(pu8PartName==NULL)
    {
        MI_ERR("Invalid parameter\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    if(MI_UBI_GetDevIdByPartName(pu8PartName, &u32DeviceId)!=MI_OK)
    {
        return MI_ERR_FAILED;
    }

    memset(u8Cmd, 0, sizeof(u8Cmd));
    sprintf((char *)u8Cmd,"ubidetach -d %lu /dev/ubi_ctrl", u32DeviceId);
    if(mi_sys_LaunchSystemCmd(u8Cmd)==MI_ERR_FAILED)
    {
        MI_ERR("detach ubi device id:%lu fail\n", u32DeviceId);
        return MI_ERR_FAILED;
    }
    else
    {
        MI_INFO("detach ubi device id:%lu success\n", u32DeviceId);
        return MI_OK;
    }
}

MI_RESULT MI_UBI_GetDevIdByPartName(MI_U8 *pu8PartName, MI_U32 *pu32DevId)
{
    MI_U32 u32Index;
    MI_U8 u8Buff[32];
    MI_U8 aszPath[64];
    MI_U32 u32MtdNum=0;
    printf("line = %d: \n",__LINE__);
    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {printf("line = %d: \n",__LINE__);
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }
    printf("line = %d: \n",__LINE__);

    if(pu8PartName==NULL || pu32DevId==NULL)
    {printf("line = %d: \n",__LINE__);
        MI_ERR("Invalid Parameter\n");
        return MI_ERR_INVALID_PARAMETER;
    }
    printf("line = %d: \n",__LINE__);

    if(_MI_UBI_FindMtdNumByName(pu8PartName, &u32MtdNum)!=MI_OK)
    {printf("line = %d: \n",__LINE__);
        MI_ERR("Can't find relative MTD: %s\n", pu8PartName);
        return MI_ERR_FAILED;
    }

    for(u32Index=0; u32Index<MI_UBI_MAX_PART_NUM; u32Index++)
    {printf("line = %d: \n",__LINE__);
        memset(u8Buff, 0, sizeof(u8Buff));
        memset(aszPath, 0, sizeof(aszPath));
        snprintf((char *)aszPath, sizeof(aszPath), UBIDEV_INFO_MTD_NUM, u32Index);
        if(_MI_UBI_ReadInfo(aszPath, u8Buff, sizeof(u8Buff))==FALSE)
            return MI_ERR_FAILED;
        printf("line = %d: \n",__LINE__);

        if(u32MtdNum == atoi((const char*)u8Buff))
        {
            MI_INFO("Find deviceId=%lu equal to mtd no=%lu\n", u32Index, u32MtdNum);
            *pu32DevId = u32Index;
            return MI_OK;
        }
    }
    printf("line = %d: \n",__LINE__);

    MI_ERR("No match ubi device has mtd number=%lu\n", u32MtdNum);
    return MI_ERR_FAILED;
}


MI_RESULT MI_UBI_Open(MI_UBI_OpenParam_t *pstOpenParam, MI_HANDLE *phUbiDevice)
{
    MI_U8 aszUbiDevicePath[64], u8Idx=0;
    MI_UBI_Slot_t *pstUbiSlot;
    MI_S32 s32DeviceFd = -1;

    MI_ENTER();

    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    if(pstOpenParam==NULL || phUbiDevice==NULL)
    {
        MI_ERR("Parameter error!!!");
        return MI_ERR_INVALID_PARAMETER;
    }

    for(u8Idx=0; u8Idx<MI_UBI_SLOT_MAX; u8Idx++)
    {
        if(_astUbiSlot[u8Idx].bUsed == FALSE)
        {
            break;
        }
    }

    if(u8Idx >= MI_UBI_SLOT_MAX)
    {
        MI_ERR("No available UART slot!!\n");
        return MI_ERR_RESOURCES;
    }
    else
    {
        MI_U8 u8VolIdx=0;
        pstUbiSlot = &(_astUbiSlot[u8Idx]);
        pstUbiSlot->stUbiDevInfo.u32DeviceId = pstOpenParam->u32DeviceId;

        for(u8VolIdx=0; u8VolIdx<MI_UBI_MAX_VOLUME; u8VolIdx++)
        {
            if(pstUbiSlot->pstUbiVolInfoTbl[u8VolIdx] == NULL)
            {
                pstUbiSlot->pstUbiVolInfoTbl[u8VolIdx] = (MI_UBI_VolInfo_t *)malloc(sizeof(MI_UBI_VolInfo_t));
                if(!pstUbiSlot->pstUbiVolInfoTbl[u8VolIdx])
                {
                    MI_ERR("malloc error\n");
                    _MI_UBI_ReleaseResource(pstUbiSlot);
                    return MI_ERR_MEMORY_ALLOCATE;
                }
                memset(pstUbiSlot->pstUbiVolInfoTbl[u8VolIdx], 0, sizeof(MI_UBI_VolInfo_t));
                pstUbiSlot->pstUbiVolInfoTbl[u8VolIdx]->s32VolFd=-1;
            }
        }

        if(_MI_UBI_Setup(pstUbiSlot) != MI_OK)
        {
            MI_ERR("Setup Device info fail!!!\n");
            _MI_UBI_ReleaseResource(pstUbiSlot);
            return MI_ERR_FAILED;
        }

        snprintf((char *)aszUbiDevicePath, sizeof(aszUbiDevicePath), UBI_DEVICE_NODE_PATH, pstUbiSlot->stUbiDevInfo.u32DeviceId);
        s32DeviceFd = open((char *)aszUbiDevicePath, O_RDWR);
        if (s32DeviceFd < 0)
        {
            MI_ERR("Open %s failed.\n", UBI_DEVICE_NODE_PATH);
            _MI_UBI_ReleaseResource(pstUbiSlot);
            return MI_ERR_FAILED;
        }

        MI_INFO("open %s success, devicd fd=%ld\n", aszUbiDevicePath, s32DeviceFd);
        pstUbiSlot->stUbiDevInfo.s32DevFd = s32DeviceFd;
        pstUbiSlot->bUsed = TRUE;
        *phUbiDevice = (MI_HANDLE) pstUbiSlot;
        MI_EXIT_OK();
        return MI_OK;
    }
}

MI_RESULT MI_UBI_Close(MI_HANDLE hUbiDevice)
{
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;

    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        MI_U8 u8VolIdx=0;
        for(u8VolIdx=0; u8VolIdx<MI_UBI_MAX_VOLUME; u8VolIdx++)
        {
            _MI_UBI_CloseVolume(pstUbiSlot, u8VolIdx);
        }
        _MI_UBI_ReleaseResource(pstUbiSlot);
        memset(&pstUbiSlot->stUbiDevInfo, 0, sizeof(pstUbiSlot->stUbiDevInfo));
        pstUbiSlot->bUsed = FALSE;
        MI_EXIT_OK();
        return MI_OK;
    }
}

MI_RESULT MI_UBI_CreateVolume(MI_HANDLE hUbiDevice, MI_U8 aszVolName[MAX_VOLUME_NAME], MI_U64 u64VolByteSize)
{
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;
    MI_UBI_MakeVolReq_t stMkVolReq;
    MI_ENTER();

    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    if(aszVolName==NULL)
    {
        MI_ERR("Parameter error\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        memset(&stMkVolReq, 0x00, sizeof(stMkVolReq));
        stMkVolReq.s64Bytes = u64VolByteSize;
        stMkVolReq.s8VolType = E_MI_UBI_VOLUME_TYPE_DYNAMIC;
        stMkVolReq.s32VolId = MI_UBI_VOL_NUM_AUTO;
        stMkVolReq.s32Alignment = 1;

        MI_INFO("create volname=%s, len=%d\n", aszVolName, strlen((char *)aszVolName));
        strncpy((char *)stMkVolReq.szName, (char *)aszVolName, sizeof(stMkVolReq.szName));
        stMkVolReq.s16NameLen = strlen((char *)aszVolName);
        MI_INFO("stMkVolReq.name:%s\n", stMkVolReq.szName);
        MI_INFO("stMkVolReq.bytes:%llu\n", stMkVolReq.s64Bytes);

        if(ioctl(pstUbiSlot->stUbiDevInfo.s32DevFd, MI_UBI_IOC_MKVOL, &stMkVolReq) < 0)
        {
            MI_ERR("ioctl failed: %s\n", strerror(errno));
            return MI_ERR_FAILED;
        }

        MI_INFO("Volume %ld created\n", stMkVolReq.s32VolId);

        if(_MI_UBI_Setup(pstUbiSlot) != MI_OK)
        {
            MI_ERR("Setup Device info fail!!!\n");
            _MI_UBI_ReleaseResource(pstUbiSlot);
            return MI_ERR_FAILED;
        }

        MI_EXIT_OK();
        return MI_OK;
    }
}

MI_RESULT MI_UBI_DestroyVolume(MI_HANDLE hUbiDevice, MI_U32 u32VolId)
{
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;

    MI_ENTER();

    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    if(u32VolId>=MI_UBI_MAX_VOLUME)
    {
        MI_ERR("Invalid  volId=%lu\n",u32VolId);
        return MI_ERR_FAILED;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        _MI_UBI_CloseVolume(pstUbiSlot, u32VolId);
        if(ioctl(pstUbiSlot->stUbiDevInfo.s32DevFd, MI_UBI_IOC_RMVOL, &u32VolId) < 0)
        {
            MI_ERR("ioctl failed: %s\n", strerror(errno));
            return MI_ERR_FAILED;
        }

        pstUbiSlot->stUbiDevInfo.s32VolCnt--;
        if(_MI_UBI_Setup(pstUbiSlot) != MI_OK)
        {
            MI_ERR("Setup Device info fail!!!\n");
            _MI_UBI_ReleaseResource(pstUbiSlot);
            return MI_ERR_FAILED;
        }

        MI_EXIT_OK();
        return MI_OK;
    }
}

MI_RESULT MI_UBI_GetBlockSize(MI_HANDLE hUbiDevice, MI_U32 *pu32BlockSize)
{
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;

    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    if(pu32BlockSize==NULL)
    {
        MI_ERR("Parameter error\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
//        MI_INFO("pstUbiSlot->stUbiDevInfo.u32BlockSize=%lu\n",pstUbiSlot->stUbiDevInfo.u32BlockSize);
        *pu32BlockSize = pstUbiSlot->stUbiDevInfo.u32BlockSize;
        MI_EXIT_OK();
        return MI_OK;
    }
}

MI_U32 MI_UBI_GetTotalSize(MI_HANDLE hUbiDevice, MI_U64 *pu64TotalSize)
{
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;

    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    if(pu64TotalSize==NULL)
    {
        MI_ERR("Parameter error\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        *pu64TotalSize = pstUbiSlot->stUbiDevInfo.u64TotalSize;
        MI_EXIT_OK();
        return MI_OK;
    }
}

MI_RESULT MI_UBI_GetVolumeCount(MI_HANDLE hUbiDevice, MI_U32 *pu32VolumeCount)
{
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;

    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    if(pu32VolumeCount==NULL)
    {
        MI_ERR("Parameter error!!!");
        return MI_ERR_INVALID_PARAMETER;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        *pu32VolumeCount = pstUbiSlot->stUbiDevInfo.s32VolCnt;
        MI_EXIT_OK();
        return MI_OK;
    }
}

MI_RESULT MI_UBI_GetVolumeNameById(MI_HANDLE hUbiDevice, MI_U32 u32VolId, MI_U8 aszVolumeName[MAX_VOLUME_NAME])
{
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;

    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    if(u32VolId>=MI_UBI_MAX_VOLUME)
    {
        MI_ERR("Invalid  volId=%lu\n",u32VolId);
        return MI_ERR_FAILED;
    }

    if(aszVolumeName==NULL)
    {
        MI_ERR("Invalid Parameter\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        strcpy((char *)aszVolumeName, (char *)pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->u8VolName);
        MI_EXIT_OK();
        return MI_OK;
    }
}

MI_RESULT MI_UBI_GetVolumeIdByName(MI_HANDLE hUbiDevice, MI_U8 *pu8VolumeName, MI_U32 *pu32VolId)
{
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;

    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    if(pu8VolumeName==NULL || pu32VolId==NULL)
    {
        MI_ERR("Invalid Parameter\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        MI_U32 u32VolIdx=0;
        for(u32VolIdx=0; u32VolIdx<MI_UBI_MAX_VOLUME; u32VolIdx++)
        {
            if(strcasecmp((char *)pu8VolumeName, (char *)pstUbiSlot->pstUbiVolInfoTbl[u32VolIdx]->u8VolName)==0)
            {
                *pu32VolId = u32VolIdx;
                MI_EXIT_OK();
                return MI_OK;
            }
        }
        MI_ERR("No match volume found!!\n");
        return MI_ERR_FAILED;
    }
}

MI_U32 MI_UBI_GetVolumeSize(MI_HANDLE hUbiDevice, MI_U32 u32VolId, MI_U64 *pu64VolumeByteSize)
{
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;

    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    if(u32VolId>=MI_UBI_MAX_VOLUME)
    {
        MI_ERR("Invalid  volId=%lu\n",u32VolId);
        return MI_ERR_FAILED;
    }

    if(pu64VolumeByteSize==NULL)
    {
        MI_ERR("Invalid Parameter\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        *pu64VolumeByteSize = pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->u64VolByteSize;
        MI_EXIT_OK();
        return MI_OK;
    }
}

MI_RESULT MI_UBI_GetVolumeBlockCount(MI_HANDLE hUbiDevice, MI_U32 u32VolId, MI_U32 *pu32BlockCount)
{
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;

    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    if(u32VolId>=MI_UBI_MAX_VOLUME)
    {
        MI_ERR("Invalid  volId=%lu\n",u32VolId);
        return MI_ERR_FAILED;
    }

    if(pu32BlockCount==NULL)
    {
        MI_ERR("Invalid Parameter\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        *pu32BlockCount = pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->u32BlockCnt;
        MI_EXIT_OK();
        return MI_OK;
    }
}

MI_RESULT MI_UBI_ResizeVolume(MI_HANDLE hUbiDevice, MI_U32 u32VolId, MI_S32 s32Size)
{
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;

    MI_UBI_ResizeVolReq_t stResizeVolReq;
    MI_ENTER();

    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    if(u32VolId>=MI_UBI_MAX_VOLUME)
    {
        MI_ERR("Invalid  volId=%lu\n",u32VolId);
        return MI_ERR_FAILED;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        memset(&stResizeVolReq, 0, sizeof(stResizeVolReq));
        stResizeVolReq.s32VolId = u32VolId;
        stResizeVolReq.s64Bytes = s32Size;

        _MI_UBI_CloseVolume(pstUbiSlot, u32VolId);
        if(ioctl(pstUbiSlot->stUbiDevInfo.s32DevFd, MI_UBI_IOC_RSVOL, &stResizeVolReq) < 0)
        {
            MI_ERR("ioctl failed: %s\n", strerror(errno));
            return MI_ERR_FAILED;
        }

        if(_MI_UBI_Setup(pstUbiSlot) != MI_OK)
        {
            MI_ERR("Setup Device info fail!!!\n");
            _MI_UBI_ReleaseResource(pstUbiSlot);
            return MI_ERR_FAILED;
        }

        MI_EXIT_OK();
        return MI_OK;
    }
}

MI_S32 MI_UBI_ReadVolumeBytes(MI_HANDLE hUbiDevice, MI_UBI_VolumeAccess_t *pstReadBytes)
{
    MI_U32 u32VolumeOffset = pstReadBytes->u32ByteOffset;
    MI_S32 s32Readlen=0;
    MI_UBI_Slot_t *pstUbiSlot=NULL;

    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return -1;
    }

    if(pstReadBytes->pu8Buf==NULL)
    {
        MI_ERR("Invalid Parameter\n");
        return -1;
    }

    if(_MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot) != MI_OK)
    {
        return -1;
    }
    else
    {
        if(_MI_UBI_OpenVolume(pstUbiSlot, pstReadBytes->u32VolId)!=MI_OK)
        {
            MI_ERR("open volume fail\n");
            return -1;
        }

        UBI_LOCK();
        lseek(pstUbiSlot->pstUbiVolInfoTbl[pstReadBytes->u32VolId]->s32VolFd, u32VolumeOffset, SEEK_SET);
        s32Readlen = read(pstUbiSlot->pstUbiVolInfoTbl[pstReadBytes->u32VolId]->s32VolFd, pstReadBytes->pu8Buf, pstReadBytes->u32Length);
        if(s32Readlen==-1)
        {
            MI_ERR("Read volume[%lu] failed: %s\n", pstReadBytes->u32VolId, strerror(errno));
        }
        else if (s32Readlen != pstReadBytes->u32Length)
        {
            MI_WRN("UBI read volume[%lu] failed, s32Retlen=%ld.\n", pstReadBytes->u32VolId, s32Readlen);
        }
        UBI_UNLOCK();
        MI_EXIT_OK();
        return s32Readlen;
    }
}

MI_S32 MI_UBI_WriteVolumeBytes(MI_HANDLE hUbiDevice, MI_UBI_VolumeAccess_t *pstWriteBytes)
{
    MI_U32 u32VolId=pstWriteBytes->u32VolId;
    MI_S32 s32Remainder=0;
    MI_U32 u32LebNum=0;
    MI_U32 u32Bytes2Write=0;
    MI_U32 u32BytesWritten = 0;
    MI_U32 u32LEBSize=0;
    MI_UBI_Slot_t *pstUbiSlot=NULL;
    MI_U32 u32WriteLength = pstWriteBytes->u32Length;
    MI_S32 s32AccessLen=0;

    MI_ENTER();
    MI_INFO("Write Offset=%lu Write length=%lu\n",pstWriteBytes->u32ByteOffset, pstWriteBytes->u32Length);
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return -1;
    }

    if(pstWriteBytes->pu8Buf == NULL)
    {
        MI_ERR("Invalid Parameter\n");
        return -1;
    }

    if(_MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot) != MI_OK)
    {
        return -1;
    }
    else
    {
        if(MI_UBI_GetBlockSize(hUbiDevice, &u32LEBSize)!=MI_OK)
        {
            MI_ERR("Get block size failed!\n");
            return -1;
        }

        s32Remainder = _MI_UBI_Offset2VolumeLebNum(hUbiDevice, pstWriteBytes->u32ByteOffset, &u32LebNum);
        if(s32Remainder<0)
        {
            MI_ERR("Offset to VolumeLebNum fail\n");
            return -1;
        }

        if (s32Remainder > 0)
        {
            u32Bytes2Write = u32LEBSize - s32Remainder;
            if (u32Bytes2Write > u32WriteLength)
                u32Bytes2Write = u32WriteLength;

            if((s32AccessLen=_MI_UBI_WriteVolumeLeb(hUbiDevice, u32VolId, u32LebNum, s32Remainder, pstWriteBytes->pu8Buf, u32Bytes2Write))<0)
            {
                MI_ERR("Write volume LEB[%lu] failed\n", u32LebNum);
                goto ubi_wirte_exit;
            }
            else if(s32AccessLen!=u32Bytes2Write)
            {
                MI_WRN("Want to Write volume LEB[%lu] %lu bytes but only partial data %ld written\n", u32LebNum, u32Bytes2Write, s32AccessLen);
                u32BytesWritten += s32AccessLen;
                goto ubi_wirte_exit;
            }
            else if(s32AccessLen == u32Bytes2Write)//write volume success
            {
                s32Remainder = 0;
                u32BytesWritten += u32Bytes2Write;
                u32WriteLength -= u32Bytes2Write;
                u32LebNum++;
            }
            else
            {
                MI_ERR("check flow!!!\n");
            }
        }

        while (u32WriteLength >= u32LEBSize)
        {
            if ((s32AccessLen=_MI_UBI_WriteVolumeLeb(hUbiDevice, u32VolId, u32LebNum, 0, pstWriteBytes->pu8Buf + u32BytesWritten, u32LEBSize))<0)
            {
                MI_ERR("Write volume LEB[%lu] failed\n", u32LebNum);
                goto ubi_wirte_exit;
            }
            else if(s32AccessLen!=u32LEBSize)
            {
                MI_WRN("Write volume LEB[%lu] partial data %ld\n", u32LebNum, s32AccessLen);
                u32BytesWritten += s32AccessLen;
                goto ubi_wirte_exit;
            }
            else if(s32AccessLen==u32LEBSize)//write volume success
            {
                u32BytesWritten += u32LEBSize;
                u32WriteLength -= u32LEBSize;
                u32LebNum++;
            }
            else
            {
                MI_ERR("check flow!!!\n");
            }
        }

        if (u32WriteLength > 0)
        {
            if ((s32AccessLen=_MI_UBI_WriteVolumeLeb(hUbiDevice, u32VolId, u32LebNum, s32Remainder, pstWriteBytes->pu8Buf + u32BytesWritten, u32WriteLength))<0)
            {
                MI_ERR("read volume LEB[%lu]err!\n", u32LebNum);
                goto ubi_wirte_exit;
            }
            else if(s32AccessLen!=u32WriteLength)
            {
                MI_WRN("Write volume LEB[%lu] partial data %ld\n", u32LebNum, s32AccessLen);
                u32BytesWritten += s32AccessLen;
                goto ubi_wirte_exit;
            }
            else if(s32AccessLen==u32WriteLength)
            {
                u32BytesWritten += u32WriteLength;
            }
            else
            {
                MI_ERR("check flow!!!\n");
            }
        }
    }

ubi_wirte_exit:
    return u32BytesWritten;
}

MI_RESULT MI_UBI_CheckVolumeExist(MI_HANDLE hUbiDevice, MI_U32 u32VolId, MI_BOOL *pbExist)
{
    MI_RESULT eErrCode=MI_ERR_FAILED;
    MI_UBI_Slot_t *pstUbiSlot=NULL;

    MI_ENTER();
    if(_bIsUBIModuleInit==FALSE)
    {
        MI_ERR("Not Inited...call init first\n");
        return MI_ERR_NOT_INITED;
    }

    if(pbExist==NULL)
    {
        MI_ERR("Invalid parameter\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    eErrCode = _MI_UBI_Handle2Slot(hUbiDevice, &pstUbiSlot);
    if(eErrCode != MI_OK)
    {
        return eErrCode;
    }
    else
    {
        if(u32VolId>=MI_UBI_MAX_VOLUME)
        {
            MI_ERR("Invalid  volId=%lu\n",u32VolId);
            return MI_ERR_FAILED;
        }

        if(pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->u32BlockSize==0 || pstUbiSlot->pstUbiVolInfoTbl[u32VolId]->u64VolByteSize==0)
        {
            *pbExist = FALSE;
        }
        else
            *pbExist = TRUE;

        MI_EXIT_OK();
        return MI_OK;
    }
}

MI_RESULT MI_UBI_SetDebugLevel(MI_DBG_LEVEL_e eDbgLevel)
{
    MI_ENTER();
    _u8UbiDbgLevel = eDbgLevel;
    return MI_OK;
}


