#include "btn_sound.h"

#include "btn_fixed.h"
#include "btn_sound_item.h"
#include "btn_audio_manager.h"

namespace btn::sound
{

void play(sound_item item)
{
    audio_manager::play_sound(item);
}

void play(sound_item item, fixed volume)
{
    audio_manager::play_sound(item, volume, 1, 0);
}

void play(sound_item item, fixed volume, fixed speed, fixed panning)
{
    audio_manager::play_sound(item, volume, speed, panning);
}

void stop_all()
{
    audio_manager::stop_all_sounds();
}

}
