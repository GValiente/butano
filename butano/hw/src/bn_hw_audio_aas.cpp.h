/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_audio_aas.h"

#include "bn_forward_list.h"
#include "bn_config_audio.h"
#include "../include/bn_hw_irq.h"
#include "../include/bn_hw_tonc.h"

extern "C"
{
    extern const uint8_t* bn_aas_music_ids[];

    extern const int8_t* const* bn_aas_sound_starts[];

    extern const int8_t* const* bn_aas_sound_ends[];

    extern const int bn_aas_sound_frequencies[];
}

namespace bn::hw::audio
{

namespace
{
    static_assert(BN_CFG_AUDIO_MAX_MUSIC_CHANNELS > 0, "Invalid max music channels");
    static_assert(BN_CFG_AUDIO_MAX_SOUND_CHANNELS > 0, "Invalid max sound channels");


    class sound_type
    {

    public:
        int id;
        int16_t priority;
        uint8_t handle;
        bool released;
    };


    class static_data
    {

    public:
        forward_list<sound_type, BN_CFG_AUDIO_MAX_SOUND_CHANNELS> sounds_queue;
        fixed sound_master_volume = 1;
        uint16_t direct_sound_control_value = 0;
        bool music_paused = false;
    };

    BN_DATA_EWRAM_BSS static_data data;


    constexpr int _mix_length = []()
    {
        switch(BN_CFG_AUDIO_MIXING_RATE)
        {

        case BN_AUDIO_MIXING_RATE_8_KHZ:
            return AAS_CONFIG_MIX_8KHZ;

        case BN_AUDIO_MIXING_RATE_12_KHZ:
            return AAS_CONFIG_MIX_12KHZ;

        case BN_AUDIO_MIXING_RATE_16_KHZ:
            return AAS_CONFIG_MIX_16KHZ;

        case BN_AUDIO_MIXING_RATE_20_KHZ:
            return AAS_CONFIG_MIX_20KHZ;

        case BN_AUDIO_MIXING_RATE_24_KHZ:
            return AAS_CONFIG_MIX_24KHZ;

        case BN_AUDIO_MIXING_RATE_28_KHZ:
            return AAS_CONFIG_MIX_28KHZ;

        case BN_AUDIO_MIXING_RATE_32_KHZ:
            return AAS_CONFIG_MIX_32KHZ;

        default:
            BN_ERROR("Invalid maxing rate: ", BN_CFG_AUDIO_MIXING_RATE);
        }
    }();

    [[nodiscard]] bool _check_sound_queue(int priority)
    {
        if(! data.sounds_queue.full())
        {
            return true;
        }

        auto before_it = data.sounds_queue.before_begin();
        auto it = ++before_it;
        auto end = data.sounds_queue.end();

        while(it != end)
        {
            const sound_type& sound = *it;

            if(! sound.released)
            {
                before_it = it;
                ++it;
            }
            else
            {
                int handle = sound.handle;
                AAS_SFX_Stop(handle);
                data.sounds_queue.erase_after(before_it);
                return true;
            }
        }

        const sound_type& first_sound = data.sounds_queue.front();

        if(first_sound.priority <= priority)
        {
            int handle = first_sound.handle;
            AAS_SFX_Stop(handle);
            data.sounds_queue.pop_front();
            return true;
        }

        return false;
    }

    [[nodiscard]] bool _is_left_channel(int channel)
    {
        if(! channel)
        {
            return true;
        }

        --channel;
        return (channel / 2) % 2;
    }

    [[nodiscard]] int _get_panned_sound_channel(bool left_panning)
    {
        for(int channel = 0, limit = AAS_SFX_GetNumChannels(); channel < limit; ++channel)
        {
            if(_is_left_channel(channel) == left_panning)
            {
                if(AAS_SFX_ChannelExists(channel) && ! AAS_SFX_IsActive(channel))
                {
                    return channel;
                }
            }
        }

        return -1;
    }

    [[nodiscard]] int _get_sound_channel()
    {
        for(int channel = 0, limit = AAS_SFX_GetNumChannels(); channel < limit; ++channel)
        {
            if(AAS_SFX_ChannelExists(channel) && ! AAS_SFX_IsActive(channel))
            {
                return channel;
            }
        }

        return -1;
    }

    [[nodiscard]] int _free_panned_sound_channel(int priority, bool left_panning)
    {
        if(data.sounds_queue.empty())
        {
            return -1;
        }

        auto before_it = data.sounds_queue.before_begin();
        auto it = ++before_it;
        auto end = data.sounds_queue.end();

        while(it != end)
        {
            const sound_type& sound = *it;

            if(sound.released)
            {
                int channel = sound.handle;

                if(_is_left_channel(channel) == left_panning)
                {
                    AAS_SFX_Stop(channel);
                    data.sounds_queue.erase_after(before_it);
                    return channel;
                }
            }

            before_it = it;
            ++it;
        }

        before_it = data.sounds_queue.before_begin();
        it = ++before_it;

        while(it != end)
        {
            const sound_type& sound = *it;
            int channel = sound.handle;

            if(_is_left_channel(channel) == left_panning)
            {
                if(sound.priority <= priority)
                {
                    AAS_SFX_Stop(channel);
                    data.sounds_queue.erase_after(before_it);
                    return channel;
                }

                return -1;
            }

            before_it = it;
            ++it;
        }

        return -1;
    }

    [[nodiscard]] int _free_sound_channel(int priority)
    {
        if(data.sounds_queue.empty())
        {
            return -1;
        }

        auto before_it = data.sounds_queue.before_begin();
        auto it = ++before_it;
        auto end = data.sounds_queue.end();

        while(it != end)
        {
            const sound_type& sound = *it;

            if(sound.released)
            {
                int channel = sound.handle;
                AAS_SFX_Stop(channel);
                data.sounds_queue.erase_after(before_it);
                return channel;
            }

            before_it = it;
            ++it;
        }

        const sound_type& first_sound = data.sounds_queue.front();

        if(first_sound.priority <= priority)
        {
            int channel = first_sound.handle;
            AAS_SFX_Stop(channel);
            data.sounds_queue.pop_front();
            return channel;
        }

        return -1;
    }

    [[nodiscard]] int _get_sound_channel(int priority, fixed panning)
    {
        if(! _check_sound_queue(priority))
        {
            return -1;
        }

        if(BN_CFG_AUDIO_STEREO && panning != 0)
        {
            bool left_panning = panning < 0;
            int result = _get_panned_sound_channel(left_panning);

            if(result >= 0)
            {
                return result;
            }

            result = _free_panned_sound_channel(priority, left_panning);

            if(result >= 0)
            {
                return result;
            }
        }

        int result = _get_sound_channel();

        if(result >= 0)
        {
            return result;
        }

        return _free_sound_channel(priority);
    }

    void _add_sound_to_queue(int id, int priority, uint16_t handle)
    {
        auto before_it = data.sounds_queue.before_begin();
        auto it = data.sounds_queue.begin();
        auto end = data.sounds_queue.end();

        while(it != end)
        {
            const sound_type& sound = *it;

            if(sound.priority <= priority)
            {
                before_it = it;
                ++it;
            }
            else
            {
                break;
            }
        }

        data.sounds_queue.insert_after(before_it, sound_type{ id, int16_t(priority), uint8_t(handle), false });
    }

    [[nodiscard]] inline int _hw_music_volume(fixed volume)
    {
        return fixed_t<8>(volume).data();
    }

    [[nodiscard]] inline int _hw_sound_volume(fixed volume)
    {
        volume = volume.unsafe_multiplication(data.sound_master_volume);
        return fixed_t<6>(volume).data();
    }
}

void init()
{
    ::new(static_cast<void*>(&data)) static_data();

    constexpr int spatial = BN_CFG_AUDIO_STEREO ? AAS_CONFIG_SPATIAL_STEREO : AAS_CONFIG_SPATIAL_MONO;
    constexpr int dynamic = BN_CFG_AUDIO_DYNAMIC_MIXING ? AAS_CONFIG_DYNAMIC_ON : AAS_CONFIG_DYNAMIC_OFF;

    [[maybe_unused]] int error_code = AAS_SetConfig(_mix_length, AAS_CONFIG_CHANS_8, spatial, dynamic);
    BN_BASIC_ASSERT(! error_code, "AAS_SetConfig call failed: ", error_code);

    irq::set_isr(irq::id::TIMER1, AAS_FastTimer1InterruptHandler);
    hw::irq::enable(hw::irq::id::TIMER1);
}

void enable()
{
    REG_SNDDSCNT = data.direct_sound_control_value;

    hw::irq::enable(hw::irq::id::TIMER1);
}

void disable()
{
    hw::irq::disable(hw::irq::id::TIMER1);

    data.direct_sound_control_value = REG_SNDDSCNT;
    REG_SNDDSCNT = 0;
}

bool music_playing()
{
    return data.music_paused || AAS_MOD_IsPlaying();
}

void play_music(int id, bool loop)
{
    [[maybe_unused]] int error_code = AAS_MOD_Play(*bn_aas_music_ids[id]);
    BN_BASIC_ASSERT(! error_code, "AAS_MOD_Play call failed: ", error_code);

    error_code = AAS_MOD_SetLoop(loop);
    BN_BASIC_ASSERT(! error_code, "AAS_MOD_SetLoop call failed: ", error_code);

    data.music_paused = false;
}

void stop_music()
{
    AAS_MOD_Stop();
    data.music_paused = false;
}

void pause_music()
{
    AAS_MOD_Pause();
    data.music_paused = true;
}

void resume_music()
{
    AAS_MOD_Resume();
    data.music_paused = false;
}

void set_music_position(int position)
{
    [[maybe_unused]] int error_code = AAS_MOD_SetSongPos(position);
    BN_BASIC_ASSERT(! error_code, "AAS_MOD_SetSongPos call failed: ", error_code);
}

void set_music_volume(fixed volume)
{
    [[maybe_unused]] int error_code = AAS_MOD_SetVolume(_hw_music_volume(volume));
    BN_BASIC_ASSERT(! error_code, "AAS_MOD_SetVolume call failed: ", error_code);
}

optional<uint16_t> play_sound(int priority, int id)
{
    int channel = _get_sound_channel(priority, 0);
    optional<uint16_t> result;

    if(channel >= 0)
    {
        if(! AAS_SFX_Play(channel, _hw_sound_volume(1), bn_aas_sound_frequencies[id], *bn_aas_sound_starts[id],
                          *bn_aas_sound_ends[id], nullptr))
        {
            uint16_t handle = channel;
            _add_sound_to_queue(id, priority, handle);
            result = handle;
        }
    }

    return result;
}

optional<uint16_t> play_sound(int priority, int id, fixed volume, fixed speed, fixed panning)
{
    int channel = _get_sound_channel(priority, panning);
    optional<uint16_t> result;

    if(channel >= 0)
    {
        int frequency = (speed * bn_aas_sound_frequencies[id]).right_shift_integer();

        if(! AAS_SFX_Play(channel, _hw_sound_volume(volume), frequency, *bn_aas_sound_starts[id],
                          *bn_aas_sound_ends[id], nullptr))
        {
            uint16_t handle = channel;
            _add_sound_to_queue(id, priority, handle);
            result = handle;
        }
    }

    return result;
}

void stop_sound(uint16_t handle)
{
    auto before_it = data.sounds_queue.before_begin();
    auto it = data.sounds_queue.begin();
    auto end = data.sounds_queue.end();

    while(it != end)
    {
        if(it->handle != handle)
        {
            before_it = it;
            ++it;
        }
        else
        {
            AAS_SFX_Stop(handle);
            data.sounds_queue.erase_after(before_it);
            return;
        }
    }
}

void release_sound(uint16_t handle)
{
    auto it = data.sounds_queue.begin();
    auto end = data.sounds_queue.end();

    while(it != end)
    {
        sound_type& sound = *it;

        if(sound.handle != handle)
        {
            ++it;
        }
        else
        {
            sound.released = true;
            return;
        }
    }
}

void set_sound_speed(uint16_t handle, fixed, fixed new_speed)
{
    auto it = data.sounds_queue.begin();
    auto end = data.sounds_queue.end();

    while(it != end)
    {
        const sound_type& sound = *it;

        if(sound.handle != handle)
        {
            ++it;
        }
        else
        {
            if(AAS_SFX_IsActive(handle))
            {
                int frequency = (new_speed * bn_aas_sound_frequencies[sound.id]).right_shift_integer();
                AAS_SFX_SetFrequency(handle, frequency);
            }

            return;
        }
    }
}

void stop_all_sounds()
{
    auto it = data.sounds_queue.begin();
    auto end = data.sounds_queue.end();

    while(it != end)
    {
        AAS_SFX_Stop(it->handle);
        ++it;
    }

    data.sounds_queue.clear();
}

void set_sound_master_volume(fixed volume)
{
    data.sound_master_volume = volume;
}

void update_sounds_queue()
{
    auto before_it = data.sounds_queue.before_begin();
    auto it = data.sounds_queue.begin();
    auto end = data.sounds_queue.end();

    while(it != end)
    {
        uint16_t handle = it->handle;

        if(AAS_SFX_IsActive(handle))
        {
            before_it = it;
            ++it;
        }
        else
        {
            it = data.sounds_queue.erase_after(before_it);
        }
    }
}

}
