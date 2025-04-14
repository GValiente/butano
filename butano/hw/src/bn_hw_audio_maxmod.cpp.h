/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_audio_maxmod.h"

#include "bn_forward_list.h"
#include "bn_config_audio.h"
#include "../include/bn_hw_tonc.h"

extern const uint8_t bn_audio_soundbank_bin[];

namespace bn::hw::audio
{

namespace
{
    static_assert(BN_CFG_AUDIO_MAX_MUSIC_CHANNELS > 0, "Invalid max music channels");
    static_assert(BN_CFG_AUDIO_MAX_SOUND_CHANNELS > 0, "Invalid max sound channels");
    static_assert(BN_CFG_AUDIO_STEREO, "Mono output not supported");
    static_assert(! BN_CFG_AUDIO_DYNAMIC_MIXING, "Dynamic mixing not supported");


    class sound_type
    {

    public:
        uint16_t handle;
        int16_t priority;
    };


    class static_data
    {

    public:
        forward_list<sound_type, BN_CFG_AUDIO_MAX_SOUND_CHANNELS> sounds_queue;
        uint16_t direct_sound_control_value = 0;
        bool music_paused = false;
    };

    BN_DATA_EWRAM_BSS static_data data;


    constexpr int _mix_length = []()
    {
        switch(BN_CFG_AUDIO_MIXING_RATE)
        {

        case BN_AUDIO_MIXING_RATE_8_KHZ:
            return MM_MIXLEN_8KHZ;

        case BN_AUDIO_MIXING_RATE_10_KHZ:
            return MM_MIXLEN_10KHZ;

        case BN_AUDIO_MIXING_RATE_13_KHZ:
            return MM_MIXLEN_13KHZ;

        case BN_AUDIO_MIXING_RATE_16_KHZ:
            return MM_MIXLEN_16KHZ;

        case BN_AUDIO_MIXING_RATE_18_KHZ:
            return MM_MIXLEN_18KHZ;

        case BN_AUDIO_MIXING_RATE_21_KHZ:
            return MM_MIXLEN_21KHZ;

        case BN_AUDIO_MIXING_RATE_27_KHZ:
            return MM_MIXLEN_27KHZ;

        case BN_AUDIO_MIXING_RATE_31_KHZ:
            return MM_MIXLEN_31KHZ;

        default:
            BN_ERROR("Invalid mixing rate: ", BN_CFG_AUDIO_MIXING_RATE);
        }
    }();

    constexpr int _max_channels = BN_CFG_AUDIO_MAX_MUSIC_CHANNELS + BN_CFG_AUDIO_MAX_SOUND_CHANNELS;

    alignas(int) BN_DATA_EWRAM_BSS uint8_t maxmod_engine_buffer[
            _max_channels * (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH + MM_SIZEOF_MIXCH) + _mix_length];

    alignas(int) uint8_t maxmod_mixing_buffer[_mix_length];


    [[nodiscard]] bool _check_sounds_queue(int priority)
    {
        if(! data.sounds_queue.full())
        {
            return true;
        }

        const sound_type& first_sound = data.sounds_queue.front();

        if(first_sound.priority <= priority)
        {
            mmEffectRelease(first_sound.handle);
            data.sounds_queue.pop_front();
            return true;
        }

        return false;
    }

    void _add_sound_to_queue(int priority, uint16_t handle)
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

        data.sounds_queue.insert_after(before_it, sound_type{ handle, int16_t(priority) });
    }

    void _erase_sound_from_queue(uint16_t handle)
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
                data.sounds_queue.erase_after(before_it);
                return;
            }
        }
    }

    [[nodiscard]] inline int _hw_sound_volume(fixed volume)
    {
        return min(fixed_t<8>(volume).data(), 255);
    }

    [[nodiscard]] inline int _hw_sound_speed(fixed speed)
    {
        return min(fixed_t<10>(speed).data(), 65535);
    }
}

void init()
{
    ::new(static_cast<void*>(&data)) static_data();

    mm_gba_system maxmod_info;
    maxmod_info.mixing_mode = mm_mixmode(BN_CFG_AUDIO_MIXING_RATE);
    maxmod_info.mod_channel_count = _max_channels;
    maxmod_info.mix_channel_count = _max_channels;
    maxmod_info.module_channels = mm_addr(maxmod_engine_buffer);
    maxmod_info.active_channels = mm_addr(maxmod_engine_buffer + (_max_channels * MM_SIZEOF_MODCH));
    maxmod_info.mixing_channels = mm_addr(maxmod_engine_buffer +
            (_max_channels * (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH)));
    maxmod_info.mixing_memory = mm_addr(maxmod_mixing_buffer);
    maxmod_info.wave_memory = mm_addr(maxmod_engine_buffer +
            (_max_channels * (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH + MM_SIZEOF_MIXCH)));
    maxmod_info.soundbank = mm_addr(bn_audio_soundbank_bin);
    mmInit(&maxmod_info);
}

void enable()
{
    REG_SNDDSCNT = data.direct_sound_control_value;
}

void disable()
{
    data.direct_sound_control_value = REG_SNDDSCNT;
    REG_SNDDSCNT = 0;
}

bool music_playing()
{
    return data.music_paused || mmActive();
}

void play_music(int id, bool loop)
{
    mmStart(mm_word(id), loop ? MM_PLAY_LOOP : MM_PLAY_ONCE);
    data.music_paused = false;
}

void stop_music()
{
    mmStop();
    data.music_paused = false;
}

void pause_music()
{
    mmPause();
    data.music_paused = true;
}

void resume_music()
{
    mmResume();
    data.music_paused = false;
}

optional<uint16_t> play_sound(int priority, int id)
{
    optional<uint16_t> result;

    if(_check_sounds_queue(priority))
    {
        uint16_t handle = mmEffect(mm_word(id));
        _add_sound_to_queue(priority, handle);
        result = handle;
    }

    return result;
}

optional<uint16_t> play_sound(int priority, int id, fixed volume, fixed speed, fixed panning)
{
    optional<uint16_t> result;

    if(_check_sounds_queue(priority))
    {
        mm_sound_effect sound_effect;
        sound_effect.id = mm_word(id);
        sound_effect.rate = mm_hword(_hw_sound_speed(speed));
        sound_effect.handle = 0;
        sound_effect.volume = mm_byte(_hw_sound_volume(volume));
        sound_effect.panning = mm_byte(_hw_sound_panning(panning));

        uint16_t handle = mmEffectEx(&sound_effect);
        _add_sound_to_queue(priority, handle);
        result = handle;
    }

    return result;
}

void stop_sound(uint16_t handle)
{
    if(mmEffectActive(handle))
    {
        mmEffectCancel(handle);
        _erase_sound_from_queue(handle);
    }
}

void release_sound(uint16_t handle)
{
    if(mmEffectActive(handle))
    {
        mmEffectRelease(handle);
        _erase_sound_from_queue(handle);
    }
}

void set_sound_speed(uint16_t handle, fixed current_speed, fixed new_speed)
{
    if(mmEffectActive(handle))
    {
        fixed_t<10> hw_current_speed = bn::max(fixed_t<10>(current_speed), fixed_t<10>::from_data(1));
        fixed_t<10> scale = fixed_t<10>(new_speed).unsafe_division(hw_current_speed);

        if(scale != 1)
        {
            int hw_scale = scale.data();
            BN_BASIC_ASSERT(hw_scale < 65536, "Speed change is too high: ", hw_scale);

            mmEffectScaleRate(handle, unsigned(hw_scale));
        }
    }
}

void stop_all_sounds()
{
    mmEffectCancelAll();
    data.sounds_queue.clear();
}

void update_sounds_queue()
{
    auto before_it = data.sounds_queue.before_begin();
    auto it = data.sounds_queue.begin();
    auto end = data.sounds_queue.end();

    while(it != end)
    {
        uint16_t handle = it->handle;

        if(mmEffectActive(handle))
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
