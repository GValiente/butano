// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020-2021 Antonio Niño Díaz

#ifndef SOUND_H__
#define SOUND_H__

#include "definitions.h"

// Enable or disable sound hardware. It must be enabled before doing anything
// else with sound hardware.
EXPORT_API void SOUND_MasterEnable(int enable);

// Set volume of DMA channels. Valid values are 100 and 50. Any other value will
// be treated as 50%.
EXPORT_API void SOUND_DMA_Volume(int dma_a_max, int dma_b_max);

// Set master volume of all PSG channels. Valid values are 100, 50 and 25.. Any
// other value will be treated as 25%.
EXPORT_API void SOUND_PSG_MasterVolume(int volume);

// Set left and right volume of all PSG channels. Valid values are 0 to 7.
EXPORT_API void SOUND_PSG_Volume(int volume_left, int volume_right);

// Specify which DMA channels are heard from which speakers.
EXPORT_API
void SOUND_DMA_Pan(int dma_a_left, int dma_a_right,
                   int dma_b_left, int dma_b_right);

// Specify which PSG channels are heard from which speakers.
EXPORT_API
void SOUND_PSG_Pan(int left_1, int right_1, int left_2, int right_2,
                   int left_3, int right_3, int left_4, int right_4);

// Set buffer that will be streamed to DMA channel A. The buffer needs to be
// aligned to 32 bits.
EXPORT_API void SOUND_DMA_Stream_A(const void *source);

// Set buffer that will be streamed to DMA channel B. The buffer needs to be
// aligned to 32 bits.
EXPORT_API void SOUND_DMA_Stream_B(const void *source);

// Set buffers that will be streamed to DMA channels A and B. The buffers need
// to be aligned to 32 bits.
EXPORT_API void SOUND_DMA_Setup_AB(const void *source_a, const void *source_b);

// Restart DMA transfer used for audio.
EXPORT_API void SOUND_DMA_Retrigger_AB(void);

// Specify which timer is assigned to each one of the DMA channels. Only timers
// 0 and 1 are valid.
EXPORT_API void SOUND_DMA_TimerSetup(int dma_a_timer, int dma_b_timer);

#endif // SOUND_H__
