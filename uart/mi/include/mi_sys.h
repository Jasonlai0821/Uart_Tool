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

#ifndef _MI_SYS_H_
#define _MI_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
//  Defines
//-------------------------------------------------------------------------------------------------
// version definition of struct MI_SYS_Mmap_t:
// 1.0: 0x1000, 1.1: 0x1001, 1.2: 0x1002, 2.0: 0x2000, 2.1: 0x2001,....
#define MI_SYS_MMAP_VERSION_1_0                 0x1000
#define MI_SYS_MMAP_VERSION_1_1                 0x1001
#define MI_SYS_MMAP_VERSION_1_2                 0x1002
#define MI_SYS_MMAP_VERSION_1_3                 0x1003
#define MI_SYS_MMAP_VERSION_1_4                 0x1004
#define MI_SYS_MMAP_VERSION_1_5                 0x1005

#define MI_SYS_MMAP_PREFIX                      0x4D490000  // 'MI'
#define MI_SYS_MMAP_VERSION                     MI_SYS_MMAP_VERSION_1_1   // current version
#define MI_SYS_MMAP_PREFIX_MASK                 0xFFFF0000
#define MI_SYS_MMAP_VERSION_MASK                0x0000FFFF

#define MI_SYS_MMAP_PREFIX_VERSION              (MI_SYS_MMAP_PREFIX | MI_SYS_MMAP_VERSION)

// version definition of struct MI_SYS_Board_t:
// 1.0: 0x1000, 1.1: 0x1001, 1.2: 0x1002, 2.0: 0x2000, 2.1: 0x2001,....
#define MI_SYS_BOARD_INFO_VERSION_1_0           0x1000
#define MI_SYS_BOARD_INFO_VERSION_1_1           0x1001
#define MI_SYS_BOARD_INFO_VERSION_1_2           0x1002
#define MI_SYS_BOARD_INFO_VERSION_1_3           0x1003
#define MI_SYS_BOARD_INFO_VERSION_1_4           0x1004
#define MI_SYS_BOARD_INFO_VERSION_1_5           0x1005
#define MI_SYS_BOARD_INFO_VERSION_1_6           0x1006
#define MI_SYS_BOARD_INFO_VERSION_1_7           0x1007

#define MI_SYS_BOARD_INFO_PREFIX                0x4D490000  // 'MI'
#define MI_SYS_BOARD_INFO_VERSION               MI_SYS_BOARD_INFO_VERSION_1_7   // current version
#define MI_SYS_BOARD_INFO_PREFIX_MASK           0xFFFF0000
#define MI_SYS_BOARD_INFO_VERSION_MASK          0x0000FFFF

#define MI_SYS_BOARD_INFO_PREFIX_VERSION        (MI_SYS_BOARD_INFO_PREFIX | MI_SYS_BOARD_INFO_VERSION)


#ifndef MEMALIGN
#define MEMALIGN(align, address)               ((((address)+(align)-1)/(align))*(align))
#endif
//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_MI_SYS_COBUFLAYER_L0 = 0,
    E_MI_SYS_COBUFLAYER_L1,
    E_MI_SYS_COBUFLAYER_L2,
    E_MI_SYS_COBUFLAYER_L3,
    E_MI_SYS_COBUFLAYER_L4,
    E_MI_SYS_COBUFLAYER_L5,
    E_MI_SYS_COBUFLAYER_L6,
    E_MI_SYS_COBUFLAYER_L7,
    E_MI_SYS_COBUFLAYER_L8,
    E_MI_SYS_COBUFLAYER_L9,
    E_MI_SYS_COBUFLAYER_L10,
    E_MI_SYS_COBUFLAYER_L11,
    E_MI_SYS_COBUFLAYER_MAX

} MI_SYS_CoBufLayer_e;

typedef enum
{
    E_MI_SYS_MIU_TYPE_0 = 0,                    ///< MIU0
    E_MI_SYS_MIU_TYPE_1,                        ///< MIU1

    E_MI_SYS_MIU_TYPE_MAX,
} MI_SYS_MiuType_e;

typedef enum
{
    E_MI_SYS_CUSTOMER_1 = 0,                    ///Customer 1
    E_MI_SYS_CUSTOMER_2,                        ///Customer 2, parameter type is a pointer to MI_U32
    E_MI_SYS_CUSTOMER_MAX,
} MI_SYS_Customer_e;

typedef enum
{
    E_MI_SYS_MMAP_ID_INVALID = -1,      ///< Invalid MMAP ID

    E_MI_SYS_MMAP_ID_MIN = 0,             ///< Min of MMAP for MI SDK

    // MIU0 Size
    E_MI_SYS_MMAP_ID_MIU0 = E_MI_SYS_MMAP_ID_MIN,                  ///< MEM0_SIZE
    // MIU1 Size
    E_MI_SYS_MMAP_ID_MIU1 = 1,                  ///< MEM1_SIZE
    // MIU0 0/1 Part
    E_MI_SYS_MMAP_ID_MIU0_0 = 2,                ///< MIU0_LOW, MMEM0_SIZE_0(deprecated)
    E_MI_SYS_MMAP_ID_MIU0_1 = 3,                ///< MIU0_HIGH, MEM0_SIZE_1(deprecated)
    // MIU1 0/1 Part
    E_MI_SYS_MMAP_ID_MIU1_0 = 4,                ///< MIU1_LOW, MIU1_BUFFER_START_0, MEM1_SIZE_0(deprecated)
    E_MI_SYS_MMAP_ID_MIU1_1 = 5,                ///< MIU1_HIGH, MIU1_BUFFER_START_1, MEM1_SIZE_1(deprecated)
    // LX1
    E_MI_SYS_MMAP_ID_LX1 = 6,                   ///< MEM_LX1_START, MEM_LX1_SIZE
    // LX2
    E_MI_SYS_MMAP_ID_LX2 = 7,                   ///< MEM_LX2_START, MEM_LX2_SIZE
    // Memory Pool for system
    E_MI_SYS_MMAP_ID_CACHED_POOL = 8,           ///< CACHED_POOL_BUF_ADDR, CACHED_POOL_BUF_LEN
    E_MI_SYS_MMAP_ID_NON_CACHED_POOL = 9,       ///< NON_CACHED_POOL_BUF_ADDR, NON_CACHED_POOL_BUF_LEN
    // MAD: AUDIO
    E_MI_SYS_MMAP_ID_MAD_SE_BUF = 10,           ///< MAD_SE_BUFFER_ADR, MAD_SE_BUFFER_LEN
    E_MI_SYS_MMAP_ID_MAD_DEC_BUF = 11,          ///< MAD_DEC_BUFFER_ADR, MAD_DEC_BUFFER_LEN
    // VE
    E_MI_SYS_MMAP_ID_VE_FRAME_BUF = 12,         ///< VE_FRAMEBUFFER_ADR, VE_FRAMEBUFFER_LEN
    // VIDEO
    E_MI_SYS_MMAP_ID_VDEC_AEON = 13,            ///< VDEC_AEON_ADR, VDEC_AEON_LEN, VDEC_AEON_MEMORY_TYPE
    E_MI_SYS_MMAP_ID_VDEC_FRAME_BUF = 14,       ///< VDEC_FRAMEBUFFER_ADR, VDEC_FRAMEBUFFER_LEN, VDEC_FRAMEBUFFER_MEMORY_TYPE
    E_MI_SYS_MMAP_ID_VDEC_BITSREAM = 15,        ///< VDEC_BITSTREAM_ADR, VDEC_BITSTREAM_LEN, VDEC_BITSTREAM_MEMORY_TYPE
    // MHEG5
    E_MI_SYS_MMAP_ID_MHEG5_JPG_BUF = 16,        ///< MHEG5_JPEG_BUFF_ADR, MHEG5_JPEG_BUFF_LEN
    E_MI_SYS_MMAP_ID_MHEG5_BUF = 17,            ///< MHEG5_BUFF_ADR, MHEG5_BUFF_LEN
    // SCALER
    E_MI_SYS_MMAP_ID_XC_MAIN_FB = 18,           ///< MAIN_FB_START_ADDR, SCALER_DNR_BUF_LEN
    E_MI_SYS_MMAP_ID_XC_SUB_FB=19,              ///< SUB_FB_START_ADDR, SCALER_DNR_BUF_LEN
    // TSP
    E_MI_SYS_MMAP_ID_TSP_SECT_BUF = 20,         ///< SECTION_BUF_ADDR, SECTION_BUF_LEN
    E_MI_SYS_MMAP_ID_TSP_FW_BUF = 21,           ///< TSP_FW_BUF, TSP_FW_LEN
    E_MI_SYS_MMAP_ID_TSP_FQ_BUF = 22,           ///< TSP_FQ_BUFFER_ADR, TSP_FQ_BUFFER_LEN
    E_MI_SYS_MMAP_ID_TSP_VQ_BUF = 23,           ///< TSP_VQ_BUF, TSP_VQ_LEN
    // PM51
    E_MI_SYS_MMAP_ID_PM51_MIU_ENTER_SR = 24,    ///< PM51_MIU_ENTER_SR_ADDR, PM51_MIU_ENTER_SR_LEN, PM51_MIU_TYPE
    E_MI_SYS_MMAP_ID_PM51_MIU_EXIT_SR = 25,     ///< PM51_MIU_EXIT_SR_ADDR, PM51_MIU_EXIT_SR_LEN, PM51_MIU_TYPE
    E_MI_SYS_MMAP_ID_PM51_HWIP_OPEN = 26,       ///< PM51_HWIP_OPEN_ADDR, PM51_HWIP_OPEN_LEN, PM51_MIU_TYPE
    E_MI_SYS_MMAP_ID_PM51_HWIP_CLOSE = 27,      ///< PM51_HWIP_CLOSE_ADDR, PM51_HWIP_CLOSE_LEN, PM51_MIU_TYPE
    E_MI_SYS_MMAP_ID_PM51_LOADER = 28,          ///< PM51_LOADER_ADDR, PM51_LOADER_LEN, PM51_MIU_TYPE
    E_MI_SYS_MMAP_ID_PM51_LITE = 29,            ///< PM51_LITE_ADDR, PM51_LITE_LEN, PM51_MIU_TYPE
    E_MI_SYS_MMAP_ID_PM51_MEM = 30,             ///< PM51_MEM_ADR, PM51_MEM_LEN, PM51_MIU_TYPE
    E_MI_SYS_MMAP_ID_PM51_VAR_MEM = 31,         ///< PM51_VAR_MEM_ADR, PM51_VAR_MEM_LEN, PM51_MIU_TYPE
    // GOP
    E_MI_SYS_MMAP_ID_GOP_REGDMA_BASE = 32,      ///< GOP_REGDMABASE_ADR, GOP_REGDMABASE_LEN
    E_MI_SYS_MMAP_ID_GOP_GWIN_RB = 33,          ///< GOP_GWIN_RB_ADR, GOP_GWIN_RB_LEN
    // JPEG decode
    E_MI_SYS_MMAP_ID_JPD_READ_BUF = 34,         ///< JPD_READBUFF_ADR, JPD_READBUFF_LEN
    E_MI_SYS_MMAP_ID_JPD_INTER_BUF = 35,        ///< JPD_INTERBUFF_ADR, JPD_INTERBUFF_LEN
    E_MI_SYS_MMAP_ID_JPD_OUT = 36,              ///< JPD_OUT_ADR, JPD_OUT_LEN
    E_MI_SYS_MMAP_ID_PHOTO_SHARE_MEM = 37,      ///< PHOTO_SHAREMEM_ADR, PHOTO_SHAREMEM_LEN
    // MM
    E_MI_SYS_MMAP_ID_MM_COPROCESSOR = 38,       ///< MM_COPROCESSOR_ADDR, MM_COPROCESSOR_LEN, MM_COPROCESSOR_TYPE
    E_MI_SYS_MMAP_ID_MM_VDPLAYER_DATA = 39,     ///< VDPLAYER_DATA_ADR, VDPLAYER_DATA_LEN, VDPLAYER_MEMORY_TYPE
    E_MI_SYS_MMAP_ID_MM_VDPLAYER_SUB_BS = 40,   ///< VDPLAYER_SUB_BS_ADR, VDPLAYER_SUB_BS_LEN, VDPLAYER_MEMORY_TYPE
    E_MI_SYS_MMAP_ID_MM_VDPLAYER_BS = 41,       ///< VDPLAYER_BS_ADR, VDPLAYER_BS_LEN, VDPLAYER_MEMORY_TYPE
    E_MI_SYS_MMAP_ID_MM_VDPLAYER_BS_EXT = 42,   ///< VDPLAYER_BS_EXT_ADR, VDPLAYER_BS_EXT_LEN, VDPLAYER_MEMORY_TYPE
    E_MI_SYS_MMAP_ID_MM_VDPLAYER_SUB = 43,      ///< VDPLAYER_SUB_ADR, VDPLAYER_SUB_LEN, VDPLAYER_MEMORY_TYPE
    E_MI_SYS_MMAP_ID_MM_VDPLAYER_DMX_SECT = 44, ///< VDPLAYER_DMX_SEC_ADR, VDPLAYER_DMX_SEC_LEN, VDPLAYER_MEMORY_TYPE
    // INJECTION
    E_MI_SYS_MMAP_ID_INJECT_BUF = 45,           ///< INJECTION_BUFFER_ADR, INJECTION_BUFFER_LEN
    // MenuLoad
    E_MI_SYS_MMAP_ID_MENULOAD_BUF = 46,         ///< MENULOAD_BUFFER_ADR, MENULOAD_BUFFER_LEN, MENULOAD_BUFFER_MEMORY_TYPE
    // Seamless zapping
    E_MI_SYS_MMAP_ID_SEAMLESS_ZAPPING_BUF = 47, ///< SEAMLESS_ZAPPING_BUF_ADR, SEAMLESS_ZAPPING_BUF_LEN, SEAMLESS_ZAPPING_BUF_MEMORY_TYPE
    // Still image zapping
    E_MI_SYS_MMAP_ID_STILL_IMAGE_ZAPPING_BUF = 48,  ///< STILL_IMG_ZAPPING_BUF_ADR, STILL_IMG_ZAPPING_BUF_LEN
    // PVR
    E_MI_SYS_MMAP_ID_PVR_DOWNLOAD           = 49,
    E_MI_SYS_MMAP_ID_PVR_UPLOAD             = 50,
    E_MI_SYS_MMAP_ID_PVR_AUDIO_BUFFER       = 51,
    E_MI_SYS_MMAP_ID_PVR_MN_VIDEO_BUFFER    = 52,
    E_MI_SYS_MMAP_ID_PVR_MN_AUDIO_BUFFER    = 53,
    E_MI_SYS_MMAP_ID_PVR_TSR_VIDEO_BUFFER   = 54,
    E_MI_SYS_MMAP_ID_PVR_TSR_AUDIO_BUFFER   = 55,
    // Audio
    E_MI_SYS_MMAP_ID_MAD_ADV_BUF = 56,        ///< MAD_ADV_BUFFER_ADR, MAD_ADV_BUFFER_LEN
    E_MI_SYS_MMAP_ID_MAD_COMM_BUF = 57,       ///< MAD_COMM_BUFFER_ADR, MAD_COMM_BUFFER_LEN

    //MM photo display
    E_MI_SYS_MMAP_ID_MM_PHOTO_DISPLAY = 58,

    //PM powerSaving bin file buffer
    E_MI_SYS_MMAP_ID_PM_POWER_SAVING_BIN_BUF = 59,

    /// System
    E_MI_SYS_MMAP_ID_MAC = 60,

    //PM STR fast standby MBOOT buffer
    E_MI_SYS_MMAP_ID_PM_STR_POWER_UP_BUF = 61,

    /// System
    E_MI_SYS_MMAP_ID_LX3 = 62,

    ///VIDEO1
    E_MI_SYS_MMAP_ID_VDEC1_FRAME_BUF = 63,
    E_MI_SYS_MMAP_ID_VDEC1_BITSREAM = 64,

    ///VIDEO MVC
    E_MI_SYS_MMAP_ID_VDEC_MVC_FRAME_BUF = 65,
    E_MI_SYS_MMAP_ID_VDEC_MVC_BITSTREAM = 66,

    ///XC1
    E_MI_SYS_MMAP_ID_XC1_MAIN_FB = 67,
    E_MI_SYS_MMAP_ID_XC1_SUB_FB  = 68,
    E_MI_SYS_MMAP_ID_MENULOAD1_BUF = 69,
    E_MI_SYS_MMAP_ID_SEAMLESS_ZAPPING1_BUF = 70,

    /// MM
    E_MI_SYS_MMAP_ID_MM_VDPLAYER_SHARE_MEM = 71,
    E_MI_SYS_MMAP_ID_MM_VDPLAYER_PHY_COPY = 72,

    // HBBTV DSMCC buffer
    E_MI_SYS_MMAP_ID_HBBTV_DSMCC_BUF = 73,

    E_MI_SYS_MMAP_ID_MAX,                       ///< Max number of MMAP used for MI SDK

    // For customer defined
    E_MI_SYS_MMAP_ID_CUSTOM_MIN = 0x10000,    ///< Min of MMAP used for Customer

    E_MI_SYS_MMAP_ID_CUSTOM_1 = E_MI_SYS_MMAP_ID_CUSTOM_MIN,
    E_MI_SYS_MMAP_ID_CUSTOM_2,
    E_MI_SYS_MMAP_ID_CUSTOM_3,
    E_MI_SYS_MMAP_ID_CUSTOM_4,
    E_MI_SYS_MMAP_ID_CUSTOM_5,
    E_MI_SYS_MMAP_ID_CUSTOM_6,
    E_MI_SYS_MMAP_ID_CUSTOM_7,
    E_MI_SYS_MMAP_ID_CUSTOM_8,
    E_MI_SYS_MMAP_ID_CUSTOM_9,
    E_MI_SYS_MMAP_ID_CUSTOM_10,

    E_MI_SYS_MMAP_ID_CUSTOM_MAX,        ///< Max number of MMAP used for Customer.
} MI_SYS_MmapId_e;


typedef enum
{
    E_MI_SYS_BOARD_INFO_ID_INVALID = -1,      ///< Invalid Board Info ID

    E_MI_SYS_BOARD_INFO_ID_MIN = 0,             ///< Min number of Board Info ID
    E_MI_SYS_BOARD_INFO_ID_AUDIO_PATH_MAIN_SPEAKER = E_MI_SYS_BOARD_INFO_ID_MIN,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_PATH_HP,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_PATH_LINEOUT,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_PATH_SIFOUT,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_PATH_SCART1,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_PATH_SCART2,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_PATH_I2S,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_PATH_SPDIF,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_PATH_HDMI,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_OUTPUT_MAIN_SPEAKER,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_OUTPUT_HP,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_OUTPUT_LINEOUT,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_OUTPUT_SIFOUT,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_OUTPUT_SCART1,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_OUTPUT_SCART2,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_DTV,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_DTV2,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_ATV,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_PC,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_YPBPR,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_YPBPR2,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_AV,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_AV2,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_AV3,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_SV,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_SV2,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_SCART,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_SCART2,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_HDMI,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_HDMI2,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_HDMI3,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_DVI,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_DVI2,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_DVI3,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_SOURCE_KTV,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_GPIO_MUTE_PIN,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_GPIO_MUTE_LEVEL,
    E_MI_SYS_BOARD_INFO_ID_AUDIO_MAX,
    E_MI_SYS_BOARD_INFO_ID_DISP_MIN = 0x100,
    E_MI_SYS_BOARD_INFO_ID_DISP_HDMITX_ANALOG_TUNING_SD = E_MI_SYS_BOARD_INFO_ID_DISP_MIN,
    E_MI_SYS_BOARD_INFO_ID_DISP_HDMITX_ANALOG_TUNING_HD,
    E_MI_SYS_BOARD_INFO_ID_DISP_HDMITX_ANALOG_TUNING_DEEP_HD,
    E_MI_SYS_BOARD_INFO_ID_DISP_HDMITX_HPD_GPIO_NUM,
    E_MI_SYS_BOARD_INFO_ID_DISP_SCART_PIN8_GPIO_NUM,
    E_MI_SYS_BOARD_INFO_ID_DISP_MAX,
    E_MI_SYS_BOARD_INFO_ID_EXTIN_MIN = 0x200,
    E_MI_SYS_BOARD_INFO_ID_EXTIN_HDMIRX_BY_PASS_GPIO_NUM = E_MI_SYS_BOARD_INFO_ID_EXTIN_MIN,
    E_MI_SYS_BOARD_INFO_ID_EXTIN_MAX,
    E_MI_SYS_BOARD_INFO_ID_MAX,        ///< Max number of Board Info ID.
} MI_SYS_BoardInfoId_e;

typedef enum
{
    E_MI_SYS_BOARD_INFO_TYPE_INVALID = -1,      ///< Invalid Board Info Type.
    E_MI_SYS_BOARD_INFO_TYPE_MIN=0,             ///< Min number of Board Info Type.
    E_MI_SYS_BOARD_INFO_TYPE_MAX,               ///< Max number of Board Info Type.
} MI_SYS_BoardInfoType_e;

typedef enum
{
    E_MI_SYS_PARAMTYPE_INVALID = -1,                        ///< Invalid Attribute of MY SYS.
    E_MI_SYS_PARAMTYPE_TYPE_MIN=0,                               ///< Min Attribute of MY SYS.
    E_MI_SYS_PARAMTYPE_PERSISTENT_DATA_BY_RESET = E_MI_SYS_PARAMTYPE_TYPE_MIN,       ///< this type means data without changed after reset and the pUsrParam is the pointer of MI_U16.
    E_MI_SYS_PARAMTYPE_MAX,                                 ///< Max Attribute of MY SYS.
} MI_SYS_ParamType_e;

typedef struct MI_SYS_MmapLayout_s
{
    MI_SYS_MmapId_e eMmapId;                    ///< enum type of mmap ID
    MI_U32 u32MemAddr;                          ///< start address of physical memory
    MI_U32 u32MemLen;                           ///< size of memory
    MI_SYS_MiuType_e eMiuType;                  ///< MIU type
    MI_SYS_CoBufLayer_e    eCoBufLayer;         ///< Co_Buffer Layer

} MI_SYS_MmapLayout_t;

typedef struct MI_SYS_Mmap_s
{
    // Version of structure
    MI_U32 u32Version;                          ///< should be MI_SYS_MMAP_PREFIX_VERSION

    // Memory Layout
    MI_U32 u32MiuInterval;                      ///< MIU_INTERVAL
    MI_U32 u32MmapLayoutCnt;                    ///< Count of mmap layout
    MI_SYS_MmapLayout_t *pstMmapLayout;         ///< array of mmap layout
} MI_SYS_Mmap_t;

typedef struct MI_SYS_BoardInfo_s
{
    MI_SYS_BoardInfoId_e eBoardInfoId;           ///< enum type of board info ID
    MI_SYS_BoardInfoType_e eInfoType;            ///< type of Board Info
    MI_U32 u32InfoLength;                        ///< lenght of Board Info by bytes
    MI_U32 au32InfoValue[8];                    ///< data pointer of Board Info
} MI_SYS_BoardInfo_t;

typedef struct MI_SYS_Board_s
{
    // Version of structure
    MI_U32 u32Version;                          ///< version of Board Info

    // Board Info
    MI_U32 u32BoardInfoCnt;                     ///< Count of board info
    MI_SYS_BoardInfo_t *pstBoardInfo;           ///< array of board info
} MI_SYS_Board_t;


typedef struct MI_SYS_InitParams_s
{
    MI_U8 *pszMmapFileName;                     ///< mmap file name for auto parsing memory layout in MI_SYS, If both pszMmapFileName & pstMemLayout are set, the MI_SYS will choose pszMmapFileName for first priority
    MI_SYS_Mmap_t stMmap;                       ///< mmap configured step by step

    // Customer_Info.h
    MI_U8 *pu8CidBuf;                           ///< CID_Buf[] in Customer_Info.h
    MI_U32 u32CidBufSize;                       ///< size of CID_Buf[]

    MI_U8 *pu8IpCtrlMapping1;                   ///< IP_Cntrol_Mapping_1[] in Customer_Info.h
    MI_U8 *pu8IpCtrlMapping2;                   ///< IP_Cntrol_Mapping_2[] in Customer_Info.h
    MI_U8 *pu8IpCtrlMapping3;                   ///< IP_Cntrol_Mapping_3[] in Customer_Info.h
    MI_U8 *pu8IpCtrlMapping4;                   ///< IP_Cntrol_Mapping_4[] in Customer_Info.h
    MI_U32 u32IpCtrlMappingSize;                ///< size of each IP_Cntrol_Maping_X[]

    MI_U8 *pu8CustomerHash;                     ///< Customer_hash[] in Customer_Info.h
    MI_U32 u32CustomerHashSize;                 ///< size of Customer_hash[]

    // Board.h information
    MI_SYS_Board_t stBoard;                     ///< board information

} MI_SYS_InitParams_t;

typedef struct MI_SYS_SdkVersion_s
{
    MI_U32 u32SdkVersion;                       ///< Version definition of SDK, 1.1: 0x0001 0001, 1.2: 0x0001 0002, 2.1: 0x0002 0001,....
    MI_U32 u32MmapVersion;                      ///< MI_SYS_MMAP_PREFIX_VERSION
    MI_U8 au8BuildTime[32];                    ///< the binary latest build date time
    MI_U8 au8MiSdkCommitId[50];               ///< git mi sdk commit id
    MI_U8 au8DdiMiscCommitId[50];       ///< git ddi misc commit id
    MI_U8 au8BspPkgCommitId[50];        ///< git bsp package commit id
} MI_SYS_SdkVersion_t;

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief Init SYSTEM module.
/// @param[in] pstInitParams: A pointer to structure MI_SYS_InitParams_t for initialization.
/// @return MI_OK: Process success.
/// @return MI_ERR_INVALID_PARAMETER: Parameters is invalid.
/// @return MI_ERR_RESOURCES: No available resource.
/// @return MI_ERR_MEMORY_ALLOCATE: Allocate memory fail.
/// @return MI_ERR_LIMITION: (EnvMmap) MMAP end address over MIU size.
/// @return MI_ERR_DATA_ERROR: Invalid MMAP name string array index.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_SYS_Init(MI_SYS_InitParams_t *pstInitParams);

//------------------------------------------------------------------------------
/// @brief Exit and finalize SYSTEM module.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_SYS_DeInit(void);

//------------------------------------------------------------------------------
/// @brief Get the version information of SDK.
/// @param[out] pstSdkVer Pointer to struct MI_SYS_SdkVersion_t to retrieve the version informaton of MI SDK.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_SYS_GetSdkVersion(MI_SYS_SdkVersion_t *pstSdkVer);

//------------------------------------------------------------------------------
/// @brief Get the hardware memory layout current stored.
/// @param[in] eMmapId ID of memory map
/// @param[out] pstMmapLayout Pointer to struct MI_SYS_MmapLayout_t to retrieve the memory map of the specified mmap id.
/// @param[out] pu32MiuInterval Pointer to MI_U32 to retrieve the MIU_INTERVAL
/// @return MI_OK: Process success.
/// @return MI_ERR_INVALID_PARAMETER: Invalid parameter.
/// @return MI_ERR_NOT_INITED: Module hasn't be inited.
/// @return MI_ERR_DATA_ERROR: MMAP layout not configured.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_SYS_GetMmapLayout(MI_SYS_MmapId_e eMmapId, MI_SYS_MmapLayout_t *pstMmapLayout, MI_U32 *pu32MiuInterval);

//------------------------------------------------------------------------------
/// @brief Get Memory pool ID.
/// @param[out] ps32CachedPoolId: Pointer to retrieve the pool id of cached memory. Set NULL to ignore.
/// @param[out] ps32NonCachedPoolId: Pointer to retrieve the pool id of Noncached memory. Set NULL to ignore.
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: MI_SYS hasn't been inited.
/// @return MI_ERR_INVALID_PARAMETER: Parameters is invalid.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_SYS_GetMemPoolId(MI_S32 *ps32CachedPoolId, MI_S32 *ps32NonCachedPoolId);

//------------------------------------------------------------------------------
/// @brief Get customer bonding information.
/// @param[in] eCustomerId: Customer ID.
/// @param[out] pInfo: Pointer to output information.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_SYS_GetCustomerBondingInfo(MI_SYS_Customer_e eCustomerId, void* pInfo);

//------------------------------------------------------------------------------
/// @brief Set SYS Attribute.
/// @param[in] eAttrType: for sys module function type
/// @param[in] pUsrParam: Pointer to input information.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_SYS_SetAttr(MI_SYS_ParamType_e eParamType, void* pUsrParam);

//------------------------------------------------------------------------------
/// @brief Get SYS Attribute.
/// @param[in] eAttrType: for sys module function type
/// @param[out] pUsrParam: Pointer to output information.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_SYS_GetAttr(MI_SYS_ParamType_e eParamType, void* pUsrParam);

//------------------------------------------------------------------------------
/// @brief Set debug level.
/// @param[in] eDgbLevel: Debug level defined in enum type MI_DBG_LEVEL_e
/// @return MI_OK: Set debug level success.
//------------------------------------------------------------------------------
MI_RESULT MI_SYS_SetDebugLevel(MI_DBG_LEVEL_e eDgbLevel);

MI_RESULT MI_SYS_LaunchSystemCmd(MI_U8 *pszCmd);
void MI_Delayus(unsigned int us);
void MI_Delayms(unsigned int ms);
int MI_Get_SystemTime_ms(void);

#ifdef __cplusplus
}
#endif

#endif///_MI_SYS_H_


