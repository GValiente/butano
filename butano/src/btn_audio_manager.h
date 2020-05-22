#ifndef BTN_AUDIO_MANAGER_H
#define BTN_AUDIO_MANAGER_H

#include "btn_fixed_fwd.h"

namespace btn
{
    class music_item;
    class sound_item;
}

namespace btn::audio_manager
{
    void init();

    void enable();

    void disable();

    [[nodiscard]] bool music_playing();

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

    void disable_vblank_handler();

    void commit();

    void enable_vblank_handler();

    void stop();
}

#endif
