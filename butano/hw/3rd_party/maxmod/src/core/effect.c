// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2021-2026, Antonio Niño Díaz

#include <string.h>

#if defined(__GBA__)
#include <maxmod.h>
#elif defined(__NDS__)
#include <maxmod7.h>
#elif defined(__HEADLESS__)
#include <maxmod_headless.h>
#endif

#include <mm_mas.h>
#include <mm_msl.h>

#include "channel_types.h"
#include "effect.h"
#include "mas.h"
#include "mixer.h"
#if defined(__GBA__)
#include "../gba/main_gba.h"
#include "../gba/mixer.h"
#elif defined(__NDS__)
#include "../ds/arm7/main_ds7.h"
#include "../ds/arm7/mixer.h"
#elif defined(__HEADLESS__)
#include "../headless/main_headless.h"
#include "../headless/mixer.h"
#endif

#define releaseLevel    200

static mm_word mm_sfx_mastervolume; // 0 to 1024

// Struct that holds information about a sfx being played
typedef struct {
    mm_byte mix_channel; // mixer channel index + 1 (0 means disabled)
    mm_byte counter; // Taken from mm_sfx_counter
} mm_sfx_channel_state;

static mm_sfx_channel_state mm_sfx_channels[EFFECT_CHANNELS];

static mm_word mm_sfx_bitmask; // Channels in use

// Counter that increments every time a new effect is played
static mm_byte mm_sfx_counter;

// Test handle and return mixing channel index
static int mme_get_mix_channel_index(mm_sfxhand handle)
{
    int sfx_channel = (handle & 0xFF) - 1;
    mm_byte handle_counter = handle >> 8;

    if (sfx_channel < 0)
        return -1;

    if (sfx_channel >= EFFECT_CHANNELS)
        return -1;

    mm_sfx_channel_state *state = &mm_sfx_channels[sfx_channel];

    // Check if counters match
    if (state->counter != handle_counter)
        return -1;

    return state->mix_channel - 1;
}

// Clear sfx channel entry and bitmask
static void mme_clear_sfx_channel(int sfx_channel)
{
    // Clear channel effect

    mm_sfx_channels[sfx_channel].counter = 0;
    mm_sfx_channels[sfx_channel].mix_channel = 0;

    // Clear effect bitmask

    mm_word bit_flag = 1U << sfx_channel;

    mm_sfx_bitmask &= ~bit_flag;
}

// This is only meant to be used by Maxmod internally when initializing the
// sound engine.
void mmResetEffects(void)
{
    for (int i = 0; i < EFFECT_CHANNELS; i++)
    {
        mm_sfx_channels[i].counter = 0;
        mm_sfx_channels[i].mix_channel = 0;
    }

    mm_sfx_bitmask = 0;
}

// Return index to free effect channel. If no channels are free, it returns -1.
static int mme_get_free_sfx_channel(void)
{
    // Look for the first clear bit
    for (int i = 0; i < EFFECT_CHANNELS; i++)
    {
        if (mm_sfx_bitmask & (1 << i))
            continue;

        return i;
    }

    return -1;
}

// Play sound effect with default parameters
mm_sfxhand mmEffect(mm_word sample_ID)
{
    mm_sound_effect effect =
    {
        .id = sample_ID,
        .sample = NULL,
        .rate = 1024,
        .handle = 0,
        .volume = 255,
        .panning = 128
    };

    return mmEffectEx(&effect);
}

// Play sound effect with specified parameters
mm_sfxhand mmEffectEx(mm_sound_effect *sound)
{
    if (sound->id >= mmGetSampleCount())
        return MM_SFXHAND_INVALID;

    int sfx_channel = -1;
    int mix_channel = NO_CHANNEL_AVAILABLE;
    mm_byte sfx_count;

    // Reuse or create new SFX handle
    // ------------------------------

    bool reused_handle = false;

    if (sound->handle != 0)
    {
        // If there is a provided handle, check if it's valid
        mix_channel = mme_get_mix_channel_index(sound->handle);
        if (mix_channel >= 0)
        {
            // It's valid, reuse the old mixer channel, as well as the sfx
            // channel and count.
            sfx_channel = (sound->handle & 0xFF) - 1;
            sfx_count = sound->handle >> 8;

            reused_handle = true;
        }
    }

    if (!reused_handle)
    {
        // No reused handle, generate a new one

        sfx_channel = mme_get_free_sfx_channel();
        if (sfx_channel < 0)
            return MM_SFXHAND_INVALID;

        // Allocate new mixer channel
        mix_channel = mmAllocChannel();
        if (mix_channel == NO_CHANNEL_AVAILABLE)
            return MM_SFXHAND_INVALID;

        sfx_count = mm_sfx_counter;

        mm_sfx_counter++;
    }

    // Generate new handle and register SFX information
    // ------------------------------------------------

    mm_sfxhand handle = (sfx_count << 8) | (sfx_channel + 1);

    mm_sfx_channels[sfx_channel].mix_channel = mix_channel + 1;
    mm_sfx_channels[sfx_channel].counter = sfx_count;

    // Mark sfx channel as used
    mm_sfx_bitmask |= 1U << sfx_channel;

    // Setup active channel
    // --------------------

    mm_active_channel *act_ch = &mm_achannels[mix_channel];

    act_ch->fvol = releaseLevel;

    if (handle == 0)
        act_ch->type = ACHN_BACKGROUND;
    else
        act_ch->type = ACHN_CUSTOM;

    act_ch->flags = MCAF_EFFECT;

    // Setup mixer channel
    // -------------------

    mm_mixer_channel *mix_ch = &mm_mix_channels[mix_channel];

    mm_mas_platform_sample *plat_sample;

    // If the sample pointer is NULL, use the MSL ID.
    if (sound->sample == NULL)
    {
        // The sound effect is using an ID number, get the corresponding address
        // of the sample in main RAM.
        plat_sample = mmGetSampleAddress(sound->id);
        if (plat_sample == NULL)
        {
            mmMixerChannelStop(mix_ch);
            return MM_SFXHAND_INVALID;
        }
    }
    else // The sound effect is using an arbitrary address with a MAS file
    {
        // Always skip the MAS prefix
        uintptr_t head_address = (uintptr_t)sound->sample + sizeof(mm_mas_prefix);

        // The pointer must be aligned to at least 4 bytes
        if ((head_address & 3) != 0)
            return MM_SFXHAND_INVALID;

        mm_mas_head *head = (mm_mas_head *)head_address;

        // Check that the MAS file only contains samples
        if ((head->order_count > 0) || (head->instr_count > 0) || (head->pattn_count > 0))
            return MM_SFXHAND_INVALID;

        // For now, only mono sound effects are supported
        // TODO: Support MAS files with 2 samples and play them as stereo
        if (head->sampl_count != 1)
            return MM_SFXHAND_INVALID;

        uintptr_t sample_address = head_address + head->tables[0];

        const mm_mas_sample_info *sample_info = (const mm_mas_sample_info *)sample_address;

        // Make sure that the sample data is after the sample info struct
        if (sample_info->msl_id != 0xFFFF)
            return MM_SFXHAND_INVALID;

        plat_sample = (mm_mas_platform_sample *)&(sample_info->data[0]);
    }

    // set pitch to original * pitch
#if defined(__GBA__) || defined(__HEADLESS__)
    mix_ch->freq = (sound->rate * plat_sample->default_frequency) >> (10 - 2);
#elif defined(__NDS__)
    mix_ch->freq = (sound->rate * plat_sample->default_frequency) >> 10;
#endif

    // Always play sound effects from the start
    mmMixerChannelSetSampleOffset(mix_ch, 0);

    // Save the start of the waveform and enable the channel
    mmMixerChannelPlaySample(mix_ch, plat_sample);

    mmMixerChannelSetVolume(mix_ch, (sound->volume * mm_sfx_mastervolume) >> 2);

    // Set some sound settings
    mmMixerChannelSetPan(mix_ch, sound->panning);

    return handle;
}

// Set master volume scale, 0->1024
void mmSetEffectsVolume(mm_word volume)
{
    if (volume > 1024)
        volume = 1024;

    mm_sfx_mastervolume = volume;
}

// Set effect panning (0..255)
mm_word mmEffectPanning(mm_sfxhand handle, mm_byte panning)
{
    int mix_channel = mme_get_mix_channel_index(handle);
    if (mix_channel < 0)
        return 0;

    mmMixerSetPan(mix_channel, panning);

    return 1;
}

// Indicates if a sound effect is active or not
mm_bool mmEffectActive(mm_sfxhand handle)
{
    int mix_channel = mme_get_mix_channel_index(handle);
    if (mix_channel < 0)
        return 0;

    return 1;
}

// Set effect volume (0..255)
mm_word mmEffectVolume(mm_sfxhand handle, mm_word volume)
{
    int mix_channel = mme_get_mix_channel_index(handle);
    if (mix_channel < 0)
        return 0;

    if (volume > 255)
        volume = 255;

    mmMixerSetVolume(mix_channel, (volume * mm_sfx_mastervolume) >> 2);

    return 1;
}

// Set effect playback rate
mm_word mmEffectRate(mm_sfxhand handle, mm_word rate)
{
    int mix_channel = mme_get_mix_channel_index(handle);
    if (mix_channel < 0)
        return 0;

    mmMixerSetFreq(mix_channel, rate);

    return 1;
}

// Scale sampling rate by 6.10 factor
mm_word mmEffectScaleRate(mm_sfxhand handle, mm_word factor)
{
    int mix_channel = mme_get_mix_channel_index(handle);
    if (mix_channel < 0)
        return 0;

    mmMixerMulFreq(mix_channel, factor);

    return 1;
}

// Stop sound effect
mm_word mmEffectCancel(mm_sfxhand handle)
{
    int mix_channel = mme_get_mix_channel_index(handle);
    if (mix_channel < 0)
        return 0;

    // Free achannel
    mm_active_channel *act_ch = &mm_achannels[mix_channel];

    act_ch->type = ACHN_BACKGROUND;
    act_ch->fvol = 0; // Clear volume for channel allocator

    mm_word sfx_channel = (handle & 0xFF) - 1;
    mme_clear_sfx_channel(sfx_channel);

    mmMixerStopChannel(mix_channel);

    return 1;
}

// Release sound effect (allow interruption)
mm_word mmEffectRelease(mm_sfxhand handle)
{
    int mix_channel = mme_get_mix_channel_index(handle);
    if (mix_channel < 0)
        return 0;

    // Release achannel
    mm_active_channel *act_ch = &mm_achannels[mix_channel];

    act_ch->type = ACHN_BACKGROUND;

    mm_word sfx_channel = (handle & 0xFF) - 1;
    mme_clear_sfx_channel(sfx_channel);

    return 1;
}

// Stop all sound effects
void mmEffectCancelAll(void)
{
    // Clear SFX channel state

    mmResetEffects();

    // Look for all active channels that are playing an effect (flag
    // MCAF_EFFECT) and stop them.

    mm_mixer_channel *mix_ch = &mm_mix_channels[0];
    mm_active_channel *act_ch = &mm_achannels[0];

    for (mm_word i = 0; i < mm_num_ach; i++, act_ch++, mix_ch++)
    {
        // Skip channels that aren't playing an effect
        if ((act_ch->flags & MCAF_EFFECT) == 0)
            continue;

        // Clear achannel data to zero
        memset(act_ch, 0, sizeof(mm_active_channel));

        // Disable mixer channel. Disabled status differs between systems.
        mmMixerChannelStop(mix_ch);
    }
}

// This looks for any sound effect that has ended and it clears its associated
// active channel and SFX channel. Then, it updates the mask of available SFX
// channels.
void mmUpdateEffects(void)
{
    // Keep track of the channels that are still active after the update
    mm_word new_bitmask = 0;

    for (int i = 0; i < EFFECT_CHANNELS; i++)
    {
        if ((mm_sfx_bitmask & (1 << i)) == 0)
            continue;

        // Get channel index

        int mix_channel = mm_sfx_channels[i].mix_channel - 1;
        if (mix_channel < 0)
            continue;

        // Test if channel is still active

        mm_mixer_channel *mix_ch = &mm_mix_channels[mix_channel];
        if (mmMixerChannelIsActive(mix_ch))
        {
            new_bitmask |= (1 << i);
            continue;
        }

        // Free achanel if it isn't active

        mm_active_channel *act_ch = &mm_achannels[mix_channel];

        act_ch->type = 0;
        act_ch->flags = 0;

        mm_sfx_channels[i].counter = 0;
        mm_sfx_channels[i].mix_channel = 0;
    }

    // Update the mask of active channels
    mm_sfx_bitmask = new_bitmask;
}
