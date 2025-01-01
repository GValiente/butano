/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sound.h"

#include "bn_assert.h"
#include "bn_sound_item.h"
#include "bn_audio_manager.h"

namespace bn
{
    class sound_handle_generator
    {

    public:
        [[nodiscard]] sound_handle generate(uint16_t handle_id) const
        {
            return sound_handle(handle_id);
        }
    };
}

namespace bn::sound
{

sound_handle play(sound_item item)
{
    uint16_t handle_id = audio_manager::play_sound(0, item);
    return sound_handle_generator().generate(handle_id);
}

sound_handle play(sound_item item, fixed volume)
{
    BN_BASIC_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);

    uint16_t handle_id = audio_manager::play_sound(0, item, volume, 1, 0);
    return sound_handle_generator().generate(handle_id);
}

sound_handle play(sound_item item, fixed volume, fixed speed, fixed panning)
{
    BN_BASIC_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);
    BN_BASIC_ASSERT(speed >= 0 && speed <= 64, "Speed range is [0..64]: ", speed);
    BN_BASIC_ASSERT(panning >= -1 && panning <= 1, "Panning range is [-1..1]: ", panning);

    uint16_t handle_id = audio_manager::play_sound(0, item, volume, speed, panning);
    return sound_handle_generator().generate(handle_id);
}

sound_handle play_with_priority(int priority, sound_item item)
{
    BN_BASIC_ASSERT(priority >= -32767 && priority <= 32767, "Priority range is [-32767..32767]: ", priority);

    uint16_t handle_id = audio_manager::play_sound(priority, item);
    return sound_handle_generator().generate(handle_id);
}

sound_handle play_with_priority(int priority, sound_item item, fixed volume)
{
    BN_BASIC_ASSERT(priority >= -32767 && priority <= 32767, "Priority range is [-32767..32767]: ", priority);
    BN_BASIC_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);

    uint16_t handle_id = audio_manager::play_sound(priority, item, volume, 1, 0);
    return sound_handle_generator().generate(handle_id);
}

sound_handle play_with_priority(int priority, sound_item item, fixed volume, fixed speed, fixed panning)
{
    BN_BASIC_ASSERT(priority >= -32767 && priority <= 32767, "Priority range is [-32767..32767]: ", priority);
    BN_BASIC_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);
    BN_BASIC_ASSERT(speed >= 0 && speed <= 64, "Speed range is [0..64]: ", speed);
    BN_BASIC_ASSERT(panning >= -1 && panning <= 1, "Panning range is [-1..1]: ", panning);

    uint16_t handle_id = audio_manager::play_sound(priority, item, volume, speed, panning);
    return sound_handle_generator().generate(handle_id);
}

void stop_all()
{
    audio_manager::stop_all_sounds();
}

fixed master_volume()
{
    return audio_manager::sound_master_volume();
}

void set_master_volume(fixed volume)
{
    BN_BASIC_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0..1]: ", volume);

    audio_manager::set_sound_master_volume(volume);
}

}
