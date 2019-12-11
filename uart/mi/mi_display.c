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
/// @file   mi_display.c
/// @brief DISPLAY module
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

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

#include "mi_display.h"

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Defines
//-------------------------------------------------------------------------------------------------

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

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Panel driver initial register settings
//-------------------------------------------------------------------------------------------------
static inline void _Spi_Init_TTL_ST7701S()
{
    MI_SetIO_Out_CS(1);
    MI_SetIO_Out_SCLK(1);
    MI_SetIO_Out_SDI(1);
    //---------------------------------------ST7701 Reset Sequence-----------------------------------------//
    MI_SetIO_Out_RESET(1);
    MI_Delayms(1);
    MI_SetIO_Out_RESET(0);
    MI_Delayms(1);
    MI_SetIO_Out_RESET(1);
    MI_Delayms(120);
    SPI_WriteCom(0x1100);
    MI_Delayms(120);

#if SPI_WRITE_VALUE_MODE == T_GPIO_SPI_WRITE_MODE_3LINE16B2
    SPI_WriteCom(0xFF00); SPI_WriteData(0x77);
    SPI_WriteCom(0xFF01); SPI_WriteData(0x01);
    SPI_WriteCom(0xFF02); SPI_WriteData(0x00);
    SPI_WriteCom(0xFF03); SPI_WriteData(0x00);
    SPI_WriteCom(0xFF04); SPI_WriteData(0x10);
    SPI_WriteCom(0xC000); SPI_WriteData(0xE9);
    SPI_WriteCom(0xC001); SPI_WriteData(0x03);
    SPI_WriteCom(0xC100); SPI_WriteData(0x13);
    SPI_WriteCom(0xC101); SPI_WriteData(0x02);
    SPI_WriteCom(0xC200); SPI_WriteData(0x31);//0x07
    SPI_WriteCom(0xC201); SPI_WriteData(0x08);
    SPI_WriteCom(0xC300); SPI_WriteData(0x0E); // RGB control(VSP:high active, HSP:high active, DP:negetive edge, EP: date high enable
    SPI_WriteCom(0xCC00); SPI_WriteData(0x38);
    //----------------------------------Gamma Cluster Setting----------------------------------------//
    SPI_WriteCom(0xB000); SPI_WriteData(0x00);
    SPI_WriteCom(0xB001); SPI_WriteData(0x18);
    SPI_WriteCom(0xB002); SPI_WriteData(0x20);
    SPI_WriteCom(0xB003); SPI_WriteData(0x0E);
    SPI_WriteCom(0xB004); SPI_WriteData(0x11);
    SPI_WriteCom(0xB005); SPI_WriteData(0x07);
    SPI_WriteCom(0xB006); SPI_WriteData(0x0A);
    SPI_WriteCom(0xB007); SPI_WriteData(0x08);
    SPI_WriteCom(0xB008); SPI_WriteData(0x07);
    SPI_WriteCom(0xB009); SPI_WriteData(0x22);
    SPI_WriteCom(0xB00A); SPI_WriteData(0x03);
    SPI_WriteCom(0xB00B); SPI_WriteData(0x0E);
    SPI_WriteCom(0xB00C); SPI_WriteData(0x0C);
    SPI_WriteCom(0xB00D); SPI_WriteData(0x28);
    SPI_WriteCom(0xB00E); SPI_WriteData(0x31);
    SPI_WriteCom(0xB00F); SPI_WriteData(0x1F);
    SPI_WriteCom(0xB100); SPI_WriteData(0x00);
    SPI_WriteCom(0xB101); SPI_WriteData(0x0F);
    SPI_WriteCom(0xB102); SPI_WriteData(0x9A);
    SPI_WriteCom(0xB103); SPI_WriteData(0x0D);
    SPI_WriteCom(0xB104); SPI_WriteData(0x12);
    SPI_WriteCom(0xB105); SPI_WriteData(0x07);
    SPI_WriteCom(0xB106); SPI_WriteData(0x0D);
    SPI_WriteCom(0xB107); SPI_WriteData(0x09);
    SPI_WriteCom(0xB108); SPI_WriteData(0x09);
    SPI_WriteCom(0xB109); SPI_WriteData(0x23);
    SPI_WriteCom(0xB10A); SPI_WriteData(0x07);
    SPI_WriteCom(0xB10B); SPI_WriteData(0x17);
    SPI_WriteCom(0xB10C); SPI_WriteData(0x16);
    SPI_WriteCom(0xB10D); SPI_WriteData(0xAB);
    SPI_WriteCom(0xB10E); SPI_WriteData(0x31);
    SPI_WriteCom(0xB10F); SPI_WriteData(0x9F);
    //------------------------------------End Gamma Setting-------------------------------------------//
    //---------------------------------End Display Control setting-------------------------------------//
    //--------------------------------------Bank0 Setting End------------------------------------------//
    //----------------------------------------Bank1 Setting------------------------------------------------//
    //----------------------------- Power Control Registers Initial -----------------------------------//
    SPI_WriteCom(0xFF00); SPI_WriteData(0x77);
    SPI_WriteCom(0xFF01); SPI_WriteData(0x01);
    SPI_WriteCom(0xFF02); SPI_WriteData(0x00);
    SPI_WriteCom(0xFF03); SPI_WriteData(0x00);
    SPI_WriteCom(0xFF04); SPI_WriteData(0x11);
    SPI_WriteCom(0xB000); SPI_WriteData(0x7D);
    //----------------------------------------Vcom Setting------------------------------------------------//
    SPI_WriteCom(0xB100); SPI_WriteData(0x2A);// Effect
    //--------------------------------------End Vcom Setting--------------------------------------------//
    SPI_WriteCom(0xB200); SPI_WriteData(0x87);
    SPI_WriteCom(0xB300); SPI_WriteData(0x80);
    SPI_WriteCom(0xB500); SPI_WriteData(0x47);
    SPI_WriteCom(0xB700); SPI_WriteData(0x85);
    SPI_WriteCom(0xB800); SPI_WriteData(0x20);
    SPI_WriteCom(0xB900); SPI_WriteData(0x00);
    SPI_WriteCom(0xB901); SPI_WriteData(0x13);
    SPI_WriteCom(0xC100); SPI_WriteData(0x78);
    SPI_WriteCom(0xC200); SPI_WriteData(0x78);
    SPI_WriteCom(0xD000); SPI_WriteData(0x88);
    //------------------------------End Power Control Registers Initial ----------------------------//
    MI_Delayms (100);
    //------------------------------------------GIP Setting-------------------------------------------------//
    SPI_WriteCom(0xE000); SPI_WriteData(0x00);
    SPI_WriteCom(0xE001); SPI_WriteData(0x00);
    SPI_WriteCom(0xE002); SPI_WriteData(0x02);
    SPI_WriteCom(0xE100); SPI_WriteData(0x0B);
    SPI_WriteCom(0xE101); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE102); SPI_WriteData(0x0D);
    SPI_WriteCom(0xE103); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE104); SPI_WriteData(0x0C);
    SPI_WriteCom(0xE105); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE106); SPI_WriteData(0x0E);
    SPI_WriteCom(0xE107); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE108); SPI_WriteData(0x00);
    SPI_WriteCom(0xE109); SPI_WriteData(0x44);
    SPI_WriteCom(0xE10A); SPI_WriteData(0x44);
    SPI_WriteCom(0xE200); SPI_WriteData(0x33);
    SPI_WriteCom(0xE201); SPI_WriteData(0x33);
    SPI_WriteCom(0xE202); SPI_WriteData(0x44);
    SPI_WriteCom(0xE203); SPI_WriteData(0x44);
    SPI_WriteCom(0xE204); SPI_WriteData(0x64);
    SPI_WriteCom(0xE205); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE206); SPI_WriteData(0x68);
    SPI_WriteCom(0xE207); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE208); SPI_WriteData(0x67);
    SPI_WriteCom(0xE209); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE20A); SPI_WriteData(0x69);
    SPI_WriteCom(0xE20B); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE20C); SPI_WriteData(0x00);
    SPI_WriteCom(0xE300); SPI_WriteData(0x00);
    SPI_WriteCom(0xE301); SPI_WriteData(0x00);
    SPI_WriteCom(0xE302); SPI_WriteData(0x33);
    SPI_WriteCom(0xE303); SPI_WriteData(0x33);
    SPI_WriteCom(0xE400); SPI_WriteData(0x44);
    SPI_WriteCom(0xE401); SPI_WriteData(0x44);
    SPI_WriteCom(0xE500); SPI_WriteData(0x0D);
    SPI_WriteCom(0xE501); SPI_WriteData(0x78);
    SPI_WriteCom(0xE502); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE503); SPI_WriteData(0xA0);
    SPI_WriteCom(0xE504); SPI_WriteData(0x0F);
    SPI_WriteCom(0xE505); SPI_WriteData(0x78);
    SPI_WriteCom(0xE506); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE507); SPI_WriteData(0xA0);
    SPI_WriteCom(0xE508); SPI_WriteData(0x11);
    SPI_WriteCom(0xE509); SPI_WriteData(0x78);
    SPI_WriteCom(0xE50A); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE50B); SPI_WriteData(0xA0);
    SPI_WriteCom(0xE50C); SPI_WriteData(0x13);
    SPI_WriteCom(0xE50D); SPI_WriteData(0x78);
    SPI_WriteCom(0xE50E); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE50F); SPI_WriteData(0xA0);
    SPI_WriteCom(0xE600); SPI_WriteData(0x00);
    SPI_WriteCom(0xE601); SPI_WriteData(0x00);
    SPI_WriteCom(0xE602); SPI_WriteData(0x33);
    SPI_WriteCom(0xE603); SPI_WriteData(0x33);
    SPI_WriteCom(0xE700); SPI_WriteData(0x44);
    SPI_WriteCom(0xE701); SPI_WriteData(0x44);
    SPI_WriteCom(0xE800); SPI_WriteData(0x0E);
    SPI_WriteCom(0xE801); SPI_WriteData(0x78);
    SPI_WriteCom(0xE802); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE803); SPI_WriteData(0xA0);
    SPI_WriteCom(0xE804); SPI_WriteData(0x10);
    SPI_WriteCom(0xE805); SPI_WriteData(0x78);
    SPI_WriteCom(0xE806); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE807); SPI_WriteData(0xA0);
    SPI_WriteCom(0xE808); SPI_WriteData(0x12);
    SPI_WriteCom(0xE809); SPI_WriteData(0x78);
    SPI_WriteCom(0xE80A); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE80B); SPI_WriteData(0xA0);
    SPI_WriteCom(0xE80C); SPI_WriteData(0x14);
    SPI_WriteCom(0xE80D); SPI_WriteData(0x78);
    SPI_WriteCom(0xE80E); SPI_WriteData(0x3C);
    SPI_WriteCom(0xE80F); SPI_WriteData(0xA0);
    SPI_WriteCom(0xEB00); SPI_WriteData(0x02);
    SPI_WriteCom(0xEB01); SPI_WriteData(0x02);
    SPI_WriteCom(0xEB02); SPI_WriteData(0x39);
    SPI_WriteCom(0xEB03); SPI_WriteData(0x39);
    SPI_WriteCom(0xEB04); SPI_WriteData(0xEE);
    SPI_WriteCom(0xEB05); SPI_WriteData(0x44);
    SPI_WriteCom(0xEB06); SPI_WriteData(0x00);
    SPI_WriteCom(0xEC00); SPI_WriteData(0x00);
    SPI_WriteCom(0xEC01); SPI_WriteData(0x00);
    SPI_WriteCom(0xED00); SPI_WriteData(0xFF);
    SPI_WriteCom(0xED01); SPI_WriteData(0xF1);
    SPI_WriteCom(0xED02); SPI_WriteData(0x04);
    SPI_WriteCom(0xED03); SPI_WriteData(0x56);
    SPI_WriteCom(0xED04); SPI_WriteData(0x72);
    SPI_WriteCom(0xED05); SPI_WriteData(0x3F);
    SPI_WriteCom(0xED06); SPI_WriteData(0xFF);
    SPI_WriteCom(0xED07); SPI_WriteData(0xFF);
    SPI_WriteCom(0xED08); SPI_WriteData(0xFF);
    SPI_WriteCom(0xED09); SPI_WriteData(0xFF);
    SPI_WriteCom(0xED0A); SPI_WriteData(0xF3);
    SPI_WriteCom(0xED0B); SPI_WriteData(0x27);
    SPI_WriteCom(0xED0C); SPI_WriteData(0x65);
    SPI_WriteCom(0xED0D); SPI_WriteData(0x40);
    SPI_WriteCom(0xED0E); SPI_WriteData(0x1F);
    SPI_WriteCom(0xED0F); SPI_WriteData(0xFF);
    //------------------------------------------End GIP Setting--------------------------------------------//
    //--------------------------- Power Control Registers Initial End--------------------------------//
    //---------------------------------------Bank1 Setting-------------------------------------------------//
    SPI_WriteCom(0xFF00); SPI_WriteData(0x77);
    SPI_WriteCom(0xFF01); SPI_WriteData(0x01);
    SPI_WriteCom(0xFF02); SPI_WriteData(0x00);
    SPI_WriteCom(0xFF03); SPI_WriteData(0x00);
    SPI_WriteCom(0xFF04); SPI_WriteData(0x00);

    #if PANEL_DRIVER_TEST_PATTERN == 1
    //MI_Delayms(10);
    SPI_WriteCom(0xFF00); SPI_WriteData(0x77);
    SPI_WriteCom(0xFF01); SPI_WriteData(0x01);
    SPI_WriteCom(0xFF02); SPI_WriteData(0x00);
    SPI_WriteCom(0xFF03); SPI_WriteData(0x00);
    SPI_WriteCom(0xFF04); SPI_WriteData(0x12);

    SPI_WriteCom(0xD100); SPI_WriteData(0x81);
    SPI_WriteCom(0xD200); SPI_WriteData(0x08);
    //MI_Delayms(10);
    #endif

    //SPI_WriteCom(0x3600); SPI_WriteData(0x08);// RGB to BGR
    SPI_WriteCom(0x3A00); SPI_WriteData(0x55);// RGB (0x77 24bit/0x55 16bit)
    SPI_WriteCom(0x2900); SPI_WriteData(0x00);
    MI_Delayms(10);
    //---------------------------------------End Setting-------------------------------------------------//
#elif SPI_WRITE_VALUE_MODE == T_GPIO_SPI_WRITE_MODE_3LINE16B
    // not ready yet.
#elif SPI_WRITE_VALUE_MODE == T_GPIO_SPI_WRITE_MODE_3LINE9B
    SPI_WriteCom(0x11);
    MI_Delayms(120);
    SPI_WriteCom(0xFF);
    SPI_WriteData(0x77);
    SPI_WriteData(0x01);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x10);
    SPI_WriteCom(0xC0);
    SPI_WriteData(0xE9);
    SPI_WriteData(0x03);
    SPI_WriteCom(0xC1);
    SPI_WriteData(0x13);
    SPI_WriteData(0x02);
    SPI_WriteCom(0xC2);
    SPI_WriteData(0x07);
    SPI_WriteData(0x08);
    SPI_WriteCom(0xC3);
    SPI_WriteData(0x0E); // RGB control(VSP:high active, HSP:high active, DP:negetive edge, EP: date high enable
    SPI_WriteCom(0xCC);
    SPI_WriteData(0x38);
    SPI_WriteCom(0xB0);
    SPI_WriteData(0x00);
    SPI_WriteData(0x18);
    SPI_WriteData(0x20);
    SPI_WriteData(0x0E);
    SPI_WriteData(0x11);
    SPI_WriteData(0x07);
    SPI_WriteData(0x0A);
    SPI_WriteData(0x08);
    SPI_WriteData(0x07);
    SPI_WriteData(0x22);
    SPI_WriteData(0x03);
    SPI_WriteData(0x0E);
    SPI_WriteData(0x0C);
    SPI_WriteData(0x28);
    SPI_WriteData(0x31);
    SPI_WriteData(0x1F);
    SPI_WriteCom(0xB1);
    SPI_WriteData(0x00);
    SPI_WriteData(0x0F);
    SPI_WriteData(0x9A);
    SPI_WriteData(0x0D);
    SPI_WriteData(0x12);
    SPI_WriteData(0x07);
    SPI_WriteData(0x0D);
    SPI_WriteData(0x09);
    SPI_WriteData(0x09);
    SPI_WriteData(0x23);
    SPI_WriteData(0x07);
    SPI_WriteData(0x17);
    SPI_WriteData(0x16);
    SPI_WriteData(0xAB);
    SPI_WriteData(0x31);
    SPI_WriteData(0x9F);
    SPI_WriteCom(0xFF);
    SPI_WriteData(0x77);
    SPI_WriteData(0x01);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x11);
    SPI_WriteCom(0xB0);
    SPI_WriteData(0x7D);
    SPI_WriteCom(0xB2);
    SPI_WriteData(0x87);
    SPI_WriteCom(0xB3);
    SPI_WriteData(0x80);
    SPI_WriteCom(0xB5);
    SPI_WriteData(0x47);
    SPI_WriteCom(0xB7);
    SPI_WriteData(0x85);
    SPI_WriteCom(0xB8);
    SPI_WriteData(0x20);
    SPI_WriteCom(0xB9);
    SPI_WriteData(0x00);
    SPI_WriteData(0x13);
    SPI_WriteCom(0xC1);
    SPI_WriteData(0x78);
    SPI_WriteCom(0xC2);
    SPI_WriteData(0x78);
    SPI_WriteCom(0xD0);
    SPI_WriteData(0x88);
    MI_Delayms(100);
    SPI_WriteCom(0xE0);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x02);
    SPI_WriteCom(0xE1);
    SPI_WriteData(0x0B);
    SPI_WriteData(0x3C);
    SPI_WriteData(0x0D);
    SPI_WriteData(0x3C);
    SPI_WriteData(0x0C);
    SPI_WriteData(0x3C);
    SPI_WriteData(0x0E);
    SPI_WriteData(0x3C);
    SPI_WriteData(0x00);
    SPI_WriteData(0x44);
    SPI_WriteData(0x44);
    SPI_WriteCom(0xE2);
    SPI_WriteData(0x33);
    SPI_WriteData(0x33);
    SPI_WriteData(0x44);
    SPI_WriteData(0x44);
    SPI_WriteData(0x66);
    SPI_WriteData(0x3C);
    SPI_WriteData(0x68);
    SPI_WriteData(0x3C);
    SPI_WriteData(0x67);
    SPI_WriteData(0x3C);
    SPI_WriteData(0x69);
    SPI_WriteData(0x3C);
    SPI_WriteData(0x00);
    SPI_WriteCom(0xE3);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x33);
    SPI_WriteData(0x33);
    SPI_WriteCom(0xE4);
    SPI_WriteData(0x44);
    SPI_WriteData(0x44);
    SPI_WriteCom(0xE5);
    SPI_WriteData(0x0D);
    SPI_WriteData(0x78);
    SPI_WriteData(0x3C);
    SPI_WriteData(0xA0);
    SPI_WriteData(0x0F);
    SPI_WriteData(0x78);
    SPI_WriteData(0x3C);
    SPI_WriteData(0xA0);
    SPI_WriteData(0x11);
    SPI_WriteData(0x78);
    SPI_WriteData(0x3C);
    SPI_WriteData(0xA0);
    SPI_WriteData(0x13);
    SPI_WriteData(0x78);
    SPI_WriteData(0x3C);
    SPI_WriteData(0xA0);
    SPI_WriteCom(0xE6);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x33);
    SPI_WriteData(0x33);
    SPI_WriteCom(0xE7);
    SPI_WriteData(0x44);
    SPI_WriteData(0x44);
    SPI_WriteCom(0xE8);
    SPI_WriteData(0x0E);
    SPI_WriteData(0x78);
    SPI_WriteData(0x3C);
    SPI_WriteData(0xA0);
    SPI_WriteData(0x10);
    SPI_WriteData(0x78);
    SPI_WriteData(0x3C);
    SPI_WriteData(0xA0);
    SPI_WriteData(0x12);
    SPI_WriteData(0x78);
    SPI_WriteData(0x3C);
    SPI_WriteData(0xA0);
    SPI_WriteData(0x14);
    SPI_WriteData(0x78);
    SPI_WriteData(0x3C);
    SPI_WriteData(0xA0);
    SPI_WriteCom(0xEB);
    SPI_WriteData(0x02);
    SPI_WriteData(0x02);
    SPI_WriteData(0x39);
    SPI_WriteData(0x39);
    SPI_WriteData(0xEE);
    SPI_WriteData(0x44);
    SPI_WriteData(0x00);
    SPI_WriteCom(0xEC);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteCom(0xED);
    SPI_WriteData(0xFF);
    SPI_WriteData(0xF1);
    SPI_WriteData(0x04);
    SPI_WriteData(0x56);
    SPI_WriteData(0x72);
    SPI_WriteData(0x3F);
    SPI_WriteData(0xFF);
    SPI_WriteData(0xFF);
    SPI_WriteData(0xFF);
    SPI_WriteData(0xFF);
    SPI_WriteData(0xF3);
    SPI_WriteData(0x27);
    SPI_WriteData(0x65);
    SPI_WriteData(0x40);
    SPI_WriteData(0x1F);
    SPI_WriteData(0xFF);
    SPI_WriteCom(0xFF);
    SPI_WriteData(0x77);
    SPI_WriteData(0x01);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
#if PANEL_DRIVER_TEST_PATTERN == 1
    SPI_WriteCom(0xFF);
    SPI_WriteData(0x77);
    SPI_WriteData(0x01);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x12);
    SPI_WriteCom(0xD1);
    SPI_WriteData(0x81);
    SPI_WriteCom(0xD2);
    SPI_WriteData(0x08);
#endif
    SPI_WriteCom(0x3A);
    SPI_WriteData(0x55);// RGB (0x77 24bit/0x55 16bit)
    SPI_WriteCom(0x29);
#elif SPI_WRITE_VALUE_MODE == T_GPIO_SPI_WRITE_MODE_4LINE8B
    // not ready yet.
#endif //SPI_WRITE_VALUE_MODE

}
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

void LCDPanelDriverIC_Init()
{
    printf(">> LCDPanelDriverIC_Init.\n");
#if PANEL_TYPE_MODLE == PANEL_TYPE_TTL_ST7701S
    printf("   [_Spi_Init_TTL_ST7701S]\n");
    _Spi_Init_TTL_ST7701S();
#else
    printf("<< No need spi initial.\n");
#endif //PANEL_TYPE_MODLE
}

void SCL_IOCTL_init()
{
#if PANEL_TYPE_MODLE == PANEL_TYPE_MIPI
    char sPnlName[] = "##MIPI <default>";
    E_DrvSclPnlType eDrvSclPnlType = E_DRV_SCL_PNL_TYPE_MIPI;
    E_DrvSclDisplayData eDrvSclDisplayData = E_DRV_SCL_DISPLAY_DATA_FROM_DIPR;
    ST_DrvSclPnlTiming stPnlConfig={0, 68, eDrvSclPnlType, 0, 1, 29, 1053, 29, 1053, 1085,
                                                           0, 5, 35,  635, 35,  635,  667, 60};
    ST_DrvSclDiprHvspConfig testDrvSclDiprHvspConfig={0, 40, 400, 400, 400, {0x2f000000, 0x3f000000}, E_DRV_SCL_ARGB8888, 600, 1024};

#elif PANEL_TYPE_MODLE == PANEL_TYPE_MIPI_RM68200
    char sPnlName[] = "##MIPI <RM68200>";
    E_DrvSclPnlType eDrvSclPnlType = E_DRV_SCL_PNL_TYPE_MIPI_RM68200;
    E_DrvSclDisplayData eDrvSclDisplayData = E_DRV_SCL_DISPLAY_DATA_FROM_DIPR;
    ST_DrvSclPnlTiming stPnlConfig={0, 68, eDrvSclPnlType, 0, 4, 12, 1292, 12, 1292, (1280+8+4+8),
                                                           0, 8, 46,  766, 46, 766,  (720+38+8+38), 60};
    ST_DrvSclDiprHvspConfig testDrvSclDiprHvspConfig={0, 40, 400, 400, 400, {0x2f000000, 0x3f000000}, E_DRV_SCL_ARGB8888, 480, 854};

#elif PANEL_TYPE_MODLE == PANEL_TYPE_LVDS
    char sPnlName[] = "##LVDS <default>";
    E_DrvSclPnlType eDrvSclPnlType = E_DRV_SCL_PNL_TYPE_LVDS;
    E_DrvSclDisplayData eDrvSclDisplayData = E_DRV_SCL_DISPLAY_DATA_FROM_DIPR;
    ST_DrvSclPnlTiming stPnlConfig={0, 68, eDrvSclPnlType, 0, 2, 16, 616, 16, 616, 635,
                                                               0, 1, 35, 1059, 35, 1059, 1344, 60};
    ST_DrvSclDiprHvspConfig testDrvSclDiprHvspConfig={0, 40, 400, 400, 400, {0x2f000000, 0x3f000000}, E_DRV_SCL_ARGB8888, 1024, 600};

#elif PANEL_TYPE_MODLE == PANEL_TYPE_I80
    char sPnlName[] = "##I80 <default>";
    E_DrvSclPnlType eDrvSclPnlType = E_DRV_SCL_PNL_TYPE_I80;
    E_DrvSclDisplayData eDrvSclDisplayData = E_DRV_SCL_DISPLAY_DATA_FROM_DIPR;
    ST_DrvSclPnlTiming stPnlConfig={0, 68, eDrvSclPnlType, 0, 10, 30, 884, 30, 884, 902,
                                                               0, 15, 35, 515, 35, 515, 565, 60};
    ST_DrvSclDiprHvspConfig testDrvSclDiprHvspConfig={0, 40, 320, 480, 320, {0x2f000000, 0x3f000000}, E_DRV_SCL_ARGB8888, 320, 480};

#elif PANEL_TYPE_MODLE == PANEL_TYPE_TYPE_TTL
    char sPnlName[] = "##TTL <default>";
    E_DrvSclPnlType eDrvSclPnlType = E_DRV_SCL_PNL_TYPE_TTL;
    E_DrvSclDisplayData eDrvSclDisplayData = E_DRV_SCL_DISPLAY_DATA_FROM_DIPR;
    ST_DrvSclPnlTiming stPnlConfig={0, 68, eDrvSclPnlType, 0, 3, 32, 512, 32, 512, 525,
                                                               0, 17, 35, 835, 35, 835, 928, 60};
    ST_DrvSclDiprHvspConfig testDrvSclDiprHvspConfig={0, 40, 400, 400, 400, {0x2f000000, 0x3f000000}, E_DRV_SCL_ARGB8888, 800, 480};

#elif PANEL_TYPE_MODLE == PANEL_TYPE_TTL_ST7701S
    char sPnlName[] = "##TTL <IVO4.95 ST7701S>";
    E_DrvSclPnlType eDrvSclPnlType = E_DRV_SCL_PNL_TYPE_TTL_ST7701S;
    E_DrvSclDisplayData eDrvSclDisplayData = E_DRV_SCL_DISPLAY_DATA_FROM_DIPR;
    ST_DrvSclPnlTiming stPnlConfig={0, 68, eDrvSclPnlType, 0, 10, 30, 884, 30, 884, 902,
                                                               0, 15, 35, 515, 35, 515, 565, 60};
    ST_DrvSclDiprHvspConfig testDrvSclDiprHvspConfig={0, 40, 400, 400, 400, {0x2f000000, 0x3f000000}, E_DRV_SCL_RGB565, 480, 854};

#else
    printf("[return] None chosen Panel Type.\n");
    return;
#endif //PANEL_TYPE_MODLE

    printf("[SCL_IOCTL_init] Panel Type[%d] %s\n", eDrvSclPnlType, sPnlName);
    int scl =0;
    scl = open("/dev/scl", O_RDWR);
    if (scl == -1)
    {
        printf("Error: cannot open scl device.\n");
    }
    printf("The scl device was opened successfully.\n");

    if (ioctl(scl, IOCTL_SCL_SET_PNL_INIT, &stPnlConfig) == -1)
    {
        printf("Error reading variable information.\n");
    }

    if (ioctl(scl, IOCTL_SCL_SET_PNL_TIMING, &stPnlConfig) == -1)
    {
        printf("Error reading variable information.\n");
    }

    if (ioctl(scl, IOCTL_SCL_SET_DIPR_HVSP_CONFIG, &testDrvSclDiprHvspConfig) == -1)
    {
        printf("Error reading variable information.\n");
    }

    if (ioctl(scl, IOCTL_SCL_SET_DISPPLAY_DATA_FROM, &eDrvSclDisplayData) == -1)
    {
        printf("Error reading variable information.\n");
    }

    if (scl > 0)
        close(scl);
}

MI_RESULT MI_DISPLAY_INIT()
{
    //1. Scl initial
    SCL_IOCTL_init();

    //2. Driver IC Initial
    LCDPanelDriverIC_Init();

    //3. Show Application image

    return TRUE;
}