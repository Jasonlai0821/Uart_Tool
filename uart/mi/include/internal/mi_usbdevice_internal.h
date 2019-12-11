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
#ifndef _MI_USBDEVICE_INTERNAL_H_
#define _MI_USBDEVICE_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
//  Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------
typedef struct mi_usbdevice_InitParams_s
{
    MI_U8 u8reserved;
} mi_usbdevice_InitParams_t;

typedef enum
{
    E_MI_USBDEVICE_EVENT_INSERT = 0,                                /// < Callback event: USBDEVICE in
    E_MI_USBDEVICE_EVENT_REMOVE,                                    /// < Callback event: USBDEVICE out

    E_MI_USBDEVICE_EVENT_MAX
} mi_usbdevice_Event_e;

typedef enum
{
    E_MI_USBDEVICE_CLASS_PER_INTERFACE          = 0x00,
	E_MI_USBDEVICE_CLASS_AUDIO                  = 0x01,
	E_MI_USBDEVICE_CLASS_COMM                   = 0x02,
	E_MI_USBDEVICE_CLASS_HID                    = 0x03,
	E_MI_USBDEVICE_CLASS_PHYSICAL               = 0x05,
	E_MI_USBDEVICE_CLASS_PRINTER                = 0x07,
	E_MI_USBDEVICE_CLASS_IMAGE                  = 0x06,
	E_MI_USBDEVICE_CLASS_MASS_STORAGE           = 0x08,
	E_MI_USBDEVICE_CLASS_HUB                    = 0x09,
	E_MI_USBDEVICE_CLASS_DATA                   = 0x0A,
	E_MI_USBDEVICE_CLASS_SMART_CARD             = 0x0B,
	E_MI_USBDEVICE_CLASS_CONTENT_SECURITY       = 0x0D,
	E_MI_USBDEVICE_CLASS_VIDEO                  = 0x0E,
	E_MI_USBDEVICE_CLASS_PERSONAL_HEALTHCARE    = 0x0F,
	E_MI_USBDEVICE_CLASS_AUDIO_VIDEO            = 0x10,
	E_MI_USBDEVICE_CLASS_DIAGNOSTIC_DEVICE      = 0xDC,
	E_MI_USBDEVICE_CLASS_WIRELESS               = 0xE0,
	E_MI_USBDEVICE_CLASS_MISCELLANEOUS          = 0xEF,
	E_MI_USBDEVICE_CLASS_APPLICATION            = 0xFE,
	E_MI_USBDEVICE_CLASS_VENDOR_SPEC            = 0xFF
} mi_usbdevice_InterfaceClass_e;

typedef struct mi_usbdevice_Descriptor_s
{
    MI_U16 u16IdVendor;
    MI_U16 u16IdProduct;
    MI_U8  u8InterfaceClass;
} mi_usbdevice_Descriptor_t;

typedef void (*mi_usbdevice_Callback)(mi_usbdevice_Event_e eEvent, mi_usbdevice_Descriptor_t stDescriptor);

typedef struct mi_usbdevice_CallbackParams_s
{
    mi_usbdevice_Callback pfEventCallback;                       /// < A function pointer for event notification
    mi_usbdevice_Descriptor_t stDescriptor;                      /// < A structure to describe usb device.
} mi_usbdevice_CallbackParams_t;
//------------------------------------------------------------------------------
/// @brief Init Usb device handle tasks.
/// @param[in] pstInitParams: Init parameters
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not initialized.
/// @return MI_ERR_INVALID_PARAMETER: Parameters is invalid.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_usbdevice_Init(mi_usbdevice_InitParams_t *pstInitParams);


//------------------------------------------------------------------------------
/// @brief Deinit Usb device handle tasks.
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not initialized.
/// @return MI_ERR_INVALID_PARAMETER: Parameters is invalid.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_usbdevice_DeInit(void);

//------------------------------------------------------------------------------
/// @brief Register event callback function.
/// @param[in] pstCallbackParams: A pointer to structure mi_usbdevice_CallbackParams_t for register callback function.
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not initialized.
/// @return MI_ERR_INVALID_PARAMETER: Parameters is invalid.
/// @return MI_ERR_RESOURCES: No available resource.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_usbdevice_RegisterCallback(mi_usbdevice_CallbackParams_t stCallbackParams);

//------------------------------------------------------------------------------
/// @brief Unregister event callback function.
/// @param[in] pstCallbackParams: A pointer to structure mi_usbdevice_CallbackParams_t for unregister callback function.
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not initialized.
/// @return MI_ERR_INVALID_PARAMETER: Parameters is invalid.
/// @return MI_ERR_RESOURCES: No available resource.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT mi_usbdevice_UnregisterCallback(mi_usbdevice_Descriptor_t stCallbackDescriptor);

#ifdef __cplusplus
}
#endif

#endif///_MI_USBDEVICE_INTERNAL_H_

