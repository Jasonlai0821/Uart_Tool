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

#ifndef _MI_OS_H_
#define _MI_OS_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "mi_common.h"

//-------------------------------------------------------------------------------------------------
//  Defines
//-------------------------------------------------------------------------------------------------
#define MI_OS_TICK_PER_ONE_MS                               (1)
#define MI_OS_WAIT_FOREVER                                  (0xffffff00/MI_OS_TICK_PER_ONE_MS)
#define MI_OS_PROCESS_PRIVATE                               (0x00000000)
#define MI_OS_PROCESS_SHARED                                (0x00000001)
#define MI_OS_IRQ_MAX                                       (128) //64 IRQs + 64 FIQs
#define MI_OS_ECOS_MAX_PRIORITY_LEVEL                       (31)
#define MI_OS_ECOS_MIN_PRIORITY_LEVEL                       (0)

typedef void ( *MI_OS_TaskEntry ) (MI_U32 argc, void *argv); ///< Task entry function  argc: pass additional data to task entry; argv: not used by eCos

//typedef void ( *SignalCb ) (MS_U32 u32Signals);        ///< Signal callback function
typedef void ( *MI_OS_TimerCb ) (MI_U32 u32StTimer, MI_U32 u32TimerId);  ///< Timer callback function  u32StTimer: not used; u32TimerId: Timer ID

//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_UCOS
/// Task priority
typedef enum MI_OS_TaskPriority_s
{
    E_MI_OS_TASK_PRI_SYS      = 0,    ///< System priority task   ( interrupt level driver, e.g. TSP, SMART )
    E_MI_OS_TASK_PRI_HIGHEST  = 16,   ///< Highest priority task  ( background monitor driver, e.g. DVBC, HDMI )
    E_MI_OS_TASK_PRI_HIGH     = 32,   ///< High priority task     ( service task )
    E_MI_OS_TASK_PRI_MEDIUM   = 48,   ///< Medium priority task   ( application task )
    E_MI_OS_TASK_PRI_LOW      = 64,   ///< Low priority task      ( nonbusy application task )
    E_MI_OS_TASK_PRI_LOWEST   = 96,   ///< Lowest priority task   ( idle application task )
} MI_OS_TaskPriority_e;
#elif defined(MSOS_TYPE_NUTTX)
/// Task priority
typedef enum MI_OS_TaskPriority_s
{
    E_MI_OS_TASK_PRI_SYS      = 255,
    E_MI_OS_TASK_PRI_HIGHEST  = 255,
    E_MI_OS_TASK_PRI_HIGH     = 178,
    E_MI_OS_TASK_PRI_MEDIUM   = 100,
    E_MI_OS_TASK_PRI_LOW      = 50,
    E_MI_OS_TASK_PRI_LOWEST   = 1,
} MI_OS_TaskPriority_e;
#else
/// Task priority
typedef enum MI_OS_TaskPriority_s
{
    E_MI_OS_TASK_PRI_SYS      = 0,    ///< System priority task   ( interrupt level driver, e.g. TSP, SMART )
    E_MI_OS_TASK_PRI_HIGHEST  = 4,    ///< Highest priority task  ( background monitor driver, e.g. DVBC, HDMI )
    E_MI_OS_TASK_PRI_HIGH     = 8,    ///< High priority task     ( service task )
    E_MI_OS_TASK_PRI_MEDIUM   = 12,   ///< Medium priority task   ( application task )
    E_MI_OS_TASK_PRI_LOW      = 16,   ///< Low priority task      ( nonbusy application task )
    E_MI_OS_TASK_PRI_LOWEST   = 24,   ///< Lowest priority task   ( idle application task )
} MI_OS_TaskPriority_e;
#endif

typedef enum MI_OS_Attribute_s
{
    E_MI_OS_MSOS_PRIORITY,            ///< Priority-order suspension
    E_MI_OS_MSOS_FIFO,                ///< FIFO-order suspension
} MI_OS_Attribute_e;

typedef enum MI_OS_EventWaitMode_s
{
    E_MI_OS_AND,                      ///< Specify all of the requested events are require.
    E_MI_OS_OR,                       ///< Specify any of the requested events are require.
    E_MI_OS_AND_CLEAR,                ///< Specify all of the requested events are require. If the request are successful, clear the event.
    E_MI_OS_OR_CLEAR,                 ///< Specify any of the requested events are require. If the request are successful, clear the event.
} MI_OS_EventWaitMode_e;

typedef enum MI_OS_MessageType_s
{
    E_MI_OS_MSG_FIXED_SIZE,           ///< Fixed size message
    E_MI_OS_MSG_VAR_SIZE,             ///< Variable size message
} MI_OS_MessageType_e;

typedef enum MI_OS_IrqDebugOpt_s
{
    E_MI_OS_IRQ_DISABLE = 0 << 0, // reserve for MI_OS_DisableInterrupt
    E_MI_OS_IRQ_ENABLE = 1 << 0, // reserve for MI_OS_EnableInterrupt
    E_MI_OS_IRQ_ACK = 1 << 1,
    E_MI_OS_IRQ_DEBUG_STATUS_FLOW = 1 << 2,
    E_MI_OS_IRQ_DEBUG_DISABLE = 1 << 31,
} MI_OS_IrqDebugOpt_e;

typedef enum
{
    // IRQ
    E_MI_OS_INT_IRQ_0x00_START                = 0x00,
    E_MI_OS_INT_IRQ_UART0                     = E_MI_OS_INT_IRQ_0x00_START+0,
    E_MI_OS_INT_IRQ_BDMA_CH0                  = E_MI_OS_INT_IRQ_0x00_START+1,
    E_MI_OS_INT_IRQ_BDMA_CH1                  = E_MI_OS_INT_IRQ_0x00_START+2,
    E_MI_OS_INT_IRQ_MVD                       = E_MI_OS_INT_IRQ_0x00_START+3,
    E_MI_OS_INT_IRQ_PS                        = E_MI_OS_INT_IRQ_0x00_START+4,
    E_MI_OS_INT_IRQ_NFIE                      = E_MI_OS_INT_IRQ_0x00_START+5,
    E_MI_OS_INT_IRQ_USB                       = E_MI_OS_INT_IRQ_0x00_START+6,
    E_MI_OS_INT_IRQ_UHC                       = E_MI_OS_INT_IRQ_0x00_START+7,
    E_MI_OS_INT_IRQ_EC_BRIDGE                 = E_MI_OS_INT_IRQ_0x00_START+8,
    E_MI_OS_INT_IRQ_EMAC                      = E_MI_OS_INT_IRQ_0x00_START+9,
    E_MI_OS_INT_IRQ_DISP                      = E_MI_OS_INT_IRQ_0x00_START+10,
    E_MI_OS_INT_IRQ_DHC                       = E_MI_OS_INT_IRQ_0x00_START+11,
    E_MI_OS_INT_IRQ_PMSLEEP                   = E_MI_OS_INT_IRQ_0x00_START+12,
    E_MI_OS_INT_IRQ_SBM                       = E_MI_OS_INT_IRQ_0x00_START+13,
    E_MI_OS_INT_IRQ_COMB                      = E_MI_OS_INT_IRQ_0x00_START+14,
    E_MI_OS_INT_IRQ_ECC_DERR                  = E_MI_OS_INT_IRQ_0x00_START+15,
    E_MI_OS_INT_IRQ_0x00_END                  = 0x0F,

    E_MI_OS_INT_IRQ_0x10_START                = 0x10,
    E_MI_OS_INT_IRQ_TSP2HK                    = E_MI_OS_INT_IRQ_0x10_START+0,
    E_MI_OS_INT_IRQ_VE                        = E_MI_OS_INT_IRQ_0x10_START+1,
    E_MI_OS_INT_IRQ_CIMAX2MCU                 = E_MI_OS_INT_IRQ_0x10_START+2,
    E_MI_OS_INT_IRQ_DC                        = E_MI_OS_INT_IRQ_0x10_START+3,
    E_MI_OS_INT_IRQ_GOP                       = E_MI_OS_INT_IRQ_0x10_START+4,
    E_MI_OS_INT_IRQ_PCM                       = E_MI_OS_INT_IRQ_0x10_START+5,
    E_MI_OS_INT_IRQ_IIC0                      = E_MI_OS_INT_IRQ_0x10_START+6,
    E_MI_OS_INT_IRQ_RTC                       = E_MI_OS_INT_IRQ_0x10_START+7,
    E_MI_OS_INT_IRQ_KEYPAD                    = E_MI_OS_INT_IRQ_0x10_START+8,
    E_MI_OS_INT_IRQ_PM                        = E_MI_OS_INT_IRQ_0x10_START+9,
    E_MI_OS_INT_IRQ_DDC2BI                    = E_MI_OS_INT_IRQ_0x10_START+10,
    E_MI_OS_INT_IRQ_SCM                       = E_MI_OS_INT_IRQ_0x10_START+11,
    E_MI_OS_INT_IRQ_VBI                       = E_MI_OS_INT_IRQ_0x10_START+12,
    E_MI_OS_INT_IRQ_M4VD                      = E_MI_OS_INT_IRQ_0x10_START+13,
    E_MI_OS_INT_IRQ_FCIE2RIU                  = E_MI_OS_INT_IRQ_0x10_START+14,
    E_MI_OS_INT_IRQ_ADCDVI2RIU                = E_MI_OS_INT_IRQ_0x10_START+15,
    E_MI_OS_INT_IRQ_0x10_END                  = 0x1F,

    // FIQ
    E_MI_OS_INT_FIQ_0x20_START                = 0x20,
    E_MI_OS_INT_FIQ_EXTIMER0                  = E_MI_OS_INT_FIQ_0x20_START+0,
    E_MI_OS_INT_FIQ_EXTIMER1                  = E_MI_OS_INT_FIQ_0x20_START+1,
    E_MI_OS_INT_FIQ_WDT                       = E_MI_OS_INT_FIQ_0x20_START+2,
    E_MI_OS_INT_FIQ_AEON_TO_8051              = E_MI_OS_INT_FIQ_0x20_START+3,
    E_MI_OS_INT_FIQ_8051_TO_AEON              = E_MI_OS_INT_FIQ_0x20_START+4,
    E_MI_OS_INT_FIQ_8051_TO_BEON              = E_MI_OS_INT_FIQ_0x20_START+5,
    E_MI_OS_INT_FIQ_BEON_TO_8051              = E_MI_OS_INT_FIQ_0x20_START+6,
    E_MI_OS_INT_FIQ_BEON_TO_AEON              = E_MI_OS_INT_FIQ_0x20_START+7,
    E_MI_OS_INT_FIQ_AEON_TO_BEON              = E_MI_OS_INT_FIQ_0x20_START+8,
    E_MI_OS_INT_FIQ_JPD                       = E_MI_OS_INT_FIQ_0x20_START+9,
    E_MI_OS_INT_FIQ_MENULOAD                  = E_MI_OS_INT_FIQ_0x20_START+10,
    E_MI_OS_INT_FIQ_HDMI_NON_PCM              = E_MI_OS_INT_FIQ_0x20_START+11,
    E_MI_OS_INT_FIQ_SPDIF_IN_NON_PCM          = E_MI_OS_INT_FIQ_0x20_START+12,
    E_MI_OS_INT_FIQ_EMAC                      = E_MI_OS_INT_FIQ_0x20_START+13,
    E_MI_OS_INT_FIQ_SE_DSP2UP                 = E_MI_OS_INT_FIQ_0x20_START+14,
    E_MI_OS_INT_FIQ_TSP2AEON                  = E_MI_OS_INT_FIQ_0x20_START+15,
    E_MI_OS_INT_FIQ_0x20_END                  = 0x2F,

    E_MI_OS_INT_FIQ_0x30_START                = 0x30,
    E_MI_OS_INT_FIQ_VIVALDI_STR               = E_MI_OS_INT_FIQ_0x30_START+0,
    E_MI_OS_INT_FIQ_VIVALDI_PTS               = E_MI_OS_INT_FIQ_0x30_START+1,
    E_MI_OS_INT_FIQ_DSP_MIU_PROT              = E_MI_OS_INT_FIQ_0x30_START+2,
    E_MI_OS_INT_FIQ_XIU_TIMEOUT               = E_MI_OS_INT_FIQ_0x30_START+3,
    E_MI_OS_INT_FIQ_DMA_DONE                  = E_MI_OS_INT_FIQ_0x30_START+4,
    E_MI_OS_INT_FIQ_VSYNC_VE4VBI              = E_MI_OS_INT_FIQ_0x30_START+5,
    E_MI_OS_INT_FIQ_FIELD_VE4VBI              = E_MI_OS_INT_FIQ_0x30_START+6,
    E_MI_OS_INT_FIQ_VDMCU2HK                  = E_MI_OS_INT_FIQ_0x30_START+7,
    E_MI_OS_INT_FIQ_VE_DONE_TT                = E_MI_OS_INT_FIQ_0x30_START+8,
    E_MI_OS_INT_FIQ_INT_CCFL                  = E_MI_OS_INT_FIQ_0x30_START+9,
    E_MI_OS_INT_FIQ_INT                       = E_MI_OS_INT_FIQ_0x30_START+10,
    E_MI_OS_INT_FIQ_IR                        = E_MI_OS_INT_FIQ_0x30_START+11,
    E_MI_OS_INT_FIQ_AFEC_VSYNC                = E_MI_OS_INT_FIQ_0x30_START+12,
    E_MI_OS_INT_FIQ_DEC_DSP2UP                = E_MI_OS_INT_FIQ_0x30_START+13,
    E_MI_OS_INT_FIQ_MIPS_WDT                  = E_MI_OS_INT_FIQ_0x30_START+14,
    E_MI_OS_INT_FIQ_DEC_DSP2MIPS              = E_MI_OS_INT_FIQ_0x30_START+15,
    E_MI_OS_INT_FIQ_0x30_END                  = 0x3F,

    E_MI_OS_INT_IRQ_0x40_START                = 0x40,
    E_MI_OS_INT_IRQ_SVD_HVD                   = E_MI_OS_INT_IRQ_0x40_START+0,
    E_MI_OS_INT_IRQ_USB2                      = E_MI_OS_INT_IRQ_0x40_START+1,
    E_MI_OS_INT_IRQ_UHC2                      = E_MI_OS_INT_IRQ_0x40_START+2,
    E_MI_OS_INT_IRQ_MIU                       = E_MI_OS_INT_IRQ_0x40_START+3,
    E_MI_OS_INT_IRQ_GDMA                      = E_MI_OS_INT_IRQ_0x40_START+4,
    E_MI_OS_INT_IRQ_UART2                     = E_MI_OS_INT_IRQ_0x40_START+5,
    E_MI_OS_INT_IRQ_UART1                     = E_MI_OS_INT_IRQ_0x40_START+6,
    E_MI_OS_INT_IRQ_DEMOD                     = E_MI_OS_INT_IRQ_0x40_START+7,
    E_MI_OS_INT_IRQ_MPIF                      = E_MI_OS_INT_IRQ_0x40_START+8,
    E_MI_OS_INT_IRQ_JPD                       = E_MI_OS_INT_IRQ_0x40_START+9,
    E_MI_OS_INT_IRQ_AEON2HI                   = E_MI_OS_INT_IRQ_0x40_START+10,
    E_MI_OS_INT_IRQ_BDMA0                     = E_MI_OS_INT_IRQ_0x40_START+11,
    E_MI_OS_INT_IRQ_BDMA1                     = E_MI_OS_INT_IRQ_0x40_START+12,
    E_MI_OS_INT_IRQ_OTG                       = E_MI_OS_INT_IRQ_0x40_START+13,
    E_MI_OS_INT_IRQ_MVD_CHECKSUM_FAIL         = E_MI_OS_INT_IRQ_0x40_START+14,
    E_MI_OS_INT_IRQ_TSP_CHECKSUM_FAIL         = E_MI_OS_INT_IRQ_0x40_START+15,
    E_MI_OS_INT_IRQ_0x40_END                  = 0x4F,

    E_MI_OS_INT_IRQ_0x50_START                = 0x50,
    E_MI_OS_INT_IRQ_CA_I3                     = E_MI_OS_INT_IRQ_0x50_START+0,
    E_MI_OS_INT_IRQ_HDMI_LEVEL                = E_MI_OS_INT_IRQ_0x50_START+1,
    E_MI_OS_INT_IRQ_MIPS_WADR_ERR             = E_MI_OS_INT_IRQ_0x50_START+2,
    E_MI_OS_INT_IRQ_RASP                      = E_MI_OS_INT_IRQ_0x50_START+3,
    E_MI_OS_INT_IRQ_CA_SVP                    = E_MI_OS_INT_IRQ_0x50_START+4,
    E_MI_OS_INT_IRQ_UART2MCU                  = E_MI_OS_INT_IRQ_0x50_START+5,
    E_MI_OS_INT_IRQ_URDMA2MCU                 = E_MI_OS_INT_IRQ_0x50_START+6,
    E_MI_OS_INT_IRQ_IIC1                      = E_MI_OS_INT_IRQ_0x50_START+7,
    E_MI_OS_INT_IRQ_HDCP                      = E_MI_OS_INT_IRQ_0x50_START+8,
    E_MI_OS_INT_IRQ_DMA_WADR_ERR              = E_MI_OS_INT_IRQ_0x50_START+9,
    E_MI_OS_INT_IRQ_UP_IRQ_UART_CA            = E_MI_OS_INT_IRQ_0x50_START+10,
    E_MI_OS_INT_IRQ_UP_IRQ_EMM_ECM            = E_MI_OS_INT_IRQ_0x50_START+11,
    E_MI_OS_INT_IRQ_ONIF                      = E_MI_OS_INT_IRQ_0x50_START+12,
    E_MI_OS_INT_IRQ_USB1                      = E_MI_OS_INT_IRQ_0x50_START+13,
    E_MI_OS_INT_IRQ_UHC1                      = E_MI_OS_INT_IRQ_0x50_START+14,
    E_MI_OS_INT_IRQ_MFE                       = E_MI_OS_INT_IRQ_0x50_START+15,
    E_MI_OS_INT_IRQ_0x50_END                  = 0x5F,

    E_MI_OS_INT_FIQ_0x60_START                = 0x60,
    E_MI_OS_INT_FIQ_IR_INT_RC                 = E_MI_OS_INT_FIQ_0x60_START+0,
    E_MI_OS_INT_FIQ_HDMITX_IRQ_EDGE           = E_MI_OS_INT_FIQ_0x60_START+1,
    E_MI_OS_INT_FIQ_UP_IRQ_UART_CA            = E_MI_OS_INT_FIQ_0x60_START+2,
    E_MI_OS_INT_FIQ_UP_IRQ_EMM_ECM            = E_MI_OS_INT_FIQ_0x60_START+3,
    E_MI_OS_INT_FIQ_PVR2MI_INT0               = E_MI_OS_INT_FIQ_0x60_START+4,
    E_MI_OS_INT_FIQ_PVR2MI_INT1               = E_MI_OS_INT_FIQ_0x60_START+5,
    E_MI_OS_INT_IRQ_FIQ_INT                   = E_MI_OS_INT_FIQ_0x60_START+6,
    E_MI_OS_INT_IRQ_UART3                     = E_MI_OS_INT_FIQ_0x60_START+7,
    E_MI_OS_INT_FIQ_AEON_TO_MIPS_VPE0         = E_MI_OS_INT_FIQ_0x60_START+8,
    E_MI_OS_INT_FIQ_AEON_TO_MIPS_VPE1         = E_MI_OS_INT_FIQ_0x60_START+9,
    E_MI_OS_INT_FIQ_SECEMAC                   = E_MI_OS_INT_FIQ_0x60_START+10,
    E_MI_OS_INT_FIQ_IR2_INT                   = E_MI_OS_INT_FIQ_0x60_START+11,
    E_MI_OS_INT_FIQ_MIPS_VPE1_TO_MIPS_VPE0    = E_MI_OS_INT_FIQ_0x60_START+12,
    E_MI_OS_INT_FIQ_MIPS_VPE1_TO_AEON         = E_MI_OS_INT_FIQ_0x60_START+13,
    E_MI_OS_INT_FIQ_MIPS_VPE1_TO_8051         = E_MI_OS_INT_FIQ_0x60_START+14,
    E_MI_OS_INT_FIQ_IR2_INT_RC                = E_MI_OS_INT_FIQ_0x60_START+15,
    E_MI_OS_INT_FIQ_0x60_END                  = 0x6F,

    E_MI_OS_INT_FIQ_0x70_START                = 0x70,
    E_MI_OS_INT_FIQ_MIPS_VPE0_TO_MIPS_VPE1    = E_MI_OS_INT_FIQ_0x70_START+0,
    E_MI_OS_INT_FIQ_MIPS_VPE0_TO_AEON         = E_MI_OS_INT_FIQ_0x70_START+1,
    E_MI_OS_INT_FIQ_MIPS_VPE0_TO_8051         = E_MI_OS_INT_FIQ_0x70_START+2,
    E_MI_OS_INT_FIQ_IR_IN                     = E_MI_OS_INT_FIQ_0x70_START+3,
    E_MI_OS_INT_FIQ_DMDMCU2HK                 = E_MI_OS_INT_FIQ_0x70_START+4,
    E_MI_OS_INT_FIQ_R2TOMCU_INT0              = E_MI_OS_INT_FIQ_0x70_START+5,
    E_MI_OS_INT_FIQ_R2TOMCU_INT1              = E_MI_OS_INT_FIQ_0x70_START+6,
    E_MI_OS_INT_FIQ_DSPTOMCU_INT0             = E_MI_OS_INT_FIQ_0x70_START+7,
    E_MI_OS_INT_FIQ_DSPTOMCU_INT1             = E_MI_OS_INT_FIQ_0x70_START+8,
    E_MI_OS_INT_FIQ_USB                       = E_MI_OS_INT_FIQ_0x70_START+9,
    E_MI_OS_INT_FIQ_UHC                       = E_MI_OS_INT_FIQ_0x70_START+10,
    E_MI_OS_INT_FIQ_USB1                      = E_MI_OS_INT_FIQ_0x70_START+11,
    E_MI_OS_INT_FIQ_UHC1                      = E_MI_OS_INT_FIQ_0x70_START+12,
    E_MI_OS_INT_FIQ_USB2                      = E_MI_OS_INT_FIQ_0x70_START+13,
    E_MI_OS_INT_FIQ_UHC2                      = E_MI_OS_INT_FIQ_0x70_START+14,
    //Not Used                          = E_MI_OS_INT_FIQ_0x70_START+15,
    E_MI_OS_INT_FIQ_0x70_END                  = 0x7F,


    // Add IRQ from 0x80 ~ 0xBF,
    // if IRQ enum from 0x00 ~ 0x1F, and 0x40 ~ 0x5F is occupied
    E_MI_OS_INT_IRQ_0x80_START                = 0x80,
    E_MI_OS_INT_IRQ_MLINK                     = E_MI_OS_INT_IRQ_0x80_START+0,
    E_MI_OS_INT_IRQ_AFEC                      = E_MI_OS_INT_IRQ_0x80_START+1,
    E_MI_OS_INT_IRQ_DPTX                      = E_MI_OS_INT_IRQ_0x80_START+2,
    E_MI_OS_INT_IRQ_TMDDRLINK                 = E_MI_OS_INT_IRQ_0x80_START+3,
    E_MI_OS_INT_IRQ_DISPI                     = E_MI_OS_INT_IRQ_0x80_START+4,
    E_MI_OS_INT_IRQ_EXP_MLINK                 = E_MI_OS_INT_IRQ_0x80_START+5,
    E_MI_OS_INT_IRQ_M4VE                      = E_MI_OS_INT_IRQ_0x80_START+6,
    E_MI_OS_INT_IRQ_DVI_HDMI_HDCP             = E_MI_OS_INT_IRQ_0x80_START+7,
    E_MI_OS_INT_IRQ_G3D2MCU                   = E_MI_OS_INT_IRQ_0x80_START+8,
    E_MI_OS_INT_IRQ_VP6                       = E_MI_OS_INT_IRQ_0x80_START+9,
    E_MI_OS_INT_IRQ_INT                       = E_MI_OS_INT_IRQ_0x80_START+10,
    E_MI_OS_INT_IRQ_CEC                       = E_MI_OS_INT_IRQ_0x80_START+11,
    E_MI_OS_INT_IRQ_HDCP_IIC                  = E_MI_OS_INT_IRQ_0x80_START+12,
    E_MI_OS_INT_IRQ_HDCP_X74                  = E_MI_OS_INT_IRQ_0x80_START+13,
    E_MI_OS_INT_IRQ_WADR_ERR                  = E_MI_OS_INT_IRQ_0x80_START+14,
    E_MI_OS_INT_IRQ_DCSUB                     = E_MI_OS_INT_IRQ_0x80_START+15,
    E_MI_OS_INT_IRQ_0x80_END                  = 0x8F,

    E_MI_OS_INT_IRQ_0x90_START                = 0x90,
    E_MI_OS_INT_IRQ_GE                        = E_MI_OS_INT_IRQ_0x90_START+0,
    E_MI_OS_INT_IRQ_SYNC_DET                  = E_MI_OS_INT_IRQ_0x90_START+1,
    E_MI_OS_INT_IRQ_FSP                       = E_MI_OS_INT_IRQ_0x90_START+2,
    E_MI_OS_INT_IRQ_PWM_RP_L                  = E_MI_OS_INT_IRQ_0x90_START+3,
    E_MI_OS_INT_IRQ_PWM_FP_L                  = E_MI_OS_INT_IRQ_0x90_START+4,
    E_MI_OS_INT_IRQ_PWM_RP_R                  = E_MI_OS_INT_IRQ_0x90_START+5,
    E_MI_OS_INT_IRQ_PWM_FP_R                  = E_MI_OS_INT_IRQ_0x90_START+6,
    E_MI_OS_INT_IRQ_FRC_SC                    = E_MI_OS_INT_IRQ_0x90_START+7,
    E_MI_OS_INT_IRQ_FRC_INT_FIQ2HST0          = E_MI_OS_INT_IRQ_0x90_START+8,
    E_MI_OS_INT_IRQ_SMART                     = E_MI_OS_INT_IRQ_0x90_START+9,
    E_MI_OS_INT_IRQ_MVD2MIPS                  = E_MI_OS_INT_IRQ_0x90_START+10,
    E_MI_OS_INT_IRQ_GPD                       = E_MI_OS_INT_IRQ_0x90_START+11,
    E_MI_OS_INT_IRQ_DS                        = E_MI_OS_INT_IRQ_0x90_START+12,
    E_MI_OS_INT_IRQ_FRC_INT_IRQ2HST0          = E_MI_OS_INT_IRQ_0x90_START+13,
    E_MI_OS_INT_IRQ_MIIC_DMA_INT3             = E_MI_OS_INT_IRQ_0x90_START+14,
    E_MI_OS_INT_IRQ_MIIC_INT3                 = E_MI_OS_INT_IRQ_0x90_START+15,
    E_MI_OS_INT_IRQ_0x90_END                  = 0x9F,

    E_MI_OS_INT_IRQ_0xA0_START                = 0xA0,
    E_MI_OS_INT_IRQ_IIC2                      = E_MI_OS_INT_IRQ_0xA0_START+0,
    E_MI_OS_INT_IRQ_MIIC_DMA0                 = E_MI_OS_INT_IRQ_0xA0_START+1,
    E_MI_OS_INT_IRQ_MIIC_DMA1                 = E_MI_OS_INT_IRQ_0xA0_START+2,
    E_MI_OS_INT_IRQ_MIIC_DMA2                 = E_MI_OS_INT_IRQ_0xA0_START+3,
    E_MI_OS_INT_IRQ_MSPI0                     = E_MI_OS_INT_IRQ_0xA0_START+4,
    E_MI_OS_INT_IRQ_MSPI1                     = E_MI_OS_INT_IRQ_0xA0_START+5,
    E_MI_OS_INT_IRQ_EXT_GPIO0                 = E_MI_OS_INT_IRQ_0xA0_START+6,
    E_MI_OS_INT_IRQ_EXT_GPIO1                 = E_MI_OS_INT_IRQ_0xA0_START+7,
    E_MI_OS_INT_IRQ_EXT_GPIO2                 = E_MI_OS_INT_IRQ_0xA0_START+8,
    E_MI_OS_INT_IRQ_EXT_GPIO3                 = E_MI_OS_INT_IRQ_0xA0_START+9,
    E_MI_OS_INT_IRQ_EXT_GPIO4                 = E_MI_OS_INT_IRQ_0xA0_START+10,
    E_MI_OS_INT_IRQ_EXT_GPIO5                 = E_MI_OS_INT_IRQ_0xA0_START+11,
    E_MI_OS_INT_IRQ_EXT_GPIO6                 = E_MI_OS_INT_IRQ_0xA0_START+12,
    E_MI_OS_INT_IRQ_EXT_GPIO7                 = E_MI_OS_INT_IRQ_0xA0_START+13,
    E_MI_OS_INT_IRQ_MIIC_DMA_INT2             = E_MI_OS_INT_IRQ_0xA0_START+14,
    E_MI_OS_INT_IRQ_MIIC_INT2                 = E_MI_OS_INT_IRQ_0xA0_START+15,
    E_MI_OS_INT_IRQ_0xA0_END                  = 0xAF,

    E_MI_OS_INT_IRQ_0xB0_START                = 0xB0,
    E_MI_OS_INT_IRQ_MIIC_DMA_INT1             = E_MI_OS_INT_IRQ_0xB0_START+0,
    E_MI_OS_INT_IRQ_MIIC_INT1                 = E_MI_OS_INT_IRQ_0xB0_START+1,
    E_MI_OS_INT_IRQ_MIIC_DMA_INT0             = E_MI_OS_INT_IRQ_0xB0_START+2,
    E_MI_OS_INT_IRQ_MIIC_INT0                 = E_MI_OS_INT_IRQ_0xB0_START+3,
    E_MI_OS_INT_IRQ_UHC30                     = E_MI_OS_INT_IRQ_0xB0_START+4,
    E_MI_OS_INT_IRQ_AU_DMA                    = E_MI_OS_INT_IRQ_0xB0_START+5,
    E_MI_OS_INT_IRQ_DIPW                      = E_MI_OS_INT_IRQ_0xB0_START+6,
    E_MI_OS_INT_IRQ_HDMITX                    = E_MI_OS_INT_IRQ_0xB0_START+7,
    E_MI_OS_INT_IRQ_U3_DPHY                   = E_MI_OS_INT_IRQ_0xB0_START+8,
    E_MI_OS_INT_IRQEXPL_TSO                   = E_MI_OS_INT_IRQ_0xB0_START+9,
    E_MI_OS_INT_IRQ_TSP_TSO0                  = E_MI_OS_INT_IRQ_0xB0_START+9,
    E_MI_OS_INT_IRQEXPH_CEC1                  = E_MI_OS_INT_IRQ_0xB0_START+10,
    E_MI_OS_INT_IRQ_TSP_TSO1                  = E_MI_OS_INT_IRQ_0xB0_START+10,
    E_MI_OS_INT_IRQ_BT_DMA                    = E_MI_OS_INT_IRQ_0xB0_START+11,
    E_MI_OS_INT_IRQ_BT_TAB                    = E_MI_OS_INT_IRQ_0xB0_START+12,
    E_MI_OS_INT_IRQ_SATA                      = E_MI_OS_INT_IRQ_0xB0_START+13,
    E_MI_OS_INT_IRQ_MHL_CBUS_PM               = E_MI_OS_INT_IRQ_0xB0_START+14,
    E_MI_OS_INT_IRQ_MHL_CBUS_WAKEUP           = E_MI_OS_INT_IRQ_0xB0_START+15,
    E_MI_OS_INT_IRQ_IDAC_PLUG_DET             = E_MI_OS_INT_IRQ_0xB0_START+15,
    E_MI_OS_INT_IRQ_0xB0_END                  = 0xBF,


    // Add FIQ from 0xC0 ~ 0xFD,
    // if FIQ enum from 0x20 ~ 0x4F, and 0x60 ~ 0x7F is occupied
    E_MI_OS_INT_FIQ_0xC0_START                = 0xC0,
    E_MI_OS_INT_FIQ_DMARD                     = E_MI_OS_INT_FIQ_0xC0_START+0,
    E_MI_OS_INT_FIQ_AU_DMA_BUF_INT            = E_MI_OS_INT_FIQ_0xC0_START+1,
    E_MI_OS_INT_FIQ_8051_TO_MIPS_VPE1         = E_MI_OS_INT_FIQ_0xC0_START+2,
    E_MI_OS_INT_FIQ_DVI_DET                   = E_MI_OS_INT_FIQ_0xC0_START+3,
    E_MI_OS_INT_FIQ_PM_GPIO0                  = E_MI_OS_INT_FIQ_0xC0_START+4,
    E_MI_OS_INT_FIQ_PM_GPIO1                  = E_MI_OS_INT_FIQ_0xC0_START+5,
    E_MI_OS_INT_FIQ_PM_GPIO2                  = E_MI_OS_INT_FIQ_0xC0_START+6,
    E_MI_OS_INT_FIQ_PM_GPIO3                  = E_MI_OS_INT_FIQ_0xC0_START+7,
    E_MI_OS_INT_FIQ_PM_XIU_TIMEOUT            = E_MI_OS_INT_FIQ_0xC0_START+8,
    E_MI_OS_INT_FIQ_PWM_RP_RP_L               = E_MI_OS_INT_FIQ_0xC0_START+9,
    E_MI_OS_INT_FIQ_PWM_RP_FP_L               = E_MI_OS_INT_FIQ_0xC0_START+10,
    E_MI_OS_INT_FIQ_PWM_RP_RP_R               = E_MI_OS_INT_FIQ_0xC0_START+11,
    E_MI_OS_INT_FIQ_PWM_RP_FP_R               = E_MI_OS_INT_FIQ_0xC0_START+12,
    E_MI_OS_INT_FIQ_8051_TO_MIPS_VPE0         = E_MI_OS_INT_FIQ_0xC0_START+13,
    E_MI_OS_INT_FIQ_FRC_R2_TO_MIPS            = E_MI_OS_INT_FIQ_0xC0_START+14,
    E_MI_OS_INT_FIQ_VP6                       = E_MI_OS_INT_FIQ_0xC0_START+15,
    E_MI_OS_INT_FIQ_0xC0_END                  = 0xCF,

    E_MI_OS_INT_FIQ_0xD0_START                = 0xD0,
    E_MI_OS_INT_FIQ_STRETCH                   = E_MI_OS_INT_FIQ_0xD0_START+0,
    E_MI_OS_INT_FIQ_LAN_ESD                   = E_MI_OS_INT_FIQ_0xD0_START+0,
    E_MI_OS_INT_FIQ_GPIO0                     = E_MI_OS_INT_FIQ_0xD0_START+1,
    E_MI_OS_INT_FIQ_REQ_HST0_TO_3             = E_MI_OS_INT_FIQ_0xD0_START+1,
    E_MI_OS_INT_FIQ_GPIO1                     = E_MI_OS_INT_FIQ_0xD0_START+2,
    E_MI_OS_INT_FIQ_REQ_HST0_TO_2             = E_MI_OS_INT_FIQ_0xD0_START+2,
    E_MI_OS_INT_FIQ_GPIO2                     = E_MI_OS_INT_FIQ_0xD0_START+3,
    E_MI_OS_INT_FIQ_REQ_HST0_TO_1             = E_MI_OS_INT_FIQ_0xD0_START+3,
    E_MI_OS_INT_FIQ_GPIO3                     = E_MI_OS_INT_FIQ_0xD0_START+4,
    E_MI_OS_INT_FIQ_ERROR_RASP                = E_MI_OS_INT_FIQ_0xD0_START+4,
    E_MI_OS_INT_FIQ_GPIO4                     = E_MI_OS_INT_FIQ_0xD0_START+5,
    E_MI_OS_INT_FIQ_REQ_HST1_TO_3             = E_MI_OS_INT_FIQ_0xD0_START+5,
    E_MI_OS_INT_FIQ_GPIO5                     = E_MI_OS_INT_FIQ_0xD0_START+6,
    E_MI_OS_INT_FIQ_REQ_HST1_TO_2             = E_MI_OS_INT_FIQ_0xD0_START+6,
    E_MI_OS_INT_FIQ_GPIO6                     = E_MI_OS_INT_FIQ_0xD0_START+7,
    E_MI_OS_INT_FIQ_REQ_HST1_TO_0             = E_MI_OS_INT_FIQ_0xD0_START+7,
    E_MI_OS_INT_FIQ_GPIO7                     = E_MI_OS_INT_FIQ_0xD0_START+8,
    E_MI_OS_INT_FIQ_REQ_HST2_TO_3             = E_MI_OS_INT_FIQ_0xD0_START+8,
    E_MI_OS_INT_FIQ_VE_VSYNC_IN               = E_MI_OS_INT_FIQ_0xD0_START+9,
    E_MI_OS_INT_FIQ_REQ_HST2_TO_1             = E_MI_OS_INT_FIQ_0xD0_START+9,
    E_MI_OS_INT_FIQEXPL_HST0_TO_3             = E_MI_OS_INT_FIQ_0xD0_START+10,
    E_MI_OS_INT_FIQ_REQ_HST2_TO_0             = E_MI_OS_INT_FIQ_0xD0_START+10,
    E_MI_OS_INT_FIQEXPL_HST1_TO_3             = E_MI_OS_INT_FIQ_0xD0_START+11,
    E_MI_OS_INT_FIQ_REQ_HST3_TO_2             = E_MI_OS_INT_FIQ_0xD0_START+11,
    E_MI_OS_INT_FIQEXPL_HST2_TO_3             = E_MI_OS_INT_FIQ_0xD0_START+12,
    E_MI_OS_INT_FIQ_REQ_HST3_TO_1             = E_MI_OS_INT_FIQ_0xD0_START+12,
    E_MI_OS_INT_FIQEXPH_CMDQ                  = E_MI_OS_INT_FIQ_0xD0_START+13,
    E_MI_OS_INT_FIQ_REQ_HST3_TO_0             = E_MI_OS_INT_FIQ_0xD0_START+13,
    E_MI_OS_INT_FIQEXPH_HDMITX_EDGE           = E_MI_OS_INT_FIQ_0xD0_START+14,
    E_MI_OS_INT_FIQEXPH_UHC30                 = E_MI_OS_INT_FIQ_0xD0_START+15,
    E_MI_OS_INT_INT_FIQ_0xD0_END              = 0xDF,

    E_MI_OS_INT_FIQ_0xE0_START                = 0xE0,
    E_MI_OS_INT_FIQ_LDM_DMA0                  = E_MI_OS_INT_FIQ_0xE0_START+0,
    E_MI_OS_INT_IRQ_RASP1                     = E_MI_OS_INT_FIQ_0xE0_START+0,
    E_MI_OS_INT_FIQ_LDM_DMA1                  = E_MI_OS_INT_FIQ_0xE0_START+1,
    E_MI_OS_INT_IRQ_SECEMAC                   = E_MI_OS_INT_FIQ_0xE0_START+1,
    E_MI_OS_INT_IRQ_SDIO                      = E_MI_OS_INT_FIQ_0xE0_START+2,
    E_MI_OS_INT_IRQ_UHC3                      = E_MI_OS_INT_FIQ_0xE0_START+3,
    E_MI_OS_INT_IRQ_USB3                      = E_MI_OS_INT_FIQ_0xE0_START+4,
    E_MI_OS_INT_FIQ_GPIO8                     = E_MI_OS_INT_FIQ_0xE0_START+5,
    E_MI_OS_INT_FIQ_GPIO9                     = E_MI_OS_INT_FIQ_0xE0_START+6,
    E_MI_OS_INT_FIQ_DISP_TGEN0                = E_MI_OS_INT_FIQ_0xE0_START+7,
    E_MI_OS_INT_IRQ_CA_CRYPTO_DMA             = E_MI_OS_INT_FIQ_0xE0_START+7,
    E_MI_OS_INT_FIQ_DISP_TGEN1                = E_MI_OS_INT_FIQ_0xE0_START+8,
    E_MI_OS_INT_IRQ_CA_PROG_PVR               = E_MI_OS_INT_FIQ_0xE0_START+8,
    E_MI_OS_INT_FIQ_DISP_TGEN2                = E_MI_OS_INT_FIQ_0xE0_START+9,
    E_MI_OS_INT_IRQ_CA_NSK_INT                = E_MI_OS_INT_FIQ_0xE0_START+9,
    E_MI_OS_INT_FIQ_DISP_TGEN3                = E_MI_OS_INT_FIQ_0xE0_START+10,
    E_MI_OS_INT_IRQ_TSP_ECM_FLT               = E_MI_OS_INT_FIQ_0xE0_START+10,
    E_MI_OS_INT_IRQ_ERROR_RESP                = E_MI_OS_INT_FIQ_0xE0_START+11,
    E_MI_OS_INT_IRQ_MIU_SECURITY              = E_MI_OS_INT_FIQ_0xE0_START+12,
    E_MI_OS_INT_FIQ_TEMPERATURE_FLAG_FALL     = E_MI_OS_INT_FIQ_0xE0_START+13,
    E_MI_OS_INT_IRQ_DISP1                     = E_MI_OS_INT_FIQ_0xE0_START+13,
    E_MI_OS_INT_FIQ_TEMPERATURE_FLAG_RISE     = E_MI_OS_INT_FIQ_0xE0_START+14,
    E_MI_OS_INT_IRQ_RTC1                      = E_MI_OS_INT_FIQ_0xE0_START+14,
    E_MI_OS_INT_FIQ_U3_DPHY                   = E_MI_OS_INT_FIQ_0xE0_START+15,
    E_MI_OS_INT_IRQ_GPU2MCU                   = E_MI_OS_INT_FIQ_0xE0_START+15,
    E_MI_OS_INT_FIQ_0xE0_END                  = 0xEF,

    E_MI_OS_INT_FIQ_0xF0_START                = 0xF0,
    E_MI_OS_INT_FIQ_DEC_DSP2R2M               = E_MI_OS_INT_FIQ_0xF0_START+0,
    E_MI_OS_INT_FIQ_AEON_TO_R2M               = E_MI_OS_INT_FIQ_0xF0_START+1,
    E_MI_OS_INT_FIQ_R2M_TO_AEON               = E_MI_OS_INT_FIQ_0xF0_START+2,
    E_MI_OS_INT_FIQ_R2M_TO_8051               = E_MI_OS_INT_FIQ_0xF0_START+3,
    E_MI_OS_INT_IRQ_VIVALDI_DMA_INTR2         = E_MI_OS_INT_FIQ_0xF0_START+4,
    E_MI_OS_INT_FIQ_AU_DMA_INT                = E_MI_OS_INT_FIQ_0xF0_START+4,
    E_MI_OS_INT_IRQ_VIVALDI_DMA_INTR1         = E_MI_OS_INT_FIQ_0xF0_START+5,
    E_MI_OS_INT_FIQ_AU_PCM_DMA_INT            = E_MI_OS_INT_FIQ_0xF0_START+5,
    E_MI_OS_INT_IRQ_AFEC_INT                  = E_MI_OS_INT_FIQ_0xF0_START+6,
    E_MI_OS_INT_FIQ_AU_SPDIF_TX_CS0           = E_MI_OS_INT_FIQ_0xF0_START+7,
    E_MI_OS_INT_FIQ_AU_SPDIF_TX_CS1           = E_MI_OS_INT_FIQ_0xF0_START+8,
    E_MI_OS_INT_FIQ_PCM_DMA                   = E_MI_OS_INT_FIQ_0xF0_START+9,
    E_MI_OS_INT_FIQ_DMDMCU2HK_1               = E_MI_OS_INT_FIQ_0xF0_START+9,
    E_MI_OS_INT_FIQ_VE_SW_WR2BUF              = E_MI_OS_INT_FIQ_0xF0_START+10,
    E_MI_OS_INT_IRQ_FRM_PM                    = E_MI_OS_INT_FIQ_0xF0_START+11,
    E_MI_OS_INT_FIQ_FRM_PM                    = E_MI_OS_INT_FIQ_0xF0_START+12,
    E_MI_OS_INT_FIQ_SATA_PHY                  = E_MI_OS_INT_FIQ_0xF0_START+13,
    E_MI_OS_INT_IRQ_FIQ_NONE                  = E_MI_OS_INT_FIQ_0xF0_START+14,
    E_MI_OS_INT_IRQ_FIQ_ALL                   = E_MI_OS_INT_FIQ_0xF0_START+15,
    E_MI_OS_INT_FIQ_0xF0_END                  = 0xFF,

} MI_OS_InterruptNum_e;

typedef void ( *MI_OS_InterruptCb ) (MI_OS_InterruptNum_e eIntNum);               ///< Interrupt callback function, put here as need MI_OS_InterruptNum

typedef struct MI_OS_MemPoolParams_s
{
    MI_U32  u32PoolSize; //Pool size in bytes
    void    *pPoolAddr; //Starting virtual address for the memory pool
} MI_OS_MemPoolParams_t;

typedef struct MI_OS_MemoryPoolInfo_s
{
    void    *pPoolAddr; //holding the starting address for the memory pool
    MI_U32  u32PoolSize; //holding the total size of the memory pool
    MI_U32  u32FreeSize; //holding the available free size of the memory pool
    MI_U32  u32LargestFreeBlockSize; //holding the size of the largest free block
} MI_OS_MemoryPoolInfo_t;

typedef struct MI_OS_FixSizeMemPoolParams_s
{
    MI_U32  u32PoolSize;//Pool size in bytes
    MI_U32  u32BlockSize;//fixed block size for each allocated block in the pool
    void    *pPoolAddr;//Starting virtual address for the memory pool
} MI_OS_FixSizeMemPoolParams_t;

typedef struct MI_OS_CreateTaskParams_s
{
    MI_OS_TaskEntry         pTaskEntry;
    MI_U32                  u32TaskEntryData;
    MI_OS_TaskPriority_e    eTaskPriority;
    MI_BOOL                 bAutoStart;
    void *                  pStackEntry;
    MI_U32                  u32StackSize;
    MI_U8                   *pTaskName;
} MI_OS_CreateTaskParams_t;

typedef struct MI_OS_TaskInfoParams_s
{
    MI_OS_TaskPriority_e    eTaskPriority;
    void *                  pStackEntry;
    MI_U32                  u32StackSize;
    MI_U32                  u32StackUsed;
    MI_U8                   aszTaskName[16];
} MI_OS_TaskInfoParams_s;

typedef struct MI_OS_CreateMutexParams_s
{
    MI_U8    *pMutexName;
    MI_U32  u32Flag;
} MI_OS_CreateMutexParams_t;

typedef struct MI_OS_MutexInfo_s
{
    MI_OS_Attribute_e   eAttribute;
    MI_U8               *pMutexName;
} MI_OS_MutexInfo_t;

typedef struct MI_OS_CreateSemParams_s
{
    MI_U8    *pSemName;
    MI_U32  u32InitCnt;
} MI_OS_CreateSemParams_t;

typedef struct MI_OS_SemInfo_s
{
    MI_U32              u32Cnt;
    MI_OS_Attribute_e   eAttribute;
    MI_U8               *pSemaphoreName;
} MI_OS_SemInfo_t;

typedef struct MI_OS_WaitEventParams_s
{
    MI_S32                  s32EventGroupId;
    MI_U32                  u32WaitEventFlag;
    MI_OS_EventWaitMode_e   eWaitMode;
    MI_U32                  u32WaitMs;
} MI_OS_WaitEventParams_t;

typedef struct MI_OS_CreateTimerParams_s
{
    MI_OS_TimerCb   pTimerCb;
    MI_U32          u32FirstTimeMs;
    MI_U32          u32PeriodTimeMs;
    MI_BOOL         bStartTimer;
    MI_U8           *pName;
} MI_OS_CreateTimerParams_t;

typedef struct MI_OS_ResetTimerParams_s
{
    MI_S32  s32TimerID;
    MI_U32  u32FirstTimeMs;
    MI_U32  u32PeriodTimeMs;
    MI_BOOL bStartTimer;
} MI_OS_ResetTimerParams_t;

typedef struct MI_OS_CreateQueueParams_s
{
    void                    *pStartAddr;
    MI_U32                  u32QueueSize;
    MI_OS_MessageType_e     eMessageType;
    MI_U32                  u32MessageSize;
    MI_OS_Attribute_e       eAttribute;
    MI_U8                   *pQueueName;
} MI_OS_CreateQueueParams_t;

typedef struct MI_OS_SendToQueueParams_s
{
    MI_S32  s32QueueId;
    MI_U8   *pu8Message;
    MI_U32  u32Size;
    MI_U32  u32WaitMs;
} MI_OS_SendToQueueParams_t;

typedef struct MI_OS_RecvFromQueueParams_s
{
    MI_S32  s32QueueId;
    MI_U32  u32IntendedSize;
    MI_U32  u32WaitMs;
    MI_U8   *pu8Message; // out params
    MI_U32  u32ActualSize; // out params
} MI_OS_RecvFromQueueParams_t;

typedef struct MI_OS_PeekFromQueueParams_s
{
    MI_S32  s32QueueId;
    MI_U32  u32IntendedSize;
    MI_U8   *pu8Message; // out params
    MI_U32  u32ActualSize; // out params
} MI_OS_PeekFromQueueParams_t;

typedef struct MI_OS_ClearQueueParams_s
{
    MI_S32  s32QueueId;
    MI_U32  u32WaitMs;
} MI_OS_ClearQueueParams_t;

typedef struct MI_OS_Caps_s
{
    MI_U32  u32MaxTaskNum;
    MI_U32  u32MaxMemPoolNum;
    MI_U32  u32MaxFixSizeMemPoolNum;
    MI_U32  u32MaxSemaphoreNum;
    MI_U32  u32MaxMutexNum;
    MI_U32  u32MaxEventGroupNum;
    MI_U32  u32MaxTimerNum;
    MI_U32  u32MaxQueueNum;
} MI_OS_Caps_t;

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief Create a variable-size virtual memory pool dynamically
/// @param[in] pstMemPoolParams : create pool params
/// @param[Out] ps32PoolId : assigned memory pool ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_CreateMemoryPool(MI_OS_MemPoolParams_t *pstMemPoolParams, MI_S32 *ps32PoolId);

//------------------------------------------------------------------------------
/// @brief Delete a variable-size memory pool
/// @param[in] s32PoolId : Pool ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DeleteMemoryPool(MI_S32 s32PoolId);

//------------------------------------------------------------------------------
/// @brief Get the information of a variable-size memory pool
/// @param[in] s32PoolId: s32PoolSize Pool ID
/// @param[Out] pstMemoryPoolInfo : InfoMemoryPool params.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_GetMemoryPoolInfo(MI_S32 s32PoolId, MI_OS_MemoryPoolInfo_t *pstMemoryPoolInfo);

//------------------------------------------------------------------------------
/// @brief Allocate a memory block with 16-Byte aligned starting address from memory pool
/// @param[in] u32Size Request size
/// @param[in] s32PoolId Memory pool ID
/// @param[Out]  pMemAddr : pointer to the allocated memory block
/// @return NULL: not enough available memory
/// @return Otherwise: pointer to the allocated memory block
//------------------------------------------------------------------------------
void* MI_OS_AllocateMemory(MI_U32 u32Size, MI_S32 s32PoolId);

//------------------------------------------------------------------------------
/// @brief ReAllocate a block of memory with 4-byte aligned start address from the variable-size memory pool
/// @param[in] pOrgAddr : points to the beginning of the original memory block
/// @param[in] u32Size : size of new memory block
/// @param[in] s32PoolId : memory pool ID
/// @return NULL: not enough available memory
/// @return Otherwise: pointer to the allocated memory block
//------------------------------------------------------------------------------
void* MI_OS_ReAllocateMemory(void *pOrgAddr, MI_U32 u32Size, MI_S32 s32PoolId);

//------------------------------------------------------------------------------
/// @brief Free a memory block from memory pool
/// @param[in] pAddress Pointer to previously allocated memory block
/// @param[in] s32PoolId Memory pool ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_FreeMemory(void* pAddr, MI_S32 s32PoolId);

//------------------------------------------------------------------------------
/// @brief Flush memory from pipe to dram
/// @param[in] None
/// @return None
//------------------------------------------------------------------------------
void MI_OS_FlushMemory(void);

//------------------------------------------------------------------------------
/// @brief Read memory from dram to pipe
/// @param[in] None
/// @return None
//------------------------------------------------------------------------------
void MI_OS_ReadMemory(void);

//------------------------------------------------------------------------------
/// @brief Create a fixed-size virtual memory pool dynamically, eCos support only.
/// @param[in] pstFixSizePoolParams : create fix size pool parameters.
/// @param[Out] ps32PoolId : assigned memory pool ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_CreateFixSizeMemoryPool(MI_OS_FixSizeMemPoolParams_t *pstFixSizePoolParams, MI_S32 *ps32PoolId);

//------------------------------------------------------------------------------
/// @brief Delete a fixed-size memory pool, eCos support only.
/// @param[in] s32PoolId : Pool ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DeleteFixSizeMemoryPool(MI_S32 s32PoolId);

//------------------------------------------------------------------------------
/// @brief Get the information of a fixed-size memory pool, eCos support only.
/// @param[in] s32PoolId: s32PoolSize Pool ID
/// @param[Out]  pstMemoryPoolInfo : parameters of fix size memory pool information.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_GetFixSizeMemoryPoolInfo(MI_S32 s32PoolId, MI_OS_MemoryPoolInfo_t *pstMemoryPoolInfo);

//------------------------------------------------------------------------------
/// @brief Allocate a memory block from the fixed-size memory pool, eCos support only.
/// @param[in] s32PoolId Memory pool ID
/// @return NULL: not enough available memory
/// @return Otherwise: pointer to the allocated memory block
//------------------------------------------------------------------------------
void* MI_OS_AllocateFixSizeMemory(MI_S32 s32PoolId);

//------------------------------------------------------------------------------
/// @brief Free a memory block from the fixed-size memory pool, eCos support only.
/// @param[in] pAddress : pointer to previously allocated memory block
/// @param[in] s32PoolId : Memory pool ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_BOOL MI_OS_FreeFixSizeMemory(void* pAddr, MI_S32 s32PoolId);

//------------------------------------------------------------------------------
/// @brief Get all task ID, eCos support only.
/// @param[out] pu32TaskNum : all task number
/// @param[out] ps32TaskIDList : the memory for the all task ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_GetTaskList(MI_U32 *pu32TaskNum, MI_S32 *ps32TaskIDList);

//------------------------------------------------------------------------------
/// @brief Create a task
/// @param[in] pstCreateTaskParams : create task parameters.
/// @param[Out] ps32TaskId : assigned Task ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_CreateTask(MI_OS_CreateTaskParams_t *pstCreateTaskParams, MI_S32 *ps32TaskId);

//------------------------------------------------------------------------------
/// @brief  Delete a previously created task
/// @param[in] s32TaskId : task ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DeleteTask(MI_S32 s32TaskId);

//------------------------------------------------------------------------------
/// @brief  Yield the execution right to ready tasks with "the same" priority
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_YieldTask (void);

//------------------------------------------------------------------------------
/// @brief  Suspend the calling task for u32Ms milliseconds
/// @param[in] u32Ms : delay 1 ~ MI_OS_WAIT_FOREVER ms
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DelayTask(MI_U32 u32Ms);

//------------------------------------------------------------------------------
/// @brief  Delay for u32Us microseconds
/// @param[in] u32Us : delay 0 ~ 999 us
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DelayTaskUs(MI_U32 u32Us);

//------------------------------------------------------------------------------
/// @brief  Delay Poll for u32Us microseconds
/// @param[in] u32Us : delay 0 ~ 999 us
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
/// @note   implemented by "busy waiting". Plz call MI_OS_DelayTask directly for ms-order delay
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DelayTaskUs_Poll(MI_U32 u32Us);

//------------------------------------------------------------------------------
/// @brief  Resume the specified suspended task
/// @param[in] s32TaskId : Task ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
/// @note   This API is not supported in Linux
//------------------------------------------------------------------------------
MI_RESULT MI_OS_ResumeTask(MI_S32 s32TaskId);

//------------------------------------------------------------------------------
/// @brief  Suspend the specified task
/// @param[in] s32TaskId : Task ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
/// @note   This API is not supported in Linux
//------------------------------------------------------------------------------
MI_RESULT MI_OS_SuspendTask(MI_S32 s32TaskId);

//------------------------------------------------------------------------------
/// @brief Get Task information
/// @param[in] s32TaskId : Task ID
/// @param[out] pstTaskInfoParams : Get task information paramenter.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_GetTaskInfo(MI_S32 s32TaskId, MI_OS_TaskInfoParams_s *pstTaskInfoParams);

//------------------------------------------------------------------------------
/// @brief  Get current task ID
/// @param[Out] ps32TaskId : current Task ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_GetTaskId(MI_S32 *ps32TaskId);

//------------------------------------------------------------------------------
/// @brief  Get thread ID of current thread/process in Linux
/// @param[Out] ps32TaskId : current Task ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_GetOSThreadId(MI_S32 *ps32TaskId);

//------------------------------------------------------------------------------
/// @brief  Create a mutex in the unlocked state
/// @param[in] pstCreateMutexParams : mutex create parameters.
/// @param[Out] ps32MutexId : assigned mutex ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
/// @note   A mutex has the concept of an owner, whereas a semaphore does not.
///         A mutex provides priority inheritance protocol against proiorty inversion,
///         whereas a binary semaphore does not.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_CreateMutex(MI_OS_CreateMutexParams_t *pstCreateMutexParams, MI_S32 *ps32MutexId);

//------------------------------------------------------------------------------
/// @brief  Delete the specified mutex
/// @param[in] s32MutexId : mutex ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
/// @note   It is important that the mutex be in the unlocked state when it is
///            destroyed, or else the behavior is undefined.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DeleteMutex(MI_S32 s32MutexId);

//------------------------------------------------------------------------------
/// @brief  Attempt to lock a mutex
/// @param[in] s32MutexId : mutex ID
/// @param[in] u32WaitMs : 0 ~ MI_OS_WAIT_FOREVER: suspend time (ms) if the mutex is locked
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_ObtainMutex(MI_S32 s32MutexId, MI_U32 u32WaitMs);

//------------------------------------------------------------------------------
/// @brief  Attempt to unlock a mutex
/// @param[in] s32MutexId : mutex ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_ReleaseMutex(MI_S32 s32MutexId);

//------------------------------------------------------------------------------
/// @brief  Get a mutex informaton
/// @param[in] s32MutexId : mutex ID
/// @param[in] pstMutexInfo : ptr to info of muxte parameters structure.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_GetMutexInfo(MI_S32 s32MutexId, MI_OS_MutexInfo_t *pstMutexInfo);

//------------------------------------------------------------------------------
/// @brief  Create a semaphore
/// @param[in] pstCreateSemParams : create semaphore parameters.
/// @param[out] ps32SemaphoreId : assigned Semaphore ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_CreateSemaphore(MI_OS_CreateSemParams_t *pstCreateSemParams, MI_S32 *ps32SemaphoreId);

//------------------------------------------------------------------------------
/// @brief  Delete the specified semaphore
/// @param[in] s32SemaphoreId : semaphore ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
/// @note   It is important that there are not any threads waiting on the semaphore
///             when this function is called or the behavior is undefined.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DeleteSemaphore(MI_S32 s32SemaphoreId);

//------------------------------------------------------------------------------
/// @brief  Attempt to decrement a semaphore count
/// @param[in] s32SemaphoreId : semaphore ID
/// @param[in] u32WaitMs :  0 ~ MI_OS_WAIT_FOREVER: suspend time (ms) if the semaphore count = 0
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_ObtainSemaphore(MI_S32 s32SemaphoreId, MI_U32 u32WaitMs);

//------------------------------------------------------------------------------
/// @brief  Increase a semaphore count
/// @param[in] s32SemaphoreId : semaphore ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
/// @note   It's possible for any thread to increase the semaphore count
//------------------------------------------------------------------------------
MI_RESULT MI_OS_ReleaseSemaphore(MI_S32 s32SemaphoreId);

//------------------------------------------------------------------------------
/// @brief  Get a semaphore informaton
/// @param[in] s32SemaphoreId : semaphore ID
/// @param[in] pstSemInfo : semaphore information parameters structure.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_GetSemaphoreInfo(MI_S32 s32SemaphoreId, MI_OS_SemInfo_t *pstSemInfo);

//------------------------------------------------------------------------------
/// @brief  Create an event group
/// @param[in] pName : ptr to event group name
/// @param[out] ps32EventGroupId : assigned Event ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_CreateEventGroup(MI_U8 *pu8Name, MI_S32 *ps32EventGroupId);

//------------------------------------------------------------------------------
/// @brief  Delete the event group
/// @param[in] s32EventGroupId : event group ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
/// @note event group that are being waited on must not be deleted
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DeleteEventGroup(MI_S32 s32EventGroupId);

//------------------------------------------------------------------------------
/// @brief Set the event flag (bitwise OR w/ current value) in the specified event group
/// @param[in] s32EventGroupId : event group ID
/// @param[in] u32EventFlag :event flag value
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
/// @note event group that are being waited on must not be deleted
//------------------------------------------------------------------------------
MI_RESULT MI_OS_SetEvent(MI_S32 s32EventGroupId, MI_U32 u32EventFlag);

//------------------------------------------------------------------------------
/// @brief Clear the specified event flag (bitwise XOR operation) in the specified event group
/// @param[in] s32EventGroupId : event group ID
/// @param[in] u32EventFlag :event flag value
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_ClearEvent(MI_S32 s32EventGroupId, MI_U32 u32EventFlag);

//------------------------------------------------------------------------------
/// @brief Wait for the specified event flag combination from the event group
/// @param[in] pstWaitEventParams : wait evet parameters
/// @param[in] pu32RetrievedEventFlag :etrieved event flag value
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_WaitEvent(MI_OS_WaitEventParams_t *pstWaitEventParams, MI_U32 *pu32RetrievedEventFlag);

//------------------------------------------------------------------------------
/// @brief Create a Timer
/// @param[in] pstCreateTimerParams : create timer parameters.
/// @param[out] ps32TimerId :assigned Timer ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_CreateTimer(MI_OS_CreateTimerParams_t *pstCreateTimerParams, MI_S32 *ps32TimerId);

//------------------------------------------------------------------------------
/// @brief Delete the Timer
/// @param[in] s32TimerId : Timer ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DeleteTimer(MI_S32 s32TimerId);

//------------------------------------------------------------------------------
/// @brief Start the Timer
/// @param[in] s32TimerId : Timer ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_StartTimer(MI_S32 s32TimerId);

//------------------------------------------------------------------------------
/// @brief Stop the Timer
/// @param[in] s32TimerId : Timer ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_StopTimer(MI_S32 s32TimerId);

//------------------------------------------------------------------------------
/// @brief Reset a Timer & reset the expiration periods
/// @param[in] pstResetTimerParams : reset timer parameter
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_ResetTimer(MI_OS_ResetTimerParams_t *pstResetTimerParams);

//------------------------------------------------------------------------------
/// @brief Get current system time in ms
/// @param[out] pu32SystemTime : system time in ms
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_GetSystemTime(MI_U32 *pu32SystemTime);

//------------------------------------------------------------------------------
/// @brief Time difference between current timer and task time[OBSOLETE]
/// @param[in] s32TimerCurrent : current timer in ms
/// @param[out] ps32DiffMs : system time diff in ms
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_TimerDiffTimeFromNow(MI_U32 s32TimerCurrent, MI_U32 *ps32DiffMs);

//------------------------------------------------------------------------------
/// @brief Time difference between setting time and task time[OBSOLETE]
/// @param[in] u32Timer : timer time in ms
/// @param[in] u32TaskTimer : current time in ms
/// @param[out] pu32DiffTime : system time diff in ms
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_TimerDiffTime(MI_U32 u32Timer, MI_U32 u32TaskTimer, MI_U32 *pu32DiffTime);

//------------------------------------------------------------------------------
/// @brief Create a Queue
/// @param[in] pstCreateQueueParams : create queue pareameter
/// @param[out] ps32QueueId : assigned message queue ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_CreateQueue(MI_OS_CreateQueueParams_t *pstCreateQueueParams, MI_S32 *ps32QueueId);

//------------------------------------------------------------------------------
/// @brief Delete the Queue
/// @param[in] s32QueueId : Queue ID
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DeleteQueue(MI_S32 s32QueueId);

//------------------------------------------------------------------------------
/// @brief Send a message to the end of the specified queue
/// @param[in] pstSendToQueueParams : send queue parameters structure
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_SendToQueue(MI_OS_SendToQueueParams_t *pstSendToQueueParams);

//------------------------------------------------------------------------------
/// @brief Receive a message from the specified queue
/// @param[in] pstRecvFromQueueParams : receive queue parameters structure
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_RecvFromQueue(MI_OS_RecvFromQueueParams_t *pstRecvFromQueueParams);

//------------------------------------------------------------------------------
/// @brief Peek a message from the specified queue
/// @param[in] pstRecvFromQueueParams : Peek queue parameters structure
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_PeekFromQueue(MI_OS_PeekFromQueueParams_t *pstPeekFromQueueParams);

//------------------------------------------------------------------------------
/// @brief clear all message in specified queue
/// @param[in] pstClearQueueParams : clear queue parameters structure
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_ClearQueue(MI_OS_ClearQueueParams_t *pstClearQueueParams);

//------------------------------------------------------------------------------
/// @brief Attach the interrupt callback function to interrupt
/// @param[in] eIntNum : Interrupt number in enumerator InterruptNum
/// @param[in] pIntCb : Interrupt callback function
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_AttachInterrupt(MI_OS_InterruptNum_e eIntNum, MI_OS_InterruptCb pIntCb);

//------------------------------------------------------------------------------
/// @brief Detach the interrupt callback function from interrupt
/// @param[in] eIntNum : Interrupt number in enumerator InterruptNum
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DetachInterrupt(MI_OS_InterruptNum_e eIntNum);

//------------------------------------------------------------------------------
/// @brief Enable (unmask) the interrupt
/// @param[in] eIntNum : Interrupt number in enumerator InterruptNum
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_EnableInterrupt(MI_OS_InterruptNum_e eIntNum);

//------------------------------------------------------------------------------
/// @brief Debug the interrupt
/// @param[in] eIntNum : Interrupt number in enumerator InterruptNum
/// @param[in] eIrqDebugOpt : Debug option
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DebugInterrupt(MI_OS_InterruptNum_e eIntNum, MI_OS_IrqDebugOpt_e eIrqDebugOpt);

//------------------------------------------------------------------------------
/// @brief Disable (mask) the interrupt #
/// @param[in] eIntNum : Interrupt number in enumerator InterruptNum
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DisableInterrupt(MI_OS_InterruptNum_e eIntNum);

//------------------------------------------------------------------------------
/// @brief Notify the interrupt complete
/// @param[in] eIntNum : Interrupt number in enumerator InterruptNum
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_CompleteInterrupt(MI_OS_InterruptNum_e eIntNum);

//------------------------------------------------------------------------------
/// @brief In interrupt Context or not
/// @param[Out] pbInISR : ture-> YES, False->NO
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_InInterrupt(MI_BOOL *pbInISR);

//------------------------------------------------------------------------------
/// @brief Disable all interrupts (including timer interrupt), the scheduler is disab
/// @param[out] pu32InterruptsStatus :Interrupt register value before all interrupts disable
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DisableAllInterrupts(MI_U32 *pu32InterruptsStatus);

//------------------------------------------------------------------------------
/// @brief Restore the interrupts from last MI_OS_DisableAllInterrupts.
/// @param[in] u32OldInterrupts : Interrupt register value from @ref MI_OS_DisableAllInterrupts
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_RestoreAllInterrupts(MI_U32 u32OldInterrupts);

//------------------------------------------------------------------------------
/// @brief Enable all CPU interrupts.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_EnableAllInterrupts(void);

//------------------------------------------------------------------------------
/// @brief Write back if dirty & Invalidate the cache lines in the given range.
/// @param[in] u32Start : start address (must be 16-B aligned and in cacheable area)
/// @param[in] u32Size : size (must be 16-B aligned)
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_DcacheFlush(MI_U32 u32Start, MI_U32 u32Size);

//------------------------------------------------------------------------------
/// @brief Check the VA is belong to cache pool or not.
/// @param[in] virtStartAddress : The Va will be checked.
/// @param[out] pbIsCache : Indicate The VA is belong to cache or not.
/// @return MI_OK: Process success.
/// @return MI_ERR_INVALID_PARAMETER: The virtStartAddress isn't belong to MPool address.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_IsCacheAddr(void *virtStartAddress, MI_BOOL *pbIsCache);

//------------------------------------------------------------------------------
/// @brief conver virutal address to physical address.
/// @param[in] u32VaAddr : virutal address
/// @param[out] pu32PaAddr : physial address
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_VA2PA(MI_U32 u32VaAddr, MI_U32 *pu32PaAddr);

//------------------------------------------------------------------------------
/// @brief conver physical address to convert to cached virtual address
/// @param[in] u32PaAddr : physial address
/// @param[out] pu32Kseg0Addr : cached virtual address
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_PA2KSEG0(MI_U32 u32PaAddr, MI_U32 *pu32Kseg0Addr);

//------------------------------------------------------------------------------
/// @brief conver physical address to convert to Non cached virtual address
/// @param[in] u32PaAddr : physial address
/// @param[out] pu32Kseg0Addr :  Non cached virtual address
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_PA2KSEG1(MI_U32 u32PaAddr, MI_U32 *pu32Kseg1Addr);

//------------------------------------------------------------------------------
/// @brief conver physical address to convert to cached virtual address
/// @param[in] u32PaAddr : physial address
/// @param[out] pu32Kseg0Addr : cached virtual address
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_PA2CachedVA(MI_U32 u32PaAddr, MI_U32 *pu32CachedAddr);

//------------------------------------------------------------------------------
/// @brief conver physical address to convert to Non cached virtual address
/// @param[in] u32PaAddr : physial address
/// @param[out] pu32Kseg0Addr :  Non cached virtual address
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_PA2NonCachedVA(MI_U32 u32PaAddr, MI_U32 *pu32NonCachedAddr);

//------------------------------------------------------------------------------
/// @brief Get OS capability
/// @param[out] pstOsCaps : Get OS Capability parameter.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_GetCaps(MI_OS_Caps_t *pstOsCaps);

//------------------------------------------------------------------------------
/// @brief Set OS debug level.
/// @param[in] eDbgLevel.
/// @return MI_OK: Set debug level success.
//------------------------------------------------------------------------------
MI_RESULT MI_OS_SetDebugLevel(MI_DBG_LEVEL_e eDbgLevel);

//------------------------------------------------------------------------------
/// @brief Sync memory from pipe to dram
/// @param[in] None
/// @return None
//------------------------------------------------------------------------------
void MI_OS_Sync(void);

#ifdef __cplusplus
}
#endif

#endif///_MI_OS_H_

