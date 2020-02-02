#include "../include/btn_hw_audio.h"

#include "tonc.h"
#include "maxmod.h"
#include "btn_assert.h"
#include "btn_music_item.h"
#include "btn_sound_item.h"
#include "btn_config_audio.h"
#include "../include/btn_hw_irq.h"

extern const uint8_t _btn_audio_soundbank_bin[];

namespace btn::hw::audio
{

namespace
{
    static_assert(BTN_CFG_AUDIO_MAX_CHANNELS > 0, "Invalid max audio channels");

    constexpr int mix_length()
    {
        return BTN_CFG_AUDIO_KHZ == BTN_AUDIO_KHZ_8 ? MM_MIXLEN_8KHZ :
                BTN_CFG_AUDIO_KHZ == BTN_AUDIO_KHZ_10 ? MM_MIXLEN_10KHZ :
                BTN_CFG_AUDIO_KHZ == BTN_AUDIO_KHZ_13 ? MM_MIXLEN_13KHZ :
                BTN_CFG_AUDIO_KHZ == BTN_AUDIO_KHZ_16 ? MM_MIXLEN_16KHZ :
                BTN_CFG_AUDIO_KHZ == BTN_AUDIO_KHZ_18 ? MM_MIXLEN_18KHZ :
                BTN_CFG_AUDIO_KHZ == BTN_AUDIO_KHZ_21 ? MM_MIXLEN_21KHZ :
                BTN_CFG_AUDIO_KHZ == BTN_AUDIO_KHZ_27 ? MM_MIXLEN_27KHZ :
                MM_MIXLEN_31KHZ;
    }

    class static_data
    {

    public:
        bool music_playing = false;
        bool music_paused = false;
        volatile bool locked = false;
    };

    BTN_DATA_EWRAM static_data data;

    alignas(sizeof(int)) BTN_DATA_EWRAM uint8_t maxmod_engine_buffer[BTN_CFG_AUDIO_MAX_CHANNELS *
            (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH + MM_SIZEOF_MIXCH) + mix_length()];

    alignas(sizeof(int)) uint8_t maxmod_mixing_buffer[mix_length()];


    class lock
    {

    public:
        lock()
        {
            while(data.locked)
            {
            }

            data.locked = true;
        }

        ~lock()
        {
            data.locked = false;
        }
    };

    void _commit_handler()
    {
        lock lock;

        mmFrame();
    }
}

void init()
{
    mm_gba_system maxmod_info;
    maxmod_info.mixing_mode = mm_mixmode(BTN_CFG_AUDIO_KHZ);
    maxmod_info.mod_channel_count = BTN_CFG_AUDIO_MAX_CHANNELS;
    maxmod_info.mix_channel_count = BTN_CFG_AUDIO_MAX_CHANNELS;
    maxmod_info.module_channels = mm_addr(maxmod_engine_buffer);
    maxmod_info.active_channels = mm_addr(maxmod_engine_buffer + (BTN_CFG_AUDIO_MAX_CHANNELS * MM_SIZEOF_MODCH));
    maxmod_info.mixing_channels = mm_addr(maxmod_engine_buffer +
            (BTN_CFG_AUDIO_MAX_CHANNELS * (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH)));
    maxmod_info.mixing_memory = mm_addr(maxmod_mixing_buffer);
    maxmod_info.wave_memory = mm_addr(maxmod_engine_buffer +
            (BTN_CFG_AUDIO_MAX_CHANNELS * (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH + MM_SIZEOF_MIXCH)));
    maxmod_info.soundbank = mm_addr(_btn_audio_soundbank_bin);
    mmInit(&maxmod_info);
}

void stop()
{
    if(data.music_playing)
    {
        stop_music();
    }

    stop_all_sounds();
}

void add_irq()
{
    lock lock;

    irq::add(irq::id::VBLANK, mmVBlank);
}

void remove_irq()
{
    lock lock;

    irq::remove(irq::id::VBLANK);
}

void pause_commits()
{
    lock lock;

    mmSetVBlankHandler(nullptr);
}

void resume_commits()
{
    _commit_handler();

    lock lock;

    mmSetVBlankHandler(reinterpret_cast<void*>(_commit_handler));
}

bool music_playing()
{
    lock lock;

    return data.music_playing && mmActive();
}

void play_music(music_item item, bool loop, int volume)
{
    BTN_ASSERT(volume >= 0 && volume <= 1024, "Volume range is [0, 1024]: ", volume);

    lock lock;

    if(data.music_playing)
    {
        mmStop();
    }

    mmSetModuleVolume(0);
    mmStart(unsigned(item.id()), loop ? MM_PLAY_LOOP : MM_PLAY_ONCE);
    mmSetModuleVolume(mm_word(volume));
    data.music_playing = true;
    data.music_paused = false;
}

void stop_music()
{
    BTN_ASSERT(data.music_playing, "There's no music playing");

    lock lock;

    mmStop();
    data.music_playing = data.music_paused = false;
}

void pause_music()
{
    BTN_ASSERT(data.music_playing, "There's no music playing");
    BTN_ASSERT(! data.music_paused, "Music was already paused");

    lock lock;

    mmPause();
    data.music_paused = true;
}

void resume_music()
{
    BTN_ASSERT(data.music_paused, "Music was not paused");

    lock lock;

    mmResume();
    data.music_paused = false;
}

void set_music_volume(int volume)
{
    BTN_ASSERT(volume >= 0 && volume <= 1024, "Volume range is [0, 1024]: ", volume);
    BTN_ASSERT(data.music_playing, "There's no music playing");

    lock lock;

    mmSetModuleVolume(mm_word(volume));
}

void play_sound(sound_item item)
{
    lock lock;

    mm_sfxhand sfx_handle = mmEffect(unsigned(item.id()));
    mmEffectRelease(sfx_handle);
}

void play_sound(sound_item item, int volume, int speed, int panning)
{
    BTN_ASSERT(volume >= 0 && volume <= 255, "Volume range is [0, 255]: ", volume);
    BTN_ASSERT(speed >= 0 && speed <= 65535, "Speed range is [0, 65535]: ", speed);
    BTN_ASSERT(panning >= 0 && panning <= 255, "Panning range is [0, 255]: ", panning);

    lock lock;

    mm_sound_effect sound_effect;
    sound_effect.id = unsigned(item.id());
    sound_effect.rate = mm_hword(speed);
    sound_effect.handle = 0;
    sound_effect.volume = mm_byte(volume);
    sound_effect.panning = mm_byte(panning);

    mm_sfxhand sfx_handle = mmEffectEx(&sound_effect);
    mmEffectRelease(sfx_handle);
}

void stop_all_sounds()
{
    lock lock;

    mmEffectCancelAll();
}

int direct_sound_control_value()
{
    return REG_SNDDSCNT;
}

void set_direct_sound_control_value(int value)
{
    lock lock;

    REG_SNDDSCNT = uint16_t(value);
}

}
