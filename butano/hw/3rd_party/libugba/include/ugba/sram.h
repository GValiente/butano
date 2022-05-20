// SPDX-License-Identifier: MIT
//
// Copyright (c) 2022 Antonio Niño Díaz

#ifndef SRAM_H__
#define SRAM_H__

#include <stddef.h>

#include "definitions.h"

// SRAM can only be accessed in 8-bits reads and writes.
//
// The functions in this file are placed in EWRAM. According to some sources,
// reading SRAM from code in ROM doesn't work (even though it hasn't been
// verified).
//
// If you are going to use SRAM, you should try to make it easier for emulators
// and flashcarts to detect your save type:
//
// - Add a string like "SRAM_V111" to your ROM, preferably close to the start.
//   Magic strings like that are found in commercial games developed with the
//   SDK of Nintendo. Emulators look for them when loading ROMs.
//
// - The game code of your ROM should start with "2" for the EverDrive GBA to
//   detect it as SRAM:
//
//      1 - EEPROM || 2 - SRAM || 3 - FLASH-64 || 4 - FLASH-128
//
//      https://krikzz.com/pub/support/everdrive-gba/OS/changelist.txt

// Writes a buffer into SRAM. The destination must be inside SRAM, and the size
// of the buffer must be at most MEM_SRAM_SIZE bytes long. Returns 0 on success.
EXPORT_API EWRAM_CODE int SRAM_Write(void *dst, const void *src, size_t size);

// Reads a buffer from SRAM. The source must be inside SRAM, and the size of the
// buffer must be at most MEM_SRAM_SIZE bytes long. Returns 0 on success.
EXPORT_API EWRAM_CODE int SRAM_Read(void *dst, const void *src, size_t size);

#endif // SRAM_H__
