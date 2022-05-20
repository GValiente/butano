// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020 Antonio Niño Díaz

#ifndef FP_MATH_H__
#define FP_MATH_H__

#include "definitions.h"

#define FP_PI       (0x8000)
#define FP_PI_2     (FP_PI / 2)
#define FP_2_PI     (2 * FP_PI)

// Input: A full cirle is 0x10000 (PI = 0x8000)
// Output: Between 1 << 16 and -1 << 16 (1.0 to -1.0)
EXPORT_API IWRAM_CODE int32_t FP_Sin(int32_t x);
EXPORT_API IWRAM_CODE int32_t FP_Cos(int32_t x);

#endif // FP_MATH_H__
