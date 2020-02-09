#ifndef BTN_HW_AUDIO_H
#define BTN_HW_AUDIO_H

#include "btn_common.h"

namespace btn::hw::audio
{
    void init();

    void add_irq();

    void remove_irq();

    [[nodiscard]] bool music_playing();

    void play_music(int id, int volume, bool loop);

    void stop_music();

    void pause_music();

    void resume_music();

    void set_music_volume(int volume);

    void play_sound(int id);

    void play_sound(int id, int volume, int speed, int panning);

    void stop_all_sounds();

    [[nodiscard]] int direct_sound_control_value();

    void set_direct_sound_control_value(int value);

    void disable_vblank_handler();

    void enable_vblank_handler();
}

#endif
