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

/**
 *  @file mdrv_scl_io.h
 *  @brief SCL Driver IOCTL interface
 */


#ifndef _MDRV_SCL_IO_H
#define _MDRV_SCL_IO_H

//=============================================================================
// Includs
//=============================================================================
#include "mdrv_scl_io.h"

//=============================================================================
// IOCTRL defines
//=============================================================================

#define IOCTL_SCL_SET_PNL_TIMING_NR                 (0)   ///< The IOCTL NR definition,IOCTL_SCL_SET_IN_CONFIG
#define IOCTL_SCL_SET_DIPR_HVSP_CONFIG_NR           (1)   ///< The IOCTL NR definition,IOCTL_SCL_SET_DIPR_HVSP_CONFIG_NR
#define IOCTL_SCL_SET_DISPPLAY_DATA_FROM_NR         (2)   ///< The IOCTL NR definition,IOCTL_SCL_SET_DISPPLAY_DATA_FROM_NR
#define IOCTL_SCL_SET_PNL_INIT_NR                   (3)   ///< The IOCTL NR definition,IOCTL_SCL_SET_DISPPLAY_DATA_FROM_NR
#define IOCTL_SCL_GET_MIPI_READ_NR                  (4)   ///< The IOCTL NR definition,IOCTL_SCL_SET_DISPPLAY_DATA_FROM_NR
#define IOCTL_SCL_MAX_NR                            (IOCTL_SCL_GET_MIPI_READ_NR+1)


// use 'm' as magic number
#define IOCTL_SCL_MAGIC                       ('1')///< The Type definition of IOCTL for hvsp driver
/**
* Used to set Input MUX,capture window, use ST_IOCTL_SCL_INPUT_CONFIG.
*/
#define IOCTL_SCL_SET_PNL_TIMING                _IOWR(IOCTL_SCL_MAGIC,  IOCTL_SCL_SET_PNL_TIMING_NR, ST_DrvSclPnlTiming)
#define IOCTL_SCL_SET_DIPR_HVSP_CONFIG          _IOWR(IOCTL_SCL_MAGIC,  IOCTL_SCL_SET_DIPR_HVSP_CONFIG_NR, ST_DrvSclDiprHvspConfig)
#define IOCTL_SCL_SET_DISPPLAY_DATA_FROM        _IOWR(IOCTL_SCL_MAGIC,  IOCTL_SCL_SET_DISPPLAY_DATA_FROM_NR, E_DrvSclDisplayData)
#define IOCTL_SCL_SET_PNL_INIT                  _IOWR(IOCTL_SCL_MAGIC,  IOCTL_SCL_SET_PNL_INIT_NR, ST_DrvSclPnlTiming)
#define IOCTL_SCL_GET_MIPI_READ                 _IOWR(IOCTL_SCL_MAGIC,  IOCTL_SCL_GET_MIPI_READ_NR, unsigned long)


#endif //
/** @} */ // end of hvsp_group
