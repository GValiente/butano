// GBT Player v3.0.9
//
// SPDX-License-Identifier: MIT
//
// Copyright (c) 2009-2022, Antonio Niño Díaz <antonio_nd@outlook.com>

#include <stdint.h>

#ifdef GBT_USE_LIBUGBA
# include <ugba/ugba.h>
#else
# include "gbt_hardware.h"
#endif

#include "../include/gbt_player.h"

typedef int (*effect_handler)(uint32_t args);

struct gbt_data_t
{
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
    uint8_t cut_note_tick[4]; // If tick == gbt_data.cut_note_tick, stop note.

    // Last step of last pattern this is set to 1
    uint8_t have_to_stop_next_step;

    uint8_t update_pattern_pointers; // set to 1 by jump effects
};

__attribute__((section(".ewram"))) static struct gbt_data_t gbt_data;

static void gbt_get_pattern_ptr(int pattern_number)
{
    gbt_data.current_step_data_ptr = gbt_data.pattern_array_ptr[pattern_number];
}

void gbt_play(const void *song, int speed)
{
    gbt_data.pattern_array_ptr = song;
    gbt_data.speed = speed;

    gbt_get_pattern_ptr(0);

    gbt_data.current_step = 0;
    gbt_data.current_pattern = 0;
    gbt_data.ticks_elapsed = 0;
    gbt_data.loop_enabled = 0;
    gbt_data.have_to_stop_next_step = 0;
    gbt_data.update_pattern_pointers = 0;

    gbt_data.channel3_loaded_instrument = 0xFF;

    gbt_data.channels_enabled = GBT_ENABLE_CH_ALL;

    gbt_data.pan[0] = 0x11; // L and R
    gbt_data.pan[1] = 0x22;
    gbt_data.pan[2] = 0x44;
    gbt_data.pan[3] = 0x88;

    gbt_data.vol[0] = 0xF000; // 100%
    gbt_data.vol[1] = 0xF000;
    gbt_data.vol[2] = 0x2000;
    gbt_data.vol[3] = 0xF000;

    gbt_data.instr[0] = 0;
    gbt_data.instr[1] = 0;
    gbt_data.instr[2] = 0;
    gbt_data.instr[3] = 0;

    gbt_data.freq[0] = 0;
    gbt_data.freq[1] = 0;
    gbt_data.freq[2] = 0;

    gbt_data.arpeggio_enabled[0] = 0;
    gbt_data.arpeggio_enabled[1] = 0;
    gbt_data.arpeggio_enabled[2] = 0;

    gbt_data.cut_note_tick[0] = 0xFF;
    gbt_data.cut_note_tick[1] = 0xFF;
    gbt_data.cut_note_tick[2] = 0xFF;
    gbt_data.cut_note_tick[3] = 0xFF;

    // REG_SOUNDCNT_X = SOUNDCNT_X_MASTER_ENABLE;

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

    gbt_data.playing = 1;
}

void gbt_pause(int play)
{
    gbt_data.playing = play;

    uint16_t mask =
        SOUNDCNT_L_PSG_1_ENABLE_RIGHT | SOUNDCNT_L_PSG_1_ENABLE_LEFT |
        SOUNDCNT_L_PSG_2_ENABLE_RIGHT | SOUNDCNT_L_PSG_2_ENABLE_LEFT |
        SOUNDCNT_L_PSG_3_ENABLE_RIGHT | SOUNDCNT_L_PSG_3_ENABLE_LEFT |
        SOUNDCNT_L_PSG_4_ENABLE_RIGHT | SOUNDCNT_L_PSG_4_ENABLE_LEFT;

    if (play)
    {
        // Unmute sound if playback is resumed
        uint16_t new_pan = gbt_data.pan[0] | gbt_data.pan[1] | gbt_data.pan[2] | gbt_data.pan[3];
        REG_SOUNDCNT_L = (REG_SOUNDCNT_L & ~mask) | (new_pan << 8);
    }
    else
    {
        REG_SOUNDCNT_L &= ~mask;
    }
}

void gbt_loop(int loop)
{
    gbt_data.loop_enabled = loop;
}

void gbt_stop(void)
{
    gbt_data.playing = 0;
    REG_SOUNDCNT_L = 0; // Disable all PSG channels
    // REG_SOUNDCNT_X = SOUNDCNT_X_MASTER_DISABLE; // Don't do this!
}

void gbt_enable_channels(int flags)
{
    gbt_data.channels_enabled = flags;
}

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

static const uint16_t gbt_frequencies[] = {
      44,  156,  262,  363,  457,  547,  631,  710,  786,  854,  923,  986,
    1046, 1102, 1155, 1205, 1253, 1297, 1339, 1379, 1417, 1452, 1486, 1517,
    1546, 1575, 1602, 1627, 1650, 1673, 1694, 1714, 1732, 1750, 1767, 1783,
    1798, 1812, 1825, 1837, 1849, 1860, 1871, 1881, 1890, 1899, 1907, 1915,
    1923, 1930, 1936, 1943, 1949, 1954, 1959, 1964, 1969, 1974, 1978, 1982,
    1985, 1988, 1992, 1995, 1998, 2001, 2004, 2006, 2009, 2011, 2013, 2015
};

static uint16_t _gbt_get_freq_from_index(int index)
{
    return gbt_frequencies[index];
}

static int gbt_ch1234_nop(uint32_t args)
{
    (void)(args);
    return 0;
}

static int gbt_ch1234_speed(uint32_t args)
{
    gbt_data.speed = args;
    gbt_data.ticks_elapsed = 0;
    return 0;
}

static int gbt_ch1234_jump_pattern(uint32_t args)
{
    gbt_data.current_pattern = args;

    gbt_data.current_step = 0;
    gbt_data.have_to_stop_next_step = 0; // Clear stop flag

    gbt_data.update_pattern_pointers = 1;

    return 0;
}

static int gbt_ch1234_jump_position(uint32_t args)
{
    gbt_data.current_step = args;

    gbt_data.current_pattern++;

    // Check to see if jump puts us past end of song
    gbt_get_pattern_ptr(gbt_data.current_pattern);
    if (gbt_data.current_step_data_ptr == NULL)
    {
        gbt_data.current_pattern = 0;
    }

    gbt_data.update_pattern_pointers = 1;

    return 0;
}

// -----------------------------------------------------------------------------
// --------------------------------- Channel 1 ---------------------------------
// -----------------------------------------------------------------------------

static int gbt_ch1_pan(uint32_t args)
{
    gbt_data.pan[0] = args & 0x11;
    return 0; // do not update registers, only NR51 at end.
}

static int gbt_ch1_cut_note(uint32_t args)
{
    gbt_data.cut_note_tick[0] = args;
    return 0;
}

static int gbt_ch1_arpeggio(uint32_t args)
{
    uint32_t base_index = gbt_data.arpeggio_freq_index[0][0];
    gbt_data.arpeggio_freq_index[0][1] = base_index + ((args >> 4) & 0xF);
    gbt_data.arpeggio_freq_index[0][2] = base_index + (args & 0xF);

    gbt_data.arpeggio_enabled[0] = 1;
    gbt_data.arpeggio_tick[0] = 1;

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
    REG_SOUND1CNT_H = gbt_data.vol[0] | gbt_data.instr[0];
    REG_SOUND1CNT_X = SOUND1CNT_X_RESTART | gbt_data.freq[0];
}

static const uint8_t *gbt_channel_1_handle(const uint8_t *data)
{
    if ((gbt_data.channels_enabled & GBT_ENABLE_CH1) == 0)
    {
        // Channel is disabled. Increment pointer as needed

        uint8_t b = *data++;

        if (b & BIT(7))
        {
            b = *data++;

            if (b & BIT(7))
                data++;
        }
        else if (b & BIT(6))
        {
            data++;
        }

        return data;
    }

    // Channel 1 is enabled

    uint8_t b = *data++;

    if (b & BIT(7)) // Has frequency
    {
        uint32_t index = b & 0x7F;
        gbt_data.arpeggio_freq_index[0][0] = index;
        gbt_data.freq[0] = _gbt_get_freq_from_index(index);

        b = *data++;

        if (b & BIT(7)) // Freq + Instr + Effect
        {
            gbt_data.instr[0] = (b & 0x30) << 2; // Instrument
            gbt_channel_1_set_effect(b & 0xF, *data++);
            channel1_refresh_registers();
        }
        else // Freq + Instr + Volume
        {
            gbt_data.instr[0] = (b & 0x30) << 2; // Instrument
            gbt_data.vol[0] = (b & 0xF) << 12;
            channel1_refresh_registers();
        }
    }
    else if (b & BIT(6)) // Set instrument and effect
    {
        gbt_data.instr[0] = (b & 0x30) << 2; // Instrument
        gbt_channel_1_set_effect(b & 0xF, *data++);
        channel1_refresh_registers();
    }
    else if (b & BIT(5)) // Set volume
    {
        gbt_data.vol[0] = (b & 0xF) << 12;
        channel1_refresh_registers();
    }
    else
    {
        // NOP
    }

    return data;
}

// Returns 1 if it needed to update sound registers
static int channel1_update_effects(void)
{
    // Cut note
    // --------

    if (gbt_data.cut_note_tick[0] == gbt_data.ticks_elapsed)
    {
        gbt_data.cut_note_tick[0] = 0xFF; // Disable cut note

        REG_SOUND1CNT_H = 0; // Set volume to 0
        REG_SOUND1CNT_X = SOUND1CNT_X_RESTART;
    }

    // Arpeggio
    // --------

    if (gbt_data.arpeggio_enabled[0])
    {
        uint8_t tick = gbt_data.arpeggio_tick[0];
        if (tick == 2)
            gbt_data.arpeggio_tick[0] = 0;
        else
            gbt_data.arpeggio_tick[0] = tick + 1;

        uint32_t index = gbt_data.arpeggio_freq_index[0][tick];
        gbt_data.freq[0] = _gbt_get_freq_from_index(index);

        return 1;
    }

    return 0;
}

// -----------------------------------------------------------------------------
// --------------------------------- Channel 2 ---------------------------------
// -----------------------------------------------------------------------------

static int gbt_ch2_pan(uint32_t args)
{
    gbt_data.pan[1] = args & 0x22;
    return 0; // do not update registers, only NR51 at end.
}

static int gbt_ch2_cut_note(uint32_t args)
{
    gbt_data.cut_note_tick[1] = args;
    return 0;
}

static int gbt_ch2_arpeggio(uint32_t args)
{
    uint32_t base_index = gbt_data.arpeggio_freq_index[1][0];
    gbt_data.arpeggio_freq_index[1][1] = base_index + ((args >> 4) & 0xF);
    gbt_data.arpeggio_freq_index[1][2] = base_index + (args & 0xF);

    gbt_data.arpeggio_enabled[1] = 1;
    gbt_data.arpeggio_tick[1] = 1;

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
    REG_SOUND2CNT_L = gbt_data.vol[1] | gbt_data.instr[1];
    REG_SOUND2CNT_H = SOUND2CNT_H_RESTART | gbt_data.freq[1];
}

static const uint8_t *gbt_channel_2_handle(const uint8_t *data)
{
    if ((gbt_data.channels_enabled & GBT_ENABLE_CH2) == 0)
    {
        // Channel is disabled. Increment pointer as needed

        uint8_t b = *data++;

        if (b & BIT(7))
        {
            b = *data++;

            if (b & BIT(7))
                data++;
        }
        else if (b & BIT(6))
        {
            data++;
        }

        return data;
    }

    // Channel 2 is enabled

    uint8_t b = *data++;

    if (b & BIT(7)) // Has frequency
    {
        uint32_t index = b & 0x7F;
        gbt_data.arpeggio_freq_index[1][0] = index;
        gbt_data.freq[1] = _gbt_get_freq_from_index(index);

        b = *data++;

        if (b & BIT(7)) // Freq + Instr + Effect
        {
            gbt_data.instr[1] = (b & 0x30) << 2; // Instrument
            gbt_channel_2_set_effect(b & 0xF, *data++);
            channel2_refresh_registers();
        }
        else // Freq + Instr + Volume
        {
            gbt_data.instr[1] = (b & 0x30) << 2; // Instrument
            gbt_data.vol[1] = (b & 0xF) << 12;
            channel2_refresh_registers();
        }
    }
    else if (b & BIT(6)) // Set instrument and effect
    {

        gbt_data.instr[1] = (b & 0x30) << 2; // Instrument
        gbt_channel_2_set_effect(b & 0xF, *data++);
        channel2_refresh_registers();
    }
    else if (b & BIT(5)) // Set volume
    {
        gbt_data.vol[1] = (b & 0xF) << 12;
        channel2_refresh_registers();
    }
    else
    {
        // NOP
    }

    return data;
}

// Returns 1 if it needed to update sound registers
static int channel2_update_effects(void)
{
    // Cut note
    // --------

    if (gbt_data.cut_note_tick[1] == gbt_data.ticks_elapsed)
    {
        gbt_data.cut_note_tick[1] = 0xFF; // Disable cut note

        REG_SOUND2CNT_L = 0; // Set volume to 0
        REG_SOUND2CNT_H = SOUND2CNT_H_RESTART;
    }

    // Arpeggio
    // --------

    if (gbt_data.arpeggio_enabled[1])
    {
        uint8_t tick = gbt_data.arpeggio_tick[1];
        if (tick == 2)
            gbt_data.arpeggio_tick[1] = 0;
        else
            gbt_data.arpeggio_tick[1] = tick + 1;

        uint32_t index = gbt_data.arpeggio_freq_index[1][tick];
        gbt_data.freq[1] = _gbt_get_freq_from_index(index);

        return 1;
    }

    return 0;
}

// -----------------------------------------------------------------------------
// --------------------------------- Channel 3 ---------------------------------
// -----------------------------------------------------------------------------

static int gbt_ch3_pan(uint32_t args)
{
    gbt_data.pan[2] = args & 0x44;
    return 0; // do not update registers, only NR51 at end.
}

static int gbt_ch3_cut_note(uint32_t args)
{
    gbt_data.cut_note_tick[2] = args;
    return 0;
}

static int gbt_ch3_arpeggio(uint32_t args)
{
    uint32_t base_index = gbt_data.arpeggio_freq_index[2][0];
    gbt_data.arpeggio_freq_index[2][1] = base_index + ((args >> 4) & 0xF);
    gbt_data.arpeggio_freq_index[2][2] = base_index + (args & 0xF);

    gbt_data.arpeggio_enabled[2] = 1;
    gbt_data.arpeggio_tick[2] = 1;

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

    uint32_t instr = gbt_data.instr[2];
    if (gbt_data.channel3_loaded_instrument != instr)
    {
        for (int i = 0; i < 16; i += 2)
        {
            REG_WAVE_RAM[i / 2] = (uint16_t)gbt_wave[instr][i] |
                                 ((uint16_t)gbt_wave[instr][i + 1] << 8);
        }

        gbt_data.channel3_loaded_instrument = instr;
    }

    REG_SOUND3CNT_L = SOUND3CNT_L_SIZE_32 | SOUND3CNT_L_BANK_SET(0) |
                      SOUND3CNT_L_ENABLE;

    // TODO: Support 75%
    REG_SOUND3CNT_H = gbt_data.vol[2];
    REG_SOUND3CNT_X = SOUND3CNT_X_RESTART | gbt_data.freq[2];
}

static const uint8_t *gbt_channel_3_handle(const uint8_t *data)
{
    if ((gbt_data.channels_enabled & GBT_ENABLE_CH3) == 0)
    {
        // Channel is disabled. Increment pointer as needed

        uint8_t b = *data++;

        if (b & BIT(7))
        {
            b = *data++;

            if (b & BIT(7))
                data++;
        }
        else if (b & BIT(6))
        {
            data++;
        }

        return data;
    }

    // Channel 3 is enabled

    uint8_t b = *data++;

    if (b & BIT(7)) // Has frequency
    {
        uint32_t index = b & 0x7F;
        gbt_data.arpeggio_freq_index[2][0] = index;
        gbt_data.freq[2] = _gbt_get_freq_from_index(index);

        b = *data++;

        if (b & BIT(7)) // Freq + Instr + Effect
        {
            gbt_data.instr[2] = b & 0x0F; // Instrument
            // Effects 8-15 not available from this mode
            gbt_channel_3_set_effect((b >> 4) & 0x7, *data++);
            channel3_refresh_registers();
        }
        else // Freq + Instr + Volume
        {
            gbt_data.instr[2] = b & 0x0F; // Instrument
            gbt_data.vol[2] = (b & 0x30) << 9;
            channel3_refresh_registers();
        }
    }
    else if (b & BIT(6)) // Set effect
    {
        if (gbt_channel_3_set_effect(b & 0xF, *data++))
            channel3_refresh_registers();
    }
    else if (b & BIT(5)) // Set volume
    {
        gbt_data.vol[2] = (b & 0x3) << 13;
        channel3_refresh_registers();
    }
    else
    {
        // NOP
    }

    return data;
}

// Returns 1 if it needed to update sound registers
static int channel3_update_effects(void)
{
    // Cut note
    // --------

    if (gbt_data.cut_note_tick[2] == gbt_data.ticks_elapsed)
    {
        gbt_data.cut_note_tick[2] = 0xFF; // Disable cut note

        REG_SOUND3CNT_X = SOUND3CNT_X_RESTART;
        REG_SOUND3CNT_H = SOUND3CNT_H_VOLUME_0;
        REG_SOUND3CNT_X = SOUND3CNT_X_RESTART;
    }

    // Arpeggio
    // --------

    if (gbt_data.arpeggio_enabled[2])
    {
        uint8_t tick = gbt_data.arpeggio_tick[2];
        if (tick == 2)
            gbt_data.arpeggio_tick[2] = 0;
        else
            gbt_data.arpeggio_tick[2] = tick + 1;

        uint32_t index = gbt_data.arpeggio_freq_index[2][tick];
        gbt_data.freq[2] = _gbt_get_freq_from_index(index);

        return 1;
    }

    return 0;
}

// -----------------------------------------------------------------------------
// --------------------------------- Channel 4 ---------------------------------
// -----------------------------------------------------------------------------

static int gbt_ch4_pan(uint32_t args)
{
    gbt_data.pan[3] = args & 0x88;
    return 0; // do not update registers, only NR51 at end.
}

static int gbt_ch4_cut_note(uint32_t args)
{
    gbt_data.cut_note_tick[3] = args;
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
    REG_SOUND4CNT_L = gbt_data.vol[3];
    REG_SOUND4CNT_H = SOUND2CNT_H_RESTART | gbt_data.instr[3];
}

static const uint8_t *gbt_channel_4_handle(const uint8_t *data)
{
    if ((gbt_data.channels_enabled & GBT_ENABLE_CH4) == 0)
    {
        // Channel is disabled. Increment pointer as needed

        uint8_t b = *data++;

        if (b & BIT(7))
        {
            b = *data++;

            if (b & BIT(7))
                data++;
        }
        else if (b & BIT(6))
        {
            data++;
        }

        return data;
    }

    // Channel 4 is enabled

    uint8_t b = *data++;

    if (b & BIT(7)) // Has instrument
    {
        uint32_t index = b & 0x1F;
        gbt_data.instr[3] = gbt_noise[index];

        b = *data++;

        if (b & BIT(7)) // Instr + Effect
        {
            gbt_channel_4_set_effect(b & 0xF, *data++);
            channel4_refresh_registers();
        }
        else // Instr + Volume
        {
            gbt_data.vol[3] = (b & 0xF) << 12;
            channel4_refresh_registers();
        }
    }
    else if (b & BIT(6)) // Set effect
    {
        if (gbt_channel_4_set_effect(b & 0xF, *data++))
            channel4_refresh_registers();
    }
    else if (b & BIT(5)) // Set volume
    {
        gbt_data.vol[3] = (b & 0xF) << 12;
        channel4_refresh_registers();
    }
    else
    {
        // NOP
    }

    return data;
}

// Returns 1 if it needed to update sound registers
static int channel4_update_effects(void)
{
    // Cut note
    // --------

    if (gbt_data.cut_note_tick[3] == gbt_data.ticks_elapsed)
    {
        gbt_data.cut_note_tick[3] = 0xFF; // Disable cut note

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
    if (gbt_data.playing == 0)
        return;

    // Handle tick counter

    gbt_data.ticks_elapsed++;
    if (gbt_data.ticks_elapsed != gbt_data.speed)
    {
        // Update effects and exit
        gbt_update_effects_internal();
        return;
    }

    gbt_data.ticks_elapsed = 0; // reset tick counter

    // Clear tick-based effects
    // ------------------------

    gbt_data.arpeggio_enabled[0] = 0; // Disable arpeggio
    gbt_data.arpeggio_enabled[1] = 0;
    gbt_data.arpeggio_enabled[2] = 0;

    gbt_data.cut_note_tick[0] = 0xFF; // Disable cut note
    gbt_data.cut_note_tick[1] = 0xFF;
    gbt_data.cut_note_tick[2] = 0xFF;
    gbt_data.cut_note_tick[3] = 0xFF;

    // Update effects
    // --------------

    gbt_update_effects_internal();

    // Check if last step
    // ------------------

    if (gbt_data.have_to_stop_next_step)
    {
        gbt_stop();
        gbt_data.have_to_stop_next_step = 0;
        return;
    }

    // Increment step/pattern
    // ----------------------

    gbt_data.current_step++;
    if (gbt_data.current_step == 64)
    {
        // Increment pattern

        gbt_data.current_step = 0;

        gbt_data.current_pattern++;

        gbt_get_pattern_ptr(gbt_data.current_pattern);

        if (gbt_data.current_step_data_ptr == NULL)
        {
            // The song has ended

            if (gbt_data.loop_enabled)
            {
                // If loop is enabled, jump to pattern 0
                gbt_data.current_pattern = 0;
                gbt_get_pattern_ptr(gbt_data.current_pattern);
            }
            else
            {
                // If loop is disabled, stop song
                // Stop it next step, if not this step won't be played

                gbt_data.have_to_stop_next_step = 1;
            }
        }
    }

    // Update channels

    const uint8_t *ptr = gbt_data.current_step_data_ptr;

    ptr = gbt_channel_1_handle(ptr);
    ptr = gbt_channel_2_handle(ptr);
    ptr = gbt_channel_3_handle(ptr);
    ptr = gbt_channel_4_handle(ptr);

    gbt_data.current_step_data_ptr = ptr;

    // Handle panning

    uint16_t mask =
        SOUNDCNT_L_PSG_1_ENABLE_RIGHT | SOUNDCNT_L_PSG_1_ENABLE_LEFT |
        SOUNDCNT_L_PSG_2_ENABLE_RIGHT | SOUNDCNT_L_PSG_2_ENABLE_LEFT |
        SOUNDCNT_L_PSG_3_ENABLE_RIGHT | SOUNDCNT_L_PSG_3_ENABLE_LEFT |
        SOUNDCNT_L_PSG_4_ENABLE_RIGHT | SOUNDCNT_L_PSG_4_ENABLE_LEFT;

    uint16_t new_pan = gbt_data.pan[0] | gbt_data.pan[1] | gbt_data.pan[2] | gbt_data.pan[3];

    REG_SOUNDCNT_L = (REG_SOUNDCNT_L & ~mask) | (new_pan << 8);

    // Check if any effect has changed the pattern or step
    if (gbt_data.update_pattern_pointers)
    {
        gbt_data.update_pattern_pointers = 0; // clear flag

        gbt_data.have_to_stop_next_step = 0; // clear stop flag

        gbt_get_pattern_ptr(gbt_data.current_pattern); // set ptr to start of pattern

        // Search the step

        const uint8_t *src_search = gbt_data.current_step_data_ptr;

        for (int i = 0; i < 4 * gbt_data.current_step; i++)
        {
            uint8_t b = *src_search++;

            if (b & BIT(7))
            {
                b = *src_search++;

                if (b & BIT(7))
                    src_search++;
            }
            else if (b & BIT(6))
            {
                src_search++;
            }
        }

        gbt_data.current_step_data_ptr = src_search;
    }
}
