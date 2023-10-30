// GBT Player v4.4.1
//
// SPDX-License-Identifier: MIT
//
// Copyright (c) 2009-2022 Antonio Niño Díaz <antonio_nd@outlook.com>

#include <stdint.h>

#ifdef GBT_USE_LIBUGBA
# include <ugba/ugba.h>
#else
# include "../include/gbt_hardware.h"
#endif

#include "../include/gbt_player.h"

typedef int (*effect_handler)(uint32_t args);

// Player state
// ============

typedef struct {

    // Array of commands to run before the start of the song (or NULL)
    const uint8_t *startup_cmds_ptr;

    // Pointer to the pattern pointer array
    uint8_t * const *pattern_array_ptr;

    // Pointer to next row data
    const uint8_t *current_row_data_ptr;

    uint8_t playing;
    uint8_t loop_enabled;

    uint8_t speed;

    uint8_t ticks_elapsed;
    uint8_t current_row;
    uint8_t current_order;

    // This keeps track of the step that has just been executed so that it can
    // be returned by gbt_get_position().
    uint8_t previous_row;
    uint8_t previous_order;

    // This is initialized to 0 before main() is called, so all channels are
    // enabled by default.
    uint8_t channels_disabled;

    struct {
        uint16_t pan;
        uint16_t vol;
        uint16_t instr;
        uint16_t freq; // Active frequence
        uint16_t base_freq; // Original frequence read from the pattern

        // Arpeggio
        uint8_t arpeggio_freq_index[3]; // { base_index, index + x, index + y }
        uint8_t arpeggio_enabled; // if 0, disabled
        uint8_t arpeggio_tick;

        // Vibrato
        uint8_t vibrato_enabled; // if 0, disabled
        uint8_t vibrato_position;
        uint8_t vibrato_args;

        // Volume slide
        uint16_t volslide_args;

        // Cut note
        uint8_t cut_note_tick; // If tick == gbt.cut_note_tick, stop note.
    } ch1, ch2;

    struct {
        uint16_t pan;
        uint16_t vol;
        uint16_t instr;
        uint16_t freq; // Active frequence
        uint16_t base_freq; // Original frequence read from the pattern

        // Arpeggio
        uint8_t arpeggio_freq_index[3]; // { base_index, index + x, index + y }
        uint8_t arpeggio_enabled; // if 0, disabled
        uint8_t arpeggio_tick;

        // Vibrato
        uint8_t vibrato_enabled; // if 0, disabled
        uint8_t vibrato_position;
        uint8_t vibrato_args;

        // Cut note
        uint8_t cut_note_tick; // If tick == gbt.cut_note_tick, stop note.

        // Currently loaded instrument (0xFF if none)
        uint8_t loaded_instrument;

        // Channel 3 instruments
        uint8_t instrument_flags[8];
        const uint8_t *instrument[8];
    } ch3;

    struct {
        uint16_t pan;
        uint16_t vol;
        uint16_t instr;

        // Volume slide
        uint16_t volslide_args;

        // Cut note
        uint8_t cut_note_tick; // If tick == gbt.cut_note_tick, stop note.
    } ch4;

    // The volume setting in REG_SOUNDCNT_L can't actually silence the music.
    // The best it can do is set it to 1/8th of the maximum volume. To be able
    // to reach zero, it is needed to use the flags that enable output to the
    // left and right speakers.
    uint16_t global_volume;
    uint16_t pan_volume_mask;

    uint8_t jump_requested; // set to 1 by jump effects
    uint8_t jump_target_row;
    uint8_t jump_target_order;

    gbt_event_callback event_callback;

} gbt_player_info_t;

EWRAM_BSS static gbt_player_info_t gbt;

// Player constants
// ================

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

// Channel handling routines
// =========================

static void channel1_refresh_registers(void)
{
    REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_1_ENABLE_RIGHT | SOUNDCNT_L_PSG_1_ENABLE_LEFT);
    // Panning is set to the right values at the end of the row handling

    REG_SOUND1CNT_L = 0;
    REG_SOUND1CNT_H = gbt.ch1.instr | gbt.ch1.vol | gbt.ch1.volslide_args;
    REG_SOUND1CNT_X = SOUND1CNT_X_RESTART | gbt.ch1.freq;
}

static void channel1_silence(void)
{
    REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_1_ENABLE_RIGHT | SOUNDCNT_L_PSG_1_ENABLE_LEFT);

    REG_SOUND1CNT_L = 0;
    REG_SOUND1CNT_H = 0; // Set volume to 0
    REG_SOUND1CNT_X = SOUND1CNT_X_RESTART;
}

static void channel2_refresh_registers(void)
{
    REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_2_ENABLE_RIGHT | SOUNDCNT_L_PSG_2_ENABLE_LEFT);
    // Panning is set to the right values at the end of the row handling

    REG_SOUND2CNT_L = gbt.ch2.instr | gbt.ch2.vol | gbt.ch2.volslide_args;
    REG_SOUND2CNT_H = SOUND2CNT_H_RESTART | gbt.ch2.freq;
}

static void channel2_silence(void)
{
    REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_2_ENABLE_RIGHT | SOUNDCNT_L_PSG_2_ENABLE_LEFT);

    REG_SOUND2CNT_L = 0; // Set volume to 0
    REG_SOUND2CNT_H = SOUND2CNT_H_RESTART;
}

static void channel3_refresh_registers(void)
{
    // On the GBA, the output of channel 3 is inverted. This causes the channel
    // to output a loud spike when disabled. It’s a good idea to "remove" the
    // channel using NR51 (SOUNDCNT_L) before refreshing wave RAM.

    REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_3_ENABLE_RIGHT | SOUNDCNT_L_PSG_3_ENABLE_LEFT);
    // Panning is set to the right values at the end of the row handling

    uint32_t instr = gbt.ch3.instr;

    if (gbt.ch3.loaded_instrument != instr)
    {
        const uint8_t *wave = gbt.ch3.instrument[instr];
        uint8_t flags = gbt.ch3.instrument_flags[instr];

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
        gbt.ch3.loaded_instrument = instr;
    }

    REG_SOUND3CNT_H = gbt.ch3.vol;
    REG_SOUND3CNT_X = SOUND3CNT_X_RESTART | gbt.ch3.freq;
}

static void channel3_silence(void)
{
    REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_3_ENABLE_RIGHT | SOUNDCNT_L_PSG_3_ENABLE_LEFT);

    REG_SOUND3CNT_X = SOUND3CNT_X_RESTART;
    REG_SOUND3CNT_H = SOUND3CNT_H_VOLUME_0;
    REG_SOUND3CNT_X = SOUND3CNT_X_RESTART;
}

static void channel4_refresh_registers(void)
{
    REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_4_ENABLE_RIGHT | SOUNDCNT_L_PSG_4_ENABLE_LEFT);
    // Panning is set to the right values at the end of the row handling

    REG_SOUND4CNT_L = gbt.ch4.vol | gbt.ch4.volslide_args; // Volume slide index 2
    REG_SOUND4CNT_H = SOUND4CNT_H_RESTART | gbt.ch4.instr;
}

static void channel4_silence(void)
{
    REG_SOUNDCNT_L &= ~(SOUNDCNT_L_PSG_4_ENABLE_RIGHT | SOUNDCNT_L_PSG_4_ENABLE_LEFT);

    REG_SOUND4CNT_L = 0; // Set volume to 0
    REG_SOUND4CNT_H = SOUND4CNT_H_RESTART;
}

// Player routines
// ===============

static void gbt_refresh_pattern_ptr(void)
{
    const uint8_t *src_search = gbt.pattern_array_ptr[gbt.current_order];

    // Seek the requested row

    for (int i = 0; i < gbt.current_row; i++)
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

    // Save pointer to current row

    gbt.current_row_data_ptr = src_search;
}

#define STARTUP_CMD_DONE                0
#define STARTUP_CMD_SPEED               1
#define STARTUP_CMD_PANING              2
#define STARTUP_CMD_CHANNEL3_INSTRUMENT 3

#define SAMPLE_64_ENTRIES   BIT(7)

static void gbt_run_startup_commands(const uint8_t *ptr)
{
    if (ptr == NULL)
        return;

    while (1)
    {
        uint8_t cmd = *ptr++;

        if (cmd == STARTUP_CMD_DONE) // End setup
        {
            break;
        }
        else if (cmd == STARTUP_CMD_SPEED) // Initial speed
        {
            gbt.speed = *ptr++;
        }
        else if (cmd == STARTUP_CMD_PANING) // Initial panning
        {
            gbt.ch1.pan = *ptr++;
            gbt.ch2.pan = *ptr++;
            gbt.ch3.pan = *ptr++;
            gbt.ch4.pan = *ptr++;
        }
        else if (cmd == STARTUP_CMD_CHANNEL3_INSTRUMENT) // Set channel 3 sample
        {
            uint8_t flags = *ptr++;
            uint8_t index = flags & 0x3F;
            gbt.ch3.instrument_flags[index] = flags;

            gbt.ch3.instrument[index] = ptr;

            uint32_t len = 32 / 2;
            if (flags & SAMPLE_64_ENTRIES)
                len = 64 / 2;

            ptr += len;
        }
    }
}

void gbt_play(const void *song, int speed)
{
    // If the user hasn't specified a speed, set the default value for MOD
    // songs. In S3M songs this value is specified in the S3M file so it is
    // overwriten during the startup commands handling.

    if (speed <= 0)
        speed = 6;

    // Load pointers to startup commands and to list of patterns

    uint8_t * const *ptr = song;

    gbt.startup_cmds_ptr = ptr[0];
    gbt.pattern_array_ptr = ptr;
    gbt.pattern_array_ptr++;

    // Initialize player state

    gbt.loop_enabled = 0;

    gbt.speed = speed; // This may be overwriten by gbt_run_startup_commands()

    gbt.ticks_elapsed = 0;
    gbt.current_row = 0;
    gbt.current_order = 0;

    gbt.previous_row = 0;
    gbt.previous_order = 0;

    gbt_refresh_pattern_ptr();

    gbt.ch3.loaded_instrument = 0xFF;

    gbt.event_callback = NULL;

    gbt.ch1.pan = 0x11; // L and R
    gbt.ch2.pan = 0x22;
    gbt.ch3.pan = 0x44;
    gbt.ch4.pan = 0x88;

    gbt.ch1.vol = 0xF000; // 100%
    gbt.ch2.vol = 0xF000;
    gbt.ch3.vol = 0x2000;
    gbt.ch4.vol = 0xF000;

    gbt.ch1.instr = 0;
    gbt.ch2.instr = 0;
    gbt.ch3.instr = 0;
    gbt.ch4.instr = 0;

    gbt.ch1.freq = 0;
    gbt.ch2.freq = 0;
    gbt.ch3.freq = 0;

    gbt.ch1.arpeggio_enabled = 0;
    gbt.ch2.arpeggio_enabled = 0;
    gbt.ch3.arpeggio_enabled = 0;

    gbt.ch1.vibrato_enabled = 0;
    gbt.ch2.vibrato_enabled = 0;
    gbt.ch3.vibrato_enabled = 0;

    gbt.ch1.vibrato_position = 0;
    gbt.ch2.vibrato_position = 0;
    gbt.ch3.vibrato_position = 0;

    gbt.ch1.vibrato_args = 0;
    gbt.ch2.vibrato_args = 0;
    gbt.ch3.vibrato_args = 0;

    gbt.ch1.volslide_args = 0;
    gbt.ch2.volslide_args = 0;
    gbt.ch4.volslide_args = 0;

    gbt.ch1.cut_note_tick = 0xFF;
    gbt.ch2.cut_note_tick = 0xFF;
    gbt.ch3.cut_note_tick = 0xFF;
    gbt.ch4.cut_note_tick = 0xFF;

    gbt.jump_requested = 0;

    // Default channel 3 instruments

    for (int i = 0; i < 8; i++)
        gbt.ch3.instrument_flags[i] = 0;

    gbt.ch3.instrument[0] = &gbt_default_wave_0[0];
    gbt.ch3.instrument[1] = &gbt_default_wave_1[0];
    gbt.ch3.instrument[2] = &gbt_default_wave_2[0];
    gbt.ch3.instrument[3] = &gbt_default_wave_3[0];
    gbt.ch3.instrument[4] = &gbt_default_wave_4[0];
    gbt.ch3.instrument[5] = &gbt_default_wave_5[0];
    gbt.ch3.instrument[6] = &gbt_default_wave_6[0];
    gbt.ch3.instrument[7] = &gbt_default_wave_7[0];

    // Run startup commands after internal player status has been initialized

    gbt_run_startup_commands(gbt.startup_cmds_ptr);

    // Initialize hardware registers

    REG_SOUNDCNT_X = SOUNDCNT_X_MASTER_ENABLE;

    // Silence all PSG channels controlled by GBT Player
    uint16_t enabled_ch_mask = GBT_ENABLE_CH_ALL & ~gbt.channels_disabled;
    enabled_ch_mask = (enabled_ch_mask << 8 | enabled_ch_mask << 12);
    REG_SOUNDCNT_L = REG_SOUNDCNT_L & ~enabled_ch_mask;

    // Don't enable all channels. Maybe the user has disabled a channel before
    // the song playback starts. This variable is set to 0 when the program
    // starts, so it will only be modified if the user actually calls
    // gbt_enable_channels(). If not, all channels will always be enabled.
    //
    // gbt.channels_disabled = 0;
    //
    // Only silence channels that are owned by GBT Player.
    if ((gbt.channels_disabled & GBT_ENABLE_CH1) == 0)
        channel1_silence();
    if ((gbt.channels_disabled & GBT_ENABLE_CH2) == 0)
        channel2_silence();
    if ((gbt.channels_disabled & GBT_ENABLE_CH3) == 0)
        channel3_silence();
    if ((gbt.channels_disabled & GBT_ENABLE_CH4) == 0)
        channel4_silence();

    gbt_volume(GBT_VOLUME_MAX, GBT_VOLUME_MAX);

    // Force refresh as soon as possible

    gbt.ticks_elapsed = gbt.speed - 1;

    // Everything is ready

    gbt.playing = 1;
}

void gbt_set_event_callback_handler(gbt_event_callback callback)
{
    gbt.event_callback = callback;
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

int gbt_is_playing(void)
{
    return gbt.playing;
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
        uint16_t new_pan = gbt.ch1.pan | gbt.ch2.pan | gbt.ch3.pan | gbt.ch4.pan;
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

    // Silence all PSG channels controlled by GBT Player
    uint16_t enabled_ch_mask = GBT_ENABLE_CH_ALL & ~gbt.channels_disabled;
    enabled_ch_mask = (enabled_ch_mask << 8 | enabled_ch_mask << 12);
    REG_SOUNDCNT_L = REG_SOUNDCNT_L & ~enabled_ch_mask;

    // Don't do this! The sound hardware should be left on in case there is
    // something else using it.
    // REG_SOUNDCNT_X = SOUNDCNT_X_MASTER_DISABLE;
}

void gbt_enable_channels(int flags)
{
    gbt.channels_disabled = GBT_ENABLE_CH_ALL & ~flags;
}

static uint16_t gbt_get_freq_from_index(int index)
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
    gbt.jump_target_row = 0;
    gbt.jump_target_order = args;

    return 0;
}

static int gbt_ch1234_jump_position(uint32_t args)
{
    gbt.jump_requested = 1;
    gbt.jump_target_row = args;
    gbt.jump_target_order = gbt.current_order + 1;

    return 0;
}

static int gbt_ch1234_event(uint32_t args)
{
    if (gbt.event_callback)
        gbt.event_callback(args, gbt.current_order, gbt.current_row);

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
    gbt.ch1.pan = args & 0x11;
    return 0; // Panning is always updated
}

static int gbt_ch1_cut_note(uint32_t args)
{
    gbt.ch1.cut_note_tick = args;
    return 0;
}

static int gbt_ch1_arpeggio(uint32_t args)
{
    uint32_t base_index = gbt.ch1.arpeggio_freq_index[0];
    gbt.ch1.arpeggio_freq_index[1] = base_index + ((args >> 4) & 0xF);
    gbt.ch1.arpeggio_freq_index[2] = base_index + (args & 0xF);

    gbt.ch1.arpeggio_enabled = 1;
    gbt.ch1.arpeggio_tick = 1;

    return 1;
}

static int gbt_ch1_vibrato(uint32_t args)
{
    if (args != 0)
    {
        gbt.ch1.vibrato_position = 0;
        gbt.ch1.vibrato_args = args;
    }

    gbt.ch1.vibrato_enabled = 1;

    return 1;
}

static int gbt_ch1_volslide(uint32_t args)
{
    gbt.ch1.volslide_args = args << 8; // Move to the right location
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
    gbt_ch1234_event,
};

// returns 1 if needed to update registers, 0 if not
static int gbt_channel_1_set_effect(uint32_t effect, uint8_t data)
{
    return gbt_ch1_jump_table[effect](data);
}

static const uint8_t *gbt_channel_1_handle(const uint8_t *data)
{
    // Calculate pointer to next channel
    // Note: The volume bit doesn't affect the final size.
    const uint8_t sizes[8] = { 1, 2, 3, 3, 2, 3, 4, 4 };
    uint8_t bits = (*data) >> 5;
    const uint8_t *next = data + sizes[bits];

    // If the channel is disabled, exit
    if (gbt.channels_disabled & GBT_ENABLE_CH1)
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
        gbt.ch1.vol = ((uint16_t)(header & 0xF)) << 12;
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
            gbt.ch1.arpeggio_freq_index[0] = index;
            gbt.ch1.base_freq = gbt_get_freq_from_index(index);
            gbt.ch1.freq = gbt.ch1.base_freq;
            has_to_update_registers = 1;
        }
    }

    if (has_instrument)
    {
        gbt.ch1.instr = (*data & 0x30) << 2;
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
        channel1_silence();
    }
    else if (has_to_update_registers)
    {
        channel1_refresh_registers();
    }

    return next;
}

static void channel1_update_effects(void)
{
    int update_registers = 0;

    // Cut note
    // --------

    if (gbt.ch1.cut_note_tick == gbt.ticks_elapsed)
    {
        gbt.ch1.cut_note_tick = 0xFF; // Disable cut note

        channel1_silence();
    }

    // Arpeggio
    // --------

    if (gbt.ch1.arpeggio_enabled)
    {
        uint8_t tick = gbt.ch1.arpeggio_tick;
        if (tick == 2)
            gbt.ch1.arpeggio_tick = 0;
        else
            gbt.ch1.arpeggio_tick = tick + 1;

        uint32_t index = gbt.ch1.arpeggio_freq_index[tick];
        gbt.ch1.freq = gbt_get_freq_from_index(index);

        update_registers = 1;
    }

    // Vibrato
    // -------

    if (gbt.ch1.vibrato_enabled)
    {
        gbt.ch1.vibrato_position += (gbt.ch1.vibrato_args >> 4);
        gbt.ch1.vibrato_position &= 63;

        int32_t pos = gbt.ch1.vibrato_position;
        int32_t delta = vibrato_sine[pos];
        delta *= (int32_t)(gbt.ch1.vibrato_args & 0xF);
        delta >>= 7;

        int32_t freq = (int32_t)gbt.ch1.base_freq - delta;
        if (freq < 0)
            freq = 0;
        else if (freq > 0x7FF)
            freq = 0x7FF;
        gbt.ch1.freq = freq;

        update_registers = 1;
    }

    // Update channel registers if needed
    // ----------------------------------

    if (update_registers)
        channel1_refresh_registers();
}

// -----------------------------------------------------------------------------
// --------------------------------- Channel 2 ---------------------------------
// -----------------------------------------------------------------------------

static int gbt_ch2_pan(uint32_t args)
{
    gbt.ch2.pan = args & 0x22;
    return 0; // Panning is always updated
}

static int gbt_ch2_cut_note(uint32_t args)
{
    gbt.ch2.cut_note_tick = args;
    return 0;
}

static int gbt_ch2_arpeggio(uint32_t args)
{
    uint32_t base_index = gbt.ch2.arpeggio_freq_index[0];
    gbt.ch2.arpeggio_freq_index[1] = base_index + ((args >> 4) & 0xF);
    gbt.ch2.arpeggio_freq_index[2] = base_index + (args & 0xF);

    gbt.ch2.arpeggio_enabled = 1;
    gbt.ch2.arpeggio_tick = 1;

    return 1;
}

static int gbt_ch2_vibrato(uint32_t args)
{
    if (args != 0)
    {
        gbt.ch2.vibrato_position = 0;
        gbt.ch2.vibrato_args = args;
    }

    gbt.ch2.vibrato_enabled = 1;

    return 1;
}

static int gbt_ch2_volslide(uint32_t args)
{
    gbt.ch2.volslide_args = args << 8; // Move to the right location
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
    gbt_ch1234_event,
};

// returns 1 if needed to update registers, 0 if not
static int gbt_channel_2_set_effect(uint32_t effect, uint8_t data)
{
    return gbt_ch2_jump_table[effect](data);
}

static const uint8_t *gbt_channel_2_handle(const uint8_t *data)
{
    // Calculate pointer to next channel
    // Note: The volume bit doesn't affect the final size.
    const uint8_t sizes[8] = { 1, 2, 3, 3, 2, 3, 4, 4 };
    uint8_t bits = (*data) >> 5;
    const uint8_t *next = data + sizes[bits];

    // If the channel is disabled, exit
    if (gbt.channels_disabled & GBT_ENABLE_CH2)
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
        gbt.ch2.vol = ((uint16_t)(header & 0xF)) << 12;
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
            gbt.ch2.arpeggio_freq_index[0] = index;
            gbt.ch2.base_freq = gbt_get_freq_from_index(index);
            gbt.ch2.freq = gbt.ch2.base_freq;
            has_to_update_registers = 1;
        }
    }

    if (has_instrument)
    {
        gbt.ch2.instr = (*data & 0x30) << 2;
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
        channel2_silence();
    }
    else if (has_to_update_registers)
    {
        channel2_refresh_registers();
    }

    return next;
}

static void channel2_update_effects(void)
{
    int update_registers = 0;

    // Cut note
    // --------

    if (gbt.ch2.cut_note_tick == gbt.ticks_elapsed)
    {
        gbt.ch2.cut_note_tick = 0xFF; // Disable cut note

        channel2_silence();
    }

    // Arpeggio
    // --------

    if (gbt.ch2.arpeggio_enabled)
    {
        uint8_t tick = gbt.ch2.arpeggio_tick;
        if (tick == 2)
            gbt.ch2.arpeggio_tick = 0;
        else
            gbt.ch2.arpeggio_tick = tick + 1;

        uint32_t index = gbt.ch2.arpeggio_freq_index[tick];
        gbt.ch2.freq = gbt_get_freq_from_index(index);

        update_registers = 1;
    }

    // Vibrato
    // -------

    if (gbt.ch2.vibrato_enabled)
    {
        gbt.ch2.vibrato_position += (gbt.ch2.vibrato_args >> 4);
        gbt.ch2.vibrato_position &= 63;

        int32_t pos = gbt.ch2.vibrato_position;
        int32_t delta = vibrato_sine[pos];
        delta *= (int32_t)(gbt.ch2.vibrato_args & 0xF);
        delta >>= 7;

        int32_t freq = (int32_t)gbt.ch2.base_freq - delta;
        if (freq < 0)
            freq = 0;
        else if (freq > 0x7FF)
            freq = 0x7FF;
        gbt.ch2.freq = freq;

        update_registers = 1;
    }

    // Update channel registers if needed
    // ----------------------------------

    if (update_registers)
        channel2_refresh_registers();
}

// -----------------------------------------------------------------------------
// --------------------------------- Channel 3 ---------------------------------
// -----------------------------------------------------------------------------

static int gbt_ch3_pan(uint32_t args)
{
    gbt.ch3.pan = args & 0x44;
    return 0; // Panning is always updated
}

static int gbt_ch3_cut_note(uint32_t args)
{
    gbt.ch3.cut_note_tick = args;
    return 0;
}

static int gbt_ch3_arpeggio(uint32_t args)
{
    uint32_t base_index = gbt.ch3.arpeggio_freq_index[0];
    gbt.ch3.arpeggio_freq_index[1] = base_index + ((args >> 4) & 0xF);
    gbt.ch3.arpeggio_freq_index[2] = base_index + (args & 0xF);

    gbt.ch3.arpeggio_enabled = 1;
    gbt.ch3.arpeggio_tick = 1;

    return 1;
}

static int gbt_ch3_vibrato(uint32_t args)
{
    if (args != 0)
    {
        gbt.ch3.vibrato_position = 0;
        gbt.ch3.vibrato_args = args;
    }

    gbt.ch3.vibrato_enabled = 1;

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
    gbt_ch1234_event,
};

// returns 1 if needed to update registers, 0 if not
static int gbt_channel_3_set_effect(uint32_t effect, uint8_t data)
{
    return gbt_ch3_jump_table[effect](data);
}

static const uint8_t *gbt_channel_3_handle(const uint8_t *data)
{
    // Calculate pointer to next channel
    // Note: The volume bit doesn't affect the final size.
    const uint8_t sizes[8] = { 1, 2, 3, 3, 2, 3, 4, 4 };
    uint8_t bits = (*data) >> 5;
    const uint8_t *next = data + sizes[bits];

    // If the channel is disabled, exit
    if (gbt.channels_disabled & GBT_ENABLE_CH3)
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
        gbt.ch3.vol = ((uint16_t)(header & 0x7)) << 13;
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
            gbt.ch3.arpeggio_freq_index[0] = index;
            gbt.ch3.base_freq = gbt_get_freq_from_index(index);
            gbt.ch3.freq = gbt.ch3.base_freq;
            has_to_update_registers = 1;
        }
    }

    if (has_instrument)
    {
        gbt.ch3.instr = (*data & 0x70) >> 4;
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
        channel3_silence();
    }
    else if (has_to_update_registers)
    {
        channel3_refresh_registers();
    }

    return next;
}

static void channel3_update_effects(void)
{
    int update_registers = 0;

    // Cut note
    // --------

    if (gbt.ch3.cut_note_tick == gbt.ticks_elapsed)
    {
        gbt.ch3.cut_note_tick = 0xFF; // Disable cut note

        channel3_silence();
    }

    // Arpeggio
    // --------

    if (gbt.ch3.arpeggio_enabled)
    {
        uint8_t tick = gbt.ch3.arpeggio_tick;
        if (tick == 2)
            gbt.ch3.arpeggio_tick = 0;
        else
            gbt.ch3.arpeggio_tick = tick + 1;

        uint32_t index = gbt.ch3.arpeggio_freq_index[tick];
        gbt.ch3.freq = gbt_get_freq_from_index(index);

        update_registers = 1;
    }

    // Vibrato
    // -------

    if (gbt.ch3.vibrato_enabled)
    {
        gbt.ch3.vibrato_position += (gbt.ch3.vibrato_args >> 4);
        gbt.ch3.vibrato_position &= 63;

        int32_t pos = gbt.ch3.vibrato_position;
        int32_t delta = vibrato_sine[pos];
        delta *= (int32_t)(gbt.ch3.vibrato_args & 0xF);
        delta >>= 7;

        int32_t freq = (int32_t)gbt.ch3.base_freq - delta;
        if (freq < 0)
            freq = 0;
        else if (freq > 0x7FF)
            freq = 0x7FF;
        gbt.ch3.freq = freq;

        update_registers = 1;
    }

    // Update channel registers if needed
    // ----------------------------------

    if (update_registers)
        channel3_refresh_registers();
}

// -----------------------------------------------------------------------------
// --------------------------------- Channel 4 ---------------------------------
// -----------------------------------------------------------------------------

static int gbt_ch4_pan(uint32_t args)
{
    gbt.ch4.pan = args & 0x88;
    return 0; // Panning is always updated
}

static int gbt_ch4_cut_note(uint32_t args)
{
    gbt.ch4.cut_note_tick = args;
    return 0;
}

static int gbt_ch4_volslide(uint32_t args)
{
    // Volume slide index 2
    gbt.ch4.volslide_args = args << 8; // Move to the right location
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
    gbt_ch1234_event,
};

// returns 1 if needed to update registers, 0 if not
static int gbt_channel_4_set_effect(uint32_t effect, uint8_t data)
{
    return gbt_ch4_jump_table[effect](data);
}

static const uint8_t *gbt_channel_4_handle(const uint8_t *data)
{
    // Calculate pointer to next channel
    // Note: The volume bit doesn't affect the final size.
    const uint8_t sizes[4] = { 1, 3, 2, 4 };
    uint8_t bits = (*data) >> 6;
    const uint8_t *next = data + sizes[bits];

    // If the channel is disabled, exit
    if (gbt.channels_disabled & GBT_ENABLE_CH4)
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
        gbt.ch4.vol = ((uint16_t)(header & 0xF)) << 12;
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
            gbt.ch4.instr = gbt_noise[index];
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
        channel4_silence();
    }
    else if (has_to_update_registers)
    {
        channel4_refresh_registers();
    }

    return next;
}

static void channel4_update_effects(void)
{
    int update_registers = 0;

    // Cut note
    // --------

    if (gbt.ch4.cut_note_tick == gbt.ticks_elapsed)
    {
        gbt.ch4.cut_note_tick = 0xFF; // Disable cut note

        channel4_silence();
    }

    // Update channel registers if needed
    // ----------------------------------

    if (update_registers)
        channel4_refresh_registers();
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

static void gbt_update_effects_internal(void)
{
    channel1_update_effects();
    channel2_update_effects();
    channel3_update_effects();
    channel4_update_effects();
}

static void gbt_update_refresh_panning(void)
{
    // This function needs to take care of three things:
    //
    // - Panning can be modified by effects in the song (saved in gbt.chX.pan).
    //
    // - The global volume of PSG channels can't reach 0, the only way to make
    //   it reach 0 is by setting all the left/right enable bits to 0. This is
    //   what gbt_volume() does.
    //
    // - Channels disabled by gbt_enable_channels() aren't owned by GBT Player,
    //   so their bits need to be preserved in case the user is modifying them
    //   manually.

    uint16_t enabled_ch_mask = GBT_ENABLE_CH_ALL & ~gbt.channels_disabled;
    enabled_ch_mask = (enabled_ch_mask << 8 | enabled_ch_mask << 12);

    uint16_t old_pan = REG_SOUNDCNT_L & (0xFF << 8);

    uint16_t new_pan = gbt.ch1.pan | gbt.ch2.pan | gbt.ch3.pan | gbt.ch4.pan;
    new_pan = (new_pan << 8) & gbt.pan_volume_mask;

    uint16_t result_pan = (old_pan & ~enabled_ch_mask)
                        | (new_pan & enabled_ch_mask);

    REG_SOUNDCNT_L = gbt.global_volume | result_pan;
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

    gbt.ch1.arpeggio_enabled = 0; // Disable arpeggio
    gbt.ch2.arpeggio_enabled = 0;
    gbt.ch3.arpeggio_enabled = 0;

    gbt.ch1.vibrato_enabled = 0; // Disable vibrato
    gbt.ch2.vibrato_enabled = 0;
    gbt.ch3.vibrato_enabled = 0;

    gbt.ch1.volslide_args = 0; // Disable volume slide
    gbt.ch2.volslide_args = 0;
    gbt.ch4.volslide_args = 0;

    gbt.ch1.cut_note_tick = 0xFF; // Disable cut note
    gbt.ch2.cut_note_tick = 0xFF;
    gbt.ch3.cut_note_tick = 0xFF;
    gbt.ch4.cut_note_tick = 0xFF;

    // Update effects
    // --------------

    gbt_update_effects_internal();

    // Check if the song has ended
    // ---------------------------

    if (gbt.current_row_data_ptr == NULL)
    {
        if (gbt.event_callback)
            gbt.event_callback(GBT_EVENT_END, -1, -1);

        if (gbt.loop_enabled)
        {
            // If loop is enabled, jump to pattern 0
            gbt.current_order = 0;
            gbt_refresh_pattern_ptr();
            gbt_run_startup_commands(gbt.startup_cmds_ptr);
        }
        else
        {
            // If loop is disabled, stop song
            gbt_stop();
            return;
        }
    }

    // Update saved position
    // ---------------------

    gbt.previous_row = gbt.current_row;
    gbt.previous_order = gbt.current_order;

    // Update channels
    // ---------------

    const uint8_t *ptr = gbt.current_row_data_ptr;

    ptr = gbt_channel_1_handle(ptr);
    ptr = gbt_channel_2_handle(ptr);
    ptr = gbt_channel_3_handle(ptr);
    ptr = gbt_channel_4_handle(ptr);

    gbt.current_row_data_ptr = ptr;

    // Handle panning
    // --------------

    gbt_update_refresh_panning();

    // Increment row
    // -------------

    if (gbt.jump_requested == 0)
    {
        gbt.current_row++;

        if (gbt.current_row == 64)
        {
            // Increment pattern
            gbt.current_row = 0;
            gbt.current_order++;
            gbt_refresh_pattern_ptr();
        }
    }
    else
    {
        gbt.jump_requested = 0;

        gbt.current_row = gbt.jump_target_row;
        gbt.current_order = gbt.jump_target_order;

        gbt_refresh_pattern_ptr();
    }
}

void gbt_get_position(int *order, int *row, int *tick)
{
    if (gbt.playing == 0)
    {
        if (order)
            *order = -1;
        if (row)
            *row = -1;
        if (tick)
            *tick = -1;
    }
    else
    {
        gbt_get_position_unsafe(order, row, tick);
    }
}

void gbt_get_position_unsafe(int *order, int *row, int *tick)
{
    if (order)
        *order = gbt.previous_order;
    if (row)
        *row = gbt.previous_row;
    if (tick)
        *tick = gbt.ticks_elapsed;
}

void gbt_set_position(int order, int row)
{
    if (gbt.playing == 0)
        return;

    // Silence channels until the next tick
    if (!(gbt.channels_disabled & GBT_ENABLE_CH1))
        channel1_silence();
    if (!(gbt.channels_disabled & GBT_ENABLE_CH2))
        channel2_silence();
    if (!(gbt.channels_disabled & GBT_ENABLE_CH3))
        channel3_silence();
    if (!(gbt.channels_disabled & GBT_ENABLE_CH4))
        channel4_silence();

    // Force refresh as soon as possible, in the next tick
    gbt.ticks_elapsed = gbt.speed - 1;

    gbt.current_row = row;
    gbt.current_order = order;

    gbt_refresh_pattern_ptr();
}
