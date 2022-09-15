/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AUDIO_MANAGER_H
#define BN_AUDIO_MANAGER_H

#include "bn_fixed.h"
#include "bn_optional.h"

namespace bn
{
    class music_item;
    class sound_item;
    class dmg_music_item;
    class dmg_music_position;
}

namespace bn::audio_manager
{
    void init();

    void enable();

    void disable();

    // music

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

    // dmg_music

    [[nodiscard]] bool dmg_music_playing();

    [[nodiscard]] optional<dmg_music_item> playing_dmg_music_item();

    void play_dmg_music(dmg_music_item item, int speed, bool loop);

    void stop_dmg_music();

    [[nodiscard]] bool dmg_music_paused();

    void pause_dmg_music();

    void resume_dmg_music();

    [[nodiscard]] const dmg_music_position& dmg_music_position();

    void set_dmg_music_position(const bn::dmg_music_position& position);

    [[nodiscard]] fixed dmg_music_left_volume();

    [[nodiscard]] fixed dmg_music_right_volume();

    void set_dmg_music_left_volume(fixed left_volume);

    void set_dmg_music_right_volume(fixed right_volume);

    void set_dmg_music_volume(fixed left_volume, fixed right_volume);

    [[nodiscard]] bool dmg_sync_enabled();

    void set_dmg_sync_enabled(bool enabled);

    // sound

    void play_sound(int priority, sound_item item);

    void play_sound(int priority, sound_item item, fixed volume, fixed speed, fixed panning);

    void stop_all_sounds();

    [[nodiscard]] bool update_on_vblank();

    void set_update_on_vblank(bool update_on_vblank);

    void disable_vblank_handler();

    void update();

    void execute_commands();

    void commit();

    void stop();
}

#endif
