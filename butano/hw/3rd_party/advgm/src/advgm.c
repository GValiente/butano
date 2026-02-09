// SPDX-FileCopyrightText: Copyright 2023-2025 akkera102
// SPDX-FileCopyrightText: Copyright 2026 copyrat90
// SPDX-License-Identifier: 0BSD

#include "../include/advgm.h"

#include "advgm_hardware.h"

#include <stddef.h>

enum advgm_music_cmd_t
{
    ADVGM_MUSIC_CMD_WAIT_FOR_VBLANK = 0x61,
    ADVGM_MUSIC_CMD_END_MARK = 0x66,
    ADVGM_MUSIC_CMD_WRITE_REG = 0xB3,
};

typedef struct advgm_player_t
{
    // Make sure `playing` is `true` before accessing these!
    const uint8_t* next_fetch_pos;
    const uint8_t* music;

    uint16_t snddmgcnt_on_pause;

    bool playing;
    bool paused;

    bool loop;

} advgm_player;

ADVGM_EWRAM_DATA static advgm_player player;

void advgm_set_master_volume(advgm_master_volume master_volume)
{
    ADVGM_REG_SNDDSCNT = (ADVGM_REG_SNDDSCNT & ~ADVGM_SNDDSCNT_PSG_VOLUME_MASK) |
                         ((unsigned)master_volume & ADVGM_SNDDSCNT_PSG_VOLUME_MASK);
}

advgm_master_volume advgm_get_master_volume()
{
    return (advgm_master_volume)(ADVGM_REG_SNDDSCNT & ADVGM_SNDDSCNT_PSG_VOLUME_MASK);
}

bool advgm_vblank_callback(void)
{
    if (!advgm_playing() || advgm_paused())
        return true;

    for (;;)
    {
        uint8_t command = *player.next_fetch_pos++;

        switch (command)
        {
        case ADVGM_MUSIC_CMD_WAIT_FOR_VBLANK:
            return true;

        case ADVGM_MUSIC_CMD_WRITE_REG: {
            const uint8_t offset = *player.next_fetch_pos++;
            uint8_t data = *player.next_fetch_pos++;

            // Ignore Ch3 bank switching done by music commands
            if (offset == ADVGM_OFFSET_SND3SEL)
            {
                const uint8_t prev_bank = ADVGM_SND3SEL_BANK_GET(ADVGM_REG_SND3SEL);
                const uint8_t non_bank_mask = ~ADVGM_SND3SEL_BANK_SET(1);

                data = (data & non_bank_mask) | ADVGM_SND3SEL_BANK_SET(prev_bank);
            }

            ADVGM_REG_8(offset) = data;

            // Automatically switch Ch3 bank after writing final byte of `WAVE_RAM`.
            if (offset == ADVGM_OFFSET_WAVE_RAM3_H + 1)
            {
                const uint8_t prev_bank = ADVGM_SND3SEL_BANK_GET(ADVGM_REG_SND3SEL);
                const uint8_t next_bank = (prev_bank == 0) ? 1 : 0;
                const uint8_t non_bank_mask = ~ADVGM_SND3SEL_BANK_SET(1);

                ADVGM_REG_SND3SEL = (ADVGM_REG_SND3SEL & non_bank_mask) | ADVGM_SND3SEL_BANK_SET(next_bank);
            }

            continue;
        }

        case ADVGM_MUSIC_CMD_END_MARK: {
            if (!player.loop)
            {
                advgm_stop();
                return true;
            }

            uint32_t loop_pos = *player.next_fetch_pos++;
            loop_pos |= *player.next_fetch_pos++ << 8;
            loop_pos |= *player.next_fetch_pos++ << 16;
            loop_pos |= *player.next_fetch_pos++ << 24;

            player.next_fetch_pos = player.music + loop_pos;
            continue;
        }

        default:
            return false;
        }
    }
}

void advgm_play(const uint8_t* music, bool loop)
{
    advgm_stop();

    if (music == NULL)
        return;

    player.music = music;
    player.next_fetch_pos = music;
    player.loop = loop;

    player.playing = true;
}

void advgm_stop(void)
{
    player.playing = false;
    player.paused = false;

    // `SNDSTAT_MASTER` must be enabled before touching anything
    ADVGM_REG_SNDSTAT = ADVGM_SNDSTAT_MASTER_ENABLE;

    // Channel 1
    ADVGM_REG_SND1SWEEP = ADVGM_SND1SWEEP_SWEEP_TIME_SET(0);
    ADVGM_REG_SND1CNT = ADVGM_SND1CNT_ENV_VOLUME_SET(0) | ADVGM_SND1CNT_ENV_DIR_INC;
    ADVGM_REG_SND1FREQ = ADVGM_SND1FREQ_RETRIG;

    // Channel 2
    ADVGM_REG_SND2CNT = ADVGM_SND2CNT_ENV_VOLUME_SET(0) | ADVGM_SND2CNT_ENV_DIR_INC;
    ADVGM_REG_SND2FREQ = ADVGM_SND2FREQ_RETRIG;

    // Channel 3
    ADVGM_REG_WAVE_RAM0 = 0x00000000;
    ADVGM_REG_WAVE_RAM1 = 0x00000000;
    ADVGM_REG_WAVE_RAM2 = 0x00000000;
    ADVGM_REG_WAVE_RAM3 = 0x00000000;
    ADVGM_REG_SND3SEL = ADVGM_SND3SEL_DISABLE;

    ADVGM_REG_SND3CNT = ADVGM_SND3CNT_VOLUME_0;
    ADVGM_REG_SND3FREQ = 0;

    // Channel 4
    ADVGM_REG_SND4CNT = ADVGM_SND4CNT_ENV_VOLUME_SET(0) | ADVGM_SND4CNT_ENV_DIR_INC;
    ADVGM_REG_SND4FREQ = ADVGM_SND4FREQ_RETRIG;

    // Master
    ADVGM_REG_SNDDMGCNT = ADVGM_SNDDMGCNT_PSG_VOL_LEFT_SET(7) | ADVGM_SNDDMGCNT_PSG_VOL_RIGHT_SET(7) |
                          ADVGM_SNDDMGCNT_PSG_1_ENABLE_LEFT | ADVGM_SNDDMGCNT_PSG_1_ENABLE_RIGHT |
                          ADVGM_SNDDMGCNT_PSG_2_ENABLE_LEFT | ADVGM_SNDDMGCNT_PSG_2_ENABLE_RIGHT |
                          ADVGM_SNDDMGCNT_PSG_3_ENABLE_LEFT | ADVGM_SNDDMGCNT_PSG_3_ENABLE_RIGHT |
                          ADVGM_SNDDMGCNT_PSG_4_ENABLE_LEFT | ADVGM_SNDDMGCNT_PSG_4_ENABLE_RIGHT;
}

void advgm_pause(void)
{
    if (!advgm_playing() || advgm_paused())
        return;

    player.paused = true;
    player.snddmgcnt_on_pause = ADVGM_REG_SNDDMGCNT;

    ADVGM_REG_SNDDMGCNT &=
        ~(ADVGM_SNDDMGCNT_PSG_1_ENABLE_LEFT | ADVGM_SNDDMGCNT_PSG_1_ENABLE_RIGHT | ADVGM_SNDDMGCNT_PSG_2_ENABLE_LEFT |
          ADVGM_SNDDMGCNT_PSG_2_ENABLE_RIGHT | ADVGM_SNDDMGCNT_PSG_3_ENABLE_LEFT | ADVGM_SNDDMGCNT_PSG_3_ENABLE_RIGHT |
          ADVGM_SNDDMGCNT_PSG_4_ENABLE_LEFT | ADVGM_SNDDMGCNT_PSG_4_ENABLE_RIGHT);
}

void advgm_resume(void)
{
    if (!advgm_paused())
        return;

    ADVGM_REG_SNDDMGCNT = player.snddmgcnt_on_pause;

    player.paused = false;
}

bool advgm_playing(void)
{
    return player.playing;
}

bool advgm_paused(void)
{
    return player.paused;
}

const uint8_t* advgm_get_active_music(void)
{
    if (!advgm_playing())
        return NULL;

    return player.music;
}

uint32_t advgm_get_music_offset(void)
{
    if (!advgm_playing())
        return 0;

    return (uint32_t)(player.next_fetch_pos - player.music);
}

void advgm_set_music_offset(uint32_t offset)
{
    if (!advgm_playing())
        return;

    player.next_fetch_pos = player.music + offset;
}
