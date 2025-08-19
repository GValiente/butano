// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2021, Antonio Niño Díaz (antonio_nd@outlook.com)

#ifndef MM_GBA_MIXER_H
#define MM_GBA_MIXER_H

#include "../core/channel_types.h"

#define MP_SAMPFRAC     12

#define REG_SOUNDCNT_L  *(volatile uint16_t *)0x4000080
#define REG_SOUNDCNT_H  *(volatile uint16_t *)0x4000082
#define REG_SOUNDCNT_X  *(volatile uint16_t *)0x4000084

#define REG_TM0CNT      *(volatile uint32_t *)0x4000100

#define REG_DMA1SAD     *(volatile uint32_t *)0x40000BC
#define REG_DMA1DAD     *(volatile uint32_t *)0x40000C0
#define REG_DMA1CNT     *(volatile uint32_t *)0x40000C4
#define REG_DMA1CNT_H   *(volatile uint16_t *)0x40000C6

#define REG_DMA2SAD     *(volatile uint32_t *)0x40000C8
#define REG_DMA2DAD     *(volatile uint32_t *)0x40000CC
#define REG_DMA2CNT     *(volatile uint32_t *)0x40000D0
#define REG_DMA2CNT_H   *(volatile uint16_t *)0x40000D2

#define REG_DMA3SAD     *(volatile uint32_t *)0x40000D4
#define REG_DMA3DAD     *(volatile uint32_t *)0x40000D8
#define REG_DMA3CNT     *(volatile uint32_t *)0x40000DC

#define REG_SGFIFOA     (volatile uint32_t *)0x40000A0
#define REG_SGFIFOB     (volatile uint32_t *)0x40000A4

extern mm_mixer_channel *mm_mix_channels;
extern mm_word mm_mixlen;

extern mm_word mm_bpmdv;

void mmMixerInit(mm_gba_system* setup);
void mmMixerMix(mm_word samples_count);
void mmMixerSetRead(int channel, mm_word value);
void mmMixerEnd(void);

#endif // MM_GBA_MIXER_H
