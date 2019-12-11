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

////////////////////////////////////////////////////////////////////////////////
/// @file   mi_sar.c
/// @brief  sar moudle
/// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//  Include Files
//------------------------------------------------------------------------------
#include "string.h"
//#include "drvSAR.h"
#include "mi_common.h"
#include "mi_sar.h"
#include "mi_sys.h"
#include "internal/mi_sys_internal.h"
#include "mi_os.h"
#include "mdrv_sar.h"
#include <fcntl.h>
#include <linux/string.h>
#include <stdio.h>
#include <limits.h>
#include <linux/mman.h>
#include <unistd.h>
#include <sys/times.h>
#include <poll.h>
#include <malloc.h>
#include <sys/ioctl.h>
#include <signal.h>
#include "irqreturn.h"
#include "ms_types.h"
#include <linux/sched.h>
#include <linux/kernel.h>
#include <pthread.h>
#include <err.h>
#include <time.h>
//------------------------------------------------------------------------------
//  Local Compiler Options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Debug Defines
//------------------------------------------------------------------------------
#define DBG_LEVEL                   _u8SarDbgLevel

#ifdef MS_DEBUG
    #define DBG_TRACE(fmt, args...) { if (DBG_LEVEL >= MI_DBG_INFO) {printf(fmt, ##args);} }
    #define DBG_WARN(fmt, args...)  { if (DBG_LEVEL >= MI_DBG_WRN) {printf(fmt, ##args);} }
    #define DBG_ERROR(fmt, args...) { if (DBG_LEVEL >= MI_DBG_ERR) {printf(fmt, ##args);} }
#else
    #define DBG_TRACE(fmt, args...) {}
    #define DBG_WARN(fmt, args...)  {}
    #define DBG_ERROR(fmt, args...) {}
#endif


//------------------------------------------------------------------------------
//  Local Defines
//------------------------------------------------------------------------------

#define SAR_DEFAULT_VALUE 0x3FF
#define KEY_DUMMY 0xFF
#define KEYPAD_DEFAULT_INIT_DELAY_TIME_MS    50//300
#define KEYPAD_DEFAULT_2ND_DELAY_TIME_MS    2000//300
#define KEYPAD_DEFAULT_FOLLOWING_DELAY_TIME_MS    330//100

//------------------------------------------------------------------------------
//  Local Structures
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//  Global Variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Variables
//------------------------------------------------------------------------------
static MI_U8 _u8SarDbgLevel = MI_DBG_ALL;
static MI_SAR_Callback _pfSARGetKeyCallback = NULL;
static char* pDevice_SAR = "/dev/saradc";
static MI_SAR_RegCfg_t  _stSarKpdCfg;

static MI_U8            _u8RepFlag = 0;
static KeypadProperty   _Key_ePrevKeyProperty = E_KEYPAD_KEY_PROPERTY_FOLLOWING;
static MI_U8            _Key_u8PrevKeyCode = KEY_DUMMY;
static MI_U32           _Key_u32PrevKeyTime = 0;

//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------
unsigned int _GetTime0(void)
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

unsigned int _TimeDiffFromNow0(unsigned int u32Time)
{
    unsigned int u32Now;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    u32Now = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);

    if(u32Now >= u32Time)
    {
        return u32Now - u32Time;
    }
    else
    {
        return (0xFFFFFFFF - u32Time) + u32Now;
    }
}

MI_RESULT MI_SAR_GetValue(MI_U8 u8SARNum, MI_U32 *peLevel)
{
    int sarFd = -1;
    ADC_CONFIG_READ_ADC Read_ADC_Config;

    Read_ADC_Config.adc_value=0;
    Read_ADC_Config.channel_value=u8SARNum;
    sarFd = open(pDevice_SAR, O_WRONLY);
    if( sarFd < 0 )
    {
        DBG_ERROR("MI_SAR_GetValue Unable to open Read_ADC_Config! sarFd = %d\n", sarFd);
        return MI_ERR_FAILED;
    }
    ioctl(sarFd, MS_SAR_SET_CHANNEL_READ_VALUE, &Read_ADC_Config);
    *peLevel = Read_ADC_Config.adc_value;
    //printf("adc_value = 0x%X ", Read_ADC_Config.adc_value);

    close(sarFd);

    return MI_OK;
}

MI_RESULT MI_SAR_VoltageVBat_GetValue( MI_U32 pgain,MI_U32 *volLevel)
{
    int sarFd = -1;
    sarFd = open(pDevice_SAR, O_WRONLY);
    if( sarFd < 0 )
    {
        DBG_ERROR("MI_SAR_GetValue Unable to open ! sarFd = %d\n", sarFd);
        return MI_ERR_FAILED;
    }
    *volLevel=(int)ioctl(sarFd, MS_SAR_GET_BATTERY_VOLTAGEVBAT_VALUE, pgain);
    close(sarFd);
	//printf("MI_SAR_VoltageVBat_GetValue: pgain =%d  VoltageVBat = %d \n",pgain,*volLevel);

    return MI_OK;
}


//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------
MI_RESULT MI_SAR_Init(void)
{
    MI_ENTER();

    int sarFd = -1;
    sarFd = open(pDevice_SAR, O_WRONLY);
    if( sarFd < 0 )
    {
        DBG_ERROR("MI_SAR_Init Unable to open saradc! sarFd = %d\n", sarFd);
        return MI_ERR_FAILED;
    }
    if( ioctl(sarFd, MS_SAR_INIT) < 0 )
    {
        DBG_ERROR("ioctl MS_SAR_INIT failed!\n");
        return MI_ERR_FAILED;
    }
    close(sarFd);

    MI_EXIT_OK();
    return MI_OK;
}

MI_RESULT MI_SAR_DeInit(void)
{
    DBG_TRACE("ioctl MI_SAR_DeInit!\n");
    return MI_OK;
}

int MI_SAR_Interrupt_callback(int level)
{
    int i;
    DBG_TRACE("level = 0x%X\n", level);
    for(i = 0; i<_stSarKpdCfg.u8KeyLevelNum; i++)
    {
        if((level >= _stSarKpdCfg.au32KeyThreshold[i]+_stSarKpdCfg.u8LoBnd)&&(level <= _stSarKpdCfg.au32KeyThreshold[i]+_stSarKpdCfg.u8UpBnd))//0x0
        {
            _pfSARGetKeyCallback(_stSarKpdCfg.au8KeyCode[i], 0);
        }
    }
    return MI_OK;
}

static MI_BOOL _Keypad_GetKey(MI_U8 u8Channel, MI_U8 *pu8Key, MI_U8 *pu8Flag)
{
    MI_U32 u32ChData, u32KeyData;
    MI_U32 CurTime = 0;
    MI_BOOL bGetKey = FALSE;
    int i;

    MI_SAR_GetValue(_stSarKpdCfg.u8SarChID, &u32KeyData);
    //DBG_TRACE("MI_SAR_GetValue u32KeyData=0x%X\n", u32KeyData);

    if( u32KeyData > SAR_DEFAULT_VALUE-KEY_DUMMY+_stSarKpdCfg.u8UpBnd ) //Keypad released
    {
        if(_Key_ePrevKeyProperty == E_KEYPAD_KEY_PROPERTY_2st)
        {
            _Key_ePrevKeyProperty = E_KEYPAD_KEY_PROPERTY_INIT;
            bGetKey = TRUE;
            *pu8Key = _Key_u8PrevKeyCode;
            *pu8Flag = _u8RepFlag;
            _u8RepFlag = 0;
            _Key_u32PrevKeyTime = 0;
            return TRUE;
        }
        else
        {
            _Key_ePrevKeyProperty = E_KEYPAD_KEY_PROPERTY_INIT;
            _u8RepFlag = 0;
            _Key_u32PrevKeyTime = 0;
            return FALSE;
        }
    }
    else
    {
        DBG_TRACE("KEY GOT = 0x%X\n", u32KeyData);
        bGetKey = TRUE;
    }

    if(bGetKey)
    {
        for(i = 0; i<_stSarKpdCfg.u8KeyLevelNum; i++)
        {
            if((u32KeyData >= _stSarKpdCfg.au32KeyThreshold[i]+_stSarKpdCfg.u8LoBnd)
                &&(u32KeyData <= _stSarKpdCfg.au32KeyThreshold[i]+_stSarKpdCfg.u8UpBnd))
            {
                u32ChData =_stSarKpdCfg.au8KeyCode[i];
                bGetKey = TRUE;
                break;
            }
            else
            {
                bGetKey = FALSE;
            }
        }

        if(bGetKey) //Keypad pressed
        {
            if (_Key_u8PrevKeyCode != u32ChData)
            {
                _Key_ePrevKeyProperty = E_KEYPAD_KEY_PROPERTY_INIT;
            }

            CurTime = _GetTime0();

            DBG_TRACE("_GetTime0 CurTime=%d, _Key_u32PrevKeyTime=%d\n", CurTime, _Key_u32PrevKeyTime);
            if( _Key_ePrevKeyProperty == E_KEYPAD_KEY_PROPERTY_INIT )
            {
                //Key pressed
                DBG_TRACE("E_KEYPAD_KEY_PROPERTY_INIT\n");
                _Key_u8PrevKeyCode = u32ChData;
                _Key_u32PrevKeyTime = CurTime;
                _Key_ePrevKeyProperty = E_KEYPAD_KEY_PROPERTY_1st;
                bGetKey = FALSE;
            }
            else if(_Key_ePrevKeyProperty == E_KEYPAD_KEY_PROPERTY_1st)
            {
                DBG_TRACE("E_KEYPAD_KEY_PROPERTY_1st %d VS %d\n", CurTime - _Key_u32PrevKeyTime, KEYPAD_DEFAULT_INIT_DELAY_TIME_MS);

                if( (CurTime - _Key_u32PrevKeyTime) > KEYPAD_DEFAULT_INIT_DELAY_TIME_MS ) //key>50ms
                {
                    _Key_u32PrevKeyTime = CurTime;
                    _Key_ePrevKeyProperty  = E_KEYPAD_KEY_PROPERTY_2st;
                    bGetKey = FALSE;
                }
                else
                {
                    bGetKey = FALSE;
                }
            }
            else if(_Key_ePrevKeyProperty == E_KEYPAD_KEY_PROPERTY_2st)
            {
                //2000ms>key>50ms
                DBG_TRACE("E_KEYPAD_KEY_PROPERTY_2st %d VS %d\n", CurTime - _Key_u32PrevKeyTime, KEYPAD_DEFAULT_2ND_DELAY_TIME_MS);
                if( (CurTime - _Key_u32PrevKeyTime) > KEYPAD_DEFAULT_2ND_DELAY_TIME_MS )
                {
                    _Key_u32PrevKeyTime = CurTime;
                    _Key_ePrevKeyProperty  = E_KEYPAD_KEY_PROPERTY_FOLLOWING;
                }
                else
                {
                    bGetKey = FALSE;
                }
            }
            else //E_KEYPAD_KEY_PROPERTY_FOLLOWING
            {
                //key>2000ms
                DBG_TRACE("E_IR_KEY_PROPERTY_FOLLOWING %d VS %d\n", CurTime - _Key_u32PrevKeyTime, KEYPAD_DEFAULT_FOLLOWING_DELAY_TIME_MS);
                if( (CurTime - _Key_u32PrevKeyTime) > KEYPAD_DEFAULT_FOLLOWING_DELAY_TIME_MS )
                {
                    _Key_u32PrevKeyTime = CurTime;
                    //bGetKey = TRUE;
                    _u8RepFlag = 1;
                }
                else
                {
                    bGetKey = FALSE;
                }
            }
        }
        else
        {
            u32ChData = KEY_DUMMY;
            _u8RepFlag = 0;
            bGetKey = FALSE;
        }
    }
    else
    {
        u32ChData = KEY_DUMMY;
        _u8RepFlag = 0;
    }
    *pu8Key = u32ChData;
    *pu8Flag = _u8RepFlag;
    return bGetKey;
}

MI_RESULT MI_SAR_SetCallback(MI_SAR_Callback pCallback)
{
    MI_ENTER();
    _pfSARGetKeyCallback = pCallback;
    MI_EXIT_OK();
    return MI_OK;
}

MI_RESULT MI_SAR_Config(MI_SAR_RegCfg_t *pstSarRegCfg)
{
    MI_ENTER();
    int i = 0;

    memset(&_stSarKpdCfg, 0, sizeof(MI_SAR_RegCfg_t));

    _stSarKpdCfg.u8SarChID = pstSarRegCfg->u8SarChID;
    _stSarKpdCfg.u8KeyLevelNum = pstSarRegCfg->u8KeyLevelNum;
    _stSarKpdCfg.u8LoBnd = pstSarRegCfg->u8LoBnd;
    _stSarKpdCfg.u8UpBnd = pstSarRegCfg->u8UpBnd;

    for(i = 0; i<_stSarKpdCfg.u8KeyLevelNum; i++)
    {
        _stSarKpdCfg.au8KeyCode[i] = pstSarRegCfg->au8KeyCode[i];
        _stSarKpdCfg.au32KeyThreshold[i] = pstSarRegCfg->au32KeyThreshold[i];
    }

    MI_EXIT_OK();
    return MI_OK;
}

void* SAR_Interrupt(void *arg)
{
    MI_U32 level;
    MI_U8 u8Channel = 0;
    MI_U8 u8Key;
    MI_U8 u8Flag;

    if(MI_OK != MI_SAR_Init())
    {
        return FALSE;
    }
    while(1)
    {
        if( _Keypad_GetKey(u8Channel, &u8Key, &u8Flag) )
        {
            if(_pfSARGetKeyCallback)
            {
                _pfSARGetKeyCallback(u8Key, u8Flag);
            }
        }
        usleep(30000);
    }

    return((void*)0);
}

int MI_SAR_EnableInterrupt_init(void)
{
    int err;
    static pthread_t ntid;

    err = pthread_create(&ntid, NULL, SAR_Interrupt, NULL);
    if(err != 0)
    {
        DBG_ERROR("can't create thread\n");
        return MI_ERR_FAILED;
    }
    return MI_OK;
}

MI_RESULT MI_SAR_EnableInterrupt(void)
{
    return MI_OK;
}

MI_RESULT MI_SAR_DisableInterrupt(void)
{
    MI_RESULT eErrCode = MI_ERR_FAILED;

    return eErrCode;
}

MI_RESULT MI_SAR_SetDebugLevel(MI_DBG_LEVEL_e eDbgLevel)
{
    MI_ENTER();

    _u8SarDbgLevel = eDbgLevel;

    MI_EXIT_OK();
    return MI_OK;
}
