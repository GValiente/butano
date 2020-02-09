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

    void add_irq();

    void remove_irq();

    [[nodiscard]] bool music_playing();

    void play_music(music_item item, fixed volume, bool loop);

    void stop_music();

    void pause_music();

    void resume_music();

    void set_music_volume(fixed volume);

    void play_sound(sound_item item);

    void play_sound(sound_item item, fixed volume, fixed speed, fixed panning);

    void stop_all_sounds();

    void sleep();

    void wake_up();

    void update();

    void commit();

    void stop();
}

#endif
