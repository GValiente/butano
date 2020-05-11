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
        uint16_t direct_sound_control_value = 0;
        volatile bool locked = false;
    };

    BTN_DATA_EWRAM static_data data;


    constexpr int _mix_length()
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

    constexpr const int _max_channels = BTN_CFG_AUDIO_MAX_MUSIC_CHANNELS + BTN_CFG_AUDIO_MAX_SOUND_CHANNELS;

    alignas(sizeof(int)) BTN_DATA_EWRAM uint8_t maxmod_engine_buffer[
            _max_channels * (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH + MM_SIZEOF_MIXCH) + _mix_length()];

    alignas(sizeof(int)) uint8_t maxmod_mixing_buffer[_mix_length()];


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

    void _commit_handler()
    {
        lock lock;

        mmFrame();
    }
}

void init()
{
    lock lock;

    irq::add(irq::id::VBLANK, mmVBlank);

    mm_gba_system maxmod_info;
    maxmod_info.mixing_mode = mm_mixmode(BTN_CFG_AUDIO_KHZ);
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
    lock lock;

    irq::enable(irq::id::VBLANK);
}

void disable()
{
    lock lock;

    irq::disable(irq::id::VBLANK);
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

void release_inactive_sounds()
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

void sleep()
{
    lock lock;

    data.direct_sound_control_value = REG_SNDDSCNT;
    REG_SNDDSCNT = 0;
}

void wake_up()
{
    lock lock;

    REG_SNDDSCNT = uint16_t(data.direct_sound_control_value);
}

void disable_vblank_handler()
{
    lock lock;

    mmSetVBlankHandler(nullptr);
}

void enable_vblank_handler()
{
    lock lock;

    mmFrame();
    mmSetVBlankHandler(reinterpret_cast<void*>(_commit_handler));
}

void stop()
{
    lock lock;

    stop_music();
    stop_all_sounds();
}

}
