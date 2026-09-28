// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2021-2026, Antonio Niño Díaz

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <maxmod.h>
#include <mm_mas.h>

#include "main_gba.h"
#include "mixer.h"

#define ARM_CODE   __attribute__((target("arm")))
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))

static mm_byte mp_mix_seg; // Mixing segment select

mm_addr mm_mixbuffer;

mm_mixer_channel *mm_mix_channels;

mm_word mm_bpmdv;

mm_word mm_mixlen;

mm_word mm_ratescale;

mm_addr mp_writepos; // wavebuffer write position

static mm_addr mm_wavebuffer;

static mm_word mm_mixch_count;

mm_mixer_channel *mm_mixch_end;

static mm_word mm_timerfreq;

// Pointer to a user function to be called during the vblank irq
static mm_voidfunc mm_vblank_function;

// Set channel volume
void mmMixerSetVolume(int channel, mm_word volume)
{
    mm_mix_channels[channel].vol = volume;
}

// Set channel panning
void mmMixerSetPan(int channel, mm_byte panning)
{
    mm_mix_channels[channel].pan = panning;
}

// Scale mixing frequency
void mmMixerMulFreq(int channel, mm_word factor)
{
    mm_word freq = mm_mix_channels[channel].freq;

    freq = (freq * factor) >> 10;

    mm_mix_channels[channel].freq = freq;
}

// Stop mixing channel
void mmMixerStopChannel(int channel)
{
    // Set MSB (disable) of source
    mm_mix_channels[channel].src = MIXCH_GBA_SRC_STOPPED;
}

// Set channel read position
void mmMixerSetRead(int channel, mm_word value)
{
    // Store new offset
    mm_mix_channels[channel].read = value;
}

// Set channel mixing rate
void mmMixerSetFreq(int channel, mm_word rate)
{
    mm_mix_channels[channel].freq = rate << 2;
}

static bool vblank_handler_enabled = false;

// VBL wrapper, used to reset DMA. It needs the highest priority.
IWRAM_CODE ARM_CODE void mmVBlank(void)
{
    // Disable until ready
    if (vblank_handler_enabled)
    {
        // Swap mixing segment
        mp_mix_seg = ~mp_mix_seg;

        if (mp_mix_seg != 0)
        {
            // DMA control: Restart DMA

            // Disable DMA
            REG_DMA1CNT_H = 0x0440;
            REG_DMA2CNT_H = 0x0440;

            // Restart DMA
            REG_DMA1CNT_H = 0xB600;
            REG_DMA2CNT_H = 0xB600;
        }
        else
        {
            // Restart write position
            mp_writepos = mm_wavebuffer;
        }
    }

    // Call user handler
    if (mm_vblank_function != NULL)
        mm_vblank_function();
}

// Set function to be called during the vblank IRQ
void mmSetVBlankHandler(mm_voidfunc function)
{
    mm_vblank_function = function;
}

// Get function to be called during the vblank IRQ
mm_voidfunc mmGetVBlankHandler(void)
{
    return mm_vblank_function;
}

// Initialize mixer
void mmMixerInit(mm_gba_system *setup)
{
    mm_mixch_count = setup->mix_channel_count;

    mm_mix_channels = setup->mixing_channels;

    mm_mixch_end = &mm_mix_channels[mm_mixch_count];

    mm_mixbuffer = setup->mixing_memory;

    mm_wavebuffer = setup->wave_memory;

    mp_writepos = mm_wavebuffer;

    mm_word mode = setup->mixing_mode;

    // round(rate / 59.737)
    static const mm_hword mp_mixing_lengths[] = {
        136,  176,   224,   264,   304,   352,   448,   528
    //  8khz, 10khz, 13khz, 16khz, 18khz, 21khz, 27khz, 32khz
    };

    mm_mixlen = mp_mixing_lengths[mode];

    // 15768*16384 / rate
    static const mm_hword mp_rate_scales[] = {
        31812, 24576, 19310, 16384, 14228, 12288,  9655,  8192
    //  8khz,  10khz, 13khz, 16khz, 18khz, 21khz, 27khz, 32khz
    //  8121,  10512, 13379, 15768, 18157, 21024, 26758, 31536
    };

    mm_ratescale = mp_rate_scales[mode];

    // gbaclock / rate
    static const mm_hword mp_timing_sheet[] = {
        -2066, -1596, -1254, -1064, -924,  -798,  -627,  -532
    //  8khz,  10khz, 13khz, 16khz, 18khz, 21khz, 27khz, 32khz
    };

    mm_timerfreq = mp_timing_sheet[mode];

    // rate * 2.5
    static const mm_word mp_bpm_divisors[] = {
        20302, 26280, 33447, 39420, 45393, 52560, 66895, 78840
    };

    mm_bpmdv = mp_bpm_divisors[mode];

    // Clear wave buffer
    memset(mm_wavebuffer, 0, mm_mixlen * sizeof(mm_word));

    // Reset mixing segment
    mp_mix_seg = 0;

    // Disable mixing channels

    mm_mixer_channel *mix_ch = &mm_mix_channels[0];

    for (mm_word i = 0; i < mm_mixch_count; i++)
        mix_ch[i].src = MIXCH_GBA_SRC_STOPPED;

    // Enable VBL routine
    vblank_handler_enabled = true;

    // Clear fifo data
    *REG_SGFIFOA = 0;
    *REG_SGFIFOB = 0;

    // Reset direct sound
    REG_SOUNDCNT_H = 0;

    // Setup sound: DIRECT SOUND A/B reset, timer0, A=left, B=right, volume=100%
    REG_SOUNDCNT_H = 0x9A0C;

    // Setup DMA source addresses (playback buffers)
    REG_DMA1SAD = (mm_word)mm_wavebuffer;
    REG_DMA2SAD = (mm_word)mm_wavebuffer + mm_mixlen * 2;

    // Setup DMA destination (sound fifo)
    REG_DMA1DAD = (mm_word)REG_SGFIFOA;
    REG_DMA2DAD = (mm_word)REG_SGFIFOB;

    // Enable DMA [enable, fifo request, 32-bit, repeat]
    REG_DMA1CNT = 0xB6000000;
    REG_DMA2CNT = 0xB6000000;

    // Master sound enable
    REG_SOUNDCNT_X = 0x80;

    // Enable sampling timer
    REG_TM0CNT = mm_timerfreq | (0x80 << 16);
}

void mmMixerEnd(void)
{
    // Silence direct sound channels
    REG_SOUNDCNT_H = 0;

    // Disable VBL routine
    vblank_handler_enabled = false;

    // Disable DMA
    REG_DMA1CNT = 0;
    REG_DMA2CNT = 0;

    // Disable sampling timer
    REG_TM0CNT = 0;
}

#ifdef MM_GBA_MIXER_IN_C

// This mixer is here for debugging purposes. It's much slower than the assembly
// version, and there's some issue that causes noise to be heard during
// playback. However, it's pretty hard to understand what the assembly version
// does, and this is a very minimalistic mixer that shows how the system works.

ARM_CODE IWRAM_CODE
void mmMixerMix(mm_word samples_count)
{
    if (samples_count == 0)
        return;

    // Part 0. Initialization
    // ----------------------

    // Each sample is a mm_hword, and the output is stereo (two channels)
    memset(mm_mixbuffer, 0, samples_count * sizeof(mm_hword) * 2);

    // Begin mixing routine
    // --------------------

    for (mm_word ch = 0; ch < mm_mixch_count; ch++)
    {
        mm_mixer_channel *rchan = &mm_mix_channels[ch];

        if (rchan->src & MIXCH_GBA_SRC_STOPPED)
            continue;

        // Part 1: Calculations
        // --------------------

        // Read frequency

        mm_word rfreq = rchan->freq;
        if (rfreq == 0)
            continue;

        rfreq = (rfreq * mm_ratescale) >> 14;

        // Calculate volume of right and left speakers

        mm_word rvol = rchan->vol; // volume = 0-255
        if (rvol == 0)
            continue;

        // pan = 0-255

        mm_sword rvolL = ((256 - rchan->pan) * rvol) >> 8; // right volume = (vol*pan)
        mm_sword rvolR = (rchan->pan * rvol) >> 8; // left volume = (256-pan) * vol
        // rvolL and rvolR go from 0 to 256

        // Part 2: Mixing
        // --------------

        mm_mas_gba_sample *sample = (mm_mas_gba_sample *)(rchan->src - sizeof(mm_mas_gba_sample));

        // Fetch samples from the waveform at the current playback frequency and
        // add them to mm_mixbuffer

        mm_word rread = rchan->read;
        mm_shword *dest = (mm_shword*)mm_mixbuffer;

        for (mm_word i = 0; i < samples_count; i++)
        {
            mm_shword val = sample->data[rread >> MP_SAMPFRAC];

            // The waveform stored in the soundbank is unsigned, make it signed
            // so that it's easier to operate on it. Also, the GBA expects
            // signed values in the PCM channels, so we need to convert it at
            // some point anyway.
            val -= 128;

            // We need to reduce the volume a bit. mm_mixbuffer only has space
            // for one mm_hword per output sample, so we need to consider that
            // many channels adding to the same sample can cause overflows.
            // However, don't divide it by the max volume (256) yet to improve
            // accuracy. Shift it by 5 now, it will be shifted by 3 later.
            mm_shword left = *dest + ((val * rvolL) >> 5);
            *dest++ = left;

            mm_shword right = *dest + ((val * rvolR) >> 5);
            *dest++ = right;

            rread += rfreq;

            // Check if we've reached the end of the sample
            if (rread >= (sample->length << MP_SAMPFRAC))
            {
                // The sample doesn't loop, stop it
                if (sample->loop_length == 0xFFFFFFFF)
                {
                    rchan->src = MIXCH_GBA_SRC_STOPPED;
                    break;
                }

                rread -= sample->loop_length << MP_SAMPFRAC;
            }
        }

        rchan->read = rread;
    }

    // Part 3. Post-processing
    // -----------------------

    // Copy mm_mixbuffer to mm_wavebuffer at the position pointed by mp_writepos
    // Size of mm_wavebuffer: mm_mixlen * sizeof(mm_word)

    // The first half is the left buffer, the second half is the right buffer
    mm_sbyte *pwriteL = mp_writepos;
    mm_sbyte *pwriteR = pwriteL + mm_mixlen * 2;

    mm_shword *source = (mm_shword*)mm_mixbuffer;

    for (mm_word i = 0; i < samples_count; i++)
    {
        mm_sword sampleL = *source++;
        mm_sword sampleR = *source++;

        // Divide by the rest of the volume
        sampleL >>= 3;
        sampleR >>= 3;

        if (sampleL > 127)
            sampleL = 127;
        if (sampleL < -128)
            sampleL = -128;

        if (sampleR > 127)
            sampleR = 127;
        if (sampleR < -128)
            sampleR = -128;

        *pwriteL++ = sampleL;
        *pwriteR++ = sampleR;
    }

    mp_writepos = pwriteL;
}

#endif // MM_GBA_MIXER_IN_C
