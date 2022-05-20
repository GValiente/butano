// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020 Antonio Niño Díaz

#ifndef BACKGROUND_H__
#define BACKGROUND_H__

#include "bios.h"
#include "hardware.h"

// List of all possible sizes of regular backgrounds
typedef enum {
    BG_REGULAR_256x256,
    BG_REGULAR_512x256,
    BG_REGULAR_256x512,
    BG_REGULAR_512x512,

    BG_REGULAR_NUMBER,

    BG_REGULAR_INVALID
} bg_regular_size;

// List of all possible sizes of affine backgrounds
typedef enum {
    BG_AFFINE_128x128,
    BG_AFFINE_256x256,
    BG_AFFINE_512x512,
    BG_AFFINE_1024x1024,

    BG_AFFINE_NUMBER,

    BG_AFFINE_INVALID
} bg_affine_size;

// Types of background based on their bit depth
typedef enum {
    BG_16_COLORS,
    BG_256_COLORS
} bg_color_mode;

// Initialize a regular background.
EXPORT_API
void BG_RegularInit(int index, bg_regular_size size, bg_color_mode colors,
                    uintptr_t tile_base_addr, uintptr_t map_base_addr);

// Functions to set and get the scroll of a regular background. It doesn't work
// for affine backgrounds. Note that the scroll hardware registers are
// read-only. The functions below keep a copy of the scroll value so that it can
// be recovered later.
EXPORT_API void BG_RegularScrollSet(int index, int x, int y);
EXPORT_API void BG_RegularScrollGet(int index, int *x, int *y);

// Initialize an affine background.
EXPORT_API
void BG_AffineInit(int index, bg_affine_size size, uintptr_t tile_base_addr,
                   uintptr_t map_base_addr, int wrap);

// Set affine transformation values for the specified affine background.
EXPORT_API void BG_AffineTransformSet(int index, bg_affine_dst *tr);

// Enable wrap mode of an affine background.
EXPORT_API void BG_AffineWrapEnable(int index, int wrap);

// Set priority of a background.
EXPORT_API void BG_PrioritySet(int index, int priority);

// Enable mosaic effect in the specified background.
EXPORT_API void BG_MosaicEnable(int index, int enabled);

// Get a pointer to the framebuffer (in mode 3).
EXPORT_API uint16_t *BG_Mode3FramebufferGet(void);

// Get a pointer to the active or non-active framebuffer (in mode 4).
EXPORT_API uint16_t *BG_Mode4FramebufferActiveGet(void);
EXPORT_API uint16_t *BG_Mode4FramebufferBackGet(void);

// Get a pointer to the active or non-active framebuffer (in mode 5).
EXPORT_API uint16_t *BG_Mode5FramebufferActiveGet(void);
EXPORT_API uint16_t *BG_Mode5FramebufferBackGet(void);

// Set active buffer (0 or 1).
EXPORT_API void BG_FramebufferSet(int backbuffer);

// Swap active framebuffer (used for modes 4 and 5).
EXPORT_API void BG_FramebufferSwap(void);

// Set backdrop color (background palette 0 color).
EXPORT_API void BG_BackdropColorSet(uint16_t color);

#endif // BACKGROUND_H__
