#include "btn_sound_item.h"

#include "btn_fixed.h"
#include "btn_audio_manager.h"

namespace btn
{

void sound_item::play() const
{
    audio_manager::play_sound(0, *this);
}

void sound_item::play(fixed volume) const
{
    audio_manager::play_sound(0, *this, volume, 1, 0);
}

void sound_item::play(fixed volume, fixed speed, fixed panning) const
{
    audio_manager::play_sound(0, *this, volume, speed, panning);
}

void sound_item::play_with_priority(int priority) const
{
    audio_manager::play_sound(priority, *this);
}

void sound_item::play_with_priority(int priority, fixed volume) const
{
    audio_manager::play_sound(priority, *this, volume, 1, 0);
}

void sound_item::play_with_priority(int priority, fixed volume, fixed speed, fixed panning) const
{
    audio_manager::play_sound(priority, *this, volume, speed, panning);
}

}
