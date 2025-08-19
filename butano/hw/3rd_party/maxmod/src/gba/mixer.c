// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2021-2025, Antonio Niño Díaz (antonio_nd@outlook.com)

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <maxmod.h>
#include <mm_mas.h>

#include "main_gba.h"
#include "mixer.h"

#define ARM_CODE   __attribute__((target("arm")))
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))

mm_byte mp_mix_seg; // Mixing segment select

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
