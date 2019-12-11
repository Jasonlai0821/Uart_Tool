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
/// @file   mi_input.c
/// @brief INPUT module
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/times.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "mi_common.h"
#include "mi_os.h"
#include "mi_input.h"
#include "mi_sys.h"



//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Defines
//-------------------------------------------------------------------------------------------------
#define DBG_LEVEL                       _gu8InputDbgLevel

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
// read from cst066 i2c
#define BUTTON_NUM_SLEEP              0x3
#define BUTTON_NUM_PAUSE              0x2
#define BUTTON_NUM_VOLUMEDOWN         0x8
#define BUTTON_NUM_VOLUMEUP           0xA
#define BUTTON_NUM_WAKEUP             0x0

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MI_U8 _gu8InputDbgLevel = MI_DBG_ERR;
static char* _pDeviceInput= "/dev/input/event0";
static char* _pDeviceI2c= "/dev/cst066";
static int _gInputFd = -1;
static int _gI2cFd = -1;
static MI_BOOL gInitDoneFlag = FALSE;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MI_RESULT MI_INPUT_Init(MI_INPUT_InitParam_t *pstInitParam)
{
    MI_ENTER();
    if(gInitDoneFlag)
    {
        MI_ERR("INPUT has inited!\n");
        return MI_HAS_INITED;
    }

    _gInputFd = open(_pDeviceInput, O_RDONLY);
    if(_gInputFd < 0)
    {
        MI_ERR("%s Unable to open %s! inputFd = %d\n", __FUNCTION__, _pDeviceInput, _gInputFd);
        return MI_ERR_FAILED;
    }

    _gI2cFd = open(_pDeviceI2c, O_RDONLY);
    if(_gI2cFd < 0)
    {
        MI_ERR("%s Unable to open %s! i2cFd = %d\n", __FUNCTION__, _pDeviceI2c, _gI2cFd);
        close(_gInputFd);
        return MI_ERR_FAILED;
    }

    gInitDoneFlag = TRUE;

    MI_EXIT_OK();
    return MI_OK;
}

MI_RESULT MI_INPUT_DeInit(void)
{
    MI_ENTER();

    if(!gInitDoneFlag)
    {
        MI_ERR("please call MI_INPUT_Init first!\n");
        return MI_ERR_NOT_INITED;
    }

    gInitDoneFlag = FALSE;

    MI_EXIT_OK();
    return MI_OK;
}

static MI_INPUT_KEY_Event_e _MI_INPUT_ConvertI2cKey2Mi(MI_U8 u8I2cKeyCode)
{
    MI_INPUT_KEY_Event_e eMiKey = 0;

    switch(u8I2cKeyCode)
    {
        case BUTTON_NUM_WAKEUP:
            eMiKey = MI_INPUT_KEY_WAKEUP;
            break;
        case BUTTON_NUM_VOLUMEDOWN:
            eMiKey = MI_INPUT_KEY_VOLUMEDOWN;
            break;
        case BUTTON_NUM_VOLUMEUP:
            eMiKey = MI_INPUT_KEY_VOLUMEUP;
            break;
        case BUTTON_NUM_PAUSE:
            eMiKey = MI_INPUT_KEY_PLAYPAUSE;
            break;
        case BUTTON_NUM_SLEEP:
            eMiKey = MI_INPUT_KEY_SLEEP;
            break;
        default:
            MI_ERR("Not support i2c key = %d\n", u8I2cKeyCode);
            break;
    }
    return eMiKey;
}


MI_RESULT MI_INPUT_GetKeyStatus(MI_BOOL *pbStatus)
{
    struct input_event event;

    if(read(_gInputFd, &event, sizeof(event)) == sizeof(event))
    {
        if(event.type == EV_KEY)
        {
            // key down: 1, key up: 0
            *pbStatus = event.value;
        }
    }
    return MI_OK;
}

MI_RESULT MI_INPUT_GetKeyValue(MI_INPUT_KEY_Event_e *peValue)
{
// depends on cst066 chip
#define BUF_DATA_SIZE   3

    char buffer[BUF_DATA_SIZE];

    if(read(_gI2cFd, &buffer, sizeof(buffer)) == sizeof(buffer))
    {
        *peValue = _MI_INPUT_ConvertI2cKey2Mi(buffer[2]);
    }
    return MI_OK;
}

MI_RESULT MI_INPUT_SetDebugLevel(MI_DBG_LEVEL_e eDbgLevel)
{
    _gu8InputDbgLevel = eDbgLevel;
    return MI_OK;
}
