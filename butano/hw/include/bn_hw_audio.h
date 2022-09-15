/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_AUDIO_H
#define BN_HW_AUDIO_H

#include "bn_common.h"

extern "C"
{
    #include "../3rd_party/gbt-player/include/gbt_player.h"
}

namespace bn::hw::audio
{
    void init();

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

    inline void play_dmg_music(const void* song, int speed, bool loop)
    {
        gbt_play(song, speed);
        gbt_loop(loop);
    }

    inline void stop_dmg_music()
    {
        gbt_stop();
    }

    inline void pause_dmg_music()
    {
        gbt_pause(0);
    }

    inline void resume_dmg_music()
    {
        gbt_pause(1);
    }

    inline void dmg_music_position(int& pattern, int& row)
    {
        gbt_get_position(&pattern, &row, nullptr);
    }

    inline void set_dmg_music_position(int pattern, int row)
    {
        gbt_set_position(pattern, row);
    }

    inline void set_dmg_music_volume(int left_volume, int right_volume)
    {
        gbt_volume(unsigned(left_volume), unsigned(right_volume));
    }

    void play_sound(int priority, int id);

    void play_sound(int priority, int id, int volume, int speed, int panning);

    void stop_all_sounds();

    [[nodiscard]] bool update_on_vblank();

    void set_update_on_vblank(bool update_on_vblank);

    void disable_vblank_handler();

    void update(bool dmg_sync);

    void update_sounds_queue();

    void commit();
}

#endif
