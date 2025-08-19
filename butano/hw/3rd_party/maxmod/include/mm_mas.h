// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2025, Antonio Niño Díaz (antonio_nd@outlook.com)

/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ `__ \/ __ `/ |/ / __ `__ \/ __ \/ __  /               *
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

#define MAS_TYPE_SONG       0
#define MAS_TYPE_SAMPLE_GBA 1
#define MAS_TYPE_SAMPLE_NDS 2

typedef struct tmm_mas_head
{
    mm_byte     order_count;
    mm_byte     instr_count;
    mm_byte     sampl_count;
    mm_byte     pattn_count;
    mm_byte     flags;
    mm_byte     global_volume;
    mm_byte     initial_speed;
    mm_byte     initial_tempo;
    mm_byte     repeat_position;
    mm_byte     reserved[3];
    mm_byte     channel_volume[32];
    mm_byte     channel_panning[32];
    mm_byte     sequence[200];

    mm_addr     tables[];

    // ::instrument table
    // ::sample info table
    // ::pattern table
} mm_mas_head;

#define MAS_HEADER_FLAG_LINK_GXX    (1 << 0) // Shared Gxx
#define MAS_HEADER_FLAG_OLD_EFFECTS (1 << 1) // TODO: Unused flag
#define MAS_HEADER_FLAG_FREQ_MODE   (1 << 2) // 1 = Linear freqs, 0 = Amiga freqs
#define MAS_HEADER_FLAG_XM_MODE     (1 << 3) // 1 = XM mode, 0 = Other/IT mode?
#define MAS_HEADER_FLAG_MSL_DEP     (1 << 4) // TODO: Unused flag
#define MAS_HEADER_FLAG_OLD_MODE    (1 << 5) // 1 = MOD/S3M, 0 = Other

typedef struct tmm_mas_instrument
{
    mm_byte     global_volume;
    mm_byte     fadeout;
    mm_byte     random_volume;
    mm_byte     dct;
    mm_byte     nna;
    mm_byte     env_flags;
    mm_byte     panning;
    mm_byte     dca;
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

#define MAS_INSTR_FLAG_VOL_ENV_EXISTS   (1 << 0) // Volume envelope exists
#define MAS_INSTR_FLAG_PAN_ENV_EXISTS   (1 << 1) // Panning envelope exists
#define MAS_INSTR_FLAG_PITCH_ENV_EXISTS (1 << 2) // Pitch envelope exists
#define MAS_INSTR_FLAG_VOL_ENV_ENABLED  (1 << 3) // Volume envelope enabled
// In XM, bits 0 and 3 are always set together. In IT, they can be set
// independently. Other formats don't use them.

typedef struct
{
    mm_shword   delta;
    mm_hword    base : 7;  // node_y. (0->64 for vol, -32->+32 for panning or pitch)
    mm_hword    range : 9; // 1 to 511
}
mm_mas_envelope_node;

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

typedef struct tmm_mas_sample_info
{
    mm_byte     default_volume;
    mm_byte     panning;
    mm_hword    frequency;
    mm_byte     av_type; // VIT (auto vibrato)
    mm_byte     av_depth; // VID
    mm_byte     av_speed; // VIS
    mm_byte     global_volume; // GV
    mm_hword    av_rate; // VIR
    mm_hword    msl_id;

    // If msl_id == 0xFFFF this is a mm_mas_gba_sample or mm_mas_ds_sample
    // struct. If not, there is no more data.
    mm_byte     data[];

    // ::sample may follow
} mm_mas_sample_info;

typedef struct tmm_mas_pattern
{
    mm_byte     row_count; // Number of rows of the pattern

    mm_byte     pattern_data[];

    // ::pattern data
} mm_mas_pattern;

typedef struct tmm_mas_gba_sample
{
    mm_word     length;
    mm_word     loop_length; // Loop lenght (0xFFFFFFFF if sample doesn't loop)
    mm_byte     format;
    mm_byte     reserved;
    mm_hword    default_frequency;

    mm_byte     data[];

    // ::8-bit sample data
} mm_mas_gba_sample;

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

#define MM_SFORMAT_8BIT         0 // 8 bit
#define MM_SFORMAT_16BIT        1 // 16 bit
#define MM_SFORMAT_ADPCM        2 // ADPCM (invalid)

#define MM_SREPEAT_FORWARD      1 // Forward loop
#define MM_SREPEAT_OFF          2 // No loop

#ifdef __cplusplus
}
#endif

#endif // MM_MAS_H__
