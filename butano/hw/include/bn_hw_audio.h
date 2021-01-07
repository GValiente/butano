/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_AUDIO_H
#define BN_HW_AUDIO_H

#include "bn_common.h"

namespace bn::hw::audio
{
    typedef void (*func_type)(void);

    void init(func_type hp_vblank_function, func_type lp_vblank_function);

    void enable();

    void disable();

    [[nodiscard]] bool music_playing();

    void play_music(int id, int volume, bool loop);

    void stop_music();

    void pause_music();

    void resume_music();

    [[nodiscard]] int music_position();

    void set_music_position(int position);

    void set_music_volume(int volume);

    void play_sound(int priority, int id);

    void play_sound(int priority, int id, int volume, int speed, int panning);

    void stop_all_sounds();

    void disable_vblank_handler();

    void commit();

    void enable_vblank_handler();
}

#endif
