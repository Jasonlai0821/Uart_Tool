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

#ifndef _DRV_GPIO_H_
#define _DRV_GPIO_H_

#include <asm/types.h>
#include "mdrv_types.h"

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
void MDrv_GPIO_Init(void);
void MDrv_GPIO_Pad_Set(U32 u32IndexGPIO);
void MDrv_GPIO_Pad_Unset(U32 u32IndexGPIO);
void MDrv_GPIO_Pad_Oen(U32 u32IndexGPIO);
void MDrv_GPIO_Pad_Odn(U32 u32IndexGPIO);
U8 MDrv_GPIO_Pad_Read(U32 u32IndexGPIO);
U8 MDrv_GPIO_Pad_InOut(U32 u32IndexGPIO);
void MDrv_GPIO_Pull_High(U32 u32IndexGPIO);
void MDrv_GPIO_Pull_Low(U32 u32IndexGPIO);
void MDrv_GPIO_Set_High(U32 u32IndexGPIO);
void MDrv_GPIO_Set_Low(U32 u32IndexGPIO);
//void MDrv_Enable_GPIO_INT(U32 u32IndexGPIO);
int MDrv_GPIO_To_Irq(U32 u32IndexGPIO);
//void MDrv_GPIO_Set_POLARITY(U32 u32IndexGPIO, U8 reverse);

#endif // _DRV_GPIO_H_

