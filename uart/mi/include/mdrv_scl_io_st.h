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
 *  @file mdrv_scl_io_st.h
 *  @brief scl Driver IOCTL parameter interface
 */

 /**
 * \ingroup scl_group
 * @{
 */

#ifndef _MDRV_SCL_IO_ST_H
#define _MDRV_SCL_IO_ST_H

//=============================================================================
// Defines
//=============================================================================
#define IOCTL_SCL_VERSION                        0x0001


//=============================================================================
// enum
//=============================================================================
typedef enum
{
    E_DRV_SCL_DISPLAY_DATA_FROM_DIPR,
    E_DRV_SCL_DISPLAY_DATA_FROM_IPM2,
    E_DRV_SCL_DISPLAY_DATA_NUM,
}E_DrvSclDisplayData;

typedef enum
{
    E_DRV_SCL_PNL_TYPE_MIPI,
    E_DRV_SCL_PNL_TYPE_MIPI_RM68200,
    E_DRV_SCL_PNL_TYPE_TTL,
    E_DRV_SCL_PNL_TYPE_LVDS,
    E_DRV_SCL_PNL_TYPE_I80,
    E_DRV_SCL_PNL_TYPE_TTL_ST7701S,
    E_DRV_SCL_PNL_TYPE_NUM,
}E_DrvSclPnlType;

typedef enum
{
    E_DRV_SCL_YUV422,
    E_DRV_SCL_RGB565,
    E_DRV_SCL_ARGB8888,
    E_DRV_SCL_YUV420_LINEAR,
    E_DRV_SCL_YUV420_TILED_16X32,
    E_DRV_SCL_YUV420_TILED_32X16,
    E_DRV_SCL_YUV420_TILED_32X32,
    E_DRV_SCL_COLOR_FORMAT_NUM,
}E_DrvSclMemColorFormat;

//=============================================================================
// struct
//=============================================================================
/**
* Used to setup the SCL timing
*/
typedef struct
{
    unsigned long u32Version ; ///< structure version
    unsigned long u32VersionSize; ///< structure size for version checking

    E_DrvSclPnlType eDrvSclPnlType; ///< panel Type

    unsigned long u32VsyncSt; ///< vertical sync start point
    unsigned long u32VsyncEnd;///< vertical sync end point
    unsigned long u32VfdeSt;  ///< vertical frame data enable start point
    unsigned long u32VfdeEnd; ///< vertical frame data enable end point
    unsigned long u32VdeSt;   ///< vertical data enable start point
    unsigned long u32VdeEnd;  ///< vertical data enable end point
    unsigned long u32Vtt;      ///< vertical  total

    unsigned long u32HsyncSt; ///< horizontal sync start point
    unsigned long u32HsyncEnd;///< horizontal sync end point
    unsigned long u32HfdeSt;  ///< horizontal frame data enable start point
    unsigned long u32HfdeEnd; ///< horizontal frame data enable end point
    unsigned long u32HdeSt;   ///< horizontal data enable start point
    unsigned long u32HdeEnd;  ///< horizontal data enable end point
    unsigned long u32Htt;      ///< horizontal total

    unsigned long u32VFreqX10; ///< vertical frequecny x10

}__attribute__ ((__packed__))ST_DrvSclPnlTiming;

typedef struct
{
    unsigned long u32Version ; ///< structure version
    unsigned long u32VersionSize; ///< structure size for version checking

    unsigned long u32MemImageWidth;
    unsigned long u32MemImageHeight;
    unsigned long u32FetchImageWidth;
    unsigned long u32BaseAddr[2];
    E_DrvSclMemColorFormat  eMemColorFormat;

    unsigned long u32AfterScalingWidth;
    unsigned long u32AfterScalingHeight;

}__attribute__ ((__packed__))ST_DrvSclDiprHvspConfig;

//=============================================================================


#endif //
/** @} */ // end of hvsp_group
