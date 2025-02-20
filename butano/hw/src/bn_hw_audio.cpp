/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_audio.h"

#include "bn_forward_list.h"
#include "bn_config_audio.h"
#include "../include/bn_hw_irq.h"
#include "../include/bn_hw_link.h"
#include "../include/bn_hw_dmg_audio.h"

extern const uint8_t _bn_audio_soundbank_bin[];

namespace bn::core
{
    void on_vblank();
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
        mm_sfxhand handle;
        int16_t priority;
    };


    class static_data
    {

    public:
        forward_list<sound_type, BN_CFG_AUDIO_MAX_SOUND_CHANNELS> sounds_queue;
        uint16_t direct_sound_control_value = 0;
        bool music_paused = false;
        bool update_on_vblank = false;
        bool delay_commit = true;
    };

    BN_DATA_EWRAM_BSS static_data data;


    constexpr int _mix_length()
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
            BN_ERROR("Invalid maxing rate: ", BN_CFG_AUDIO_MIXING_RATE);
        }
    }

    constexpr int _max_channels = BN_CFG_AUDIO_MAX_MUSIC_CHANNELS + BN_CFG_AUDIO_MAX_SOUND_CHANNELS;

    alignas(int) BN_DATA_EWRAM_BSS uint8_t maxmod_engine_buffer[
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

    void _erase_sound_from_queue(mm_sfxhand handle)
    {
        auto before_it = data.sounds_queue.before_begin();
        auto it = data.sounds_queue.begin();
        auto end = data.sounds_queue.end();

        while(it != end)
        {
            if(it->handle == handle)
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

    void _commit()
    {
        mmFrame();
        dmg_audio::commit();
    }

    void _vblank_handler()
    {
        core::on_vblank();

        if(! data.delay_commit)
        {
            _commit();
        }

        hw::link::commit();
    }
}

void init()
{
    ::new(static_cast<void*>(&data)) static_data();

    irq::set_isr(irq::id::VBLANK, mmVBlank);
    irq::enable(irq::id::VBLANK);

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
    maxmod_info.soundbank = mm_addr(_bn_audio_soundbank_bin);
    mmInit(&maxmod_info);

    mmSetVBlankHandler(reinterpret_cast<void*>(_vblank_handler));
}

void enable()
{
    REG_SNDDSCNT = data.direct_sound_control_value;

    irq::enable(irq::id::VBLANK);
}

void disable()
{
    irq::disable(irq::id::VBLANK);

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

mm_sfxhand play_sound(int priority, int id)
{
    _check_sounds_queue();

    mm_sfxhand handle = mmEffect(mm_word(id));
    _add_sound_to_queue(priority, handle);
    return handle;
}

mm_sfxhand play_sound(int priority, int id, int volume, int speed, int panning)
{
    mm_sound_effect sound_effect;
    sound_effect.id = mm_word(id);
    sound_effect.rate = mm_hword(speed);
    sound_effect.handle = 0;
    sound_effect.volume = mm_byte(volume);
    sound_effect.panning = mm_byte(panning);
    _check_sounds_queue();

    mm_sfxhand handle = mmEffectEx(&sound_effect);
    _add_sound_to_queue(priority, handle);
    return handle;
}

void stop_sound(mm_sfxhand handle)
{
    if(mmEffectActive(handle))
    {
        mmEffectCancel(handle);
        _erase_sound_from_queue(handle);
    }
}

void release_sound(mm_sfxhand handle)
{
    if(mmEffectActive(handle))
    {
        mmEffectRelease(handle);
        _erase_sound_from_queue(handle);
    }
}

void stop_all_sounds()
{
    mmEffectCancelAll();
    data.sounds_queue.clear();
}

bool update_on_vblank()
{
    return data.update_on_vblank;
}

void set_update_on_vblank(bool update_on_vblank)
{
    data.update_on_vblank = update_on_vblank;
}

void update()
{
    data.delay_commit = ! data.update_on_vblank;
}

void update_sounds_queue()
{
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

void commit()
{
    if(data.delay_commit)
    {
        _commit();
        data.delay_commit = false;
    }
}

}
