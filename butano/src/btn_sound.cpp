#include "btn_sound.h"

#include "btn_fixed.h"
#include "btn_algorithm.h"
#include "btn_sound_item.h"
#include "../hw/include/btn_hw_audio.h"

namespace btn::sound
{

void play(sound_item item)
{
    hw::audio::play_sound(item);
}

void play(sound_item item, fixed volume)
{
    BTN_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0, 1]: ", volume);

    int hw_volume = volume.unsafe_multiplication(255).integer();
    hw::audio::play_sound(item, hw_volume, 1024, 128);
}

void play(sound_item item, fixed volume, fixed speed, fixed panning)
{
    BTN_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0, 1]: ", volume);
    BTN_ASSERT(speed >= 0 && speed <= 63, "Speed range is [0, 63]: ", speed);
    BTN_ASSERT(panning >= -1 && panning <= 1, "Panning range is [-1, 1]: ", panning);

    int hw_volume = volume.unsafe_multiplication(255).integer();
    int hw_speed = speed.unsafe_multiplication(1024).integer();
    int hw_panning = min((panning + 1).unsafe_multiplication(128).integer(), 255);
    hw::audio::play_sound(item, hw_volume, hw_speed, hw_panning);
}

void stop_all()
{
    hw::audio::stop_all_sounds();
}

}
