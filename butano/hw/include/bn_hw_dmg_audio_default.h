/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_DMG_AUDIO_DEFAULT_H
#define BN_HW_DMG_AUDIO_DEFAULT_H

#include "bn_fixed.h"
#include "bn_hw_tonc.h"

namespace bn
{
    enum class dmg_music_type : uint8_t;
    enum class dmg_music_master_volume : uint8_t;
}

namespace bn::hw::dmg_audio
{
    void init();

    void enable();

    void disable();

    [[nodiscard]] bool music_playing();

    void stop_music();

    void play_music(const void* song, dmg_music_type type, int speed, bool loop);

    void pause_music();

    void resume_music();

    void music_position(int& pattern, int& row);

    void set_music_position(int pattern, int row);

    void set_music_volume(fixed left_volume, fixed right_volume);

    inline void set_music_master_volume(dmg_music_master_volume volume)
    {
        uint16_t snddscnt = REG_SNDDSCNT;
        snddscnt &= 0b1111111111111100;
        snddscnt += uint16_t(volume);
        REG_SNDDSCNT = snddscnt;
    }

    inline void stop()
    {
        stop_music();
    }

    void commit();

    void check_commit_result();
}

#endif
