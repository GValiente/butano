// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2021, Antonio Niño Díaz

#ifndef MM_CORE_EFFECT_H__
#define MM_CORE_EFFECT_H__

#include <mm_types.h>

// This must be at most 254 to prevent overflows in SFX handles
#define EFFECT_CHANNELS 16

void mmResetEffects(void);
void mmUpdateEffects(void);

#endif // MM_CORE_EFFECT_H__
