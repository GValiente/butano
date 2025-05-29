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

// MAXMOD SOUNDBANK FORMAT DEFINITIONS

#ifndef MM_MSL_H
#define MM_MSL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mm_types.h>

typedef struct tmslheaddata
{
    mm_hword    sampleCount;
    mm_hword    moduleCount;
    mm_word     reserved[2];
} msl_head_data;

typedef struct tmslhead
{
    msl_head_data   head_data;
    mm_addr         sampleTable[]; // [MSL_NSAMPS];
    //mm_addr         moduleTable[MSL_NSONGS];
} msl_head;

// sample structure......................................

typedef struct {
    mm_word len;        // Length
    mm_word loop;       // Loop lenght (0xFFFFFFFF if sample doesn't loop)
    mm_byte format;     // Format
    mm_byte unknown;    // BYTESMASHER?
    mm_hword freqyency;
    mm_word point;      // TODO: mmutil doesn't seem to save this value: Write_SampleData()
    mm_byte data[];
} msl_gba_sample;

#define C_SAMPLE_LEN        0
#define C_SAMPLE_LOOP       4
#define C_SAMPLE_POINT      12
#define C_SAMPLE_DATA       16

typedef struct {
    mm_word lstart;     // Loop start
    mm_word len;        // Length
    mm_byte format;     // 0 = 8 bit, 1 = 16 bit, 2 = ADPCM/other (invalid)
    mm_byte rep;        // 1 = forward loop, 2 = no loop
    mm_hword frequency;
    mm_word point;      // TODO: This is always set to 0 by mmutil: Write_SampleData()
    mm_byte data[];     // Sample data
} msl_ds_sample;

#define C_SAMPLEN_LSTART    0
#define C_SAMPLEN_LEN       4
#define C_SAMPLEN_FORMAT    8
#define C_SAMPLEN_REP       9
#define C_SAMPLEN_POINT     12
#define C_SAMPLEN_DATA      16

#define C_SAMPLEC_DFREQ     10

#ifdef __cplusplus
}
#endif

#endif // MM_MSL_H
