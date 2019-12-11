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

#ifndef _MI_GPIO_SPI_H_
#define _MI_GPIO_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mi_gpio.h"
#include "gpio.h"

//-------------------------------------------------------------------------------------------------
//  Transfer mode defines
//-------------------------------------------------------------------------------------------------
#define  T_GPIO_SPI_WRITE_MODE_4LINE8B    0x01
#define  T_GPIO_SPI_WRITE_MODE_3LINE9B    0x02
#define  T_GPIO_SPI_WRITE_MODE_3LINE16B   0x03
#define  T_GPIO_SPI_WRITE_MODE_3LINE16B2  0x04

//-------------------------------------------------------------------------------------------------
//  Customer Defines
//-------------------------------------------------------------------------------------------------
// Select GPIO as SPI pin
#define  SCLK_NUMBER            PAD_GPIO0
#define  CS_NUMBER              PAD_GPIO1
#define  SDI_NUMBER             PAD_GPIO2
#define  RST_NUMBER             PAD_GPIO3

// Select SPI signal active
#define  SCLK_ACTIVE            1 //1:High active or 0:Low active
#define  CS_ACTIVE              1 //1:High active or 0:Low active
#define  SDI_ACTIVE             1 //1:High active or 0:Low active
#define  RST_ACTIVE             1 //1 is default

// Select Signal hold time per bit
#define  HOLD_TIME_USEC_PER_BIT      5

// Select GPIO-SPI transfer mode
#define  SPI_WRITE_VALUE_MODE   T_GPIO_SPI_WRITE_MODE_3LINE16B2

#if SPI_WRITE_VALUE_MODE == T_GPIO_SPI_WRITE_MODE_4LINE8B
#define  SPI_WriteCom           SPI_WriteCom_4line8bit
#define  SPI_WriteData          SPI_WriteData_4line8bit
#elif SPI_WRITE_VALUE_MODE == T_GPIO_SPI_WRITE_MODE_3LINE9B
#define  SPI_WriteCom           SPI_WriteCom_3line9bit
#define  SPI_WriteData          SPI_WriteData_3line9bit
#elif SPI_WRITE_VALUE_MODE == T_GPIO_SPI_WRITE_MODE_3LINE16B
#define  SPI_WriteCom           SPI_WriteCom_3line16bit
#define  SPI_WriteData          SPI_WriteData_3line16bit
#elif SPI_WRITE_VALUE_MODE == T_GPIO_SPI_WRITE_MODE_3LINE16B2
#define  SPI_WriteCom           SPI_WriteCom_3line16bit2
#define  SPI_WriteData          SPI_WriteData_3line16bit2
#endif


//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    unsigned long bit_hold_time; ///

}MI_GPIO_SPI_InitParam_t;


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void MI_Delayus(unsigned int us);
void MI_Delayms(unsigned int ms);

//------------------------------------------------------------------------------
/// @brief Send commond to device driverIC regsiter as SPI.
/// @param[in] 1Bytes(ex.0xFF) or 2Bytes(ex.0xFF00) regsiter address.
/// @return void.
//------------------------------------------------------------------------------
void SPI_WriteCom(unsigned short value);

//------------------------------------------------------------------------------
/// @brief Send data to device driverIC regsiter as SPI.
/// @param[in] 1Bytes(ex.0x11) regsiter value.
/// @return void.
//------------------------------------------------------------------------------
void SPI_WriteData(unsigned short value);

MI_RESULT MI_SetIO_Out_CS(int statue);
MI_RESULT MI_SetIO_Out_SCLK(int statue);
MI_RESULT MI_SetIO_Out_SDI(int statue);
MI_RESULT MI_SetIO_Out_RESET(int statue);

//-------------------------------------------------------------------------------------------------
//  MI Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief Set GPIO simulate SPI module.
/// @param[in] SPI data struct.
/// @return MI_OK: Send data success.
//------------------------------------------------------------------------------
//MI_RESULT MI_GPIO_SIMULATE_SPI_WR(struct _spi_transfer_st *tr);



#ifdef __cplusplus
}
#endif

#endif///_MI_GPIO_SPI_H_

