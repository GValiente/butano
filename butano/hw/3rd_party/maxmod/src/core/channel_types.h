// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2021-2026, Antonio Niño Díaz
// Copyright (c) 2023, Lorenzooone (lollo.lollo.rbiz@gmail.com)

#ifndef MM_CORE_CHANNEL_TYPES_H__
#define MM_CORE_CHANNEL_TYPES_H__

#include <assert.h>

#ifdef __GBA__
#include <maxmod.h>
#endif
#include <mm_types.h>

// Module channel
// --------------

typedef struct {
    mm_byte     alloc;  // Allocated active channel number
    mm_byte     cflags; // Pattern comression flags, called "maskvariable" in ITTECH.TXT
    mm_byte     panning;
    mm_byte     volcmd; // Volume column command
    mm_byte     effect; // Effect number
    mm_byte     param;  // Effect parameter
    mm_byte     fxmem;  // Effect memory
    mm_byte     note;   // Translated note
    mm_byte     flags;  // Channel flags
    mm_byte     inst;   // Instrument#

    mm_byte     vibdep;
    mm_byte     vibspd;
    mm_byte     vibpos;
    mm_byte     vibtype; // MOD/XM: 0 to 7; S3M/IT: 0 to 3

    mm_byte     trempos;
    mm_byte     tremtype; // MOD/XM: 0 to 7; S3M/IT: 0 to 3

    mm_word     period;

    mm_hword    bflags;
    mm_byte     volume;
    mm_byte     cvolume;

    mm_byte     pnoter;  // Pattern note
    mm_byte     memory[15];
} mm_module_channel;

#ifdef __GBA__
static_assert(sizeof(mm_module_channel) == MM_SIZEOF_MODCH);
#endif

// Module Channel flags

// The flags MF_START, MF_DVOL, MF_HASVCMD, and MF_HASFX are provided by mmutil.
// They are saved in the top 4 bits of mm_module_channel.cflags. Then, they are
// saved to the bottom 4 bits of mm_active_channel.flags when reading the
// pattern data. The flags MF_NEWINSTR, MF_NOTEOFF and MF_NOTECUT are set by
// Maxmod while reading the pattern data.
#define MF_START        1
#define MF_DVOL         2
#define MF_HASVCMD      4
#define MF_HASFX        8
#define MF_NEWINSTR     16  // New instrument
// 32 is unused
#define MF_NOTEOFF      64
#define MF_NOTECUT      128

// Module channel bflags

// New note action (NNA)
#define MCH_BFLAGS_NNA_SHIFT    6
#define MCH_BFLAGS_NNA_MASK     (3 << MCH_BFLAGS_NNA_SHIFT)
#define MCH_BFLAGS_NNA_GET(x)   (((x) & MCH_BFLAGS_NNA_MASK) >> MCH_BFLAGS_NNA_SHIFT)
#define MCH_BFLAGS_NNA_SET(x)   (((x) << MCH_BFLAGS_NNA_SHIFT) & MCH_BFLAGS_NNA_MASK)

#define MCH_BFLAGS_TREMOR       (1 << 9)    // Tremor variable
#define MCH_BFLAGS_CUT_VOLUME   (1 << 10)   // Cut channel volume

// BFLAGS
//
// Bits  | Meaning
// ------+------------------------------------
// 0-1   | Vibrato waveform         // Unused
// 2-3   | Tremolo waveform         // Unused
// 4-5   | Panbrello waveform       // Unused
// 6-7   | New note action (NNA)
//
// ??-?? | Duplicate check type     // Unused
//
// 8     | Volume envelope enabled  // Unused
// 9     | Tremor variable
// 10    | Cut channel volume
// 11-15 | Unused

// Other Definitions

#define IT_NNA_CUT      0 // New note actions
#define IT_NNA_CONT     1
#define IT_NNA_OFF      2
#define IT_NNA_FADE     3

#define IT_DCA_CUT      0 // Duplicate check actions
#define IT_DCA_OFF      1
#define IT_DCA_FADE     2

// Active channel
// --------------

typedef struct {
    mm_word     period;     // internal period
    mm_hword    fade;
    mm_hword    envc_vol;
    mm_hword    envc_pan;
    mm_hword    envc_pic;
    mm_hword    avib_dep;
    mm_hword    avib_pos;
    mm_byte     fvol;
    mm_byte     type;
    mm_byte     inst;
    mm_byte     panning;
    mm_hword    flags;
    mm_byte     volume;
    mm_byte     sample;
    mm_byte     parent;
    mm_byte     envn_vol;
    mm_byte     envn_pan;
    mm_byte     envn_pic;
} mm_active_channel;

#ifdef __GBA__
static_assert(sizeof(mm_active_channel) == MM_SIZEOF_ACTCH);
#endif

// Active channel flags

#define MCAF_KEYON      (1 << 0) // Key is on
#define MCAF_FADE       (1 << 1) // Note-fade is activated
#define MCAF_START      (1 << 2) // [re]start sample
#define MCAF_UPDATED    (1 << 3) // Already updated by pchannel routine
#define MCAF_ENVEND     (1 << 4) // End of envelope
#define MCAF_VOLENV     (1 << 5) // Volume envelope enabled
#define MCAF_SUB        (1 << 6) // 1 = Channel used for jingle. 0 = Used for main module
#define MCAF_EFFECT     (1 << 7) // 1 = Channel is used for an effect, not module or jingle
#define MCAF_PANENV     (1 << 8) // Panning envelope enabled
#define MCAF_PITCHENV   (1 << 9) // Pitch envelope enabled
// Note: Don't move MCAF_SUB or MCAF_EFFECT from their current places. Some
// functions read both of them in one go.

// Active channel types

#define ACHN_DISABLED   0 // LOCKED (multiple routines)
#define ACHN_RESERVED   1 // (can't be used [alloc channel])
#define ACHN_BACKGROUND 2 // LOCKED (alloc channel)
#define ACHN_FOREGROUND 3
#define ACHN_CUSTOM     4

#endif // MM_CORE_CHANNEL_TYPES_H__
