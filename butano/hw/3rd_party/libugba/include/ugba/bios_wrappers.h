// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020 Antonio Niño Díaz

#ifndef BIOS_WRAPPERS_H__
#define BIOS_WRAPPERS_H__

#include <stddef.h>

#include "bios.h"

// Note: This file contains wrappers to use some BIOS services easier than with
// the actual interface.

// Copy from source to destination in 16-bit chunks.
EXPORT_API void SWI_CpuSet_Copy16(const void *src, void *dst, size_t len);

// Fill destination with the 16-bit value from source.
EXPORT_API void SWI_CpuSet_Fill16(const void *src, void *dst, size_t len);

// Copy from source to destination in 32-bit chunks.
EXPORT_API void SWI_CpuSet_Copy32(const void *src, void *dst, size_t len);

// Fill destination with the 32-bit value from source.
EXPORT_API void SWI_CpuSet_Fill32(const void *src, void *dst, size_t len);


// Copy from source to destination in 32-bit chunks.
EXPORT_API void SWI_CpuFastSet_Copy32(const void *src, void *dst, size_t len);

// Fill destination with the 32-bit value from source.
EXPORT_API void SWI_CpuFastSet_Fill32(const void *src, void *dst, size_t len);


// Fills an array of object affine matrices from an array of transformations.
EXPORT_API void SWI_ObjAffineSet_OAM(const obj_affine_src *src,
                                     oam_matrix_entry *dst, uint32_t count);

#endif // BIOS_WRAPPERS_H__
