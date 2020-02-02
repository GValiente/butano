#include "../include/btn_hw_audio.h"

#include "tonc.h"
#include "maxmod.h"
#include "btn_assert.h"
#include "btn_vector.h"
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


    class command
    {

    public:
        [[nodiscard]] static command music_play(music_item item, bool loop, int volume)
        {
            return command(MUSIC_PLAY, item.id(), volume, loop);
        }

        [[nodiscard]] static command music_stop()
        {
            return command(MUSIC_STOP);
        }

        [[nodiscard]] static command music_pause()
        {
            return command(MUSIC_PAUSE);
        }

        [[nodiscard]] static command music_resume()
        {
            return command(MUSIC_RESUME);
        }

        [[nodiscard]] static command music_set_volume(int volume)
        {
            return command(MUSIC_SET_VOLUME, 0, volume);
        }

        [[nodiscard]] static command sound_play(sound_item item)
        {
            return command(SOUND_PLAY, item.id());
        }

        [[nodiscard]] static command sound_play(sound_item item, int volume, int speed, int panning)
        {
            return command(SOUND_PLAY_EX, item.id(), volume, false, speed, panning);
        }

        [[nodiscard]] static command sound_stop_all()
        {
            return command(SOUND_STOP_ALL);
        }

        void execute() const
        {
            switch(type(_type))
            {

            case MUSIC_PLAY:
                mmStart(_id, _loop ? MM_PLAY_LOOP : MM_PLAY_ONCE);
                mmSetModuleVolume(_volume);
                return;

            case MUSIC_STOP:
                mmStop();
                return;

            case MUSIC_PAUSE:
                mmPause();
                return;

            case MUSIC_RESUME:
                mmResume();
                return;

            case MUSIC_SET_VOLUME:
                mmSetModuleVolume(_volume);
                return;

            case SOUND_PLAY:
                mmEffectRelease(mmEffect(_id));
                return;

            case SOUND_PLAY_EX:
                {
                    mm_sound_effect sound_effect;
                    sound_effect.id = _id;
                    sound_effect.rate = _speed;
                    sound_effect.handle = 0;
                    sound_effect.volume = mm_byte(_volume);
                    sound_effect.panning = _panning;
                    mmEffectRelease(mmEffectEx(&sound_effect));
                }
                return;

            case SOUND_STOP_ALL:
                mmEffectCancelAll();
                return;
            }
        }

    private:
        unsigned _id;
        uint16_t _volume;
        uint16_t _speed;
        uint8_t _type;
        uint8_t _panning;
        bool _loop;

        enum type
        {
            MUSIC_PLAY,
            MUSIC_STOP,
            MUSIC_PAUSE,
            MUSIC_RESUME,
            MUSIC_SET_VOLUME,
            SOUND_PLAY,
            SOUND_PLAY_EX,
            SOUND_STOP_ALL
        };

        explicit command(type type, int id = 0, int volume = 0, bool loop = false, int speed = 0, int panning = 0) :
            _id(unsigned(id)),
            _volume(uint16_t(volume)),
            _speed(uint16_t(speed)),
            _type(type),
            _panning(uint8_t(panning)),
            _loop(loop)
        {
        }
    };


    class static_data
    {

    public:
        vector<command, BTN_CFG_AUDIO_MAX_COMMANDS> commands;
        bool music_playing = false;
        bool music_paused = false;
        volatile bool locked = false;
    };

    BTN_DATA_EWRAM static_data data;

    alignas(sizeof(int)) BTN_DATA_EWRAM uint8_t maxmod_engine_buffer[BTN_CFG_AUDIO_MAX_CHANNELS *
            (MM_SIZEOF_MODCH + MM_SIZEOF_ACTCH + MM_SIZEOF_MIXCH) + _mix_length()];

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


    void _commit_handler()
    {
        lock lock;

        mmFrame();
    }
}

void init()
{
    lock lock;

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

bool music_playing()
{
    return data.music_playing;
}

void play_music(music_item item, bool loop, int volume)
{
    BTN_ASSERT(volume >= 0 && volume <= 1024, "Volume range is [0, 1024]: ", volume);

    if(data.music_playing)
    {
        stop_music();
    }

    BTN_ASSERT(! data.commands.full(), "No more audio commands available");

    data.commands.push_back(command::music_play(item, loop, volume));
    data.music_playing = true;
}

void stop_music()
{
    BTN_ASSERT(data.music_playing, "There's no music playing");
    BTN_ASSERT(! data.commands.full(), "No more audio commands available");

    data.commands.push_back(command::music_stop());
    data.music_playing = false;
    data.music_paused = false;
}

void pause_music()
{
    BTN_ASSERT(data.music_playing, "There's no music playing");
    BTN_ASSERT(! data.music_paused, "Music was already paused");
    BTN_ASSERT(! data.commands.full(), "No more audio commands available");

    data.commands.push_back(command::music_pause());
    data.music_paused = true;
}

void resume_music()
{
    BTN_ASSERT(data.music_paused, "Music was not paused");
    BTN_ASSERT(! data.commands.full(), "No more audio commands available");

    data.commands.push_back(command::music_resume());
    data.music_paused = false;
}

void set_music_volume(int volume)
{
    BTN_ASSERT(volume >= 0 && volume <= 1024, "Volume range is [0, 1024]: ", volume);
    BTN_ASSERT(data.music_playing, "There's no music playing");
    BTN_ASSERT(! data.commands.full(), "No more audio commands available");

    data.commands.push_back(command::music_set_volume(volume));
}

void play_sound(sound_item item)
{
    BTN_ASSERT(! data.commands.full(), "No more audio commands available");

    data.commands.push_back(command::sound_play(item));
}

void play_sound(sound_item item, int volume, int speed, int panning)
{
    BTN_ASSERT(volume >= 0 && volume <= 255, "Volume range is [0, 255]: ", volume);
    BTN_ASSERT(speed >= 0 && speed <= 65535, "Speed range is [0, 65535]: ", speed);
    BTN_ASSERT(panning >= 0 && panning <= 255, "Panning range is [0, 255]: ", panning);
    BTN_ASSERT(! data.commands.full(), "No more audio commands available");

    data.commands.push_back(command::sound_play(item, volume, speed, panning));
}

void stop_all_sounds()
{
    BTN_ASSERT(! data.commands.full(), "No more audio commands available");

    data.commands.push_back(command::sound_stop_all());
}

int direct_sound_control_value()
{
    lock lock;

    return REG_SNDDSCNT;
}

void set_direct_sound_control_value(int value)
{
    lock lock;

    REG_SNDDSCNT = uint16_t(value);
}

void update()
{
    lock lock;

    mmSetVBlankHandler(nullptr);

    for(const command& command : data.commands)
    {
        command.execute();
    }

    data.commands.clear();

    if(data.music_playing && ! mmActive())
    {
        data.music_playing = false;
    }
}

void commit()
{
    lock lock;

    mmFrame();
    mmSetVBlankHandler(reinterpret_cast<void*>(_commit_handler));
}

void stop()
{
    lock lock;

    if(data.music_playing)
    {
        mmStop();
    }

    mmEffectCancelAll();
    mmSetVBlankHandler(nullptr);
}

}
