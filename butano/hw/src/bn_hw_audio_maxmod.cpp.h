/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_audio_maxmod.h"

#include "bn_memory.h"
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
    static_assert(BN_CFG_AUDIO_MAX_EVENTS > 0 && BN_CFG_AUDIO_MAX_EVENTS < 256, "Invalid max events");
    static_assert(BN_CFG_AUDIO_STEREO, "Mono output not supported");
    static_assert(! BN_CFG_AUDIO_DYNAMIC_MIXING, "Dynamic mixing not supported");


    constexpr audio_mixing_rate mixing_rates[] =
    {
        audio_mixing_rate::KHZ_8,
        audio_mixing_rate::KHZ_10,
        audio_mixing_rate::KHZ_13,
        audio_mixing_rate::KHZ_16,
        audio_mixing_rate::KHZ_18,
        audio_mixing_rate::KHZ_21,
        audio_mixing_rate::KHZ_27,
        audio_mixing_rate::KHZ_31,
    };


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
        uint8_t last_event_ids[BN_CFG_AUDIO_MAX_EVENTS];
        uint8_t current_event_ids[BN_CFG_AUDIO_MAX_EVENTS];
        uint16_t direct_sound_control_value = 0;
        uint8_t last_event_count = 0;
        uint8_t current_event_count = 0;
        bool music_paused = false;
        bool event_handler_enabled = false;
    };

    BN_DATA_EWRAM_BSS static_data data;


    [[nodiscard]] constexpr int mix_length(audio_mixing_rate mixing_rate)
    {
        switch(mixing_rate)
        {

        case audio_mixing_rate::KHZ_8:
            return MM_MIXLEN_8KHZ;

        case audio_mixing_rate::KHZ_10:
            return MM_MIXLEN_10KHZ;

        case audio_mixing_rate::KHZ_13:
            return MM_MIXLEN_13KHZ;

        case audio_mixing_rate::KHZ_16:
            return MM_MIXLEN_16KHZ;

        case audio_mixing_rate::KHZ_18:
            return MM_MIXLEN_18KHZ;

        case audio_mixing_rate::KHZ_21:
            return MM_MIXLEN_21KHZ;

        case audio_mixing_rate::KHZ_27:
            return MM_MIXLEN_27KHZ;

        case audio_mixing_rate::KHZ_31:
            return MM_MIXLEN_31KHZ;

        default:
            BN_ERROR("Invalid mixing rate: ", int(mixing_rate));
            return MM_MIXLEN_8KHZ;
        }
    }

    constexpr int _max_channels = BN_CFG_AUDIO_MAX_MUSIC_CHANNELS + BN_CFG_AUDIO_MAX_SOUND_CHANNELS;

    constexpr int _initial_mix_length = mix_length(audio_mixing_rate(BN_CFG_AUDIO_MIXING_RATE));

    alignas(int) BN_DATA_EWRAM_BSS uint8_t maxmod_engine_buffer[
            _max_channels * (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH + MM_SIZEOF_MIXCH) + _initial_mix_length];

    alignas(int) uint8_t maxmod_mixing_buffer[_initial_mix_length];


    void _init_impl(audio_mixing_rate mixing_rate)
    {
        mm_gba_system maxmod_info;
        maxmod_info.mixing_mode = mm_mixmode(mixing_rate);
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

    mm_word _maxmod_event_handler(mm_word msg, mm_word param)
    {
        if(msg == MMCB_SONGMESSAGE)
        {
            int event_count = data.current_event_count;

            if(event_count < BN_CFG_AUDIO_MAX_EVENTS)
            {
                data.current_event_ids[event_count] = uint8_t(param);
                data.current_event_count = uint8_t(event_count + 1);
            }
        }

        return 0;
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

    _init_impl(audio_mixing_rate(BN_CFG_AUDIO_MIXING_RATE));
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

span<const audio_mixing_rate> available_mixing_rates()
{
    int size = 0;

    for(audio_mixing_rate mixing_rate : mixing_rates)
    {
        if(int(mixing_rate) <= BN_CFG_AUDIO_MIXING_RATE)
        {
            ++size;
        }
        else
        {
            break;
        }
    }

    return span<const audio_mixing_rate>(mixing_rates, size);
}

void set_mixing_rate(audio_mixing_rate mixing_rate)
{
    BN_ASSERT(mix_length(mixing_rate) <= _initial_mix_length, "Mixing rate is higher than the initial one");

    mmEnd();
    memory::clear(maxmod_engine_buffer);
    memory::clear(maxmod_mixing_buffer);
    _init_impl(mixing_rate);
}

span<uint8_t> event_ids()
{
    return span<uint8_t>(data.last_event_ids, data.last_event_count);
}

void update_events(bool enabled)
{
    if(data.event_handler_enabled)
    {
        uint8_t event_count = data.current_event_count;
        data.current_event_count = 0;

        if(enabled)
        {
            data.last_event_count = event_count;
            bn::copy(data.current_event_ids, data.current_event_ids + event_count, data.last_event_ids);
        }
        else
        {
            data.last_event_count = 0;
            data.event_handler_enabled = false;
            mmSetEventHandler(nullptr);
        }
    }
    else
    {
        if(enabled)
        {
            data.event_handler_enabled = true;
            mmSetEventHandler(_maxmod_event_handler);
        }
    }
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
