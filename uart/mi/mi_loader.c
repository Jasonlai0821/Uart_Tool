#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>

#include "mi_common.h"
#include "mi_sys.h"
#include "mi_wlan.h"
#include "mi_flash.h"
#include "mi_ubi.h"

#include "mi_gpio.h"
#include "mi_rgbled.h"

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

typedef enum
{
    TYPE_PARTITION_MTD =0,
    TYPE_PARTITION_UBI,
    TYPE_PARTITION_MAX,
} OTA_PARITION_Type_e;

typedef enum
{
    PARTITION_UPGREADE_UPDATE =0,
    PARTITION_UPGREADE_ADD,
    PARTITION_UPGREADE_REMOVE,
    PARTITION_UPGREADE_RESIZE,
    PARTITION_UPGREADE_MAX,
} PARTITION_UPGREADE_Type_e;

typedef struct OTA_PARTITION_Info_s
{
    MI_BOOL has_data;
    MI_U8 partition_name[MI_FLASH_ENV_DATA_SIZE];
    OTA_PARITION_Type_e partition_type;
    PARTITION_UPGREADE_Type_e upgrade_type;
    MI_U32 partition_size;
    MI_U32 data_offset;
    MI_U32 data_len;
    MI_U32 haswrite;

} OTA_PARTITION_Info_t;

#define MI_FLASH_ENV_NAME_LEN_MAX    256
#define MI_FLASH_ENV_VALUE_LEN_MAX  512
#define OTA_PARTITION_MAX 24
#define DBG_LEVEL                       MI_DBG_ERR

#define OTA_INFO_SIZE 0x1000
#define LOADER_IMG_PATH "/tmp/ota.bin"

#define UPGREADE_UPDATE "update"
#define UPGREADE_ADD "add"
#define UPGREADE_REMOVE "remove"
#define UPGREADE_RESIZE "resize"

#define PARITION_MTD "MTD"
#define PARITION_UBI "UBI"

#define UBI_PARTITION_NAME "UBI"
#define KERNEL_PARTITION_NAME "kernel"
#define RECOVERY_PARTITION_NAME "RECOVERY"
#define RECOVERY_IMAGE "/data/recovery.xz.img"
#define RECOVERY_UPDATE_FLAG "/data/recovery_flag"

#define UPGRADE_FLAG "ota_tag"
#define OTA_URL "ota_url"
#define OTA_STATUS "ota_upgrade_status"



#define LED_R     101 //PAD_TTL_IO9
#define LED_G     119 //PAD_I2S1_BCK
#define LED_B     118 //PAD_I2S1_WS
#define LED_1     125 //PAD_I2S0_RX
#define LED_2     124 //PAD_I2S0_TX
#define LED_3     123 //PAD_I2S0_WS
#define LED_4     129 //PAD_JTAG_TDI

//for cmcc
#define SAR_LED_BLUE 101
#define SAR_LED_RED 118
#define SAR_LED_GREEN 119


#define LED_BAIDU     0
#define LED_MSTAR     1
#define LED_CMCC     2


#define LED_TYPE LED_CMCC

typedef struct OTA_ENV_s
{
    MI_U8 env_name[MI_FLASH_ENV_NAME_LEN_MAX];
    MI_U8 env_value[MI_FLASH_ENV_VALUE_LEN_MAX];
} OTA_ENV_s;


static OTA_PARTITION_Info_t ota_partition_info[OTA_PARTITION_MAX]={0};
static MI_HANDLE _hUbiDevice = 0;
MI_FLASH_EnvInfo_t GetCurEnv ;
int has_prepare_ota = 0;
static MI_BOOL update_suc = FALSE ;
static int need_update_partition = 0;
static int  data_total_size = 0;
static char runMode = 0; // 0: kernel app; 1: recovery

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


unsigned long str2int(char *str)
{
    unsigned char is_hex_type = 0;
    unsigned long out = 0;
    char c;
    int i, length;

    if (NULL == str)
    {
        return 0;
    }

    length = strlen(str);
    for(i=0; i<length; i++)
    {
        str[i] = toupper(str[i]);
    }

    if (str[0] == '0' && str[1] == 'X')
    {
        is_hex_type = 1;
    }

    i = (is_hex_type) ? 2 : 0;
    for (;i<length;i++)
    {
        c = str[i];
        if (is_hex_type)
        {
            if (c<'0' || (c>'9' && c<'A') || c>'F')
            {
                return 0;
            }
            c = (c>'9') ? (c-'A'+10) : (c-'0');
            out = (16*out)+c;
        }
        else
        {
            if (c<'0' || c>'9')
            {
                return 0;
            }
            c = c-'0';
            out = (10*out)+c;
        }
    }
    return out;
}


/*
partition_name=kernel,
partition_type=MTD,
partition_size=0x580000,
upgrade_type=update,
image_offset=0,
image_size=3323160

*/


static MI_BOOL string_to_ota_partitioninfo(char* ota_data_buf,char* stringbuf,OTA_PARTITION_Info_t * ota_partition_info)
{

    char *cur_pos = NULL;
    char *start = NULL;
    char *end = NULL;
    int len = 0;
    char *tmp_buffer = NULL;
    tmp_buffer = malloc(128);
    if(ota_partition_info == NULL || stringbuf == NULL ||tmp_buffer == NULL )
        return FALSE ;
    
    printf("\n[Partition Info]\n");
    
    cur_pos = stringbuf;
    start = strstr(cur_pos,"=");
    end = strstr(cur_pos,",");
    if(end - start > MI_FLASH_ENV_DATA_SIZE)
    {
        MI_ERR("length of partition name in ota info is too long!!!! \n");
        return FALSE ;
    }

    memcpy(ota_partition_info->partition_name,start+1,end-start-1);
    ota_partition_info->has_data = 1;
    cur_pos = end+1 ;
    printf("Partition name: %s\n", ota_partition_info->partition_name);

//to copy partiton type
    start = strstr(cur_pos,"=");
    end = strstr(cur_pos,",");
    len = end - start - 1;
    memset(tmp_buffer,0,128);
    memcpy(tmp_buffer,start+1,len);
    if (strcasecmp(tmp_buffer, PARITION_MTD) == 0)
    {
//        MI_INFO("partition type is mtd \n");
        ota_partition_info->partition_type = TYPE_PARTITION_MTD;
    }
    else if (strcasecmp(tmp_buffer, PARITION_UBI) == 0)
    {
//        MI_INFO("partition type is UBI \n");
        ota_partition_info->partition_type = TYPE_PARTITION_UBI;
    }else
    {
//        MI_INFO("partition type is UNKONWN \n");
        ota_partition_info->partition_type = TYPE_PARTITION_MAX;
    }
    cur_pos = end+1;
    printf("Partition type: %s\n", (ota_partition_info->partition_type == 0) ? "MTD" : "UBI");

    //to copy partition_size
    start = strstr(cur_pos,"=");
    end = strstr(cur_pos,",");
    len = end - start - 1;
    memset(tmp_buffer,0,128);
    memcpy(tmp_buffer,start+1,len);
    ota_partition_info->partition_size = str2int(tmp_buffer);
    cur_pos = end+1 ;
    printf("Partition size: 0x%08X\n", ota_partition_info->partition_size);

    //to copy partition_size
    start = strstr(cur_pos,"=");
    end = strstr(cur_pos,",");

    len = (end - start) - 1;
    memset(tmp_buffer,0,128);
    memcpy(tmp_buffer,start+1,len);

    if (strcasecmp(tmp_buffer, UPGREADE_UPDATE) == 0)
    {
//        MI_INFO("upgrade_type is PARTITION_UPGREADE_UPDATE \n");
        ota_partition_info->upgrade_type = PARTITION_UPGREADE_UPDATE;
    }
    else if (strcasecmp(tmp_buffer, UPGREADE_ADD) == 0)
    {
//        MI_INFO("upgrade_type is PARTITION_UPGREADE_UPDATE \n");
        ota_partition_info->upgrade_type = PARTITION_UPGREADE_ADD;
    }
    else if (strcasecmp(tmp_buffer, UPGREADE_REMOVE) == 0)
    {
//        MI_INFO("upgrade_type is PARTITION_UPGREADE_UPDATE \n");
        ota_partition_info->upgrade_type = PARTITION_UPGREADE_REMOVE;
    }
    else if (strcasecmp(tmp_buffer, UPGREADE_RESIZE) == 0)
    {
//        MI_INFO("upgrade_type is PARTITION_UPGREADE_UPDATE \n");
        ota_partition_info->upgrade_type = PARTITION_UPGREADE_RESIZE;
    }
    else
    {
//        MI_INFO("upgrade_type is UNKOWN \n");
        ota_partition_info->upgrade_type = PARTITION_UPGREADE_MAX;
    }
    cur_pos = end+1 ;
    printf("Upgrade type  : %d (0: UPDATE, 1: ADD, 2: REMOVE, 3: RESIZE)\n", ota_partition_info->upgrade_type);

    //to copy data_buffer
    start = strstr(cur_pos,"=");
    end = strstr(cur_pos,",");
    len = end - start - 1;
    memset(tmp_buffer,0,128);
    memcpy(tmp_buffer,start+1,len);
    ota_partition_info->data_offset = str2int(tmp_buffer);
    cur_pos = end+1 ;
    printf("Image offset  : 0x%08X\n", ota_partition_info->data_offset);

    //to copy partition_size
    start = strstr(cur_pos,"=");
    end = strstr(cur_pos,";");
    len = end - start - 1;
    memset(tmp_buffer,0,128);

    memcpy(tmp_buffer,start+1,len);
    ota_partition_info->data_len= str2int(tmp_buffer);
    data_total_size = data_total_size+ota_partition_info->data_len;
    printf("Image size    : 0x%08X\n\n", ota_partition_info->data_len);

    free(tmp_buffer);
    return true;
}

MI_BOOL ms_parse_otainfo(char* ota_buffer)
{
    MI_INFO("parse ota info -------> \n");
    int parse = 0;
    int len = 0;
    int partition_index = 0;
    need_update_partition = 0;
    data_total_size = 0;
    MI_INFO("&ota_partition_info= 0x%lx, ota_partition_info=0x%lx\n",&ota_partition_info,ota_partition_info);
    memset(&ota_partition_info,0,sizeof(OTA_PARTITION_Info_t)*OTA_PARTITION_MAX);
    MI_INFO("sizeof(OTA_PARTITION_Info_t)*OTA_PARTITION_MAX = 0x%lx \n",sizeof(OTA_PARTITION_Info_t)*OTA_PARTITION_MAX);

    while(parse < OTA_INFO_SIZE) {
        char * tmp_buffer = strstr(ota_buffer + parse, "partition_name");
        if(tmp_buffer == NULL) break;

        char * tmp_buffer1 = strstr(ota_buffer+parse,";");
        if(tmp_buffer1 == NULL) break;
        len = tmp_buffer1 - tmp_buffer;
        char * stringbuf = malloc(len+2);
        memset(stringbuf,0,len+2);
        memcpy(stringbuf,ota_buffer+parse,len+2);
        string_to_ota_partitioninfo(ota_buffer + OTA_INFO_SIZE,stringbuf,&ota_partition_info[partition_index]);
        partition_index++;
        parse = parse + len + 2;
        free(stringbuf);
    }

    need_update_partition =  partition_index + 1 ;
    return true;
}

//bootcmd=nand read 0x21000000 KERNEL 0x860000;bootm 0x21000000
static MI_BOOL update_bootcmd(char * env_name,MI_U32 kernel_size)
{
    char * bootcmdValue = NULL ;
    char *tmp_buffer = NULL ;
    char *start = NULL ;
    char *end = NULL ;
    int len = 0;
    MI_U32 old_kernel_size = 0;

    bootcmdValue = malloc(MI_FLASH_ENV_VALUE_LEN_MAX);

    if(bootcmdValue == NULL || env_name == NULL )
    {
        MI_ERR("malloc bootcmdValue  fail \n");
        goto    FAIL;
    }
    memset(bootcmdValue,0,MI_FLASH_ENV_VALUE_LEN_MAX);

    if(MI_OK != MI_FLASH_GetEnvVar(&GetCurEnv,env_name,bootcmdValue))
    {
        MI_ERR("get bootcmd fail \n");
        goto    FAIL;
    }
    MI_INFO("bootcmd= %s \n",bootcmdValue);

    start =strstr(bootcmdValue,"KERNEL");
    if(start == NULL )
        start = strstr(bootcmdValue,"kernel");
    if(start == NULL )
        goto    FAIL;
    MI_INFO("bootcmdValue=0x%lx ,end=0x%lx \n",bootcmdValue,start);
    start = strstr(start," ");
    MI_INFO("bootcmdValue=0x%lx ,start=0x%lx \n",bootcmdValue,start);
    if(start == NULL )
        goto    FAIL;
    MI_INFO("bootcmdValue=0x%lx ,start=0x%lx \n",bootcmdValue,start);

    end = strstr(bootcmdValue,";");

    len = end - start - 1;
    tmp_buffer = malloc(MI_FLASH_ENV_VALUE_LEN_MAX);
    if(tmp_buffer == NULL )
    {
        MI_INFO("malloc buffer fail \n");
        goto    FAIL;
    }
    memset(tmp_buffer,0,MI_FLASH_ENV_VALUE_LEN_MAX);
    memcpy(tmp_buffer,start+1,len);
    MI_INFO("tmp_buffer= %s \n",tmp_buffer);
    old_kernel_size  = str2int(tmp_buffer);
    if(kernel_size <= old_kernel_size)
    {
        MI_INFO("new kernel size is smaller than old kernel size ,donot to upgrade it \n");

        if(bootcmdValue != NULL)
            free(bootcmdValue);
        if(tmp_buffer != NULL)
            free(tmp_buffer);
        return TRUE;
    }

    memset(bootcmdValue, 0, MI_FLASH_ENV_VALUE_LEN_MAX);
    sprintf(bootcmdValue,"nand read 0x26000000 KERNEL 0x%x;bootm 0x26000000",kernel_size);
    MI_INFO("newbootcmd= %s!!! \n",bootcmdValue);
    if(MI_OK != MI_FLASH_SetEnvVar(&GetCurEnv,env_name,bootcmdValue))
    {
        MI_ERR("set bootcmd fail \n");
        goto    FAIL;
    }
    if(MI_OK != MI_FLASH_StoreEnv(E_MI_FLASH_TYPE_NAND,&GetCurEnv))
    {
        MI_ERR("store bootcmd fail \n");
        goto    FAIL;
    }

    return TRUE;
    FAIL:
        if(bootcmdValue != NULL)
            free(bootcmdValue);
        if(tmp_buffer != NULL)
            free(tmp_buffer);

        return FALSE;
}

static MI_BOOL update_recoverycmd(char * env_name,MI_U32 kernel_size)
{
    char * bootcmdValue = NULL ;
    char *tmp_buffer = NULL ;
    char *start = NULL ;
    char *end = NULL ;
    int len = 0;
    MI_U32 old_kernel_size = 0;

    bootcmdValue = malloc(MI_FLASH_ENV_VALUE_LEN_MAX);

    if(bootcmdValue == NULL || env_name == NULL )
    {
        MI_ERR("malloc bootcmdValue  fail \n");
        goto    FAIL;

    }
    memset(bootcmdValue,0,MI_FLASH_ENV_VALUE_LEN_MAX);

    if(MI_OK != MI_FLASH_GetEnvVar(&GetCurEnv,env_name,bootcmdValue))
    {
        MI_ERR("get bootcmd fail \n");
        goto    FAIL;

    }
    MI_INFO("bootcmd= %s \n",bootcmdValue);

    start =strstr(bootcmdValue,"RECOVERY");
    if(start == NULL )
        start = strstr(bootcmdValue,"RECOVERY");
    if(start == NULL )
        goto    FAIL;
    MI_INFO("bootcmdValue=0x%lx ,end=0x%lx \n",bootcmdValue,start);
    start = strstr(start," ");
    MI_INFO("bootcmdValue=0x%lx ,start=0x%lx \n",bootcmdValue,start);
    if(start == NULL )
        goto    FAIL;
    MI_INFO("bootcmdValue=0x%lx ,start=0x%lx \n",bootcmdValue,start);

    end = strstr(bootcmdValue,";");

    len = end - start - 1;
    tmp_buffer = malloc(MI_FLASH_ENV_VALUE_LEN_MAX);
    if(tmp_buffer == NULL )
    {
        MI_INFO("malloc buffer fail \n");
        goto    FAIL;
    }
    memset(tmp_buffer,0,MI_FLASH_ENV_VALUE_LEN_MAX);
    memcpy(tmp_buffer,start+1,len);
    MI_INFO("tmp_buffer= %s \n",tmp_buffer);
    old_kernel_size  = str2int(tmp_buffer);
    if(kernel_size <= old_kernel_size)
    {
        MI_INFO("new recovery size is smaller than old recovery size ,donot to upgrade it \n");

        if(bootcmdValue != NULL)
            free(bootcmdValue);
        if(tmp_buffer != NULL)
            free(tmp_buffer);
        return TRUE;

    }

    memset(bootcmdValue, 0, MI_FLASH_ENV_VALUE_LEN_MAX);
    sprintf(bootcmdValue,"nand read 0x26000000 RECOVERY 0x%x;bootm 0x26000000",kernel_size);
    MI_INFO("newbootcmd= %s!!! \n",bootcmdValue);
    if(MI_OK != MI_FLASH_SetEnvVar(&GetCurEnv,env_name,bootcmdValue))
    {
        MI_ERR("set bootcmd fail \n");
        goto    FAIL;

    }
    if(MI_OK != MI_FLASH_StoreEnv(E_MI_FLASH_TYPE_NAND,&GetCurEnv))
    {
        MI_ERR("store bootcmd fail \n");
        goto    FAIL;

    }


    return TRUE;
    FAIL:
        if(bootcmdValue != NULL)
            free(bootcmdValue);
        if(tmp_buffer != NULL)
            free(tmp_buffer);

        return FALSE;
}

static MI_BOOL update_partition(OTA_PARTITION_Info_t * ota_partition_info,char * data_buffer,MI_U32 offset,MI_U32 len)
{
    if(ota_partition_info == NULL || data_buffer == NULL )
    {
        return FALSE;
    }
    if(ota_partition_info->partition_type == TYPE_PARTITION_MTD)
    {
        if(ota_partition_info->upgrade_type != PARTITION_UPGREADE_UPDATE)
        {
            MI_ERR("MTD partiton just supoort update! \n");
            return FALSE;
        }
        else
        {
            if(ota_partition_info->data_len > 0)
            {
                if(strcasecmp(ota_partition_info->partition_name, KERNEL_PARTITION_NAME) == 0)
                {
                    if(update_bootcmd("bootcmd",ota_partition_info->data_len)!= TRUE)
                        return FALSE;
                }
                if(strcasecmp(ota_partition_info->partition_name, RECOVERY_PARTITION_NAME) == 0)
                {
                    if(update_recoverycmd("recoverycmd",ota_partition_info->data_len)!= TRUE)
                        return FALSE;
                }
                if(MI_FLASH_MTD_WRITE(ota_partition_info->partition_name,data_buffer,offset,len)!= len)
                {
                    MI_ERR("update ota_partition_info->partition_name = %s fail \n",ota_partition_info->partition_name);
                    return FALSE;
                }
                ota_partition_info->haswrite = ota_partition_info->haswrite + len;
            }

        }


    }
    else if(ota_partition_info->partition_type = TYPE_PARTITION_UBI)
    {
        MI_U32 u32VolId = 0;
        MI_UBI_VolumeAccess_t  stWriteBytes;
        MI_U32 VolumeByteSize = 0;
        memset((char *)&stWriteBytes,0,sizeof(MI_UBI_VolumeAccess_t));
        if(ota_partition_info->upgrade_type == PARTITION_UPGREADE_REMOVE)
        {
            if(MI_OK != MI_UBI_GetVolumeIdByName(_hUbiDevice,ota_partition_info->partition_name,&u32VolId))
            {
                MI_ERR("ubi volume:%s not found ! \n",ota_partition_info->partition_name);
                return FALSE;
            }
            if(MI_OK != MI_UBI_DestroyVolume(_hUbiDevice,u32VolId))
            {
                MI_ERR("remove partition %s fail ! \n",ota_partition_info->partition_name);
                return FALSE;
            }

        }else if(ota_partition_info->upgrade_type == PARTITION_UPGREADE_UPDATE)
        {
            if(MI_OK != MI_UBI_GetVolumeIdByName(_hUbiDevice,ota_partition_info->partition_name,&u32VolId))
            {
                MI_ERR("ubi volume:%s not found ! \n",ota_partition_info->partition_name);
                return FALSE;
            }
            MI_U64 u64VolumeByteSize = 0;
            if(MI_OK != MI_UBI_GetVolumeSize(_hUbiDevice,u32VolId,&u64VolumeByteSize))
            {
                MI_ERR("ubi volume:%s get size fail ! \n",ota_partition_info->partition_name);
                return FALSE;
            }
            VolumeByteSize = (MI_U32)u64VolumeByteSize;
            if(VolumeByteSize < ota_partition_info->data_len)
            {
                MI_ERR("ubi[%s]  VolumeByteSize[%lx] < ota_partition_info->data_len[%lx] ! \n",ota_partition_info->partition_name ,VolumeByteSize,ota_partition_info->data_len);
                return FALSE;
            }
            if(ota_partition_info->data_len == 0)
            {
                MI_ERR("ubi volume:%s do not need to upgrade image ! \n",ota_partition_info->partition_name);
                return TRUE;
            }
            stWriteBytes.u32VolId = u32VolId;
            stWriteBytes.u32ByteOffset = offset;
            stWriteBytes.pu8Buf = data_buffer;
            stWriteBytes.u32Length =  len;
            if(MI_UBI_WriteVolumeBytes(_hUbiDevice,&stWriteBytes) <  len)
            {
                MI_ERR("ubi volume:%s write fail ! \n",ota_partition_info->partition_name);
                return FALSE;

            }
            ota_partition_info->haswrite = ota_partition_info->haswrite + len;
        }
        else if(ota_partition_info->upgrade_type == PARTITION_UPGREADE_ADD)
        {

            if(MI_OK != MI_UBI_CreateVolume(_hUbiDevice,ota_partition_info->partition_name,(MI_U64)ota_partition_info->partition_size))
            {
                MI_ERR("create ubi volume:%s fail ! \n",ota_partition_info->partition_name);
                return FALSE;
            }
            if(MI_OK != MI_UBI_GetVolumeIdByName(_hUbiDevice,ota_partition_info->partition_name,&u32VolId))
            {
                MI_ERR("get ubi volume:%s id fail ! \n",ota_partition_info->partition_name);
                return FALSE;
            }
            MI_U64 u64VolumeByteSize = 0;
            if(MI_OK != MI_UBI_GetVolumeSize(_hUbiDevice,u32VolId,&u64VolumeByteSize))
            {
                MI_ERR("ubi volume:%s get size fail ! \n",ota_partition_info->partition_name);
                return FALSE;
            }
            VolumeByteSize = (MI_U32)u64VolumeByteSize;
            if(VolumeByteSize < ota_partition_info->data_len)
            {
                MI_ERR("ubi[%s]  VolumeByteSize[%lx] < ota_partition_info->data_len[%lx] ! \n",ota_partition_info->partition_name ,VolumeByteSize,ota_partition_info->data_len);
                return FALSE;
            }
            if(ota_partition_info->data_len == 0)
            {
                MI_ERR("ubi volume:%s do not need to upgrade image ! \n",ota_partition_info->partition_name);
                return TRUE;
            }
            stWriteBytes.u32VolId = u32VolId;
            stWriteBytes.u32ByteOffset = offset;
            stWriteBytes.pu8Buf = data_buffer;
            stWriteBytes.u32Length =  len;
            if(MI_UBI_WriteVolumeBytes(_hUbiDevice,&stWriteBytes) <  len)
            {
                MI_ERR("ubi volume:%s write fail ! \n",ota_partition_info->partition_name);
                return FALSE;

            }
            ota_partition_info->haswrite = ota_partition_info->haswrite + len;
        }
        else if(ota_partition_info->upgrade_type == PARTITION_UPGREADE_RESIZE)
        {

            if(MI_OK != MI_UBI_GetVolumeIdByName(_hUbiDevice,ota_partition_info->partition_name,&u32VolId))
            {
                MI_ERR("get ubi volume:%s id fail ! \n",ota_partition_info->partition_name);
                return FALSE;
            }
            if(MI_OK != MI_UBI_ResizeVolume(_hUbiDevice,u32VolId,(MI_S32)ota_partition_info->partition_size))
            {
                MI_ERR("resize ubi volume:%s fail ! \n",ota_partition_info->partition_name);
                return FALSE;
            }
            MI_U64 u64VolumeByteSize = 0;
            MI_UBI_GetVolumeSize(_hUbiDevice,u32VolId,&u64VolumeByteSize);
            VolumeByteSize = (MI_U32)u64VolumeByteSize;
            if(VolumeByteSize < ota_partition_info->data_len)
            {
                MI_ERR("ubi[%s]  VolumeByteSize[%lx] < ota_partition_info->data_len[%lx] ! \n",ota_partition_info->partition_name ,VolumeByteSize,ota_partition_info->data_len);
                return FALSE;
            }
            if(ota_partition_info->data_len == 0)
            {
                MI_ERR("ubi volume:%s do not need to upgrade image ! \n",ota_partition_info->partition_name);
                return TRUE;
            }
            stWriteBytes.u32VolId = u32VolId;
            stWriteBytes.u32ByteOffset = offset ;
            stWriteBytes.pu8Buf = data_buffer;
            stWriteBytes.u32Length = len;
            if(MI_UBI_WriteVolumeBytes(_hUbiDevice,&stWriteBytes) <  len)
            {
                MI_ERR("ubi volume:%s write fail ! \n",ota_partition_info->partition_name);
                return FALSE;

            }
            ota_partition_info->haswrite = ota_partition_info->haswrite + len;

        }
        else
        {
            MI_ERR("ubi action not supoort! \n");
            return FALSE;

        }
    }
    return TRUE;
}

int get_date_write_info(MI_U32 offset , MI_U32 buffer_unwrite_len ,MI_U32 *partition_offset, MI_U32 *write_len)
{
    int index = 0;

    for(index = 0; index < OTA_PARTITION_MAX ;index++)
    {
        if(offset >= data_total_size)
        {
            return need_update_partition;
        }
        if(ota_partition_info[index].has_data)
        {
            if((offset == ota_partition_info[index].data_offset )||((offset > ota_partition_info[index].data_offset) && (offset < (ota_partition_info[index].data_offset + ota_partition_info[index].data_len))))
            {
                *partition_offset = ota_partition_info[index].haswrite ;
                if(buffer_unwrite_len > (ota_partition_info[index].data_len - ota_partition_info[index].haswrite))
                {
                    *write_len = ota_partition_info[index].data_len - ota_partition_info[index].haswrite;
                }else
                {
                    *write_len = buffer_unwrite_len ;
                }

                MI_INFO("[%s]partition_offset: %08X, write_len: %08X\n", ota_partition_info[index].partition_name, *partition_offset, *write_len);

                return index;
            }
         }
    }

    return -1;

}

int ms_getenv(char * envname,char * envvalue)
{
    if(envvalue == NULL || envname == NULL )
    {
        printf("envvalue or envname is null \n");
         return -1;
    }

    MI_U8 aszValue [ MI_FLASH_ENV_VALUE_LEN_MAX ]={0};
    memset(aszValue,0,MI_FLASH_ENV_VALUE_LEN_MAX);

    if(MI_OK != MI_FLASH_GetEnvVar(&GetCurEnv, envname, aszValue))
    {
        printf("get env %s fail \n",envname);
        return  -1;
    }
    memcpy(envvalue,aszValue,strlen(aszValue));
    printf("envvalue:%s \n",envvalue);
    printf("aszValue:%s \n",aszValue);
    return 0;
}

int ms_setenv(char * envname, char * envvalue)
{
    if(envvalue == NULL ||envname == NULL )
        return -1;
    MI_U8 aszValue [ MI_FLASH_ENV_VALUE_LEN_MAX ]={0};
    memset(aszValue,0,MI_FLASH_ENV_VALUE_LEN_MAX);
    memcpy(aszValue,envvalue,strlen(envvalue));
    if(MI_OK != MI_FLASH_SetEnvVar(&GetCurEnv, envname, aszValue))
    {
        printf("set env %s fail \n",envname);
        return  -1;
    }
    if(MI_OK != MI_FLASH_StoreEnv(E_MI_FLASH_TYPE_NAND, &GetCurEnv))
    {
        printf("set env %s fail \n",envname);
        return  -1;
    }
     return  0;
}
int ms_ota_update_start(size_t offset, char * buffer, size_t data_size)
{
    int ret = 1;
    int partition_index = 0;
    MI_U32 data_write = 0 ;
    MI_U32 partition_offset = 0 ;
    MI_U32 write_len = 0 ;
    MI_U32 cur_offset = 0;

    if(!has_prepare_ota)
    {
        MI_ERR("prepare_ota shuold be call first \n");
        return 0;
    }

    cur_offset = offset ;
    printf("offset=[%08X] ,buffer=[%08X],data_size=[%08X],need_update_partition=[%d]\n",offset,buffer,data_size,need_update_partition);
    while(data_write < data_size)
    {
        partition_index = get_date_write_info((cur_offset - OTA_INFO_SIZE), (data_size - data_write), &partition_offset, &write_len);
        if(partition_index < 0) {
            ret = 0 ;
            break;
        }
        if(partition_index == need_update_partition) {
            ret = 1;
            MI_INFO("all partition has been update, need_update_partition= %d \n",need_update_partition);
            break;
        }
        MI_INFO("write partition: %s , data_write=[%08X],offset = %08X ,len = %08X \n",
        ota_partition_info[partition_index].partition_name, data_write, partition_offset, write_len);

        if (runMode == 0) {
            if (strstr(ota_partition_info[partition_index].partition_name, RECOVERY_PARTITION_NAME) != NULL) {
                if (!update_partition(&ota_partition_info[partition_index], buffer + data_write, partition_offset, write_len)) {
                    MI_ERR("write partition err \n");
                    ret = 0 ;
                    break;
                }
            }
            else {
                ota_partition_info[partition_index].haswrite = ota_partition_info[partition_index].haswrite + write_len;
                printf("Do not upgrade %s partition in runMode(%d)\n", ota_partition_info[partition_index].partition_name, runMode);
            }
        }
        else { // runMode == 1
            if (strstr(ota_partition_info[partition_index].partition_name, RECOVERY_PARTITION_NAME) == NULL) {
                if (!update_partition(&ota_partition_info[partition_index], buffer + data_write, partition_offset, write_len)) {
                    MI_ERR("write partition err \n");
                    ret = 0;
                    break;
                }
            }
            else {
                creat(RECOVERY_UPDATE_FLAG, S_IRUSR);
                sync();
                ota_partition_info[partition_index].haswrite = ota_partition_info[partition_index].haswrite + write_len;
                printf("Do not upgrade %s partition in runMode(%d)\n", ota_partition_info[partition_index].partition_name, runMode);
            }
        }

        data_write = data_write + write_len;
        cur_offset = cur_offset + write_len;
        MI_INFO("write partition done: %s , data_write=[%08X],offset = %08X ,len = %08X \n",ota_partition_info[partition_index].partition_name, data_write, partition_offset, write_len);
    }
    return ret;
}

int ms_write_update_file(size_t offset, char * file_name, size_t data_size)
{
    int ret = 0;
    FILE *fp = NULL ;
    char * ota_buffer = NULL;
    if(!has_prepare_ota)
    {
        MI_ERR("prepare_ota shuold be call first \n");
        return  ret;
    }


    fp = fopen(file_name,"r+w");
    if(fp == NULL )
    {
        MI_ERR("file open file \n");
        return 0;
    }

    ota_buffer = malloc(data_size);
    if(ota_buffer == NULL )
    {
        MI_ERR("malloc buffer fail");
        fclose(fp);
        return 0;
    }
    memset(ota_buffer,0,data_size);
    fseek(fp, 0, SEEK_SET);
    fread(ota_buffer, data_size,1, fp);
    fclose(fp);
    ret = ms_ota_update_start(offset, ota_buffer, data_size);
    free(ota_buffer);
    return ret;
}

int ms_update_recovery()
{
    int ret = -1;
    FILE *fp = NULL ;
    char * ota_buffer = NULL;
    int filesize = 0;
    char cmdline[256] = {0};

    if(!has_prepare_ota)
    {
        MI_ERR("prepare_ota shuold be call first \n");
        return  0;
    }

    fp = fopen(RECOVERY_IMAGE,"r+w");
    if(fp == NULL )
    {
        MI_ERR("file open fail \n");
        return 1;
    }
    fseek(fp, 0L, SEEK_END);
    filesize = ftell(fp);
    ota_buffer = malloc(filesize);

    if(ota_buffer == NULL )
    {
        MI_ERR("malloc buffer fail");
        fclose(fp);
        return ret;
    }
    memset(ota_buffer,0,filesize);
    fseek(fp, 0, SEEK_SET);
    fread(ota_buffer, filesize,1, fp);
    fclose(fp);
    if(MI_FLASH_MTD_WRITE(RECOVERY_PARTITION_NAME, ota_buffer, 0, filesize) > 0)
    {
        sprintf(cmdline, "rm %s", RECOVERY_IMAGE);
        system(cmdline);
        ret = 1;
    }

    free(ota_buffer);
    return ret;
}

int prepare_ota(void)
{
    has_prepare_ota = 0;

    if(MI_ERR_FAILED == MI_FLASH_Init(NULL))
    {
        MI_ERR("mi flash init fail \n");
        return FALSE;
    }
    if(MI_OK != MI_FLASH_LoadEnv(E_MI_FLASH_TYPE_NAND,&GetCurEnv))
    {
        MI_ERR("loader env fail \n");
        return FALSE;
    }

    char upgrade_status[4] = {0};
    if (ms_getenv(OTA_STATUS, upgrade_status) != 0) {
        MI_ERR("get %s fail \n", upgrade_status);
        exit(1);
    }
    runMode = atoi(upgrade_status);

    MI_UBI_InitParam_t stInitParam;
    memset(&stInitParam, 0, sizeof(stInitParam));
    if(MI_UBI_Init(&stInitParam)!=MI_OK)
    {
        MI_ERR("UBI module init fail!\n");
        return FALSE;
    }

    MI_UBI_OpenParam_t stOpenParam;
    MI_U32 u32DevId = 0;

    if(MI_UBI_GetDevIdByPartName(UBI_PARTITION_NAME, &u32DevId)!=MI_OK)
    {
        MI_ERR("Get device Id of partition UBI fail\n");
        return FALSE;
    }
    MI_INFO("Partition UBI, device id=%lu!!!\n", u32DevId);

    memset(&stOpenParam, 0, sizeof(stOpenParam));
    stOpenParam.u32DeviceId =  u32DevId;
    if(MI_UBI_Open(&stOpenParam, &_hUbiDevice)!=MI_OK)
    {
        MI_ERR("open device %lu fail\n", u32DevId);
        return FALSE;
    }
    if(_hUbiDevice == NULL )
    {
        MI_ERR("open _hUbiDevice %lu fail\n", _hUbiDevice);
        return FALSE;
    }
    has_prepare_ota = 1;

    return TRUE;
}

void * thread_update (void *arg)
{
    printf("starting loader ----------> \n ");
    unsigned long filesize = -1;
    FILE *fp = NULL ;
    char * ota_buffer = NULL;
    char crcbuffer[4]={0};
    MI_U32 OTA_CRC_VALUE = 0;
    MI_U32 OTA_CRC[1]={0};


    fp = fopen(LOADER_IMG_PATH,"r+w");
    if(fp == NULL )
    {
        MI_ERR("file open file \n");
        exit(0);
    }

    fseek(fp, 0L, SEEK_END);
    filesize = ftell(fp);
    ota_buffer = malloc(filesize);
    if(ota_buffer == NULL )
    {
        MI_ERR("malloc buffer fail");
        fclose(fp);
        exit(0);
    }
    MI_INFO("LINE=%d filesize= 0x%lx\n",__LINE__,filesize);
    memset(ota_buffer,0,filesize);
    fseek(fp, 0, SEEK_SET);
    fread(ota_buffer, filesize,1, fp);
    fclose(fp);
    OTA_CRC_VALUE = MI_FLASH_CRC32(0,ota_buffer,filesize-4);

    memcpy(OTA_CRC,ota_buffer+filesize-4,4);
    MI_ERR("OTA_CRC=%lx,OTA_CRC_VALUE=%lx \n",OTA_CRC[0],OTA_CRC_VALUE);
    if(OTA_CRC[0]!= OTA_CRC_VALUE)
    {
        MI_ERR("OTA BIN CRC CHECK FAIL, the data ota.bin may has been lost,exit!!!  \n");
        exit(0);
    }
    ms_parse_otainfo(ota_buffer);
    ms_ota_update_start(0, ota_buffer, filesize);

}


void led_init(void)
{
#if(LED_TYPE ==LED_BAIDU)
    MI_GPIO_Init(LED_R);
    MI_GPIO_Init(LED_G);
    MI_GPIO_Init(LED_B);
    MI_GPIO_Init(LED_1);
    MI_GPIO_Init(LED_2);
    MI_GPIO_Init(LED_3);
    MI_GPIO_Init(LED_4);
    printf("LED is baidu board \n");
#elif(LED_TYPE ==LED_MSTAR)
        printf("LED is mstar board \n");
#elif(LED_TYPE ==LED_CMCC)
     printf("LED is cmcc board \n");
     MI_GPIO_Init(SAR_LED_BLUE);
     MI_GPIO_Init(SAR_LED_RED);
     MI_GPIO_Init(SAR_LED_GREEN);
#else
     printf("no define led \n");
#endif
}
void led_on_off(int led_num, int on_off)
{
    if(on_off)
    {
        MI_GPIO_SetOutput(led_num, E_MI_GPIO_LEVEL_OFF);
    }
    else
    {
        MI_GPIO_SetInput(led_num);
    }
}

void led_rgb_on_off(int rgb_num, int on_off)
{
    if(on_off)
    {
        MI_GPIO_SetOutput(rgb_num, E_MI_GPIO_LEVEL_ON);
    }
    else
    {
        MI_GPIO_SetOutput(rgb_num, E_MI_GPIO_LEVEL_OFF);
    }
}

void led_all_r_on(void)
{
#if(LED_TYPE ==LED_BAIDU)
    led_on_off(LED_1, 1);
    led_on_off(LED_2, 1);
    led_on_off(LED_3, 1);
    led_on_off(LED_4, 1);
    led_rgb_on_off(LED_R, 0);
    led_rgb_on_off(LED_G, 1);
    led_rgb_on_off(LED_B, 0);
#elif(LED_TYPE ==LED_MSTAR)
    MI_RGBLED_SetColor(0x00F000,0,0,0,0,0,0,0);
#elif(LED_TYPE ==LED_CMCC)

     MI_GPIO_SetOutput(SAR_LED_BLUE, E_MI_GPIO_LEVEL_ON);
     MI_GPIO_SetOutput(SAR_LED_RED, E_MI_GPIO_LEVEL_ON);
     MI_GPIO_SetOutput(SAR_LED_GREEN, E_MI_GPIO_LEVEL_ON);
#else

#endif

}

void led_all_off(void)
{
#if(LED_TYPE == LED_BAIDU)

    led_on_off(LED_1, 0);
    led_on_off(LED_2, 0);
    led_on_off(LED_3, 0);
    led_on_off(LED_4, 0);
    led_rgb_on_off(LED_R, 0);
    led_rgb_on_off(LED_G, 0);
    led_rgb_on_off(LED_B, 0);
#elif(LED_TYPE ==LED_MSTAR)
    MI_RGBLED_SetColor(0,0,0,0,0,0,0,0);
#elif(LED_TYPE ==LED_CMCC)
     MI_GPIO_SetOutput(SAR_LED_BLUE, E_MI_GPIO_LEVEL_OFF);
     MI_GPIO_SetOutput(SAR_LED_RED, (SAR_LED_BLUE, E_MI_GPIO_LEVEL_OFF));
     MI_GPIO_SetOutput(SAR_LED_GREEN, (SAR_LED_BLUE, E_MI_GPIO_LEVEL_OFF));
#else

#endif


}


//loader main_loop entry
//int main(int argc, char* argv[])
int update_ota_test_demo(void)
{
    int ret = 0;
    MI_INFO("loader env suc \n");
    MI_U8 update_flag [MI_FLASH_ENV_VALUE_LEN_MAX] = {0};
    MI_U8 ota_url [MI_FLASH_ENV_VALUE_LEN_MAX] = {0};
    if(!prepare_ota())
    {
       MI_ERR("prepare ota fail ,exit !!!  \n");
       exit(0);
    }

    if(MI_OK != MI_FLASH_GetEnvVar(&GetCurEnv,UPGRADE_FLAG,update_flag))
    {
        MI_ERR("get  UPGRADE_FLAG fail \n");
        return -1;
    }
    if(MI_OK != MI_FLASH_GetEnvVar(&GetCurEnv,OTA_URL,ota_url))
    {
        MI_ERR("get  OTA_URL fail \n");
        return -1;
    }


    MI_INFO("UPGRADE_FLAG = %s ,OTA_URL = %s \n",update_flag,ota_url);
    //

    //if ota download success ,begin to update !
    pthread_t tid;
    if(pthread_create(&tid, NULL, thread_update, NULL) == -1)
    {
        puts("fail to create pthread t0");
        exit(1);
    }

    void * result;
    if(pthread_join(tid, &result) == -1){
        puts("fail to recollect t0");
        exit(1);
    }
    while(!update_suc)
    {

        sleep(2);
    }
    return 0;
}



