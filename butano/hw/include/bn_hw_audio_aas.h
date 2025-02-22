/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_AUDIO_AAS_H
#define BN_HW_AUDIO_AAS_H

#include "AAS.h"
#include "bn_fixed.h"
#include "bn_optional.h"

namespace bn::hw::audio
{
    [[nodiscard]] constexpr int first_free_timer_id()
    {
        return 0;
    }

    [[nodiscard]] constexpr bool dma_channel_3_free()
    {
        return false;
    }

    void init();

    void enable();

    void disable();

    [[nodiscard]] bool music_playing();

    void play_music(int id, bool loop);

    void stop_music();

    void pause_music();

    void resume_music();

    [[nodiscard]] inline int music_position()
    {
        return AAS_MOD_GetSongPos();
    }

    void set_music_position(int position);

    void set_music_volume(fixed volume);

    inline void set_music_tempo(fixed tempo)
    {
        BN_BASIC_ASSERT(tempo == 1, "Music tempo not supported");
    }

    inline void set_music_pitch(fixed pitch)
    {
        BN_BASIC_ASSERT(pitch == 1, "Music pitch not supported");
    }

    [[nodiscard]] inline bool jingle_playing()
    {
        return false;
    }

    inline void play_jingle(int)
    {
        BN_ERROR("Jingles not supported");
    }

    inline void set_jingle_volume(fixed)
    {
    }

    [[nodiscard]] inline bool sound_active(uint16_t handle)
    {
        return AAS_SFX_IsActive(handle);
    }

    [[nodiscard]] optional<uint16_t> play_sound(int priority, int id);

    [[nodiscard]] optional<uint16_t> play_sound(int priority, int id, fixed volume, fixed speed, fixed panning);

    void stop_sound(uint16_t handle);

    void release_sound(uint16_t handle);

    void set_sound_speed(uint16_t handle, fixed current_speed, fixed new_speed);

    inline void set_sound_panning(uint16_t, fixed)
    {
        BN_ERROR("Sound panning change not supported");
    }

    void stop_all_sounds();

    void set_sound_master_volume(fixed volume);

    void update_sounds_queue();

    inline void on_vblank()
    {
    }

    inline void commit()
    {
        AAS_DoWork();
    }

    inline void stop()
    {
        stop_music();
        stop_all_sounds();
    }
}

#endif
