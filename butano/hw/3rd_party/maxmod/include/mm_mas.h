/****************************************************************************
 *                                                          __              *
 *                ____ ___  ____ __  ______ ___  ____  ____/ /              *
 *               / __ `__ \/ __ `/ |/ / __ `__ \/ __ \/ __  /               *
 *              / / / / / / /_/ />  </ / / / / / /_/ / /_/ /                *
 *             /_/ /_/ /_/\__,_/_/|_/_/ /_/ /_/\____/\__,_/                 *
 *                                                                          *
 *         Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)         *
 *                                                                          *
 * Permission to use, copy, modify, and/or distribute this software for any *
 * purpose with or without fee is hereby granted, provided that the above   *
 * copyright notice and this permission notice appear in all copies.        *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES *
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF         *
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR  *
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES   *
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN    *
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF  *
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.           *
 ****************************************************************************/

// MAXMOD MODULE FORMAT DEFINITIONS

#ifndef MM_MAS_H
#define MM_MAS_H

#define _____badbear_____

#include "mm_types.h"

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
	mm_byte		nna;
	mm_byte		dct;
	mm_byte		dca;
	mm_byte		env_flags;
	mm_byte		panning;
	mm_hword	note_map[120];

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
	mm_byte		global_volume;
	mm_byte		default_volume;
	mm_hword	frequency;
	mm_byte		av_type; // (auto vibrato)
	mm_byte		av_depth;
	mm_byte		av_speed;
	mm_byte		panning;
	mm_hword	av_rate;
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
	}
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

#endif
