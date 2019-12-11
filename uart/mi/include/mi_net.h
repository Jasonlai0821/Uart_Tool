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

#ifndef _MI_NET_H_
#define _MI_NET_H_

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
//  Defines
//-------------------------------------------------------------------------------------------------
#define MI_NET_MAC_LENGTH 20
#define MI_MAX_INTERFACE_NAME 16
#define MI_MAX_NET_INTERFACE 20


//-------------------------------------------------------------------------------------------------
//  Structures
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_MI_NET_NIC_CONFIG_UNKNOW = 0,
    E_MI_NET_AUTO_CONFIG,
    E_MI_NET_10BASE_HALF_DUMPLEX,
    E_MI_NET_10BASE_FULL_DUMPLEX,
    E_MI_NET_100BASE_HALF_DUMPLEX,
    E_MI_NET_100BASE_FULL_DUMPLEX
} MI_NET_NicMode_e;

typedef enum
{
    E_MI_NET_DEVICE_STATUS_DOWN = 0,                //device down
    E_MI_NET_DEVICE_STATUS_UP,                       //device up
} MI_NET_DeviceStatus_e;

typedef enum
{
    E_MI_NET_PHYSICAL_LINK_STATUS_DISCONNECT = 0, //physical layer not connect: ex.RJ-45 plug-out
    E_MI_NET_PHYSICAL_LINK_STATUS_CONNECT,        //RJ-45 plug-in
} MI_NET_PhysicalLinkStatus_e;

typedef struct MI_NET_Status_s
{
    MI_NET_DeviceStatus_e eDevStatus;
    MI_NET_PhysicalLinkStatus_e ePhyLinkStatus;
} MI_NET_Status_t;

typedef struct MI_NET_PingParam_s
{
    MI_U8 aszHostIp[32];
    MI_U32 u32PingTimes;
    MI_U32 u32MsTimeout;
    MI_U16 u16DataBlockSize;
} MI_NET_PingParam_t;

typedef struct MI_NET_PingResult_s
{
    MI_U32 u32SuccessCount;
    MI_U32 u32FailCount;
    MI_U32 u32MsMinTime;
    MI_U32 u32MsMaxTime;
    MI_U32 u32MsAvgTime;
} MI_NET_PingResult_t;

typedef struct MI_NET_InitParam_s
{
    MI_U32 u32Reserved;
} MI_NET_InitParam_t;

typedef struct MI_NET_OpenParam_s
{
    MI_U8 szInterfaceName[MI_MAX_INTERFACE_NAME];
} MI_NET_OpenParam_t;

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief Init NET module.
/// @param[in] pstInitParam..
/// @return MI_OK: Process success.
/// @return MI_HAS_INITED: UART module had inited.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_Init(MI_NET_InitParam_t *pstInitParam);

//------------------------------------------------------------------------------
/// @brief Finalize NET module.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_NET_DeInit(void);

//------------------------------------------------------------------------------
/// @brief Open the NET handle.
/// @param[in] pstOpenParam. Parameter to open uart
/// @param[out] phNet. return net handle after opening success
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_RESOURCES: No available resource.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_NET_OpenHandle(MI_NET_OpenParam_t *pstOpenParam, MI_HANDLE *phNet);

//------------------------------------------------------------------------------
/// @brief Close a NET handle.
/// @param[in] hNet. Net handle to close
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_CloseHandle(MI_HANDLE hNet);

//------------------------------------------------------------------------------
/// @brief Ping a target server by IP
/// @param[in] hNet. Net handle to process
/// @param[in] stPingParam. ping parameters to set
/// @param[out] pstPingResult. ping result
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_Ping(MI_HANDLE hNet, MI_NET_PingParam_t stPingParam, MI_NET_PingResult_t *pstPingResult);

//------------------------------------------------------------------------------
/// @brief Network Device Up
/// @param[in] hNet. Net handle to process
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_DeviceUp(MI_HANDLE hNet);

//------------------------------------------------------------------------------
/// @brief Network Device Down
/// @param[in] hNet. Net handle to process
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_DeviceDown(MI_HANDLE hNet);

//------------------------------------------------------------------------------
/// @brief Use Network DHCP to get IP
/// @param[in] hNet. Net handle to process
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_DhcpOn(MI_HANDLE hNet);

//------------------------------------------------------------------------------
/// @brief Destroy Network DHCP process
/// @param[in] hNet. Net handle to process
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_DhcpOff(MI_HANDLE hNet);

//------------------------------------------------------------------------------
/// @brief Set Mac Address to Device
/// @param[in] hNet. Net handle to process
/// @param[in] pszMac. Mac address to set
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_SetMac(MI_HANDLE hNet, MI_U8 *pszMac);

//------------------------------------------------------------------------------
/// @brief Set Network DNS address
/// @param[in] bCover. True: Cover dns, False: Add to last
/// @param[in] pszDns. pointer to get dns address
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_SetDns(MI_BOOL bCover, MI_U8 *pu8Dns);

//------------------------------------------------------------------------------
/// @brief Set Network IP Address
/// @param[in] hNet. Net handle to process
/// @param[in] pszIpAddr. the ip address to be set
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_SetIpAddr(MI_HANDLE hNet, MI_U8 *pszIpAddr);

//------------------------------------------------------------------------------
/// @brief Set Network Netmask
/// @param[in] hNet. Net handle to process
/// @param[in] netmask. the netmask address to be set
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_SetNetMask(MI_HANDLE hNet, MI_U8 *pszNetMask);

//------------------------------------------------------------------------------
/// @brief Set Network Gateway
/// @param[in] hNet. Net handle to process
/// @param[in] pszGateway. the gateway address to be set
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_SetGateway(MI_HANDLE hNet, MI_U8 *pszGateway);

//------------------------------------------------------------------------------
/// @brief Get Network Connect Status
/// @param[in] hNet. Net handle to process
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_GetConnectStatus(MI_HANDLE hNet, MI_NET_Status_t *pstStatus);

//------------------------------------------------------------------------------
/// @brief Get Network MAC address
/// @param[in] hNet. Net handle to process
/// @param[out] pszMac. pointer to get mac address
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_GetMac(MI_HANDLE hNet, MI_U8 *pszMac);

//------------------------------------------------------------------------------
/// @brief Get Network IP address
/// @param[in] hNet. Net handle to process
/// @param[out] pszIpAddr. pointer to get ip address
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_GetIpAddr(MI_HANDLE hNet, MI_U8 *pszIpAddr);

//------------------------------------------------------------------------------
/// @brief Get Network netmask address
/// @param[in] hNet. Net handle to process
/// @param[out] pszNetmask. pointer to get mask address
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_GetNetMask(MI_HANDLE hNet, MI_U8 *pszNetmask);

//------------------------------------------------------------------------------
/// @brief Get Network geteway address
/// @param[in] hNet. Net handle to process
/// @param[out] pszGateway. pointer to get gateway address
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_GetGateWay(MI_HANDLE hNet, MI_U8 *pszGateway);

//------------------------------------------------------------------------------
/// @brief Get Network Nic Mode
/// @param[in] hNet. Net handle to process
/// @param[out] peNicMode. pointer to get the module info
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_HANDLE: Invalid handle.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_GetNicModeInfo(MI_HANDLE hNet, MI_NET_NicMode_e *peNicMode);

//------------------------------------------------------------------------------
/// @brief Get Network Interface Name
/// @param[out] pszInterfaceName. pointer to get the interface name
/// @param[out] pu8InterfaceCount. pointer to get the total interface
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_GetInterfaceName(MI_U8 pszInterfaceName[MI_MAX_NET_INTERFACE][MI_MAX_INTERFACE_NAME], MI_U8 *pu8InterfaceCount);

//------------------------------------------------------------------------------
/// @brief Get Network DNS address
/// @param[out] pszDns. pointer to get dns address
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not inted.
/// @return MI_ERR_INVALID_PARAMETER: parameter error.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_GetDns(MI_U8 *pszDns);

//------------------------------------------------------------------------------
/// @brief Set Network module debug level
/// @param[in] interface_name.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_NET_SetDebugLevel(MI_DBG_LEVEL_e eDgbLevel);

#ifdef __cplusplus
}
#endif

#endif///_MI_NET_H_

