// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2021, Antonio Niño Díaz

#ifndef MM_GBA_MAIN_H
#define MM_GBA_MAIN_H

#include <mm_mas.h>
#include <mm_msl.h>

mm_word *mppGetSampleNameList(void);
mm_word *mppGetModuleNameList(void);

mm_mas_gba_sample *mmGetSampleAddress(mm_word id);
uintptr_t mmGetModuleAddress(mm_word id);

#endif // MM_GBA_MAIN_H
