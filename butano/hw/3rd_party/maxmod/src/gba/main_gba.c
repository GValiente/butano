// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2021-2025, Antonio Niño Díaz (antonio_nd@outlook.com)

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <maxmod.h>
#include <mm_mas.h>
#include <mm_msl.h>

#include "../core/effect.h"
#include "../core/mas.h"
#include "../core/mixer.h"
#include "../core/player_types.h"
#include "mixer.h"

// Buffer that holds the mixed data
#define mixlen MM_MIXLEN_16KHZ // 16 KHz
static uint32_t mixbuffer[mixlen / sizeof(uint32_t)];

// Address of soundbank in memory/rom
msl_head *mp_solution;

// Number of modules in sound bank
mm_word mmModuleCount;

// Number of samples in sound bank
mm_word mmSampleCount;

// Pointer to buffer allocated by mmInitDefault()
static mm_addr mm_init_default_buffer = NULL;

// This is set to true when Maxmod is initialized
static bool mm_initialized = false;

// Initialize maxmod
bool mmInit(mm_gba_system *setup)
{
    mp_solution = setup->soundbank;

    mmSampleCount = mp_solution->head_data.sampleCount;
    mmModuleCount = mp_solution->head_data.moduleCount;

    mm_achannels = setup->active_channels;
    mm_pchannels = setup->module_channels;
    mm_num_mch = setup->mod_channel_count;
    mm_num_ach = setup->mix_channel_count;

    if ((mm_num_mch > 32) || (mm_num_ach > 32))
        return false;

    mmMixerInit(setup); // Initialize software/hardware mixer

    mm_ch_mask = (1U << mm_num_ach) - 1;

    mmSetModuleVolume(0x400);
    mmSetJingleVolume(0x400);
    mmSetEffectsVolume(0x400);

    mmSetModuleTempo(0x400);

    mmSetModulePitch(0x400);

    mmResetEffects();

    mm_initialized = true;

    return true;
}

bool mmInitDefault(mm_addr soundbank, mm_word number_of_channels)
{
    if (number_of_channels > 32)
        return false;

    // Allocate buffer
    size_t size_of_channel = sizeof(mm_module_channel) + sizeof(mm_active_channel) + sizeof(mm_mixer_channel);
    size_t size_of_buffer = mixlen + (number_of_channels * size_of_channel);

    mm_init_default_buffer = calloc(1, size_of_buffer);
    if (mm_init_default_buffer == NULL)
        return false;

    // Split up buffer
    mm_addr wave_memory, module_channels, active_channels, mixing_channels;

    wave_memory = mm_init_default_buffer;
    module_channels = (mm_addr)(((uintptr_t)wave_memory) + mixlen);
    active_channels = (mm_addr)(((uintptr_t)module_channels) + (number_of_channels * sizeof(mm_module_channel)));
    mixing_channels = (mm_addr)(((uintptr_t)active_channels) + (number_of_channels * sizeof(mm_active_channel)));

    mm_gba_system setup =
    {
        .mixing_mode = MM_MIX_16KHZ,
        .mod_channel_count = number_of_channels,
        .mix_channel_count = number_of_channels,
        .module_channels = module_channels,
        .active_channels = active_channels,
        .mixing_channels = mixing_channels,
        .mixing_memory = (mm_addr)&mixbuffer[0],
        .wave_memory = wave_memory,
        .soundbank = soundbank
    };

    if (!mmInit(&setup))
    {
        free(mm_init_default_buffer);
        return false;
    }

    return true;
}

bool mmEnd(void)
{
    mm_initialized = false;

    mmMixerEnd();

    if (mm_init_default_buffer)
    {
        free(mm_init_default_buffer);
        mm_init_default_buffer = NULL;
    }

    return true;
}

// Work routine, user _must_ call this every frame.
void mmFrame(void)
{
    if (!mm_initialized)
        return;

    // Update effects

    mmUpdateEffects();

    // Update sub layer
    // Sub layer has 60hz accuracy

    mppUpdateSub();

    // Update main layer and mix samples.
    // Main layer is sample-accurate.

    // Copy channels
    mpp_channels = mm_pchannels;

    // Copy #channels
    mpp_nchannels = mm_num_mch;

    // layer=0 (main)
    mpp_clayer = MM_MAIN;

    // Copy layer pointer
    mpp_layerp = &mmLayerMain; // mpp_layerA

    // Check if main layer is active.
    // Skip processing if disabled (and just mix samples)
    if (mpp_layerp->isplaying == 0)
    {
        // Main layer isn't active, mix full amount
        mmMixerMix(mm_mixlen);
        return;
    }

    // mixlen is divisible by 2
    int remaining_len = mm_mixlen;

    while (1)
    {
        // Get samples/tick
        int sample_num = mpp_layerp->tickrate;

        // Get sample count
        int sampcount = mpp_layerp->sampcount;

        // Calc tickrate-counter
        sample_num -= sampcount;

        if (sample_num < 0)
            sample_num = 0;

        if (sample_num >= remaining_len)
            break; // Mix remaining samples

        // Mix and process tick

        // Reset sample counter
        mpp_layerp->sampcount = 0;

        // subtract from #samples to mix
        remaining_len -= sample_num;

        mmMixerMix(sample_num); // mix samples

        mppProcessTick();
    }

    // Add samples remaining to SAMPCOUNT and mix more samples

    mpp_layerp->sampcount += remaining_len;

    mmMixerMix(remaining_len);
}

mm_word mmGetModuleCount(void)
{
    return mmModuleCount;
}

mm_word mmGetSampleCount(void)
{
    return mmSampleCount;
}
