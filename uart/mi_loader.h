///////////////////////////////////////////////////////////////////////////////////////////////////
//
// * Copyright (c) 2006 - 2017 MStar Semiconductor, Inc.
// This program is free software.
// You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation;
// either version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program;
// if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   mdrv_gpio.h
/// @brief  GPIO Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MI_LOADER_H_
#define _MI_LOADER_H_


#ifdef __cplusplus
extern "C"{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
unsigned long str2int(char *str);

int ms_getenv(char * envname,char * envvalue);

int ms_setenv(char * envname, char * envvalue);

//ota 之前需要调用该接口做初始化，否则升级失败
//bool prepare_ota(void);

/* 数据写入接口，下载模块可以把数据以存放的buffer传给底层，一定要告诉底层当前数据在原始OTA.BIN 中的偏移
    input
    offset :准备写入的数据在ota.bin 中的偏移
    buffer： 写入数据的内存地址
    data_size： 写入数据的size

*/
int ms_ota_update_start(size_t offset, char * buffer, size_t data_size);

/* 数据写入接口 ，下载模块可以把数据以文件的形式传给底层，底层根据文件名自己去拿数据，一定要告诉底层当前数据在原始OTA.BIN 中的偏移
    input
    offset :准备写入的数据在ota.bin 中的偏移
    buffer： 写入数据的文件名字
    data_size： 写入数据的size

*/
int ms_write_update_file(size_t offset, char * file_name, size_t data_size);

int prepare_ota(void);
MI_BOOL ms_parse_otainfo(char* ota_buffer);

#ifdef __cplusplus
}
#endif


#endif //

