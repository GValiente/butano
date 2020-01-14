#include "btn_sound.h"

#include "btn_fixed.h"
#include "btn_algorithm.h"
#include "btn_sound_item.h"
#include "../hw/include/btn_hw_audio.h"

namespace btn::sound
{

namespace
{
    int hw_volume(fixed volume)
    {
        return min(fixed_t<8>(volume).value(), 255);
    }

    int hw_speed(fixed speed)
    {
        return min(fixed_t<10>(speed).value(), 65535);
    }

    int hw_panning(fixed panning)
    {
        return min(fixed_t<7>(panning + 1).value(), 255);
    }
}

void play(sound_item item)
{
    hw::audio::play_sound(item);
}

void play(sound_item item, fixed volume)
{
    BTN_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0, 1]: ", volume);

    hw::audio::play_sound(item, hw_volume(volume), hw_speed(1), hw_panning(0));
}

void play(sound_item item, fixed volume, fixed speed, fixed panning)
{
    BTN_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0, 1]: ", volume);
    BTN_ASSERT(speed >= 0 && speed <= 64, "Speed range is [0, 64]: ", speed);
    BTN_ASSERT(panning >= -1 && panning <= 1, "Panning range is [-1, 1]: ", panning);

    hw::audio::play_sound(item, hw_volume(volume), hw_speed(speed), hw_panning(panning));
}

void stop_all()
{
    hw::audio::stop_all_sounds();
}

}
