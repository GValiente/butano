/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_AUDIO_NULL_H
#define BN_HW_AUDIO_NULL_H

#include "bn_span.h"
#include "bn_fixed.h"
#include "bn_optional.h"

namespace bn
{
    enum class audio_mixing_rate : uint8_t;
}

namespace bn::hw::audio
{
    [[nodiscard]] constexpr bool timer_free(int)
    {
        return true;
    }

    [[nodiscard]] constexpr bool dma_channel_free(int)
    {
        return true;
    }

    inline void init()
    {
    }

    inline void enable()
    {
    }

    inline void disable()
    {
    }

    [[nodiscard]] inline span<const audio_mixing_rate> available_mixing_rates()
    {
        return span<const audio_mixing_rate>();
    }

    inline void set_mixing_rate(audio_mixing_rate)
    {
    }

    [[nodiscard]] inline span<uint8_t> event_ids()
    {
        return span<uint8_t>();
    }

    inline void update_events(bool)
    {
    }

    [[nodiscard]] inline bool music_playing()
    {
        return false;
    }

    inline void play_music(int, bool)
    {
    }

    inline void stop_music()
    {
    }

    inline void pause_music()
    {
    }

    inline void resume_music()
    {
    }

    [[nodiscard]] inline int music_position()
    {
        return 0;
    }

    inline void set_music_position(int)
    {
    }

    inline void set_music_volume(fixed)
    {
    }

    inline void set_music_tempo(fixed)
    {
    }

    inline void set_music_pitch(fixed)
    {
    }

    [[nodiscard]] inline bool jingle_playing()
    {
        return false;
    }

    inline void play_jingle(int)
    {
    }

    inline void set_jingle_volume(fixed)
    {
    }

    [[nodiscard]] inline bool sound_active(uint16_t)
    {
        return false;
    }

    [[nodiscard]] inline optional<uint16_t> play_sound(int, int)
    {
        return nullopt;
    }

    [[nodiscard]] inline optional<uint16_t> play_sound(int, int, fixed, fixed, fixed)
    {
        return nullopt;
    }

    inline void stop_sound(uint16_t)
    {
    }

    inline void release_sound(uint16_t)
    {
    }

    inline void set_sound_speed(uint16_t, fixed, fixed)
    {
    }

    inline void set_sound_panning(uint16_t, fixed)
    {
    }

    inline void stop_all_sounds()
    {
    }

    inline void set_sound_master_volume(fixed)
    {
    }

    inline void update_sounds_queue()
    {
    }

    inline void on_vblank()
    {
    }

    inline void commit()
    {
    }

    inline void stop()
    {
    }
}

#endif
