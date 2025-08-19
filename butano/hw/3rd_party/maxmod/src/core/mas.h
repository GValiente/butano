// SPDX-License-Identifier: ISC
//
// Copyright (c) 2008, Mukunda Johnson (mukunda@maxmod.org)
// Copyright (c) 2021-2025, Antonio Niño Díaz (antonio_nd@outlook.com)

#ifndef MM_CORE_MAS_H__
#define MM_CORE_MAS_H__

#include <mm_types.h>
#include <mm_mas.h>

#include "channel_types.h"
#include "player_types.h"

// This is the number of channels for the sub layer (used for jingles)
#define MP_SCHANNELS    4

// Returned by mmAllocChannel() if there are no channels available
#define NO_CHANNEL_AVAILABLE        255

extern mm_word mm_ch_mask;

extern mpl_layer_information mmLayerMain;
extern mpl_layer_information mmLayerSub;

extern mpl_layer_information *mpp_layerp;
extern mpv_active_information mpp_vars;
extern mm_module_channel *mpp_channels;

extern mm_byte mpp_nchannels;
extern mm_layer_type mpp_clayer;

extern mm_active_channel *mm_achannels;
extern mm_module_channel *mm_pchannels;
extern mm_word mm_num_mch;
extern mm_word mm_num_ach;
extern mm_module_channel mm_schannels[MP_SCHANNELS];

void mmSetEventHandler(mm_callback);

void mmSetResolution(mm_word);
void mmPulse(void);
void mppUpdateSub(void);
void mppProcessTick(void);

mm_word mmAllocChannel(void);
void mmUpdateChannel_T0(mm_module_channel*, mpl_layer_information*, mm_byte);
void mmUpdateChannel_TN(mm_module_channel*, mpl_layer_information*);
mm_word mmGetPeriod(mpl_layer_information*, mm_word, mm_byte);
mm_bool mmReadPattern(mpl_layer_information*);

mm_word mpp_Process_VolumeCommand(mpl_layer_information*, mm_active_channel*, mm_module_channel*, mm_word);

mm_word mpp_Process_Effect(mpl_layer_information*, mm_active_channel*, mm_module_channel*, mm_word);

mm_word mpp_Update_ACHN_notest(mpl_layer_information *layer, mm_active_channel *act_ch,
                               mm_word period, mm_word ch);

void mpp_Channel_NewNote(mm_module_channel*, mpl_layer_information*);


static inline
mm_mas_sample_info *mpp_SamplePointer(mpl_layer_information *layer, mm_word sampleN)
{
    mm_byte *base = (mm_byte *)layer->songadr;
    return (mm_mas_sample_info *)(base + layer->samptable[sampleN - 1]);
}

static inline
mm_mas_instrument *mpp_InstrumentPointer(mpl_layer_information *layer, mm_word instN)
{
    mm_byte *base = (mm_byte *)layer->songadr;
    return (mm_mas_instrument*)(base + layer->insttable[instN - 1]);
}

static inline
mm_mas_pattern *mpp_PatternPointer(mpl_layer_information *layer, mm_word entry)
{
    mm_byte *base = (mm_byte *)layer->songadr;
    return (mm_mas_pattern *)(base + layer->patttable[entry]);
}

#endif // MM_CORE_MAS_H__
