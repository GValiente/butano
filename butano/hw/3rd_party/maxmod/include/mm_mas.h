// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2025, Antonio Niño Díaz

/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ '__ \/ __ '/ |/ / __ '__ \/ __ \/ __  /               *
 *              / / / / / / /_/ />  </ / / / / / /_/ / /_/ /                *
 *             /_/ /_/ /_/\__,_/_/|_/_/ /_/ /_/\____/\__,_/                 *
 *                                                                          *
 *               Nintendo DS & Gameboy Advance Sound System                 *
 *                                                                          *
 ****************************************************************************/

// MAXMOD MODULE FORMAT DEFINITIONS

#ifndef MM_MAS_H__
#define MM_MAS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mm_types.h>

// prefix comes first

typedef struct tmm_mas_prefix
{
    mm_word     size;
    mm_byte     type; // MAS_TYPE_SONG, MAS_TYPE_SAMPLE_GBA or MAS_TYPE_SAMPLE_NDS
    mm_byte     version;
    mm_byte     reserved[2]; // Unused
} mm_mas_prefix;

MM_STATIC_ASSERT(mm_mas_prefix, 8)

#define MAS_TYPE_SONG       0
#define MAS_TYPE_SAMPLE_GBA 1
#define MAS_TYPE_SAMPLE_NDS 2

// Max number of pattern orders in MAS files. The last value must be '255'.
#define MAS_MAX_PATTERN_ORDERS  200

// Max number of channels in MAS files
#define MAS_MAX_CHANNELS        32

typedef struct tmm_mas_head
{
#ifdef MM_MMUTIL_DEVKITPRO_COMPAT
    mm_byte     order_count; // TODO: mmutil always exports MAS_MAX_PATTERN_ORDERS. Unused.
    mm_byte     instr_count;
    mm_byte     sampl_count;
    mm_byte     pattn_count;
#else
    mm_hword    order_count; // TODO: mmutil always exports MAS_MAX_PATTERN_ORDERS. Unused.
    mm_hword    instr_count;
    mm_hword    sampl_count;
    mm_hword    pattn_count; // TODO: This is unused
#endif
    mm_byte     flags;
    mm_byte     global_volume;
#ifdef MM_MMUTIL_DEVKITPRO_COMPAT
    mm_byte     initial_speed;
    mm_byte     initial_tempo;
    mm_byte     repeat_position;
    mm_byte     reserved[3];
#else
    mm_hword    initial_speed;
    mm_hword    initial_tempo;
    mm_hword    repeat_position;
    mm_byte     reserved[4];
#endif
    mm_byte     channel_volume[MAS_MAX_CHANNELS];
    mm_byte     channel_panning[MAS_MAX_CHANNELS]; // 0..255
    mm_byte     sequence[MAS_MAX_PATTERN_ORDERS];

    mm_word     tables[];

    // ::instrument table
    // ::sample info table
    // ::pattern table
} mm_mas_head;

#ifdef MM_MMUTIL_DEVKITPRO_COMPAT
MM_STATIC_ASSERT(mm_mas_head, 276)
#else
MM_STATIC_ASSERT(mm_mas_head, 284)
#endif

// The MAS_HEADER_FLAG_OLD_EFFECTS flag is only set by IT songs. It affects the
// behaviour of some effects, which changed between the first version of Impulse
// Tracker and newer versions of it. According to the Impulse Tracker II User
// Manual:
//
// - In new mode, vibrato and tremolo are updated every frame in new mode,
//   independently from the song speed. In old mode (and other formats, like
//   MOD/S3M), it's updated every non-row frame, and it depends on the song
//   speed. Also, in old mode, the effect is twice as deep.
// - Command `Oxx` (sample offset) can be asked to set the offset past the end
//   of the sample. In old mode, the sample is played from the end point. In new
//   mode, commands that set the offset after the end are ignored.

#define MAS_HEADER_FLAG_LINK_GXX    (1 << 0) // Shared Gxx memory (effect and volcmd)
#define MAS_HEADER_FLAG_OLD_EFFECTS (1 << 1) // MOD/S3M = true. XM/IT = false. TODO: Unused
#define MAS_HEADER_FLAG_FREQ_MODE   (1 << 2) // 1 = Linear freqs, 0 = Amiga freqs
#define MAS_HEADER_FLAG_XM_MODE     (1 << 3) // 1 = MOD/XM mode, 0 = S3M/IT mode
#define MAS_HEADER_FLAG_MSL_DEP     (1 << 4) // MSL file = true, MAS file = false. TODO: Unused
#define MAS_HEADER_FLAG_OLD_MODE    (1 << 5) // 1 = MOD/S3M, 0 = S3M/IT

typedef struct tmm_mas_instrument
{
    mm_byte     global_volume;
    mm_byte     fadeout;
    mm_byte     random_volume;
    mm_byte     dct; // Duplicate Check Type (0-3 = Off/Note/Sample/Instrument)
    mm_byte     nna; // New Note Action (0-3 = Cut/Continue/Note off/Note fade)
    mm_byte     env_flags;
    mm_byte     panning; // If bit 7 is 0, use the value. Value = 0..127
    mm_byte     dca; // Duplicate Check Action (0-2 = Cut/Note off/Note fade)
    mm_hword    note_map_offset : 15;
    mm_hword    is_note_map_invalid : 1;
    mm_hword    reserved; // Always zero

    mm_byte     data[]; // The data that follows is variable

    // Volume envelope, panning envelope and pitch envelopes.
    //
    // Any of them may be missing. Check env_flags to see which envelopes are
    // present.
    //mm_byte     envelopes[];

    // After the envelopes there is a note map if is_note_map_invalid == 0.
    //
    // The address of the note map is the base of the mm_mas_instrument struct
    // plus the value of note_map_offset.
    //mm_hword    note_map[120];

} mm_mas_instrument;

MM_STATIC_ASSERT(mm_mas_instrument, 12)

#define MAS_INSTR_FLAG_VOL_ENV_EXISTS    (1 << 0) // Volume envelope exists
#define MAS_INSTR_FLAG_PAN_ENV_EXISTS    (1 << 1) // Panning envelope exists
#define MAS_INSTR_FLAG_PITCH_ENV_EXISTS  (1 << 2) // Pitch envelope exists
#define MAS_INSTR_FLAG_VOL_ENV_ENABLED   (1 << 3) // Volume envelope enabled
#define MAS_INSTR_FLAG_PAN_ENV_ENABLED   (1 << 4) // Panning envelope enabled
#define MAS_INSTR_FLAG_PITCH_ENV_ENABLED (1 << 5) // Pitch envelope enabled
// In XM pitch envelope doesn't exist, only volume and panning envelopes. Also,
// the "exists" and "enabled" flags are always together.
//
// In IT, the "enabled" and "exists" flags are set independently (an envelope
// may start disabled and it can be enabled with S7x effects.
//
// MOD and S3M don't have instruments, so they don't have instrument envelopes

typedef struct
{
    mm_shword   delta;
    mm_hword    base : 7;  // node_y. (0->64 for vol, -32->+32 for panning or pitch)
    mm_hword    range : 9; // 1 to 511
}
mm_mas_envelope_node;

MM_STATIC_ASSERT(mm_mas_envelope_node, 4)

typedef struct tmm_mas_envelope
{
    mm_byte     size;
    mm_byte     loop_start;
    mm_byte     loop_end;
    mm_byte     sus_start;
    mm_byte     sus_end;
    mm_byte     node_count;
    mm_byte     is_filter; // (maybe supported someday :)
    mm_byte     wasted;

    mm_mas_envelope_node env_nodes[];

    // ::envelope nodes
} mm_mas_envelope;

MM_STATIC_ASSERT(mm_mas_envelope, 8)

typedef struct tmm_mas_sample_info
{
    mm_byte     default_volume;
    mm_byte     panning; // If bit 7 is 0, use the value. Value = 0..127
    mm_hword    frequency;
    mm_byte     av_type; // VIT (auto vibrato)
    mm_byte     av_depth; // VID
    mm_byte     av_speed; // VIS
    mm_byte     global_volume; // GV
    mm_hword    av_rate; // VIR

    // MSL files contain MAS files that can contain sample waveforms or songs.
    // In order to save space, all samples in the MSL file are extracted from
    // their songs, deduplicated, and stored as MAS files. Then, songs have
    // references to the MSL ID of the MAS file with that sample, and data[] is
    // empty.
    //
    // Individual MAS files don't move the sample waveforms out of the song. In
    // that case, the MSL ID is set to 0xFFFF, and the sample waveform data is
    // stored after the sample information. This means that data[] is a
    // mm_mas_gba_sample or mm_mas_ds_sample struct.
    mm_hword    msl_id;

    // If msl_id == 0xFFFF this is a mm_mas_gba_sample or mm_mas_ds_sample
    // struct. If not, there is no more data.
    mm_byte     data[];

    // ::sample may follow
} mm_mas_sample_info;

MM_STATIC_ASSERT(mm_mas_sample_info, 12)

typedef struct tmm_mas_pattern
{
    mm_byte     row_count; // Number of rows of the pattern

    mm_byte     pattern_data[];

    // ::pattern data
} mm_mas_pattern;

MM_STATIC_ASSERT(mm_mas_pattern, 1)

typedef enum
{
    MAS_FX_NONE = 0,

    // IT and S3M effects (S3M is a subset of IT)
    MAS_FX_SET_SPEED            = 1,
    MAS_FX_POSITION_JUMP        = 2,
    MAS_FX_PATTERN_BREAK        = 3,
    MAS_FX_VOLUME_SLIDE         = 4, // Affected by MAS_HEADER_FLAG_XM_MODE
    MAS_FX_PORTAMENTO_DOWN      = 5,
    MAS_FX_PORTAMENTO_UP        = 6,
    MAS_FX_GLISSANDO            = 7, // Porta to note
    MAS_FX_VIBRATO              = 8,
    MAS_FX_TREMOR               = 9, // S3M
    MAS_FX_ARPEGGIO             = 10,
    MAS_FX_VIBRATO_VOLUME       = 11, // Affected by MAS_HEADER_FLAG_XM_MODE
    MAS_FX_PORTA_VOLUME         = 12, // Affected by MAS_HEADER_FLAG_XM_MODE
    MAS_FX_CHANNEL_VOLUME       = 13,
    MAS_FX_CHANNEL_VOLUME_SLIDE = 14,
    MAS_FX_SAMPLE_OFFSET        = 15,
    MAS_FX_PANNING_SLIDE        = 16,
    MAS_FX_RETRIGGER            = 17,
    MAS_FX_TREMOLO              = 18,
    MAS_FX_EXTENDED             = 19,
    MAS_FX_SET_TEMPO            = 20,
    MAS_FX_FINE_VIBRATO         = 21,
    MAS_FX_SET_GLOBAL_VOLUME    = 22,
    MAS_FX_GLOBAL_VOLUME_SLIDE  = 23,
    MAS_FX_SET_PANNING          = 24,
    MAS_FX_PANBRELLO            = 25,
    MAS_FX_SET_FILTER           = 26,

    // Most MOD/XM effects can be mapped to IT/S3M effects, but not all
    MAS_FX_XM_SET_VOLUME        = 27, // MOD/XM
    MAS_FX_XM_KEY_OFF           = 28, // XM
    MAS_FX_XM_ENVELOPE_POS      = 29, // XM
    MAS_FX_XM_TREMOR            = 30, // XM
}
mm_mas_pattern_effect;

typedef enum
{
    MAS_FX_EXT_FINE_VOL_SLIDE_UP    = 0,
    MAS_FX_EXT_FINE_VOL_SLIDE_DOWN  = 1,
    MAS_FX_EXT_OLD_RETRIGGER        = 2,
    MAS_FX_EXT_VIBRATO_WAVEFORM     = 3,
    MAS_FX_EXT_TREMOLO_WAVEFORM     = 4,
    MAS_FX_EXT_PANBRELLO_WAVEFORM   = 5,
    MAS_FX_EXT_FINE_PATTERN_DELAY   = 6,
    MAS_FX_EXT_INSTRUMENT_CONTROL   = 7,
    MAS_FX_EXT_SET_PANNING          = 8,
    MAS_FX_EXT_SOUND_CONTROL        = 9,
    MAS_FX_EXT_HIGH_OFFSET          = 10,
    MAS_FX_EXT_PATTERN_LOOP         = 11,
    MAS_FX_EXT_NOTE_CUT             = 12,
    MAS_FX_EXT_NOTE_DELAY           = 13,
    MAS_FX_EXT_PATTERN_DELAY        = 14,
    MAS_FX_EXT_SONG_MESSAGE         = 15,
}
mm_mas_pattern_effect_extended;

#define MAS_WAVE_CONTINUE 8

typedef enum
{
    // Retrigger
    // ---------

    MAS_WAVE_SINE                       = 0, // MOD, XM, S3M
    MAS_WAVE_DOWN_RAMP_BROKEN           = 1, // MOD, XM
    MAS_WAVE_DOWN_RAMP_OFFSET           = 2, // S3M
    MAS_WAVE_DOWN_RAMP_NORMAL           = 3,
    MAS_WAVE_SQUARE_FULL                = 4, // MOD, XM, S3M
    MAS_WAVE_SQUARE_HALF                = 5,
    MAS_WAVE_RANDOM                     = 6, // MOD, XM, S3M, IT
    // 7

    // Continue
    // --------

    MAS_WAVE_SINE_CONTINUE              = 0 | MAS_WAVE_CONTINUE, // MOD, XM, IT
    MAS_WAVE_DOWN_RAMP_BROKEN_CONTINUE  = 1 | MAS_WAVE_CONTINUE, // MOD, XM
    MAS_WAVE_DOWN_RAMP_OFFSET_CONTINUE  = 2 | MAS_WAVE_CONTINUE,
    MAS_WAVE_DOWN_RAMP_NORMAL_CONTINUE  = 3 | MAS_WAVE_CONTINUE, // IT
    MAS_WAVE_SQUARE_FULL_CONTINUE       = 4 | MAS_WAVE_CONTINUE, // MOD, XM
    MAS_WAVE_SQUARE_HALF_CONTINUE       = 5 | MAS_WAVE_CONTINUE, // IT
    MAS_WAVE_RANDOM_CONTINUE            = 6 | MAS_WAVE_CONTINUE,
    // 7 | MAS_WAVE_CONTINUE,
}
mm_mas_waveform_type;

typedef struct tmm_mas_gba_sample
{
    mm_word     length;
    mm_word     loop_length; // Loop length (0xFFFFFFFF if sample doesn't loop)
    mm_byte     format;
    mm_byte     reserved;
    mm_hword    default_frequency;

    mm_byte     data[];

    // ::8-bit sample data
} mm_mas_gba_sample;

MM_STATIC_ASSERT(mm_mas_gba_sample, 12)

typedef struct tmm_mas_ds_sample
{
    mm_word     loop_start;
    union
    {
        mm_word     loop_length;
        mm_word     length;
    };
    mm_byte     format;             // MM_SFORMAT_8BIT, MM_SFORMAT_16BIT or MM_SFORMAT_ADPCM
    mm_byte     repeat_mode;        // MM_SREPEAT_FORWARD or MM_SREPEAT_OFF
    mm_hword    default_frequency;
    mm_word     point;              // Always zero in mmutil

    mm_byte     data[];             // Sample data

    // ::sample data
} mm_mas_ds_sample;

MM_STATIC_ASSERT(mm_mas_ds_sample, 16)

#define MM_SFORMAT_8BIT         0 // 8 bit
#define MM_SFORMAT_16BIT        1 // 16 bit
#define MM_SFORMAT_ADPCM        2 // ADPCM (invalid)

#define MM_SREPEAT_FORWARD      1 // Forward loop
#define MM_SREPEAT_OFF          2 // No loop

#ifdef __cplusplus
}
#endif

#endif // MM_MAS_H__
