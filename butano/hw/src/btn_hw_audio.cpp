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

    constexpr mm_mixmode mix_mode()
    {
        return BTN_CFG_AUDIO_KHZ == BTN_AUDIO_KHZ_8 ? MM_MIX_8KHZ :
                BTN_CFG_AUDIO_KHZ == BTN_AUDIO_KHZ_10 ? MM_MIX_10KHZ :
                BTN_CFG_AUDIO_KHZ == BTN_AUDIO_KHZ_13 ? MM_MIX_13KHZ :
                BTN_CFG_AUDIO_KHZ == BTN_AUDIO_KHZ_16 ? MM_MIX_16KHZ :
                BTN_CFG_AUDIO_KHZ == BTN_AUDIO_KHZ_18 ? MM_MIX_18KHZ :
                BTN_CFG_AUDIO_KHZ == BTN_AUDIO_KHZ_21 ? MM_MIX_21KHZ :
                BTN_CFG_AUDIO_KHZ == BTN_AUDIO_KHZ_27 ? MM_MIX_27KHZ :
                MM_MIX_31KHZ;
    }

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
        bool audio_enabled = true;
        bool update_audio = false;
    };

    alignas(sizeof(int)) BTN_DATA_EWRAM uint8_t maxmod_engine_buffer[BTN_CFG_AUDIO_MAX_CHANNELS *
            (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH + MM_SIZEOF_MIXCH) + mix_length()];

    alignas(sizeof(int)) uint8_t maxmod_mixing_buffer[mix_length()];

    BTN_DATA_EWRAM static_data data;

    void commit_handler()
    {
        if(data.audio_enabled)
        {
            mmFrame();
        }
        else
        {
            data.update_audio = true;
        }
    }

    void enable_audio()
    {
        if(data.update_audio)
        {
            mmFrame();
            data.update_audio = false;
        }

        data.audio_enabled = true;
    }
}

void init()
{
    mm_gba_system maxmod_info;
    maxmod_info.mixing_mode = mix_mode();
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
    irq::add(irq::id::VBLANK, mmVBlank);
}

void remove_irq()
{
    irq::remove(irq::id::VBLANK);
}

void pause_commits()
{
    mmSetVBlankHandler(nullptr);
}

void resume_commits()
{
    commit_handler();
    mmSetVBlankHandler(reinterpret_cast<void*>(commit_handler));
}

bool music_playing()
{
    return data.music_playing && mmActive();
}

void play_music(music_item item, bool loop, int volume)
{
    BTN_ASSERT(volume >= 0 && volume <= 1024, "Volume range is [0, 1024]: ", volume);

    if(data.music_playing)
    {
        stop_music();
    }

    data.audio_enabled = false;
    mmSetModuleVolume(0);
    mmStart(unsigned(item.id()), loop ? MM_PLAY_LOOP : MM_PLAY_ONCE);
    enable_audio();

    data.music_playing = true;
    set_music_volume(volume);
}

void stop_music()
{
    BTN_ASSERT(data.music_playing, "There's no music playing");

    data.audio_enabled = false;
    mmStop();
    enable_audio();

    data.music_playing = data.music_paused = false;
}

void pause_music()
{
    BTN_ASSERT(data.music_playing, "There's no music playing");
    BTN_ASSERT(! data.music_paused, "Music was already paused");

    data.audio_enabled = false;
    mmPause();
    enable_audio();

    data.music_paused = true;
}

void resume_music()
{
    BTN_ASSERT(data.music_paused, "Music was not paused");

    data.audio_enabled = false;
    mmResume();
    enable_audio();

    data.music_paused = false;
}

void set_music_volume(int volume)
{
    BTN_ASSERT(volume >= 0 && volume <= 1024, "Volume range is [0, 1024]: ", volume);
    BTN_ASSERT(data.music_playing, "There's no music playing");

    data.audio_enabled = false;
    mmSetModuleVolume(mm_word(volume));
    enable_audio();
}

void play_sound(sound_item item)
{
    mm_sfxhand sfx_handle = mmEffect(unsigned(item.id()));
    mmEffectRelease(sfx_handle);
}

void play_sound(sound_item item, int volume, int speed, int panning)
{
    BTN_ASSERT(volume >= 0 && volume <= 255, "Volume range is [0, 255]: ", volume);
    BTN_ASSERT(speed >= 0 && speed <= 65535, "Speed range is [0, 65535]: ", speed);
    BTN_ASSERT(panning >= 0 && panning <= 255, "Panning range is [0, 255]: ", panning);

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
    data.audio_enabled = false;
    mmEffectCancelAll();
    enable_audio();
}

int direct_sound_control_value()
{
    return REG_SNDDSCNT;
}

void set_direct_sound_control_value(int value)
{
    REG_SNDDSCNT = uint16_t(value);
}

}
