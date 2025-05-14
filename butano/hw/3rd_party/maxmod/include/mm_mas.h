// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)

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

#ifndef MM_MAS_H
#define MM_MAS_H

#ifdef __cplusplus
extern "C" {
#endif

#define _____badbear_____

#include <mm_types.h>

// prefix comes first

typedef struct tmm_mas_prefix
{
	mm_word		size;
	mm_byte		type;
	mm_byte		version;
	mm_byte		reserved[2];
} mm_mas_prefix;

typedef struct tmm_mas_head
{
	mm_byte		order_count;
	mm_byte		instr_count;
	mm_byte		sampl_count;
	mm_byte		pattn_count;
	mm_byte		flags;
	mm_byte		global_volume;
	mm_byte		initial_speed;
	mm_byte		initial_tempo;
	mm_byte		repeat_position;
	mm_byte		reserved[3];
	mm_byte		channel_volume[32];
	mm_byte		channel_panning[32];
	mm_byte		sequence[200];

#ifdef _____badbear_____
	mm_addr		tables[];
#endif
	// ::instrument table
	// ::sample info table
	// ::pattern table
} mm_mas_head;

typedef struct tmm_mas_instrument
{
	mm_byte		global_volume;
	mm_byte		fadeout;
	mm_byte		random_volume;
	mm_byte		dct;
	mm_byte		nna;
	mm_byte		env_flags;
	mm_byte		panning;
	mm_byte		dca;
	mm_hword	note_map_offset : 15;
	mm_hword	is_note_map_invalid : 1;
	mm_hword	note_map[119];

#ifdef _____badbear_____
	mm_byte		envelopes[];
#endif
	// ::envelopes
} mm_mas_instrument;

typedef struct tmm_mas_envelope
{
	mm_byte		size;
	mm_byte		loop_start;
	mm_byte		loop_end;
	mm_byte		sus_start;
	mm_byte		sus_end;
	mm_byte		node_count;
	mm_byte		is_filter; // (maybe supported someday :)
	mm_byte		wasted;

#ifdef _____badbear_____
	mm_byte		env_nodes[];
#endif

	// ::envelope nodes
} mm_mas_envelope;

typedef struct tmm_mas_sample_info
{
	mm_byte		default_volume;
	mm_byte		panning;
	mm_hword	frequency;
	mm_byte		av_type; // VIT (auto vibrato)
	mm_byte		av_depth; // VID
	mm_byte		av_speed; // VIS
	mm_byte		global_volume; // GV
	mm_hword	av_rate; // VIR
	mm_hword	msl_id;

#ifdef _____badbear_____
	mm_byte		data[];
#endif

	// ::sample may follow
} mm_mas_sample_info;

typedef struct tmm_mas_pattern
{
	mm_byte		row_count;

#ifdef _____badbear_____
	mm_byte		pattern_data[];
#endif

	// ::pattern data
} mm_mas_pattern;

typedef struct tmm_mas_gba_sample
{
	mm_word		length;
	mm_word		loop_length;
	mm_hword	reserved;
	mm_hword	default_frequency;

#ifdef _____badbear_____
	mm_byte		data[];
#endif

	// ::8-bit sample data
} mm_mas_gba_sample;

typedef struct tmm_mas_ds_sample
{
	mm_word		loop_start;
	union {
		mm_word	loop_length;
		mm_word	length;
	};
	mm_byte		format;
	mm_byte		repeat_mode;
	mm_hword	default_frequency;

#ifdef _____badbear_____
	mm_byte		data[];
#endif

	// ::sample data
} mm_mas_ds_sample;

#define MM_SFORMAT_8BIT		0
#define MM_SFORMAT_16BIT	1
#define MM_SFORMAT_ADPCM	2

#define MM_SREPEAT_FORWARD	1
#define MM_SREPEAT_OFF		2

// -----------------------------------------------------------

#include "mp_mas_structs.h"

extern mpl_layer_information mmLayerMain;
extern mpl_layer_information mmLayerSub;

extern mpl_layer_information *mpp_layerp;
extern mpv_active_information mpp_vars;
extern mm_addr mpp_channels;
extern mm_word mpp_resolution;

extern mm_word mm_mastertempo;
extern mm_word mm_masterpitch;

extern mm_byte mpp_nchannels;
extern mm_byte mpp_clayer;
extern mm_active_channel *mm_achannels;
extern mm_addr mm_pchannels;
extern mm_word mm_num_mch;
extern mm_word mm_num_ach;
extern mm_module_channel mm_schannels[]; // [MP_SCHANNELS]

void mmSetResolution(mm_word);
void mmPulse(void);
void mppUpdateSub(void);
void mppProcessTick(void); // TODO: Mark this as IWRAM in GBA?

#ifdef __cplusplus
}
#endif

#endif
