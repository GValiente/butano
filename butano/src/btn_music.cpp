#include "btn_music.h"

#include "btn_fixed.h"
#include "btn_assert.h"
#include "btn_music_item.h"
#include "../hw/include/btn_hw_audio.h"

namespace btn::music
{

namespace
{
    int hw_volume(fixed volume)
    {
        return volume.unsafe_multiplication(1024).integer();
    }
}

bool playing()
{
    return hw::audio::music_playing();
}

void play(music_item item)
{
    hw::audio::play_music(item, true, hw_volume(1));
}

void play(music_item item, bool loop, fixed volume)
{
    BTN_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0, 1]: ", volume);

    hw::audio::play_music(item, loop, hw_volume(volume));
}

void stop()
{
    hw::audio::stop_music();
}

void pause()
{
    hw::audio::pause_music();
}

void resume()
{
    hw::audio::resume_music();
}

void set_volume(fixed volume)
{
    BTN_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0, 1]: ", volume);

    hw::audio::set_music_volume(hw_volume(volume));
}

}
