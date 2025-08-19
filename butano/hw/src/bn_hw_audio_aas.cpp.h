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

    constexpr unsigned sound_channel_bits = 4;
    constexpr unsigned sound_counter_bits = 16 - 1 - sound_channel_bits;

    constexpr audio_mixing_rate mixing_rates[] =
    {
        audio_mixing_rate(BN_CFG_AUDIO_MIXING_RATE)
    };


    class sound_type
    {

    public:
        int id;
        int16_t priority;

        uint16_t handle: sound_counter_bits;
        uint8_t channel: sound_channel_bits;
        bool released: 1;
    };


    class static_data
    {

    public:
        forward_list<sound_type, BN_CFG_AUDIO_MAX_SOUND_CHANNELS> sounds_queue;
        fixed sound_master_volume = 1;
        unsigned sound_counter = 0;
        uint16_t direct_sound_control_value = 0;
        bool music_paused = false;
    };

    BN_DATA_EWRAM_BSS static_data data;


    [[nodiscard]] constexpr int _mix_length(audio_mixing_rate mixing_rate)
    {
        switch(mixing_rate)
        {

        case audio_mixing_rate::KHZ_8:
            return AAS_CONFIG_MIX_8KHZ;

        case audio_mixing_rate::KHZ_12:
            return AAS_CONFIG_MIX_12KHZ;

        case audio_mixing_rate::KHZ_16:
            return AAS_CONFIG_MIX_16KHZ;

        case audio_mixing_rate::KHZ_20:
            return AAS_CONFIG_MIX_20KHZ;

        case audio_mixing_rate::KHZ_24:
            return AAS_CONFIG_MIX_24KHZ;

        case audio_mixing_rate::KHZ_28:
            return AAS_CONFIG_MIX_28KHZ;

        case audio_mixing_rate::KHZ_32:
            return AAS_CONFIG_MIX_32KHZ;

        default:
            BN_ERROR("Invalid mixing rate: ", int(mixing_rate));
            return AAS_CONFIG_MIX_8KHZ;
        }
    };

    void _init_impl(int mix_length)
    {
        constexpr int spatial = BN_CFG_AUDIO_STEREO ? AAS_CONFIG_SPATIAL_STEREO : AAS_CONFIG_SPATIAL_MONO;
        constexpr int dynamic = BN_CFG_AUDIO_DYNAMIC_MIXING ? AAS_CONFIG_DYNAMIC_ON : AAS_CONFIG_DYNAMIC_OFF;

        [[maybe_unused]] int error_code = AAS_SetConfig(mix_length, AAS_CONFIG_CHANS_8, spatial, dynamic);
        BN_BASIC_ASSERT(! error_code, "AAS_SetConfig call failed: ", error_code);
    }

    [[nodiscard]] bool _check_sounds_queue(int priority)
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
                AAS_SFX_Stop(sound.channel);
                data.sounds_queue.erase_after(before_it);
                return true;
            }
        }

        const sound_type& first_sound = data.sounds_queue.front();

        if(first_sound.priority <= priority)
        {
            AAS_SFX_Stop(first_sound.channel);
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
                int channel = sound.channel;

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
            int channel = sound.channel;

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
                int channel = sound.channel;
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
            int channel = first_sound.channel;
            AAS_SFX_Stop(channel);
            data.sounds_queue.pop_front();
            return channel;
        }

        return -1;
    }

    [[nodiscard]] int _get_sound_channel(int priority, fixed panning)
    {
        if(! _check_sounds_queue(priority))
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

    [[nodiscard]] uint16_t _add_sound_to_queue(int id, int priority, int channel)
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

        unsigned sound_counter = data.sound_counter + 1;

        if(sound_counter == 1 << sound_counter_bits)
        {
            sound_counter = 0;
        }

        sound_type sound{ id, int16_t(priority), uint16_t(sound_counter), uint8_t(channel), false };
        data.sounds_queue.insert_after(before_it, sound);
        data.sound_counter = sound_counter;
        return sound_counter;
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
    constexpr int mix_length = _mix_length(audio_mixing_rate(BN_CFG_AUDIO_MIXING_RATE));

    ::new(static_cast<void*>(&data)) static_data();

    _init_impl(mix_length);

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

span<const audio_mixing_rate> available_mixing_rates()
{
    return mixing_rates;
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

bool sound_active(uint16_t handle)
{
    for(const sound_type& sound : data.sounds_queue)
    {
        if(sound.handle == handle)
        {
            return AAS_SFX_IsActive(sound.channel);
        }
    }

    return false;
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
            result = _add_sound_to_queue(id, priority, channel);
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
            result = _add_sound_to_queue(id, priority, channel);
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
        const sound_type& sound = *it;

        if(sound.handle != handle)
        {
            before_it = it;
            ++it;
        }
        else
        {
            AAS_SFX_Stop(sound.channel);
            data.sounds_queue.erase_after(before_it);
            return;
        }
    }
}

void release_sound(uint16_t handle)
{
    for(sound_type& sound : data.sounds_queue)
    {
        if(sound.handle == handle)
        {
            sound.released = true;
            return;
        }
    }
}

void set_sound_speed(uint16_t handle, fixed, fixed new_speed)
{
    for(const sound_type& sound : data.sounds_queue)
    {
        if(sound.handle == handle)
        {
            int channel = sound.channel;

            if(AAS_SFX_IsActive(channel))
            {
                int frequency = (new_speed * bn_aas_sound_frequencies[sound.id]).right_shift_integer();
                AAS_SFX_SetFrequency(channel, frequency);
            }

            return;
        }
    }
}

void stop_all_sounds()
{
    for(const sound_type& sound : data.sounds_queue)
    {
        AAS_SFX_Stop(sound.channel);
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
        if(AAS_SFX_IsActive(it->channel))
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
