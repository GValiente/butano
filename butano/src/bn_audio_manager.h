/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AUDIO_MANAGER_H
#define BN_AUDIO_MANAGER_H

#include "bn_fixed_fwd.h"
#include "bn_optional_fwd.h"

namespace bn
{
    class music_item;
    class sound_item;
}

namespace bn::audio_manager
{
    using func_type = void(*)();

    void init(func_type hp_vblank_function, func_type lp_vblank_function);

    void enable();

    void disable();

    [[nodiscard]] bool music_playing();

    [[nodiscard]] optional<music_item> playing_music_item();

    void play_music(music_item item, fixed volume, bool loop);

    void stop_music();

    [[nodiscard]] bool music_paused();

    void pause_music();

    void resume_music();

    [[nodiscard]] int music_position();

    void set_music_position(int position);

    [[nodiscard]] fixed music_volume();

    void set_music_volume(fixed volume);

    void play_sound(int priority, sound_item item);

    void play_sound(int priority, sound_item item, fixed volume, fixed speed, fixed panning);

    void stop_all_sounds();

    [[nodiscard]] bool update_on_vblank();

    void set_update_on_vblank(bool update_on_vblank);

    void disable_vblank_handler();

    void update();

    void commit();

    void stop();
}

#endif
