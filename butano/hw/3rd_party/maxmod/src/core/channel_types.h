// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2021-2025, Antonio Niño Díaz (antonio_nd@outlook.com)
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
    mm_byte     pflags; // Playback flags (unused)
    mm_byte     vibdep;
    mm_byte     vibspd;
    mm_byte     vibpos;
    mm_byte     volume;
    mm_byte     cvolume;
    mm_word     period;
    mm_hword    bflags;
    mm_byte     pnoter;  // Pattern note
    mm_byte     memory[15];
    mm_byte     padding[2];
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
    mm_byte     volume;
    mm_byte     sample;
    mm_byte     parent;
    mm_byte     flags;
    mm_byte     envn_vol;
    mm_byte     envn_pan;
    mm_byte     envn_pic;
    mm_byte     sfx;        // can store this anywhere
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
// Note: Don't move MCAF_SUB or MCAF_EFFECT from their current places. Some
// functions read both of them in one go.

// Active channel types

#define ACHN_DISABLED   0 // LOCKED (multiple routines)
#define ACHN_RESERVED   1 // (can't be used [alloc channel])
#define ACHN_BACKGROUND 2 // LOCKED (alloc channel)
#define ACHN_FOREGROUND 3
#define ACHN_CUSTOM     4

// Mixer channel
// -------------

#ifdef __NDS__

// A DS mixer channel is active if "samp" isn't zero.
typedef struct {
    mm_word     samp   : 24;    // 0:23  mainram address
    mm_word     tpan   : 7;     // 24:30 = target panning
    mm_word     key_on : 1;     // 31 = key-on

    mm_hword    freq;           // unsigned 3.10, top 3 cleared
    mm_hword    vol;            // target volume   0..65535
    mm_word     read;           // unsigned 22.10. See MP_SAMPFRAC
    mm_hword    cvol;           // current volume  0..65535
    mm_hword    cpan;           // current panning 0..65535
} mm_mixer_channel;

// Make sure that the size matches the assembly code
static_assert(sizeof(mm_mixer_channel) == 16);

// Fractionary part of the sample read offset
#define MP_SAMPFRAC         10

// scale = 65536*1024*2 / mixrate
#define MIXER_SCALE         4096 //6151

#endif // __NDS__

#ifdef __GBA__

// A GBA mixer channel is active if "src & (1 << 31)" is zero.
typedef struct {
    uintptr_t   src;
    mm_word     read; // Fixed point 20.12. See MP_SAMPFRAC
    mm_byte     vol;
    mm_byte     pan;
    mm_byte     unused_0;
    mm_byte     unused_1;
    mm_word     freq;
} mm_mixer_channel;

// Fractionary part of the sample read offset
#define MP_SAMPFRAC             12

#define MIXCH_GBA_SRC_STOPPED   (1u << ((sizeof(uintptr_t) * 8) - 1))

// Make sure that the size matches the assembly code
static_assert(sizeof(mm_mixer_channel) == 16);
static_assert(sizeof(mm_mixer_channel) == MM_SIZEOF_MIXCH);

#endif // __GBA__

#endif // MM_CORE_CHANNEL_TYPES_H__
