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
/// @file   mi_gpio.c
/// @brief GPIO module
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include <linux/string.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <linux/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>
#include <poll.h>
#include <malloc.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "mi_common.h"
#include "mi_os.h"
#include "mi_gpio.h"
#include "mi_sys.h"
#include "mdrv_gpio_io.h"
#include "gpio.h"
#include <linux/sched.h>
#include <linux/kernel.h>
#include <pthread.h>
#include <err.h>


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Defines
//-------------------------------------------------------------------------------------------------
#define DBG_LEVEL                       _gu8GpioDbgLevel

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MI_U8 _gu8GpioDbgLevel = MI_DBG_ERR;
static char* pDevice_gpio = "/dev/gpioioctl";

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MI_RESULT MI_GPIO_Init(MI_U8 u8GpioNum)
{
    MI_ENTER();
    int gpioFd = -1;
    MI_U8 gpionum = u8GpioNum;
    //MI_INFO("MI_GPIO_Init gpionum = %d\n", gpionum);
    gpioFd = open(pDevice_gpio, O_WRONLY);
    if( gpioFd < 0 )
    {
        MI_ERR("MI_GPIO_Init Unable to open gpioioctl! gpioFd = %d\n", gpioFd);
        return MI_ERR_FAILED;
    }
    if( ioctl(gpioFd, IOCTL_GPIO_SET, gpionum) < 0 )
    {
        MI_ERR("ioctl IOCTL_GPIO_SET failed!\n");
        return MI_ERR_FAILED;
    }
    close(gpioFd);
    return MI_OK;
}

MI_RESULT MI_GPIO_DeInit(MI_U8 u8GpioNum)
{
    MI_ENTER();
    int gpioFd = -1;
    MI_U8 gpionum = u8GpioNum;
    //MI_INFO("MI_GPIO_DeInit gpionum = %d\n", gpionum);
    gpioFd = open(pDevice_gpio, O_WRONLY);
    if( gpioFd < 0 )
    {
        MI_ERR("MI_GPIO_DeInit Unable to open gpioioctl! gpioFd = %d\n", gpioFd);
        return MI_ERR_FAILED;
    }
    if( ioctl(gpioFd, IOCTL_GPIO_FREE, gpionum) < 0 )
    {
        MI_ERR("ioctl IOCTL_GPIO_FREE failed!\n");
        return MI_ERR_FAILED;
    }
    close(gpioFd);
    return MI_OK;
}

MI_RESULT MI_GPIO_SetInput(MI_U8 u8GpioNum)
{
    MI_ENTER();
    int gpioFd = -1;
    MI_U8 gpionum = u8GpioNum;
    //MI_INFO("MI_GPIO_SetInput gpionum = %d\n", gpionum);
    gpioFd = open(pDevice_gpio, O_WRONLY);
    if( gpioFd < 0 )
    {
        MI_ERR("MI_GPIO_SetInput Unable to open gpioioctl! gpioFd = %d\n", gpioFd);
        return MI_ERR_FAILED;
    }
    if( ioctl(gpioFd, IOCTL_GPIO_ODN, gpionum) < 0 )
    {
        MI_ERR("ioctl IOCTL_GPIO_ODN failed!\n");
        return MI_ERR_FAILED;
    }

    close(gpioFd);
    return MI_OK;
}

MI_RESULT MI_GPIO_SetOutput(MI_U8 u8GpioNum, MI_GPIO_Level_e eLevel)
{
    MI_ENTER();
    int gpioFd = -1;
    MI_U8 gpionum = u8GpioNum;
    //MI_INFO("MI_GPIO_SetOutput gpionum = %d\n", gpionum);
    gpioFd = open(pDevice_gpio, O_WRONLY);
    if( gpioFd < 0 )
    {
        MI_ERR("MI_GPIO_SetOutput Unable to open gpioioctl! gpioFd = %d\n", gpioFd);
        return MI_ERR_FAILED;
    }

    if(eLevel)
    {
        if( ioctl(gpioFd, IOCTL_GPIO_OEN_HIGH, gpionum) < 0 )
        {
            MI_ERR("ioctl IOCTL_GPIO_OEN_HIGH failed!\n");
            return MI_ERR_FAILED;
        }
    }
    else
    {
        if( ioctl(gpioFd, IOCTL_GPIO_OEN_LOW, gpionum) < 0 )
        {
            MI_ERR("ioctl IOCTL_GPIO_OEN_LOW failed!\n");
            return MI_ERR_FAILED;
        }
    }

    close(gpioFd);
    return MI_OK;
}

MI_RESULT MI_GPIO_GetLevel(MI_U8 u8GpioNum, MI_GPIO_Level_e *peLevel, MI_BOOL *pbInvert)
{
    MI_ENTER();

    int gpioFd = -1;
    MI_U8 gpionum = u8GpioNum;
    //printf("MI_GPIO_GetLevel gpionum = %d\n", gpionum);
    gpioFd = open(pDevice_gpio, O_WRONLY);
    if( gpioFd < 0 )
    {
        MI_ERR("MI_GPIO_GetLevel Unable to open gpioioctl! gpioFd = %d\n", gpioFd);
        return MI_ERR_FAILED;
    }
    *peLevel = ioctl(gpioFd, IOCTL_GPIO_READ, gpionum);
    //printf("*peLevel = %d\n", *peLevel);
    close(gpioFd);

    return MI_OK;
}

int  MI_GPIO_Interrupt_callback(int eIntNum)
{
    MI_INFO("####in APP MI_GPIO_Interrupt_callback eIntNum %d \n", eIntNum);
    return MI_OK;
}

void* GPIO_Interrupt(void *arg)
{
    MI_U8 u8GpioNum=16;
    MI_GPIO_Level_e peLevel;
    MI_BOOL pbInvert;

    while(1)
    {
        MI_GPIO_GetLevel(PAD_GPIO4, &peLevel, &pbInvert);
        if(peLevel==1)
        {
            MI_GPIO_Interrupt_callback(PAD_GPIO4);
        }
        MI_GPIO_GetLevel(PAD_PM_GPIO2, &peLevel, &pbInvert);
        if(peLevel==1)
        {
            MI_GPIO_Interrupt_callback(PAD_PM_GPIO2);
        }
        usleep(300000);
    }

    return((void*)0);
}

int MI_GPIO_EnableInterrupt_init(void)
{
    int     err;
    static pthread_t ntid;

    err = pthread_create(&ntid, NULL, GPIO_Interrupt, NULL);
    if(err!=0)
    {
        MI_ERR("can't create thread\n");
        return MI_ERR_FAILED;
    }
    return MI_OK;
}

MI_RESULT MI_GPIO_EnableInterrupt(MI_GPIO_EnableIntr_t stGpioIntr)
{
    MI_ENTER();

    int gpioFd = -1;
    MI_U8 gpionum = stGpioIntr.u8GpioNum;
    MI_U8 virq_id = 0;

    //printf("MI_GPIO_EnableInterrupt gpionum = %d\n", gpionum);
    gpioFd = open(pDevice_gpio, O_WRONLY);
    if( gpioFd < 0 )
    {
        MI_ERR("MI_GPIO_SetInput Unable to open gpioioctl! gpioFd = %d\n", gpioFd);
        return MI_ERR_FAILED;
    }

    if(( virq_id = ioctl(gpioFd, IOCTL_GPIO_TO_IRQ, gpionum)) < 0 )
    {
        MI_ERR("ioctl IOCTL_GPIO_TO_IRQ failed!\n");
        return MI_ERR_FAILED;
    }
    MI_INFO("IOCTL_GPIO_TO_IRQ virq_id = %d\n", virq_id);

    close(gpioFd);

    return MI_OK;
}

MI_RESULT MI_GPIO_DisableInterrupt(void)
{
    MI_RESULT eErrCode = MI_ERR_FAILED;

    return eErrCode;
}

MI_RESULT MI_GPIO_SetDebugLevel(MI_DBG_LEVEL_e eDbgLevel)
{
    _gu8GpioDbgLevel = eDbgLevel;
    return MI_OK;
}
