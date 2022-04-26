// GBT Player v4.0.1
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
    uint16_t freq[3]; // Ch 1-3

    // Currently loaded instrument (0xFF if none)
    uint8_t channel3_loaded_instrument;

    // Arpeggio -> Ch 1-3
    // {base index, base index+x, base index+y}
    uint8_t arpeggio_freq_index[3][3];
    uint8_t arpeggio_enabled[3]; // if 0, disabled
    uint8_t arpeggio_tick[3];

    // Cut note
    uint8_t cut_note_tick[4]; // If tick == gbt.cut_note_tick, stop note.

    uint8_t jump_requested; // set to 1 by jump effects
    uint8_t jump_target_step;
    uint8_t jump_target_pattern;

} gbt_player_info_t;

EWRAM_BSS static gbt_player_info_t gbt;

static const uint8_t gbt_wave[8][16] = { // 8 sounds
    { 0xA5, 0xD7, 0xC9, 0xE1, 0xBC, 0x9A, 0x76, 0x31,
      0x0C, 0xBA, 0xDE, 0x60, 0x1B, 0xCA, 0x03, 0x93 }, // random
    { 0xF0, 0xE1, 0xD2, 0xC3, 0xB4, 0xA5, 0x96, 0x87,
      0x78, 0x69, 0x5A, 0x4B, 0x3C, 0x2D, 0x1E, 0x0F },
    { 0xFD, 0xEC, 0xDB, 0xCA, 0xB9, 0xA8, 0x97, 0x86,
      0x79, 0x68, 0x57, 0x46, 0x35, 0x24, 0x13, 0x02 }, // up-downs
    { 0xDE, 0xFE, 0xDC, 0xBA, 0x9A, 0xA9, 0x87, 0x77,
      0x88, 0x87, 0x65, 0x56, 0x54, 0x32, 0x10, 0x12 },
    { 0xAB, 0xCD, 0xEF, 0xED, 0xCB, 0xA0, 0x12, 0x3E,
      0xDC, 0xBA, 0xBC, 0xDE, 0xFE, 0xDC, 0x32, 0x10 }, // triangular broken
    { 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88,
      0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 }, // triangular
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // square
    { 0x79, 0xBC, 0xDE, 0xEF, 0xFF, 0xEE, 0xDC, 0xB9,
      0x75, 0x43, 0x21, 0x10, 0x00, 0x11, 0x23, 0x45 }, // sine
};

static const uint8_t gbt_noise[16] = { // 16 sounds
    // 7 bit
    0x5F, 0x5B, 0x4B, 0x2F, 0x3B, 0x58, 0x1F, 0x0F,
    // 15 bit
    0x90, 0x80, 0x70, 0x50, 0x00,
    0x67, 0x63, 0x53
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

    gbt.cut_note_tick[0] = 0xFF;
    gbt.cut_note_tick[1] = 0xFF;
    gbt.cut_note_tick[2] = 0xFF;
    gbt.cut_note_tick[3] = 0xFF;

    // Run startup commands after internal player status has been initialized

    gbt_run_startup_commands(gbt.startup_cmds_ptr);

    // Initialize hardware registers

    REG_SOUNDCNT_X = SOUNDCNT_X_MASTER_ENABLE;

    REG_SOUNDCNT_L = 0;

    REG_SOUND1CNT_L = 0;
    REG_SOUND1CNT_H = 0;
    REG_SOUND1CNT_X = 0;

    REG_SOUND2CNT_L = 0;
    REG_SOUND2CNT_H = 0;

    REG_SOUND3CNT_L = 0;
    REG_SOUND3CNT_H = 0;
    REG_SOUND3CNT_X = 0;

    REG_SOUND4CNT_L = 0;
    REG_SOUND4CNT_H = 0;

    REG_SOUNDCNT_L = SOUNDCNT_L_PSG_VOL_RIGHT_SET(7)
                   | SOUNDCNT_L_PSG_VOL_LEFT_SET(7);

    // Everything is ready

    gbt.playing = 1;
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

static effect_handler gbt_ch1_jump_table[16] = {
    gbt_ch1_pan,
    gbt_ch1_arpeggio,
    gbt_ch1_cut_note,
    gbt_ch1234_nop,
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
static int gbt_channel_1_set_effect(uint32_t effect, uint8_t data)
{
    return gbt_ch1_jump_table[effect](data);
}

static void channel1_refresh_registers(void)
{
    REG_SOUND1CNT_L = 0;
    REG_SOUND1CNT_H = gbt.vol[0] | gbt.instr[0];
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

    if (has_volume)
    {
        gbt.vol[0] = (header & 0xF) << 12;
        has_to_update_registers = 1;
    }

    if (has_note)
    {
        uint32_t index = *data++ & 0x7F;
        gbt.arpeggio_freq_index[0][0] = index;
        gbt.freq[0] = _gbt_get_freq_from_index(index);
        has_to_update_registers = 1;
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

    if (has_to_update_registers)
    {
        channel1_refresh_registers();
    }

    return next;
}

// Returns 1 if it needed to update sound registers
static int channel1_update_effects(void)
{
    // Cut note
    // --------

    if (gbt.cut_note_tick[0] == gbt.ticks_elapsed)
    {
        gbt.cut_note_tick[0] = 0xFF; // Disable cut note

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

        return 1;
    }

    return 0;
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

static effect_handler gbt_ch2_jump_table[16] = {
    gbt_ch2_pan,
    gbt_ch2_arpeggio,
    gbt_ch2_cut_note,
    gbt_ch1234_nop,
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
static int gbt_channel_2_set_effect(uint32_t effect, uint8_t data)
{
    return gbt_ch2_jump_table[effect](data);
}

static void channel2_refresh_registers(void)
{
    REG_SOUND2CNT_L = gbt.vol[1] | gbt.instr[1];
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

    if (has_volume)
    {
        gbt.vol[1] = (header & 0xF) << 12;
        has_to_update_registers = 1;
    }

    if (has_note)
    {
        uint32_t index = *data++ & 0x7F;
        gbt.arpeggio_freq_index[1][0] = index;
        gbt.freq[1] = _gbt_get_freq_from_index(index);
        has_to_update_registers = 1;
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

    if (has_to_update_registers)
    {
        channel2_refresh_registers();
    }

    return next;
}

// Returns 1 if it needed to update sound registers
static int channel2_update_effects(void)
{
    // Cut note
    // --------

    if (gbt.cut_note_tick[1] == gbt.ticks_elapsed)
    {
        gbt.cut_note_tick[1] = 0xFF; // Disable cut note

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

        return 1;
    }

    return 0;
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

static effect_handler gbt_ch3_jump_table[16] = {
    gbt_ch3_pan,
    gbt_ch3_arpeggio,
    gbt_ch3_cut_note,
    gbt_ch1234_nop,
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
    // Disable channel and set bank 0 as writable
    REG_SOUND3CNT_L = SOUND3CNT_L_DISABLE | SOUND3CNT_L_BANK_SET(1);

    uint32_t instr = gbt.instr[2];
    if (gbt.channel3_loaded_instrument != instr)
    {
        for (int i = 0; i < 16; i += 2)
        {
            REG_WAVE_RAM[i / 2] = (uint16_t)gbt_wave[instr][i] |
                                 ((uint16_t)gbt_wave[instr][i + 1] << 8);
        }

        gbt.channel3_loaded_instrument = instr;
    }

    REG_SOUND3CNT_L = SOUND3CNT_L_SIZE_32 | SOUND3CNT_L_BANK_SET(0) |
                      SOUND3CNT_L_ENABLE;

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

    if (has_volume)
    {
        gbt.vol[2] = (header & 0x7) << 13;
        has_to_update_registers = 1;
    }

    if (has_note)
    {
        uint32_t index = *data++ & 0x7F;
        gbt.arpeggio_freq_index[2][0] = index;
        gbt.freq[2] = _gbt_get_freq_from_index(index);
        has_to_update_registers = 1;
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

    if (has_to_update_registers)
    {
        channel3_refresh_registers();
    }

    return next;
}

// Returns 1 if it needed to update sound registers
static int channel3_update_effects(void)
{
    // Cut note
    // --------

    if (gbt.cut_note_tick[2] == gbt.ticks_elapsed)
    {
        gbt.cut_note_tick[2] = 0xFF; // Disable cut note

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

        return 1;
    }

    return 0;
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

static effect_handler gbt_ch4_jump_table[16] = {
    gbt_ch4_pan,
    gbt_ch1234_nop,
    gbt_ch4_cut_note,
    gbt_ch1234_nop,
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
static int gbt_channel_4_set_effect(uint32_t effect, uint8_t data)
{
    return gbt_ch4_jump_table[effect](data);
}

static void channel4_refresh_registers(void)
{
    REG_SOUND4CNT_L = gbt.vol[3];
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

    if (has_volume)
    {
        gbt.vol[3] = (header & 0xF) << 12;
        has_to_update_registers = 1;
    }

    if (has_kit)
    {
        uint32_t index = *data++ & 0x0F;
        gbt.instr[3] = gbt_noise[index];
        has_to_update_registers = 1;
    }

    if (has_effect)
    {
        uint8_t effect = (*data++) & 0x0F;
        uint8_t args = *data++;
        has_to_update_registers |= gbt_channel_4_set_effect(effect, args);
    }

    if (has_to_update_registers)
    {
        channel4_refresh_registers();
    }

    return next;
}

// Returns 1 if it needed to update sound registers
static int channel4_update_effects(void)
{
    // Cut note
    // --------

    if (gbt.cut_note_tick[3] == gbt.ticks_elapsed)
    {
        gbt.cut_note_tick[3] = 0xFF; // Disable cut note

        REG_SOUND4CNT_L = 0; // Set volume to 0
        REG_SOUND4CNT_H = SOUND4CNT_H_RESTART;
    }

    return 0;
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
        return;
    }

    gbt.ticks_elapsed = 0; // reset tick counter

    // Clear tick-based effects
    // ------------------------

    gbt.arpeggio_enabled[0] = 0; // Disable arpeggio
    gbt.arpeggio_enabled[1] = 0;
    gbt.arpeggio_enabled[2] = 0;

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

    uint16_t mask =
        SOUNDCNT_L_PSG_1_ENABLE_RIGHT | SOUNDCNT_L_PSG_1_ENABLE_LEFT |
        SOUNDCNT_L_PSG_2_ENABLE_RIGHT | SOUNDCNT_L_PSG_2_ENABLE_LEFT |
        SOUNDCNT_L_PSG_3_ENABLE_RIGHT | SOUNDCNT_L_PSG_3_ENABLE_LEFT |
        SOUNDCNT_L_PSG_4_ENABLE_RIGHT | SOUNDCNT_L_PSG_4_ENABLE_LEFT;

    uint16_t new_pan = gbt.pan[0] | gbt.pan[1] | gbt.pan[2] | gbt.pan[3];

    REG_SOUNDCNT_L = (REG_SOUNDCNT_L & ~mask) | (new_pan << 8);

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
