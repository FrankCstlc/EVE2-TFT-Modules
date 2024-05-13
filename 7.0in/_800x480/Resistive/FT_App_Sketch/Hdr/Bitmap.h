#include "FT_PLATFORM.h"

#ifndef _BITMAP_H_
#define _BITMAP_H_

typedef enum {
    BM_ARGB1555,
    BM_L1,
    BM_L4,
    BM_L8,
    BM_RGB332,
    BM_ARGB2,
    BM_ARGB4,
    BM_RGB565,
    BM_TEXT8X8 = 9,
    BM_TEXTVGA,
    BM_BARGRAPH,
    BM_PALETTED565 = 14,
    BM_PALETTED4444,
    BM_PALETTED8,
    BM_L2,
} BitmapFormat_Enum;

typedef struct {
    const ft_uint8_t * const data;
    const size_t size;
    ft_uint32_t * const address;
    BitmapFormat_Enum format;
    ft_uint16_t width;
    ft_uint16_t height;
    ft_uint16_t stride;
} Bitmap_st;

#endif /*_BITMAP_H_*/