/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_AUDIO_H
#define BN_HW_AUDIO_H

#include "bn_common.h"
#include "maxmod.h"

namespace bn::hw::audio
{
    using func_type = void(*)();

    void init(func_type hp_vblank_function, func_type lp_vblank_function);

    void enable();

    void disable();

    [[nodiscard]] inline bool music_playing()
    {
        return mmActive();
    }

    inline void play_music(int id, int volume, bool loop)
    {
        if(mmActive())
        {
            mmStop();
        }

        mmStart(mm_word(id), loop ? MM_PLAY_LOOP : MM_PLAY_ONCE);
        mmSetModuleVolume(mm_word(volume));
    }

    inline void stop_music()
    {
        mmStop();
    }

    inline void pause_music()
    {
        mmPause();
    }

    inline void resume_music()
    {
        mmResume();
    }

    [[nodiscard]] inline int music_position()
    {
        return int(mmGetPosition());
    }

    inline void set_music_position(int position)
    {
        mmSetPosition(mm_word(position));
    }

    inline void set_music_volume(int volume)
    {
        mmSetModuleVolume(mm_word(volume));
    }

    void play_sound(int priority, int id);

    void play_sound(int priority, int id, int volume, int speed, int panning);

    void stop_all_sounds();

    [[nodiscard]] bool update_on_vblank();

    void set_update_on_vblank(bool update_on_vblank);

    void disable_vblank_handler();

    void update();

    void commit();
}

#endif
