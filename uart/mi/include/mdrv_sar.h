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

#ifndef __MDRV_SAR_H
#define __MDRV_SAR_H

typedef struct
{
    int channel_value;
    int adc_value;
}ADC_CONFIG_READ_ADC;

#define SARADC_IOC_MAGIC                'a'
#define MS_SAR_INIT                     _IO(SARADC_IOC_MAGIC, 0)
#define MS_SAR_SET_CHANNEL_READ_VALUE   _IO(SARADC_IOC_MAGIC, 1)
#define MS_SAR_GET_BATTERY_VOLTAGEVBAT_VALUE   _IO(SARADC_IOC_MAGIC, 2)


#endif
