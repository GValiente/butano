// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2021-2026, Antonio Niño Díaz

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

#define DEFAULT_MIXLEN MM_MIXLEN_16KHZ

// Buffer that holds the mixed data
static uint32_t default_mixbuffer[DEFAULT_MIXLEN / sizeof(uint32_t)];

// Address of soundbank in memory/rom
static msl_head *mp_solution;

// Number of modules in sound bank
static mm_word mmModuleCount;

// Number of samples in sound bank
static mm_word mmSampleCount;

// Pointer to buffer allocated by mmInitDefault()
static mm_addr mm_init_default_buffer = NULL;

// This is set to true when Maxmod is initialized
static bool mm_initialized = false;

// Initialize maxmod
bool mmInit(mm_gba_system *setup)
{
    if (mm_initialized)
        return false;

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

    mmSetModuleTempo(1024);
    mmSetModulePitch(1024);

    mmSetJingleTempo(1024);
    mmSetJinglePitch(1024);

    mmResetEffects();

    mm_initialized = true;

    return true;
}

bool mmInitDefault(mm_addr soundbank, mm_word number_of_channels)
{
    if (mm_initialized)
        return false;

    if (number_of_channels > 32)
        return false;

    // Allocate buffer
    size_t size_of_channel = sizeof(mm_module_channel) + sizeof(mm_active_channel) + sizeof(mm_mixer_channel);
    size_t size_of_buffer = DEFAULT_MIXLEN + (number_of_channels * size_of_channel);

    mm_init_default_buffer = calloc(1, size_of_buffer);
    if (mm_init_default_buffer == NULL)
        return false;

    // Split up buffer
    mm_addr wave_memory, module_channels, active_channels, mixing_channels;

    wave_memory = mm_init_default_buffer;
    module_channels = (mm_addr)(((uintptr_t)wave_memory) + DEFAULT_MIXLEN);
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
        .mixing_memory = (mm_addr)&default_mixbuffer[0],
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
    if (!mm_initialized)
        return false;

    mm_initialized = false;

    mmMixerEnd();

    mmModuleCount = 0;
    mmSampleCount = 0;

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

    // Note: mm_mixlen is divisible by 2
    mm_sword remaining_samples = mm_mixlen;

    while (1)
    {
        // Check which of the two layers will have a tick first. If one of them
        // has reached a tick, process it.

        mm_sword samples_to_next_event = 0x7FFFFFFF;

        if (mmLayerMain.isplaying)
        {
            mm_sword samples_to_next_tick_main = mmLayerMain.samples_per_tick
                                               - mmLayerMain.samples_elapsed;
            if (samples_to_next_tick_main <= 0)
            {
                // mmLayerMain.samples_per_tick may change in mppProcessTickMain()
                mppProcessTickMain();
                mmLayerMain.samples_elapsed = 0;
                samples_to_next_tick_main = mmLayerMain.samples_per_tick;
            }

            samples_to_next_event = samples_to_next_tick_main;
        }

        if (mmLayerSub.isplaying)
        {
            mm_sword samples_to_next_tick_sub = mmLayerSub.samples_per_tick
                                              - mmLayerSub.samples_elapsed;
            if (samples_to_next_tick_sub <= 0)
            {
                // mmLayerSub.samples_per_tick may change in mppProcessTickSub()
                mppProcessTickSub();
                mmLayerSub.samples_elapsed = 0;
                samples_to_next_tick_sub = mmLayerSub.samples_per_tick;
            }

            if (samples_to_next_tick_sub < samples_to_next_event)
                samples_to_next_event = samples_to_next_tick_sub;
        }

        // Process samples until next event (or until the end of the buffer)

        if (samples_to_next_event > remaining_samples)
            samples_to_next_event = remaining_samples;

        mmMixerMix(samples_to_next_event);
        mmLayerMain.samples_elapsed += samples_to_next_event;
        mmLayerSub.samples_elapsed += samples_to_next_event;

        remaining_samples -= samples_to_next_event;

        if (remaining_samples == 0)
            break;
    }

    // This checks which SFXs have ended and it clears the active channel and
    // SFX channel associated to them. It's better to update it after mixing so
    // that we know which SFXs have ended right away (with mmEffectActive())
    // instead of having a frame of delay.
    //
    // Note that mmEffectCancel() clears the active and SFX channels as well, so
    // mmFrame() is always called with the SFX state up to date.
    mmUpdateEffects();
}

mm_word mmGetModuleCount(void)
{
    return mmModuleCount;
}

mm_word mmGetSampleCount(void)
{
    return mmSampleCount;
}

mm_mas_gba_sample *mmGetSampleAddress(mm_word id)
{
    uintptr_t sampleOffset = (uintptr_t)mp_solution->sampleTable[id];

    uintptr_t sample_addr = ((uintptr_t)mp_solution) + sampleOffset;

    // The pointers in sampleTank are pointers to MAS file objects. We need to
    // skip the header of the MAS file to get the pointer to the sample data.
    // Normally there is more data after the header, but a MAS file generated
    // from a WAV file only has the header and a sample.
    return (mm_mas_gba_sample *)(sample_addr + sizeof(mm_mas_prefix));
}

uintptr_t mmGetModuleAddress(mm_word id)
{
    // In the MSL format, the module table goes right after the sample table,
    // but the size of both isn't fixed. We need to calculate the start of the
    // module table by checking how big the sample table is.
    mm_hword sampleCount = mp_solution->head_data.sampleCount;
    mm_word *moduleTable = (mm_word *)&(mp_solution->sampleTable[sampleCount]);

    // Calculate the address of the module now that we have the module table. It
    // represents offsets inside the soundbank.
    uintptr_t moduleAddress = ((uintptr_t)mp_solution) + moduleTable[id];

    return moduleAddress;
}

// -----------------------------------------------------------------------------

mm_word *mppGetSampleNameList(void)
{
    mm_word *offset = (mm_word *)&(mp_solution->sampleTable[mmSampleCount + mmModuleCount]);

    mm_word dictOffset = *offset;
    if (dictOffset == 0xFFFFFFFF)
        return NULL;

    msl_names_dictionary *dict = (void *)(dictOffset + (uintptr_t)mp_solution);
    uintptr_t address = ((uintptr_t)dict) + sizeof(msl_names_dictionary);

    return (mm_word *)address;
}

mm_word *mppGetModuleNameList(void)
{
    mm_word *offset = (mm_word *)&(mp_solution->sampleTable[mmSampleCount + mmModuleCount]);

    mm_word dictOffset = *offset;
    if (dictOffset == 0xFFFFFFFF)
        return NULL;

    msl_names_dictionary *dict = (void *)(dictOffset + (uintptr_t)mp_solution);
    uintptr_t address = ((uintptr_t)dict) + sizeof(msl_names_dictionary) + dict->samplesDictSize;

    return (mm_word *)address;
}
