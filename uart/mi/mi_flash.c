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
/// @file   mi_flash.c
/// @brief Serial Flash module
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mount.h>  // for _IOW, _IOR, mount()
#include <sys/stat.h>
#include <mtd/mtd-user.h>
#include <assert.h>

//#include "MsCommon.h"
#include "mi_common.h"
#include "mi_sys.h"
#include "mi_os.h"
#include "mi_flash.h"

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Defines
//-------------------------------------------------------------------------------------------------
#define DBG_LEVEL   MI_DBG_FW



//#define DEV_UNFDBLOCK_NAME "/dev/unfdblk0"

#define MI_MTD_NAME_PATH            "/sys/devices/virtual/mtd/mtd%lu/name"
#define MI_DEV_PATT                 "/dev/block/mtdblock%lu"
#define NAME_MAX_LEN 64
#define MTD_MAX_COUNT 24

#define DEV_ENV_PARTITION_NAME "env"

#define MI_FLASH_NAND_SECTOR_SIZE       (512)
#define MI_FLASH_POLYNOMIAL_EXPONENTIAL (31)


#define MI_FLASH_WRITE_ERROR_RETRY_TIME     5
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
static MI_U8 _gu8FlashDbgLevel = MI_DBG_ERR;
static MI_BOOL _gbIsFlashModuleInit = FALSE;

static MI_S32 _gs32FlashMutexId = -1;
static MI_U32 _gu32EnvStart[2] = {0};
static MI_BOOL _bLoadEnvSuccess[E_MI_FLASH_TYPE_MAX] = {FALSE};

static MI_S64 _s64DataBytes=0;
static MI_S32 _s32UsableEbSize=0;
static MI_S32 _s32DevNum=0;
static MI_S32 _s32VolId=0;

static MI_U32 _gu32CrcTable[256] = {0};
static MI_BOOL _gbIsCrcTableEmpty = TRUE;

static char *_gCurrentPartitionName=NULL ;
static int _gCurrentPartitionBadblock=0;


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


typedef struct {
    MtdPartition *partitions;
    int partitions_allocd;
    int partition_count;
} MtdState;

static MtdState g_mtd_state = {
    NULL,   // partitions
    0,      // partitions_allocd
    -1      // partition_count
};

struct MtdPartition {
    int device_index;
    unsigned int size;
    unsigned int erase_size;
    char *name;
};


#define MTD_PROC_FILENAME   "/proc/mtd"


static int mtd_scan_partitions()
{
    char buf[2048];
    const char *bufp;
    int fd;
    int i;
    ssize_t nbytes;

    if (g_mtd_state.partitions == NULL) {
        const int nump = 32;
        MtdPartition *partitions = malloc(nump * sizeof(*partitions));
        if (partitions == NULL) {
            errno = ENOMEM;
            return -1;
        }
        g_mtd_state.partitions = partitions;
        g_mtd_state.partitions_allocd = nump;
        memset(partitions, 0, nump * sizeof(*partitions));
    }
    g_mtd_state.partition_count = 0;

    /* Initialize all of the entries to make things easier later.
     * (Lets us handle sparsely-numbered partitions, which
     * may not even be possible.)
     */
    for (i = 0; i < g_mtd_state.partitions_allocd; i++) {
        MtdPartition *p = &g_mtd_state.partitions[i];
        if (p->name != NULL) {
            free(p->name);
            p->name = NULL;
        }
        p->device_index = -1;
    }

    /* Open and read the file contents.
     */
    fd = open(MTD_PROC_FILENAME, O_RDONLY);
    if (fd < 0) {
        goto bail;
    }
    nbytes = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    if (nbytes < 0) {
        goto bail;
    }
    buf[nbytes] = '\0';

    /* Parse the contents of the file, which looks like:
     *
     *     # cat /proc/mtd
     *     dev:    size   erasesize  name
     *     mtd0: 00080000 00020000 "bootloader"
     *     mtd1: 00400000 00020000 "mfg_and_gsm"
     *     mtd2: 00400000 00020000 "0000000c"
     *     mtd3: 00200000 00020000 "0000000d"
     *     mtd4: 04000000 00020000 "system"
     *     mtd5: 03280000 00020000 "userdata"
     */
    bufp = buf;
    while (nbytes > 0) {
        int mtdnum, mtdsize, mtderasesize;
        int matches;
        char mtdname[64];
        mtdname[0] = '\0';
        mtdnum = -1;

        matches = sscanf(bufp, "mtd%d: %x %x \"%63[^\"]",
                &mtdnum, &mtdsize, &mtderasesize, mtdname);
        /* This will fail on the first line, which just contains
         * column headers.
         */
        if (matches == 4) {
            MtdPartition *p = &g_mtd_state.partitions[mtdnum];
            p->device_index = mtdnum;
            p->size = mtdsize;
            p->erase_size = mtderasesize;
            p->name = strdup(mtdname);
            if (p->name == NULL) {
                errno = ENOMEM;
                goto bail;
            }
            g_mtd_state.partition_count++;
        }

        /* Eat the line.
         */
        while (nbytes > 0 && *bufp != '\n') {
            bufp++;
            nbytes--;
        }
        if (nbytes > 0) {
            bufp++;
            nbytes--;
        }
    }

    return g_mtd_state.partition_count;

bail:
    // keep "partitions" around so we can free the names on a rescan.
    g_mtd_state.partition_count = -1;
    return -1;
}

static MtdPartition * mtd_find_partition_by_name(const char *name)
{
    if (g_mtd_state.partitions != NULL) {
        int i;
        for (i = 0; i < g_mtd_state.partitions_allocd; i++) {
            MtdPartition *p = &g_mtd_state.partitions[i];
            if (p->device_index >= 0 && p->name != NULL) {
                if (strcasecmp(p->name, name) == 0) {
                    printf("p->name=[%s] \n",p->name);
                    return p;
                }
            }
        }
    }
    return NULL;
}

static ssize_t mtd_read_data(MtdPartition * partition, char *data , size_t offset , size_t len)
{
    //int read = 0;
    int readin = 0;
    int fd = -1;
    char mtddevname[32];
    char * buffer =NULL ;
    int mgbb;
    buffer = malloc(partition->erase_size);

    if(buffer == NULL )
        goto READ_FAIL;
    memset(buffer,0,partition->erase_size);

    sprintf(mtddevname, "/dev/mtd/mtd%d", partition->device_index);
    fd = open(mtddevname, O_RDONLY);

    if(fd < 0)
    {
        MI_ERR("open mtddevname= %s fail !!!!! \n",mtddevname);
        goto READ_FAIL;
    }


    loff_t pos = offset;
    while(readin < len)
    {
        if(pos%(partition->erase_size)!=0)
        {
            while((mgbb = ioctl(fd, MEMGETBADBLOCK, &pos)==1))
            {
                fprintf(stderr,"mtd: MEMGETBADBLOCK returned %d at 0x%08llx (errno=%d)\n",mgbb, pos, errno);
                pos += partition->erase_size;
                if(pos > partition->size)
                {
                    MI_ERR("read size is larger than  partition size,because there are too many bad block !!!!! \n");
                    goto READ_FAIL;
                }

            }
            //cur_seek = pos%(partition->erase_size);
            pos = lseek(fd, pos, SEEK_SET);
            readin = partition->erase_size - (pos%(partition->erase_size));
            MI_INFO("line=[%d] ,read =[%lx],pos=[%lx] \n",__LINE__,readin,pos);
            read(fd, buffer, readin);
            if(readin > len)
            {
                memcpy(data,buffer,len);
                readin = len;
            }else
            {
                memcpy(data,buffer,readin);
            }

            pos = pos + readin;
            MI_INFO("line=[%d] ,read =[%lx],pos=[%lx] \n",__LINE__,readin,pos);
            //cur_seek = pos;
        }
        else
        {
            MI_INFO("line=[%d] ,read =[%lx],pos=[%lx] \n",__LINE__,readin,pos);
            while((mgbb = ioctl(fd, MEMGETBADBLOCK, &pos)==1))
            {
                fprintf(stderr,
                    "mtd: MEMGETBADBLOCK returned %d at 0x%08llx (errno=%d)\n",
                    mgbb, pos, errno);
                pos += partition->erase_size;
                if(pos > partition->size)
                {
                    MI_ERR("read size is larger than  partition size,because there are too many bad block !!!!! \n");
                    goto READ_FAIL;
                }

            }
            pos = lseek(fd, pos , SEEK_SET);
            read(fd, buffer, partition->erase_size);
            MI_INFO("line=[%d] ,read =[%lx],pos=[%lx] \n",__LINE__,readin,pos);
            if((readin+partition->erase_size) > len)
            {
                int copysize = (len+offset)%partition->erase_size;
                memcpy(data+readin,buffer,copysize);
                readin = len;
                MI_INFO("line=[%d] ,read =[%lx],pos=[%lx] \n",__LINE__,readin,pos);

            }else
            {
                memcpy(data+readin,buffer,partition->erase_size);
                readin +=partition->erase_size;
                pos += partition->erase_size;
            }

        }

    }

    close(fd);
    free(buffer);
    return readin;

READ_FAIL:
    if(fd>0)
        close(fd);
    if(buffer !=NULL)
        free(buffer);
    return -1;
}

static ssize_t mtd_write_data(MtdPartition *partition, char *data, size_t offset,size_t len)
{
    int writein = 0;
    int fd = -1;
    char mtddevname[32];
    char * buffer =NULL ;
    int mgbb;
    struct erase_info_user erase_info;
    buffer = malloc(partition->erase_size);

    if(buffer == NULL )
        goto READ_FAIL;

    sprintf(mtddevname, "/dev/mtd/mtd%d", partition->device_index);
    fd = open(mtddevname, O_RDWR);
    if(fd < 0)
        goto READ_FAIL;

    MI_INFO("line=[%d] ,offset =[%lx],len=[%lx] \n",__LINE__,offset,len);
    if ((offset==0)||(strcmp(_gCurrentPartitionName,partition->name) != 0))
    {
        memcpy(_gCurrentPartitionName,partition->name,strlen(partition->name));
        MI_INFO("_gCurrentPartitionName :[%s] \n",_gCurrentPartitionName);
        _gCurrentPartitionBadblock =0;
    }
    loff_t pos = offset+_gCurrentPartitionBadblock*partition->erase_size;
    MI_INFO("line=[%d] ,offset =[%lx],len=[%lx] pos=[%lx] \n",__LINE__,offset,len,pos);
    loff_t erasepos = 0;
    while(writein < len)
    {
        memset(buffer,0,partition->erase_size);
        if(pos%(partition->erase_size)!=0)
        {
            while((mgbb = ioctl(fd, MEMGETBADBLOCK, &pos)==1))
            {
                fprintf(stderr,"mtd: MEMGETBADBLOCK returned %d at 0x%08llx (errno=%d)\n",mgbb, pos, errno);
                pos += partition->erase_size;
                _gCurrentPartitionBadblock++;
                if(pos > partition->size)
                {
                    MI_ERR("write size is larger than  partition size,because there are too many bad block !!!!! \n");
                    goto READ_FAIL;
                }

            }
            writein = partition->erase_size - (pos%(partition->erase_size));
            MI_INFO("line=[%d] ,writein =[%lx],pos=[%lx] \n",__LINE__,writein,pos);

            erasepos = (pos/partition->erase_size)*partition->erase_size;
            lseek(fd, (pos/partition->erase_size)*partition->erase_size, SEEK_SET);
            read(fd,buffer,partition->erase_size);

            erase_info.start = erasepos;
            erase_info.length = partition->erase_size ;

            if(writein > len)
            {
                MI_INFO("line=[%d] , offset + readsize is before the fisrt block we read =[%lx],pos=[%lx] \n",__LINE__,writein,pos);
                memcpy(buffer+(pos%(partition->erase_size)),data,len);
                writein = len;
            }
            else
            {
                MI_INFO("line=[%d] , read part of block which offset addr in  =[%lx],pos=[%lx] \n",__LINE__,writein,pos);
                memcpy(buffer+(pos%(partition->erase_size)),data,writein);
            }
            //struct erase_info_user erase_info;
            erase_info.start = (pos/partition->erase_size)*partition->erase_size;
            erase_info.length = partition->erase_size ;
            int retry = 0;
            MI_INFO("line=[%d] :mtd: erase at 0x%08lx \n",__LINE__,erase_info.start);
            for(retry = 0;retry <3; retry ++)
            {
                if (ioctl(fd, MEMERASE, &erase_info) < 0)
                {
                    MI_ERR("mtd: erase failure at 0x%08lx (%s)\n",erase_info.start, strerror(errno));
                    retry ++;

                }else
                {
                    break;
                }
            }
            if(retry == 3)
            {
                MI_ERR("line=[%d] ,erase block fail mark it as  bad block writein =[%lx],pos=[%lx] \n",__LINE__,pos);
                ioctl(fd, MEMSETBADBLOCK, &pos);
                writein = 0;
                pos = offset;
                continue;

            }
            MI_INFO("line=[%d] , erasepos=[%lx],pos=[%lx] \n",__LINE__,erasepos,pos);
            lseek(fd, erasepos , SEEK_SET);
            if(write(fd, buffer, partition->erase_size) < 0 )
            {
                MI_WRN("line=[%d] ,write fail =[%lx],pos=[%lx] \n",__LINE__,pos);
                continue;
            }
            pos = pos + writein;
            MI_INFO("line=[%d] ,read =[%lx],pos=[%lx] \n",__LINE__,writein,pos);
            //cur_seek = pos;
        }
        else
        {
            printf("line=[%d] ,write =[%lx],pos=[%lx] \n",__LINE__,writein,pos);
            while((mgbb = ioctl(fd, MEMGETBADBLOCK, &pos)==1))
            {
                fprintf(stderr,
                    "mtd: MEMGETBADBLOCK returned %d at 0x%08llx (errno=%d)\n",
                    mgbb, pos, errno);
                pos += partition->erase_size;
                _gCurrentPartitionBadblock++;
                if(pos > partition->size)
                {
                    printf("write size is larger than  partition size,because there are too many bad block !!!!! \n");
                    goto READ_FAIL;
                }

            }
            erase_info.start = pos;
            erase_info.length = partition->erase_size ;
            pos = lseek(fd, pos , SEEK_SET);
            MI_INFO("len=[%d] ,writein =[%lx],pos=[%lx] \n",len,writein,pos);
            if((writein+partition->erase_size) > len)
            {
                int copysize = (len+offset)%partition->erase_size;
                read(fd,buffer,partition->erase_size);
                memcpy(buffer,data+writein,copysize);
                //writein = len;
                MI_INFO("len=[%d] ,read =[%lx],pos=[%lx] \n",len,writein,pos);

            }else
            {
                memcpy(buffer,data+writein,partition->erase_size);

            }
            int retry = 0;
            for(int retry = 0;retry <3; retry ++)
            {
                if (ioctl(fd, MEMERASE, &erase_info) < 0)
                {
                    MI_WRN("mtd: erase failure at 0x%08lx (%s)\n",pos, strerror(errno));
                    retry ++;

                }else
                {
                    break;
                }

            }
            if(retry == 3)
            {
                printf("line=[%d] ,erase block fail mark it as  bad block writein =[%lx],pos=[%lx] \n",__LINE__,pos);
                ioctl(fd, MEMSETBADBLOCK, &pos);
                continue;

            }
            printf("line=[%d] ,writein =[%lx],pos=[%lx] \n",__LINE__,writein,pos);
            lseek(fd, erase_info.start, SEEK_SET);
            if(write(fd, buffer, partition->erase_size) < 0 )
            {
                MI_WRN("line=[%d] ,write fail =[%lx],pos=[%lx] \n",__LINE__,pos);
                continue;
            }
            if((writein+partition->erase_size) > len)
                writein = len;
            else
            {
                writein +=partition->erase_size;
                pos += partition->erase_size;
            }

            MI_INFO("line=[%d] ,writein =[%lx],pos=[%lx] \n",__LINE__,writein,pos);

        }

    }

    close(fd);
    free(buffer);
    return writein;

READ_FAIL:
    if(fd>0)
        close(fd);
    if(buffer !=NULL)
        free(buffer);
    return -1;
}


static void _MI_FLASH_CreateCRCTable(void)
{
    MI_U32 u32Crc;
    MI_S32 i, j;
    MI_U32 u32Poly;            /* polynomial exclusive-or pattern */
    /* terms of polynomial defining this crc (except x^32): */
    static const MI_U8 u8polynomial[] = {0,1,2,4,5,7,8,10,11,12,16,22,23,26};

    /* make exclusive-or pattern from polynomial (0xedb88320L) */
    u32Poly = 0L;
    for (i = 0; i < sizeof(u8polynomial)/sizeof(MI_U8); i++)
    {
        u32Poly |= 1L << (MI_FLASH_POLYNOMIAL_EXPONENTIAL - u8polynomial[i]);
    }
    for (i = 0; i < 256; i++)
    {
        u32Crc = (MI_U32)i;
        for (j = 0; j < 8; j++)
        {
            u32Crc = u32Crc & 1 ? u32Poly ^ (u32Crc >> 1) : u32Crc >> 1;
        }
        _gu32CrcTable[i] = u32Crc;
    }
    _gbIsCrcTableEmpty = FALSE;
}

static MI_U32 _crc32 (MI_U32 u32Crc, const MI_U8 *pu8Buf, MI_U32 u32Len)
{
#define DO1(pu8Buf)  u32Crc = _gu32CrcTable[((int)u32Crc ^ (*pu8Buf)) & 0xff] ^ (u32Crc >> 8);pu8Buf++;
#define DO2(pu8Buf)  DO1(pu8Buf); DO1(pu8Buf);
#define DO4(pu8Buf)  DO2(pu8Buf); DO2(pu8Buf);
#define DO8(pu8Buf)  DO4(pu8Buf); DO4(pu8Buf);

    if(_gbIsCrcTableEmpty)
    {
        _MI_FLASH_CreateCRCTable();
    }

    u32Crc = u32Crc ^ 0xffffffffL;
    while(u32Len >= 8)
    {
        DO8(pu8Buf);
        u32Len -= 8;
    }

    if(u32Len)
    {
        do
        {
            DO1(pu8Buf);
        }
        while(--u32Len);
    }

    MI_INFO("CRC check Value = %08lX \n",u32Crc ^ 0xffffffffL);
    return u32Crc ^ 0xffffffffL;
}

static MI_BOOL _MI_FLASH_GetEnvStr(MI_FLASH_EnvInfo_t *pstEnvInfo, MI_U32 u32Index, MI_U8 *pu8Buf, MI_U32 *pu32size)
{
    MI_U32 u32Idx = u32Index, u32size;

    pu8Buf[0] = '\0';
    u32size = 0 ;

    while ( '\0' !=  pstEnvInfo->u8Data[u32Idx])
    {
        pu8Buf[u32size] = pstEnvInfo->u8Data[u32Idx];
        u32size ++;
        u32Idx ++;
    }
    pu8Buf[u32size] = '\0';

    *pu32size  = u32size ;

    if (0 == u32size) return FALSE ;

    return TRUE ;


}

static MI_U32 _MI_FLASH_Name(MI_U8 *pu8Buf, MI_U32 u32Size)
{

    MI_U32 i = 0 ;

    for(i = 0 ;  i < u32Size ; i ++)
    {
        if( '=' == pu8Buf[i]) return i ;
    }
    return 0;
}

static void _MI_FLASH_DumpEnv(MI_FLASH_EnvInfo_t *pstEnv)
{

    MI_U32 i ;

    MI_PRINT("\n============== Dump Data ==============\n");

    for( i =  0 ; i < 0x40 ; i ++ )
    {
        if( 0 == (i % 16) )
            MI_PRINT("\n");

        MI_PRINT("%02x ",pstEnv->u8Data[i]);
    }

    MI_PRINT("\n============== Dump End ==============\n");
}


static void _MI_FLASH_Dumpbuffer(char *pstEnv)
{

    MI_U32 i ;

    MI_PRINT("\n============== Dump Data ==============\n");

    for( i =  0 ; i < 0x40 ; i ++ )
    {
        if( 0 == (i % 16) )
            MI_PRINT("\n");

        MI_PRINT("%02x ",pstEnv);
    }

    MI_PRINT("\n============== Dump End ==============\n");
}

static MI_BOOL _MI_NAND_RwENVPartition(MI_BOOL bWrite, MI_U32 u32PartType, MI_U32 u32StartOffset, MI_U8 *pu8Buf,  MI_U32 u32Length)
{
    int fd=-1, ret;
    MI_BOOL bRet = FALSE;
    MtdPartition * gEnvPartition = mtd_find_partition_by_name(DEV_ENV_PARTITION_NAME);

    //printf("gEnvPartition->name :[%s] \n",gEnvPartition->name);

    if(gEnvPartition == NULL )
    {
        MI_ERR("GET PARTITION INFO FAIL !!\n");
        return FALSE ;
    }

    if(pu8Buf==NULL)
    {
        MI_ERR("Invalid Parameter\n");
        return FALSE;
    }

    if(bWrite == 0)
    {
        int checklen = mtd_read_data(gEnvPartition, pu8Buf,0, u32Length);
        if (checklen <= 0)
        {
             MI_ERR("Fail to read %d bytes", (int)u32Length);

            goto error;
        }

    }
    else
    {
        int checklen = mtd_write_data(gEnvPartition, pu8Buf,0, u32Length);
        if (checklen <= 0)
        {
             MI_ERR("Fail to read %d bytes", (int)u32Length);
            goto error;
        }

    }

    bRet = TRUE;
error:

    return bRet;
}

static MI_BOOL _MI_FLASH_SetEnvVal(MI_FLASH_EnvInfo_t *pstEnvInfo  ,MI_U8* pu8Name, MI_U8* pu8Value)
{
    MI_U32 i = 0, u32Cnt = 0;

    while(1)
    {
        if ((i + 1) >= MI_FLASH_ENV_DATA_SIZE)
        {
            return FALSE;
        }
        if ((0 == pstEnvInfo->u8Data[i] ) && (0 == pstEnvInfo->u8Data[i+1])) // find the tail .
        {
            i = i + 1 ;
            while ( 0 != pu8Name[u32Cnt] ) // copy name section ;
            {
                pstEnvInfo->u8Data[i]  = pu8Name[u32Cnt] ;
                i ++;
                u32Cnt ++;
                if (i >= MI_FLASH_ENV_DATA_SIZE)
                {
                    MI_ERR("Environment size OverfLow!!\n");
                    return FALSE ;
                }
            }

            pstEnvInfo->u8Data[i] = '=' ;
            i ++;
            u32Cnt = 0 ;

            while ( 0 != pu8Value[u32Cnt] ) // copy value section
            {
                if(i<(MI_FLASH_ENV_SIZE - sizeof(MI_U32)))
                {
                    pstEnvInfo->u8Data[i]  = pu8Value[u32Cnt] ;
                }
                else
                {
                    return FALSE ;
                }
                i ++;
                u32Cnt ++;
                if (i >= MI_FLASH_ENV_DATA_SIZE)
                {
                    MI_ERR("Environment size OverfLow!!\n");
                    return FALSE ;
                }
            }

            if ((i + 1) >= MI_FLASH_ENV_DATA_SIZE)
            {
                MI_ERR("Environment size OverfLow!!\n");
                return FALSE ;
            }
            // add tail flag
            pstEnvInfo->u8Data[i] = 0 ;
            i ++;
            pstEnvInfo->u8Data[i] = 0 ;
            i ++;
            return TRUE ;
        }
        i ++ ;
    }
    return FALSE;

}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MI_RESULT MI_FLASH_Init(MI_FLASH_InitParam_t *pstInitParam)
{
    MI_ENTER();
    //static MtdPartition * partition = NULL ;
    if(_gbIsFlashModuleInit)//inited already
    {
        MI_ERR("Inited already!!\n");
        return MI_HAS_INITED;
    }
    if(mtd_scan_partitions() <= 0)
    {
        MI_ERR("GET PARTITION INFO FAIL !!\n");
        return MI_ERR_FAILED ;
    }
	_gCurrentPartitionName = malloc(sizeof(char)*64);
	memset(_gCurrentPartitionName,0,sizeof(char)*64);

    //memcpy((char *)gEnvPartition[0],partition,sizeof(partition));
    _gbIsFlashModuleInit=TRUE;
    MI_EXIT_OK();
    return MI_OK;
}

MI_RESULT MI_FLASH_DeInit(void)
{
    MI_ENTER();
    MI_U8 u8Index;

    _gbIsFlashModuleInit=FALSE;
    for(u8Index=E_MI_FLASH_TYPE_SPI; u8Index<E_MI_FLASH_TYPE_MAX; u8Index++)
    {
        _bLoadEnvSuccess[u8Index]=FALSE;
    }
    MI_EXIT_OK();
    return MI_OK;
}

MI_RESULT MI_FLASH_LoadEnv(MI_FLASH_Type_e eType, MI_FLASH_EnvInfo_t *pstGetCurEnv)
{
    MI_ENTER();
    if(!_gbIsFlashModuleInit)
    {
        MI_ERR("ERROR Not inited\n");
        return MI_ERR_NOT_INITED;
    }

    if(pstGetCurEnv==NULL || (eType<E_MI_FLASH_TYPE_SPI || eType>=E_MI_FLASH_TYPE_MAX))
    {
        MI_ERR("Invalid Parameter");
        return MI_ERR_INVALID_PARAMETER;
    }

    if(eType == E_MI_FLASH_TYPE_SPI)
    {
        MI_ERR("Error not support flash type\n");
        return MI_ERR_FAILED;
    }

     else if(eType ==E_MI_FLASH_TYPE_NAND)
    {
        if(_MI_NAND_RwENVPartition(FALSE, 0, 0, (MI_U8 *)pstGetCurEnv, MI_FLASH_ENV_SIZE )== FALSE)
        {
            MI_ERR("non linux OS, current can't support UBI Nand access!! \n");
            return MI_ERR_FAILED;
        }
        if(pstGetCurEnv->u32Crc != _crc32(0, pstGetCurEnv->u8Data, MI_FLASH_ENV_DATA_SIZE))
        {
            MI_ERR("u32Crc value in nand:%08lX!!", pstGetCurEnv->u32Crc ^ 0xffffffffL);
            return false;
        }
        _bLoadEnvSuccess[E_MI_FLASH_TYPE_NAND]=TRUE;
    }
    else
    {
        MI_ERR("Error not support flash type\n");
        return MI_ERR_FAILED;
    }
    MI_EXIT_OK();
    return MI_OK;
}

MI_RESULT MI_FLASH_StoreEnv(MI_FLASH_Type_e eType, MI_FLASH_EnvInfo_t *pstPutNewEnv)
{

    MI_BOOL ret = FALSE;
    MI_ENTER();

    if(!_gbIsFlashModuleInit)
    {
        MI_ERR("ERROR Not inited!!\n");
        return MI_ERR_NOT_INITED;
    }

    if (!pstPutNewEnv)
    {
        MI_ERR("Parameter error!!\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    if(eType == E_MI_FLASH_TYPE_SPI)
    {
        MI_ERR("Error not support flash type\n");
    }

    else if(eType ==E_MI_FLASH_TYPE_NAND)
    {
        if(_bLoadEnvSuccess[E_MI_FLASH_TYPE_NAND]==FALSE)
        {
            MI_ERR("ERROR Not Load...Call MI_FLASH_LoadEnv first!!\n");
            return MI_ERR_FAILED;
        }

        if(pstPutNewEnv->u32Crc != _crc32(0, pstPutNewEnv->u8Data, MI_FLASH_ENV_DATA_SIZE))
        {
            MI_ERR("crc error!!\n");
            return MI_ERR_FAILED;
        }

        ret = _MI_NAND_RwENVPartition(TRUE,0,0,(MI_U8 *)pstPutNewEnv,MI_FLASH_ENV_SIZE);
    }
    else
    {
        MI_ERR("Error not support flash type\n");
        return MI_ERR_FAILED;
    }

    if (ret == FALSE)
    {
        MI_ERR("ENV write back fail !!! \n");
        return MI_ERR_FAILED;
    }

    MI_EXIT_OK();
    return MI_OK;
}

MI_RESULT MI_FLASH_GetEnvVar(MI_FLASH_EnvInfo_t *pstCurEnv, MI_U8 *pszName, MI_U8 aszValue[MI_FLASH_ENV_VALUE_LEN_MAX])
{

    MI_U32 size = 0, index = 0, NameSize = 0;//tmpindex = 0 ,
    MI_U8 name[MI_FLASH_ENV_NAME_LEN_MAX], data[MI_FLASH_ENV_VALUE_LEN_MAX];
    MI_ENTER();

    if(!_gbIsFlashModuleInit)
    {
        MI_ERR("ERROR Not inited!!\n");
        return MI_ERR_NOT_INITED;
    }

    if(pstCurEnv==NULL)
    {
        MI_ERR("ERROR Not Load...Call MI_FLASH_LoadEnv first!!\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    while(_MI_FLASH_GetEnvStr(pstCurEnv, index, data, &size))
    {
        //MI_PRINT("ENV  >> %s \n",data);
        NameSize = _MI_FLASH_Name(data, size) ;
        strncpy((char*)name ,(char*)data, NameSize) ;
        name[NameSize] = '\0';
        //MI_PRINT("Name >> %s \n",name);
        if (0 == strcmp((const char *)name, (const char *)pszName))
        {
            MI_U32 u32ValueLen = size-NameSize;

            if(u32ValueLen > MI_FLASH_ENV_VALUE_LEN_MAX)
            {
                MI_WRN("Too long value length\n");
                return MI_ERR_FAILED;
            }
            memcpy(aszValue, &(pstCurEnv->u8Data[index + NameSize + 1 ]), size-NameSize);
            MI_EXIT_OK();
            return MI_OK;
        }
        index += size + 1;
    }

    MI_ERR("Get Env String fail!!\n");
    return MI_ERR_FAILED;
}

MI_RESULT MI_FLASH_SetEnvVar(MI_FLASH_EnvInfo_t *pstEnvInfo, MI_U8 *pszName, MI_U8 aszValue[MI_FLASH_ENV_VALUE_LEN_MAX])
{
    MI_U32 size = 0, index = 0, tmpindex = 0, NameSize;
    MI_U8 name[MI_FLASH_ENV_NAME_LEN_MAX], data[MI_FLASH_ENV_VALUE_LEN_MAX];
    MI_FLASH_EnvInfo_t *pstOrgEnvInfo=NULL;

    MI_ENTER();
    if(!_gbIsFlashModuleInit)
    {
        MI_ERR("Flash module not inited!!\n");
        return MI_ERR_NOT_INITED;
    }

    if(pstEnvInfo==NULL)
    {
        MI_ERR("Invalid Parameter!!\n");
        return MI_ERR_INVALID_PARAMETER;
    }

    MI_INFO("SetEnv : %s = %s \n", pszName, aszValue);

    pstOrgEnvInfo = (MI_FLASH_EnvInfo_t *)malloc(sizeof(MI_FLASH_EnvInfo_t));
    memcpy(pstOrgEnvInfo, pstEnvInfo, sizeof(MI_FLASH_EnvInfo_t));

    tmpindex = 0 ;
    pstEnvInfo->u32Crc = 0 ;
    memset(pstEnvInfo->u8Data, 0, MI_FLASH_ENV_DATA_SIZE);
    while (_MI_FLASH_GetEnvStr(pstOrgEnvInfo, index, data, &size))
    {
        //MI_PRINT("ENV  >> %s \n",data);
        NameSize = _MI_FLASH_Name(data, size ) ;
        strncpy((char*)name ,(char*)data, NameSize ) ;
        name[NameSize] = '\0';
        //MI_PRINT("Name >> %s \n",name);
        if (0 != strcmp((const char *)name, (const char *)pszName))
        {
            memcpy( &pstEnvInfo->u8Data[tmpindex], data, size );
            tmpindex += size ;
            pstEnvInfo->u8Data[tmpindex] = 0x0 ;
            tmpindex+= 1;
        }
        index += size + 1;
    }

    pstEnvInfo->u8Data[tmpindex] = 0 ;
    if (!_MI_FLASH_SetEnvVal( pstEnvInfo, pszName, aszValue))
    {
        MI_ERR("Set Env Fail !\n");
        free(pstOrgEnvInfo);
        return MI_ERR_FAILED;
    }

    pstEnvInfo->u32Crc = _crc32(0, pstEnvInfo->u8Data, MI_FLASH_ENV_DATA_SIZE);
    free(pstOrgEnvInfo);
    MI_EXIT_OK();
    return MI_OK;
}

/*
MI_BOOL MI_FLASH_FINDMTD_BY_NAME(MI_U8 * PartitionName, MI_U8 * DevicePath)
{
    int fd = -1;
    MI_U8 szFilePath[strlen(MI_MTD_NAME_PATH) + 100];
    MI_U8 pu8Buf[NAME_MAX_LEN]={0};
    MI_U8 ptmpBuf[NAME_MAX_LEN]={0};
    if(PartitionName == NULL || DevicePath == NULL)
    {
        MI_ERR("Invalid Parameter\n");
        return FALSE;
    }
    int index = 0;
    for(index = 0;index < MTD_MAX_COUNT ;index++)
    {
        sprintf((char *)szFilePath,MI_MTD_NAME_PATH,index);
        printf("----------->DevicePath = %s    \n",szFilePath);
        memset(pu8Buf,0,NAME_MAX_LEN);
        memset(ptmpBuf,0,NAME_MAX_LEN);
        fd = open(szFilePath, O_RDONLY);
        if(fd == -1)
        {
            MI_ERR("partiton [%s] is not a mtd partition   \n",PartitionName);
            return FALSE;
        }
        lseek(fd, 0, SEEK_SET);
        read(fd, pu8Buf,NAME_MAX_LEN);
        close(fd);
        printf("----------->pu8Buf = %s,    \n",pu8Buf);
        printf("----------->PartitionName = %s,    \n",PartitionName);
        sscanf(pu8Buf,"%[^\n]",ptmpBuf);
        if(strcasecmp(ptmpBuf,PartitionName) == 0)
        {
            sprintf(DevicePath,MI_DEV_PATT,index);
            break;
        }
    }
    if(index == MTD_MAX_COUNT )
    {
        MI_ERR("partiton [%s] is not a mtd partition   \n",PartitionName);
        return FALSE;
    }
    return TRUE;
}
*/

MI_U32 MI_FLASH_MTD_READ(MI_U8 * partitionName, MI_U8 * pu8Buf ,MI_U32 u32StartOffset,MI_U32 u32Length)
{
    MI_U32 checklen = -1;
    MtdPartition * partition = NULL;

    if(pu8Buf == NULL || partitionName == NULL)
    {
        MI_ERR("Invalid Parameter\n");
        return checklen;
    }
    partition = mtd_find_partition_by_name(partitionName);

    if(partition == NULL)
    {
        MI_ERR("can not find partition:[%s] \n",partitionName);
        return checklen;
    }
    checklen = mtd_read_data(partition,pu8Buf,u32StartOffset,u32Length);
    if (checklen <= 0)
    {
         MI_ERR("Fail to read %d bytes", (int)u32Length);
         return checklen;
    }

    return checklen;

}

MI_U32 MI_FLASH_MTD_WRITE(MI_U8 * partitionName, MI_U8 * pu8Buf ,MI_U32 u32StartOffset,MI_U32 u32Length)
{
    MI_U32 checklen = -1;
    MtdPartition * partition = NULL;
    if(pu8Buf == NULL || partitionName == NULL)
    {
        MI_ERR("Invalid Parameter\n");
        return checklen;
    }
    partition = mtd_find_partition_by_name(partitionName);
    if(partition == NULL)
    {
        MI_ERR("can not find partition:[%s] \n",partitionName);
        return checklen;
    }
    checklen = mtd_write_data(partition,pu8Buf,u32StartOffset,u32Length);
    if (checklen <= 0)
    {
         MI_ERR("Fail to read %d bytes", (int)u32Length);
         return checklen;
    }
    return checklen;
}


MI_U32 MI_FLASH_CRC32 (MI_U32 u32Crc, const MI_U8 *pu8Buf, MI_U32 u32Len)
{
    return _crc32(u32Crc, pu8Buf, u32Len);
}



MI_RESULT MI_FLASH_SetDebugLevel(MI_DBG_LEVEL_e eDbgLevel)
{
    _gu8FlashDbgLevel = eDbgLevel;
    return MI_OK;
}


