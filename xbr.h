/*
 * XBR filter extracted from FFmpeg into a separate library.
 *
 *
 * Copyright (c) 2011, 2012 Hyllian/Jararaca <sergiogdb@gmail.com>
 * Copyright (c) 2014 Arwa Arif <arwaarif1994@gmail.com>
 * Copyright (c) 2015 Treeki <treeki@gmail.com>
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#define XBR_INLINE __inline
#define XBR_EXPORT __declspec(dllexport)
#else
#define XBR_INLINE inline
#define XBR_EXPORT
#endif

typedef struct {
    uint32_t rgbtoyuv[1<<24];
} xbr_data;

typedef struct {
    const uint8_t *input;
    uint8_t *output;
    int inWidth, inHeight;
    int inPitch, outPitch;
    const xbr_data *data;
} xbr_params;

XBR_EXPORT void xbr_filter_2x(const xbr_params *ctx);
XBR_EXPORT void xbr_filter_3x(const xbr_params *ctx);
XBR_EXPORT void xbr_filter_4x(const xbr_params *ctx);

XBR_EXPORT void xbr_init_data(xbr_data *data);


#ifdef __cplusplus
}
#endif

