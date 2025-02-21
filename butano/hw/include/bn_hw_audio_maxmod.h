/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_AUDIO_MAXMOD_H
#define BN_HW_AUDIO_MAXMOD_H

#include "maxmod.h"
#include "bn_fixed.h"
#include "bn_optional.h"
#include "bn_hw_tonc.h"

namespace bn::hw::audio
{
    [[nodiscard]] inline int _hw_music_volume(fixed volume)
    {
        return fixed_t<10>(volume).data();
    }

    [[nodiscard]] inline int _hw_music_tempo(fixed tempo)
    {
        return fixed_t<10>(tempo).data();
    }

    [[nodiscard]] inline int _hw_music_pitch(fixed pitch)
    {
        return fixed_t<10>(pitch).data();
    }

    [[nodiscard]] inline int _hw_sound_panning(fixed panning)
    {
        return min(fixed_t<7>(panning + 1).data(), 255);
    }

    [[nodiscard]] inline int _hw_sound_master_volume(fixed volume)
    {
        return fixed_t<10>(volume).data();
    }

    [[nodiscard]] constexpr int first_free_timer_id()
    {
        return 1;
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
        return int(mmGetPosition());
    }

    inline void set_music_position(int position)
    {
        mmSetPosition(mm_word(position));
    }

    inline void set_music_volume(fixed volume)
    {
        mmSetModuleVolume(mm_word(_hw_music_volume(volume)));
    }

    inline void set_music_tempo(fixed tempo)
    {
        mmSetModuleTempo(mm_word(_hw_music_tempo(tempo)));
    }

    inline void set_music_pitch(fixed pitch)
    {
        mmSetModulePitch(mm_word(_hw_music_pitch(pitch)));
    }

    [[nodiscard]] inline bool jingle_playing()
    {
        return mmActiveSub();
    }

    inline void play_jingle(int id)
    {
        mmJingle(mm_word(id));
    }

    inline void set_jingle_volume(fixed volume)
    {
        mmSetJingleVolume(mm_word(_hw_music_volume(volume)));
    }

    [[nodiscard]] inline bool sound_active(uint16_t handle)
    {
        return mmEffectActive(handle);
    }

    [[nodiscard]] optional<uint16_t> play_sound(int priority, int id);

    [[nodiscard]] optional<uint16_t> play_sound(int priority, int id, fixed volume, fixed speed, fixed panning);

    void stop_sound(uint16_t handle);

    void release_sound(uint16_t handle);

    void set_sound_speed(uint16_t handle, fixed current_speed, fixed new_speed);

    inline void set_sound_panning(uint16_t handle, fixed panning)
    {
        if(mmEffectActive(handle))
        {
            mmEffectPanning(handle, uint8_t(_hw_sound_panning(panning)));
        }
    }

    void stop_all_sounds();

    inline void set_sound_master_volume(fixed volume)
    {
        mmSetEffectsVolume(mm_word(_hw_sound_master_volume(volume)));
    }

    void update_sounds_queue();

    inline void on_vblank()
    {
        mmVBlank();
    }

    inline void commit()
    {
        mmFrame();
    }

    inline void stop()
    {
        stop_music();

        if(jingle_playing())
        {
            set_jingle_volume(0);
        }

        stop_all_sounds();
    }

    inline void start_dma(int channel, unsigned flags, const void* source, void* destination)
    {
        REG_DMA[channel].cnt = 0;
        REG_DMA[channel].src = source;
        REG_DMA[channel].dst = destination;
        REG_DMA[channel].cnt = flags;
    }

    inline void stop_dma(int channel)
    {
        REG_DMA[channel].cnt = 0;
    }
}

#endif
