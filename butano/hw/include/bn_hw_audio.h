/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_AUDIO_H
#define BN_HW_AUDIO_H

#include "maxmod.h"
#include "bn_dmg_music_type.h"

namespace bn::hw::audio
{
    void init();

    void enable();

    void disable();

    [[nodiscard]] inline bool music_playing()
    {
        return mmActive();
    }

    inline void play_music(int id, bool loop)
    {
        if(mmActive())
        {
            mmStop();
        }

        mmStart(mm_word(id), loop ? MM_PLAY_LOOP : MM_PLAY_ONCE);
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

    inline void set_music_tempo(int tempo)
    {
        mmSetModuleTempo(mm_word(tempo));
    }

    inline void set_music_pitch(int pitch)
    {
        mmSetModulePitch(mm_word(pitch));
    }

    void play_dmg_music(const void* song, dmg_music_type type, int speed, bool loop);

    void stop_dmg_music();

    void pause_dmg_music();

    void resume_dmg_music();

    void dmg_music_position(int& pattern, int& row);

    void set_dmg_music_position(int pattern, int row);

    void set_dmg_music_volume(int left_volume, int right_volume);

    [[nodiscard]] inline bool sound_active(mm_sfxhand handle)
    {
        return mmEffectActive(handle);
    }

    [[nodiscard]] mm_sfxhand play_sound(int priority, int id);

    [[nodiscard]] mm_sfxhand play_sound(int priority, int id, int volume, int speed, int panning);

    void stop_sound(mm_sfxhand handle);

    void release_sound(mm_sfxhand handle);

    void set_sound_speed(mm_sfxhand handle, int speed_scale);

    void set_sound_panning(mm_sfxhand handle, int panning);

    void stop_all_sounds();

    inline void set_sound_master_volume(int volume)
    {
        mmSetEffectsVolume(mm_word(volume));
    }

    [[nodiscard]] bool update_on_vblank();

    void set_update_on_vblank(bool update_on_vblank);

    void disable_vblank_handler();

    void update(bool dmg_sync);

    void update_sounds_queue();

    void commit();
}

#endif
