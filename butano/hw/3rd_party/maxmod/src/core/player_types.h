// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2021-2025, Antonio Niño Díaz (antonio_nd@outlook.com)

#ifndef MM_CORE_PLAYER_TYPES_H__
#define MM_CORE_PLAYER_TYPES_H__

#include <mm_types.h>

typedef struct {
    mm_byte     tick;       // Current tick count
    mm_byte     row;        // Current row being played
    mm_byte     position;   // Module sequence position
    mm_byte     nrows;      // Number of rows in current pattern
    mm_byte     global_volume; // Global volume multiplier
    mm_byte     speed;      // Speed of module (ticks / row)
    mm_byte     isplaying;  // Module is active
    mm_byte     bpm;        // Tempo of module
    mm_word    *insttable;  // Table of offsets (from mm_mas_head base) to instrument data
    mm_word    *samptable;  // Table of offsets (from mm_mas_head base) to sample data
    mm_word    *patttable;  // Table of offsets (from mm_mas_head base) to pattern data
    mm_mas_head *songadr;   // Pointer to the current MAS module being played
    mm_byte     flags;
    mm_byte     oldeffects;
    mm_byte     pattjump;
    mm_byte     pattjump_row;
    mm_byte     fpattdelay;
    mm_byte     pattdelay;

    mm_byte     ploop_row;   // }
    mm_byte     ploop_times; // } Variables used for pattern loops
    mm_byte    *ploop_adr;   // }
    mm_byte    *pattread;
    mm_byte     ploop_jump;
    mm_byte     valid;

    mm_hword    tickrate;  // 1.15 fixed point OR sample count
    union {
        mm_hword    sampcount; // sample timing
        mm_hword    tickfrac;  // vsync  timing 0.16 fixed point
    };

    mm_byte     mode;
    mm_byte     reserved2;
    mm_word     mch_update;
    mm_hword    volume;
    mm_hword    reserved3;
} mpl_layer_information;

// Active Information
// ------------------

typedef struct {
    mm_word     reserved;
    mm_byte    *pattread_p;
    mm_byte     afvol;
    mm_byte     sampoff;
    mm_sbyte    volplus;
    mm_byte     notedelay;
    mm_hword    panplus;
    mm_hword    reserved2;
} mpv_active_information;

#endif // MM_CORE_PLAYER_TYPES_H__
