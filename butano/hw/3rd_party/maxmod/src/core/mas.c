// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2021-2025, Antonio Niño Díaz (antonio_nd@outlook.com)

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <maxmod.h>
#include <mm_mas.h>
#include <mm_msl.h>

#include "channel_types.h"
#include "mas.h"
#include "player_types.h"

#if defined(__GBA__)
#include "../gba/main_gba.h"
#include "../gba/mixer.h"
#elif defined(__NDS__)
#include "../ds/arm7/main_ds7.h"
#include "../ds/arm7/mixer.h"
#endif

#ifdef __NDS__
#define IWRAM_CODE
#endif

#ifdef __GBA__
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))
#endif

#define S3M_FREQ_DIVIDER        57268224 // (s3m,xm,it)
#define MOD_FREQ_DIVIDER_PAL    56750314 // (mod)
#define MOD_FREQ_DIVIDER_NTSC   57272724 // (---)

static mm_word mppe_DoVibrato(mm_word period, mm_module_channel *channel,
                              mpl_layer_information *layer);

static mm_word mppe_glis_backdoor(mm_word param, mm_word period, mm_active_channel *act_ch,
                                  mm_module_channel *channel, mpl_layer_information *layer);

static void mpp_Update_ACHN(mpl_layer_information *layer, mm_active_channel *act_ch,
                            mm_word period, mm_word ch);

// Channel data/sizes
mm_active_channel *mm_achannels;
mm_module_channel *mm_pchannels;
mm_word mm_num_mch;
mm_word mm_num_ach;

// Module channels used for the jingle. The main module has a number of channels
// defined by mmInit() in GBA and hardcoded to NUM_CHANNELS in NDS. However, the
// jingle is hardcoded to MP_SCHANNELS in both platforms.
mm_module_channel mm_schannels[MP_SCHANNELS];

// Layer data for module playback.
mpl_layer_information mmLayerMain;

// Layer data for jingle playback.
mpl_layer_information mmLayerSub;

// Holds intermediate data during the module processing.
mpv_active_information mpp_vars;

// Pointer to layer data during processing.
mpl_layer_information *mpp_layerp;

// Pointer to channel array during processing.
mm_module_channel *mpp_channels;

// Master tempo scaler.
static mm_word mm_mastertempo;

// Master pitch scaler.
mm_word mm_masterpitch;

// Number of channels allocated for current layer being processed
mm_byte mpp_nchannels;

// Current layer being processed: MM_MAIN or MM_JINGLE
mm_layer_type mpp_clayer;

#if defined(__NDS__)
// Speed divider for DS timing.
static mm_word mpp_resolution;
#endif

// Function pointer to user event handler
static mm_callback mmCallback;

// Set function for handling playback events
void mmSetEventHandler(mm_callback handler)
{
    mmCallback = handler;
}

// Set BPM. bpm = 32..255
// Input r5 = layer, r0 = bpm
static void mpp_setbpm(mpl_layer_information *layer_info, mm_word bpm)
{
    layer_info->bpm = bpm;

#if defined(__GBA__)

    if (mpp_clayer == MM_MAIN)
    {
        // Multiply by master tempo
        mm_word tempo = (mm_mastertempo * bpm) >> 10;

        // Samples per tick ~= mixfreq / (bpm / 2.5) ~= mixfreq * 2.5 / bpm
        mm_word rate = mm_bpmdv / tempo;

        // Make it a multiple of two
        rate &= ~1;

        layer_info->tickrate = rate;
    }
    else
    {
        // SUB LAYER, time using vsync (rate = (bpm / 2.5) / 59.7)

        layer_info->tickrate = (bpm << 15) / 149;
    }

#elif defined(__NDS__)

    // vsync = ~59.8261 HZ (says GBATEK)
    // divider = hz * 2.5 * 64

    if (mpp_clayer == MM_MAIN)
    {
        // Multiply by master tempo
        bpm = bpm * mm_mastertempo;
        bpm <<= 16 + 6 - 10;
    }
    else
    {
        bpm <<= 16 + 6;
    }

    // using 60hz vsync for timing
    // Should this be better approximated?!
    layer_info->tickrate = (bpm / mpp_resolution) >> 1;

#endif
}

// Suspend main module and associated channels.
static void mpp_suspend(mm_layer_type layer)
{
    mm_active_channel *act_ch = &mm_achannels[0];
    mm_mixer_channel *mix_ch = &mm_mix_channels[0];

    for (mm_word count = mm_num_ach; count != 0; count--, act_ch++, mix_ch++)
    {
        // Make sure that this channel is used by the requested layer. Also,
        // check that this isn't a sound effect (MCAF_EFFECT isn't set).
        if ((act_ch->flags & (MCAF_SUB | MCAF_EFFECT)) != (layer << 6))
            continue;

#ifdef __GBA__
        mix_ch->freq = 0;
#else
        mix_ch->freq = 0;
        mix_ch->vol = 0;
#endif
    }
}

// Pause module playback.
void mmPause(void)
{
    if (mmLayerMain.valid == 0)
        return;

    mmLayerMain.isplaying = 0;

    mpp_suspend(MM_MAIN);
}

// Resume module playback.
void mmResume(void)
{
    if (mmLayerMain.valid == 0)
        return;

    mmLayerMain.isplaying = 1;
}

void mmJinglePause(void)
{
    if (mmLayerSub.valid == 0)
        return;

    mmLayerSub.isplaying = 0;

    mpp_suspend(MM_JINGLE);
}

void mmJingleResume(void)
{
    if (mmLayerSub.valid == 0)
        return;

    mmLayerSub.isplaying = 1;
}

// Returns true if module is playing.
mm_bool mmActive(void)
{
    return mmLayerMain.isplaying;
}

// Returns true if a jingle is playing.
mm_bool mmJingleActive(void)
{
    return mmLayerSub.isplaying;
}

// Set master module volume.
//
// volume : 0->1024
void mmSetModuleVolume(mm_word volume)
{
    // Clamp volume 0->1024
    if (volume > 1024)
        volume = 1024;

    mmLayerMain.volume = volume;
}

// Set master jingle volume.
//
// volume : 0->1024
void mmSetJingleVolume(mm_word volume)
{
    // Clamp volume 0->1024
    if (volume > 1024)
        volume = 1024;

    mmLayerSub.volume = volume; // mpp_layerB
}

static void mpps_backdoor(mm_word id, mm_pmode mode, mm_layer_type layer)
{
#if defined(__GBA__)
    // In the MSL format, the module table goes right after the sample table,
    // but the size of both isn't fixed. We need to calculate the start of the
    // module table by checking how big the sample table is.
    mm_hword sampleCount = mp_solution->head_data.sampleCount;
    mm_word *moduleTable = (mm_word *)&(mp_solution->sampleTable[sampleCount]);

    // Calculate the address of the module now that we have the module table. It
    // represents offsets inside the soundbank.
    uintptr_t moduleAddress = (uintptr_t)mp_solution + sizeof(mm_mas_prefix) + moduleTable[id];

    // Play this module
    mmPlayModule(moduleAddress, mode, layer);
#elif defined(__NDS__)
    mm_word address = (mm_word)mmModuleBank[id];
    if (address == 0)
        return;

    // This address is a pointer to a MAS file that contains the module data.
    // Pass it to mmPlayModule skipping the file prefix.
    mmPlayModule(address + sizeof(mm_mas_prefix), mode, layer);
#endif
}

void mmStart(mm_word id, mm_pmode mode)
{
    if (id >= mmGetModuleCount())
        return;

    mpps_backdoor(id, mode, MM_MAIN);
}

void mmJingleStart(mm_word module_ID, mm_pmode mode)
{
    if (module_ID >= mmGetModuleCount())
        return;

    mpps_backdoor(module_ID, mode, MM_JINGLE);
}

// Reset channel data, and any active channels linked to the layer.
static void mpp_resetchannels(mm_module_channel *channels,
                              mm_word num_ch)
{
    // Clear channel data to 0
    memset(channels, 0, sizeof(mm_module_channel) * num_ch);

    // Reset channel indexes
    for (mm_word i = 0; i < num_ch; i++)
        channels[i].alloc = NO_CHANNEL_AVAILABLE;

    // Reset active channels linked to this layer.

    mm_mixer_channel *mix_ch = &mm_mix_channels[0];
    mm_active_channel *act_ch = &mm_achannels[0];

    for (mm_word i = 0; i < mm_num_ach; i++, act_ch++, mix_ch++)
    {
        // Test if layer matches and if this channel isn't being used for a
        // sound effect.
        if (((act_ch->flags & (MCAF_SUB | MCAF_EFFECT)) >> 6) != mpp_clayer)
            continue;

        // Clear achannel data to zero
        memset(act_ch, 0, sizeof(mm_active_channel));

        // Disabled mixer channel. Disabled status differs between systems.
#ifdef __NDS__
        mix_ch->key_on = 0;
        mix_ch->samp = 0;
        // Setting the panning isn't really needed, but it helps the compiler
        // optimize all 3 accesses into one single 32-bit write.
        mix_ch->tpan = 0;
#endif
#ifdef __GBA__
        mix_ch->src = MIXCH_GBA_SRC_STOPPED;
#endif
    }
}

// Stop module playback.
static void mppStop(void)
{
    mpl_layer_information *layer_info;
    mm_module_channel *channels;
    mm_word num_ch;

    if (mpp_clayer == MM_JINGLE)
    {
        layer_info = &mmLayerSub;
        channels = mm_schannels;
        num_ch = MP_SCHANNELS;
    }
    else
    {
        layer_info = &mmLayerMain;
        channels = mm_pchannels;
        num_ch = mm_num_mch;
    }

    layer_info->isplaying = 0;
    layer_info->valid = 0;

    mpp_resetchannels(channels, num_ch);
}

void mmStop(void)
{
    mpp_clayer = MM_MAIN;
    mppStop();
}

void mmJingleStop(void)
{
    mpp_clayer = MM_JINGLE;
    mppStop();
}

// Set sequence position.
static void mpp_setposition(mpl_layer_information *layer_info, mm_word position)
{
    mm_mas_head *header = layer_info->songadr;

    mm_byte entry;

    while (1)
    {
        layer_info->position = position;

        // Get sequence entry
        entry = header->sequence[position];

        // Value 254 is written by mmutil when an invalid pattern order is found
        // (when it's outside of bounds). Skip pattern orders with 254.
        if (entry == 254)
        {
            position++;
            continue;
        }

        // The last pattern order of the song is marked as 255. If we haven't
        // finished the last pattern, keep playing the song.
        if (entry != 255)
            break;

        // We have reached the end of the last pattern. If the song is looping,
        // go to the loop starting point. If not, stop the playback.
        if (layer_info->mode == MM_PLAY_ONCE)
        {
            // It's playing once
            mppStop();

            if (mmCallback != NULL)
                mmCallback(MMCB_SONGFINISHED, mpp_clayer);

            return;
        }
        else // if (layer_info->mode == MM_PLAY_LOOP)
        {
            // If looping, set position to the repeat position
            position = header->repeat_position;
        }
    }

    // Calculate pattern address
    mm_mas_pattern *patt = mpp_PatternPointer(layer_info, entry);

    // Save pattern size
    layer_info->nrows = patt->row_count;

    // Reset tick/row
    layer_info->tick = 0;
    layer_info->row = 0;
    layer_info->fpattdelay = 0;
    layer_info->pattdelay = 0;

    // Store pattern data address
    layer_info->pattread = patt->pattern_data;

    // Reset pattern loop
    layer_info->ploop_adr = patt->pattern_data;
    layer_info->ploop_row = 0;
    layer_info->ploop_times = 0;
}

// Get current number of elapsed ticks in the row being played.
mm_word mmGetPositionTick(void)
{
    return mmLayerMain.tick;
}

// Get current row being played.
mm_word mmGetPositionRow(void)
{
    return mmLayerMain.row;
}

// Get playback position
mm_word mmGetPosition(void)
{
    return mmLayerMain.position;
}

// Set master tempo
//
// tempo : x.10 fixed point tempo, 0.5->2.0
void mmSetModuleTempo(mm_word tempo)
{
    // Clamp value: 512->2048

    mm_word max = 2048;
    if (tempo > max)
        tempo = max;

    mm_word min = 512;
    if (tempo < min)
        tempo = min;

    mm_mastertempo = tempo;
    mpp_clayer = MM_MAIN;

    if (mmLayerMain.bpm != 0)
       mpp_setbpm(&mmLayerMain, mmLayerMain.bpm);
}

// Reset pattern variables
// Input r5 = layer
static void mpp_resetvars(mpl_layer_information *layer_info)
{
    layer_info->pattjump = 255;
    layer_info->pattjump_row = 0;
}

// Start playing module.
void mmPlayModule(uintptr_t address, mm_word mode, mm_word layer)
{
    mm_mas_head *header = (mm_mas_head *)address;

    mpp_clayer = layer;

    mpl_layer_information *layer_info;
    mm_module_channel *channels;
    mm_word num_ch;

    if (layer == MM_MAIN)
    {
        layer_info = &mmLayerMain;
        channels = mm_pchannels;
        num_ch = mm_num_mch;
    }
    else
    {
        layer_info = &mmLayerSub;
        channels = mm_schannels;
        num_ch = MP_SCHANNELS;
    }

    layer_info->mode = mode;

    layer_info->songadr = header;

    mpp_resetchannels(channels, num_ch);

    mm_word instn_size = header->instr_count;
    mm_word sampn_size = header->sampl_count;

    // Setup instrument, sample and pattern tables
    layer_info->insttable = (mm_word *)&header->tables[0];
    layer_info->samptable = (mm_word *)&header->tables[instn_size];
    layer_info->patttable = (mm_word *)&header->tables[instn_size + sampn_size];

    // Set pattern to 0
    mpp_setposition(layer_info, 0);

    // Load initial tempo
    mpp_setbpm(layer_info, header->initial_tempo);

    // Load initial global volume
    layer_info->global_volume = header->global_volume;

    // Load song flags
    mm_word flags = header->flags;
    layer_info->flags = flags;
    layer_info->oldeffects = (flags >> 1) & 1;

    // Load speed
    layer_info->speed = header->initial_speed;

    // mpp_playing = true, set valid flag
    layer_info->isplaying = 1;
    layer_info->valid = 1;

    mpp_resetvars(layer_info);

    // Setup channel volumes
    for (mm_word i = 0; i < num_ch; i++)
        channels[i].cvolume = header->channel_volume[i];

    // Setup channel pannings
    for (mm_word i = 0; i < num_ch; i++)
        channels[i].panning = header->channel_panning[i];
}

// Set master pitch
//
// pitch : x.10 fixed point value, range = 0.5->2.0
void mmSetModulePitch(mm_word pitch)
{
    // Clamp value: 512->2048

    mm_word max = 2048;
    if (pitch > max)
        pitch = max;

    mm_word min = 512;
    if (pitch < min)
        pitch = min;

    mm_masterpitch = pitch;
}

#ifdef __NDS__

// Set update resolution
void mmSetResolution(mm_word divider)
{
    mpp_resolution = divider;

    mpp_clayer = MM_MAIN;
    if (mmLayerMain.bpm != 0)
       mpp_setbpm(&mmLayerMain, mmLayerMain.bpm);

    mpp_clayer = MM_JINGLE;
    if (mmLayerSub.bpm != 0)
       mpp_setbpm(&mmLayerSub, mmLayerSub.bpm);
}

#endif

#ifdef __GBA__

// Update sub-module/jingle, this is bad for some reason...
void mppUpdateSub(void)
{
    if (mmLayerSub.isplaying == 0)
        return;

    mpp_channels = mm_schannels;
    mpp_nchannels = MP_SCHANNELS;
    mpp_clayer = MM_JINGLE;
    mpp_layerp = &mmLayerSub;

    mm_word tickrate = mmLayerSub.tickrate;
    mm_word tickfrac = mmLayerSub.tickfrac;

    tickfrac = tickfrac + (tickrate << 1);
    mmLayerSub.tickfrac = tickfrac;

    tickfrac >>= 16;

    while (tickfrac > 0)
    {
        mppProcessTick();
        tickfrac--;
    }
}

#endif

#ifdef __NDS__

// Update module layer
static void mppUpdateLayer(mpl_layer_information *layer)
{
    mpp_layerp = layer;
    mm_word new_tick = (layer->tickrate * 2) + layer->tickfrac;
    layer->tickfrac = new_tick;

    for (mm_word i = 0; i < (new_tick >> 16); i++)
        mppProcessTick();
}

// NDS Work Routine
void mmPulse(void)
{
    // Update main layer
    mpp_channels = mm_pchannels;
    mpp_nchannels = mm_num_mch;
    mpp_clayer = MM_MAIN;
    mppUpdateLayer(&mmLayerMain);

    // Update sub layer
    mpp_channels = mm_schannels;
    mpp_nchannels = MP_SCHANNELS;
    mpp_clayer = MM_JINGLE;
    mppUpdateLayer(&mmLayerSub);
}

#endif

// It gets the active channel from a module channel. It returns NULL if there
// isn't an active channel.
static mm_active_channel *mpp_Channel_GetACHN(mm_module_channel *channel)
{
    mm_word alloc = channel->alloc;
    if (alloc == NO_CHANNEL_AVAILABLE)
        return NULL;

    return &mm_achannels[alloc];
}

// Process new note
IWRAM_CODE void mpp_Channel_NewNote(mm_module_channel *module_channel, mpl_layer_information *layer)
{
    if (module_channel->inst == 0)
        return;

    mm_active_channel *act_ch = mpp_Channel_GetACHN(module_channel);
    if (act_ch == NULL)
        goto mppt_alloc_channel;

    mm_mas_instrument *instrument = mpp_InstrumentPointer(layer, module_channel->inst);

    if ((MCH_BFLAGS_NNA_GET(module_channel->bflags)) == IT_NNA_CUT)
        goto mppt_NNA_CUT; // Skip if zero

    bool do_dca = false;

    mm_byte dct = instrument->dct & 3; // Otherwise check duplicate check type

    if (dct == 0)
    {
        // Don't do DCA
    }
    else if (dct == 1) // DCT Note
    {
        // Get pattern note and translate to real note with note/sample map
        mm_hword *note_map = (mm_hword*)(((mm_word)instrument) + instrument->note_map_offset);
        mm_byte note = note_map[module_channel->note - 1] & 0xFF;

        // Compare it with the last note
        if (note == module_channel->note)
            do_dca = true; // Equal? perform DCA. Otherwise, skip.
    }
    else if (dct == 2) // DCT Sample
    {
        // Get pattern note and translate to real sample with note/sample map
        mm_hword *note_map = (mm_hword*)(((mm_word)instrument) + instrument->note_map_offset);
        mm_byte sample = note_map[module_channel->note - 1] >> 8;

        // Compare it with achn's sample
        if (sample == act_ch->sample)
            do_dca = true; // Equal? perform DCA. Otherwise, skip.
    }
    else if (dct == 3) // DCT Instrument
    {
        // Load instrument and compare it with the active one
        if (module_channel->inst == act_ch->inst)
            do_dca = true; // Equal? perform DCA. Otherwise, skip.
    }

    if (do_dca) // Duplicate Check Action
    {
        mm_byte dca = instrument->dca; // Read type

        if (dca == IT_DCA_CUT) // Cut?
            goto mppt_NNA_CUT;

        if (dca == IT_DCA_OFF) // Note-off?
            goto mppt_NNA_OFF;

        // Otherwise, note-fade
        goto mppt_NNA_FADE;
    }
    else
    {
        mm_hword nna = MCH_BFLAGS_NNA_GET(module_channel->bflags);

        if (nna == IT_NNA_CUT)
            goto mppt_NNA_CUT;
        else if (nna == IT_NNA_CONT)
            goto mppt_NNA_CONTINUE;
        else if (nna == IT_NNA_OFF)
            goto mppt_NNA_OFF;
        else if (nna == IT_NNA_FADE)
            goto mppt_NNA_FADE;
    }

mppt_NNA_CUT:

#ifdef __NDS__ // NDS supports volume ramping
    if (act_ch->type == 0)
        return; // Use the same channel

    act_ch->type = ACHN_BACKGROUND;
    act_ch->volume = 0;

    goto mppt_NNA_FINISHED;
#else
    return; // Use the same channel
#endif

mppt_NNA_CONTINUE:
    // Use a different channel and set the active channel to "background"
    act_ch->type = ACHN_BACKGROUND;
    goto mppt_NNA_FINISHED;

mppt_NNA_OFF:
    act_ch->flags &= ~MCAF_KEYON;
    act_ch->type = ACHN_BACKGROUND; // Set the active channel to "background"
    goto mppt_NNA_FINISHED;

mppt_NNA_FADE:
    act_ch->flags |= MCAF_FADE;
    act_ch->type = ACHN_BACKGROUND; // Set the active channel to "background"
    goto mppt_NNA_FINISHED;

mppt_NNA_FINISHED:

mppt_alloc_channel:

    mm_word alloc = mmAllocChannel(); // Find new active channel
    module_channel->alloc = alloc; // Save it

#ifdef __NDS__
    if (alloc == NO_CHANNEL_AVAILABLE)
        return;

    if (act_ch == 0) // Same channel
        return;

    // Copy data from previous channel (for volume ramping wierdness)
    memcpy(&(mm_achannels[alloc]), act_ch, sizeof(mm_active_channel));
#endif
}

// value = 2^(val/192), 16.16 fixed
static const mm_hword mpp_TABLE_LinearSlideUpTable[] = {
    0,     237,   475,   714,   953,   // 0->4 // ADD 1.0
    1194,  1435,  1677,  1920,  2164,  // 5->9
    2409,  2655,  2902,  3149,  3397,  // 10->14
    3647,  3897,  4148,  4400,  4653,  // 15->19
    4907,  5157,  5417,  5674,  5932,  // 20->24
    6190,  6449,  6710,  6971,  7233,  // 25->29
    7496,  7761,  8026,  8292,  8559,  // 30->34
    8027,  9096,  9366,  9636,  9908,  // 35->39
    10181, 10455, 10730, 11006, 11283, // 40->44
    11560, 11839, 12119, 12400, 12682, // 45->49
    12965, 13249, 13533, 13819, 14106, // 50->54
    14394, 14684, 14974, 15265, 15557, // 55->59
    15850, 16145, 16440, 16737, 17034, // 60->64
    17333, 17633, 17933, 18235, 18538, // 65->69
    18842, 19147, 19454, 19761, 20070, // 70->74
    20379, 20690, 21002, 21315, 21629, // 75->79
    21944, 22260, 22578, 22897, 23216, // 80->84
    23537, 23860, 24183, 24507, 24833, // 85->89
    25160, 25488, 25817, 26148, 26479, // 90->94
    26812, 27146, 27481, 27818, 28155, // 95->99
    28494, 28834, 29175, 29518, 29862, // 100->104
    30207, 30553, 30900, 31248, 31599, // 105->109
    31951, 32303, 32657, 33012, 33369, // 110->114
    33726, 34085, 34446, 34807, 35170, // 115->119
    35534, 35900, 36267, 36635, 37004, // 120->124
    37375, 37747, 38121, 38496, 38872, // 125->129
    39250, 39629, 40009, 40391, 40774, // 130->134
    41158, 41544, 41932, 42320, 42710, // 135->139
    43102, 43495, 43889, 44285, 44682, // 140->144
    45081, 45481, 45882, 46285, 46690, // 145->149
    47095, 47503, 47917, 48322, 48734, // 150->154
    49147, 49562, 49978, 50396, 50815, // 155->159
    51236, 51658, 52082, 52507, 52934, // 160->164
    53363, 53793, 54224, 54658, 55092, // 165->169
    55529, 55966, 56406, 56847, 57289, // 170->174
    57734, 58179, 58627, 59076, 59527, // 175->179
    59979, 60433, 60889, 61346, 61805, // 180->184
    62265, 62727, 63191, 63657, 64124, // 185->189
    64593, 65064, 0,     474,   950,   // 190->194 // ADD 2.0 w/ 192+
    1427,  1906,  2387,  2870,  3355,  // 195->199
    3841,  4327,  4818,  5310,  5803,  // 200->204
    6298,  6795,  7294,  7794,  8296,  // 205->209
    8800,  9306,  9814,  10323, 10835, // 210->214
    11348, 11863, 12380, 12899, 13419, // 215->219
    13942, 14467, 14993, 15521, 16051, // 220->224
    16583, 17117, 17653, 18191, 18731, // 225->229
    19273, 19817, 20362, 20910, 21460, // 230->234
    22011, 22565, 23121, 23678, 24238, // 235->239
    24800, 25363, 25929, 25497, 27067, // 240->244
    27639, 28213, 28789, 29367, 29947, // 245->249
    30530, 31114, 31701, 32289, 32880, // 250->254
    33473, 34068                       // 255->256
};

// value = 2^(-val/192), 16.16 fixed
static const mm_hword mpp_TABLE_LinearSlideDownTable[] = {
    65535, 65300, 65065, 64830, 64596, 64364, 64132, 63901, // 0->7
    63670, 63441, 63212, 62984, 62757, 62531, 62306, 62081, // 8->15
    61858, 61635, 61413, 61191, 60971, 60751, 60532, 60314, // 16->23
    60097, 59880, 59664, 59449, 59235, 59022, 58809, 58597, // 24->31
    58386, 58176, 57966, 57757, 57549, 57341, 57135, 56929, // 32->39
    56724, 56519, 56316, 56113, 55911, 55709, 55508, 55308, // 40->47
    55109, 54910, 54713, 54515, 54319, 54123, 53928, 53734, // 48->55
    53540, 53347, 53155, 52963, 52773, 52582, 52393, 52204, // 56->63
    52016, 51829, 51642, 51456, 51270, 51085, 50901, 50718, // 64->71
    50535, 50353, 50172, 49991, 49811, 49631, 49452, 49274, // 72->79
    49097, 48920, 48743, 48568, 48393, 48128, 48044, 47871, // 80->87
    47699, 47527, 47356, 47185, 47015, 46846, 46677, 46509, // 88->95
    46341, 46174, 46008, 45842, 45677, 45512, 45348, 45185, // 96->103
    45022, 44859, 44698, 44537, 44376, 44216, 44057, 43898, // 104->111
    43740, 43582, 43425, 43269, 43113, 42958, 42803, 42649, // 112->119
    42495, 42342, 42189, 42037, 41886, 41735, 41584, 41434, // 120->127
    41285, 41136, 40988, 40840, 40639, 40566, 40400, 40253, // 128->135
    40110, 39965, 39821, 39678, 39535, 39392, 39250, 39109, // 136->143
    38968, 38828, 38688, 38548, 38409, 38271, 38133, 37996, // 144->151
    37859, 37722, 37586, 37451, 37316, 37181, 37047, 36914, // 152->159
    36781, 36648, 36516, 36385, 36254, 36123, 35993, 35863, // 160->167
    35734, 35605, 35477, 35349, 35221, 35095, 34968, 34842, // 168->175
    34716, 34591, 34467, 34343, 34219, 34095, 33973, 33850, // 176->183
    33728, 33607, 33486, 33365, 33245, 33125, 33005, 32887, // 184->191
    32768, 32650, 32532, 32415, 32298, 32182, 32066, 31950, // 192->199
    31835, 31720, 31606, 31492, 31379, 31266, 31153, 31041, // 200->207
    30929, 30817, 30706, 30596, 30485, 30376, 30226, 30157, // 208->215
    30048, 29940, 29832, 29725, 29618, 29511, 29405, 29299, // 216->223
    29193, 29088, 28983, 28879, 28774, 28671, 28567, 28464, // 224->231
    28362, 28260, 28158, 28056, 27955, 27855, 27754, 27654, // 232->239
    27554, 27455, 27356, 27258, 27159, 27062, 26964, 26867, // 240->247
    26770, 26674, 26577, 26482, 26386, 26291, 26196, 26102, // 248->255
    26008                                                   // 256
};

static const mm_hword mpp_TABLE_FineLinearSlideUpTable[] = {
    0,   59,  118, 178, 237, // 0->4   ADD 1x
    296, 356, 415, 475, 535, // 5->9
    594, 654, 714, 773, 833, // 10->14
    893                      // 15
};

static const mm_hword mpp_TABLE_FineLinearSlideDownTable[] = {
    65535, 65477, 65418, 65359, 65300, 65241, 65182, 65359, // 0->7
    65065, 65006, 64947, 64888, 64830, 64772, 64713, 64645  // 8->15
};

static mm_word mpph_psu(mm_word period, mm_word slide_value)
{
    if (slide_value >= 192)
        period *= 2;

    // mpph_psu_fine

    mm_word val = (period >> 5) * mpp_TABLE_LinearSlideUpTable[slide_value];

    mm_word ret = (val >> (16 - 5)) + period;

    // mpph_psu_clip

    if ((ret >> (16 + 5)) > 0) // Clip to 0.0 ~ 1.0
        return 1 << (16 + 5);

    return ret;
}

static mm_word mpph_psd(mm_word period, mm_word slide_value)
{
    mm_word val = (period >> 5) * mpp_TABLE_LinearSlideDownTable[slide_value];

    mm_word ret = val >> (16 - 5);

    // mpph_psd_clip

    //if (ret < 0)
    //    ret = 0;

    return ret;
}

// Linear/Amiga slide up
// The slide value is provided divided by 4
static
mm_word mpph_PitchSlide_Up(mm_word period, mm_word slide_value, mpl_layer_information *layer)
{
    if (layer->flags & MAS_HEADER_FLAG_FREQ_MODE)
    {
        return mpph_psu(period, slide_value);
    }
    else // mpph_psu_amiga
    {
        mm_word delta = slide_value << 4;

        // mpph_psu_amiga_fine

        if (delta > period)
            return 0;

        return period - delta;
    }
}

// Linear slide up
static
mm_word mpph_LinearPitchSlide_Up(mm_word period, mm_word slide_value, mpl_layer_information *layer)
{
    if (layer->flags & MAS_HEADER_FLAG_FREQ_MODE)
        return mpph_psu(period, slide_value);
    else
        return mpph_psd(period, slide_value);
}

// Slide value in range of (0 - 15)
static
mm_word mpph_FinePitchSlide_Up(mm_word period, mm_word slide_value, mpl_layer_information *layer)
{
    if (layer->flags & MAS_HEADER_FLAG_FREQ_MODE) // mpph_psu_fine
    {
        // mpph_psu_fine

        mm_word val = (period >> 5) * mpp_TABLE_FineLinearSlideUpTable[slide_value];

        mm_word ret = (val >> (16 - 5)) + period;

        // mpph_psu_clip

        if ((ret >> (16 + 5)) > 0) // Clip to 0.0 ~ 1.0
            return 1 << (16 + 5);

        return ret;
    }
    else
    {
        mm_word delta = slide_value << 2;

        // mpph_psu_amiga_fine

        if (delta > period)
            return 0;

        return period - delta;
    }
}

static
mm_word mpph_PitchSlide_Down(mm_word period, mm_word slide_value, mpl_layer_information *layer)
{
    if (layer->flags & MAS_HEADER_FLAG_FREQ_MODE)
    {
        return mpph_psd(period, slide_value);
    }
    else // mpph_psd_amiga
    {
        mm_word delta = slide_value << 4;

        // mpph_psd_amiga_fine

        period = (period + delta);

        if ((period >> (16 + 5)) > 0) // Clip to 0.0 ~ 1.0
            return 1 << (16 + 5);

        return period;
    }
}

static
mm_word mpph_LinearPitchSlide_Down(mm_word period, mm_word slide_value,
                                   mpl_layer_information *layer)
{
    if (layer->flags & MAS_HEADER_FLAG_FREQ_MODE)
        return mpph_psd(period, slide_value);
    else
        return mpph_psu(period, slide_value);
}

// Slide value in range of (0 - 15)
static
mm_word mpph_FinePitchSlide_Down(mm_word period, mm_word slide_value,
                                 mpl_layer_information *layer)
{
    if (layer->flags & MAS_HEADER_FLAG_FREQ_MODE) // mpph_psd_fine
    {
        // mpph_psd_fine

        mm_word val = (period >> 5) * mpp_TABLE_FineLinearSlideDownTable[slide_value];

        mm_word ret = val >> (16 - 5);

        // mpph_psd_clip

        //if (ret < 0)
        //    ret = 0;

        return ret;
    }
    else
    {
        mm_word delta = slide_value << 2;

        // mpph_psd_amiga_fine

        period = (period + delta);

        if ((period >> (16 + 5)) > 0) // Clip to 0.0 ~ 1.0
            return 1 << (16 + 5);

        return period;
    }
}

// =============================================================================
//                                EFFECT MEMORY
// =============================================================================

// There are 15 entries in the channel memory

// XM effects memory
// -----------------

// 0 is a common memory entry
#define MPP_XM_VOLSLIDE         1 // FX: [A] Volume slide
#define MPP_XM_PORTA_DOWN       2 // FX: [2, E, X] Portamento down
#define MPP_XM_PORTA_UP         3 // FX: [1, E, X] Portamento up
#define MPP_XM_UNKNOWN          4 // FX: [I] Not available in XM // TODO: Why does it have a value?
#define MPP_XM_VOLSLIDE_VIBR    5 // FX: [6] Volume slide + Vibrato
#define MPP_XM_VOLSLIDE_GLIS    6 // FX: [5] Volume slide + Glissando/Porta to note
#define MPP_XM_SAMP_OFFSET      7 // FX: [9] Set offset
#define MPP_XM_PANSLIDE         8 // FX: [P] Panning slide
#define MPP_XM_RETRIG_NOTE      9 // FX: [R] Retrigger note
#define MPP_XM_TREMOLO          10 // FX: [7] Tremolo
#define MPP_XM_GL_VOLSLIDE      11 // FX: [H] Global volume slide
#define MPP_XM_TREMOR           12 // FX: [T] Tremor

// The entries used for VCMD overlap the ones used for regular effects
#define MPP_XM_VCMD_MEM_PANSL   7   // VCMD: Panning slide  || Value = 0xLR : Left, Right
#define MPP_XM_VCMD_MEM_VS      12  // VCMD: Volume slide   || Value = 0xUD : Up, Down
#define MPP_XM_VCMD_MEM_FVS     13  // VCMD: Fine vol slide || Value = 0xUD : Up, Down
#define MPP_XM_VCMD_MEM_GLIS    14  // VCMD: Glissando      || Value = 0x0X : Zero, Value

// IT effects memory
// -----------------

// Note: MPP_IT_VOLSLIDE and MPP_IT_VCMD_MEM are different memory entries
// because the VCMD volume slide commands don't share memory with the regular
// volume slide effects (Dxx).

// 0 is a common memory entry
#define MPP_IT_VOLSLIDE         1 // FX: [D] Volume slide

#define MPP_IT_PORTA            2 // VCMD: Porta up/down. Porta to note/glissando,
                                  // FX: [E, F] Porta up/down. [G] Porta to note/glissando.

#define MPP_IT_TREMOR           3 // FX: [I] Tremor
#define MPP_IT_ARPEGGIO         4 // FX: [J] Arpeggio
#define MPP_IT_CH_VOLSLIDE      5 // FX: [N] Channel volume slide
#define MPP_IT_SAMP_OFFSET      6 // FX: [O] Set sample offset
#define MPP_IT_PANSLIDE         7 // FX: [P] Pan slide
#define MPP_IT_RETRIG_NOTE      8 // FX: [Q] Retriggers a note
#define MPP_IT_TREMOLO          9 // FX: [R] Tremolo
#define MPP_IT_EXTENDED_FX      10 // FX: [S] Extended effects
#define MPP_IT_TEMPO            11 // FX: [T] Tempo
#define MPP_IT_GL_VOLSLIDE      12 // FX: [W] Global volume slide
#define MPP_IT_PANBRELLO        13 // FX: [Y] Panbrello
#define MPP_IT_VCMD_MEM         14 // VCMD: Fine volume slide. Volume slide

// Common effect memory
// --------------------

// The command below is handled from mpp_Process_VolumeCommand() so it needs
// to be shared between all module types. It isn't handled as part of
// mpp_Channel_ExchangeMemory(), which can decide different memory layouts
// depending on the format of the module.

// IT: VCMD [193-202]:   Porta to note/glissando
// IT: FX:  [L]:         Porta to note/glissando + Volume slide
// XM: VCMD [0xF0-0xFF]: Porta to note/glissando
// XM: FX:  [5]:         Porta to note/glissando + Volume slide
// Not used by mpp_Channel_ExchangeMemory(). Used by mpp_Process_VolumeCommand(),
// mppe_PortaVolume() and mppe_Glissando().
#define MPP_XM_IT_GLIS          0

mm_word mpp_Process_VolumeCommand(mpl_layer_information *layer,
                                  mm_active_channel *act_ch,
                                  mm_module_channel *channel, mm_word period)
{
    mm_byte tick = layer->tick;

    mm_mas_head *header = layer->songadr;

    mm_byte volcmd = channel->volcmd;

    if (header->flags & MAS_HEADER_FLAG_XM_MODE) // XM commands
    {
        if (volcmd == 0) // 0 = none
        {
            // Do nothing
        }
        else if (volcmd <= 0x50) // Set volume : mppuv_xm_setvol
        {
            if (tick == 0)
                channel->volume = volcmd - 0x10;
        }
        else if (volcmd < 0x80) // Volume slide : mppuv_xm_volslide
        {
            if (tick == 0)
                return period;

            int volume = channel->volume;
            mm_byte mem = channel->memory[MPP_XM_VCMD_MEM_VS];

            if (volcmd < 0x70) // mppuv_xm_volslide_down
            {
                volcmd -= 0x60;

                int delta;

                if (volcmd == 0)
                {
                    delta = mem & 0xF;
                }
                else
                {
                    delta = volcmd;
                    channel->memory[MPP_XM_VCMD_MEM_VS] = (mem & ~0xF) | volcmd;
                }

                // mppuv_voldownA

                volume -= delta;
                if (volume < 0) // Clamp
                    volume = 0;

                channel->volume = volume;
            }
            else // mppuv_xm_volslide_up
            {
                volcmd -= 0x70;

                int delta;

                if (volcmd == 0)
                {
                    delta = mem >> 4;
                }
                else
                {
                    delta = volcmd;
                    channel->memory[MPP_XM_VCMD_MEM_VS] = (volcmd << 4) | (mem & 0xF);
                }

                volume += delta;
                if (volume > 64)
                    volume = 64;

                channel->volume = volume;
            }
        }
        else if (volcmd < 0xA0) // Fine volume slide : mppuv_xm_fvolslide
        {
            if (tick != 0)
                return period;

            int volume = channel->volume;
            mm_byte mem = channel->memory[MPP_XM_VCMD_MEM_FVS];

            if (volcmd < 0x90) // mppuv_xm_volslide_down
            {
                volcmd -= 0x80;

                int delta;

                if (volcmd == 0)
                {
                    delta = mem & 0xF;
                }
                else
                {
                    delta = volcmd;
                    channel->memory[MPP_XM_VCMD_MEM_FVS] = (mem & ~0xF) | volcmd;
                }

                // mppuv_voldownA

                volume -= delta;
                if (volume < 0) // Clamp
                    volume = 0;

                channel->volume = volume;
            }
            else // mppuv_xm_volslide_up
            {
                volcmd -= 0x90;

                int delta;

                if (volcmd == 0)
                {
                    delta = mem >> 4;
                }
                else
                {
                    delta = volcmd;
                    channel->memory[MPP_XM_VCMD_MEM_FVS] = (volcmd << 4) | (mem & 0xF);
                }

                volume += delta;
                if (volume > 64)
                    volume = 64;

                channel->volume = volume;
            }
        }
        else if (volcmd < 0xC0) // Vibrato : mppuv_xm_vibrato
        {
            // Sets speed or depth

            if (tick == 0)
                return period;

            if (volcmd < 0xB0) // mppuv_xm_vibspd
            {
                volcmd = (volcmd - 0xA0) << 2;
                if (volcmd != 0)
                    channel->vibspd = volcmd;
            }
            else // mppuv_xm_vibdepth
            {
                volcmd = (volcmd - 0xB0) << 3;
                if (volcmd != 0)
                    channel->vibdep = volcmd;
            }

            return mppe_DoVibrato(period, channel, layer);
        }
        else if (volcmd < 0xD0) // Panning : mppuv_xm_panning
        {
            if (tick != 0)
                return period;

            mm_word panning = (volcmd - 0xC0) << 4;

            // This is a hack to make the panning reach the maximum value
            if (panning == 240)
                channel->panning = 255;
            else
                channel->panning = panning;
        }
        else if (volcmd < 0xF0) // Panning slide : mppuv_xm_panslide
        {
            if (tick == 0)
                return period;

            int panning = channel->panning;
            mm_byte mem = channel->memory[MPP_XM_VCMD_MEM_PANSL];

            if (volcmd < 0xE0) // mppuv_xm_panslide_left
            {
                volcmd -= 0xD0;

                int delta;

                if (volcmd == 0)
                {
                    delta = mem >> 4;
                }
                else
                {
                    channel->memory[MPP_XM_VCMD_MEM_PANSL] = (mem & 0xF) | (volcmd << 4);
                    delta = volcmd & 0xF;
                }

                delta <<= 2;

                panning -= delta;
                if (panning < 0)
                    panning = 0;

                channel->panning = panning;
            }
            else // mppuv_xm_panslide_right
            {
                volcmd -= 0xE0;

                int delta;

                if (volcmd == 0)
                {
                    delta = mem & 0xF;
                }
                else
                {
                    delta = volcmd;
                    channel->memory[MPP_XM_VCMD_MEM_PANSL] = volcmd | (mem & 0xF);
                }

                delta <<= 2;

                panning += delta;
                if (panning > 255)
                    panning = 255;

                channel->panning = panning;
            }
        }
        else // Glissando : mppuv_xm_toneporta
        {
            // On nonzero ticks, do a regular glissando slide at speed * 16
            if (tick == 0)
                return period;

            volcmd = (volcmd - 0xF0) << 4;

            if (volcmd != 0)
                channel->memory[MPP_XM_VCMD_MEM_GLIS] = volcmd;

            volcmd = channel->memory[MPP_XM_VCMD_MEM_GLIS];

            return mppe_glis_backdoor(volcmd, period, act_ch, channel, layer);
        }
    }
    else // IT commands
    {
        if (volcmd <= 64) // V: Set volume : mppuv_setvol
        {
            if (tick == 0)
                channel->volume = volcmd;
        }
        else if (volcmd <= 84) // A, B: Fine volume slide : mppuv_fvol
        {
            if (tick != 0)
                return period;

            int volume = channel->volume;

            if (volcmd < 75) // A: Slide up : mppuv_fvolup
            {
                volcmd -= 65; // 65-74 ==> 0-9

                if (volcmd == 0)
                    volcmd = channel->memory[MPP_IT_VCMD_MEM];
                else
                    channel->memory[MPP_IT_VCMD_MEM] = volcmd;

                volume += volcmd;
                if (volume > 64)
                    volume = 64;
            }
            else // B: Slide down : mppuv_fvoldown
            {
                volcmd -= 75; // 75-84 ==> 0-9

                if (volcmd == 0)
                    volcmd = channel->memory[MPP_IT_VCMD_MEM];
                else
                    channel->memory[MPP_IT_VCMD_MEM] = volcmd;

                volume -= volcmd;
                if (volume < 0)
                    volume = 0;
            }

            channel->volume = volume;
        }
        else if (volcmd <= 104) // C, D: Volume slide : mppuv_volslide
        {
            if (tick == 0)
                return period;

            int volume = channel->volume;

            if (volcmd < 95) // C: Slide up : mppuv_vs_up
            {
                volcmd -= 85; // 85-94 ==> 0-9

                if (volcmd == 0)
                    volcmd = channel->memory[MPP_IT_VCMD_MEM];
                else
                    channel->memory[MPP_IT_VCMD_MEM] = volcmd;

                volume += volcmd;
                if (volume > 64)
                    volume = 64;
            }
            else // D: Slide down : mppuv_vs_down
            {
                volcmd -= 95; // 95-104 ==> 0-9

                if (volcmd == 0)
                    volcmd = channel->memory[MPP_IT_VCMD_MEM];
                else
                    channel->memory[MPP_IT_VCMD_MEM] = volcmd;

                volume -= volcmd;
                if (volume < 0)
                    volume = 0;
            }

            channel->volume = volume;
        }
        else if (volcmd <= 124) // E, F: Pitch slide/Portamento up/down : mppuv_porta
        {
            if (tick == 0)
                return period;

            mm_word r0;

            if (volcmd >= 115) // F: mppuv_porta_up
            {
                volcmd = (volcmd - 115) << 2;

                if (volcmd == 0)
                    volcmd = channel->memory[MPP_IT_PORTA];

                channel->memory[MPP_IT_PORTA] = volcmd;

                r0 = mpph_PitchSlide_Up(channel->period, volcmd, layer);
            }
            else // E: mppuv_porta_down
            {
                volcmd = (volcmd - 105) << 2;

                if (volcmd == 0)
                    volcmd = channel->memory[MPP_IT_PORTA];

                channel->memory[MPP_IT_PORTA] = volcmd;

                r0 = mpph_PitchSlide_Down(channel->period, volcmd, layer);
            }

            mm_word r1 = channel->period;

            channel->period = period;

            return period + r0 - r1;
        }
        else if (volcmd <= 192) // P: Panning : mppuv_panning
        {
            if (tick == 0)
            {
                int panning = volcmd - 128; // Map to 0->64

                panning <<= 2;
                if (panning > 255)
                    panning = 255;

                channel->panning = panning;
            }
        }
        else if (volcmd <= 202) // G: Glissando/Portamento to note : mppuv_glissando
        {
            if (tick == 0)
                return period;

            const mm_byte vcmd_glissando_table[] = {
                0, 1, 4, 8, 16, 32, 64, 96, 128, 255
            };

            volcmd -= 193;

            mm_word glis = vcmd_glissando_table[volcmd];

            if (layer->flags & MAS_HEADER_FLAG_LINK_GXX)
            {
                // Gxx is shared, IT MODE ONLY!!

                // When this flag is enabled, link effect G's memory with the
                // memory used by effects E/F

                if (glis == 0)
                    glis = channel->memory[MPP_IT_PORTA];

                channel->memory[MPP_IT_PORTA] = glis; // E/F memory
                channel->memory[MPP_XM_IT_GLIS] = glis;

                mm_byte mem = channel->memory[MPP_XM_IT_GLIS];

                return mppe_glis_backdoor(mem, period, act_ch, channel, layer);
            }
            else // Single Gxx
            {
                if (glis == 0)
                    glis = channel->memory[MPP_XM_IT_GLIS];

                channel->memory[MPP_XM_IT_GLIS] = glis;

                mm_byte mem = channel->memory[MPP_XM_IT_GLIS];

                return mppe_glis_backdoor(mem, period, act_ch, channel, layer);
            }
        }
        else if (volcmd <= 212) // H: Vibrato (Speed) : mppuv_vibrato
        {
            // VCMD vibrato uses the same memory as effects Hxx/Uxx.

            if (tick == 0)
                return period;

            // Set speed

            volcmd = volcmd - 203;
            if (volcmd != 0)
            {
                volcmd = volcmd << 2;
                channel->vibspd = volcmd;
            }

            return mppe_DoVibrato(volcmd, channel, layer);
        }
    }

    return period;
}

static void mpph_FastForward(mpl_layer_information *layer, mm_word rows_to_skip)
{
    if (rows_to_skip == 0)
        return;

    // layer->nrows has the number of rows in the current pattern minus one
    if (rows_to_skip > layer->nrows)
        return;

    layer->row = rows_to_skip;

    while (1)
    {
        mm_bool ok = mmReadPattern(layer);

        // If there was some error (the module uses too many channels, for
        // example), stop it right away.
        if (ok == 0)
        {
            mppStop();

            if (mmCallback != NULL)
                mmCallback(MMCB_SONGERROR, mpp_clayer);

            break;
        }

        rows_to_skip--;
        if (rows_to_skip == 0)
            break;
    }
}

void mmSetPositionEx(mm_word position, mm_word row)
{
    // TODO: This was commented out in the original code. If it is uncommented,
    // all channels will be reseted when setting the position. If not, the
    // channels will keep playing and they will only be silenced when the notes
    // end or when a new note overwrites it.
    //
    //mpp_clayer = MM_MAIN;
    //mpp_resetchannels(mm_pchannels, mm_num_mch);

    mpp_setposition(&mmLayerMain, position);

    if (row != 0)
        mpph_FastForward(&mmLayerMain, row);
}

// An effect of 0 means custom behaviour, or disabled
static mm_word mpp_Channel_ExchangeMemory(mm_byte effect, mm_byte param,
                                          mm_module_channel *channel,
                                          mpl_layer_information *layer)
{
    mm_sbyte table_entry;

    // Check flags for XM mode
    if (layer->flags & MAS_HEADER_FLAG_XM_MODE) // XM Effects
    {
        // mmutil converts XM effect indices into IT effect indices (and it adds
        // effects '0' to '3').
        //
        // https://wiki.openmpt.org/Manual:_Effect_Reference#MOD_/_XM_Formats
        const mm_sbyte mpp_effect_memmap_xm[] = {
            // Legend | IT effect number (XM effect number): Description

            -1,                 // No effect
            -1,                 // A (F): Set speed
            -1,                 // B (B): Position jump
            -1,                 // C (D): Pattern break
            MPP_XM_VOLSLIDE,    // D (A): Volume slide
            MPP_XM_PORTA_DOWN,  // E (2, E, X): Portamento down / Pitch slide down
            MPP_XM_PORTA_UP,    // F (1, E, X): Portamento up / Pitch slide up
            -1,                 // G (3): Portamento to note / Glissando
            -1,                 // H (4): Vibrato
            MPP_XM_UNKNOWN,     // I: Not available in XM // TODO: Why does it have a value?
            -1,                 // J (0): Arpeggio
            MPP_XM_VOLSLIDE_VIBR, // K (6): Volume slide + Vibrato
            MPP_XM_VOLSLIDE_GLIS, // L (5): Volume slide + Glissando
            -1,                 // M: Not available in XM
            -1,                 // N: Not available in XM
            MPP_XM_SAMP_OFFSET, // O (9): Set offset
            MPP_XM_PANSLIDE,    // P (P): Panning slide
            MPP_XM_RETRIG_NOTE, // Q (R): Retrigger note
            MPP_XM_TREMOLO,     // R (7): Tremolo
            -1,                 // S (E): Extended effects
            -1,                 // T (F): Set tempo
            -1,                 // U: Not available in XM
            -1,                 // V (G): Set global volume
            MPP_XM_GL_VOLSLIDE, // W (H): Global volume slide
            -1,                 // X (8, E): Set panning
            -1,                 // Y: Not available in XM
            -1,                 // Z: Not available in XM

            // For effects not available in the IT format this array has been
            // extended. The last 4 values don't correspond to IT effects. The
            // index between [] is the value used in mmutil.

            -1,                 // 0 [27] (C): Set volume
            -1,                 // 1 [28] (K): Key off
            -1,                 // 2 [29] (L): Set envelope position
            MPP_XM_TREMOR,      // 3 [30] (T): Tremor

            // Unused XM effects: I, M, N, V. They aren't generated by mmutil
            // when converting from XM to IT.
        };

        table_entry = mpp_effect_memmap_xm[effect];
    }
    else // IT Effects
    {
        // https://wiki.openmpt.org/Manual:_Effect_Reference#IT_Effect_Commands
        const mm_sbyte mpp_effect_memmap_it[] = {
            -1,                 // No effect
            -1,                 // A: Set Speed
            -1,                 // B: Jump to order
            -1,                 // C: Break to row

            // This doesn't share memory with VCMD volume slides
            MPP_IT_VOLSLIDE,    // D: Volume slide

            // E/F/G: These commands ALL share the same memory.
            MPP_IT_PORTA,       // E: Portamento down / Pitch slide down
            MPP_IT_PORTA,       // F: Portamento up / Pitch slide up
            -1,                 // G: Portamento to note / Glissando

            -1,                 // H: Vibrato
            MPP_IT_TREMOR,      // I: Tremor
            MPP_IT_ARPEGGIO,    // J: Arpeggio

            // The parameter of the two dual commands is the volume slide parameter
            MPP_IT_VOLSLIDE,    // K: Dual command: Vibrato + Volume slide (H + D)
            MPP_IT_VOLSLIDE,    // L: Dual Command: Portamento to note and Volume slide (G + D)

            -1,                 // M: Set channel volume
            MPP_IT_CH_VOLSLIDE, // N: Channel volume slide
            MPP_IT_SAMP_OFFSET, // O: Set sample offset
            MPP_IT_PANSLIDE,    // P: Pan slide
            MPP_IT_RETRIG_NOTE, // Q: Retriggers a note
            MPP_IT_TREMOLO,     // R: Tremolo
            MPP_IT_EXTENDED_FX, // S: Extended effects
            MPP_IT_TEMPO,       // T: Tempo
            -1,                 // U: Fine vibrato
            -1,                 // V: Set Global volume
            MPP_IT_GL_VOLSLIDE, // W: Global volume slide
            -1,                 // X: Set panning
            MPP_IT_PANBRELLO,   // Y: Panbrello
            -1,                 // Z: Midi macro
        };

        table_entry = mpp_effect_memmap_it[effect];
    }

    // If the effect doesn't use the memory table there's nothing left to do
    if (table_entry == -1)
        return param;

    if (param == 0)
    {
        // If the parameter is empty, load it from memory
        channel->param = channel->memory[table_entry];
        return channel->param;
    }
    else
    {
        // If the parameter isn't empty, save it to memory
        channel->memory[table_entry] = param;
        return param;
    }
}

// Process module tick
IWRAM_CODE void mppProcessTick(void)
{
    mpl_layer_information *layer = mpp_layerp;

    // Test if module is playing
    if (layer->isplaying == 0)
        return;

    // Read pattern data

    if ((layer->pattdelay == 0) && (layer->tick == 0))
    {
        mm_bool ok = mmReadPattern(layer);

        // If there was some error (the module uses too many channels, for
        // example), stop it right away.
        if (ok == 0)
        {
            mppStop();

            if (mmCallback != NULL)
                mmCallback(MMCB_SONGERROR, mpp_clayer);

            return;
        }
    }

    // Loop through module channels

    mm_word update_bits = layer->mch_update;
    mm_module_channel *module_channels = mpp_channels;

    for (mm_word channel_counter = 0; ; channel_counter++)
    {
        if (update_bits & (1 << 0))
        {
            // Handle first tick and other ticks differently
            if (layer->tick == 0)
                mmUpdateChannel_T0(module_channels, layer, channel_counter);
            else
                mmUpdateChannel_TN(module_channels, layer);
        }

        module_channels++;

        update_bits >>= 1;
        if (update_bits == 0)
            break;
    }

    mm_active_channel *act_ch = &mm_achannels[0];

    for (mm_word ch = 0; ch < mm_num_ach; ch++)
    {
        if (act_ch->type != ACHN_DISABLED)
        {
            // Check if this active channel is being used by the selected layer
            // (and check that it isn't a sound effect).
            if (mpp_clayer == ((act_ch->flags & (MCAF_SUB | MCAF_EFFECT)) >> 6))
            {
                mpp_vars.afvol = act_ch->volume;
                mpp_vars.panplus = 0;

                mpp_Update_ACHN(layer, act_ch, act_ch->period, ch);
            }
        }

        act_ch->flags &= ~MCAF_UPDATED;

        act_ch++;
    }

    // This is the inlined code of mppProcessTick_incframe()

    mm_word new_tick = layer->tick + 1;

    // If the new tick is lower than the speed continue with this row
    if (new_tick < layer->speed)
    {
        // Continue current row
        layer->tick = new_tick;
        return;
    }

    if (layer->fpattdelay != 0)
        layer->fpattdelay--;

    // Otherwise clear the tick count and advance to next row

    if (layer->pattdelay != 0)
    {
        layer->pattdelay--;
        if (layer->pattdelay != 0)
        {
            // Continue current row
            layer->tick = 0;
            return;
        }
    }

    layer->tick = 0;

    if (layer->pattjump != 255)
    {
        mpp_setposition(layer, layer->pattjump);
        layer->pattjump = 255;

        if (layer->pattjump_row == 0)
            return;

        mpph_FastForward(layer, layer->pattjump_row);
        layer->pattjump_row = 0;
        return;
    }

    if (layer->ploop_jump != 0)
    {
        layer->ploop_jump = 0;
        layer->row = layer->ploop_row;
        layer->pattread = layer->ploop_adr;
        return;
    }

    int new_row = layer->row + 1;
    if (new_row != (layer->nrows + 1))
    {
        // If they are different, continue playing this pattern
        layer->row = new_row;
        return;
    }

    // Advance position
    mpp_setposition(layer, layer->position + 1);
}

// Note: This is also used for panning slide
static mm_word mpph_VolumeSlide(int volume, mm_word param, mm_word tick, int max_volume,
                                mpl_layer_information *layer)
{
    if (layer->flags & MAS_HEADER_FLAG_XM_MODE) // mpph_vs_XM
    {
        if (tick != 0)
        {
            int r3 = param >> 4;
            int r1 = param & 0xF;

            int new_val = volume + r3 - r1;

            if (new_val > max_volume)
                new_val = max_volume;
            if (new_val < 0)
                new_val = 0;

            volume = new_val;
        }

        return volume;
    }
    else // mpph_vs_IT
    {
        if (param == 0xF)
        {
            volume -= 0xF;
            if (volume < 0)
                return 0;

            return volume;
        }

        if (param == 0xF0)
        {
            if (tick != 0)
                return volume;

            volume += 0xF;
            if (volume > max_volume)
                return max_volume;

            return volume;
        }

        if ((param & 0xF) == 0) // Test for Dx0 : mpph_vs_add
        {
            if (tick == 0)
                return volume;

            volume += param >> 4;
            if (volume > max_volume)
                return max_volume;

            return volume;
        }

        if ((param >> 4) == 0) // Test for D0x : mpph_vs_sub
        {
            if (tick == 0)
                return volume;

            volume -= param & 0xF;
            if (volume < 0)
                return 0;

            return volume;
        }

        // Fine slides now... only slide on tick 0
        if (tick != 0)
            return volume;

        if ((param & 0xF) == 0xF) // Test for DxF
        {
            volume += param >> 4;
            if (volume > max_volume)
                return max_volume;

            return volume;
        }

        if ((param >> 4) == 0xF) // Test for DFx
        {
            volume -= param & 0xF;
            if (volume < 0)
                return 0;

            return volume;
        }

        return volume;
    }
}

static mm_word mpph_VolumeSlide64(int volume, mm_word param, mm_word tick,
                                  mpl_layer_information *layer)
{
    return mpph_VolumeSlide(volume, param, tick, 64, layer);
}

static const mm_sbyte mpp_TABLE_FineSineData[] = {
      0,   2,   3,   5,   6,   8,   9,  11,  12,  14,  16,  17,  19,  20,  22,  23,
     24,  26,  27,  29,  30,  32,  33,  34,  36,  37,  38,  39,  41,  42,  43,  44,
     45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  56,  57,  58,  59,
     59,  60,  60,  61,  61,  62,  62,  62,  63,  63,  63,  64,  64,  64,  64,  64,
     64,  64,  64,  64,  64,  64,  63,  63,  63,  62,  62,  62,  61,  61,  60,  60,
     59,  59,  58,  57,  56,  56,  55,  54,  53,  52,  51,  50,  49,  48,  47,  46,
     45,  44,  43,  42,  41,  39,  38,  37,  36,  34,  33,  32,  30,  29,  27,  26,
     24,  23,  22,  20,  19,  17,  16,  14,  12,  11,   9,   8,   6,   5,   3,   2,
      0,  -2,  -3,  -5,  -6,  -8,  -9, -11, -12, -14, -16, -17, -19, -20, -22, -23,
    -24, -26, -27, -29, -30, -32, -33, -34, -36, -37, -38, -39, -41, -42, -43, -44,
    -45, -46, -47, -48, -49, -50, -51, -52, -53, -54, -55, -56, -56, -57, -58, -59,
    -59, -60, -60, -61, -61, -62, -62, -62, -63, -63, -63, -64, -64, -64, -64, -64,
    -64, -64, -64, -64, -64, -64, -63, -63, -63, -62, -62, -62, -61, -61, -60, -60,
    -59, -59, -58, -57, -56, -56, -55, -54, -53, -52, -51, -50, -49, -48, -47, -46,
    -45, -44, -43, -42, -41, -39, -38, -37, -36, -34, -33, -32, -30, -29, -27, -26,
    -24, -23, -22, -20, -19, -17, -16, -14, -12, -11,  -9,  -8,  -6,  -5,  -3,  -2,
};

static mm_word mppe_DoVibrato(mm_word period, mm_module_channel *channel,
                              mpl_layer_information *layer)
{
    mm_byte position;

    if ((layer->oldeffects == 0) || (layer->tick != 0)) // Update effect
    {
        position = channel->vibspd + channel->vibpos; // Wrap to 8 bits
        channel->vibpos = position;
    }
    else
    {
        position = channel->vibpos;
    }

    mm_sword value = mpp_TABLE_FineSineData[position];
    mm_sword depth = channel->vibdep;

    value = (value * depth) >> 8;

    if (value < 0)
        return mpph_PitchSlide_Down(period, -value, layer);

    return mpph_PitchSlide_Up(period, value, layer);
}

// =============================================================================
//                                      EFFECTS
// =============================================================================

// EFFECT Axy: SET SPEED
static void mppe_SetSpeed(mm_word param, mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return;

    if (param != 0)
        layer->speed = param;
}

// EFFECT Bxy: SET POSITION
static void mppe_PositionJump(mm_word param, mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return;

    layer->pattjump = param;
}

// EFFECT Cxy: PATTERN BREAK
static void mppe_PatternBreak(mm_word param, mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return;

    layer->pattjump_row = param;

    if (layer->pattjump == 255) // Check if pattjump is empty
        layer->pattjump = layer->position + 1;
}

// EFFECT Dxy: VOLUME SLIDE
static void mppe_VolumeSlide(mm_word param, mm_module_channel *channel,
                      mpl_layer_information *layer)
{
    channel->volume = mpph_VolumeSlide64(channel->volume, param, layer->tick, layer);
}

// EFFECT Exy/Fxy: Portamento
static mm_word mppe_Portamento(mm_word param, mm_word period,
                               mm_module_channel *channel, mpl_layer_information *layer)
{
    bool is_fine = false;

    if ((param >> 4) == 0xE) // Test for Ex param (Extra fine slide)
    {
        if (layer->tick != 0) // Extra fine slide: only slide on tick 0
            return period;

        param &= 0xF; // Mask out slide value
        is_fine = true;
    }
    else if ((param >> 4) == 0xF) // Test for Fx param (Fine slide)
    {
        if (layer->tick != 0) // Fine slide: only slide on tick 0
            return period;

        param &= 0xF; // Mask out slide value
    }
    else
    {
        // Regular slide
        if (layer->tick == 0)
            return period;
    }

    // Check slide direction

    mm_word new_period;

    if (channel->effect == 5) // 5 = portamento down
    {
        // Slide down
        if (is_fine)
            new_period = mpph_FinePitchSlide_Down(channel->period, param, layer);
        else
            new_period = mpph_PitchSlide_Down(channel->period, param, layer);
    }
    else
    {
        // Slide up
        if (is_fine)
            new_period = mpph_FinePitchSlide_Up(channel->period, param, layer);
        else
            new_period = mpph_PitchSlide_Up(channel->period, param, layer);
        // TODO: This doesn't seem to have any check to prevent overflows
    }

    int old_period = channel->period;

    channel->period = new_period;

    int delta = new_period - old_period;

    return period + delta;
}

static mm_word mppe_glis_backdoor(mm_word param, mm_word period, mm_active_channel *act_ch,
                                  mm_module_channel *channel, mpl_layer_information *layer)
{
    if (act_ch == NULL) // Exit if no active channel
        return period;

    mm_mas_sample_info *sample = mpp_SamplePointer(layer, act_ch->sample); // Get target period

    mm_word target_period = mmGetPeriod(layer, sample->frequency * 4, channel->note);

    mm_word new_period;

    if (layer->flags & MAS_HEADER_FLAG_FREQ_MODE)
    {
        if (channel->period < target_period) // Slide up
        {
            new_period = mpph_PitchSlide_Up(channel->period, param, layer);

            if (new_period > target_period)
                new_period = target_period;
        }
        else if (channel->period > target_period) // Slide down
        {
            new_period = mpph_PitchSlide_Down(channel->period, param, layer);

            if (new_period < target_period)
                new_period = target_period;
        }
        else // No slide
        {
            return period;
        }
    }
    else // Amiga
    {
        // TODO: Is this correct? In the original code, mpph_PitchSlide_Down and
        // mpph_PitchSlide_Down were swapped in the Amiga side. However, if this
        // is swapped, the effect breaks.

        if (channel->period < target_period) // Slide up
        {
            new_period = mpph_PitchSlide_Down(channel->period, param, layer);

            if (new_period > target_period)
                new_period = target_period;
        }
        else if (channel->period > target_period) // Slide down
        {
            new_period = mpph_PitchSlide_Up(channel->period, param, layer);

            if (new_period < target_period)
                new_period = target_period;
        }
        else // No slide
        {
            return period;
        }
    }

    mm_word old_period = channel->period;

    channel->period = new_period;

    int delta = new_period - old_period;

    return period + delta;
}

// EFFECT Gxy: Glissando
static mm_word mppe_Glissando(mm_word param, mm_word period, mm_active_channel *act_ch,
                              mm_module_channel *channel, mpl_layer_information *layer)
{
    if (layer->tick == 0)
    {
        if (layer->flags & MAS_HEADER_FLAG_LINK_GXX)
        {
            // Gxx is shared, IT MODE ONLY!!

            if (param == 0)
            {
                param = channel->memory[MPP_IT_PORTA];
                channel->param = param;
            }

            channel->memory[MPP_IT_PORTA] = param;
            channel->memory[MPP_XM_IT_GLIS] = param; // For simplification later
        }
        else
        {
            // Gxx is separate
            if (param == 0)
            {
                param = channel->memory[MPP_XM_IT_GLIS];
                channel->param = param;
            }

            channel->memory[MPP_XM_IT_GLIS] = param;

            return period;
        }
    }

    param = channel->memory[MPP_XM_IT_GLIS];

    period = mppe_glis_backdoor(param, period, act_ch, channel, layer);

    return period;
}

// EFFECT Hxy: Vibrato
static mm_word mppe_Vibrato(mm_word param, mm_word period, mm_module_channel *channel,
                            mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return mppe_DoVibrato(period, channel, layer);

    mm_word x = param >> 4;
    mm_word y = param & 0xF;

    if (x != 0)
        channel->vibspd = x * 4;

    if (y != 0)
    {
        mm_word depth = y * 4;

        // if (OldEffects)
        //     depth <<= 1;
        channel->vibdep = depth << layer->oldeffects;

        return mppe_DoVibrato(period, channel, layer);
    }

    return period;
}

// EFFECT Jxy: Arpeggio
static mm_word mppe_Arpeggio(mm_word param, mm_word period, mm_active_channel *act_ch,
                             mm_module_channel *channel, mpl_layer_information *layer)
{
    if (layer->tick == 0)
        channel->fxmem = 0;

    if (act_ch == NULL)
        return period;

    mm_word semitones;

    if (channel->fxmem > 1) // mppe_arp_2
    {
        channel->fxmem = 0; // Set next tick to 0
        semitones = param & 0xF; // Mask out low nibble of param
    }
    else if (channel->fxmem == 1) // mppe_arp_1
    {
        channel->fxmem = 2; // Set next tick to 2
        semitones = param >> 4; // Mask out high nibble of param
    }
    else // mppe_arp_0
    {
        // Do nothing! :)
        channel->fxmem = 1;
        return period;
    }

    // The assembly code had the following conditional, but the register used to
    // store the period was overwritten by mpph_LinearPitchSlide_Up() right
    // after jumping to it, even in the original assembly code before the C port
    // started. Is this a bug, or do we need to enable it?
    //
    // See if its >= 12. Double period if so... (set an octave higher)
    //if (semitones >= 12)
    //    period *= 2;

    semitones *= 16; // 16 hwords

    period = mpph_LinearPitchSlide_Up(period, semitones, layer);
    return period;
}

// EFFECT Kxy: Vibrato+Volume Slide
static mm_word mppe_VibratoVolume(mm_word param, mm_word period, mm_module_channel *channel,
                                  mpl_layer_information *layer)
{
    mm_word new_period = mppe_DoVibrato(period, channel, layer);

    mppe_VolumeSlide(param, channel, layer);

    return new_period;
}

// EFFECT Lxy: Portamento+Volume Slide
static mm_word mppe_PortaVolume(mm_word param, mm_word period, mm_active_channel *act_ch,
                                mm_module_channel *channel, mpl_layer_information *layer)
{
    mm_word mem = channel->memory[MPP_XM_IT_GLIS];

    period = mppe_Glissando(mem, period, act_ch, channel, layer);

    mppe_VolumeSlide(param, channel, layer);

    return period;
}

// EFFECT Mxy: Set Channel Volume
static void mppe_ChannelVolume(mm_word param, mm_module_channel *channel,
                               mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return;

    if (param > 0x40) // Ignore command if parameter is greater than 0x40
        return;

    channel->cvolume = param;
}

// EFFECT Nxy: Channel Volume Slide
static void mppe_ChannelVolumeSlide(mm_word param, mm_module_channel *channel,
                                    mpl_layer_information *layer)
{
    channel->cvolume = mpph_VolumeSlide64(channel->cvolume, param, layer->tick, layer);
}

// EFFECT Oxy Sample Offset
static void mppe_SampleOffset(mm_word param, mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return;

    mpp_vars.sampoff = param;
}

#if 0
// EFFECT Pxy Panning Slide
static void mppe_PanningSlide(mm_word param, mm_module_channel *channel,
                              mpl_layer_information *layer)
{
    // TODO: This is unused! Is that a mistake, or was this buggy?
    channel->panning = mpph_VolumeSlide(channel->panning, param, layer->tick, 255, layer);
}
#endif

// EFFECT Qxy Retrigger Note
static void mppe_Retrigger(mm_word param, mm_active_channel *act_ch, mm_module_channel *channel)
{
    // We don't check layer->tick here. We set channel->fxmem to the parameter
    // and every time this function gets called it goes down by one. When it
    // reaches 1, the command is executed.
    //
    // Note that we can't use 0 as a countdown target. That would be interpreted
    // as "fxmem hasn't been set", so we need to store the count plus 1.

    mm_word mem = channel->fxmem;

    if (mem == 0) // mppe_retrig_refill
    {
        channel->fxmem = (param & 0xF) + 1;
        return;
    }

    mem--;

    if (mem != 1)
    {
        channel->fxmem = mem;
        return;
    }

    channel->fxmem = (param & 0xF) + 1;

    // Handle subcommand

    int vol = channel->volume;

    mm_word arg = param >> 4;

    if (arg == 0) // No change
    {
    }
    else if (arg <= 5) // -1, -2, -4, -8, -16
    {
        vol -= 1 << (arg - 1);
        if (vol < 0)
            vol = 0;
    }
    else if (arg == 6) // * (2 / 3)
    {
        vol = (vol * 171) >> 8;
    }
    else if (arg == 7) // * (1 / 2)
    {
        vol >>= 1;
    }
    else if (arg == 8) // Nothing
    {
    }
    else if (arg <= 0xD) // +1, +2, +4, +8, +16
    {
        vol += 1 << (arg - 9);
        if (vol > 64)
            vol = 64;
    }
    else if (arg == 0xE) // * (3 / 2)
    {
        vol = (vol * 192) >> 7;
    }
    else // if (arg == 0xF) // * 2
    {
        vol <<= 1;
        if (vol > 64)
            vol = 64;
    }

    channel->volume = vol;

    if (act_ch != NULL)
        act_ch->flags |= MCAF_START;
}

// EFFECT Rxy: Tremolo
static void mppe_Tremolo(mm_word param, mm_module_channel *channel, mpl_layer_information *layer)
{
    // X = speed, Y = depth

    if (layer->tick != 0)
    {
        // Get sine position
        mm_word position = channel->fxmem;

        mm_word speed = param >> 4;
        // Mask out SPEED, multiply by 4 to compensate for larger sine table
        position += speed * 4;

        // Save (value & 255)
        channel->fxmem = position;
    }

    mm_word position = channel->fxmem; // Get sine position
    mm_sword sine = mpp_TABLE_FineSineData[position]; // Load sine table value

    mm_word depth = param & 0xF;

    mm_sword result = (sine * depth) >> 6; // Sine * depth / 64

    if (layer->flags & MAS_HEADER_FLAG_XM_MODE)
        result >>= 1;

    mpp_vars.volplus = result; // Set volume addition variable
}

// EFFECT Txy: Set Tempo / Tempo Slide
static void mppe_SetTempo(mm_word param, mpl_layer_information *layer)
{
    if (param < 0x10) // 0x = Slide down
    {
        if (layer->tick == 0)
            return;

        int bpm = layer->bpm - param;

        if (bpm < 32)
            bpm = 32;

        mpp_setbpm(layer, bpm);
    }
    else if (param < 0x20) // 1x = Slide up
    {
        if (layer->tick == 0)
            return;

        int bpm = layer->bpm + (param & 0xF);

        if (bpm > 255)
            bpm = 255;

        mpp_setbpm(layer, bpm);
    }
    else // 2x+ = Set
    {
        if (layer->tick != 0)
            return;

        mpp_setbpm(layer, param);
    }
}

// EFFECT Uxy: Fine Vibrato
static mm_word mppe_FineVibrato(mm_word param, mm_word period, mm_module_channel *channel,
                                mpl_layer_information *layer)
{
    if (layer->tick == 0)
    {
        mm_word x = param >> 4;
        mm_word y = param & 0xF;

        if (x != 0)
            channel->vibspd = x * 4;

        if (y != 0)
        {
            mm_word depth = y * 4;

            // if (OldEffects)
            //     depth <<= 1;
            channel->vibdep = depth << layer->oldeffects;
        }
    }

    return mppe_DoVibrato(period, channel, layer);
}

// EFFECT Vxy: Set Global Volume
static void mppe_SetGlobalVolume(mm_word param, mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return;

    mm_word mask = MAS_HEADER_FLAG_XM_MODE | MAS_HEADER_FLAG_OLD_MODE;

    mm_word maxvol;

    if (layer->flags & mask)
        maxvol = 64;
    else
        maxvol = 128;

    layer->global_volume = (param < maxvol) ? param : maxvol;
}

// EFFECT Wxy: Global Volume Slide
static void mppe_GlobalVolumeSlide(mm_word param, mpl_layer_information *layer)
{
    mm_word maxvol;

    if (layer->flags & MAS_HEADER_FLAG_XM_MODE)
        maxvol = 64;
    else
        maxvol = 128;

    layer->global_volume = mpph_VolumeSlide(layer->global_volume, param,
                                            layer->tick, maxvol, layer);
}

// EFFECT Xxy: Set Panning
static void mppe_SetPanning(mm_word param, mm_module_channel *channel,
                     mpl_layer_information *layer)
{
    if (layer->tick == 0)
        channel->panning = param;
}

// =============================================================================
//                                  EXTENDED EFFECTS
// =============================================================================

static void mppex_XM_FVolSlideUp(mm_word param, mm_module_channel *channel,
                                 mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return;

    int volume = channel->volume + (param & 0xF);

    if (volume > 64)
        volume = 64;

    channel->volume = volume;
}

static void mppex_XM_FVolSlideDown(mm_word param, mm_module_channel *channel,
                                   mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return;

    int volume = channel->volume - (param & 0xF);

    if (volume < 0)
        volume = 0;

    channel->volume = volume;
}

static void mppex_OldRetrig(mm_word param, mm_active_channel *act_ch,
                            mm_module_channel *channel, mpl_layer_information *layer)
{
    if (layer->tick == 0)
    {
        channel->fxmem = param & 0xF;
        return;
    }

    channel->fxmem--;
    if (channel->fxmem == 0)
    {
        channel->fxmem = param & 0xF;

        if (act_ch != NULL)
            act_ch->flags |= MCAF_START;
    }
}

static void mppex_FPattDelay(mm_word param, mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return;

    layer->fpattdelay = param & 0xF;
}

static void mppex_InstControl(mm_word param, mm_active_channel *act_ch,
                              mm_module_channel *channel, mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return;

    mm_word subparam = param & 0xF;

    if (subparam <= 2) // mppex_ic_pastnotes
    {
        // TODO
    }
    else if (subparam <= 6) // mppex_ic_nna
    {
        // Overwrite NNA
        channel->bflags &= ~MCH_BFLAGS_NNA_MASK;
        channel->bflags |= MCH_BFLAGS_NNA_SET(subparam - 3);
    }
    else if (subparam <= 8) // mppex_ic_envelope
    {
        if (act_ch != NULL)
        {
            int val = subparam - 7; // val can be 0 or 1

            if (val)
                act_ch->flags |= MCAF_VOLENV;
            else
                act_ch->flags &= ~MCAF_VOLENV;
        }
    }
}

static void mppex_SetPanning(mm_word param, mm_module_channel *channel)
{
    channel->panning = param << 4;
}

static void mppex_SoundControl(mm_word param)
{
    if (param != 0x91)
        return;

    // Set surround
    // TODO
}

static void mppex_PatternLoop(mm_word param, mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return;

    mm_word subparam = param & 0xF;

    if (subparam == 0)
    {
        layer->ploop_row = layer->row;
        layer->ploop_adr = mpp_vars.pattread_p;
        return;
    }

    mm_word counter = layer->ploop_times; // Get pattern loop counter

    if (counter == 0)
    {
        layer->ploop_times = subparam; // Save parameter to counter
        layer->ploop_jump = 1; // Enable jump
    }
    else
    {
        // It is already active
        layer->ploop_times = counter - 1;
        if (layer->ploop_times != 0)
            layer->ploop_jump = 1; // Enable jump
    }
}

static void mppex_NoteCut(mm_word param, mm_module_channel *channel,
                          mpl_layer_information *layer)
{
    mm_word reference = param & 0xF;

    if (layer->tick != reference)
        return;

    channel->volume = 0;
}

static void mppex_NoteDelay(mm_word param, mpl_layer_information *layer)
{
    mm_word reference = param & 0xF;

    if (layer->tick >= reference)
        return;

    mpp_vars.notedelay = reference;
}

static void mppex_PatternDelay(mm_word param, mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return;

    if (layer->pattdelay == 0)
        layer->pattdelay = (param & 0xF) + 1;
}

static void mppex_SongMessage(mm_word param, mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return;

    if (mmCallback != NULL)
        mmCallback(MMCB_SONGMESSAGE, (param & 0xF) | (mpp_clayer << 4));
}

// EFFECT Sxy: Extended Effects
static void mppe_Extended(mm_word param, mm_active_channel *act_ch,
                          mm_module_channel *channel, mpl_layer_information *layer)
{
    mm_word subcmd = param >> 4;

    switch (subcmd)
    {
        case 0x0: // S0x
            mppex_XM_FVolSlideUp(param, channel, layer);
            break;
        case 0x1: // S1x
            mppex_XM_FVolSlideDown(param, channel, layer);
            break;
        case 0x2: // S2x
            mppex_OldRetrig(param, act_ch, channel, layer);
            break;
        case 0x3: // S3x
            // mppex_VibForm
            break;
        case 0x4: // S4x
            // mppex_TremForm
            break;
        case 0x5: // S5x
            // mppex_PanbForm
            break;

        case 0x6: // S6x
            mppex_FPattDelay(param, layer);
            break;
        case 0x7: // S7x
            mppex_InstControl(param, act_ch, channel, layer);
            break;
        case 0x8: // S8x
            mppex_SetPanning(param, channel);
            break;
        case 0x9: // S9x
            mppex_SoundControl(param);
            break;
        case 0xA: // SAx
            // mppex_HighOffset
            break;
        case 0xB: // SBx
            mppex_PatternLoop(param, layer);
            break;
        case 0xC: // SCx
            mppex_NoteCut(param, channel, layer);
            break;
        case 0xD: // SDx
            mppex_NoteDelay(param, layer);
            break;
        case 0xE: // SEx
            mppex_PatternDelay(param, layer);
            break;
        case 0xF: // SFx
            mppex_SongMessage(param, layer);
            break;

        default:
            break;
    }
}

// =============================================================================
//                      XM EFFECTS (NOT AVAILABLE IN IT)
// =============================================================================

// EFFECT 0xx: Set Volume
static void mppe_SetVolume(mm_word param, mm_module_channel *channel,
                           mpl_layer_information *layer)
{
    if (layer->tick == 0)
        channel->volume = param;
}

// EFFECT 1xx: Key Off
static void mppe_KeyOff(mm_word param, mm_active_channel *act_ch,
                        mpl_layer_information *layer)
{
    if (layer->tick != param)
        return;

    if (act_ch != NULL)
        act_ch->flags &= ~MCAF_KEYON;
}

#if 0
// EFFECT 1xx: Envelope Position
static void mppe_EnvelopePos(mm_word param, mm_active_channel *act_ch,
                             mpl_layer_information *layer)
{
    if (layer->tick != 0)
        return;

    if (act_ch == NULL)
        return;

    // NOT SUPPORTED ANYMORE

    //act_ch->envp_vol = r1; // Set volume envelope position
    //act_ch->envp_pan = r1; // Set panning envelope position
    // Pitch envelope wasn't invented yet
}

#endif

// EFFECT 3xy: Old Tremor
static void mppe_OldTremor(mm_word param, mm_module_channel *channel,
                           mpl_layer_information *layer)
{
    if (layer->tick == 0)
        return;

    int mem = channel->fxmem;
    if (mem == 0) // Old
    {
        channel->fxmem = mem - 1;
    }
    else // New
    {
        channel->bflags ^= MCH_BFLAGS_TREMOR | MCH_BFLAGS_CUT_VOLUME;

        if (channel->bflags & MCH_BFLAGS_CUT_VOLUME)
            channel->fxmem = (param >> 4) + 1;
        else
            channel->fxmem = (param & 0xF) + 1;
    }

    if ((channel->bflags & MCH_BFLAGS_CUT_VOLUME) == 0) // Cut note
        mpp_vars.volplus = -64;
}

// Process pattern effect
mm_word mpp_Process_Effect(mpl_layer_information *layer, mm_active_channel *act_ch,
                           mm_module_channel *channel, mm_word period)
{
    // First, update effect. If "channel->param" is zero, the function will
    // return the last parameter provided for the effect specified in
    // "channel->effect". Only some effects have memory. If the effect doesn't
    // have memory this function will return "channel->param" right away.
    mm_word param = mpp_Channel_ExchangeMemory(channel->effect, channel->param, channel, layer);

    mm_word effect = channel->effect;

    switch (effect)
    {
        case 0:
            // No effect
            return period;

        case 1:
            mppe_SetSpeed(param, layer);
            return period;

        case 2:
            mppe_PositionJump(param, layer);
            return period;

        case 3:
            mppe_PatternBreak(param, layer);
            return period;

        case 4:
            mppe_VolumeSlide(param, channel, layer);
            return period;

        case 5:
        case 6:
            return mppe_Portamento(param, period, channel, layer);

        case 7:
            return mppe_Glissando(param, period, act_ch, channel, layer);

        case 8:
            return mppe_Vibrato(param, period, channel, layer);

        case 9: // Tremor
            // TODO: This isn't implemented. Would it work with the OldTremor code?
            return period;

        case 10:
            return mppe_Arpeggio(param, period, act_ch, channel, layer);

        case 11:
            return mppe_VibratoVolume(param, period, channel, layer);

        case 12:
            return mppe_PortaVolume(param, period, act_ch, channel, layer);

        case 13:
            mppe_ChannelVolume(param, channel, layer);
            return period;

        case 14:
            mppe_ChannelVolumeSlide(param, channel, layer);
            return period;

        case 15:
            mppe_SampleOffset(param, layer);
            return period;

        case 16: // Panning slide
            // TODO
            //mppe_PanningSlide(param, channel, layer);
            return period;

        case 17:
            mppe_Retrigger(param, act_ch, channel);
            return period;

        case 18:
            mppe_Tremolo(param, channel, layer);
            return period;

        case 19:
            mppe_Extended(param, act_ch, channel, layer);
            return period;

        case 20:
            mppe_SetTempo(param, layer);
            return period;

        case 21:
            return mppe_FineVibrato(param, period, channel, layer);

        case 22:
            mppe_SetGlobalVolume(param, layer);
            return period;

        case 23:
            mppe_GlobalVolumeSlide(param, layer);
            return period;

        case 24:
            mppe_SetPanning(param, channel, layer);
            return period;

        case 25: // Panbrello
            // TODO
            return period;

        case 26: // Set Filter
            // TODO: Not supported
            return period;

        case 27:
            mppe_SetVolume(param, channel, layer);
            return period;

        case 28:
            mppe_KeyOff(param, act_ch, layer);
            return period;

        case 29: // Envelope Pos
            // TODO
            //mppe_EnvelopePos(param, act_ch, layer);
            return period;

        case 30:
            mppe_OldTremor(param, channel, layer);
            return period;

        default: // Effect not implemented
            return period;
    }
}

// =============================================================================
// =============================================================================

// TODO: What do the defines actually mean?
#define PE_FADE_ALLOWED         2
#define PE_1                    1
#define PE_FADE_NOT_ALLOWED     0

// Processes the provided envelope. It updates the values pointed by "count_",
// "node_" and "value_mul_64".
static
mm_word mpph_ProcessEnvelope(mm_hword *count_, mm_byte *node_, mm_mas_envelope *envelope,
                             mm_active_channel *act_ch, mm_word *value_mul_64)
{
    mm_hword count = *count_;
    mm_byte node = *node_;

    // Get node and base
    mm_mas_envelope_node *node_info = &(envelope->env_nodes[node]);

    *value_mul_64 = node_info->base * 64;

    if (count == 0) // Process a node that has just been reached
    {
        // Process envelope loop

        if (node == envelope->loop_end)
        {
            *count_ = count;
            *node_ = envelope->loop_start;
            return PE_FADE_ALLOWED;
        }

        // Process envelope sustain loop

        if (act_ch->flags & MCAF_KEYON)
        {
            if (node == envelope->sus_end)
            {
                *count_ = count;
                *node_ = envelope->sus_start;
                return PE_FADE_NOT_ALLOWED;
            }
        }

        // Check for end

        mm_hword last_node = (mm_hword)envelope->node_count - 1;
        if (node == last_node)
        {
            *count_ = count;
            *node_ = node;
            return PE_FADE_ALLOWED;
        }
    }
    else // If we're between nodes, interpolate the value of the two nodes
    {
        //                           delta * count
        // formula : y = base * 64 + -------------
        //                                8

        mm_sword delta = node_info->delta;

        *value_mul_64 += ((mm_sword)(delta * count)) >> 3;
    }

    // Increment count and check if == read count

    count++;

    if (count == node_info->range)
    {
        // Increment node and reset counter
        count = 0;
        node = node + 1;
    }

    *count_ = count;
    *node_ = node;
    return PE_FADE_ALLOWED; // TODO: This was undefined in the ASM version!
}

static mm_word mpp_Update_ACHN_notest_envelopes(mpl_layer_information *layer,
                                                mm_active_channel *act_ch, mm_word period)
{
    mm_mas_instrument *instrument = mpp_InstrumentPointer(layer, act_ch->inst);

    // Get envelope flags

    mm_byte *env_ptr = &(instrument->data[0]);

    if (instrument->env_flags & MAS_INSTR_FLAG_VOL_ENV_EXISTS)
    {
        if ((act_ch->flags & MCAF_VOLENV) == 0)
        {
            // Volume envelope not enabled
            env_ptr += env_ptr[0];
        }
        else
        {
            // Volume envelope enabled
            mm_word value_mul_64;
            mm_mas_envelope *env = (mm_mas_envelope *)env_ptr;

            env_ptr += env_ptr[0];

            mm_word exit_value = mpph_ProcessEnvelope(&act_ch->envc_vol, &act_ch->envn_vol,
                                                      env, act_ch, &value_mul_64);

            if (exit_value == PE_1)
            {
                // TODO: It looks like this condition is never met

                // XM doesn't fade out at envelope end, IT does.
                if (layer->flags & MAS_HEADER_FLAG_XM_MODE)
                    act_ch->flags |= MCAF_ENVEND;
                else
                    act_ch->flags |= MCAF_ENVEND | MCAF_FADE;
            }
            else if (exit_value == PE_FADE_ALLOWED)
            {
                // Check keyon and turn on fade...
                if ((act_ch->flags & MCAF_KEYON) == 0)
                    act_ch->flags |= MCAF_FADE;
            }

            mm_sword afvol = mpp_vars.afvol;
            mpp_vars.afvol = (afvol * value_mul_64) >> (6 + 6);

            goto mppt_has_volenv;
        }
    }

// mppt_no_volenv

    if ((act_ch->flags & MCAF_KEYON) == 0)
    {
        act_ch->flags |= MCAF_FADE | MCAF_ENVEND;

        // Check XM MODE and cut note
        if (layer->flags & MAS_HEADER_FLAG_XM_MODE)
            act_ch->fade = 0;
    }

mppt_has_volenv:

    if (instrument->env_flags & MAS_INSTR_FLAG_PAN_ENV_EXISTS)
    {
        mm_word value_mul_64;
        mm_mas_envelope *env = (mm_mas_envelope *)env_ptr;

        env_ptr += env_ptr[0];

        mpph_ProcessEnvelope(&act_ch->envc_pan, &act_ch->envn_pan, env, act_ch, &value_mul_64);

        mpp_vars.panplus += (value_mul_64 >> 4) - 128;
    }

    if (instrument->env_flags & MAS_INSTR_FLAG_PITCH_ENV_EXISTS)
    {
        mm_word value_mul_64;
        mm_mas_envelope *env = (mm_mas_envelope *)env_ptr;

        if (env->is_filter == 0)
        {
            mpph_ProcessEnvelope(&act_ch->envc_pic, &act_ch->envn_pic, env, act_ch, &value_mul_64);

            mm_sword value = (value_mul_64 >> 3) - 256;

            if (value < 0)
                period = mpph_LinearPitchSlide_Down(period, -value, layer);
            else
                period = mpph_LinearPitchSlide_Up(period, value, layer);
        }
    }

    if (act_ch->flags & MCAF_FADE)
    {
        mm_mas_instrument *inst = mpp_InstrumentPointer(layer, act_ch->inst);

        mm_sword value = act_ch->fade - inst->fadeout;
        if (value < 0)
            value = 0;

        act_ch->fade = value;
    }

    // mppt_achn_keyon

    return period;
}

static mm_word mpp_Update_ACHN_notest_auto_vibrato(mpl_layer_information *layer,
                                                   mm_active_channel *act_ch, mm_word period)
{
    mm_mas_sample_info *sample = mpp_SamplePointer(layer, act_ch->sample);

    // Get av-rate, check if auto vibrato is enabled
    mm_hword av_rate = sample->av_rate;
    if (av_rate != 0)
    {
        // Handle depth counter
        mm_word new_rate = act_ch->avib_dep + av_rate;
        if (new_rate > 32768) // Clamp
            new_rate = 32768;
        act_ch->avib_dep = new_rate;

        // Get av-depth
        mm_sword new_depth = sample->av_depth * new_rate;

        // Add av-speed to table position and wrap to 0->255
        act_ch->avib_pos = (act_ch->avib_pos + sample->av_speed) & 0xFF;

        // Load table value at the current position
        mm_sword slide_val = mpp_TABLE_FineSineData[act_ch->avib_pos];

        slide_val = (slide_val * new_depth) >> 23;

        // Perform slide
        if (slide_val >= 0)
            period = mpph_PitchSlide_Up(period, slide_val, layer);
        else
            period = mpph_PitchSlide_Down(period, -slide_val, layer);
    }

    return period;
}

static mm_mixer_channel *mpp_Update_ACHN_notest_update_mix(mpl_layer_information *layer,
                                                           mm_active_channel *act_ch,
                                                           mm_word channel)
{
    mm_mixer_channel *mix_ch = &mm_mix_channels[channel];

    // Update mixing information

    if ((act_ch->flags & MCAF_START) == 0)
        goto mppt_achn_nostart;

    // Start note

    act_ch->flags &= ~MCAF_START;

    // Get sample number. Quit if invalid.
    if (act_ch->sample == 0)
        goto mppt_achn_nostart;

    mm_mas_sample_info *sample = mpp_SamplePointer(layer, act_ch->sample);

    if (sample->msl_id == 0xFFFF)
    {
        // The sample has been provided
#ifdef __GBA__
        mm_mas_gba_sample *gba_sample = (mm_mas_gba_sample *)&(sample->data[0]);

        mix_ch->src = (uintptr_t)&(gba_sample->data[0]);
#else
        mm_mas_ds_sample *ds_sample = (mm_mas_ds_sample *)&(sample->data[0]);

        mix_ch->samp = ((mm_word)ds_sample) - 0x2000000;
        mix_ch->tpan = 0;
        mix_ch->key_on = 1;
#endif
    }
    else
    {
        // Get sample from solution
#ifdef __GBA__
        msl_head *head = mp_solution;
        mm_word sample_offset = (mm_word)head->sampleTable[sample->msl_id];

        mm_byte *sample_addr = ((mm_byte *)mp_solution) + sample_offset;
        mm_mas_gba_sample *gba_sample = (mm_mas_gba_sample *)(sample_addr + sizeof(mm_mas_prefix));

        mix_ch->src = (uintptr_t)(&(gba_sample->data[0]));
#else
        mm_word source = mmSampleBank[sample->msl_id];
        source &= 0xFFFFFF; // Mask out counter value

        // The pointers in mmSampleBank are pointers to MAS file objects. We
        // need to skip the header of the MAS file to get the pointer to the
        // sample data. Normally there is more data after the header, but a MAS
        // file generated from a WAV file only has the header and a sample.
        source += sizeof(mm_mas_prefix);

        mix_ch->samp = source;
        mix_ch->tpan = 0;
        mix_ch->key_on = 1;
#endif
    }

#ifdef __GBA__
    mix_ch->read = ((mm_word)mpp_vars.sampoff) << (MP_SAMPFRAC + 8);
#else
    mix_ch->read = mpp_vars.sampoff;
#endif

mppt_achn_nostart:

    return mix_ch;
}

// This returns the resulting volume of the channel
static mm_word mpp_Update_ACHN_notest_set_pitch_volume(mpl_layer_information *layer,
                                                       mm_active_channel *act_ch, mm_word period,
                                                       mm_mixer_channel *mix_ch)
{
    // Set pitch
    // ---------

    if (act_ch->sample == 0) // Check sample number
    {
        act_ch->fvol = 0; // Mute channel if invalid sample
        return 0;
    }

    mm_mas_sample_info *sample = mpp_SamplePointer(layer, act_ch->sample);

    if (layer->flags & MAS_HEADER_FLAG_FREQ_MODE)
    {
        // Linear frequencies

        mm_hword speed = sample->frequency; // Get C5SPEED

        mm_word value = ((period >> 8) * (speed << 2)) >> 8;

        if (mpp_clayer == MM_MAIN)
            value = (value * mm_masterpitch) >> 10;

#ifdef __GBA__
        const mm_word scale = (4096 * 65536) / 15768;
        mix_ch->freq = (scale * value) >> 16;
#else
        mix_ch->freq = (MIXER_SCALE * value) >> (16 + 1);
#endif
    }
    else
    {
        // Amiga frequencies

        if (period != 0) // Avoid divisions by zero
        {
            mm_word value = MOD_FREQ_DIVIDER_PAL / period;

            if (mpp_clayer == MM_MAIN)
                value = (value * mm_masterpitch) >> 10;

#ifdef __GBA__
            const mm_word scale = (4096 * 65536) / 15768;
            mix_ch->freq = (scale * value) >> 16;
#else
            mix_ch->freq = (MIXER_SCALE * value) >> (16 + 1);
#endif
        }
    }

    // Set volume
    // ----------

    if (act_ch->inst == 0)
    {
        act_ch->fvol = 0; // Mute channel if invalid instrument
        return 0;
    }

    mm_mas_instrument *inst = mpp_InstrumentPointer(layer, act_ch->inst);

    mm_word vol = sample->global_volume; // SV, 6-bit

    // Set volume
    vol *= inst->global_volume; // IV, 7-bit
    vol *= mpp_vars.afvol; // ((CV * VOL) / 32 * VEV / 64) 7-bit

    // Get global volume
    mm_byte global_volume = layer->global_volume;
    if (layer->flags & MAS_HEADER_FLAG_XM_MODE)
        global_volume <<= 1; // XM mode global volume is only 0->64, shift to 0->128
    vol = (vol * global_volume) >> 10;

    vol = (vol * act_ch->fade) >> 10;

    vol *= layer->volume;

#ifdef __NDS__
    vol = vol >> (19 - 3 - 5);  // (19 - 3) (new 16-bit levels!)

    if (vol > 65535) // Clip values over 65535 // 2047
        vol = 65535;

    act_ch->fvol = vol >> 8; // 3 (new 16-bit levels!)
#else
    vol = vol >> 19;

    if (vol > 255) // Clip values over 255
        vol = 255;

    act_ch->fvol = vol;
#endif

    return vol;
}

static void mpp_Update_ACHN_notest_disable_and_panning(mm_word volume, mm_active_channel *act_ch,
                                                       mm_mixer_channel *mix_ch)
{
    if (volume != 0)
        goto mppt_achn_audible;

#ifdef __NDS__ // NDS has volume ramping!
    if (act_ch->type == ACHN_BACKGROUND)
    {
        if (act_ch->volume == 0)
        {
            if (mix_ch->cvol == 0)
                goto mppt_achn_not_audible;
        }
    }
#endif

    if ((act_ch->flags & MCAF_ENVEND) == 0)
        goto mppt_achn_audible;

    if (act_ch->flags & MCAF_KEYON)
        goto mppt_achn_audible;

#ifdef __NDS__
mppt_achn_not_audible:
#endif

    // Stop channel
    // ------------

#ifdef __GBA__
    mix_ch->src = MIXCH_GBA_SRC_STOPPED;
#else
    mix_ch->samp = 0;
    mix_ch->tpan = 0;
    mix_ch->key_on = 0;
#endif

    if (act_ch->type == ACHN_FOREGROUND)
    {
        // Has parent
        mpp_channels[act_ch->parent].alloc = NO_CHANNEL_AVAILABLE;
    }

    act_ch->type = ACHN_DISABLED;
    return;

mppt_achn_audible:

    mix_ch->vol = volume;

    // Check if mixer channel has ended
#ifdef __GBA__
    if (mix_ch->src & MIXCH_GBA_SRC_STOPPED)
    {
#else
    if (mix_ch->samp == 0)
    {
#endif
        if (act_ch->type == ACHN_FOREGROUND)
        {
            // Stop channel if channel ended
            mpp_channels[act_ch->parent].alloc = NO_CHANNEL_AVAILABLE;
        }

        // TODO: This isn't required because we've just checked if the mixer
        // channel is stopped
        // Stop mixer channel
#ifdef __GBA__
        mix_ch->src = MIXCH_GBA_SRC_STOPPED;
#else
        mix_ch->samp = 0;
        mix_ch->tpan = 0;
        mix_ch->key_on = 0;
#endif

        act_ch->type = ACHN_DISABLED;
        return;
    }

    // Set panning

    mm_shword panplus = mpp_vars.panplus;
    mm_word old_panning = act_ch->panning;

    mm_sword newpan = old_panning + panplus;
    if (newpan < 0)
        newpan = 0;
    else if (newpan > 255)
        newpan = 255;

#ifdef __NDS__
    mix_ch->tpan = newpan >> 1;
#endif

#ifdef __GBA__
    mix_ch->pan = newpan;
#endif

    return;
}

mm_word mpp_Update_ACHN_notest(mpl_layer_information *layer, mm_active_channel *act_ch,
                               mm_word period, mm_word ch)
{
    // TODO: This variable was left uninitialized in the original assembly code,
    // so this was the actual result of that code.
    mm_mixer_channel *mix_ch = (mm_mixer_channel *)ch;

    // ------------------------------------------------------------------------
    // Process Envelope
    // ------------------------------------------------------------------------

    if (act_ch->inst == 0)
        goto mppt_achn_noinst;

    period = mpp_Update_ACHN_notest_envelopes(layer, act_ch, period);

    // ------------------------------------------------------------------------
    // Process Auto Vibrato
    // ------------------------------------------------------------------------

    if (act_ch->sample == 0) // No sample
        goto mppt_achn_nostart_;

    period = mpp_Update_ACHN_notest_auto_vibrato(layer, act_ch, period);

    // ------------------------------------------------------------------------

mppt_achn_noinst:

    mix_ch = mpp_Update_ACHN_notest_update_mix(layer, act_ch, ch);

mppt_achn_nostart_:

    mm_word volume = mpp_Update_ACHN_notest_set_pitch_volume(layer, act_ch, period, mix_ch);

    mpp_Update_ACHN_notest_disable_and_panning(volume, act_ch, mix_ch);

    return period;
}

static void mpp_Update_ACHN(mpl_layer_information *layer, mm_active_channel *act_ch,
                            mm_word period, mm_word ch)
{
    if (act_ch->flags & MCAF_UPDATED)
        return;

    mpp_Update_ACHN_notest(layer, act_ch, period, ch);
}
