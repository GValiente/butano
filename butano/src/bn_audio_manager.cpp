/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_audio_manager.h"

#include "bn_math.h"
#include "bn_config_audio.h"
#include "bn_dmg_music_position.h"
#include "../hw/include/bn_hw_audio.h"

#include "bn_audio.cpp.h"
#include "bn_music.cpp.h"
#include "bn_sound.cpp.h"
#include "bn_dmg_music.cpp.h"
#include "bn_music_item.cpp.h"
#include "bn_sound_item.cpp.h"
#include "bn_dmg_music_item.cpp.h"

namespace bn::audio_manager
{

namespace
{
    constexpr int max_commands = BN_CFG_AUDIO_MAX_COMMANDS;
    static_assert(max_commands > 2, "Invalid max audio commands");


    class play_music_command
    {

    public:
        play_music_command(int id, bool loop, int volume) :
            _id(id),
            _volume(volume),
            _loop(loop)
        {
        }

        void execute() const
        {
            hw::audio::play_music(_id, _volume, _loop);
        }

    private:
        int _id;
        int _volume;
        bool _loop;
    };


    class set_music_position_command
    {

    public:
        explicit set_music_position_command(int position) :
            _position(position)
        {
        }

        void execute() const
        {
            hw::audio::set_music_position(_position);
        }

    private:
        int _position;
    };


    class set_music_volume_command
    {

    public:
        explicit set_music_volume_command(int volume) :
            _volume(volume)
        {
        }

        void execute() const
        {
            hw::audio::set_music_volume(_volume);
        }

    private:
        int _volume;
    };


    class play_dmg_music_command
    {

    public:
        play_dmg_music_command(const void* song, bool loop, int speed) :
            _song(song),
            _speed(speed),
            _loop(loop)
        {
        }

        void execute() const
        {
            hw::audio::play_dmg_music(_song, _speed, _loop);
        }

    private:
        const void* _song;
        int _speed;
        bool _loop;
    };


    class set_dmg_music_position_command
    {

    public:
        set_dmg_music_position_command(int pattern, int row) :
            _pattern(pattern),
            _row(row)
        {
        }

        void execute() const
        {
            hw::audio::set_dmg_music_position(_pattern, _row);
        }

    private:
        int _pattern;
        int _row;
    };


    class set_dmg_music_volume_command
    {

    public:
        set_dmg_music_volume_command(int left_volume, int right_volume) :
            _left_volume(left_volume),
            _right_volume(right_volume)
        {
        }

        void execute() const
        {
            hw::audio::set_dmg_music_volume(_left_volume, _right_volume);
        }

    private:
        int _left_volume;
        int _right_volume;
    };


    class play_sound_command
    {

    public:
        play_sound_command(int priority, int id) :
            _priority(priority),
            _id(id)
        {
        }

        void execute() const
        {
            hw::audio::play_sound(_priority, _id);
        }

    private:
        int _priority;
        int _id;
    };


    class play_sound_ex_command
    {

    public:
        play_sound_ex_command(int priority, int id, int volume, int speed, int panning) :
            _priority(priority),
            _id(id),
            _speed(uint16_t(speed)),
            _volume(uint8_t(volume)),
            _panning(uint8_t(panning))
        {
        }

        void execute() const
        {
            hw::audio::play_sound(_priority, _id, _volume, _speed, _panning);
        }

    private:
        int _priority;
        int _id;
        uint16_t _speed;
        uint8_t _volume;
        uint8_t _panning;
    };


    enum command_code : uint8_t
    {
        MUSIC_PLAY,
        MUSIC_STOP,
        MUSIC_PAUSE,
        MUSIC_RESUME,
        MUSIC_SET_POSITION,
        MUSIC_SET_VOLUME,
        DMG_MUSIC_PLAY,
        DMG_MUSIC_STOP,
        DMG_MUSIC_PAUSE,
        DMG_MUSIC_RESUME,
        DMG_MUSIC_SET_POSITION,
        DMG_MUSIC_SET_VOLUME,
        SOUND_PLAY,
        SOUND_PLAY_EX,
        SOUND_STOP_ALL
    };


    struct command_data
    {
        int data[3];
    };


    static_assert(sizeof(play_sound_ex_command) == sizeof(command_data));
    static_assert(alignof(play_sound_ex_command) == alignof(command_data));


    class static_data
    {

    public:
        command_data command_datas[max_commands];
        fixed music_volume;
        bn::dmg_music_position dmg_music_position;
        fixed dmg_music_left_volume;
        fixed dmg_music_right_volume;
        int commands_count = 0;
        int music_item_id = 0;
        int music_position = 0;
        const uint8_t* dmg_music_data = nullptr;
        command_code command_codes[max_commands];
        bool music_playing = false;
        bool music_paused = false;
        bool dmg_music_paused = false;
        bool dmg_sync_enabled = false;
    };

    BN_DATA_EWRAM static_data data;


    int _hw_music_volume(fixed volume)
    {
        return fixed_t<10>(volume).data();
    }

    int _hw_sound_volume(fixed volume)
    {
        return min(fixed_t<8>(volume).data(), 255);
    }

    int _hw_dmg_music_volume(fixed volume)
    {
        return fixed_t<3>(volume).data();
    }

    int _hw_sound_speed(fixed speed)
    {
        return min(fixed_t<10>(speed).data(), 65535);
    }

    int _hw_sound_panning(fixed panning)
    {
        return min(fixed_t<7>(panning + 1).data(), 255);
    }
}

void init()
{
    hw::audio::init();
}

void enable()
{
    hw::audio::enable();
}

void disable()
{
    hw::audio::disable();
}

bool music_playing()
{
    return data.music_playing;
}

optional<music_item> playing_music_item()
{
    optional<music_item> result;

    if(data.music_playing)
    {
        result = music_item(data.music_item_id);
    }

    return result;
}

void play_music(music_item item, fixed volume, bool loop)
{
    int commands = data.commands_count;
    BN_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = MUSIC_PLAY;
    new(data.command_datas + commands) play_music_command(item.id(), loop, _hw_music_volume(volume));
    data.commands_count = commands + 1;

    data.music_item_id = item.id();
    data.music_position = 0;
    data.music_volume = volume;
    data.music_playing = true;
    data.music_paused = false;
}

void stop_music()
{
    BN_ASSERT(data.music_playing, "There's no music playing");

    int commands = data.commands_count;
    BN_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = MUSIC_STOP;
    data.commands_count = commands + 1;

    data.music_playing = false;
    data.music_paused = false;
}

bool music_paused()
{
    return data.music_paused;
}

void pause_music()
{
    BN_ASSERT(data.music_playing, "There's no music playing");
    BN_ASSERT(! data.music_paused, "Music is already paused");

    int commands = data.commands_count;
    BN_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = MUSIC_PAUSE;
    data.commands_count = commands + 1;

    data.music_paused = true;
}

void resume_music()
{
    BN_ASSERT(data.music_paused, "Music is not paused");

    int commands = data.commands_count;
    BN_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = MUSIC_RESUME;
    data.commands_count = commands + 1;

    data.music_paused = false;
}

int music_position()
{
    BN_ASSERT(data.music_playing, "There's no music playing");

    return data.music_position;
}

void set_music_position(int position)
{
    BN_ASSERT(data.music_playing, "There's no music playing");

    int commands = data.commands_count;
    BN_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = MUSIC_SET_POSITION;
    new(data.command_datas + commands) set_music_position_command(position);
    data.commands_count = commands + 1;

    data.music_position = position;
}

fixed music_volume()
{
    BN_ASSERT(data.music_playing, "There's no music playing");

    return data.music_volume;
}

void set_music_volume(fixed volume)
{
    if(volume != data.music_volume)
    {
        BN_ASSERT(data.music_playing, "There's no music playing");

        int commands = data.commands_count;
        BN_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = MUSIC_SET_VOLUME;
        new(data.command_datas + commands) set_music_volume_command(_hw_music_volume(volume));
        data.commands_count = commands + 1;

        data.music_volume = volume;
    }
}

bool dmg_music_playing()
{
    return data.dmg_music_data;
}

optional<dmg_music_item> playing_dmg_music_item()
{
    optional<dmg_music_item> result;

    if(const uint8_t* dmg_music_data = data.dmg_music_data)
    {
        result = dmg_music_item(*dmg_music_data);
    }

    return result;
}

void play_dmg_music(dmg_music_item item, int speed, bool loop)
{
    int commands = data.commands_count;
    BN_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = DMG_MUSIC_PLAY;
    new(data.command_datas + commands) play_dmg_music_command(item.data_ptr(), loop, speed);
    data.commands_count = commands + 1;

    data.dmg_music_position = bn::dmg_music_position();
    data.dmg_music_left_volume = 1;
    data.dmg_music_right_volume = 1;
    data.dmg_music_data = item.data_ptr();
    data.dmg_music_paused = false;
}

void stop_dmg_music()
{
    BN_ASSERT(data.dmg_music_data, "There's no DMG music playing");

    int commands = data.commands_count;
    BN_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = DMG_MUSIC_STOP;
    data.commands_count = commands + 1;

    data.dmg_music_data = nullptr;
    data.dmg_music_paused = false;
}

bool dmg_music_paused()
{
    return data.dmg_music_paused;
}

void pause_dmg_music()
{
    BN_ASSERT(data.dmg_music_data, "There's no DMG music playing");
    BN_ASSERT(! data.dmg_music_paused, "DMG music is already paused");

    int commands = data.commands_count;
    BN_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = DMG_MUSIC_PAUSE;
    data.commands_count = commands + 1;

    data.dmg_music_paused = true;
}

void resume_dmg_music()
{
    BN_ASSERT(data.dmg_music_paused, "DMG music is not paused");

    int commands = data.commands_count;
    BN_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = DMG_MUSIC_RESUME;
    data.commands_count = commands + 1;

    data.dmg_music_paused = false;
}

const bn::dmg_music_position& dmg_music_position()
{
    BN_ASSERT(data.dmg_music_data, "There's no DMG music playing");

    return data.dmg_music_position;
}

void set_dmg_music_position(const bn::dmg_music_position& position)
{
    BN_ASSERT(data.dmg_music_data, "There's no DMG music playing");

    int commands = data.commands_count;
    BN_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = DMG_MUSIC_SET_POSITION;
    new(data.command_datas + commands) set_dmg_music_position_command(position.pattern(), position.row());
    data.commands_count = commands + 1;

    data.dmg_music_position = position;
}

fixed dmg_music_left_volume()
{
    BN_ASSERT(data.dmg_music_data, "There's no DMG music playing");

    return data.dmg_music_left_volume;
}

fixed dmg_music_right_volume()
{
    BN_ASSERT(data.dmg_music_data, "There's no DMG music playing");

    return data.dmg_music_right_volume;
}

void set_dmg_music_left_volume(fixed left_volume)
{
    set_dmg_music_volume(left_volume, data.dmg_music_right_volume);
}

void set_dmg_music_right_volume(fixed right_volume)
{
    set_dmg_music_volume(data.dmg_music_left_volume, right_volume);
}

void set_dmg_music_volume(fixed left_volume, fixed right_volume)
{
    if(left_volume != data.dmg_music_left_volume || right_volume != data.dmg_music_right_volume)
    {
        BN_ASSERT(data.dmg_music_data, "There's no DMG music playing");

        int commands = data.commands_count;
        BN_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = DMG_MUSIC_SET_VOLUME;
        new(data.command_datas + commands) set_dmg_music_volume_command(
                    _hw_dmg_music_volume(left_volume), _hw_dmg_music_volume(right_volume));
        data.commands_count = commands + 1;

        data.dmg_music_left_volume = left_volume;
        data.dmg_music_right_volume = right_volume;
    }
}

bool dmg_sync_enabled()
{
    return data.dmg_sync_enabled;
}

void set_dmg_sync_enabled(bool enabled)
{
    data.dmg_sync_enabled = enabled;
}

void play_sound(int priority, sound_item item)
{
    int commands = data.commands_count;
    BN_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = SOUND_PLAY;
    new(data.command_datas + commands) play_sound_command(priority, item.id());
    data.commands_count = commands + 1;
}

void play_sound(int priority, sound_item item, fixed volume, fixed speed, fixed panning)
{
    int commands = data.commands_count;
    BN_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = SOUND_PLAY_EX;
    new(data.command_datas + commands) play_sound_ex_command(
                priority, item.id(), _hw_sound_volume(volume), _hw_sound_speed(speed), _hw_sound_panning(panning));
    data.commands_count = commands + 1;
}

void stop_all_sounds()
{
    int commands = data.commands_count;
    BN_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = SOUND_STOP_ALL;
    data.commands_count = commands + 1;
}

bool update_on_vblank()
{
    return hw::audio::update_on_vblank();
}

void set_update_on_vblank(bool update_on_vblank)
{
    hw::audio::set_update_on_vblank(update_on_vblank);
}

void disable_vblank_handler()
{
    hw::audio::disable_vblank_handler();
}

void update()
{
    hw::audio::update(data.dmg_sync_enabled);
}

void execute_commands()
{
    hw::audio::update_sounds_queue();

    for(int index = 0, limit = data.commands_count; index < limit; ++index)
    {
        switch(data.command_codes[index])
        {

        case MUSIC_PLAY:
            reinterpret_cast<const play_music_command&>(data.command_datas[index].data).execute();
            break;

        case MUSIC_STOP:
            hw::audio::stop_music();
            break;

        case MUSIC_PAUSE:
            hw::audio::pause_music();
            break;

        case MUSIC_RESUME:
            hw::audio::resume_music();
            break;

        case MUSIC_SET_POSITION:
            reinterpret_cast<const set_music_position_command&>(data.command_datas[index].data).execute();
            break;

        case MUSIC_SET_VOLUME:
            reinterpret_cast<const set_music_volume_command&>(data.command_datas[index].data).execute();
            break;

        case DMG_MUSIC_PLAY:
            reinterpret_cast<const play_dmg_music_command&>(data.command_datas[index].data).execute();
            break;

        case DMG_MUSIC_STOP:
            hw::audio::stop_dmg_music();
            break;

        case DMG_MUSIC_PAUSE:
            hw::audio::pause_dmg_music();
            break;

        case DMG_MUSIC_RESUME:
            hw::audio::resume_dmg_music();
            break;

        case DMG_MUSIC_SET_POSITION:
            reinterpret_cast<const set_dmg_music_position_command&>(data.command_datas[index].data).execute();
            break;

        case DMG_MUSIC_SET_VOLUME:
            reinterpret_cast<const set_dmg_music_volume_command&>(data.command_datas[index].data).execute();
            break;

        case SOUND_PLAY:
            reinterpret_cast<const play_sound_command&>(data.command_datas[index].data).execute();
            break;

        case SOUND_PLAY_EX:
            reinterpret_cast<const play_sound_ex_command&>(data.command_datas[index].data).execute();
            break;

        case SOUND_STOP_ALL:
            hw::audio::stop_all_sounds();
            break;

        default:
            break;
        }
    }

    data.commands_count = 0;

    if(data.music_playing && hw::audio::music_playing())
    {
        data.music_position = hw::audio::music_position();
    }

    if(data.dmg_music_data)
    {
        int pattern;
        int row;
        hw::audio::dmg_music_position(pattern, row);

        if(pattern >= 0)
        {
            data.dmg_music_position = bn::dmg_music_position(pattern, row);
        }
    }
}

void commit()
{
    hw::audio::commit();
}

void stop()
{
    data.commands_count = 0;

    if(data.music_playing)
    {
        stop_music();
    }

    stop_all_sounds();
}

}
