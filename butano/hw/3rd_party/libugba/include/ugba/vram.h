// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020 Antonio Niño Díaz

#ifndef VRAM_H__
#define VRAM_H__

#include <stddef.h>
#include <stdint.h>

#include "definitions.h"

// Copy 16-color object tiles to VRAM (0x06010000) starting at the given
// index (0 - 1023).
EXPORT_API
void VRAM_OBJTiles16Copy(const void *src, size_t size, uint32_t tile_index);

// Copy 256-color object tiles to VRAM (0x06010000) starting at the given
// index (0 - 511).
EXPORT_API
void VRAM_OBJTiles256Copy(const void *src, size_t size, uint32_t tile_index);

// Copy a 16-bit palette to object palette memory.
EXPORT_API
void VRAM_OBJPalette16Copy(const void *src, size_t size, uint32_t pal_index);

// Copy a 256-bit palette to object palette memory.
EXPORT_API
void VRAM_OBJPalette256Copy(const void *src, size_t size);

// Copy 16-color background tiles to VRAM (0x06000000) starting at the given
// index (0 - 2047).
EXPORT_API
void VRAM_BGTiles16Copy(const void *src, size_t size, uint32_t tile_index);

// Copy 256-color background tiles to VRAM (0x06000000) starting at the given
// index (0 - 1023).
EXPORT_API
void VRAM_BGTiles256Copy(const void *src, size_t size, uint32_t tile_index);

// Copy a 16-bit palette to background palette memory.
EXPORT_API
void VRAM_BGPalette16Copy(const void *src, size_t size, uint32_t pal_index);

// Copy a 256-bit palette to background palette memory.
EXPORT_API
void VRAM_BGPalette256Copy(const void *src, size_t size);

#endif // VRAM_H__
