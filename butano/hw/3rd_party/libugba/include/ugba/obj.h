// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020 Antonio Niño Díaz

#ifndef OBJ_H__
#define OBJ_H__

#include "definitions.h"

typedef enum {
    OBJ_16_COLORS,
    OBJ_256_COLORS
} oam_color_mode;

typedef enum {
    OBJ_MODE_NORMAL,
    OBJ_MODE_TRANSPARENT,
    OBJ_MODE_WINDOW
} oam_entry_mode;

typedef enum {
    // Square
    OBJ_SIZE_8x8,
    OBJ_SIZE_16x16,
    OBJ_SIZE_32x32,
    OBJ_SIZE_64x64,

    // Horizontal
    OBJ_SIZE_16x8,
    OBJ_SIZE_32x8,
    OBJ_SIZE_32x16,
    OBJ_SIZE_64x32,

    // Vertical
    OBJ_SIZE_8x16,
    OBJ_SIZE_8x32,
    OBJ_SIZE_16x32,
    OBJ_SIZE_32x64,

    OBJ_SIZE_NUMBER,

    OBJ_SIZE_INVALID

} oam_entry_size;

// Regular objects functions
// -------------------------

EXPORT_API void OBJ_RegularInit(int index, int x, int y, oam_entry_size size,
                                oam_color_mode colors, int pal, int tile);
EXPORT_API void OBJ_RegularEnableSet(int index, int enable);
EXPORT_API void OBJ_RegularHFlipSet(int index, int enable);
EXPORT_API void OBJ_RegularVFlipSet(int index, int enable);

// Affine objects functions
// ------------------------

EXPORT_API void OBJ_AffineInit(int index, int x, int y, oam_entry_size size,
                               int matrix, oam_color_mode colors, int pal,
                               int tile, int doublesize);
EXPORT_API void OBJ_AffineMatrixSet(int index, int matrix_index);
EXPORT_API void OBJ_AffineDoubleSizeSet(int index, int enable);

// Common objects functions
// ------------------------

EXPORT_API void OBJ_PositionSet(int index, int x, int y);
EXPORT_API void OBJ_PositionGet(int index, int *x, int *y);

EXPORT_API void OBJ_ModeSet(int index, oam_entry_mode mode);

EXPORT_API void OBJ_GetShapeSize(oam_entry_size size,
                                 uint16_t *attr0_shape, uint16_t *attr1_size);
EXPORT_API oam_entry_size OBJ_GetSizeFromDimensions(int width, int height);
EXPORT_API void OBJ_GetDimensionsFromSize(oam_entry_size size,
                                          int *width, int *height);
EXPORT_API void OBJ_SizeSet(int index, oam_entry_size size);

// Enable or disable mosaic effect.
EXPORT_API void OBJ_MosaicSet(int index, int enable);

// Set palette of the object. Only for 16 color objects.
EXPORT_API void OBJ_Palette16Set(int index, int palette);

EXPORT_API void OBJ_PrioritySet(int index, int priority);

// This function automatically detects if the object is in 16 or 256 color mode
EXPORT_API void OBJ_TileSet(int index, int tile);

#endif // OBJ_H__
