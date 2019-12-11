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
#include "mi_sys.h"


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Defines
//-------------------------------------------------------------------------------------------------
#define DBG_LEVEL                       _u8SysDbgLevel

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MI_U8 _u8SysDbgLevel = MI_DBG_ALL;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MI_RESULT MI_SYS_LaunchSystemCmd(MI_U8 *pszCmd)
{
    //signal(SIGCHLD, SIG_DFL);
    MI_INFO("CMD:%s\n", (char *)pszCmd);
    if(system((char *)pszCmd) != 0)
    {
        return MI_ERR_FAILED;
    }

    return MI_OK;
}

//-------------------------------------------------------------------------------------------------
//  Local get time Functions
//-------------------------------------------------------------------------------------------------
static inline unsigned long long get_time0_us(void)
{
    struct timespec ts;
    unsigned long long us;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    us = (ts.tv_sec * 1000000) + (ts.tv_nsec / 1000);
    if(us == 0)
    {
        us = 1;
    }
    return us;
}

static inline unsigned long long time_us_diff_from_now0(unsigned long long Time0)
{
    unsigned long long Now;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    Now = (ts.tv_sec * 1000000) + (ts.tv_nsec / 1000);

    if(Now >= Time0)
    {
        return Now - Time0;
    }
    else
    {
        return (0xFFFFFFFFFFFFFFFF - Time0) + Now;
    }
}

static inline unsigned int get_time0_ms(void)
{
    struct timespec ts;
    unsigned int ms;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    ms = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
    if(ms == 0)
    {
        ms = 1;
    }
    return ms;
}

static inline unsigned int time_ms_diff_from_now0(unsigned int Time0)
{
    unsigned int Now;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    Now = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);

    if(Now >= Time0)
    {
        return Now - Time0;
    }
    else
    {
        return (0xFFFFFFFF - Time0) + Now;
    }
}

//-------------------------------------------------------------------------------------------------
//  Global MI Functions
//-------------------------------------------------------------------------------------------------

// used for Waiting time
void MI_Delayus(unsigned int us)
{
    unsigned long long currentTime = get_time0_us();
    while(time_us_diff_from_now0(currentTime) < us);
}

void MI_Delayms(unsigned int ms)
{
    unsigned int currentTime = get_time0_ms();
    while(time_ms_diff_from_now0(currentTime) < ms);
}

int MI_Get_SystemTime_ms(void)
{
    unsigned int currentTime = get_time0_ms();
    return currentTime;
}


MI_RESULT MI_SYS_SetDebugLevel(MI_DBG_LEVEL_e eDbgLevel)
{
    MI_ENTER();

    _u8SysDbgLevel = eDbgLevel;

    MI_EXIT_OK();
    return MI_OK;
}


