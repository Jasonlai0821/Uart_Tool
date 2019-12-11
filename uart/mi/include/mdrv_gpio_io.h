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
/// @file   mdrv_gpio_io.h
/// @brief  GPIO Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <asm/types.h>
#include "mdrv_types.h"
#include <asm/ioctl.h>
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
struct GPIO_Reg
{
    U32 u32Reg;
    U8 u8Enable;
    U8 u8BitMsk;
} __attribute__ ((packed));

typedef struct GPIO_Reg GPIO_Reg_t;

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define IOCTL_GPIO_INIT_NR                              (0)
#define IOCTL_GPIO_SET_NR                               (1)
#define IOCTL_GPIO_OEN_HIGH_NR                          (2)
#define IOCTL_GPIO_OEN_LOW_NR                           (3)
#define IOCTL_GPIO_ODN_NR                               (4)
#define IOCTL_GPIO_READ_NR                              (5)
#define IOCTL_GPIO_PULL_HIGH_NR                         (6)
#define IOCTL_GPIO_PULL_LOW_NR                          (7)
#define IOCTL_GPIO_INOUT_NR                             (8)
#define IOCTL_GPIO_FREE_NR                              (9)
#define IOCTL_GPIO_TO_IRQ_NR                            (10)
#define IOCTL_GPIO_WREGB_NR                             (11)

#define GPIO_IOC_MAGIC               'g'
#define IOCTL_GPIO_INIT               _IO(GPIO_IOC_MAGIC, IOCTL_GPIO_INIT_NR)
#define IOCTL_GPIO_SET                _IO(GPIO_IOC_MAGIC, IOCTL_GPIO_SET_NR)
#define IOCTL_GPIO_OEN_HIGH           _IO(GPIO_IOC_MAGIC, IOCTL_GPIO_OEN_HIGH_NR)
#define IOCTL_GPIO_OEN_LOW            _IO(GPIO_IOC_MAGIC, IOCTL_GPIO_OEN_LOW_NR)
#define IOCTL_GPIO_ODN                _IO(GPIO_IOC_MAGIC, IOCTL_GPIO_ODN_NR)
#define IOCTL_GPIO_READ               _IO(GPIO_IOC_MAGIC, IOCTL_GPIO_READ_NR)
#define IOCTL_GPIO_PULL_HIGH          _IO(GPIO_IOC_MAGIC, IOCTL_GPIO_PULL_HIGH_NR)
#define IOCTL_GPIO_PULL_LOW           _IO(GPIO_IOC_MAGIC, IOCTL_GPIO_PULL_LOW_NR)
#define IOCTL_GPIO_INOUT              _IO(GPIO_IOC_MAGIC, IOCTL_GPIO_INOUT_NR)
#define IOCTL_GPIO_FREE               _IO(GPIO_IOC_MAGIC, IOCTL_GPIO_FREE_NR)
#define IOCTL_GPIO_TO_IRQ             _IO(GPIO_IOC_MAGIC, IOCTL_GPIO_TO_IRQ_NR)
#define IOCTL_GPIO_WREGB              _IO(GPIO_IOC_MAGIC, IOCTL_GPIO_WREGB_NR)

#define GPIO_IOC_MAXNR              12

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

void __mod_gpio_init(void);

