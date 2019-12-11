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
/// @file   mi_gpio_spi.c
/// @brief GPIO simulate SPI module
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include "mi_common.h"
#include "mi_sys.h"
#include "mi_gpio_spi.h"

//-------------------------------------------------------------------------------------------------
//  Debug Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local MI Functions
//-------------------------------------------------------------------------------------------------
MI_RESULT MI_SetIO_Out_CS(int statue)
{
    if ((1 == statue) == CS_ACTIVE)
    {
        return MI_GPIO_SetOutput(CS_NUMBER, E_MI_GPIO_LEVEL_ON);
    }
    else
    {
        return MI_GPIO_SetOutput(CS_NUMBER, E_MI_GPIO_LEVEL_OFF);
    }
}

MI_RESULT MI_SetIO_Out_SCLK(int statue)
{
    if ((1 == statue) == SCLK_ACTIVE)
    {
        return MI_GPIO_SetOutput(SCLK_NUMBER, E_MI_GPIO_LEVEL_ON);
    }
    else
    {
        return MI_GPIO_SetOutput(SCLK_NUMBER, E_MI_GPIO_LEVEL_OFF);
    }
}

MI_RESULT MI_SetIO_Out_SDI(int statue)
{
    if ((1 == statue) == SDI_ACTIVE)
    {
        return MI_GPIO_SetOutput(SDI_NUMBER, E_MI_GPIO_LEVEL_ON);
    }
    else
    {
        return MI_GPIO_SetOutput(SDI_NUMBER, E_MI_GPIO_LEVEL_OFF);
    }
}

MI_RESULT MI_SetIO_Out_RESET(int statue)
{
    if ((1 == statue) == RST_ACTIVE)
    {
        return MI_GPIO_SetOutput(RST_NUMBER, E_MI_GPIO_LEVEL_ON);
    }
    else
    {
        return MI_GPIO_SetOutput(RST_NUMBER, E_MI_GPIO_LEVEL_OFF);
    }
}



//-------------------------------------------------------------------------------------------------
//  Local GPIO simulate SPI Functions
//-------------------------------------------------------------------------------------------------
static inline void _SDI_write_8bit(unsigned char value)
{
    unsigned long b;

    MI_SetIO_Out_SCLK(0);
    for(b=0;b<8;b++)
    {
        if(value&0x80) MI_SetIO_Out_SDI(1);
        else         MI_SetIO_Out_SDI(0);
        value<<=1;
        MI_SetIO_Out_SCLK(0);    MI_Delayus(HOLD_TIME_USEC_PER_BIT);

        MI_SetIO_Out_SCLK(1);    MI_Delayus(HOLD_TIME_USEC_PER_BIT);
    }
    MI_SetIO_Out_SCLK(0);
}

static inline void _SDI_write_16bit(unsigned short value)
{
    unsigned long b;

    MI_SetIO_Out_SCLK(0);
    for(b=0;b<16;b++)
    {
        if(value&0x80) MI_SetIO_Out_SDI(1);
        else         MI_SetIO_Out_SDI(0);
        value<<=1;
        MI_SetIO_Out_SCLK(0);    MI_Delayus(HOLD_TIME_USEC_PER_BIT);

        MI_SetIO_Out_SCLK(1);    MI_Delayus(HOLD_TIME_USEC_PER_BIT);
    }
    MI_SetIO_Out_SCLK(0);
}

// E_GPIO_SPI_WRITE_MODE_3LINE16B_1
static inline void _WriteCmd_16bit(unsigned short value)
{
    unsigned short addr_high, addr_low;
    unsigned char head_cmd_h = 0x20, head_cmd_l = 0x00;
    unsigned char cnt, state;

    addr_high = (head_cmd_h << 8) | ((value >> 8) & 0x00FF);
    addr_low = (head_cmd_l << 8) | (value & 0x00FF);

    MI_SetIO_Out_SDI(0);
    MI_SetIO_Out_SCLK(0);
    MI_Delayus(HOLD_TIME_USEC_PER_BIT);
    MI_SetIO_Out_CS(0);
    MI_Delayus(HOLD_TIME_USEC_PER_BIT);

    while(1) {
        //Addr high
        for (cnt = 16; cnt > 0; cnt--) {
            state = (addr_high >> (cnt -1)) & 0x0001;
            if (state == 0x01)
                MI_SetIO_Out_SDI(1);
            else
                MI_SetIO_Out_SDI(0);
             MI_Delayus(HOLD_TIME_USEC_PER_BIT<<1);
            MI_SetIO_Out_SCLK(1);
             MI_Delayus(HOLD_TIME_USEC_PER_BIT);
            MI_SetIO_Out_SCLK(0);
             MI_Delayus(HOLD_TIME_USEC_PER_BIT);
            MI_SetIO_Out_SDI(0);
             MI_Delayus(HOLD_TIME_USEC_PER_BIT);
        }
        MI_SetIO_Out_CS(1);
        MI_Delayus(HOLD_TIME_USEC_PER_BIT<<1);
        MI_SetIO_Out_CS(0);
        MI_Delayus(HOLD_TIME_USEC_PER_BIT);
        //Addr low
        for (cnt = 16; cnt > 0; cnt--) {
            state = (addr_low >> (cnt -1)) & 0x0001;
            if (state == 0x01)
                MI_SetIO_Out_SDI(1);
            else
                MI_SetIO_Out_SDI(0);
             MI_Delayus(HOLD_TIME_USEC_PER_BIT<<1);
            MI_SetIO_Out_SCLK(1);
             MI_Delayus(HOLD_TIME_USEC_PER_BIT);
            MI_SetIO_Out_SCLK(0);
             MI_Delayus(HOLD_TIME_USEC_PER_BIT);
            MI_SetIO_Out_SDI(0);
             MI_Delayus(HOLD_TIME_USEC_PER_BIT);
        }
        break;
    }
    MI_SetIO_Out_CS(1);
    MI_SetIO_Out_SDI(1);
    MI_Delayus(HOLD_TIME_USEC_PER_BIT);
}

// E_GPIO_SPI_WRITE_MODE_3LINE16B2
static inline void _WriteDat_16bit(unsigned short value)
{
    unsigned short send_data;
    unsigned char head_data = 0x40;
    unsigned char cnt, state;

    MI_SetIO_Out_SDI(0);
    MI_SetIO_Out_SCLK(0);
    MI_Delayus(HOLD_TIME_USEC_PER_BIT);
    MI_SetIO_Out_CS(0);
    MI_Delayus(HOLD_TIME_USEC_PER_BIT);

    while(1) {
        send_data = (head_data << 8) | (value & 0xFF);

        for (cnt = 16; cnt > 0; cnt--) {
            state = (send_data >> (cnt -1)) & 0x0001;
            if (state == 0x01)
                MI_SetIO_Out_SDI(1);
            else
                MI_SetIO_Out_SDI(0);
             MI_Delayus(HOLD_TIME_USEC_PER_BIT<<1);
            MI_SetIO_Out_SCLK(1);
             MI_Delayus(HOLD_TIME_USEC_PER_BIT);
            MI_SetIO_Out_SCLK(0);
             MI_Delayus(HOLD_TIME_USEC_PER_BIT);
            MI_SetIO_Out_SDI(0);
             MI_Delayus(HOLD_TIME_USEC_PER_BIT);
        }
        break;
    }
    MI_SetIO_Out_CS(1);
    MI_SetIO_Out_SDI(1);
    MI_Delayus(HOLD_TIME_USEC_PER_BIT);

}

// E_GPIO_SPI_WRITE_MODE_4LINE8B
static inline void _WriteCmd_8bit(unsigned short value)
{
    MI_SetIO_Out_SCLK(0);
    MI_SetIO_Out_SDI(0);
    MI_SetIO_Out_CS(0);

    //MI_SetIO_Out_DC(0);
    _SDI_write_8bit(value);
    MI_SetIO_Out_CS(1);
}

static inline void _WriteDat_8bit(unsigned short value)
{
    MI_SetIO_Out_SCLK(0);
    MI_SetIO_Out_SDI(0);
    MI_SetIO_Out_CS(0);

    //MI_SetIO_Out_DC(1);
    _SDI_write_8bit(value);
    MI_SetIO_Out_CS(1);
}


// E_GPIO_SPI_WRITE_MODE_3LINE9B
static inline void _WriteCmd_9bit(unsigned short value)
{
    MI_SetIO_Out_SCLK(0);
    MI_SetIO_Out_SDI(0);
    MI_SetIO_Out_CS(0);
    MI_Delayus(HOLD_TIME_USEC_PER_BIT);
    MI_SetIO_Out_SCLK(1);
    MI_Delayus(HOLD_TIME_USEC_PER_BIT);

    _SDI_write_8bit(value);
    MI_SetIO_Out_CS(1);
}

static inline void _WriteDat_9bit(unsigned short value)
{
    MI_SetIO_Out_SCLK(0);
    MI_SetIO_Out_SDI(1);
    MI_SetIO_Out_CS(0);
    MI_Delayus(HOLD_TIME_USEC_PER_BIT);
    MI_SetIO_Out_SCLK(1);
    MI_Delayus(HOLD_TIME_USEC_PER_BIT);

    _SDI_write_8bit(value);
    MI_SetIO_Out_CS(1);
}

// E_GPIO_SPI_WRITE_MODE_3LINE16B
static inline void _WriteValue_16bit(unsigned short value)
{
    MI_SetIO_Out_SCLK(0);
    MI_SetIO_Out_CS(0);
    MI_Delayus(HOLD_TIME_USEC_PER_BIT);

    _SDI_write_16bit(value);
    MI_SetIO_Out_CS(1);
    MI_SetIO_Out_SCLK(1);
}




// used for SPI_WriteCom & SPI_WriteData
void SPI_WriteCom_3line16bit2(unsigned short value)
{
    _WriteCmd_16bit(value);
}

void SPI_WriteData_3line16bit2(unsigned short value)
{
    _WriteDat_16bit(value);
}

void SPI_WriteCom_3line16bit(unsigned short value)
{
    _WriteValue_16bit(value);
}

void SPI_WriteData_3line16bit(unsigned short value)
{
    _WriteValue_16bit(value);
}

void SPI_WriteCom_3line9bit(unsigned short value)
{
    _WriteCmd_9bit(value);
}

void SPI_WriteData_3line9bit(unsigned short value)
{
    _WriteDat_9bit(value);
}

void SPI_WriteCom_4line8bit(unsigned short value)
{
    _WriteCmd_8bit(value);
}

void SPI_WriteData_4line8bit(unsigned short value)
{
    _WriteDat_8bit(value);
}

