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
/// @file   MI_WLAN.h
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MI_WLAN_H_
#define _MI_WLAN_H_

#include "mi_net.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MI_MAX_INTERFACE_NAME 16

/// WLAN max MAC len
#define MI_WLAN_MAX_MAC_LEN 32
/// WLAN max SSID len
#define MI_WLAN_MAX_SSID_LEN 32
/// WLAN max password len
#define MI_WLAN_MAX_PASSWD_LEN 40
/// WLAN max AP number
#define MI_WLAN_MAX_APINFO_NUM 64
/// WLAN max path len
#define MI_WLAN_MAX_FOLDERPATH_LEN 256
///WLAN Status
typedef enum
{
    /// WLAN empty status
    E_MI_WLAN_STATUS_EMPTY        = 0,
    /// WLAN inited status
    E_MI_WLAN_STATUS_INITED       = 1,
    /// WLAN opened status
    E_MI_WLAN_STATUS_OPENED       = 2,
    /// WLAN scanning status
    E_MI_WLAN_STATUS_SCANNING     = 3,
    /// WLAN connecting status
    E_MI_WLAN_STATUS_CONNECTING   = 4,
    /// WLAN running status
    E_MI_WLAN_STATUS_CONNECTED    = 5,
    /// WLAN max status
    E_MI_WLAN_STATUS_MAX          = 0xff,
} MI_WLAN_Status_e;

typedef enum
{
    /// WLAN module MT7601U
    E_MI_WLAN_MODULE_MT7601U = 0,
    /// WLAN module 8188ETV
    E_MI_WLAN_MODULE_8188ETV,
    /// WLAN module RT5572
    E_MI_WLAN_MODULE_RT5572,
    /// WLAN module AUTO
    E_MI_WLAN_MODULE_AUTO,
    /// WLAN module AUTO AP mode
    E_MI_WLAN_MODULE_AUTOAP,
    /// WLAN module max
    E_MI_WLAN_MODULE_MAX
} MI_WLAN_Module_e;


typedef enum
{
    /// WLAN dongle in
    E_MI_WLAN_EVENT_IN = 0,
    /// WLAN dongle out
    E_MI_WLAN_EVENT_OUT,
    /// WLAN dongle max
    E_MI_WLAN_EVENT_MAX
} MI_WLAN_Event_e;

typedef enum
{
    /// WLAN module security key off
    E_MI_WLAN_SECURITY_NONE       = 0,
    /// WLAN module security key unknow
    E_MI_WLAN_SECURITY_UNKNOWTYPE = 1,
    /// WLAN module security key WEP
    E_MI_WLAN_SECURITY_WEP        = 2,
    /// WLAN module security key WPA
    E_MI_WLAN_SECURITY_WPA        = 3,
    /// WLAN module security key WPA2
    E_MI_WLAN_SECURITY_WPA2       = 4,
    /// WLAN module max
    E_MI_WLAN_SECURITY_MAX        = 0xff,
} MI_WLAN_Security_e;

typedef enum
{
    /// WLAN module encrypt type none
    E_MI_WLAN_ENCRYPT_NONE        = 0,
    /// WLAN module encrypt type unknown
    E_MI_WLAN_ENCRYPT_UNKNOWN     = 1,
    /// WLAN module encrypt type WEP
    E_MI_WLAN_ENCRYPT_WEP         = 2,
    /// WLAN module encrypt type TKIP
    E_MI_WLAN_ENCRYPT_TKIP        = 3,
    /// WLAN module encrypt type AES
    E_MI_WLAN_ENCRYPT_AES         = 4,
    /// WLAN module max
    E_MI_WLAN_ENCRYPT_MAX        = 0xff,
} MI_WLAN_Encrypt_e;

/// Parameter type
typedef enum
{
    /// WLAN get status (enable or disable),parameter type is a pointer to MI_BOOL
    E_MI_WLAN_PARAMTYPE_CONNECTSTATUS,
    /// WLAN get access point info by index, parameter type is a pointer to MI_WLAN_QueryAPInfo_t
    E_MI_WLAN_PARAMTYPE_APINFO_BYINDEX,
    /// WLAN get interface name, parameter type is a pointer to MI_U8 array, size should be >= MI_MAX_INTERFACE_NAME
    E_MI_WLAN_PARAMTYPE_INTERFACENAME,
    /// WLAN set bridge enable/disable, parameter type is a pointer to MI_WLAN_BridgeSetting_t
    E_MI_WLAN_PARAMTYPE_BRIDGE_SETTING,
    /// WLAN param max
    E_MI_WLAN_PARAMTYPE_MAX
} MI_WLAN_ParamType_e;

typedef enum
{
    /// WLAN network infrastructure type
    E_MI_WLAN_NETWORKTYPE_INFRA,
    /// WLAN network AP type
    E_MI_WLAN_NETWORKTYPE_AP,
    /// WLAN network AdHoc type
    E_MI_WLAN_NETWORKTYPE_ADHOC,
    /// WLAN network Monitor type
    E_MI_WLAN_NETWORKTYPE_MONITOR,
    /// WLAN param max
    E_MI_WLAN_NETWORKTYPE_MAX
} MI_WLAN_NetworkType_e;

typedef enum
{
    /// WPA open type
    E_MI_WLAN_SOFTAP_WPATYPE_OPEN = 0,
    /// WPA WPA type
    E_MI_WLAN_SOFTAP_WPATYPE_WPA = 1,
    /// WPA WPA2 type
    E_MI_WLAN_SOFTAP_WPATYPE_WPA2 = 2
} MI_WLAN_SOFTAP_WPAType_e;

typedef void (*MI_WLAN_Callback)(MI_WLAN_Event_e eEvent, MI_WLAN_Module_e eModule);

/// WLAN init parameter
typedef struct MI_WLAN_InitParams_s
{
    /// WLAN module
    MI_WLAN_Module_e eModule;
    /// WLAN reserved for CUS dongle
    MI_U8 au8WLanFolderPath[MI_WLAN_MAX_FOLDERPATH_LEN];
    /// WLAN Hotplug event callback function pointer
    MI_WLAN_Callback pfEventCallback;
} MI_WLAN_InitParams_t;

/// WLAN open parameter
typedef struct MI_WLAN_OpenParam_s
{
    // reserved
    MI_BOOL bReserved;
} MI_WLAN_OpenParams_t;

/// WLAN connect info
typedef struct MI_WLAN_ConnectParam_s
{
    // WLAN network type, currenty only supprot infra mode
    MI_WLAN_NetworkType_e eNetworkType;
    // WLan security mode
    MI_WLAN_Security_e eSecurity;
    // WLan SSID
    MI_U8 au8SSId[MI_WLAN_MAX_SSID_LEN];
    // WLan password
    MI_U8 au8Password[MI_WLAN_MAX_PASSWD_LEN];
} MI_WLAN_ConnectParam_t;

/// WLAN ap info
typedef struct MI_WLAN_APInfo_s
{
    // WLAN Security mode
    MI_WLAN_Security_e eSecurity;
    // WLAN Frequency GHz
    MI_FLOAT fFrequency;
    // WLAN Bitrate Mb/s
    MI_FLOAT fBitRate;
    // WLAN Quality %
    MI_U32 u32Quality;
    // WLAN SSID
    MI_U8 au8SSId[MI_WLAN_MAX_SSID_LEN];
    // WLAN Channel
    MI_U8 u8Channel;
    // WLAN MAC
    MI_U8 au8Mac[MI_WLAN_MAX_MAC_LEN];
    // WLAN Encryption type
    MI_WLAN_Encrypt_e eEncrypt;
    // WLAN AP type (Infrastructure / Ad-Hoc)
    MI_WLAN_NetworkType_e eMode;
    // WLAN Strength %
    MI_U32 u32Strehgth;
    // WLAN Support PSK
    MI_BOOL bSupportPSK;
} MI_WLAN_APInfo_t;

/// WLAN ap info
typedef struct MI_WLAN_ScanParam_s
{
    // WLan set block mode
    MI_BOOL bBlock; //reserved
} MI_WLAN_ScanParam_t;

/// WLAN ap info
typedef struct MI_WLAN_ScanResult_s
{
    // WLan AP number
    MI_U8 u8APNumber;
} MI_WLAN_ScanResult_t;

typedef struct MI_WLAN_QueryAPInfo_s
{
    // WLan AP info
    MI_WLAN_APInfo_t stAPInfo; //output
    // WLan AP index
    MI_U8 u8Idx;    //input
} MI_WLAN_QueryAPInfo_t;

/// WLAN dongle device info
typedef struct MI_WLAN_DeviceInfo_s
{
    // WLan dongle vendor ID
    MI_U16 u16IdVendor;
    // WLan dongle product ID
    MI_U16 u16IdProduct;
    // WLan dongle init script path
    MI_U8 au8ScriptPath[MI_WLAN_MAX_FOLDERPATH_LEN];
    // WLan dongle interface name
    MI_U8 au8InterfaceName[MI_MAX_INTERFACE_NAME];
} MI_WLAN_DeviceInfo_t;

typedef struct MI_WLAN_BridgeSetting_s
{
    // WLan enable bridge function
    MI_BOOL bEnable; //input
    // WLan STA(e.g. apcli0) or ethernet(e.g. eth0) interface name (if length = 0, default as apcli0)
    MI_U8 au8InterfaceName[MI_MAX_INTERFACE_NAME];    //input
} MI_WLAN_BridgeSetting_t;

typedef struct MI_WLAN_Status_s
{
    MI_U8 bssid[20];
    MI_U8 freq[8];
    MI_U8 ssid[48];
    MI_U8 id[4];
    MI_U8 mode[8];
    MI_U8 key_mgmt[12];
    MI_U8 wpa_state[16];
    MI_U8 ip_address[16];
    MI_U8 address[20];
    MI_U32 channel;
    MI_U32 RSSI;
    MI_U8 Bandwidth[8];
}MI_WLAN_Status_t;

//------------------------------------------------------------------------------
/// @brief do wlan init , and change wlan status to E_MI_WLAN_STATUS_INITED
/// @param[in] *pstInitParams reserved
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: init fail.
/// @return MI_HAS_INITED: already inited.
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Init(MI_WLAN_InitParams_t *pstInitParams);

//------------------------------------------------------------------------------
/// @brief do wlan deinit , and change wlan status to E_MI_WLAN_STATUS_EMPTY
/// @param[in] none
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: deinit fail.
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_DeInit(void);

//------------------------------------------------------------------------------
/// @brief Open wlan handler, and change wlan status to E_MI_WLAN_STATUS_OPENED
/// @param[in] *pstParam memory pool id (cache & noncached)
/// @param[out] *phwlan wlan handler
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
/// @return MI_ERR_INVALID_HANDLE: Null handle
/// @return MI_ERR_INVALID_PARAMETER: Null parameter
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Open(MI_WLAN_OpenParams_t *pstParam, MI_HANDLE *phwlan);

//------------------------------------------------------------------------------
/// @brief close wlan handler, and change wlan status to E_MI_WLAN_STATUS_INITED
/// @param[in] hWLan wlan handle.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Close(MI_HANDLE hWLan);

//------------------------------------------------------------------------------
/// @brief add device information to support list
/// @param[in] pstDeviceInfo: A pointer to structure MI_WLAN_DeviceInfo_t for device information.
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not initialized.
/// @return MI_ERR_INVALID_PARAMETER: Parameters is invalid.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_AddDeviceInfo(MI_WLAN_DeviceInfo_t *pstDeviceInfo);

//------------------------------------------------------------------------------
/// @brief set wlan attr, current reserved
/// @param[in] hWLan wlan handle.
/// @param[in] eParamType attr type.
/// @param[in] *pUsrParam attr param.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_SetAttr(MI_HANDLE hWLan, MI_WLAN_ParamType_e eParamType, void *pUsrParam);

//------------------------------------------------------------------------------
/// @brief get wlan attr
/// @param[in] hWLan wlan handle.
/// @param[in] eParamType attr type.
/// @param[in] *pUsrParam attr param.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_GetAttr(MI_HANDLE hWLan, MI_WLAN_ParamType_e eParamType, void *pUsrParam);

//------------------------------------------------------------------------------
/// @brief connect, and change wlan status to E_MI_WLAN_STATUS_CONNECTING
/// @param[in] hWLan wlan handle.
/// @param[in] *pstConnectParam info param.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Connect(MI_HANDLE hWLan, MI_WLAN_ConnectParam_t *pstConnectParam);

//------------------------------------------------------------------------------
/// @brief disconnect wlan, and change wlan status to E_MI_WLAN_STATUS_OPENED
/// @param[in] hWLan wlan handle.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Disconnect(MI_HANDLE hWLan);

//------------------------------------------------------------------------------
/// @brief scan ap info, if use block mode, it will change to E_MI_WLAN_STATUS_SCANNING
/// @param[in] hWLan wlan handle.
/// @param[in] *pstParam info param. //reserved
/// @param[out] *pstResult result.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Scan(MI_HANDLE hWLan, MI_WLAN_ScanParam_t *pstParam, MI_WLAN_ScanResult_t *pstResult);

//------------------------------------------------------------------------------
/// @brief set wlan debug level
/// @param[in] eDgbLevel debug level.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_SetDebugLevel(MI_DBG_LEVEL_e eDgbLevel);

//------------------------------------------------------------------------------
/// @brief get current wlan status
/// @param[in] the struct to store wlan status
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_GetStatus(MI_WLAN_Status_t *status);

//------------------------------------------------------------------------------
/// @brief start AP
/// @param[in] mode AP's encrypt type
/// @param[in] ssid AP's SSID
/// @param[in] passwd AP's password
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_StartAP(MI_WLAN_SOFTAP_WPAType_e mode, MI_U8 ssid[], MI_U8 passwd[]);

//------------------------------------------------------------------------------
/// @brief close AP
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_CloseAP();

//------------------------------------------------------------------------------
/// @brief get wlan chip version
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_GetWlanChipVersion(MI_U8 *ChipVersion);

//------------------------------------------------------------------------------
/// @brief set Network Type
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_SetNetworkType(MI_WLAN_NetworkType_e eNetworkType);

//------------------------------------------------------------------------------
/// @brief set channe in monitor mode
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_SetChannel(MI_U8 ch);

//------------------------------------------------------------------------------
/// @brief dump tcp data
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_TcpDump(MI_U8 *cmd);

MI_RESULT DeviceUp(char * interface);

//------------------------------------------------------------------------------
/// @brief Detection of network connection status
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Detector(void);

//------------------------------------------------------------------------------
/// @brief Close Detector thread
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Close_Detector_Thread(void);

#ifdef __cplusplus
}
#endif

#endif
