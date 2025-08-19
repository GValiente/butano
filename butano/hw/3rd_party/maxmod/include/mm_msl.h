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

// MAXMOD SOUNDBANK FORMAT DEFINITIONS

#ifndef MM_MSL_H__
#define MM_MSL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <mm_types.h>

typedef struct tmslheaddata
{
    mm_hword    sampleCount;
    mm_hword    moduleCount;
    mm_word     reserved[2]; // This contains the string "*maxmod*"
}
msl_head_data;

/// This structure represents a sound bank file.
///
/// A sound bank file is just a MSL file. It's composed of a MSL header followed
/// by multiple MAS files. There are two types of MAS file:
///
/// - MAS file formed of one sample: Sound effects. Created from WAV files.
///
/// - MAS file with samples, instruments and patterns: Songs. Created from MOD,
///   S3M, XM and IT files).
///
/// The MSL header contains the total number of samples and modules, as well as
/// two tables with offsets to each MAS file with a sample and each MAS file
/// with a module.
typedef struct tmslhead
{
    msl_head_data   head_data;

    // The table of samples is followed by the list of modules. They are both
    // variable-length, so you need to check head_data.sampleCOunt to know where
    // the module table starts.
    mm_addr         sampleTable[]; // [sampleCount]
    //mm_addr         moduleTable[moduleCount];
}
msl_head;

#ifdef __cplusplus
}
#endif

#endif // MM_MSL_H__
