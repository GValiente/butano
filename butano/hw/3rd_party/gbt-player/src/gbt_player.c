// GBT Player v4.1.1
//
// SPDX-License-Identifier: MIT
//
// Copyright (c) 2009-2022 Antonio Niño Díaz <antonio_nd@outlook.com>

#include <stdint.h>

#ifdef GBT_USE_LIBUGBA
# include <ugba/ugba.h>
#else
# include "gbt_hardware.h"
#endif

#include "../include/gbt_player.h"

typedef int (*effect_handler)(uint32_t args);

typedef struct {

    // Array of commands to run before the start of the song (or NULL)
    const uint8_t *startup_cmds_ptr;

    // Pointer to the pattern pointer array
    uint8_t * const *pattern_array_ptr;

    // Pointer to next step data
    const uint8_t *current_step_data_ptr;

    uint8_t playing;
    uint8_t loop_enabled;

    uint8_t speed;

    uint8_t ticks_elapsed;
    uint8_t current_step;
    uint8_t current_pattern;

    uint8_t channels_enabled;

    uint16_t pan[4]; // Ch 1-4
    uint16_t vol[4]; // Ch 1-4
    uint16_t instr[4]; // Ch 1-4
    uint16_t freq[3]; // Ch 1-3 | Active frequence
    uint16_t base_freq[3]; // Ch 1-3 | Original frequence read from the pattern

    // The volume setting in REG_SOUNDCNT_L can't actually silence the music.
    // The best it can do is set it to 1/8th of the maximum volume. To be able
    // to reach zero, it is needed to use the flags that enable output to the
    // left and right speakers.
    uint16_t global_volume;
    uint16_t pan_volume_mask;

    // Currently loaded instrument (0xFF if none)
    uint8_t channel3_loaded_instrument;

    // Arpeggio -> Ch 1-3
    uint8_t arpeggio_freq_index[3][3]; // { base_index, index + x, index + y }
    uint8_t arpeggio_enabled[3]; // if 0, disabled
    uint8_t arpeggio_tick[3];

    // Vibrato
    uint8_t vibrato_enabled[3]; // if 0, disabled
    uint8_t vibrato_position[3];
    uint8_t vibrato_args[3];

    // Volume slide
    uint16_t volslide_args[3];

    // Cut note
    uint8_t cut_note_tick[4]; // If tick == gbt.cut_note_tick, stop note.

    uint8_t jump_requested; // set to 1 by jump effects
    uint8_t jump_target_step;
    uint8_t jump_target_pattern;

    // Channel 3 instruments
    uint8_t ch3_instrument_flags[8];
    const uint8_t *ch3_instrument[8];

} gbt_player_info_t;

EWRAM_BSS static gbt_player_info_t gbt;

// Waveforms of the default channel 3 instruments

static const uint8_t gbt_default_wave_0[16] = { // random
    0xA5, 0xD7, 0xC9, 0xE1, 0xBC, 0x9A, 0x76, 0x31,
    0x0C, 0xBA, 0xDE, 0x60, 0x1B, 0xCA, 0x03, 0x93
};
static const uint8_t gbt_default_wave_1[16] = {
    0xF0, 0xE1, 0xD2, 0xC3, 0xB4, 0xA5, 0x96, 0x87,
    0x78, 0x69, 0x5A, 0x4B, 0x3C, 0x2D, 0x1E, 0x0F
};
static const uint8_t gbt_default_wave_2[16] = { // up-downs
    0xFD, 0xEC, 0xDB, 0xCA, 0xB9, 0xA8, 0x97, 0x86,
    0x79, 0x68, 0x57, 0x46, 0x35, 0x24, 0x13, 0x02
};
static const uint8_t gbt_default_wave_3[16] = {
    0xDE, 0xFE, 0xDC, 0xBA, 0x9A, 0xA9, 0x87, 0x77,
    0x88, 0x87, 0x65, 0x56, 0x54, 0x32, 0x10, 0x12
};
static const uint8_t gbt_default_wave_4[16] = { // triangular broken
    0xAB, 0xCD, 0xEF, 0xED, 0xCB, 0xA0, 0x12, 0x3E,
    0xDC, 0xBA, 0xBC, 0xDE, 0xFE, 0xDC, 0x32, 0x10
};
static const uint8_t gbt_default_wave_5[16] = { // triangular
    0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88,
    0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00
};
static const uint8_t gbt_default_wave_6[16] = { // square
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t gbt_default_wave_7[16] = { // sine
    0x79, 0xBC, 0xDE, 0xEF, 0xFF, 0xEE, 0xDC, 0xB9,
    0x75, 0x43, 0x21, 0x10, 0x00, 0x11, 0x23, 0x45
};

// Parameters of the channel 4 instruments

static const uint8_t gbt_noise[16] = { // 16 sounds
    // 7 bit
    0x5F, 0x5B, 0x4B, 0x2F, 0x3B, 0x58, 0x1F, 0x0F,
    // 15 bit
    0x90, 0x80, 0x70, 0x50, 0x00,
    0x67, 0x63, 0x53
};

// Sine wave used for vibrato effect

static const int16_t vibrato_sine[64] = {
       0,   24,   49,   74,   97,  120,  141,  161,
     180,  197,  212,  224,  235,  244,  250,  253,
     255,  253,  250,  244,  235,  224,  212,  197,
     180,  161,  141,  120,   97,   74,   49,   24,
       0,  -24,  -49,  -74,  -97, -120, -141, -161,
    -180, -197, -212, -224, -235, -244, -250, -253,
    -255, -253, -250, -244, -235, -224, -212, -197,
    -180, -161, -141, -120,  -97,  -74,  -49,  -24,
};

static void gbt_get_pattern_ptr(int pattern_number)
{
    gbt.current_step_data_ptr = gbt.pattern_array_ptr[pattern_number];
}

static void gbt_run_startup_commands(const uint8_t *ptr)
{
    if (ptr == NULL)
        return;

    while (1)
    {
        uint8_t cmd = *ptr++;

        if (cmd == 0) // End setup
        {
            break;
        }
        else if (cmd == 1) // Initial speed
        {
            gbt.speed = *ptr++;
        }
        else if (cmd == 2) // Initial panning
        {
            gbt.pan[0] = *ptr++;
            gbt.pan[1] = *ptr++;
            gbt.pan[2] = *ptr++;
            gbt.pan[3] = *ptr++;
        }
        else if (cmd == 3) // Set channel 3 sample
        {
            uint8_t flags = *ptr++;
            uint8_t index = flags & 0x3F;
            gbt.ch3_instrument_flags[index] = flags;

            gbt.ch3_instrument[index] = ptr;

            uint32_t len = 32 / 2;
            if (flags & BIT(7))
                len = 64 / 2;

            ptr += len;
        }
    }
}

void gbt_play(const void *song, int speed)
{
    // If the user hasn't specified a speed, set the default value

    if (speed <= 0)
        speed = 6;

    // Load pointers to startup commands and to list of patterns

    uint8_t * const *ptr = song;

    gbt.startup_cmds_ptr = ptr[0];
    gbt.pattern_array_ptr = ptr;
    gbt.pattern_array_ptr++;

    // Initialize player state

    gbt.speed = speed;

    gbt_get_pattern_ptr(0);

    gbt.current_step = 0;
    gbt.current_pattern = 0;
    gbt.ticks_elapsed = 0;
    gbt.loop_enabled = 0;
    gbt.jump_requested = 0;

    // Force refresh as soon as possible
    gbt.ticks_elapsed = gbt.speed - 1;

    gbt.channel3_loaded_instrument = 0xFF;

    gbt.channels_enabled = GBT_ENABLE_CH_ALL;

    gbt.pan[0] = 0x11; // L and R
    gbt.pan[1] = 0x22;
    gbt.pan[2] = 0x44;
    gbt.pan[3] = 0x88;

    gbt.vol[0] = 0xF000; // 100%
    gbt.vol[1] = 0xF000;
    gbt.vol[2] = 0x2000;
    gbt.vol[3] = 0xF000;

    gbt.instr[0] = 0;
    gbt.instr[1] = 0;
    gbt.instr[2] = 0;
    gbt.instr[3] = 0;

    gbt.freq[0] = 0;
    gbt.freq[1] = 0;
    gbt.freq[2] = 0;

    gbt.arpeggio_enabled[0] = 0;
    gbt.arpeggio_enabled[1] = 0;
    gbt.arpeggio_enabled[2] = 0;

    gbt.vibrato_enabled[0] = 0;
    gbt.vibrato_enabled[1] = 0;
    gbt.vibrato_enabled[2] = 0;

    gbt.vibrato_position[0] = 0;
    gbt.vibrato_position[1] = 0;
    gbt.vibrato_position[2] = 0;

    gbt.vibrato_args[0] = 0;
    gbt.vibrato_args[1] = 0;
    gbt.vibrato_args[2] = 0;

    gbt.volslide_args[0] = 0;
    gbt.volslide_args[1] = 0;
    gbt.volslide_args[2] = 0;

    gbt.cut_note_tick[0] = 0xFF;
    gbt.cut_note_tick[1] = 0xFF;
    gbt.cut_note_tick[2] = 0xFF;
    gbt.cut_note_tick[3] = 0xFF;

    // Default channel 3 instruments

    gbt.ch3_instrument_flags[0] = 0;
    gbt.ch3_instrument_flags[1] = 0;
    gbt.ch3_instrument_flags[2] = 0;
    gbt.ch3_instrument_flags[3] = 0;
    gbt.ch3_instrument_flags[4] = 0;
    gbt.ch3_instrument_flags[5] = 0;
    gbt.ch3_instrument_flags[6] = 0;
    gbt.ch3_instrument_flags[7] = 0;
    gbt.ch3_instrument[0] = &gbt_default_wave_0[0];
    gbt.ch3_instrument[1] = &gbt_default_wave_1[0];
    gbt.ch3_instrument[2] = &gbt_default_wave_2[0];
    gbt.ch3_instrument[3] = &gbt_default_wave_3[0];
    gbt.ch3_instrument[4] = &gbt_default_wave_4[0];
    gbt.ch3_instrument[5] = &gbt_default_wave_5[0];
    gbt.ch3_instrument[6] = &gbt_default_wave_6[0];
    gbt.ch3_instrument[7] = &gbt_default_wave_7[0];

    // Run startup commands after internal player status has been initialized

    gbt_run_startup_commands(gbt.startup_cmds_ptr);

    // Initialize hardware registers

    REG_SOUNDCNT_X = SOUNDCNT_X_MASTER_ENABLE;

    REG_SOUNDCNT_L = 0;

    REG_SOUND1CNT_L = 0;
    REG_SOUND1CNT_H = 0;
    REG_SOUND1CNT_X = SOUND1CNT_X_RESTART;

    REG_SOUND2CNT_L = 0;
    REG_SOUND2CNT_H = SOUND2CNT_H_RESTART;

    REG_SOUND3CNT_L = 0;
    REG_SOUND3CNT_H = 0;
    REG_SOUND3CNT_X = SOUND3CNT_X_RESTART;

    REG_SOUND4CNT_L = 0;
    REG_SOUND4CNT_H = SOUND4CNT_H_RESTART;

    gbt_volume(GBT_VOLUME_MAX, GBT_VOLUME_MAX);

    // Everything is ready

    gbt.playing = 1;
}

void gbt_volume(unsigned int left, unsigned int right)
{
    const uint16_t left_mask =
        SOUNDCNT_L_PSG_1_ENABLE_LEFT | SOUNDCNT_L_PSG_2_ENABLE_LEFT |
        SOUNDCNT_L_PSG_3_ENABLE_LEFT | SOUNDCNT_L_PSG_4_ENABLE_LEFT;
    const uint16_t right_mask =
        SOUNDCNT_L_PSG_1_ENABLE_RIGHT | SOUNDCNT_L_PSG_2_ENABLE_RIGHT |
        SOUNDCNT_L_PSG_3_ENABLE_RIGHT | SOUNDCNT_L_PSG_4_ENABLE_RIGHT;

    gbt.pan_volume_mask = 0;
    gbt.global_volume = 0;

    if (left > 8)
        left = 8;

    if (right > 8)
        right = 8;

    if (left > 0)
    {
        gbt.global_volume |= SOUNDCNT_L_PSG_VOL_LEFT_SET(left - 1);
        gbt.pan_volume_mask |= left_mask;
    }

    if (right > 0)
    {
        gbt.global_volume |= SOUNDCNT_L_PSG_VOL_RIGHT_SET(right - 1);
        gbt.pan_volume_mask |= right_mask;
    }
}

void gbt_pause(int play)
{
    gbt.playing = play;

    uint16_t mask =
        SOUNDCNT_L_PSG_1_ENABLE_RIGHT | SOUNDCNT_L_PSG_1_ENABLE_LEFT |
        SOUNDCNT_L_PSG_2_ENABLE_RIGHT | SOUNDCNT_L_PSG_2_ENABLE_LEFT |
        SOUNDCNT_L_PSG_3_ENABLE_RIGHT | SOUNDCNT_L_PSG_3_ENABLE_LEFT |
        SOUNDCNT_L_PSG_4_ENABLE_RIGHT | SOUNDCNT_L_PSG_4_ENABLE_LEFT;

    if (play)
    {
        // Unmute sound if playback is resumed
        uint16_t new_pan = gbt.pan[0] | gbt.pan[1] | gbt.pan[2] | gbt.pan[3];
        REG_SOUNDCNT_L = (REG_SOUNDCNT_L & ~mask) | (new_pan << 8);
    }
    else
    {
        REG_SOUNDCNT_L &= ~mask;
    }
}

void gbt_loop(int loop)
{
    gbt.loop_enabled = loop;
}

void gbt_stop(void)
{
    gbt.playing = 0;
    REG_SOUNDCNT_L = 0; // Disable all PSG channels
    // REG_SOUNDCNT_X = SOUNDCNT_X_MASTER_DISABLE; // Don't do this!
}

void gbt_enable_channels(int flags)
{
    gbt.channels_enabled = flags;
}

static uint16_t _gbt_get_freq_from_index(int index)
{
    const uint16_t gbt_frequencies[] = {
        44,  156,  262,  363,  457,  547,  631,  710,  786,  854,  923,  986,
        1046, 1102, 1155, 1205, 1253, 1297, 1339, 1379, 1417, 1452, 1486, 1517,
        1546, 1575, 1602, 1627, 1650, 1673, 1694, 1714, 1732, 1750, 1767, 1783,
        1798, 1812, 1825, 1837, 1849, 1860, 1871, 1881, 1890, 1899, 1907, 1915,
        1923, 1930, 1936, 1943, 1949, 1954, 1959, 1964, 1969, 1974, 1978, 1982,
        1985, 1988, 1992, 1995, 1998, 2001, 2004, 2006, 2009, 2011, 2013, 2015
    };

    return gbt_frequencies[index];
}

static int gbt_ch1234_nop(uint32_t args)
{
    (void)args;
    return 0;
}

static int gbt_ch1234_speed(uint32_t args)
{
    gbt.speed = args;
    gbt.ticks_elapsed = 0;
    return 0;
}

static int gbt_ch1234_jump_pattern(uint32_t args)
{
    gbt.jump_requested = 1;
    gbt.jump_target_step = 0;
    gbt.jump_target_pattern = args;

    return 0;
}

static int gbt_ch1234_jump_position(uint32_t args)
{
    gbt.jump_requested = 1;
    gbt.jump_target_step = args;
    gbt.jump_target_pattern = gbt.current_pattern + 1;

    return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

#define HAS_VOLUME      BIT(4)
#define HAS_INSTRUMENT  BIT(5)
#define HAS_EFFECT      BIT(6)
#define HAS_NOTE        BIT(7)
#define HAS_KIT         BIT(7)

// -----------------------------------------------------------------------------
// --------------------------------- Channel 1 ---------------------------------
// -----------------------------------------------------------------------------

static int gbt_ch1_pan(uint32_t args)
{
    gbt.pan[0] = args & 0x11;
    return 0; // Panning is always updated
}

static int gbt_ch1_cut_note(uint32_t args)
{
    gbt.cut_note_tick[0] = args;
    return 0;
}

static int gbt_ch1_arpeggio(uint32_t args)
{
    uint32_t base_index = gbt.arpeggio_freq_index[0][0];
    gbt.arpeggio_freq_index[0][1] = base_index + ((args >> 4) & 0xF);
    gbt.arpeggio_freq_index[0][2] = base_index + (args & 0xF);

    gbt.arpeggio_enabled[0] = 1;
    gbt.arpeggio_tick[0] = 1;

    return 1;
}

static int gbt_ch1_vibrato(uint32_t args)
{
    if (args != 0)
    {
        gbt.vibrato_position[0] = 0;
        gbt.vibrato_args[0] = args;
    }

    gbt.vibrato_enabled[0] = 1;

    return 1;
}

static int gbt_ch1_volslide(uint32_t args)
{
    gbt.volslide_args[0] = args << 8; // Move to the right location
    return 1;
}

static effect_handler gbt_ch1_jump_table[16] = {
    gbt_ch1_pan,
    gbt_ch1_arpeggio,
    gbt_ch1_cut_note,
    gbt_ch1_vibrato,
    gbt_ch1_volslide,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_jump_pattern,
    gbt_ch1234_jump_position,
    gbt_ch1234_speed,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
};

// returns 1 if needed to update registers, 0 if not
static int gbt_channel_1_set_effect(uint32_t effect, uint8_t data)
{
    return gbt_ch1_jump_table[effect](data);
}

static void channel1_refresh_registers(void)
{
    REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_1_ENABLE_RIGHT | SOUNDCNT_L_PSG_1_ENABLE_LEFT);

    REG_SOUND1CNT_L = 0;
    REG_SOUND1CNT_H = gbt.instr[0] | gbt.vol[0] | gbt.volslide_args[0];
    REG_SOUND1CNT_X = SOUND1CNT_X_RESTART | gbt.freq[0];
}

static const uint8_t *gbt_channel_1_handle(const uint8_t *data)
{
    // Calculate pointer to next channel
    // Note: The volume bit doesn't affect the final size.
    const uint8_t sizes[8] = { 1, 2, 3, 3, 2, 3, 4, 4 };
    uint8_t bits = (*data) >> 5;
    const uint8_t *next = data + sizes[bits];

    // If the channel is disabled, exit
    if ((gbt.channels_enabled & GBT_ENABLE_CH1) == 0)
    {
        return next;
    }

    uint8_t header = *data++;

    int has_volume = header & HAS_VOLUME;
    int has_instrument = header & HAS_INSTRUMENT;
    int has_effect = header & HAS_EFFECT;
    int has_note = header & HAS_NOTE;

    int has_to_update_registers = 0;
    int note_cut = 0;

    if (has_volume)
    {
        gbt.vol[0] = (header & 0xF) << 12;
        has_to_update_registers = 1;
    }

    if (has_note)
    {
        uint32_t index = *data++;
        if (index == 0xFE)
        {
            note_cut = 1;
        }
        else
        {
            gbt.arpeggio_freq_index[0][0] = index;
            gbt.base_freq[0] = _gbt_get_freq_from_index(index);
            gbt.freq[0] = gbt.base_freq[0];
            has_to_update_registers = 1;
        }
    }

    if (has_instrument)
    {
        gbt.instr[0] = (*data & 0x30) << 2;
        has_to_update_registers = 1;
    }

    if (has_effect)
    {
        uint8_t effect = (*data++) & 0x0F;
        uint8_t args = *data;
        has_to_update_registers |= gbt_channel_1_set_effect(effect, args);
    }

    if (note_cut)
    {
        REG_SOUND1CNT_H = 0; // Set volume to 0
        REG_SOUND1CNT_X = SOUND1CNT_X_RESTART;
    }
    else if (has_to_update_registers)
    {
        channel1_refresh_registers();
    }

    return next;
}

// Returns 1 if it needed to update sound registers
static int channel1_update_effects(void)
{
    int ret = 0;

    // Cut note
    // --------

    if (gbt.cut_note_tick[0] == gbt.ticks_elapsed)
    {
        gbt.cut_note_tick[0] = 0xFF; // Disable cut note

        REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_1_ENABLE_RIGHT | SOUNDCNT_L_PSG_1_ENABLE_LEFT);

        REG_SOUND1CNT_H = 0; // Set volume to 0
        REG_SOUND1CNT_X = SOUND1CNT_X_RESTART;
    }

    // Arpeggio
    // --------

    if (gbt.arpeggio_enabled[0])
    {
        uint8_t tick = gbt.arpeggio_tick[0];
        if (tick == 2)
            gbt.arpeggio_tick[0] = 0;
        else
            gbt.arpeggio_tick[0] = tick + 1;

        uint32_t index = gbt.arpeggio_freq_index[0][tick];
        gbt.freq[0] = _gbt_get_freq_from_index(index);

        ret = 1;
    }

    // Vibrato
    // -------

    if (gbt.vibrato_enabled[0])
    {
        gbt.vibrato_position[0] += (gbt.vibrato_args[0] >> 4);
        gbt.vibrato_position[0] &= 63;

        int32_t pos = gbt.vibrato_position[0];
        int32_t delta = vibrato_sine[pos];
        delta *= (int32_t)(gbt.vibrato_args[0] & 0xF);
        delta >>= 7;

        int32_t freq = (int32_t)gbt.base_freq[0] - delta;
        if (freq < 0)
            freq = 0;
        else if (freq > 0x7FF)
            freq = 0x7FF;
        gbt.freq[0] = freq;

        ret = 1;
    }

    return ret;
}

// -----------------------------------------------------------------------------
// --------------------------------- Channel 2 ---------------------------------
// -----------------------------------------------------------------------------

static int gbt_ch2_pan(uint32_t args)
{
    gbt.pan[1] = args & 0x22;
    return 0; // Panning is always updated
}

static int gbt_ch2_cut_note(uint32_t args)
{
    gbt.cut_note_tick[1] = args;
    return 0;
}

static int gbt_ch2_arpeggio(uint32_t args)
{
    uint32_t base_index = gbt.arpeggio_freq_index[1][0];
    gbt.arpeggio_freq_index[1][1] = base_index + ((args >> 4) & 0xF);
    gbt.arpeggio_freq_index[1][2] = base_index + (args & 0xF);

    gbt.arpeggio_enabled[1] = 1;
    gbt.arpeggio_tick[1] = 1;

    return 1;
}

static int gbt_ch2_vibrato(uint32_t args)
{
    if (args != 0)
    {
        gbt.vibrato_position[1] = 0;
        gbt.vibrato_args[1] = args;
    }

    gbt.vibrato_enabled[1] = 1;

    return 1;
}

static int gbt_ch2_volslide(uint32_t args)
{
    gbt.volslide_args[1] = args << 8; // Move to the right location
    return 1;
}

static effect_handler gbt_ch2_jump_table[16] = {
    gbt_ch2_pan,
    gbt_ch2_arpeggio,
    gbt_ch2_cut_note,
    gbt_ch2_vibrato,
    gbt_ch2_volslide,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_jump_pattern,
    gbt_ch1234_jump_position,
    gbt_ch1234_speed,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
};

// returns 1 if needed to update registers, 0 if not
static int gbt_channel_2_set_effect(uint32_t effect, uint8_t data)
{
    return gbt_ch2_jump_table[effect](data);
}

static void channel2_refresh_registers(void)
{
    REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_2_ENABLE_RIGHT | SOUNDCNT_L_PSG_2_ENABLE_LEFT);

    REG_SOUND2CNT_L = gbt.instr[1] | gbt.vol[1] | gbt.volslide_args[1];
    REG_SOUND2CNT_H = SOUND2CNT_H_RESTART | gbt.freq[1];
}

static const uint8_t *gbt_channel_2_handle(const uint8_t *data)
{
    // Calculate pointer to next channel
    // Note: The volume bit doesn't affect the final size.
    const uint8_t sizes[8] = { 1, 2, 3, 3, 2, 3, 4, 4 };
    uint8_t bits = (*data) >> 5;
    const uint8_t *next = data + sizes[bits];

    // If the channel is disabled, exit
    if ((gbt.channels_enabled & GBT_ENABLE_CH2) == 0)
    {
        return next;
    }

    uint8_t header = *data++;

    int has_volume = header & HAS_VOLUME;
    int has_instrument = header & HAS_INSTRUMENT;
    int has_effect = header & HAS_EFFECT;
    int has_note = header & HAS_NOTE;

    int has_to_update_registers = 0;
    int note_cut = 0;

    if (has_volume)
    {
        gbt.vol[1] = (header & 0xF) << 12;
        has_to_update_registers = 1;
    }

    if (has_note)
    {
        uint32_t index = *data++;
        if (index == 0xFE)
        {
            note_cut = 1;
        }
        else
        {
            gbt.arpeggio_freq_index[1][0] = index;
            gbt.base_freq[1] = _gbt_get_freq_from_index(index);
            gbt.freq[1] = gbt.base_freq[1];
            has_to_update_registers = 1;
        }
    }

    if (has_instrument)
    {
        gbt.instr[1] = (*data & 0x30) << 2;
        has_to_update_registers = 1;
    }

    if (has_effect)
    {
        uint8_t effect = (*data++) & 0x0F;
        uint8_t args = *data;
        has_to_update_registers |= gbt_channel_2_set_effect(effect, args);
    }

    if (note_cut)
    {
        REG_SOUND2CNT_L = 0; // Set volume to 0
        REG_SOUND2CNT_H = SOUND2CNT_H_RESTART;
    }
    else if (has_to_update_registers)
    {
        channel2_refresh_registers();
    }

    return next;
}

// Returns 1 if it needed to update sound registers
static int channel2_update_effects(void)
{
    int ret = 0;

    // Cut note
    // --------

    if (gbt.cut_note_tick[1] == gbt.ticks_elapsed)
    {
        gbt.cut_note_tick[1] = 0xFF; // Disable cut note

        REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_2_ENABLE_RIGHT | SOUNDCNT_L_PSG_2_ENABLE_LEFT);

        REG_SOUND2CNT_L = 0; // Set volume to 0
        REG_SOUND2CNT_H = SOUND2CNT_H_RESTART;
    }

    // Arpeggio
    // --------

    if (gbt.arpeggio_enabled[1])
    {
        uint8_t tick = gbt.arpeggio_tick[1];
        if (tick == 2)
            gbt.arpeggio_tick[1] = 0;
        else
            gbt.arpeggio_tick[1] = tick + 1;

        uint32_t index = gbt.arpeggio_freq_index[1][tick];
        gbt.freq[1] = _gbt_get_freq_from_index(index);

        ret = 1;
    }

    // Vibrato
    // -------

    if (gbt.vibrato_enabled[1])
    {
        gbt.vibrato_position[1] += (gbt.vibrato_args[1] >> 4);
        gbt.vibrato_position[1] &= 63;

        int32_t pos = gbt.vibrato_position[1];
        int32_t delta = vibrato_sine[pos];
        delta *= (int32_t)(gbt.vibrato_args[1] & 0xF);
        delta >>= 7;

        int32_t freq = (int32_t)gbt.base_freq[1] - delta;
        if (freq < 0)
            freq = 0;
        else if (freq > 0x7FF)
            freq = 0x7FF;
        gbt.freq[1] = freq;

        ret = 1;
    }

    return ret;
}

// -----------------------------------------------------------------------------
// --------------------------------- Channel 3 ---------------------------------
// -----------------------------------------------------------------------------

static int gbt_ch3_pan(uint32_t args)
{
    gbt.pan[2] = args & 0x44;
    return 0; // Panning is always updated
}

static int gbt_ch3_cut_note(uint32_t args)
{
    gbt.cut_note_tick[2] = args;
    return 0;
}

static int gbt_ch3_arpeggio(uint32_t args)
{
    uint32_t base_index = gbt.arpeggio_freq_index[2][0];
    gbt.arpeggio_freq_index[2][1] = base_index + ((args >> 4) & 0xF);
    gbt.arpeggio_freq_index[2][2] = base_index + (args & 0xF);

    gbt.arpeggio_enabled[2] = 1;
    gbt.arpeggio_tick[2] = 1;

    return 1;
}

static int gbt_ch3_vibrato(uint32_t args)
{
    if (args != 0)
    {
        gbt.vibrato_position[2] = 0;
        gbt.vibrato_args[2] = args;
    }

    gbt.vibrato_enabled[2] = 1;

    return 1;
}

static effect_handler gbt_ch3_jump_table[16] = {
    gbt_ch3_pan,
    gbt_ch3_arpeggio,
    gbt_ch3_cut_note,
    gbt_ch3_vibrato,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_jump_pattern,
    gbt_ch1234_jump_position,
    gbt_ch1234_speed,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
};

// returns 1 if needed to update registers, 0 if not
static int gbt_channel_3_set_effect(uint32_t effect, uint8_t data)
{
    return gbt_ch3_jump_table[effect](data);
}

static void channel3_refresh_registers(void)
{
    REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_3_ENABLE_RIGHT | SOUNDCNT_L_PSG_3_ENABLE_LEFT);

    // On the GBA, the output of channel 3 is inverted. This causes the channel
    // to output a loud spike when disabled. It’s a good idea to "remove" the
    // channel using NR51 before refreshing wave RAM.

    uint32_t instr = gbt.instr[2];

    if (gbt.channel3_loaded_instrument != instr)
    {
        const uint8_t *wave = gbt.ch3_instrument[instr];
        uint8_t flags = gbt.ch3_instrument_flags[instr];

        // Disable channel and set bank 0 as writable
        REG_SOUND3CNT_L = SOUND3CNT_L_DISABLE | SOUND3CNT_L_BANK_SET(1);

        for (int i = 0; i < 16; i += 2)
        {
            uint16_t wave_lo = *wave++;
            uint16_t wave_hi = *wave++;
            REG_WAVE_RAM[i / 2] = wave_lo | (wave_hi << 8);
        }

        if (flags & BIT(7)) // 64 samples
        {
            // Disable channel and set bank 1 as writable
            REG_SOUND3CNT_L = SOUND3CNT_L_DISABLE | SOUND3CNT_L_BANK_SET(0);

            for (int i = 0; i < 16; i += 2)
            {
                uint16_t wave_lo = *wave++;
                uint16_t wave_hi = *wave++;
                REG_WAVE_RAM[i / 2] = wave_lo | (wave_hi << 8);
            }

            // Use the 64 samples mode
            REG_SOUND3CNT_L = SOUND3CNT_L_SIZE_64 | SOUND3CNT_L_ENABLE;
        }
        else
        {
            // Set bank 0 as active and use the 32 samples mode
            REG_SOUND3CNT_L = SOUND3CNT_L_SIZE_32 | SOUND3CNT_L_BANK_SET(0) |
                            SOUND3CNT_L_ENABLE;
        }


        // Done
        gbt.channel3_loaded_instrument = instr;
    }

    REG_SOUND3CNT_H = gbt.vol[2];
    REG_SOUND3CNT_X = SOUND3CNT_X_RESTART | gbt.freq[2];
}

static const uint8_t *gbt_channel_3_handle(const uint8_t *data)
{
    // Calculate pointer to next channel
    // Note: The volume bit doesn't affect the final size.
    const uint8_t sizes[8] = { 1, 2, 3, 3, 2, 3, 4, 4 };
    uint8_t bits = (*data) >> 5;
    const uint8_t *next = data + sizes[bits];

    // If the channel is disabled, exit
    if ((gbt.channels_enabled & GBT_ENABLE_CH3) == 0)
    {
        return next;
    }

    uint8_t header = *data++;

    int has_volume = header & HAS_VOLUME;
    int has_instrument = header & HAS_INSTRUMENT;
    int has_effect = header & HAS_EFFECT;
    int has_note = header & HAS_NOTE;

    int has_to_update_registers = 0;
    int note_cut = 0;

    if (has_volume)
    {
        gbt.vol[2] = (header & 0x7) << 13;
        has_to_update_registers = 1;
    }

    if (has_note)
    {
        uint32_t index = *data++;
        if (index == 0xFE)
        {
            note_cut = 1;
        }
        else
        {
            gbt.arpeggio_freq_index[2][0] = index;
            gbt.base_freq[2] = _gbt_get_freq_from_index(index);
            gbt.freq[2] = gbt.base_freq[2];
            has_to_update_registers = 1;
        }
    }

    if (has_instrument)
    {
        gbt.instr[2] = (*data & 0x70) >> 4;
        has_to_update_registers = 1;
    }

    if (has_effect)
    {
        uint8_t effect = (*data++) & 0x0F;
        uint8_t args = *data;
        has_to_update_registers |= gbt_channel_3_set_effect(effect, args);
    }

    if (note_cut)
    {
        REG_SOUND3CNT_X = SOUND3CNT_X_RESTART;
        REG_SOUND3CNT_H = SOUND3CNT_H_VOLUME_0;
        REG_SOUND3CNT_X = SOUND3CNT_X_RESTART;
    }
    else if (has_to_update_registers)
    {
        channel3_refresh_registers();
    }

    return next;
}

// Returns 1 if it needed to update sound registers
static int channel3_update_effects(void)
{
    int ret = 0;

    // Cut note
    // --------

    if (gbt.cut_note_tick[2] == gbt.ticks_elapsed)
    {
        gbt.cut_note_tick[2] = 0xFF; // Disable cut note

        REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_3_ENABLE_RIGHT | SOUNDCNT_L_PSG_3_ENABLE_LEFT);

        REG_SOUND3CNT_X = SOUND3CNT_X_RESTART;
        REG_SOUND3CNT_H = SOUND3CNT_H_VOLUME_0;
        REG_SOUND3CNT_X = SOUND3CNT_X_RESTART;
    }

    // Arpeggio
    // --------

    if (gbt.arpeggio_enabled[2])
    {
        uint8_t tick = gbt.arpeggio_tick[2];
        if (tick == 2)
            gbt.arpeggio_tick[2] = 0;
        else
            gbt.arpeggio_tick[2] = tick + 1;

        uint32_t index = gbt.arpeggio_freq_index[2][tick];
        gbt.freq[2] = _gbt_get_freq_from_index(index);

        ret = 1;
    }

    // Vibrato
    // -------

    if (gbt.vibrato_enabled[2])
    {
        gbt.vibrato_position[2] += (gbt.vibrato_args[2] >> 4);
        gbt.vibrato_position[2] &= 63;

        int32_t pos = gbt.vibrato_position[2];
        int32_t delta = vibrato_sine[pos];
        delta *= (int32_t)(gbt.vibrato_args[2] & 0xF);
        delta >>= 7;

        int32_t freq = (int32_t)gbt.base_freq[2] - delta;
        if (freq < 0)
            freq = 0;
        else if (freq > 0x7FF)
            freq = 0x7FF;
        gbt.freq[2] = freq;

        ret = 1;
    }

    return ret;
}

// -----------------------------------------------------------------------------
// --------------------------------- Channel 4 ---------------------------------
// -----------------------------------------------------------------------------

static int gbt_ch4_pan(uint32_t args)
{
    gbt.pan[3] = args & 0x88;
    return 0; // Panning is always updated
}

static int gbt_ch4_cut_note(uint32_t args)
{
    gbt.cut_note_tick[3] = args;
    return 0;
}

static int gbt_ch4_volslide(uint32_t args)
{
    // Volume slide index 2
    gbt.volslide_args[2] = args << 8; // Move to the right location
    return 1;
}

static effect_handler gbt_ch4_jump_table[16] = {
    gbt_ch4_pan,
    gbt_ch1234_nop,
    gbt_ch4_cut_note,
    gbt_ch1234_nop,
    gbt_ch4_volslide,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_jump_pattern,
    gbt_ch1234_jump_position,
    gbt_ch1234_speed,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
    gbt_ch1234_nop,
};

// returns 1 if needed to update registers, 0 if not
static int gbt_channel_4_set_effect(uint32_t effect, uint8_t data)
{
    return gbt_ch4_jump_table[effect](data);
}

static void channel4_refresh_registers(void)
{
    REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_4_ENABLE_RIGHT | SOUNDCNT_L_PSG_4_ENABLE_LEFT);

    REG_SOUND4CNT_L = gbt.vol[3] | gbt.volslide_args[2]; // Volume slide index 2
    REG_SOUND4CNT_H = SOUND4CNT_H_RESTART | gbt.instr[3];
}

static const uint8_t *gbt_channel_4_handle(const uint8_t *data)
{
    // Calculate pointer to next channel
    // Note: The volume bit doesn't affect the final size.
    const uint8_t sizes[4] = { 1, 3, 2, 4 };
    uint8_t bits = (*data) >> 6;
    const uint8_t *next = data + sizes[bits];

    // If the channel is disabled, exit
    if ((gbt.channels_enabled & GBT_ENABLE_CH4) == 0)
    {
        return next;
    }

    uint8_t header = *data++;

    int has_volume = header & HAS_VOLUME;
    int has_effect = header & HAS_EFFECT;
    int has_kit = header & HAS_KIT;

    int has_to_update_registers = 0;
    int note_cut = 0;

    if (has_volume)
    {
        gbt.vol[3] = (header & 0xF) << 12;
        has_to_update_registers = 1;
    }

    if (has_kit)
    {
        uint32_t index = *data++;
        if (index == 0xFE)
        {
            note_cut = 1;
        }
        else
        {
            index &= 0x0F;
            gbt.instr[3] = gbt_noise[index];
            has_to_update_registers = 1;
        }
    }

    if (has_effect)
    {
        uint8_t effect = (*data++) & 0x0F;
        uint8_t args = *data++;
        has_to_update_registers |= gbt_channel_4_set_effect(effect, args);
    }

    if (note_cut)
    {
        REG_SOUND4CNT_L = 0; // Set volume to 0
        REG_SOUND4CNT_H = SOUND4CNT_H_RESTART;
    }
    else if (has_to_update_registers)
    {
        channel4_refresh_registers();
    }

    return next;
}

// Returns 1 if it needed to update sound registers
static int channel4_update_effects(void)
{
    int ret = 0;

    // Cut note
    // --------

    if (gbt.cut_note_tick[3] == gbt.ticks_elapsed)
    {
        gbt.cut_note_tick[3] = 0xFF; // Disable cut note

        REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_4_ENABLE_RIGHT | SOUNDCNT_L_PSG_4_ENABLE_LEFT);

        REG_SOUND4CNT_L = 0; // Set volume to 0
        REG_SOUND4CNT_H = SOUND4CNT_H_RESTART;
    }

    return ret;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

static void gbt_update_effects_internal(void)
{
    if (channel1_update_effects())
        channel1_refresh_registers();

    if (channel2_update_effects())
        channel2_refresh_registers();

    if (channel3_update_effects())
        channel3_refresh_registers();

    if (channel4_update_effects())
        channel4_refresh_registers();
}

static void gbt_update_refresh_panning(void)
{
    uint16_t new_pan = gbt.pan[0] | gbt.pan[1] | gbt.pan[2] | gbt.pan[3];
    new_pan = (new_pan << 8) & gbt.pan_volume_mask;

    REG_SOUNDCNT_L = gbt.global_volume | new_pan;
}

void gbt_update(void)
{
    // If not playing, return
    if (gbt.playing == 0)
        return;

    // Handle tick counter

    gbt.ticks_elapsed++;
    if (gbt.ticks_elapsed != gbt.speed)
    {
        // Update effects and exit
        gbt_update_effects_internal();
        gbt_update_refresh_panning();
        return;
    }

    gbt.ticks_elapsed = 0; // reset tick counter

    // Clear tick-based effects
    // ------------------------

    gbt.arpeggio_enabled[0] = 0; // Disable arpeggio
    gbt.arpeggio_enabled[1] = 0;
    gbt.arpeggio_enabled[2] = 0;

    gbt.vibrato_enabled[0] = 0; // Disable vibrato
    gbt.vibrato_enabled[1] = 0;
    gbt.vibrato_enabled[2] = 0;

    gbt.volslide_args[0] = 0; // Disable volume slide
    gbt.volslide_args[1] = 0;
    gbt.volslide_args[2] = 0;

    gbt.cut_note_tick[0] = 0xFF; // Disable cut note
    gbt.cut_note_tick[1] = 0xFF;
    gbt.cut_note_tick[2] = 0xFF;
    gbt.cut_note_tick[3] = 0xFF;

    // Update effects
    // --------------

    gbt_update_effects_internal();

    // Check if the song has ended
    // ---------------------------

    if (gbt.current_step_data_ptr == NULL)
    {
        if (gbt.loop_enabled)
        {
            // If loop is enabled, jump to pattern 0
            gbt.current_pattern = 0;
            gbt_get_pattern_ptr(gbt.current_pattern);
            gbt_run_startup_commands(gbt.startup_cmds_ptr);
        }
        else
        {
            // If loop is disabled, stop song
            gbt_stop();
            return;
        }
    }

    // Update channels
    // ---------------

    const uint8_t *ptr = gbt.current_step_data_ptr;

    ptr = gbt_channel_1_handle(ptr);
    ptr = gbt_channel_2_handle(ptr);
    ptr = gbt_channel_3_handle(ptr);
    ptr = gbt_channel_4_handle(ptr);

    gbt.current_step_data_ptr = ptr;

    // Handle panning
    // --------------

    gbt_update_refresh_panning();

    // Increment step
    // --------------

    if (gbt.jump_requested == 0)
    {
        gbt.current_step++;

        if (gbt.current_step == 64)
        {
            // Increment pattern
            gbt.current_step = 0;
            gbt.current_pattern++;
            gbt_get_pattern_ptr(gbt.current_pattern);
        }
    }
    else
    {
        gbt.jump_requested = 0;

        gbt.current_step = gbt.jump_target_step;
        gbt.current_pattern = gbt.jump_target_pattern;

        gbt_get_pattern_ptr(gbt.current_pattern); // set ptr to start of pattern

        // Seek the requested step

        const uint8_t *src_search = gbt.current_step_data_ptr;

        for (int i = 0; i < gbt.current_step; i++)
        {
            for (int j = 0; j < 3; j++) // Channels 1-3
            {
                // Note: The volume bit doesn't affect the final size.
                const uint8_t sizes[8] = { 1, 2, 3, 3, 2, 3, 4, 4 };
                uint8_t bits = (*src_search) >> 5;
                src_search += sizes[bits];
            }

            // Channel 4
            {
                // Note: The volume bit doesn't affect the final size.
                const uint8_t sizes[4] = { 1, 3, 2, 4 };
                uint8_t bits = (*src_search) >> 6;
                src_search += sizes[bits];
            }
        }

        gbt.current_step_data_ptr = src_search;
    }
}
