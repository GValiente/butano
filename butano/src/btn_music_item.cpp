#include "btn_music_item.h"

#include "btn_fixed.h"
#include "btn_audio_manager.h"

namespace btn
{

void music_item::play() const
{
    audio_manager::play_music(*this, 1, true);
}

void music_item::play(fixed volume) const
{
    audio_manager::play_music(*this, volume, true);
}

void music_item::play(fixed volume, bool loop) const
{
    audio_manager::play_music(*this, volume, loop);
}

}
