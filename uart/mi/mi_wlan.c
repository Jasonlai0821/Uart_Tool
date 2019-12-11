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
/// @file   MI_TTX.c
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include "mi_common.h"
#include "mi_os.h"
#include "mi_net.h"
#include "mi_wlan.h"
#include "internal/mi_usbdevice_internal.h"
#include "mi_sys.h"
#include "internal/mi_sys_internal.h"
#include "internal/mi_net_internal.h"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/if.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
//------------------------------------------------DEBUG MACROS-------------------------------------------------
#define DBG_LEVEL                       _u8WLanDbgLevel

//---------------------------------------------------MACROS-------------------------------------------------------

#define MI_WLAN_SUPPORTED_DEVICE_NUM           8
#define MI_WLAN_SUPPORTED_PCI_DEVICE_NUM       1
#define MI_WLAN_SUPPORTED_CUSTOM_DEVICE_NUM    32
#define MI_WLAN_MT7601U_INTERFACE  "ra0"
#define MI_WLAN_MT7603U_INTERFACE  "ra0"
#define MI_WLAN_MT7615_INTERFACE   "ra0"
#define MI_WLAN_RT5572_INTERFACE   "ra0"
#define MI_WLAN_INTERFACE          "wlan0"

#define MI_WLAN_IWPRIV             "iwpriv"
#define MI_WLAN_IWCONFIG           "iwconfig"
#define MI_WLAN_BRCTL              "brctl"
#define MI_WLAN_ADDBR              "addbr"
#define MI_WLAN_DELBR              "delbr"
#define MI_WLAN_ADDIF              "addif"
#define MI_WLAN_BR0                "br0"
#define MI_WLAN_IWLIST             "iwlist"
#define MI_WLAN_SH                 "sh"
#define MI_WLAN_SET                "set"
#define MI_WLAN_IFCONFIG           "ifconfig"
#define MI_WLAN_UDHCPD             "udhcpd"
#define MI_WLAN_UDHCPC             "udhcpc"
#define MI_WLAN_TCPDUMP            "tcpdump"
#define MI_WLAN_LSPCI              "lspci"
#define MI_WLAN_WPACLI             "wpa_cli"
#define MI_WLAN_STATUS             "status"
#define MI_WLAN_APCLIENABLE        "ApCliEnable"
#define MI_WLAN_SITESURVEY         "SiteSurvey"
#define MI_WLAN_GETSITESURVEY      "get_site_survey"
#define MI_WLAN_CHANNEL            "Channel"
#define MI_WLAN_MODE               "mode"




#define MI_WLAN_NETWORKTYPE         "NetworkType"
#define MI_WLAN_NETWORKTYPE_INFRA   "infra"
#define MI_WLAN_NETWORKTYPE_AP      "ap"
#define MI_WLAN_NETWORKTYPE_ADHOC   "adhoc"
#define MI_WLAN_NETWORKTYPE_MONITOR "monitor"

#define MI_WLAN_AUTHMODE            "AuthMode"
#define MI_WLAN_AUTHMODE_OPEN       "OPEN"
#define MI_WLAN_AUTHMODE_SHARE      "SHARED"
#define MI_WLAN_AUTHMODE_WPAPSK     "WPAPSK"
#define MI_WLAN_AUTHMODE_WPA2PSK    "WPA2PSK"
#define MI_WLAN_WPAPSK              "WPAPSK"

#define MI_WLAN_ENCRYPTYPE          "EncrypType"
#define MI_WLAN_ENCRYPTYPE_NONE     "NONE"
#define MI_WLAN_ENCRYPTYPE_WEP      "WEP"
#define MI_WLAN_ENCRYPTYPE_AES      "AES"

#define MI_WLAN_SSID                "SSID"
#define MI_WLAN_APCLI0              "apcli0"
#define MI_WLAN_APCLISSID           "ApCliSsid"
#define MI_WLAN_APCLIAUTHMODE       "ApCliAuthMode"
#define MI_WLAN_APCLIENCRYPTYPE     "ApCliEncrypType"
#define MI_WLAN_APCLIWPAPSK         "ApCliWPAPSK"
#define MI_WLAN_INITSH              "Init.sh"
#define MI_WLAN_DEINITSH            "DeInit.sh"
#define MI_WLAN_INITAPSH            "InitAP.sh"
#define MI_WLAN_DEINITAPSH          "DeInitAP.sh"
#define MI_WLAN_CONNECTSH           "Connect.sh"
#define MI_WLAN_DISCONNECTSH        "Disconnect.sh"
#define MI_WLAN_CONNECTAPSH         "ConnectAP.sh"
#define MI_WLAN_DISCONNECTAPSH      "DisconnectAP.sh"
#define MI_WLAN_UDHCPDCONF          "udhcpd.conf"
#define MI_WLAN_DEFAULT_FOLDER      "/system/lib/modules/"
#define MI_WLAN_SYSTEM              "/system/"
#define MI_WLAN_TMP                 "/tmp/"
#define MI_WLAN_SOFTAPSH            "softap.sh"
#define MI_WLAN_CLOSEAP             "closeSoftAP.sh"
#define MI_WLAN_GETWLANCHIP         "getWlanChip.sh"
#define MI_WLAN_GETSTATUS           "getStatus.sh"
#define MI_WLAN_STATUS_FILE         "wlan_status.txt"
#define MI_WLAN_SCAN_RESULT         "scan_result.txt"
#define MI_WLAN_SCAN_CMD            "scanning"
#define MI_WLAN_SCAN_PARAM          " | awk '                                         \
    /Cell/    {print substr($1,1) \" \" substr($2,1) \" \" substr($5,1)};             \
    /ESSID/   {print substr($1,1) \" \" substr($2,1) \" \" substr($3,1)};             \
    /Mode:/   {print substr($1,1)};                                                   \
    /Freq/    {print substr($1,1)};                                                   \
    /Freq/    {print substr($3,2) \":\" substr($4,1,length($4)-1)};                   \
    /Qual/    {print substr($1,1) \" \" substr($3,7)};                                \
    /Encr/    {print substr($2,1)};                                                   \
    /IE/      {print substr($1,1)substr($2,1)substr($3,1)substr($4,1)substr($5,1)};   \
    /Group/   {print substr($1,1) \" \" substr($4,1)};                                \
    /PSK/     {print substr($5,1)};                                                   \
'"

#define MI_WLAN_LONG_CMD_LEN        1024
#define MI_WLAN_NORMAL_CMD_LEN      256
#define MI_WLAN_SHORT_CMD_LEN       128
#define MI_WLAN_SCAN_INTERVAL       3

#define MI_WLAN_FOLDER_MT7601U      "mt7601u/"
#define MI_WLAN_FOLDER_MT7603U      "MT7603U/"
#define MI_WLAN_FOLDER_MT7615       "MT7615/"
#define MI_WLAN_FOLDER_RT5572       "RT5572/"

#define MI_WLAN_TOKEN_CELL          "Cell"
#define MI_WLAN_TOKEN_ESSID         "ESSID"
#define MI_WLAN_TOKEN_FREQ          "Frequency"
#define MI_WLAN_TOKEN_CHANNEL       "Channel"
#define MI_WLAN_TOKEN_ENCRYPT       "Group"
#define MI_WLAN_TOKEN_ENCRYPT_AES   "CCMP"
#define MI_WLAN_TOKEN_ENCRYPT_TKIP  "TKIP"
#define MI_WLAN_TOKEN_ENCRYPT_WEP   "WEP-104"
#define MI_WLAN_TOKEN_PSK           "PSK"
#define MI_WLAN_TOKEN_MODE          "Mode"
#define MI_WLAN_TOKEN_MODE_INFRA    "Managed"
#define MI_WLAN_TOKEN_MODE_ADHOC    "Ad-Hoc"
#define MI_WLAN_TOKEN_QUALITY       "Quality"
#define MI_WLAN_TOKEN_KEY           "key"
#define MI_WLAN_TOKEN_ON            "on"
#define MI_WLAN_TOKEN_OFF           "off"
#define MI_WLAN_TOKEN_IE            "IE"
#define MI_WLAN_TOKEN_WPA1          "WPAVersion1"
#define MI_WLAN_TOKEN_WPA18960      "WPAVersion18960"
#define MI_WLAN_TOKEN_WPA2          "IEEE802.11i/WPA2Version1"
#define MI_WLAN_AP_DEFAULT_IP       "192.168.0.19"


#define LOCK_ENTER() \
    MI_ENTER(); \
    _MI_WLAN_Lock();\

#define UNLOCK_FAIL_RETURN(e) \
    if(e!=TRUE)\
    {\
        _MI_WLAN_UnLock(); \
        MI_EXIT_ERR("\t"); \
        return MI_ERR_FAILED;\
    }\

#define UNLOCK_RETURN(e) \
    _MI_WLAN_UnLock(); \
    if(e==TRUE)\
    {\
        MI_EXIT_OK(); \
        return MI_OK;\
    }\
    else\
    {\
        MI_EXIT_ERR("\t"); \
        return MI_ERR_FAILED;\
    }\

#define CHECK_PARAM(e) \
    if(e == NULL)   \
    {   \
        _MI_WLAN_UnLock(); \
        MI_EXIT_ERR("PARAM is Null!\n");    \
        return MI_ERR_INVALID_PARAMETER;   \
    }

#define CHECK_HANDLE(e) \
    if((e == MI_HANDLE_NULL)||   \
            (e != _hWLanHandle))  \
    {   \
        _MI_WLAN_UnLock(); \
        MI_EXIT_ERR("MI_ERR_INVALID_HANDLE!\n");    \
        return MI_ERR_INVALID_HANDLE;   \
    }

#define CHECK_REINIT()\
    if(_eWLanStatus == E_MI_WLAN_STATUS_INITED)\
    {\
        _MI_WLAN_UnLock(); \
        return MI_HAS_INITED;\
    }\

#define CHECK_IS_EMPTY()\
    if(_eWLanStatus != E_MI_WLAN_STATUS_EMPTY)\
    {\
        _MI_WLAN_UnLock(); \
        return MI_ERR_FAILED;\
    }\

#define CHECK_IS_INITED()\
    if(_eWLanStatus != E_MI_WLAN_STATUS_INITED) \
    {\
        _MI_WLAN_UnLock(); \
        MI_EXIT_ERR("\t"); \
        return MI_ERR_FAILED;\
    }\

#define CHECK_IS_OPENED()\
    if(_eWLanStatus != E_MI_WLAN_STATUS_OPENED) \
    {\
        _MI_WLAN_UnLock(); \
        MI_EXIT_ERR("\t"); \
        return MI_ERR_FAILED;\
    }\

#define CHECK_IS_CONNECTED()\
    if(_eWLanStatus != E_MI_WLAN_STATUS_CONNECTED) \
    {\
        _MI_WLAN_UnLock(); \
        MI_EXIT_ERR("\t"); \
        return MI_ERR_FAILED;\
    }\

#define CHECK_OPENED()\
    if(_eWLanStatus < E_MI_WLAN_STATUS_OPENED) \
    {\
        _MI_WLAN_UnLock(); \
        MI_EXIT_ERR("\t"); \
        return MI_ERR_FAILED;\
    }\
//----------------------------local struct here--------------------------------------------------------------
typedef struct
{
    MI_U16 u16IdVendor;
    MI_U16 u16IdProduct;
}MI_WLAN_PciDevice_t ;

//----------------------------local variable here------------------------------------------------------------
static MI_WLAN_Module_e _eModule                                = E_MI_WLAN_MODULE_MAX;
static MI_WLAN_Status_e _eWLanStatus                            = E_MI_WLAN_STATUS_EMPTY;
static MI_HANDLE _hWLanHandle                                   = MI_HANDLE_NULL;
static MI_S32 _s32WLanMutexId                                   = -1;


static MI_WLAN_Event_e  _eCallbackEvent                         = E_MI_WLAN_EVENT_MAX;
static MI_WLAN_Module_e _eCallbackModule                        = E_MI_WLAN_MODULE_MAX;
static MI_WLAN_Callback _pfHotplugCallback                      = NULL;
static MI_U16 _u16CustomDeviceNum                               = 0;
static MI_S32 _s32WLanCallbackTask                              = -1;
static MI_U8 _au8WLanCallbackTaskStack[4096]                    = {0};
static MI_U8 _au8Wirelessfolder[MI_WLAN_SHORT_CMD_LEN]          = {'\0'};

static MI_BOOL _bCtrlAPClient                                         = FALSE;
static MI_BOOL _bAPMode                                         = FALSE;
static MI_BOOL _bAPClient                                         = FALSE;
static MI_BOOL _bCustomerDevice                                 = TRUE;
static MI_U16  _u16CustomerIndex                                = 0;
static MI_WLAN_DeviceInfo_t _astCustomDeviceInfo[MI_WLAN_SUPPORTED_CUSTOM_DEVICE_NUM] = {{0}};

static MI_U8 _u8ssid[48]                                          = {0};
static MI_U8 _u8chipname[8]                                       = {0};
static MI_U8 HOSTAPD_CONF_FILE[]                                  = "/data/hostapd.conf";
static pthread_t detector_thread;

static mi_usbdevice_Descriptor_t _astDeviceDescriptor[MI_WLAN_SUPPORTED_DEVICE_NUM]=
{
    {0x148f, 0x7601, E_MI_USBDEVICE_CLASS_VENDOR_SPEC}, //MT7601U
    {0x0e8d, 0x7603, E_MI_USBDEVICE_CLASS_VENDOR_SPEC}, //MT7603U
    {0x148f, 0x5572, E_MI_USBDEVICE_CLASS_VENDOR_SPEC}, //RT5572
    {0x148f, 0x3070, E_MI_USBDEVICE_CLASS_VENDOR_SPEC}, //RT3070
    {0x148f, 0x3370, E_MI_USBDEVICE_CLASS_VENDOR_SPEC}, //RT3370
    {0x148f, 0x3572, E_MI_USBDEVICE_CLASS_VENDOR_SPEC}, //RT3572
    {0x148f, 0x5370, E_MI_USBDEVICE_CLASS_VENDOR_SPEC}, //RT5370
    {0x148f, 0x5372, E_MI_USBDEVICE_CLASS_VENDOR_SPEC}  //RT5372
};

static MI_WLAN_PciDevice_t _astPciDeviceDescriptor[MI_WLAN_SUPPORTED_PCI_DEVICE_NUM]=
{
    {0x14c3, 0x7615}  //MT7615
};

static MI_WLAN_APInfo_t _stAPInfo[MI_WLAN_MAX_APINFO_NUM];
static MI_U8 _u8APNum                                           = 0;
static MI_U8 _au8InterfaceName[MI_MAX_INTERFACE_NAME]           = {'\0'};
static MI_U8 _u8WLanDbgLevel                                    = MI_DBG_ERR;


//----------------------------local function define here------------------------------------------------------------
MI_RESULT mi_sys_LaunchSystemCmd(MI_U8 *pszCmd)
{
    //signal(SIGCHLD, SIG_DFL);
    if(system((char *)pszCmd) != 0)
    {
        return MI_ERR_FAILED;
    }

    return MI_OK;
}

MI_RESULT mi_sys_SystemCmdWithReturn(MI_U8 *pszCmd)
{
    signal(SIGCHLD, SIG_DFL);
    pid_t status;
    status = system((char *)pszCmd);
    if (-1 == status)
    {
        printf("system error\n");
        return MI_ERR_FAILED;
    }
    else
    {
        if (WIFEXITED(status))
        {
            if (0 != WEXITSTATUS(status))
            {
                return WEXITSTATUS(status);
            }
        }
    }
    return MI_ERR_FAILED;
}


#ifdef NOUSING
static void _MI_WLAN_Lock(void)
{
    /* Create re-entry mutex */
    if(_s32WLanMutexId < 0)
    {

        MI_OS_CreateMutexParams_t stCreateMutexParams;
        MI_U8 au8MutexName[] = "MI_WLAN_MUTEX";
        ///Create mutex
        memset(&stCreateMutexParams, 0, sizeof(stCreateMutexParams));
        stCreateMutexParams.pMutexName = au8MutexName;
        stCreateMutexParams.u32Flag = MI_OS_PROCESS_SHARED;
        if (MI_OK != MI_OS_CreateMutex(&stCreateMutexParams, &_s32WLanMutexId))
        {
            MI_ERR("Create mutex error!!\n");
            return;
        }
    }

    if (_s32WLanMutexId < 0)
    {
        MI_ERR("create _s32WLanMutexId failed\n");
    }
    if (MI_OK != MI_OS_ObtainMutex(_s32WLanMutexId, MI_OS_WAIT_FOREVER))
    {
        MI_ERR("_Lock: Obtain mutex failed.\n");
    }
}

static void _MI_WLAN_UnLock(void)
{
    MI_OS_ReleaseMutex(_s32WLanMutexId);
}
#else

static void _MI_WLAN_UnLock(void)
{
;
}

static void _MI_WLAN_Lock(void)
{
;
}
#endif

MI_RESULT DeviceUp(char * interface)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;
    strncpy(ifr.ifr_name,interface ,16);
    ifr.ifr_name[15] = 0;
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0)
    {
        MI_ERR("ioctl fail\n");
        return MI_ERR_FAILED;
    }
    if(ifr.ifr_flags & IFF_UP)
    {
        MI_INFO("DeviceUp already\n");
    }
    else
    {
        ifr.ifr_flags |= IFF_UP;
        if (ioctl(sock, SIOCSIFFLAGS, &ifr) < 0)
        {
            MI_ERR("ioctl fail\n");
            return MI_ERR_FAILED;
        }
    }
    return MI_OK;
}

MI_RESULT DeviceDown(char * interface)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;
    strncpy(ifr.ifr_name,interface ,16);
    ifr.ifr_name[15] = 0;
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0)
    {
        MI_ERR("ioctl fail\n");
        return MI_ERR_FAILED;
    }
    if(!(ifr.ifr_flags & IFF_UP))
    {
        MI_INFO("DeviceDown already\n");
    }
    else
    {
        ifr.ifr_flags &= (~IFF_UP);
        if (ioctl(sock, SIOCSIFFLAGS, &ifr) < 0)
        {
            MI_ERR("ioctl fail\n");
            return MI_ERR_FAILED;
        }
    }
    return MI_OK;
}

static void _MI_WLAN_ChoseChannel(MI_U32 singnals,MI_U32 *singnals_channel)
{
    switch(singnals)
    {
        case 2412:
            *singnals_channel = 1;
            break;
        case 2417:
            *singnals_channel = 2;
            break;
        case 2422:
            *singnals_channel = 3;
            break;
        case 2427:
            *singnals_channel = 4;
            break;
        case 2432:
            *singnals_channel = 5;
            break;
        case 2437:
            *singnals_channel = 6;
            break;
        case 2442:
            *singnals_channel = 7;
            break;
        case 2447:
            *singnals_channel = 8;
            break;
        case 2452:
            *singnals_channel = 9;
            break;
        case 2457:
            *singnals_channel = 10;
            break;
        case 2462:
            *singnals_channel = 11;
            break;
        case 2467:
            *singnals_channel = 12;
            break;
        case 2472:
            *singnals_channel = 13;
            break;
        case 2484:
            *singnals_channel = 14;
            break;
        default:
            break;
    }
}

static MI_BOOL _MI_WLAN_SetStatus(MI_WLAN_Status_e eStatus)
{
    _eWLanStatus = eStatus;
    return TRUE;
}

static MI_BOOL _MI_WLAN_Open(MI_WLAN_OpenParams_t *pstParam, MI_HANDLE *phwlan)
{
    MI_NET_OpenParam_t stOpenParam;
    memset(&stOpenParam, 0x00, sizeof(MI_NET_OpenParam_t));
    memcpy(stOpenParam.szInterfaceName, _au8InterfaceName, sizeof(_au8InterfaceName));

    if(_eWLanStatus >= E_MI_WLAN_STATUS_OPENED)
    {
        MI_ERR("MI_Wlan only support 1 device at once.\n");
        return FALSE;
    }
/*
    if(MI_NET_OpenHandle(&stOpenParam, &_hWLanHandle) != MI_OK)
    {
        return FALSE;
    }

    *phwlan = _hWLanHandle;


    if(DeviceUp((*phwlan)) != MI_OK)
    {
        return FALSE;
    }
*/
    return TRUE;
}

static MI_BOOL _MI_WLAN_RunInitScript(MI_U8* au8ScriptPath, MI_U8* au8InterfaceName)
{
    MI_U8 au8Cmd[MI_WLAN_NORMAL_CMD_LEN];
    memset(au8Cmd, 0x00, sizeof(au8Cmd));
    if(_bAPMode == TRUE)
    {
        snprintf((char *)au8Cmd, sizeof(au8Cmd)-1 , "%s %s/%s", \
                 (char *)MI_WLAN_SH, au8ScriptPath , MI_WLAN_INITAPSH);
    }
    else
    {
        snprintf((char *)au8Cmd, sizeof(au8Cmd)-1 , "%s %s/%s %s %s", \
                 (char *)MI_WLAN_SH, au8ScriptPath , MI_WLAN_INITSH,MI_WLAN_INTERFACE,_u8chipname);
    }
    mi_sys_LaunchSystemCmd(au8Cmd);
    //snprintf((char *)&_au8InterfaceName, sizeof(_au8InterfaceName)-1, "%s", au8InterfaceName);
    //_eModule = E_MI_WLAN_MODULE_AUTO;
    return TRUE;
}

static MI_BOOL _MI_WLAN_RunDeInitScript(MI_U8* au8ScriptPath)
{
    MI_U8 au8Cmd[MI_WLAN_NORMAL_CMD_LEN];
    memset(au8Cmd, 0x00, sizeof(au8Cmd));
    if(_bAPMode == TRUE)
    {
        snprintf((char *)au8Cmd, sizeof(au8Cmd)-1, "%s %s/%s", \
                 (char *)MI_WLAN_SH, au8ScriptPath , MI_WLAN_DEINITAPSH);
    }
    else
    {
        snprintf((char *)au8Cmd, sizeof(au8Cmd)-1, "%s %s/%s", \
                 (char *)MI_WLAN_SH, au8ScriptPath , MI_WLAN_DEINITSH);
    }
    mi_sys_LaunchSystemCmd(au8Cmd);
    _eModule = E_MI_WLAN_MODULE_MAX;
    return TRUE;
}

static void _MI_WLAN_trim(char *c, int arrayLen)
{
    int idx=0;

    for(idx=arrayLen-1;idx>=0;idx--)
    {
        if(!isspace(*(c+idx)))
        {
            *(c+idx+1)= '\0';
            break;
        }
    }
}

static MI_BOOL _MI_WLAN_ScanIwpriv(MI_WLAN_ScanResult_t *pstResult)
{
    static MI_U8 au8ScanCmd[MI_WLAN_LONG_CMD_LEN];
    MI_U8 newline[MI_WLAN_NORMAL_CMD_LEN];
    MI_U8 szTemp[MI_WLAN_NORMAL_CMD_LEN];
    int i;
    memset(&_stAPInfo, 0x00, sizeof(_stAPInfo));
    memset(&au8ScanCmd, 0x00, sizeof(au8ScanCmd));
    memset(&newline, 0x00, sizeof(newline));
    _u8APNum = 0;
    snprintf((char *)au8ScanCmd, sizeof(au8ScanCmd) - 1, "%s %s %s %s=", MI_WLAN_IWPRIV, _au8InterfaceName, MI_WLAN_SET, MI_WLAN_SITESURVEY);
    MI_INFO("au8ScanCmd = %s\n cmd len %d\n", au8ScanCmd, strlen((char*)au8ScanCmd));
    mi_sys_LaunchSystemCmd(au8ScanCmd);
//    MI_OS_DelayTask(6000);
    sleep(6000);
    memset(&au8ScanCmd, 0x00, sizeof(au8ScanCmd));
    snprintf((char *)au8ScanCmd, sizeof(au8ScanCmd) - 1, "%s %s %s", MI_WLAN_IWPRIV,_au8InterfaceName, MI_WLAN_GETSITESURVEY);
    FILE *fd = popen((char*)au8ScanCmd, "r");
    for(i=0;i<3;i++)
    {
        fgets((char*)newline, sizeof(newline), fd);
    }

    if(fd != NULL)
    {
        while((fgets((char*)newline, sizeof(newline), fd)) != NULL)
        {
            if((*(newline+4)==' ')||(strlen((char*)newline)!=120))
            {
               continue;
            }
            memset(&szTemp, 0x00, sizeof(szTemp));
            memcpy((char *)szTemp, (char *)(newline+4+4),33);
            _MI_WLAN_trim((char *)szTemp,33);
            strncpy((char *)_stAPInfo[_u8APNum].au8SSId,(char *)szTemp,sizeof(_stAPInfo[_u8APNum].au8SSId)-1);


            memset(&szTemp, 0x00, sizeof(szTemp));
            memcpy((char *)szTemp, (char *)(newline+4),4);
            _MI_WLAN_trim((char *)szTemp,4);
             _stAPInfo[_u8APNum].u8Channel = atoi((char*)szTemp);

            memset(&szTemp, 0x00, sizeof(szTemp));
            memcpy((char *)szTemp, (char *)(newline+4+4+33),20);
            _MI_WLAN_trim((char *)szTemp,20);
             strncpy((char *)_stAPInfo[_u8APNum].au8Mac,(char *)szTemp,sizeof(_stAPInfo[_u8APNum].au8Mac)-1);

            memset(&szTemp, 0x00, sizeof(szTemp));
            memcpy((char *)szTemp, (char *)(newline+4+4+33+20),23);
            _MI_WLAN_trim((char *)szTemp,23);
              if(0==strcmp((char*)szTemp,"WPA2PSK/AES"))
            {
                _stAPInfo[_u8APNum].bSupportPSK = TRUE;
                _stAPInfo[_u8APNum].eEncrypt = E_MI_WLAN_ENCRYPT_AES;
                _stAPInfo[_u8APNum].eSecurity = E_MI_WLAN_SECURITY_WPA2;
            }
            else if(0==strcmp((char*)szTemp,"WPAPSK/TKIP"))
            {
                _stAPInfo[_u8APNum].bSupportPSK = TRUE;
                _stAPInfo[_u8APNum].eEncrypt = E_MI_WLAN_ENCRYPT_TKIP;
                _stAPInfo[_u8APNum].eSecurity = E_MI_WLAN_SECURITY_WPA;
            }
            else if(0==strcmp((char*)szTemp,"WPA/TKIP"))
            {
                _stAPInfo[_u8APNum].bSupportPSK = FALSE;
                _stAPInfo[_u8APNum].eEncrypt = E_MI_WLAN_ENCRYPT_TKIP;
                _stAPInfo[_u8APNum].eSecurity = E_MI_WLAN_SECURITY_WPA;
            }
            else if(0==strcmp((char*)szTemp,"WPA2PSK/TKIP"))
            {
                _stAPInfo[_u8APNum].bSupportPSK = TRUE;
                _stAPInfo[_u8APNum].eEncrypt = E_MI_WLAN_ENCRYPT_TKIP;
                _stAPInfo[_u8APNum].eSecurity = E_MI_WLAN_SECURITY_WPA2;
            }
            else if(0==strcmp((char*)szTemp,"WPA1PSKWPA2PSK/TKIPAES"))
            {
                _stAPInfo[_u8APNum].bSupportPSK = TRUE;
                _stAPInfo[_u8APNum].eEncrypt = E_MI_WLAN_ENCRYPT_AES;
                _stAPInfo[_u8APNum].eSecurity = E_MI_WLAN_SECURITY_WPA;
            }
            else if(0==strcmp((char*)szTemp,"WPA2PSK/TKIPAES"))
            {
                _stAPInfo[_u8APNum].bSupportPSK = TRUE;
                _stAPInfo[_u8APNum].eEncrypt = E_MI_WLAN_ENCRYPT_AES;
                _stAPInfo[_u8APNum].eSecurity = E_MI_WLAN_SECURITY_WPA2;
            }
            else if(0==strcmp((char*)szTemp,"WEP"))
            {
                _stAPInfo[_u8APNum].bSupportPSK = FALSE;
                _stAPInfo[_u8APNum].eEncrypt = E_MI_WLAN_ENCRYPT_WEP;
                _stAPInfo[_u8APNum].eSecurity = E_MI_WLAN_SECURITY_WEP;
            }
            else if(0==strcmp((char*)szTemp,"NONE"))
            {
                _stAPInfo[_u8APNum].bSupportPSK = FALSE;
                _stAPInfo[_u8APNum].eEncrypt = E_MI_WLAN_ENCRYPT_NONE;
                _stAPInfo[_u8APNum].eSecurity = E_MI_WLAN_SECURITY_NONE;
            }

            memset(&szTemp, 0x00, sizeof(szTemp));
            memcpy((char *)szTemp, (char *)(newline+4+4+33+20+23),9);
            _MI_WLAN_trim((char *)szTemp,9);
            _stAPInfo[_u8APNum].u32Strehgth= atoi((char*)szTemp);
            _stAPInfo[_u8APNum].u32Quality= atoi((char*)szTemp);

            _stAPInfo[_u8APNum].eMode= E_MI_WLAN_NETWORKTYPE_INFRA;
            _stAPInfo[_u8APNum].fBitRate = 0;//not support
            _stAPInfo[_u8APNum].fFrequency = 0;//not support
            _u8APNum++;
        }
    }
    pstResult->u8APNumber = _u8APNum;
    pclose(fd);
    return TRUE;
}

static MI_BOOL _MI_WLAN_Init(MI_WLAN_InitParams_t *pstInitParams)
{
    memset(&_au8Wirelessfolder, 0x00, sizeof(_au8Wirelessfolder));
    memcpy(&_au8Wirelessfolder, MI_WLAN_DEFAULT_FOLDER, sizeof(MI_WLAN_DEFAULT_FOLDER));
    memset(&_stAPInfo, 0x00, sizeof(_stAPInfo));
    memset(&_au8InterfaceName, 0x00, sizeof(_au8InterfaceName));
    _u8APNum = 0;
    if(pstInitParams->pfEventCallback != NULL)
    {
        _pfHotplugCallback = pstInitParams->pfEventCallback;
    }
    _bAPMode = FALSE;
    //mi_usbdevice_InitParams_t stUsbDeviceInitParams;
    MI_U8 au8ScriptPath[MI_WLAN_MAX_FOLDERPATH_LEN];
/*
    memset(&stUsbDeviceInitParams, 0, sizeof(stUsbDeviceInitParams));
    if(MI_OK != mi_usbdevice_Init(&stUsbDeviceInitParams))
    {
        MI_ERR("mi_usbdevice_Init fail!\n");
        return FALSE;
    }
*/
    switch(pstInitParams->eModule)
    {
        case E_MI_WLAN_MODULE_MT7601U:
        {
            snprintf((char*)au8ScriptPath,sizeof(au8ScriptPath), "%s%s",_au8Wirelessfolder, MI_WLAN_FOLDER_MT7601U);
            return _MI_WLAN_RunInitScript(au8ScriptPath,(MI_U8*)MI_WLAN_MT7601U_INTERFACE);
        }
        break;

        case E_MI_WLAN_MODULE_8188ETV:
        {
            snprintf((char*)au8ScriptPath,sizeof(au8ScriptPath), "%s%s",_au8Wirelessfolder, MI_WLAN_FOLDER_MT7601U);
            return _MI_WLAN_RunInitScript(au8ScriptPath,(MI_U8*)MI_WLAN_MT7601U_INTERFACE);
        }
        break;

        case E_MI_WLAN_MODULE_RT5572:
        {
            snprintf((char*)au8ScriptPath,sizeof(au8ScriptPath), "%s%s",_au8Wirelessfolder, MI_WLAN_FOLDER_RT5572);
            return _MI_WLAN_RunInitScript(au8ScriptPath,(MI_U8*)MI_WLAN_RT5572_INTERFACE);
        }
        break;

        default:
            break;
    }

    return FALSE;
}

static MI_BOOL _MI_WLAN_DeInit(void)
{
    MI_U8 au8ScriptPath[MI_WLAN_MAX_FOLDERPATH_LEN];
/*
    if(MI_OK != mi_usbdevice_DeInit())
    {
        MI_ERR("mi_usbdevice_DeInit fail\n");
        return FALSE;
    }
*/
    if(_bCustomerDevice)
    {
        snprintf((char*)au8ScriptPath,sizeof(au8ScriptPath), "%s",_astCustomDeviceInfo[_u16CustomerIndex].au8ScriptPath);
    }
    switch(_eModule)
    {
        case E_MI_WLAN_MODULE_MT7601U:
        {
            snprintf((char*)au8ScriptPath,sizeof(au8ScriptPath), "%s%s",_au8Wirelessfolder, MI_WLAN_FOLDER_MT7601U);
        }
        break;

        case E_MI_WLAN_MODULE_RT5572:
        {
            snprintf((char*)au8ScriptPath,sizeof(au8ScriptPath), "%s%s",_au8Wirelessfolder, MI_WLAN_FOLDER_RT5572);
        }
        break;

        default:
            break;
    }
    memset(&_astCustomDeviceInfo, 0, sizeof(_astCustomDeviceInfo));
    _u16CustomDeviceNum = 0;
    _u16CustomerIndex = 0;
    _bCustomerDevice = FALSE;
    return _MI_WLAN_RunDeInitScript(au8ScriptPath);
}



static MI_BOOL _MI_WLAN_Close(MI_HANDLE hWLan)
{
    if(DeviceDown(MI_WLAN_INTERFACE) != MI_OK)
    {
        MI_WRN("WLAN Device May be already down\n");
    }

/*
    if(MI_NET_CloseHandle(hWLan) != MI_OK)
    {
        return FALSE;
    }
*/
    _hWLanHandle = MI_HANDLE_NULL;
    return TRUE;
}

static MI_BOOL _MI_WLAN_GetAPInfoByIdx(MI_U8 u8idx, MI_WLAN_APInfo_t *pstApInfo)
{
    if(u8idx >= _u8APNum)
    {
        return FALSE;
    }

    memcpy(pstApInfo, &_stAPInfo[u8idx], sizeof(MI_WLAN_APInfo_t));
    return TRUE;
}

static MI_BOOL _MI_WLAN_SetNetworkType(MI_WLAN_NetworkType_e eNetworkType)
{
    MI_U8 au8cmd[MI_WLAN_SHORT_CMD_LEN];
    memset(au8cmd, 0x00, sizeof(au8cmd));

    switch(eNetworkType)
    {
        case E_MI_WLAN_NETWORKTYPE_INFRA:
        {
            //snprintf((char *)au8cmd, sizeof(au8cmd) - 1, "%s %s %s %s=%s", (char *)MI_WLAN_IWPRIV, \
            //         _au8InterfaceName, MI_WLAN_SET , MI_WLAN_NETWORKTYPE , MI_WLAN_NETWORKTYPE_INFRA);
            snprintf((char *)au8cmd, sizeof(au8cmd) - 1, "%s %s %s %s", (char *)MI_WLAN_IWCONFIG, \
                     MI_WLAN_INTERFACE, MI_WLAN_MODE , "managed");
        }
        break;

        case E_MI_WLAN_NETWORKTYPE_AP:
        {
            snprintf((char *)au8cmd, sizeof(au8cmd) - 1, "%s %s %s %s=%s", (char *)MI_WLAN_IWPRIV, \
                     MI_WLAN_INTERFACE, MI_WLAN_SET , MI_WLAN_NETWORKTYPE , MI_WLAN_NETWORKTYPE_AP);
        }
        break;

        case E_MI_WLAN_NETWORKTYPE_ADHOC:
        {
            snprintf((char *)au8cmd, sizeof(au8cmd) - 1, "%s %s %s %s=%s", (char *)MI_WLAN_IWPRIV, \
                     MI_WLAN_INTERFACE, MI_WLAN_SET , MI_WLAN_NETWORKTYPE , MI_WLAN_NETWORKTYPE_ADHOC);
        }
        break;

        case E_MI_WLAN_NETWORKTYPE_MONITOR:
        {
            // iwconfig wlan0 mode monitor
            snprintf((char *)au8cmd, sizeof(au8cmd) - 1, "%s %s %s %s", (char *)MI_WLAN_IWCONFIG, \
                     MI_WLAN_INTERFACE, MI_WLAN_MODE , MI_WLAN_NETWORKTYPE_MONITOR);
        }
        break;

        default:
            return FALSE;
            break;
    }

    mi_sys_LaunchSystemCmd(au8cmd);
    return TRUE;
}

static MI_BOOL _MI_WLAN_SetSecurity(MI_WLAN_Security_e eSecurity, MI_U8 * pu8passwd)
{
    MI_U8 au8Authcmd[MI_WLAN_NORMAL_CMD_LEN]={0};
    MI_U8 au8Encrypcmd[MI_WLAN_NORMAL_CMD_LEN]={0};
    MI_U8 pzAuthMode[MI_WLAN_SHORT_CMD_LEN]={0};
    MI_U8 pzEncryptType[MI_WLAN_SHORT_CMD_LEN]={0};
    MI_U8 pzInterfaceName[MI_WLAN_SHORT_CMD_LEN]={0};
    MI_U8 pzWPAPSK[MI_WLAN_SHORT_CMD_LEN]={0};
    if(_bCtrlAPClient==TRUE)
    {
            snprintf((char*)pzAuthMode,MI_WLAN_SHORT_CMD_LEN-1,"%s",MI_WLAN_APCLIAUTHMODE);
            snprintf((char*)pzEncryptType,MI_WLAN_SHORT_CMD_LEN-1,"%s",MI_WLAN_APCLIENCRYPTYPE);
            snprintf((char*)pzInterfaceName,MI_WLAN_SHORT_CMD_LEN-1,"%s",MI_WLAN_APCLI0);
            snprintf((char*)pzWPAPSK,MI_WLAN_SHORT_CMD_LEN-1,"%s",MI_WLAN_APCLIWPAPSK);
    }
    else
    {
            snprintf((char*)pzAuthMode,MI_WLAN_SHORT_CMD_LEN-1,"%s",MI_WLAN_AUTHMODE);
            snprintf((char*)pzEncryptType,MI_WLAN_SHORT_CMD_LEN-1,"%s",MI_WLAN_ENCRYPTYPE);
            snprintf((char*)pzInterfaceName,MI_WLAN_SHORT_CMD_LEN-1,"%s",_au8InterfaceName);
            snprintf((char*)pzWPAPSK,MI_WLAN_SHORT_CMD_LEN-1,"%s",MI_WLAN_WPAPSK);
    }

    switch(eSecurity)
    {
        case E_MI_WLAN_SECURITY_NONE:
        {
            snprintf((char *)au8Authcmd, sizeof(au8Authcmd)-1, "%s %s %s %s=%s", \
                     (char *)MI_WLAN_IWPRIV, pzInterfaceName, MI_WLAN_SET , pzAuthMode, MI_WLAN_AUTHMODE_OPEN);
            snprintf((char *)au8Encrypcmd, sizeof(au8Encrypcmd)-1, "%s %s %s %s=%s", \
                     (char *)MI_WLAN_IWPRIV, pzInterfaceName, MI_WLAN_SET , pzEncryptType, MI_WLAN_ENCRYPTYPE_NONE);
            mi_sys_LaunchSystemCmd(au8Authcmd);
            mi_sys_LaunchSystemCmd(au8Encrypcmd);
        }
        break;
        case E_MI_WLAN_SECURITY_WPA:
        {
            MI_U8 au8WPAPSK[MI_WLAN_NORMAL_CMD_LEN];
            memset(au8WPAPSK, 0x00, sizeof(au8WPAPSK));
            snprintf((char *)au8Authcmd, sizeof(au8Authcmd)-1, "%s %s %s %s=%s", \
                     (char *)MI_WLAN_IWPRIV, pzInterfaceName, MI_WLAN_SET , pzAuthMode, MI_WLAN_AUTHMODE_WPAPSK);
            snprintf((char *)au8Encrypcmd, sizeof(au8Encrypcmd)-1, "%s %s %s %s=%s", \
                     (char *)MI_WLAN_IWPRIV, pzInterfaceName, MI_WLAN_SET , pzEncryptType, MI_WLAN_ENCRYPTYPE_AES);
            snprintf((char *)au8WPAPSK, sizeof(au8WPAPSK)-1, "%s %s %s %s=%s", \
                     (char *)MI_WLAN_IWPRIV, pzInterfaceName, MI_WLAN_SET , pzWPAPSK, pu8passwd);
            mi_sys_LaunchSystemCmd(au8Authcmd);
            mi_sys_LaunchSystemCmd(au8Encrypcmd);
            mi_sys_LaunchSystemCmd(au8WPAPSK);
        }
        break;
        case E_MI_WLAN_SECURITY_WPA2:
        {
            MI_U8 au8WPAPSK[MI_WLAN_NORMAL_CMD_LEN];
            memset(au8WPAPSK, 0x00, sizeof(au8WPAPSK));
            snprintf((char *)au8Authcmd, sizeof(au8Authcmd)-1, "%s %s %s %s=%s", \
                     (char *)MI_WLAN_IWPRIV, pzInterfaceName, MI_WLAN_SET , pzAuthMode, MI_WLAN_AUTHMODE_WPA2PSK);
            snprintf((char *)au8Encrypcmd, sizeof(au8Encrypcmd)-1, "%s %s %s %s=%s", \
                     (char *)MI_WLAN_IWPRIV, pzInterfaceName, MI_WLAN_SET , pzEncryptType, MI_WLAN_ENCRYPTYPE_AES);
            snprintf((char *)au8WPAPSK, sizeof(au8WPAPSK)-1, "%s %s %s %s=%s", \
                     (char *)MI_WLAN_IWPRIV, pzInterfaceName, MI_WLAN_SET , pzWPAPSK, pu8passwd);
            mi_sys_LaunchSystemCmd(au8Authcmd);
            mi_sys_LaunchSystemCmd(au8Encrypcmd);
            mi_sys_LaunchSystemCmd(au8WPAPSK);
        }
        break;

        default:
            return FALSE;
            break;
    }
    return TRUE;
}

static MI_BOOL _MI_WLAN_SetSSID(MI_U8 *pSSID)
{
    MI_U8 au8cmd[MI_WLAN_NORMAL_CMD_LEN]={0};
    MI_U8 pzSSID[MI_WLAN_SHORT_CMD_LEN]={0};
    MI_U8 pzInterfaceName[MI_WLAN_SHORT_CMD_LEN]={0};
    if(_bCtrlAPClient==TRUE)
    {
            snprintf((char*)pzSSID,MI_WLAN_SHORT_CMD_LEN-1,"%s",MI_WLAN_APCLISSID);
            snprintf((char*)pzInterfaceName,MI_WLAN_SHORT_CMD_LEN-1,"%s",MI_WLAN_APCLI0);
    }
    else
    {
            snprintf((char*)pzSSID,MI_WLAN_SHORT_CMD_LEN-1,"%s",MI_WLAN_SSID);
            snprintf((char*)pzInterfaceName,MI_WLAN_SHORT_CMD_LEN-1,"%s",_au8InterfaceName);
    }
    if(pSSID != NULL)
    {
        snprintf((char *)au8cmd, sizeof(au8cmd)-1, "%s %s %s %s=%s", \
                 (char *)MI_WLAN_IWPRIV, pzInterfaceName, MI_WLAN_SET , pzSSID, pSSID);
    }
    else
    {
        snprintf((char *)au8cmd, sizeof(au8cmd)-1, "%s %s %s %s=", \
                 (char *)MI_WLAN_IWPRIV, pzInterfaceName, MI_WLAN_SET , pzSSID);
    }

    mi_sys_LaunchSystemCmd(au8cmd);
    return TRUE;
}

static MI_BOOL _MI_WLAN_Connect(MI_WLAN_ConnectParam_t *pstConnectParam)
{
    MI_U8 au8Cmd[MI_WLAN_NORMAL_CMD_LEN];
    MI_U8 au8Security[MI_WLAN_SHORT_CMD_LEN];
    MI_U8 au8Script[MI_WLAN_SHORT_CMD_LEN];
    MI_WLAN_ScanResult_t  stResult;
    int i=0,j=0;
    memset(au8Cmd, 0x00 , sizeof(au8Cmd));
    _bCtrlAPClient=FALSE;

    memset(_u8ssid, 0x00 , sizeof(_u8ssid));
    strncpy((char*)_u8ssid,(const char*)(pstConnectParam->au8SSId),strlen((const char*)(pstConnectParam->au8SSId)));

//    mi_net_IsCtrlAPClient(_bCtrlAPClient);
    if (_bCustomerDevice == TRUE)
    {
        printf("_bCustomerDevice is true");
    }
    else
    {
        _bCustomerDevice = TRUE;
    }
    if(_bCustomerDevice == TRUE)
    {
        switch(pstConnectParam->eNetworkType)
        {
            case E_MI_WLAN_NETWORKTYPE_AP:
                snprintf((char *)au8Script,sizeof(au8Script),"%s", MI_WLAN_CONNECTAPSH);
                snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s %s",MI_WLAN_IFCONFIG, _astCustomDeviceInfo[_u16CustomerIndex].au8InterfaceName, MI_WLAN_AP_DEFAULT_IP);
                mi_sys_LaunchSystemCmd(au8Cmd);
                memset(au8Cmd, 0x00 , sizeof(au8Cmd));
                snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s/%s",MI_WLAN_UDHCPD,_astCustomDeviceInfo[_u16CustomerIndex].au8ScriptPath, MI_WLAN_UDHCPDCONF);
                mi_sys_LaunchSystemCmd(au8Cmd);
                memset(au8Cmd, 0x00 , sizeof(au8Cmd));
                break;
            case E_MI_WLAN_NETWORKTYPE_INFRA:
                snprintf((char *)au8Script,sizeof(au8Script),"%s", MI_WLAN_CONNECTSH);
                break;
            default:
                MI_ERR("Unknown NetworkType type!\n");
                break;
        }
        switch(pstConnectParam->eSecurity)
        {
            case E_MI_WLAN_SECURITY_WEP:
                snprintf((char *)au8Security,sizeof(au8Security),"%s", MI_WLAN_ENCRYPTYPE_WEP);
                break;
            case E_MI_WLAN_SECURITY_WPA:
                snprintf((char *)au8Security,sizeof(au8Security),"%s", MI_WLAN_WPAPSK);
                break;
            case E_MI_WLAN_SECURITY_WPA2:
                snprintf((char *)au8Security,sizeof(au8Security),"%s", MI_WLAN_AUTHMODE_WPA2PSK);
                break;
            case E_MI_WLAN_SECURITY_NONE:
                snprintf((char *)au8Security,sizeof(au8Security),"%s", MI_WLAN_ENCRYPTYPE_NONE);
                break;
            default:
                MI_ERR("Unknown Security type!\n");
                break;
        }
        /*
        snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s/%s %s \"%s\" %s %s",MI_WLAN_SH,_astCustomDeviceInfo[_u16CustomerIndex].au8ScriptPath, au8Script, \
                _astCustomDeviceInfo[_u16CustomerIndex].au8InterfaceName,pstConnectParam->au8SSId, au8Security, pstConnectParam->au8Password);
        */
        snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s%s%s %s %s %s %s",MI_WLAN_SH,MI_WLAN_DEFAULT_FOLDER,MI_WLAN_FOLDER_MT7601U,au8Script,MI_WLAN_INTERFACE, \
                pstConnectParam->au8SSId, au8Security, pstConnectParam->au8Password);
        MI_INFO("%s\n",au8Cmd);
        if (MI_OK == mi_sys_LaunchSystemCmd(au8Cmd))
        {
            snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s -i %s",MI_WLAN_UDHCPC,MI_WLAN_INTERFACE);
            MI_INFO("%s\n",au8Cmd);
            mi_sys_LaunchSystemCmd(au8Cmd);
        }
    }
    else//_bCustomerDevice == FALSE
    {
        if(pstConnectParam->eNetworkType == E_MI_WLAN_NETWORKTYPE_AP)
        {
            snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s %s",MI_WLAN_IFCONFIG, MI_WLAN_RT5572_INTERFACE, MI_WLAN_AP_DEFAULT_IP);
            mi_sys_LaunchSystemCmd(au8Cmd);
            memset(au8Cmd, 0x00 , sizeof(au8Cmd));
            snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s",MI_WLAN_UDHCPD);
            mi_sys_LaunchSystemCmd(au8Cmd);
        }
        else
        {
            if(_bAPClient == FALSE)
            {
                if(_MI_WLAN_SetNetworkType(pstConnectParam->eNetworkType) != TRUE)
                {
                    return FALSE;
                }
            }
            else //apcli0 as STA
            {
                _bCtrlAPClient=TRUE;
//                mi_net_IsCtrlAPClient(_bCtrlAPClient);
                memset(au8Cmd, 0x00 , sizeof(au8Cmd));
                snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s %s %s=0",MI_WLAN_IWPRIV, MI_WLAN_APCLI0, MI_WLAN_SET, MI_WLAN_APCLIENABLE);
                mi_sys_LaunchSystemCmd(au8Cmd);
                memset(au8Cmd, 0x00 , sizeof(au8Cmd));
                if(_u8APNum==0)
                {
                    if(TRUE !=_MI_WLAN_ScanIwpriv(&stResult))
                    {
                        return FALSE;
                    }
                }
                for(i=0;i<_u8APNum;i++)
                {
                    if(0==strcmp((char *)pstConnectParam->au8SSId,(char *)_stAPInfo[i].au8SSId))
                    {
                        memset(au8Cmd, 0x00 , sizeof(au8Cmd));
                        snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s %s %s=%d",MI_WLAN_IWPRIV, MI_WLAN_APCLI0, MI_WLAN_SET, MI_WLAN_CHANNEL,_stAPInfo[i].u8Channel);
                        mi_sys_LaunchSystemCmd(au8Cmd);
                        break;
                    }
                    if(i==_u8APNum-1)
                    {
                        if(TRUE !=_MI_WLAN_ScanIwpriv(&stResult))
                        {
                            return FALSE;
                        }
                        for(j=0;j<_u8APNum;j++)
                        {
                            if(0==strcmp((char*)pstConnectParam->au8SSId,(char*)_stAPInfo[j].au8SSId))
                            {
                                memset(au8Cmd, 0x00 , sizeof(au8Cmd));
                                snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s %s %s=%d",MI_WLAN_IWPRIV, MI_WLAN_APCLI0, MI_WLAN_SET, MI_WLAN_CHANNEL,_stAPInfo[j].u8Channel);
                                mi_sys_LaunchSystemCmd(au8Cmd);
                                break;
                            }
                            if(j==_u8APNum-1)
                            {
                                MI_ERR("Cannot detect AP with SSID = %s\n",pstConnectParam->au8SSId);
                                return FALSE;
                            }
                        }
                    }
                }
            }
        }

        if(_MI_WLAN_SetSecurity(pstConnectParam->eSecurity, pstConnectParam->au8Password) != TRUE)
        {
            return FALSE;
        }

        if(_MI_WLAN_SetSSID(pstConnectParam->au8SSId) != TRUE)
        {
            return FALSE;
        }
        if(pstConnectParam->eNetworkType == E_MI_WLAN_NETWORKTYPE_AP)//AP mode must reset password after set SSID
        {
            if(_MI_WLAN_SetSecurity(pstConnectParam->eSecurity, pstConnectParam->au8Password) != TRUE)
            {
                return FALSE;
            }
        }
        if(_bAPClient == TRUE)//7615 AP mode must reset SSID after password..
        {
            if(_MI_WLAN_SetSSID(pstConnectParam->au8SSId) != TRUE)
            {
                return FALSE;
            }
            if(pstConnectParam->eNetworkType == E_MI_WLAN_NETWORKTYPE_INFRA)
            {
                memset(au8Cmd, 0x00 , sizeof(au8Cmd));
                snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s %s %s=1",MI_WLAN_IWPRIV, MI_WLAN_APCLI0, MI_WLAN_SET, MI_WLAN_APCLIENABLE);
                mi_sys_LaunchSystemCmd(au8Cmd);
            }
        }
    }
    return TRUE;
}

static MI_BOOL _MI_WLAN_Disconnect(void)
{
    if(_bCustomerDevice == TRUE)
    {
        MI_U8 au8Cmd[MI_WLAN_NORMAL_CMD_LEN];
        if(_bAPMode == TRUE)
        {
            snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s/%s",MI_WLAN_SH,_astCustomDeviceInfo[_u16CustomerIndex].au8ScriptPath, MI_WLAN_DISCONNECTAPSH);
        }
        else
        {
            snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s%s%s %s",MI_WLAN_SH,MI_WLAN_DEFAULT_FOLDER,MI_WLAN_FOLDER_MT7601U, MI_WLAN_DISCONNECTSH,MI_WLAN_INTERFACE);
        }
        MI_INFO("%s\n",au8Cmd);
        mi_sys_LaunchSystemCmd(au8Cmd);
    }
    else
    {
        return _MI_WLAN_SetSSID(NULL);
    }
    return TRUE;
}

static MI_BOOL _MI_WLAN_Scan(MI_WLAN_ScanResult_t *pstResult)
{
    static MI_U8 au8ScanCmd[MI_WLAN_LONG_CMD_LEN];
    MI_U8 newline[MI_WLAN_NORMAL_CMD_LEN], newlinebk[MI_WLAN_NORMAL_CMD_LEN];
    MI_S8 s8Idx = -1;
    MI_BOOL bFindCell = FALSE;
    MI_BOOL bFindFirstIE = FALSE;
    MI_U8 au8MacBuffer[MI_WLAN_MAX_MAC_LEN];
    memset(&_stAPInfo, 0x00, sizeof(_stAPInfo));
    memset(&au8ScanCmd, 0x00, sizeof(au8ScanCmd));
    memset(&newline, 0x00, sizeof(newline));
    memset(&newlinebk, 0x00, sizeof(newlinebk));
    _u8APNum = 0;
    snprintf((char *)au8ScanCmd, sizeof(au8ScanCmd) - 1, "%s %s %s %s", MI_WLAN_IWLIST, MI_WLAN_INTERFACE, MI_WLAN_SCAN_CMD, MI_WLAN_SCAN_PARAM);
    MI_INFO("au8ScanCmd = %s\n cmd len %d\n", au8ScanCmd, strlen((char*)au8ScanCmd));
    FILE *fd = popen((char*)au8ScanCmd, "r");

    if(fd != NULL)
    {
        while((fgets((char*)newline, sizeof(newline), fd)) != NULL)
        {
            //MI_INFO("We've got a newline %s", newline);
            memcpy(newlinebk, newline, sizeof(newlinebk));
            char *pCellToken = strtok((char*)newlinebk, " ");

            if(pCellToken != NULL)
            {
                if(strncmp(MI_WLAN_TOKEN_CELL, pCellToken, strlen(MI_WLAN_TOKEN_CELL)) == 0)
                {
                    bFindCell = TRUE;
                    bFindFirstIE = FALSE;
                    s8Idx++;
                    pCellToken = strtok(NULL, " ");
                    pCellToken = strtok(NULL, "\n");
                    if(pCellToken != NULL)
                    {
                        strncpy((char*)au8MacBuffer, pCellToken, sizeof(au8MacBuffer));
                    }
                }
            }

            if((bFindCell == TRUE) && (s8Idx >= 0))
            {
                {
                    //parser ESSID start
                    memcpy(newlinebk, newline, sizeof(newlinebk));
                    char *pESSIDToken = strtok((char*)newlinebk, ":");

                    if(strncmp(MI_WLAN_TOKEN_ESSID, pESSIDToken, strlen(MI_WLAN_TOKEN_ESSID)) == 0)
                    {
                        pESSIDToken = strtok(NULL, ":");

                        if(pESSIDToken != NULL)
                        {
                            MI_U8 au8ESSID[MI_WLAN_SHORT_CMD_LEN];
                            memset(&au8ESSID, 0x00, sizeof(au8ESSID));
                            strncpy((char*)&au8ESSID, pESSIDToken, sizeof(au8ESSID));
                            char *pFinalESSID = strtok((char*)au8ESSID, "\"");

                            if(pFinalESSID != NULL)
                            {
                                if(strncmp("\"\"", pESSIDToken, 2) == 0)
                                {
                                    bFindCell = FALSE; // reset when SSID is empty
                                    s8Idx--;
                                    continue;
                                }

                                memset(&_stAPInfo[s8Idx].au8SSId, 0x00, sizeof(_stAPInfo[s8Idx].au8SSId));
                                strncpy((char*)&_stAPInfo[s8Idx].au8SSId, pFinalESSID, sizeof(_stAPInfo[s8Idx].au8SSId));
                                MI_INFO("_stAPInfo[%d].au8SSId %s\n", s8Idx , _stAPInfo[s8Idx].au8SSId);
                                memset(&_stAPInfo[s8Idx].au8Mac, 0x00, sizeof(_stAPInfo[s8Idx].au8Mac));
                                strncpy((char*)&_stAPInfo[s8Idx].au8Mac, (char*)au8MacBuffer, sizeof(_stAPInfo[s8Idx].au8Mac));
                                MI_INFO("_stAPInfo[%d].au8Mac %s\n", s8Idx , _stAPInfo[s8Idx].au8Mac);
                            }
                            else
                            {
                                bFindCell = FALSE; // reset when SSID is empty
                                s8Idx--;
                            }

                            continue;
                        }
                    }
                }//parser ESSID end
                {
                    //parser Mode start
                    memcpy(newlinebk, newline, sizeof(newlinebk));
                    char *pModeToken = strtok((char*)newlinebk, ":");

                    if(strncmp(MI_WLAN_TOKEN_MODE, pModeToken, strlen(MI_WLAN_TOKEN_MODE)) == 0)
                    {
                        pModeToken = strtok(NULL, ":");

                        if(pModeToken != NULL)
                        {
                            if(strncmp(MI_WLAN_TOKEN_MODE_INFRA, pModeToken, strlen(MI_WLAN_TOKEN_MODE_INFRA)) == 0)
                            {
                                _stAPInfo[s8Idx].eMode = E_MI_WLAN_NETWORKTYPE_INFRA;
                            }
                            else if(strncmp(MI_WLAN_TOKEN_MODE_ADHOC, pModeToken, strlen(MI_WLAN_TOKEN_MODE_ADHOC)) == 0)
                            {
                                _stAPInfo[s8Idx].eMode = E_MI_WLAN_NETWORKTYPE_ADHOC;
                            }
                            MI_INFO("_stAPInfo[%d].eMode %d \n", s8Idx , _stAPInfo[s8Idx].eMode);
                            continue;
                        }
                    }
                }//parser Mode end
                {
                    //parser Frequency start
                    memcpy(newlinebk, newline, sizeof(newlinebk));
                    char *pFreqToken = strtok((char*)newlinebk, ":");

                    if(strncmp(MI_WLAN_TOKEN_FREQ, pFreqToken, strlen(MI_WLAN_TOKEN_FREQ)) == 0)
                    {
                        pFreqToken = strtok(NULL, ":");

                        if(pFreqToken != NULL)
                        {
                            _stAPInfo[s8Idx].fFrequency = atof(pFreqToken);
                            MI_INFO("_stAPInfo[%d].fFrequency %f \n", s8Idx , _stAPInfo[s8Idx].fFrequency);
                            continue;
                        }
                    }
                }//parser Frequency end
                {
                    //parser Channel start
                    memcpy(newlinebk, newline, sizeof(newlinebk));
                    char *pChannelToken = strtok((char*)newlinebk, ":");

                    if(strncmp(MI_WLAN_TOKEN_CHANNEL, pChannelToken, strlen(MI_WLAN_TOKEN_CHANNEL)) == 0)
                    {
                        pChannelToken = strtok(NULL, ":");

                        if(pChannelToken != NULL)
                        {
                            _stAPInfo[s8Idx].u8Channel = atoi(pChannelToken);
                            MI_INFO("_stAPInfo[%d].u8Channel %d \n", s8Idx , _stAPInfo[s8Idx].u8Channel);
                            continue;
                        }
                    }
                }//parser Channel end
                {
                    //parser Quality start
                    memcpy(newlinebk, newline, sizeof(newlinebk));
                    char *pQualityToken = strtok((char*)newlinebk, "=");

                    if(strncmp(MI_WLAN_TOKEN_QUALITY, pQualityToken, strlen(MI_WLAN_TOKEN_QUALITY)) == 0)
                    {
                        pQualityToken = strtok(NULL, "=");

                        if(pQualityToken != NULL)
                        {
                            char *pQualityToken2 = strtok((char*)pQualityToken, "/");

                            if(pQualityToken2 != NULL)
                            {
                                _stAPInfo[s8Idx].u32Quality = atoi(pQualityToken2);
                                MI_INFO("_stAPInfo[%d].u32Quality %d\n", s8Idx , (int)(_stAPInfo[s8Idx].u32Quality));
                                pQualityToken = strtok(NULL, " ");
                                pQualityToken = strtok(NULL, " ");
                                _stAPInfo[s8Idx].u32Strehgth = atoi(pQualityToken)+100;
                                MI_INFO("_stAPInfo[%d].u32Strehgth = %d\n", s8Idx , (int)(_stAPInfo[s8Idx].u32Strehgth));
                                continue;
                            }
                        }
                    }
                }//parser Quality end
                {
                    //parser Key end
                    memcpy(newlinebk, newline, sizeof(newlinebk));
                    char *pEncryptionToken = strtok((char*)newlinebk, ":");

                    if(pEncryptionToken != NULL)
                    {
                        if(strncmp(MI_WLAN_TOKEN_KEY, pEncryptionToken, strlen(MI_WLAN_TOKEN_KEY)) == 0)
                        {
                            pEncryptionToken = strtok(NULL, ":");

                            if(pEncryptionToken != NULL)
                            {
                                if(strncmp(MI_WLAN_TOKEN_ON, pEncryptionToken, strlen(MI_WLAN_TOKEN_ON)) == 0)
                                {
                                    if(_stAPInfo[s8Idx].eSecurity == E_MI_WLAN_SECURITY_NONE)
                                    {
                                        _stAPInfo[s8Idx].eSecurity = E_MI_WLAN_SECURITY_UNKNOWTYPE;
                                        MI_INFO("_stAPInfo[%d].eSecurity %d \n", s8Idx , _stAPInfo[s8Idx].eSecurity);
                                        continue;
                                    }
                                }
                                else if(strncmp(MI_WLAN_TOKEN_OFF, pEncryptionToken, strlen(MI_WLAN_TOKEN_OFF)) == 0)
                                {
                                    _stAPInfo[s8Idx].eSecurity = E_MI_WLAN_SECURITY_NONE;
                                    MI_INFO("_stAPInfo[%d].eSecurity %d \n", s8Idx , _stAPInfo[s8Idx].eSecurity);
                                    continue;
                                }
                            }
                        }
                    }
                }//parser Key end
                {
                    if(bFindFirstIE == FALSE)
                    {
                        //parser IE start
                        memcpy(newlinebk, newline, sizeof(newlinebk));
                        char *pIEToken = strtok((char*)newlinebk, ":");

                        if(strncmp(MI_WLAN_TOKEN_IE, pIEToken, strlen(MI_WLAN_TOKEN_IE)) == 0)
                        {
                            pIEToken = strtok(NULL, ":");

                            if(_stAPInfo[s8Idx].eSecurity == E_MI_WLAN_SECURITY_UNKNOWTYPE)
                            {
                                if(strncmp(MI_WLAN_TOKEN_WPA1, pIEToken, strlen(MI_WLAN_TOKEN_WPA1)) == 0)
                                {
                                    _stAPInfo[s8Idx].eSecurity = E_MI_WLAN_SECURITY_WPA;
                                }
                                else if(strncmp(MI_WLAN_TOKEN_WPA18960, pIEToken, strlen(MI_WLAN_TOKEN_WPA18960)) == 0)
                                {
                                    _stAPInfo[s8Idx].eSecurity = E_MI_WLAN_SECURITY_WPA;
                                }
                                else if(strncmp(MI_WLAN_TOKEN_WPA2, pIEToken, strlen(MI_WLAN_TOKEN_WPA1)) == 0)
                                {
                                    _stAPInfo[s8Idx].eSecurity = E_MI_WLAN_SECURITY_WPA2;

                                }
                                MI_INFO("_stAPInfo[%d].eSecurity %d \n", s8Idx , _stAPInfo[s8Idx].eSecurity);
                                continue;
                            }
                        }
                    }
                }//parser IE end
                //parser Encrypt type start
                {
                    if(bFindFirstIE == FALSE)
                    {
                        memcpy(newlinebk, newline, sizeof(newlinebk));
                        char *pEncryptToken = strtok((char*)newlinebk, " ");

                        if(strncmp(MI_WLAN_TOKEN_ENCRYPT, pEncryptToken, strlen(MI_WLAN_TOKEN_ENCRYPT)) == 0)
                        {
                            pEncryptToken = strtok(NULL, " ");

                            if(pEncryptToken != NULL)
                            {
                                if(strncmp(MI_WLAN_TOKEN_ENCRYPT_AES, pEncryptToken, strlen(MI_WLAN_TOKEN_ENCRYPT_AES)) == 0)
                                {
                                    _stAPInfo[s8Idx].eEncrypt= E_MI_WLAN_ENCRYPT_AES;
                                }
                                else if(strncmp(MI_WLAN_TOKEN_ENCRYPT_TKIP, pEncryptToken, strlen(MI_WLAN_TOKEN_ENCRYPT_TKIP)) == 0)
                                {
                                    _stAPInfo[s8Idx].eEncrypt = E_MI_WLAN_ENCRYPT_TKIP;
                                }
                                else if(strncmp(MI_WLAN_TOKEN_ENCRYPT_WEP, pEncryptToken, strlen(MI_WLAN_TOKEN_ENCRYPT_WEP)) == 0)
                                {
                                    _stAPInfo[s8Idx].eEncrypt = E_MI_WLAN_ENCRYPT_WEP;
                                }
                                else
                                {
                                    _stAPInfo[s8Idx].eEncrypt = E_MI_WLAN_ENCRYPT_UNKNOWN;
                                }
                                MI_INFO("_stAPInfo[%d].eEncrypt %d \n", s8Idx , _stAPInfo[s8Idx].eEncrypt);
                                bFindFirstIE = TRUE;
                                continue;
                            }
                        }
                    }
                }//parser Encrypt type end
                {
                    //parser Channel start
                    memcpy(newlinebk, newline, sizeof(newlinebk));

                    if(strncmp(MI_WLAN_TOKEN_PSK, (char *)newlinebk, strlen(MI_WLAN_TOKEN_PSK)) == 0)
                    {
                        _stAPInfo[s8Idx].bSupportPSK = TRUE;
                        MI_INFO("_stAPInfo[%d].bSupportPSK = TRUE \n", s8Idx);
                    }
                }//parser Channel end
            }
        }

        pclose(fd);
    }

    pstResult->u8APNumber = s8Idx + 1;
    _u8APNum = pstResult->u8APNumber;
    return TRUE;
}

static MI_BOOL _MI_WLAN_SetBridge(MI_WLAN_BridgeSetting_t* pstBrSetting)
{
    MI_U8 au8cmd[MI_WLAN_NORMAL_CMD_LEN]={0};
    MI_U8 au8ClientIp[MI_WLAN_NORMAL_CMD_LEN]={0};
    MI_U8 au8InterfaceName[MI_WLAN_SHORT_CMD_LEN]={0};

    if(pstBrSetting->bEnable==FALSE)
    {
        memset(au8cmd, 0x00, sizeof(au8cmd));
        snprintf((char *)au8cmd, sizeof(au8cmd)-1, "%s %s down", MI_WLAN_IFCONFIG, MI_WLAN_BR0);
        mi_sys_LaunchSystemCmd(au8cmd);
        memset(au8cmd, 0x00, sizeof(au8cmd));
        snprintf((char *)au8cmd, sizeof(au8cmd)-1, "%s %s %s", MI_WLAN_BRCTL, MI_WLAN_DELBR, MI_WLAN_BR0);
        mi_sys_LaunchSystemCmd(au8cmd);
        return TRUE;
    }

    if((pstBrSetting->au8InterfaceName== NULL)||(strlen((char*)pstBrSetting->au8InterfaceName) <=1))
    {
        snprintf((char *)au8InterfaceName, sizeof(au8InterfaceName)-1, "%s", MI_WLAN_APCLI0);
    }
    else
    {
        snprintf((char *)au8InterfaceName, sizeof(au8InterfaceName)-1, "%s", pstBrSetting->au8InterfaceName);
    }
    memset(au8cmd, 0x00, sizeof(au8cmd));
    snprintf((char *)au8cmd, sizeof(au8cmd)-1, "ifconfig %s | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'", au8InterfaceName);
    FILE *fd = popen((char*)au8cmd, "r");
    if(fd==NULL)
    {
        return FALSE;
    }
    fgets((char*)au8ClientIp, sizeof(au8ClientIp), fd);
    if((au8ClientIp==NULL)||(strlen((char*)au8ClientIp)<=1))
    {
        MI_ERR("Failed to get ip from ap-client\n");
        pclose(fd);
        return FALSE;
    }


    memset(au8cmd, 0x00, sizeof(au8cmd));
    snprintf((char *)au8cmd, sizeof(au8cmd)-1, "%s %s %s", MI_WLAN_BRCTL, MI_WLAN_ADDBR, MI_WLAN_BR0);
    mi_sys_LaunchSystemCmd(au8cmd);
    memset(au8cmd, 0x00, sizeof(au8cmd));
    snprintf((char *)au8cmd, sizeof(au8cmd)-1, "%s %s %s %s", MI_WLAN_BRCTL, MI_WLAN_ADDIF, MI_WLAN_BR0, MI_WLAN_APCLI0);
    mi_sys_LaunchSystemCmd(au8cmd);
    memset(au8cmd, 0x00, sizeof(au8cmd));
    snprintf((char *)au8cmd, sizeof(au8cmd)-1, "%s %s %s %s", MI_WLAN_BRCTL, MI_WLAN_ADDIF, MI_WLAN_BR0, _au8InterfaceName);
    mi_sys_LaunchSystemCmd(au8cmd);
    memset(au8cmd, 0x00, sizeof(au8cmd));
    snprintf((char *)au8cmd, sizeof(au8cmd)-1, "%s %s %s", MI_WLAN_IFCONFIG, MI_WLAN_BR0, au8ClientIp);
    mi_sys_LaunchSystemCmd(au8cmd);
    pclose(fd);
    return TRUE;
}

static MI_RESULT _MI_WLAN_GetStatus(MI_WLAN_Status_t * statu)
{
    MI_U8 tmp[128];
    MI_U8 file_path[128];
    MI_U8 result[3][10]={0};
    MI_U8 au8Cmd[MI_WLAN_NORMAL_CMD_LEN];
    MI_U32 i;
    MI_U32 channel = 0;
    FILE *fp = NULL;

    if (statu == NULL)
    {
        return MI_ERR_FAILED;
    }
/*
----------------------------------------------get IP MAC and more----------------------------------------------------------------------------
*/
    memset(au8Cmd, 0x00 , sizeof(au8Cmd));
    snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s -i%s %s > %s%s",MI_WLAN_WPACLI,MI_WLAN_INTERFACE,MI_WLAN_STATUS,MI_WLAN_TMP,MI_WLAN_STATUS_FILE);
    MI_INFO("%s\n",au8Cmd);
    mi_sys_LaunchSystemCmd(au8Cmd);

    memset(tmp,0,sizeof(tmp));
    memset(file_path,0,sizeof(file_path));
    snprintf((char*)file_path,sizeof(file_path),"%s%s",MI_WLAN_TMP,MI_WLAN_STATUS_FILE);
    MI_INFO("file_path == %s\n",file_path);
    fp = fopen((const char*)file_path,"r");
    if (!fp) {
        MI_INFO("Can not open file %s,please check !",file_path);
        return MI_ERR_FAILED;
    }

    while (fgets((char*)tmp, sizeof(tmp), fp))
    {
        if(!strncmp("bssid=", (const char*)tmp, 6))
            sscanf((const char*)tmp, "bssid=%s", statu->bssid);
        else if(!strncmp("freq=", (const char*)tmp, 5))
            sscanf((const char*)tmp, "freq=%s", statu->freq);
        else if(!strncmp("ssid=", (const char*)tmp, 5))
            sscanf((const char*)tmp, "ssid=%s", statu->ssid);
        else if(!strncmp("id=", (const char*)tmp, 3))
            sscanf((const char*)tmp, "id=%s", statu->id);
        else if(!strncmp("mode=", (const char*)tmp, 5))
            sscanf((const char*)tmp, "mode=%s", statu->mode);
        else if(!strncmp("key_mgmt=", (const char*)tmp, 9))
            sscanf((const char*)tmp, "key_mgmt=%s", statu->key_mgmt);
        else if(!strncmp("wpa_state=", (const char*)tmp, 10))
            sscanf((const char*)tmp, "wpa_state=%s", statu->wpa_state);
        else if(!strncmp("ip_address=", (const char*)tmp, 11))
            sscanf((const char*)tmp, "ip_address=%s", statu->ip_address);
        else if(!strncmp("address=", (const char*)tmp, 8))
            sscanf((const char*)tmp, "address=%s", statu->address);
        memset(tmp, 0, sizeof(tmp));
    }
    fclose(fp);

/*
----------------------------------------------get Singnals RSSI and Bandwidth----------------------------------------------------------------
*/
    memset(au8Cmd, 0x00 , sizeof(au8Cmd));
    snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s%s%s %s",MI_WLAN_SH,MI_WLAN_DEFAULT_FOLDER,MI_WLAN_FOLDER_MT7601U,MI_WLAN_GETSTATUS,statu->bssid);
    MI_INFO("%s\n",au8Cmd);
    mi_sys_LaunchSystemCmd(au8Cmd);

    memset(tmp,0,sizeof(tmp));
    memset(file_path,0,sizeof(file_path));
    snprintf((char*)file_path,sizeof(file_path),"%s%s",MI_WLAN_TMP,MI_WLAN_SCAN_RESULT);
    MI_INFO("file_path == %s\n",file_path);
    fp = fopen((const char*)file_path,"r");
    if (!fp) {
        MI_INFO("Can not open file %s,please check !",file_path);
        return MI_ERR_FAILED;
    }

    i = 0;
    while (fgets((char*)tmp, sizeof(tmp), fp))
    {
        strncpy((char*)(result[i]),(const char*)tmp,strlen((const char*)tmp));
        i++;
    }
    fclose(fp);


    _MI_WLAN_ChoseChannel(atoi((const char*)(result[0])),&channel);
    statu->channel = channel;
    statu->RSSI = atoi((const char*)(result[1]));
    strncpy((char*)(statu->Bandwidth),(const char*)(result[2]),sizeof(result[2]));

    return MI_OK;
}

static MI_BOOL _MI_WLAN_StartAP(MI_WLAN_SOFTAP_WPAType_e mode, MI_U8 ssid[], MI_U8 passwd[])
{
    int fd;
    MI_S8 *wbuf = NULL;
    MI_S8 *fbuf = NULL;
    asprintf((char**)(&wbuf),"interface=wlan0\ndriver=nl80211\nctrl_interface=/system\nssid=%s\nchannel=6\nhw_mode=g\nignore_broadcast_ssid=0\n",ssid);
    MI_INFO("wbuf = %s \n",wbuf);

    switch(mode)
    {
        case E_MI_WLAN_SOFTAP_WPATYPE_OPEN:
            asprintf((char**)&fbuf,"%s",wbuf);
            break;

        case E_MI_WLAN_SOFTAP_WPATYPE_WPA:
            asprintf((char**)&fbuf,"%swpa=%d\nwpa_passphrase=%s\nwpa_key_mgmt=WPA-PSK\n",wbuf,mode,passwd);
            break;

        case E_MI_WLAN_SOFTAP_WPATYPE_WPA2:
            asprintf((char**)&fbuf,"%swpa=%d\nwpa_passphrase=%s\nwpa_key_mgmt=WPA-PSK\n",wbuf,mode,passwd);
            break;
    }
    MI_INFO("fbuf = %s\n",fbuf);

    fd = open((const char*)HOSTAPD_CONF_FILE, O_CREAT | O_TRUNC | O_WRONLY | O_NOFOLLOW, 0660);
    if(fd < 0)
    {
        MI_INFO("cann`t open %s\n",HOSTAPD_CONF_FILE);
        free(wbuf);
        free(fbuf);
        return MI_ERR_FAILED;
    }
    if(write(fd,fbuf,strlen((const char*)fbuf)) <0 )
    {
        MI_INFO("cann`t write to %s",HOSTAPD_CONF_FILE);
        free(wbuf);
        free(fbuf);
        return MI_ERR_FAILED;
    }
    free(wbuf);
    free(fbuf);

    MI_U8 au8Cmd[MI_WLAN_NORMAL_CMD_LEN]={0};
    memset(au8Cmd, 0x00 , sizeof(au8Cmd));
    snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s%s%s %s %s",MI_WLAN_SH,MI_WLAN_DEFAULT_FOLDER,MI_WLAN_FOLDER_MT7601U,MI_WLAN_SOFTAPSH,MI_WLAN_INTERFACE,_u8chipname);
    MI_INFO("%s\n",au8Cmd);
    return mi_sys_LaunchSystemCmd(au8Cmd);
}

static MI_BOOL _MI_WLAN_CloseAP()
{
    MI_U8 au8Cmd[MI_WLAN_NORMAL_CMD_LEN]={0};
    memset(au8Cmd, 0x00 , sizeof(au8Cmd));
    snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s%s%s",MI_WLAN_SH,MI_WLAN_DEFAULT_FOLDER,MI_WLAN_FOLDER_MT7601U,MI_WLAN_CLOSEAP);
    MI_INFO("%s\n",au8Cmd);
    return mi_sys_LaunchSystemCmd(au8Cmd);
}

static MI_BOOL _MI_WLAN_GetWlanChipVersion(MI_U8 *ChipVersion)
{
    memset(_u8chipname, 0x00 , sizeof(_u8chipname));

    MI_U8 au8Cmd[MI_WLAN_NORMAL_CMD_LEN]={0};
    memset(au8Cmd, 0x00 , sizeof(au8Cmd));
    snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s%s%s",MI_WLAN_SH,MI_WLAN_DEFAULT_FOLDER,MI_WLAN_FOLDER_MT7601U,MI_WLAN_GETWLANCHIP);
    MI_INFO("%s\n",au8Cmd);
    int ret =  mi_sys_SystemCmdWithReturn(au8Cmd);
    printf("ret = %d\n",ret);

    switch(ret)
    {
        case 1:
            strcpy((char*)ChipVersion,"MT7601");
            strcpy((char*)_u8chipname,"MT7601");
            break;

        case 2:
            strcpy((char*)ChipVersion,"8188ETV");
            strcpy((char*)_u8chipname,"8188ETV");
            break;

        default:

            break;
    }
    printf("ChipVersion is %s\n",ChipVersion);
    if(strcmp((const char*)ChipVersion,""))
    {
        return MI_OK;
    }
    return MI_ERR_FAILED;
}

static MI_BOOL _MI_WLAN_SetChannel(MI_U8 ch)
{
    MI_U8 au8Cmd[MI_WLAN_NORMAL_CMD_LEN]={0};
    memset(au8Cmd, 0x00 , sizeof(au8Cmd));

    if (strcmp((const char*)_u8chipname, "MT7601") == 0) // MT7601
    {
        MI_U8 au8Tmp[5]={0};
        memset(au8Tmp, 0x00 , sizeof(au8Tmp));
        if (ch < 8)
        {
            snprintf((char *)au8Tmp, sizeof(au8Tmp), "2%02d", ch); // 1 ~ 7 ch
        }
        else
        {
            snprintf((char *)au8Tmp, sizeof(au8Tmp), "3%02d", ch); // 8 ~ 13 ch
        }
        snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s %s MonitorChBw=%s", MI_WLAN_IWPRIV, MI_WLAN_INTERFACE, MI_WLAN_SET,au8Tmp);
    }
    else
    {
        snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s %s %s %d", MI_WLAN_IWCONFIG, MI_WLAN_INTERFACE, MI_WLAN_CHANNEL, ch);
    }
    MI_INFO("%s\n",au8Cmd);

    return mi_sys_LaunchSystemCmd(au8Cmd);
}

static MI_BOOL _MI_WLAN_TcpDump(MI_U8 *cmd)
{
    MI_U8 au8Cmd[MI_WLAN_NORMAL_CMD_LEN]={0};
    memset(au8Cmd, 0x00 , sizeof(au8Cmd));
    snprintf((char *)au8Cmd,sizeof(au8Cmd),"%s -i %s %s", MI_WLAN_TCPDUMP, MI_WLAN_INTERFACE, cmd);
    MI_INFO("%s\n",au8Cmd);
    return mi_sys_LaunchSystemCmd(au8Cmd);
}

static MI_BOOL _MI_WLAN_Detector_Thread()
{
    MI_S8 * ret = NULL;
    MI_WLAN_Status_t status;
    MI_U8 au8Cmd[MI_WLAN_NORMAL_CMD_LEN]={0};
    MI_U8 au8HostIp[MI_WLAN_NORMAL_CMD_LEN]={0};

    memset(&status, 0, sizeof(status));
    memset(au8Cmd, 0, sizeof(au8Cmd));
    memset(au8HostIp, 0, sizeof(au8HostIp));

    while(1)
    {
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        pthread_testcancel();

        _MI_WLAN_GetStatus(&status);
        if (strcmp((const char*)(status.wpa_state),"SCANNING") == 0)
        {
            snprintf((char *)au8Cmd,sizeof(au8Cmd),"ifconfig %s | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'", MI_WLAN_INTERFACE);
            MI_INFO("IN SCANNING State : %s\n",au8Cmd);

            FILE *fd = popen((char*)au8Cmd, "r");
            if(fd==NULL)
            {
                return MI_ERR_FAILED;
            }
            memset(au8HostIp, 0, sizeof(au8HostIp));
            ret = (MI_S8*)fgets((char*)au8HostIp, sizeof(au8HostIp), fd);
            if(ret != NULL)
            {
                memset(au8Cmd, 0, sizeof(au8Cmd));
                snprintf((char *)au8Cmd,sizeof(au8Cmd),"ip addr flush dev %s", MI_WLAN_INTERFACE);
                MI_INFO("%s\n",au8Cmd);
                mi_sys_LaunchSystemCmd(au8Cmd);
            }
        }
        else if (strcmp((const char*)(status.wpa_state),"COMPLETED") == 0)
        {
            snprintf((char *)au8Cmd,sizeof(au8Cmd),"ifconfig %s | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'", MI_WLAN_INTERFACE);
            MI_INFO("IN COMPLETED State : %s\n",au8Cmd);

            FILE *fd = popen((char*)au8Cmd, "r");
            if(fd==NULL)
            {
                return MI_ERR_FAILED;
            }
            memset(au8HostIp, 0, sizeof(au8HostIp));
            ret = (MI_S8*)fgets((char*)au8HostIp, sizeof(au8HostIp), fd);

            if(ret == NULL)
            {
                memset(au8Cmd, 0, sizeof(au8Cmd));
                snprintf((char *)au8Cmd,sizeof(au8Cmd),"udhcpc -i %s", MI_WLAN_INTERFACE);
                MI_INFO("%s\n",au8Cmd);
                mi_sys_LaunchSystemCmd(au8Cmd);
            }
        }
        sleep(MI_WLAN_SCAN_INTERVAL);
    }
}

static MI_RESULT _MI_WLAN_Detector(void)
{
    int ret;
    ret = pthread_create(&detector_thread,NULL,(void *)_MI_WLAN_Detector_Thread,NULL);
    if (ret != 0)
    {
        MI_INFO("create thread error\n");
        return MI_ERR_FAILED;
    }
    return MI_OK;
}

static MI_BOOL _MI_WLAN_Close_Detector_Thread()
{
    if( !pthread_cancel(detector_thread) )
    {
        printf( "pthread_cancel OK\n " );
        return MI_OK;
    }
    return MI_ERR_FAILED;
}

//------------------------------------------------------------------------------
/// @brief do wlan init , and change wlan status to E_MI_WLAN_STATUS_INITED
/// @param[in] *pstInitParams reserved
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: init fail.
/// @return MI_HAS_INITED: already inited.
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Init(MI_WLAN_InitParams_t *pstInitParams)
{
    LOCK_ENTER();
    CHECK_REINIT();
    CHECK_IS_EMPTY();
    CHECK_PARAM(pstInitParams);
    MI_INFO("pstInitParams->eModule 0x%x \n", pstInitParams->eModule);
    UNLOCK_FAIL_RETURN(_MI_WLAN_Init(pstInitParams));
    _MI_WLAN_SetStatus(E_MI_WLAN_STATUS_INITED);
    UNLOCK_RETURN(TRUE);
}

//------------------------------------------------------------------------------
/// @brief do wlan deinit , and change wlan status to E_MI_WLAN_STATUS_EMPTY
/// @param[in] none
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: deinit fail.
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_DeInit(void)
{
    LOCK_ENTER();
    CHECK_IS_INITED();
    UNLOCK_FAIL_RETURN(_MI_WLAN_DeInit());
    _MI_WLAN_SetStatus(E_MI_WLAN_STATUS_EMPTY);
    UNLOCK_RETURN(TRUE);
}
//------------------------------------------------------------------------------
/// @brief Open wlan handler, and change wlan status to E_MI_WLAN_STATUS_OPENED
/// @param[in] *pstParam memory pool id (cache & noncached)
/// @param[out] *phwlan wlan handler
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
/// @return MI_ERR_INVALID_HANDLE: Null handle
/// @return MI_ERR_INVALID_PARAMETER: Null parameter
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Open(MI_WLAN_OpenParams_t *pstParam, MI_HANDLE *phwlan)
{
    LOCK_ENTER();
    CHECK_IS_INITED();
    CHECK_PARAM(phwlan);
    //CHECK_PARAM(pstParam); //reserved
    UNLOCK_FAIL_RETURN(_MI_WLAN_Open(pstParam, phwlan));
    _MI_WLAN_SetStatus(E_MI_WLAN_STATUS_OPENED);
    UNLOCK_RETURN(TRUE);
}

//------------------------------------------------------------------------------
/// @brief close wlan handler, and change wlan status to E_MI_WLAN_STATUS_INITED
/// @param[in] hWLan wlan handle.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Close(MI_HANDLE hWLan)
{
    LOCK_ENTER();
    if(_eWLanStatus < E_MI_WLAN_STATUS_OPENED)
    {
        UNLOCK_RETURN(TRUE);
    }
    CHECK_HANDLE(hWLan);
    UNLOCK_FAIL_RETURN(_MI_WLAN_Close(hWLan));
    _MI_WLAN_SetStatus(E_MI_WLAN_STATUS_INITED);
    UNLOCK_RETURN(TRUE);
}

#ifdef NOSUIN
//------------------------------------------------------------------------------
/// @brief add device information to support list
/// @param[in] pstDeviceInfo: A pointer to structure MI_WLAN_DeviceInfo_t for device information.
/// @return MI_OK: Process success.
/// @return MI_ERR_NOT_INITED: Module not initialized.
/// @return MI_ERR_INVALID_PARAMETER: Parameters is invalid.
/// @return MI_ERR_FAILED: Process fail.
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_AddDeviceInfo(MI_WLAN_DeviceInfo_t *pstDeviceInfo)
{
    LOCK_ENTER();
    CHECK_PARAM(pstDeviceInfo);
    UNLOCK_FAIL_RETURN(_MI_WLAN_AddDeviceInfo(pstDeviceInfo));
    UNLOCK_RETURN(TRUE);
}
#endif

//------------------------------------------------------------------------------
/// @brief set wlan attr
/// @param[in] hWLan wlan handle.
/// @param[in] eParamType attr type.
/// @param[in] *pUsrParam attr param.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_SetAttr(MI_HANDLE hWLan, MI_WLAN_ParamType_e eParamType, void *pUsrParam)
{
    LOCK_ENTER();
    CHECK_OPENED();
    CHECK_HANDLE(hWLan);
    switch(eParamType)
    {
        case E_MI_WLAN_PARAMTYPE_BRIDGE_SETTING:
        {
            MI_WLAN_BridgeSetting_t* pstBrSetting = pUsrParam;
            UNLOCK_RETURN(_MI_WLAN_SetBridge(pstBrSetting));
            break;
        }
        default:
            break;
    }
    UNLOCK_RETURN(FALSE);
}

//------------------------------------------------------------------------------
/// @brief get wlan attr
/// @param[in] hWLan wlan handle.
/// @param[in] eParamType attr type.
/// @param[in] *pUsrParam attr param.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_GetAttr(MI_HANDLE hWLan, MI_WLAN_ParamType_e eParamType, void *pUsrParam)
{
    LOCK_ENTER();
    CHECK_OPENED();
//    CHECK_HANDLE(hWLan);
    CHECK_PARAM(pUsrParam);

    switch(eParamType)
    {
        case E_MI_WLAN_PARAMTYPE_CONNECTSTATUS:
            *(MI_BOOL*)pUsrParam = _eWLanStatus==E_MI_WLAN_STATUS_CONNECTED? TRUE:FALSE;
            UNLOCK_RETURN(TRUE)
            break;

        case E_MI_WLAN_PARAMTYPE_APINFO_BYINDEX:
        {
            MI_WLAN_QueryAPInfo_t* pQueryAPInfo = pUsrParam;
            UNLOCK_RETURN(_MI_WLAN_GetAPInfoByIdx(pQueryAPInfo->u8Idx, &pQueryAPInfo->stAPInfo));
            break;
        }

        case E_MI_WLAN_PARAMTYPE_INTERFACENAME:
            break;

        default:
            break;
    }

    UNLOCK_RETURN(FALSE);
}

//------------------------------------------------------------------------------
/// @brief connect, and change wlan status to E_MI_WLAN_STATUS_CONNECTING
/// @param[in] hWLan wlan handle.
/// @param[in] *pstConnectParam info param.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Connect(MI_HANDLE hWLan, MI_WLAN_ConnectParam_t *pstConnectParam)
{
    _MI_WLAN_SetStatus(E_MI_WLAN_STATUS_CONNECTING);
    LOCK_ENTER();
    CHECK_OPENED();
    //CHECK_HANDLE(hWLan);
    CHECK_PARAM(pstConnectParam);
    UNLOCK_FAIL_RETURN(_MI_WLAN_Connect(pstConnectParam));
    _MI_WLAN_SetStatus(E_MI_WLAN_STATUS_CONNECTED);
    UNLOCK_RETURN(TRUE);
    return MI_OK;
}

//------------------------------------------------------------------------------
/// @brief disconnect wlan, and change wlan status to E_MI_WLAN_STATUS_OPENED
/// @param[in] hWLan wlan handle.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Disconnect(MI_HANDLE hWLan)
{
    LOCK_ENTER();
    if(_eWLanStatus < E_MI_WLAN_STATUS_CONNECTED)
    {
        UNLOCK_RETURN(TRUE);
    }
    //CHECK_HANDLE(hWLan);
    UNLOCK_FAIL_RETURN(_MI_WLAN_Disconnect());
    _MI_WLAN_SetStatus(E_MI_WLAN_STATUS_OPENED);
    UNLOCK_RETURN(TRUE);
}

//------------------------------------------------------------------------------
/// @brief scan ap info, if use block mode, it will change to E_MI_WLAN_STATUS_SCANNING
/// @param[in] hWLan wlan handle.
/// @param[in] *pstParam info param.
/// @param[out] *pstResult result.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Scan(MI_HANDLE hWLan, MI_WLAN_ScanParam_t *pstParam, MI_WLAN_ScanResult_t *pstResult)
{
    LOCK_ENTER();
    CHECK_OPENED();
    //CHECK_HANDLE(hWLan);
    //CHECK_PARAM(pstParam);
    CHECK_PARAM(pstResult);
    if(_bAPClient == TRUE)
    {
        UNLOCK_FAIL_RETURN(_MI_WLAN_ScanIwpriv(pstResult));
    }
    else
    {
        UNLOCK_FAIL_RETURN(_MI_WLAN_Scan(pstResult));
    }
    UNLOCK_RETURN(TRUE);
}

//------------------------------------------------------------------------------
/// @brief set wlan debug level
/// @param[in] eDgbLevel debug level.
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_SetDebugLevel(MI_DBG_LEVEL_e eDgbLevel)
{
    _u8WLanDbgLevel = eDgbLevel;
    return MI_OK;
}

//------------------------------------------------------------------------------
/// @brief get current wlan status
/// @param[in] the struct to store wlan status
/// @return MI_OK: Process success.
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_GetStatus(MI_WLAN_Status_t *status)
{
    if (status == NULL)
    {
        return MI_ERR_FAILED;
    }
//    CHECK_IS_CONNECTED();
    return _MI_WLAN_GetStatus(status);
}

//------------------------------------------------------------------------------
/// @brief start AP
/// @param[in] mode AP's encrypt type
/// @param[in] ssid AP's SSID
/// @param[in] passwd AP's password
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_StartAP(MI_WLAN_SOFTAP_WPAType_e mode, MI_U8 ssid[], MI_U8 passwd[])
{
    return _MI_WLAN_StartAP(mode,ssid,passwd);
}

//------------------------------------------------------------------------------
/// @brief close AP
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_CloseAP()
{
    return _MI_WLAN_CloseAP();
}

//------------------------------------------------------------------------------
/// @brief get wlan chip version
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_GetWlanChipVersion(MI_U8 *ChipVersion)
{
    return _MI_WLAN_GetWlanChipVersion(ChipVersion);
}

//------------------------------------------------------------------------------
/// @brief set Network Type
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_SetNetworkType(MI_WLAN_NetworkType_e eNetworkType)
{
    return _MI_WLAN_SetNetworkType(eNetworkType) ? MI_OK : MI_ERR_FAILED;
}

//------------------------------------------------------------------------------
/// @brief set channe in monitor mode
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_SetChannel(MI_U8 ch)
{
    return _MI_WLAN_SetChannel(ch);
}

//------------------------------------------------------------------------------
/// @brief dump tcp data
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_TcpDump(MI_U8 *cmd)
{
    return _MI_WLAN_TcpDump(cmd);
}

//------------------------------------------------------------------------------
/// @brief Detection of network connection status
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Detector(void)
{
    return _MI_WLAN_Detector();
}

//------------------------------------------------------------------------------
/// @brief Close Detector thread
/// @return MI_OK: Process success
/// @return MI_ERR_FAILED: Process failed
//------------------------------------------------------------------------------
MI_RESULT MI_WLAN_Close_Detector_Thread(void)
{
    return _MI_WLAN_Close_Detector_Thread();
}
