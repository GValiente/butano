/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_DMG_AUDIO_NULL_H
#define BN_HW_DMG_AUDIO_NULL_H

#include "bn_fixed.h"

namespace bn
{
    enum class dmg_music_type : uint8_t;
    enum class dmg_music_master_volume : uint8_t;
}

namespace bn::hw::dmg_audio
{
    inline void init()
    {
    }

    inline void enable()
    {
    }

    inline void disable()
    {
    }

    [[nodiscard]] inline bool music_playing()
    {
        return false;
    }

    inline void stop_music()
    {
    }

    inline void play_music(const void*, dmg_music_type, int, bool)
    {
    }

    inline void pause_music()
    {
    }

    inline void resume_music()
    {
    }

    inline void music_position(int& pattern, int& row)
    {
        pattern = 0;
        row = 0;
    }

    inline void set_music_position(int, int)
    {
    }

    inline void set_music_volume(fixed, fixed)
    {
    }

    inline void set_music_master_volume(dmg_music_master_volume)
    {
    }

    inline void stop()
    {
    }

    inline void commit()
    {
    }

    inline void check_commit_result()
    {
    }
}

#endif
