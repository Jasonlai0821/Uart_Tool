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

#ifndef _MI_SAR_H_
#define _MI_SAR_H_

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
//  Defines
//------------------------------------------------------------------------------


// channel	| measure
//----------+----------------------
// 0		| PAD_SAR_GPIO0
// 1		| PAD_SAR_GPIO1
// 2		| PAD_SAR_GPIO2
// 3		| PAD_SAR_GPIO3
// 4		| PAD_SAR_GPIO4
// 5		| PAD_SAR_GPIO5
// 6~8		| GND
// 9		| Internal temperature sensor
// 10		| External temperature sensor
// 11		| N/A
// 12		| VBAT
// 13~14	| N/A
// 15		| power detected sensor
#define PAD_SAR_GPIO0_ID 0
#define PAD_SAR_GPIO1_ID 1
#define PAD_SAR_GPIO2_ID 2
#define PAD_SAR_GPIO3_ID 3
#define PAD_SAR_GPIO4_ID 4
#define PAD_SAR_GPIO5_ID 5


typedef void (*MI_SAR_Callback)(MI_U8 u8KeyCode, MI_U8 u8RepeatFlag);

//------------------------------------------------------------------------------
//  Structures
//------------------------------------------------------------------------------

typedef struct MI_SAR_RegCfg_s
{
    MI_U8 u8SarChID;
    MI_U8 u8UpBnd;
    MI_U8 u8LoBnd;
    MI_U8 u8KeyLevelNum;
    MI_U32 au32KeyThreshold[8];
    MI_U8 au8KeyCode[8];
} MI_SAR_RegCfg_t;

typedef enum
{
    E_KEYPAD_KEY_PROPERTY_INIT,
    E_KEYPAD_KEY_PROPERTY_1st,
    E_KEYPAD_KEY_PROPERTY_2st,
    E_KEYPAD_KEY_PROPERTY_FOLLOWING
} KeypadProperty;

//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief Initialize the Sar module
/// @param[in] pstInitParams: Parameter to init Sar module
/// @return MI_OK: Process success.
/// @return MI_HAS_INITED: UART module had inited.
/// @return MI_ERR_MEMORY_ALLOCATE: Allocate memory fail
/// @return MI_ERR_RESOURCES: No available resource.
/// @note
//------------------------------------------------------------------------------
MI_RESULT MI_SAR_Init(void);

//------------------------------------------------------------------------------
/// @brief DeInit Sar.
/// @param void
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED or other value : Failure
//------------------------------------------------------------------------------
MI_RESULT MI_SAR_DeInit(void);

//------------------------------------------------------------------------------
/// @brief Set SAR Keypad channel information function.
/// @param pstSarRegCfg: sar channel info pointer
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED or other value : Failure
//------------------------------------------------------------------------------
MI_RESULT MI_SAR_Config(MI_SAR_RegCfg_t* pstSarRegCfg);


//------------------------------------------------------------------------------
/// @brief Set SAR callback function when receive SAR key. Support only one callback. If call it twice,
/// the first callback does not be called. (This sends command to PM51 using mailbox.)
/// Note: The callback runs at interrupt mode.
/// @param [in] pCallback: Set the callback function when generate SAR interrupt.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED or other value : Failure
//------------------------------------------------------------------------------
MI_RESULT MI_SAR_SetCallback(MI_SAR_Callback pCallback);


MI_RESULT MI_SAR_GetValue(MI_U8 u8SARNum, MI_U32 *peLevel);
MI_RESULT MI_SAR_VoltageVBat_GetValue( MI_U32 pgain,MI_U32 *volLevel);
int MI_SAR_EnableInterrupt_init(void);


//------------------------------------------------------------------------------
/// @brief Set SAR debug function level.
/// @param[in] eDbgLevel: MI_DBG_NONE /MI_DBG_ERR/MI_DBG_WRN/MI_DBG_INFO/MI_DBG_ALL
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED : Failure
//------------------------------------------------------------------------------
MI_RESULT MI_SAR_SetDebugLevel(MI_DBG_LEVEL_e eDbgLevel);

#ifdef __cplusplus
}
#endif

#endif///_MI_SAR_H_
