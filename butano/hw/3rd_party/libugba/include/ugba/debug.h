// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020-2021 Antonio Niño Díaz

#ifndef DEBUG_H__
#define DEBUG_H__

// This file has debug utilities that don't work on GBA binaries. However, there
// are defines here that remove them automatically so that code that uses them
// can still build without errors.

#include <stdint.h>

#include "definitions.h"

// Take a screenshot. If the specified name is NULL, it defaults to
// "screenshot.png".
#ifdef __GBA__
#define Debug_Screenshot(n)  do { (void)(n); } while (0)
#else
EXPORT_API void Debug_Screenshot(const char *name);
#endif

// Custom assert() that works on GBA and PC.
#ifndef UGBA_DEBUG
# define UGBA_Assert(expr) ((void)0)
#else
# define UGBA_Assert(expr) \
        ((expr) ? (void)0 : \
                  UGBA_AssertFunction(__FILE__, __LINE__, __func__, #expr))
#endif // UGBA_DEBUG

EXPORT_API void UGBA_AssertFunction(const char *file, int line,
                                    const char *func, const char *expr);
#endif // DEBUG_H__
