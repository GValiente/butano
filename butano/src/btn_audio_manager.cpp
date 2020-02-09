#include "btn_audio_manager.h"

#include "btn_fixed.h"
#include "btn_vector.h"
#include "btn_algorithm.h"
#include "btn_music_item.h"
#include "btn_sound_item.h"
#include "btn_config_audio.h"
#include "../hw/include/btn_hw_audio.h"

namespace btn::audio_manager
{

namespace
{
    static_assert(BTN_CFG_AUDIO_MAX_COMMANDS > 1, "Invalid audio max commands");


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
                hw::audio::play_music(_id, _volume, _loop);
                return;

            case MUSIC_STOP:
                hw::audio::stop_music();
                return;

            case MUSIC_PAUSE:
                hw::audio::pause_music();
                return;

            case MUSIC_RESUME:
                hw::audio::resume_music();
                return;

            case MUSIC_SET_VOLUME:
                hw::audio::set_music_volume(_volume);
                return;

            case SOUND_PLAY:
                hw::audio::play_sound(_id);
                return;

            case SOUND_PLAY_EX:
                hw::audio::play_sound(_id, _volume, _speed, _panning);
                return;

            case SOUND_STOP_ALL:
                hw::audio::stop_all_sounds();
                return;
            }
        }

    private:
        int _id;
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
            _id(id),
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
        int direct_sound_control_value = 0;
        bool music_playing = false;
        bool music_paused = false;
    };

    BTN_DATA_EWRAM static_data data;


    int _hw_music_volume(fixed volume)
    {
        return fixed_t<10>(volume).value();
    }

    int _hw_sound_volume(fixed volume)
    {
        return min(fixed_t<8>(volume).value(), 255);
    }

    int _hw_sound_speed(fixed speed)
    {
        return min(fixed_t<10>(speed).value(), 65535);
    }

    int _hw_sound_panning(fixed panning)
    {
        return min(fixed_t<7>(panning + 1).value(), 255);
    }
}

void init()
{
    hw::audio::init();
}

void add_irq()
{
    hw::audio::add_irq();
}

void remove_irq()
{
    hw::audio::remove_irq();
}

bool music_playing()
{
    return data.music_playing;
}

void play_music(music_item item, fixed volume, bool loop)
{
    BTN_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0, 1]: ", volume);

    if(data.music_playing)
    {
        stop_music();
    }

    BTN_ASSERT(! data.commands.full(), "No more audio commands available");

    data.commands.push_back(command::music_play(item, loop, _hw_music_volume(volume)));
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

void set_music_volume(fixed volume)
{
    BTN_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0, 1]: ", volume);
    BTN_ASSERT(data.music_playing, "There's no music playing");
    BTN_ASSERT(! data.commands.full(), "No more audio commands available");

    data.commands.push_back(command::music_set_volume(_hw_music_volume(volume)));
}

void play_sound(sound_item item)
{
    BTN_ASSERT(! data.commands.full(), "No more audio commands available");

    data.commands.push_back(command::sound_play(item));
}

void play_sound(sound_item item, fixed volume, fixed speed, fixed panning)
{
    BTN_ASSERT(volume >= 0 && volume <= 1, "Volume range is [0, 1]: ", volume);
    BTN_ASSERT(speed >= 0 && speed <= 64, "Speed range is [0, 64]: ", speed);
    BTN_ASSERT(panning >= -1 && panning <= 1, "Panning range is [-1, 1]: ", panning);
    BTN_ASSERT(! data.commands.full(), "No more audio commands available");

    data.commands.push_back(command::sound_play(item, _hw_sound_volume(volume), _hw_sound_speed(speed), _hw_sound_panning(panning)));
}

void stop_all_sounds()
{
    BTN_ASSERT(! data.commands.full(), "No more audio commands available");

    data.commands.push_back(command::sound_stop_all());
}

void sleep()
{
    data.direct_sound_control_value = hw::audio::direct_sound_control_value();
    hw::audio::set_direct_sound_control_value(0);
}

void wake_up()
{
    hw::audio::set_direct_sound_control_value(data.direct_sound_control_value);
}

void update()
{
    hw::audio::disable_vblank_handler();

    for(const command& command : data.commands)
    {
        command.execute();
    }

    data.commands.clear();

    if(data.music_playing && ! hw::audio::music_playing())
    {
        data.music_playing = false;
    }
}

void commit()
{
    hw::audio::enable_vblank_handler();
}

void stop()
{
    if(data.music_playing)
    {
        hw::audio::stop_music();
    }

    hw::audio::stop_all_sounds();
    hw::audio::disable_vblank_handler();
}

}
