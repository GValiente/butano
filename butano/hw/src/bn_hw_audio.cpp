/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/btn_hw_audio.h"

#include "maxmod.h"
#include "btn_forward_list.h"
#include "btn_config_audio.h"
#include "../include/btn_hw_irq.h"

extern const uint8_t _btn_audio_soundbank_bin[];

namespace btn::hw::audio
{

namespace
{
    static_assert(BTN_CFG_AUDIO_MAX_MUSIC_CHANNELS > 0, "Invalid max music channels");
    static_assert(BTN_CFG_AUDIO_MAX_SOUND_CHANNELS > 0, "Invalid max sound channels");


    class sound_type
    {

    public:
        mm_sfxhand handle;
        int16_t priority;
    };


    class static_data
    {

    public:
        forward_list<sound_type, BTN_CFG_AUDIO_MAX_SOUND_CHANNELS> sounds_queue;
        uint16_t stat_value = 0;
        uint16_t direct_sound_control_value = 0;
        volatile bool locked = false;
    };

    BTN_DATA_EWRAM static_data data;


    constexpr int _mix_length()
    {
        switch(BTN_CFG_AUDIO_MIXING_RATE)
        {

        case BTN_AUDIO_MIXING_RATE_8_KHZ:
            return MM_MIXLEN_8KHZ;

        case BTN_AUDIO_MIXING_RATE_10_KHZ:
            return MM_MIXLEN_10KHZ;

        case BTN_AUDIO_MIXING_RATE_13_KHZ:
            return MM_MIXLEN_13KHZ;

        case BTN_AUDIO_MIXING_RATE_16_KHZ:
            return MM_MIXLEN_16KHZ;

        case BTN_AUDIO_MIXING_RATE_18_KHZ:
            return MM_MIXLEN_18KHZ;

        case BTN_AUDIO_MIXING_RATE_21_KHZ:
            return MM_MIXLEN_21KHZ;

        case BTN_AUDIO_MIXING_RATE_27_KHZ:
            return MM_MIXLEN_27KHZ;

        case BTN_AUDIO_MIXING_RATE_31_KHZ:
            return MM_MIXLEN_31KHZ;

        default:
            BTN_ERROR("Invalid maxing rate: ", BTN_CFG_AUDIO_MIXING_RATE);
        }
    }

    constexpr const int _max_channels = BTN_CFG_AUDIO_MAX_MUSIC_CHANNELS + BTN_CFG_AUDIO_MAX_SOUND_CHANNELS;

    alignas(int) BTN_DATA_EWRAM uint8_t maxmod_engine_buffer[
            _max_channels * (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH + MM_SIZEOF_MIXCH) + _mix_length()];

    alignas(int) uint8_t maxmod_mixing_buffer[_mix_length()];


    void _check_sounds_queue()
    {
        if(data.sounds_queue.full())
        {
            mmEffectRelease(data.sounds_queue.front().handle);
            data.sounds_queue.pop_front();
        }
    }

    void _add_sound_to_queue(int priority, mm_sfxhand handle)
    {
        auto before_it = data.sounds_queue.before_begin();
        auto it = data.sounds_queue.begin();
        auto end = data.sounds_queue.end();

        while(it != end)
        {
            sound_type& sound = *it;

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
}

void init()
{
    irq::replace_or_push_back(irq::id::VBLANK, mmVBlank);

    mm_gba_system maxmod_info;
    maxmod_info.mixing_mode = mm_mixmode(BTN_CFG_AUDIO_MIXING_RATE);
    maxmod_info.mod_channel_count = _max_channels;
    maxmod_info.mix_channel_count = _max_channels;
    maxmod_info.module_channels = mm_addr(maxmod_engine_buffer);
    maxmod_info.active_channels = mm_addr(maxmod_engine_buffer + (_max_channels * MM_SIZEOF_MODCH));
    maxmod_info.mixing_channels = mm_addr(maxmod_engine_buffer +
            (_max_channels * (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH)));
    maxmod_info.mixing_memory = mm_addr(maxmod_mixing_buffer);
    maxmod_info.wave_memory = mm_addr(maxmod_engine_buffer +
            (_max_channels * (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH + MM_SIZEOF_MIXCH)));
    maxmod_info.soundbank = mm_addr(_btn_audio_soundbank_bin);
    mmInit(&maxmod_info);
}

void enable()
{
    REG_SNDSTAT = data.stat_value;
    REG_SNDDSCNT = data.direct_sound_control_value;
    irq::enable(irq::id::VBLANK);
}

void disable()
{
    irq::disable(irq::id::VBLANK);
    data.stat_value = REG_SNDSTAT;
    data.direct_sound_control_value = REG_SNDDSCNT;
    REG_SNDDSCNT = 0;
    REG_SNDSTAT = 0;
}

bool music_playing()
{
    return mmActive();
}

void play_music(int id, int volume, bool loop)
{
    if(mmActive())
    {
        mmStop();
    }

    mmStart(mm_word(id), loop ? MM_PLAY_LOOP : MM_PLAY_ONCE);
    mmSetModuleVolume(mm_word(volume));
}

void stop_music()
{
    mmStop();
}

void pause_music()
{
    mmPause();
}

void resume_music()
{
    mmResume();
}

int music_position()
{
    return int(mmGetPosition());
}

void set_music_position(int position)
{
    mmSetPosition(mm_word(position));
}

void set_music_volume(int volume)
{
    mmSetModuleVolume(mm_word(volume));
}

void play_sound(int priority, int id)
{
    _check_sounds_queue();
    _add_sound_to_queue(priority, mmEffect(mm_word(id)));
}

void play_sound(int priority, int id, int volume, int speed, int panning)
{
    mm_sound_effect sound_effect;
    sound_effect.id = mm_word(id);
    sound_effect.rate = mm_hword(speed);
    sound_effect.handle = 0;
    sound_effect.volume = mm_byte(volume);
    sound_effect.panning = mm_byte(panning);
    _check_sounds_queue();
    _add_sound_to_queue(priority, mmEffectEx(&sound_effect));
}

void stop_all_sounds()
{
    mmEffectCancelAll();
    data.sounds_queue.clear();
}

void disable_vblank_handler()
{
    mmSetVBlankHandler(nullptr);
}

void commit()
{
    mmFrame();

    auto before_it = data.sounds_queue.before_begin();
    auto it = data.sounds_queue.begin();
    auto end = data.sounds_queue.end();

    while(it != end)
    {
        mm_sfxhand handle = it->handle;

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

void enable_vblank_handler()
{
    mmSetVBlankHandler(reinterpret_cast<void*>(mmFrame));
}

}
