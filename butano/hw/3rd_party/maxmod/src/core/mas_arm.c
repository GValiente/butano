// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2021-2025, Antonio Niño Díaz (antonio_nd@outlook.com)
// Copyright (c) 2023, Lorenzooone (lollo.lollo.rbiz@gmail.com)

#include <stddef.h>

#include <maxmod.h>
#include <mm_mas.h>

#include "channel_types.h"
#include "mas.h"
#include "player_types.h"

#define ARM_CODE   __attribute__((target("arm")))

#ifdef __NDS__
#define IWRAM_CODE
#endif

#ifdef __GBA__
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))
#endif

#define COMPR_FLAG_NOTE     (1 << 0)
#define COMPR_FLAG_INSTR    (1 << 1)
#define COMPR_FLAG_VOLC     (1 << 2)
#define COMPR_FLAG_EFFC     (1 << 3)

#define GLISSANDO_EFFECT            7
#define GLISSANDO_IT_VOLCMD_START   193
#define GLISSANDO_IT_VOLCMD_END     202
#define GLISSANDO_MX_VOLCMD_START   0xF0
#define GLISSANDO_MX_VOLCMD_END     0xFF

#define NOTE_CUT        254
#define NOTE_OFF        255

#define MULT_PERIOD     133808

// Bitmask to select which hardware/software channels are free to use
mm_word mm_ch_mask;

static const mm_byte note_table_oct[] =
{
    0, 0, 0,
    1, 1, 1,
    2, 2, 2,
    3, 3, 3,
    4, 4, 4,
    5, 5, 5,
    6, 6, 6,
    7, 7, 7,
    8, 8, 8,
    9, 9, 9
};

static const mm_byte note_table_mod[] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0
};

// LUT for amiga periods.
static const mm_hword ST3_FREQTABLE[] =
{
    // More accurate scalars
    1712 * 8, 1616 * 8, 1524 * 8, 1440 * 8, 1356 * 8, 1280 * 8,
    1208 * 8, 1140 * 8, 1076 * 8, 1016 * 8, 960 * 8, 907 * 8

    // Middle octave is 4
    //
    //                  133808 * ( period(NOTE) >> octave )
    // note_st3period = -----------------------------------
    //                  middle_c_finetunevalue(INSTRUMENT)
};

// LUT for linear periods. Values are 16.16 bit
// https://ftp.modland.com/pub/documents/format_documentation/Impulse%20Tracker%20v2.04%20(.it).html
__attribute__((aligned (4))) // Align to 4 bytes so that we can read 2 hwords in one read
static const mm_hword IT_PitchTable[] =
{
    2048, 0,   2170, 0,   2299, 0,   2435, 0,   2580, 0,   2734, 0,
    2896, 0,   3069, 0,   3251, 0,   3444, 0,   3649, 0,   3866, 0, // C-0 > B-0

    4096, 0,   4340, 0,   4598, 0,   4871, 0,   5161, 0,   5468, 0,
    5793, 0,   6137, 0,   6502, 0,   6889, 0,   7298, 0,   7732, 0, // C-1 > B-1

    8192, 0,   8679, 0,   9195, 0,   9742, 0,   10321, 0,  10935, 0,
    11585, 0,  12274, 0,  13004, 0,  13777, 0,  14596, 0,  15464, 0, // Octave 2

    16384, 0,  17358, 0,  18390, 0,  19484, 0,  20643, 0,  21870, 0,
    23170, 0,  24548, 0,  26008, 0,  27554, 0,  29193, 0,  30929, 0, // Octave 3

    32768, 0,  34716, 0,  36781, 0,  38968, 0,  41285, 0,  43740, 0,
    46341, 0,  49097, 0,  52016, 0,  55109, 0,  58386, 0,  61858, 0, // Octave 4

    0, 1,      3897, 1,   8026, 1,   12400, 1,  17034, 1,  21944, 1,
    27146, 1,  32657, 1,  38496, 1,  44682, 1,  51236, 1,  58179, 1, // Octave 5

    0, 2,      7794, 2,   16051, 2,  24800, 2,  34068, 2,  43888, 2,
    54292, 2,  65314, 2,  11456, 3,  23828, 3,  36936, 3,  50823, 3, // Octave 6

    0, 4,      15588, 4,  32103, 4,  49600, 4,  2601, 5,   22240, 5,
    43048, 5,  65092, 5,  22912, 6,  47656, 6,  8336, 7,   36110, 7, // Octave 7

    0, 8,      31176, 8,  64205, 8,  33663, 9,  5201, 10,  44481, 10,
    20559, 11, 64648, 11, 45823, 12, 29776, 13, 16671, 14, 6684, 15, // Octave 8

    0, 16,     62352, 16, 62875, 17, 1790,  19, 10403, 20, 23425, 21,
    41118, 22, 63761, 23, 26111, 25, 59552, 26, 33342, 28, 13368, 30, // Octave 9
};

// Finds a channel to use
// Returns invalid channel [NO_CHANNEL_AVAILABLE] if none available
IWRAM_CODE ARM_CODE mm_word mmAllocChannel(void)
{
    // Pointer to active channels
    mm_active_channel *act_ch = &mm_achannels[0];

    mm_word bitmask = mm_ch_mask;
    mm_word best_channel = NO_CHANNEL_AVAILABLE; // 255 = none
    mm_word best_volume = 0x80000000;

    for (mm_word i = 0; bitmask != 0; i++, bitmask >>= 1, act_ch++)
    {
        // If not available, skip
        if ((bitmask & 1) == 0)
            continue;

        mm_word fvol = act_ch->fvol;
        mm_word type = act_ch->type;

        // Compare background/disabled
        if (ACHN_BACKGROUND < type)
            continue; // It's important, don't use this channel
        else if (ACHN_BACKGROUND > type)
            return i; // It's disabled, use this channel

        // Compare volume with our record
        if (best_volume <= (fvol << 23))
            continue;

        // Save this channel and volume level
        best_channel = i;
        best_volume = fvol << 23;
    }

    return best_channel;
}

// It returns 0 on error (if the song tries to use more channels than available
// to Maxmod. On success, it returns 1.
IWRAM_CODE ARM_CODE mm_bool mmReadPattern(mpl_layer_information *mpp_layer)
{
    // Prepare vars
    mm_word instr_count = mpp_layer->songadr->instr_count;
    mm_word flags = mpp_layer->flags;
    mm_module_channel *module_channels = mpp_channels;

    mpp_vars.pattread_p = mpp_layer->pattread;
    mm_byte *pattern = mpp_vars.pattread_p;

    mm_word update_bits = 0;

    while (1)
    {
        mm_byte read_byte = *pattern++;

        // 0 = end of row
        if ((read_byte & 0x7F) == 0)
            break;

        mm_word pattern_flags = 0;

        mm_byte chan_num = (read_byte & 0x7F) - 1;

        // Check that this channel index is inside of the limits of this layer
        if (chan_num >= mpp_nchannels)
            return 0;

        update_bits |= 1 << chan_num;

        mm_module_channel *module_channel = &(module_channels[chan_num]);

        // Read new maskvariable if bit was set and save it, otherwise use the
        // previous flags.
        if (read_byte & (1 << 7))
            module_channel->cflags = *pattern++;

        mm_word compr_flags = module_channel->cflags;

        if (compr_flags & COMPR_FLAG_NOTE)
        {
            mm_byte note = *pattern++;

            if (note == NOTE_CUT)
                pattern_flags |= MF_NOTECUT;
            else if (note == NOTE_OFF)
                pattern_flags |= MF_NOTEOFF;
            else
                module_channel->pnoter = note;
        }

        if (compr_flags & COMPR_FLAG_INSTR)
        {
            // Read instrument value
            mm_byte instr = *pattern++;

            // Act if it's playing
            if ((pattern_flags & (MF_NOTECUT | MF_NOTEOFF)) == 0)
            {
                // Validate instrument. Note that instrument index 0 means "no
                // instrument". If instr_count is 10, the minimum valid
                // instrument is 1 and the maximum is 10.
                if (instr > instr_count)
                    instr = 0;

                if (module_channel->inst != instr)
                {
                    // Check 'mod/s3m' flag
                    if (flags & MAS_HEADER_FLAG_OLD_MODE)
                        pattern_flags |= MF_START;

                    // Set new instrument flag
                    pattern_flags |= MF_NEWINSTR;
                }

                // Update instrument
                module_channel->inst = instr;
            }
        }

        // Copy VCMD
        if (compr_flags & COMPR_FLAG_VOLC)
            module_channel->volcmd = *pattern++;

        // Copy Effect and param
        if (compr_flags & COMPR_FLAG_EFFC)
        {
            module_channel->effect = *pattern++;
            module_channel->param = *pattern++;
        }

        // The bottom 4 bits of the MF_ flags is stored in the top 4 bits of
        // cflags by mmutil.
        module_channel->flags = pattern_flags | (compr_flags >> 4);
    }

    mpp_layer->pattread = pattern;
    mpp_layer->mch_update = update_bits;

    return 1;
}

static IWRAM_CODE ARM_CODE __attribute__((noinline))
mm_byte mmChannelStartACHN(mm_module_channel *module_channel, mm_active_channel *active_channel,
                           mpl_layer_information *mpp_layer, mm_byte channel_counter)
{
    // Clear tremor/cutvol
    module_channel->bflags &= ~(MCH_BFLAGS_CUT_VOLUME | MCH_BFLAGS_TREMOR);

    if (active_channel)
    {
        // Set foreground type
        active_channel->type = ACHN_FOREGROUND;
        // Clear SUB/EFFECT and store layer
        active_channel->flags &= ~(MCAF_SUB | MCAF_EFFECT);
        if (mpp_clayer == MM_JINGLE)
            active_channel->flags |= MCAF_SUB;
        // Store parent
        active_channel->parent = channel_counter;
        // Copy instrument
        active_channel->inst = module_channel->inst;
    }

    // Previously, it did not set the return parameter properly
    if (module_channel->inst == 0)
        return module_channel->bflags; // TODO: This is what the code does, is it a bug?

    // Get instrument pointer
    mm_mas_instrument *instrument = mpp_InstrumentPointer(mpp_layer, module_channel->inst);

    // Check if note_map exists
    // If this is set, it doesn't!
    if (instrument->is_note_map_invalid)
    {
        if (active_channel)
            active_channel->sample = instrument->note_map_offset & 0xFF;

        // write note value (without notemap, all entries == PNOTE)
        module_channel->note = module_channel->pnoter;
    }
    else
    {
        // Read notemap entry
        mm_hword *note_map = (mm_hword*)(((mm_word)instrument) + instrument->note_map_offset);
        mm_hword notemap_entry = note_map[module_channel->pnoter];

        // Write note value
        module_channel->note = notemap_entry & 0xFF;
        // Write sample value
        if (active_channel)
            active_channel->sample = notemap_entry >> 8;
    }

    return module_channel->note;
}

IWRAM_CODE ARM_CODE
mm_word mmGetPeriod(mpl_layer_information *mpp_layer, mm_word tuning, mm_byte note)
{
    // Tuning not used here with linear periods
    if (mpp_layer->flags & MAS_HEADER_FLAG_FREQ_MODE)
        return ((mm_word *)IT_PitchTable)[note]; // Read 2 halfwords at once

    mm_word r0 = note_table_mod[note];      // (note mod 12) << 1
    mm_word r2 = note_table_oct[note >> 2]; // (note / 12)

    // Uses pre-calculated results of /12 and %12
    mm_word ret_val = (ST3_FREQTABLE[r0] * MULT_PERIOD) >> r2;

    if (tuning)
        ret_val /= tuning;

    return ret_val;
}

static IWRAM_CODE ARM_CODE
mm_active_channel *get_active_channel(mm_module_channel *module_channel)
{
    mm_active_channel *act_ch = NULL;

    // Get channel, if available
    if (module_channel->alloc != NO_CHANNEL_AVAILABLE)
        act_ch = &mm_achannels[module_channel->alloc];

    return act_ch;
}

// For tick 0
IWRAM_CODE ARM_CODE void
mmUpdateChannel_T0(mm_module_channel *module_channel, mpl_layer_information *mpp_layer,
                   mm_byte channel_counter)
{
    mm_active_channel *act_ch;

    // Test start flag
    if ((module_channel->flags & MF_START) == 0)
        goto dont_start_channel;

    // Test effect flag
    if (module_channel->flags & MF_HASFX)
    {
        // Test for 'SDx' (note delay). Don't start channel if delayed
        //if (module_channel->effect == 19)
        //{
        //    if ((module_channel->param >> 4) == 0xD)
        //        goto dont_start_Channel;
        //}

        // Always start channel if it's a new instrument
        if (module_channel->flags & MF_NEWINSTR)
            goto start_channel;

        // Check if the effect is glissando
        if (module_channel->effect == GLISSANDO_EFFECT)
            goto glissando_affected;
    }

    // Always start channel if it's a new instrument
    if (module_channel->flags & MF_NEWINSTR)
        goto start_channel;

    // Test for volume commmand
    if ((module_channel->flags & MF_HASVCMD) == 0)
        goto start_channel;

    if (mpp_layer->flags & MAS_HEADER_FLAG_XM_MODE) // XM effects
    {
        // Glissando is 193..202
        if ((module_channel->volcmd < GLISSANDO_IT_VOLCMD_START) ||
            (module_channel->volcmd > GLISSANDO_IT_VOLCMD_END))
            goto start_channel;
    }
    else // IT effects
    {
        // Glissando is Fx
        if ((module_channel->volcmd < GLISSANDO_MX_VOLCMD_START) /* ||
            (module_channel->volcmd > GLISSANDO_MX_VOLCMD_END)*/ )
            goto start_channel;
    }

glissando_affected:

    act_ch = get_active_channel(module_channel);
    if (act_ch)
    {
        mmChannelStartACHN(module_channel, act_ch, mpp_layer, channel_counter);

        module_channel->flags &= ~MF_START;
        goto dont_start_channel;
    }

start_channel:

    mpp_Channel_NewNote(module_channel, mpp_layer);

    act_ch = get_active_channel(module_channel);
    if (act_ch == NULL)
    {
        mmUpdateChannel_TN(module_channel, mpp_layer);
        return;
    }

    mm_word note = mmChannelStartACHN(module_channel, act_ch, mpp_layer, channel_counter);

    if (act_ch->sample)
    {
        mm_mas_sample_info *sample = mpp_SamplePointer(mpp_layer, act_ch->sample);
        module_channel->period = mmGetPeriod(mpp_layer, sample->frequency << 2, note);
        act_ch->flags |= MCAF_START;
    }

    goto channel_started;

dont_start_channel:

    act_ch = get_active_channel(module_channel);
    if (act_ch == NULL)
    {
        mmUpdateChannel_TN(module_channel, mpp_layer);
        return;
    }

channel_started:

    if (module_channel->flags & MF_DVOL)
    {
        if (module_channel->inst)
        {
            // Get instrument pointer
            mm_mas_instrument *instrument = mpp_InstrumentPointer(mpp_layer, module_channel->inst);

            // Clear old nna and set the new one
            module_channel->bflags &= ~MCH_BFLAGS_NNA_MASK;
            module_channel->bflags |= MCH_BFLAGS_NNA_SET(instrument->nna);

            if (instrument->env_flags & MAS_INSTR_FLAG_VOL_ENV_ENABLED)
                act_ch->flags |= MCAF_VOLENV;
            else
                act_ch->flags &= ~MCAF_VOLENV;

            // The MSB determines if we need to set a new panning value
            if (instrument->panning & 0x80)
                module_channel->panning = (instrument->panning & 0x7F) << 1;
        }

        if (act_ch->sample)
        {
            // Get sample pointer
            mm_mas_sample_info *sample = mpp_SamplePointer(mpp_layer, act_ch->sample);
            module_channel->volume = sample->default_volume;

            // The MSB determines if we need to set a new panning value
            if (sample->panning & 0x80)
                module_channel->panning = (sample->panning & 0x7F) << 1;
        }
    }

    if (module_channel->flags & (MF_START | MF_DVOL))
    {
        if (((mpp_layer->flags & MAS_HEADER_FLAG_XM_MODE) == 0) || (module_channel->flags & MF_DVOL))
        {
            // Reset volume
            act_ch->fade = 1024; // Max volume
            act_ch->envc_vol = 0;
            act_ch->envc_pan = 0;
            act_ch->envc_pic = 0;
            act_ch->avib_dep = 0;
            act_ch->avib_pos = 0;
            act_ch->envn_vol = 0;
            act_ch->envn_pan = 0;
            act_ch->envn_pic = 0;

            // Clear fx memory
            module_channel->fxmem = 0;

            // Set keyon and clear envend + fade
            act_ch->flags |= MCAF_KEYON;
            act_ch->flags &= ~(MCAF_ENVEND | MCAF_FADE);
        }
    }

    if (module_channel->flags & MF_NOTEOFF)
    {
        act_ch->flags &= ~MCAF_KEYON;

        mm_bool is_xm_mode = mpp_layer->flags & MAS_HEADER_FLAG_XM_MODE;

        // XM starts fade immediately on note-off
        if (is_xm_mode)
            act_ch->flags |= MCAF_FADE;
    }

    if (module_channel->flags & MF_NOTECUT)
        module_channel->volume = 0;

    module_channel->flags &= ~MF_START;

    // Execute the rest as normal
    mmUpdateChannel_TN(module_channel, mpp_layer);
}

// For ticks that are not the first one. Note that mpp_layer->ticks may be zero
// when this function is called (if a channel is active and the row increases).
void mmUpdateChannel_TN(mm_module_channel *module_channel, mpl_layer_information *mpp_layer)
{
    // Get channel, if available
    mm_active_channel *act_ch = get_active_channel(module_channel);

    // Get period, edited by other functions...
    mm_word period = module_channel->period;

    // Clear variables
    mpp_vars.sampoff = 0;
    mpp_vars.volplus = 0;
    mpp_vars.notedelay = 0;
    mpp_vars.panplus = 0;

    // Update volume commands. Used by S3M, XM and IT. Not used by MOD.
    if (module_channel->flags & MF_HASVCMD)
        period = mpp_Process_VolumeCommand(mpp_layer, act_ch, module_channel, period);

    // Update effects
    if (module_channel->flags & MF_HASFX)
        period = mpp_Process_Effect(mpp_layer, act_ch, module_channel, period);

    if (act_ch == NULL)
        return;

    int volume = (module_channel->volume * module_channel->cvolume) >> 5;
    act_ch->volume = volume;
    mm_sword vol_addition = mpp_vars.volplus;

    volume += vol_addition << 3;

    // Clamp volume
    if (volume < 0)
        volume = 0;
    if (volume > 128)
        volume = 128;

    mpp_vars.afvol = volume;

    if (mpp_vars.notedelay)
    {
        act_ch->flags |= MCAF_UPDATED;
        return;
    }

    // Copy panning and period
    act_ch->panning = module_channel->panning;
    act_ch->period = module_channel->period;
    // Set to 0 temporarily. Reserved for later use.
    mpp_vars.panplus = 0;

    act_ch->flags |= MCAF_UPDATED;

    period = mpp_Update_ACHN_notest(mpp_layer, act_ch,
                                    period, module_channel->alloc);
}
