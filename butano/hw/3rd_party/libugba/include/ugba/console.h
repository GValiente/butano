// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020 Antonio Niño Díaz

#ifndef CONSOLE_H__
#define CONSOLE_H__

#include "definitions.h"

// Initialize the console by loading the needed tileset and map to the last tile
// block in VRAM.
EXPORT_API void CON_InitDefault(void);

// Print a character.
EXPORT_API void CON_PutChar(char c);

// Print an unformatted string.
EXPORT_API void CON_Print(const char *string);

// Set cursor at the specified coordinates.
EXPORT_API void CON_CursorSet(int x, int y);

// Clear the console.
EXPORT_API void CON_Clear(void);

#endif // CONSOLE_H__
