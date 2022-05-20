// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020, 2022 Antonio Niño Díaz

#ifndef BIOS_H__
#define BIOS_H__

#include <assert.h>

#include "definitions.h"
#include "hardware.h"

// Note: Names taken from GBATEK

// Perform a soft reset. Not supported on the SDL2 port.
EXPORT_API NORETURN void SWI_SoftReset(void);

// Flags to use with SWI_RegisterRamReset()
#define SWI_RAM_RESET_EWRAM     (1 << 0)
#define SWI_RAM_RESET_IWRAM     (1 << 1) // Except for the last 0x200 bytes
#define SWI_RAM_RESET_PALETTE   (1 << 2)
#define SWI_RAM_RESET_VRAM      (1 << 3)
#define SWI_RAM_RESET_OAM       (1 << 4)
#define SWI_RAM_RESET_IO_SERIAL (1 << 5)
#define SWI_RAM_RESET_IO_SOUND  (1 << 6)
#define SWI_RAM_RESET_IO_OTHER  (1 << 7)

// Clear different areas of memory and I/O registers
EXPORT_API void SWI_RegisterRamReset(uint32_t flags);

// Wait until an interrupt happens.
EXPORT_API void SWI_Halt(void);

// Wait until the VBlank interrupt happens.
EXPORT_API void SWI_VBlankIntrWait(void);

// Wait for any of the interrupts in the flags specified. If an interrupt has
// already happened, it is possible to discard it by setting the argument to 1.
// If it is set to 0, and the interrupt has happened already, this function will
// return right away.
//
// Note that if no flags are passed the CPU will stay in an infinite loop inside
// this function.
EXPORT_API void SWI_IntrWait(uint32_t discard_old_flags, uint16_t wait_flags);

// For SWI_CpuSet() and SWI_CpuFastSet()
#define SWI_MODE_COPY   (0 << 24)
#define SWI_MODE_FILL   (1 << 24)
// For SWI_CpuSet() only
#define SWI_MODE_16BIT  (0 << 26)
#define SWI_MODE_32BIT  (1 << 26)

// Source and destination must be aligned to 4 bytes for 32-bit copies, or 2
// bytes for 16-bit copies. The length is expressed either in 32-bit or 16-bit
// chunks (words or halfwords).
EXPORT_API void SWI_CpuSet(const void *src, void *dst, uint32_t len_mode);

// Source and destination must be aligned to 4 bytes. The length must be a
// multiple of 8 bytes.
EXPORT_API void SWI_CpuFastSet(const void *src, void *dst, uint32_t len_mode);

// Calculate result or modulus of dividing num by div.
EXPORT_API int32_t SWI_Div(int32_t num, int32_t div);
EXPORT_API int32_t SWI_DivMod(int32_t num, int32_t div);

// Calculate square root.
EXPORT_API uint16_t SWI_Sqrt(uint32_t value);

// Calculate arc tangent
EXPORT_API int16_t SWI_ArcTan(int16_t tan);

// Calculate arc tangent 2
EXPORT_API int16_t SWI_ArcTan2(int16_t x, int16_t y);

// Known values that SWI_GetBiosChecksum() can return
#define SWI_CHECKSUM_GBA    (0xBAAE187F) // GBA, GBA SP, GB Micro
#define SWI_CHECKSUM_NDS    (0xBAAE1880) // NDS, 3DS in NDS mode

// Returns checksum of the BIOS
EXPORT_API uint32_t SWI_GetBiosChecksum(void);

// Struct that holds the input to SWI_BgAffineSet()
typedef struct ALIGNED(4) {
    int32_t bgx;        // 24.8 fixed point
    int32_t bgy;        // 24.8 fixed point
    int16_t scrx;
    int16_t scry;
    int16_t scalex;     // 8.8 fixed point
    int16_t scaley;     // 8.8 fixed point
    uint32_t angle;     // 8.8 fixed point. Low 8 bits ignored.
    // The angle is a 32 bit integer (instead of adding a padding field) for
    // conveniency. Only bits 8-15 are actually used.
} bg_affine_src;

static_assert(sizeof(bg_affine_src) == 20, "Wrong bg_affine_src size");

// Struct that holds the state of a background affine transformation. It is used
// as container of the output of SWI_BgAffineSet()
typedef struct ALIGNED(4) {
    int16_t pa;
    int16_t pb;
    int16_t pc;
    int16_t pd;
    int32_t xoff;
    int32_t yoff;
} bg_affine_dst;

static_assert(sizeof(bg_affine_dst) == 16, "Wrong bg_affine_dst size");

// This function gets a list of background transformations and outputs the
// correct affine matrices for the GBA hardware.
EXPORT_API
void SWI_BgAffineSet(const bg_affine_src *src, bg_affine_dst *dst,
                     uint32_t count);

// Struct that holds the input to SWI_ObjAffineSet()
typedef struct ALIGNED(2) {
    int16_t sx;         // 8.8 fixed point
    int16_t sy;         // 8.8 fixed point
    uint16_t angle;     // 8.8 fixed point. Range: 0 - 0xFFFF
    uint16_t padding;
} obj_affine_src;

static_assert(sizeof(obj_affine_src) == 8, "Wrong obj_affine_src size");

// This function gets a list of objects transformations and outputs the correct
// affine matrices for the GBA hardware.
EXPORT_API
void SWI_ObjAffineSet(const obj_affine_src *src, void *dst,
                      uint32_t count, uint32_t increment);

// Struct that holds information used by SWI_BitUnPack()
typedef struct ALIGNED(4) {
    int16_t source_length;
    uint8_t source_width;
    uint8_t dest_width;
    uint32_t data_offset;
} bit_unpack_info;

static_assert(sizeof(bit_unpack_info) == 8, "Wrong bit_unpack_info size");

// Used in data_offset in bit_unpack_info.
#define SWI_BITUNPACK_OFFSET_ZERO (1 << 31)

// Unpack data of the specified width in source into the specified width in the
// destination. Used to increase the color depth of images, for example. It
// writes the result to the destination using 32-bit writes. VRAM can be used as
// destination.
EXPORT_API
void SWI_BitUnPack(const void *source, void *dest, const bit_unpack_info *info);

// Values that specify the type of the compression in the 32-bit header of a
// compressed blob. It can be found in bits 4-7 of the header.
#define SWI_UNCOMP_TYPE_LZ77        (1)
#define SWI_UNCOMP_TYPE_HUFFMAN     (2)
#define SWI_UNCOMP_TYPE_RL          (3)
#define SWI_UNCOMP_TYPE_DIFF        (8)

// Value specified in bits 0-3 of the header
#define SWI_DIFF_SIZE_8BIT          (1)
#define SWI_DIFF_SIZE_16BIT         (2)

// Decompresses LZ77 data from the source and writes the result to the
// destination using 8-bit writes. It can't be used to decompress directly to
// VRAM, as it only accepts 16 and 32-bit accesses.
EXPORT_API
void SWI_LZ77UnCompReadNormalWrite8bit(const void *source, void *dest);

// Decompresses LZ77 data from the source and writes the result to the
// destination using 16-bit writes. VRAM can be used as destination.
EXPORT_API
void SWI_LZ77UnCompReadNormalWrite16bit(const void *source, void *dest);

// Decompresses Huffman-encoded data from the source and writes the result to
// the destination using 32-bit writes. VRAM can be used as destination.
EXPORT_API void SWI_HuffUnComp(const void *source, void *dest);

// Decompresses Run-Length data from the source and writes the result to the
// destination using 8-bit writes. It can't be used to decompress directly to
// VRAM, as it only accepts 16 and 32-bit accesses.
EXPORT_API void SWI_RLUnCompWram(const void *source, void *dest);

// Decompresses Run-Length data from the source and writes the result to the
// destination using 16-bit writes. VRAM can be used as destination.
EXPORT_API void SWI_RLUnCompVram(const void *source, void *dest);

// Convert data in diff format to original data. In this version, data elements
// are 8 bits wide and the result is writen in 8-bit accesses.
EXPORT_API void SWI_Diff8bitUnFilterWram(const void *source, void *dest);

// Convert data in diff format to original data. In this version, data elements
// are 8 bits wide and the result is writen in 16-bit accesses. This means that
// VRAM can be used as destination, as it doesn't accept 8-bit writes.
EXPORT_API void SWI_Diff8bitUnFilterVram(const void *source, void *dest);

// Convert data in diff format to original data. In this version, data elements
// are 16 bits wide and the result is writen in 16-bit accesses.
EXPORT_API void SWI_Diff16bitUnFilter(const void *source, void *dest);

// Set the level of the SOUNDBIAS register by doing small changes. A level of 0
// sets the BIAS level to 0, any other value sets it to 0x200.
EXPORT_API void SWI_SoundBias(uint32_t level);

// Perform a hard reset. Not supported on the SDL2 port.
EXPORT_API NORETURN void SWI_HardReset(void);

#endif // BIOS_H__
