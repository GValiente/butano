#ifndef BTN_HW_AUDIO_H
#define BTN_HW_AUDIO_H

#include "btn_common.h"

namespace btn
{

class music_item;
class sound_item;

namespace hw::audio
{
    void init();

    void add_irq();

    void remove_irq();

    [[nodiscard]] bool music_playing();

    void play_music(music_item item, int volume, bool loop);

    void stop_music();

    void pause_music();

    void resume_music();

    void set_music_volume(int volume);

    void play_sound(sound_item item);

    void play_sound(sound_item item, int volume, int speed, int panning);

    void stop_all_sounds();

    [[nodiscard]] int direct_sound_control_value();

    void set_direct_sound_control_value(int value);

    void update();

    void commit();

    void stop();
}

}

#endif
