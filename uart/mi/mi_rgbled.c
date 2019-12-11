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

#include "mi_common.h"
#include "mi_rgbled.h"


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Defines
//-------------------------------------------------------------------------------------------------
#define DBG_LEVEL                       MI_DBG_ERR

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MI_WRITE_CONTROL_DATA_TO_DEVICE_START   "echo -n -e "
#define MI_WRITE_CONTROL_DATA_TO_DEVICE_END     " > /dev/led-hrgb32k3"
#define MI_READ_CURRENT_COLOR_FROM_DEVICE       "dd bs=24 count=1 if=/dev/led-hrgb32k3"



//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MI_RESULT mi_sys_LaunchSystemCmd(MI_U8 *pszCmd)
{
    //signal(SIGCHLD, SIG_DFL);
    if(system((char *)pszCmd) != 0)
    {
        return MI_ERR_FAILED;
    }

    return MI_OK;
}


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MI_RESULT MI_RGBLED_SetColor(MI_U32 u32Led_1_Color, MI_U32 u32Led_2_Color, MI_U32 u32Led_3_Color, MI_U32 u32Led_4_Color, MI_U32 u32Led_5_Color, MI_U32 u32Led_6_Color, MI_U32 u32Led_7_Color, MI_U32 u32Led_8_Color)
{
    struct stat sts;

    MI_U8 u8Cmd[512]= {0};

    snprintf((char *)u8Cmd, sizeof(u8Cmd), MI_WRITE_CONTROL_DATA_TO_DEVICE_START"\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X""\\\\x%02X"MI_WRITE_CONTROL_DATA_TO_DEVICE_END, ((u32Led_1_Color&0x00ff0000)>>16),((u32Led_1_Color&0x0000ff00)>>8),(u32Led_1_Color&0x000000ff),((u32Led_2_Color&0x00ff0000)>>16),((u32Led_2_Color&0x0000ff00)>>8),(u32Led_2_Color&0x000000ff),((u32Led_3_Color&0x00ff0000)>>16),((u32Led_3_Color&0x0000ff00)>>8),(u32Led_3_Color&0x000000ff),((u32Led_4_Color&0x00ff0000)>>16),((u32Led_4_Color&0x0000ff00)>>8),(u32Led_4_Color&0x000000ff),((u32Led_5_Color&0x00ff0000)>>16),((u32Led_5_Color&0x0000ff00)>>8),(u32Led_5_Color&0x000000ff),((u32Led_6_Color&0x00ff0000)>>16),((u32Led_6_Color&0x0000ff00)>>8),(u32Led_6_Color&0x000000ff),((u32Led_7_Color&0x00ff0000)>>16),((u32Led_7_Color&0x0000ff00)>>8),(u32Led_7_Color&0x000000ff),((u32Led_8_Color&0x00ff0000)>>16),((u32Led_8_Color&0x0000ff00)>>8),(u32Led_8_Color&0x0000ffff));
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

MI_RESULT MI_RGBLED_ReadColor(void)
{
    struct stat sts;

    MI_U8 u8Cmd[128]= {0};

    snprintf((char *)u8Cmd, sizeof(u8Cmd), MI_READ_CURRENT_COLOR_FROM_DEVICE);
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

MI_RESULT MI_RGBLED_SetDebugLevel(MI_DBG_LEVEL_e eDbgLevel)
{
    MI_ENTER();
    //_u8UbiDbgLevel = eDbgLevel;
    return MI_OK;
}


