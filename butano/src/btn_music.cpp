#include "btn_music.h"

#include "btn_fixed.h"
#include "btn_music_item.h"
#include "btn_audio_manager.h"

namespace btn::music
{

bool playing()
{
    return audio_manager::music_playing();
}

void play(music_item item)
{
    audio_manager::play_music(item, 1, true);
}

void play(music_item item, fixed volume)
{
    audio_manager::play_music(item, volume, true);
}

void play(music_item item, fixed volume, bool loop)
{
    audio_manager::play_music(item, volume, loop);
}

void stop()
{
    audio_manager::stop_music();
}

bool paused()
{
    return audio_manager::music_paused();
}

void pause()
{
    audio_manager::pause_music();
}

void resume()
{
    audio_manager::resume_music();
}

int position()
{
    return audio_manager::music_position();
}

void set_position(int position)
{
    audio_manager::set_music_position(position);
}

fixed volume()
{
    return audio_manager::music_volume();
}

void set_volume(fixed volume)
{
    audio_manager::set_music_volume(volume);
}

}
