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
    mm_word     reserved[2]; // This contains the string "*maxmod*"
}
msl_head_data;

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

#endif // MM_MSL_H
