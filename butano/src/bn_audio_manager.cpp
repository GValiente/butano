/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_audio_manager.h"

#include "bn_config_audio.h"
#include "bn_unordered_map.h"
#include "bn_identity_hasher.h"
#include "bn_dmg_music_position.h"
#include "../hw/include/bn_hw_audio.h"

#include "bn_audio.cpp.h"
#include "bn_music.cpp.h"
#include "bn_sound.cpp.h"
#include "bn_jingle.cpp.h"
#include "bn_dmg_music.cpp.h"
#include "bn_music_item.cpp.h"
#include "bn_sound_item.cpp.h"
#include "bn_sound_handle.cpp.h"
#include "bn_dmg_music_item.cpp.h"

namespace bn::audio_manager
{

struct sound_data_type
{
    int item_id;
    fixed speed;
    fixed panning;
    optional<mm_sfxhand> hw_handle;

    void init(bn::sound_item _item, fixed _speed, fixed _panning)
    {
        item_id = _item.id();
        speed = _speed;
        panning = _panning;
        hw_handle.reset();
    }
};

namespace
{
    constexpr int max_commands = BN_CFG_AUDIO_MAX_COMMANDS;
    static_assert(max_commands > 2, "Invalid max audio commands");

    constexpr int max_sound_channels = BN_CFG_AUDIO_MAX_SOUND_CHANNELS;
    static_assert(max_sound_channels > 0, "Invalid max sound channels");
    static_assert(power_of_two(max_sound_channels), "Invalid max sound channels");


    int _hw_music_volume(fixed volume)
    {
        return fixed_t<10>(volume).data();
    }

    int _hw_music_tempo(fixed tempo)
    {
        return fixed_t<10>(tempo).data();
    }

    int _hw_music_pitch(fixed pitch)
    {
        return fixed_t<10>(pitch).data();
    }

    int _hw_sound_volume(fixed volume)
    {
        return min(fixed_t<8>(volume).data(), 255);
    }

    int _hw_sound_master_volume(fixed volume)
    {
        return fixed_t<10>(volume).data();
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
            hw::audio::play_music(_id, _loop);
            hw::audio::set_music_volume(_volume);
            hw::audio::set_music_tempo(_hw_music_tempo(1));
            hw::audio::set_music_pitch(_hw_music_pitch(1));
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


    class set_music_tempo_command
    {

    public:
        explicit set_music_tempo_command(int tempo) :
            _tempo(tempo)
        {
        }

        void execute() const
        {
            hw::audio::set_music_tempo(_tempo);
        }

    private:
        int _tempo;
    };


    class set_music_pitch_command
    {

    public:
        explicit set_music_pitch_command(int pitch) :
            _pitch(pitch)
        {
        }

        void execute() const
        {
            hw::audio::set_music_pitch(_pitch);
        }

    private:
        int _pitch;
    };


    class play_jingle_command
    {

    public:
        play_jingle_command(int id, int volume) :
            _id(id),
            _volume(volume)
        {
        }

        void execute() const
        {
            hw::audio::play_jingle(_id);
            hw::audio::set_jingle_volume(_volume);
        }

    private:
        int _id;
        int _volume;
    };


    class set_jingle_volume_command
    {

    public:
        explicit set_jingle_volume_command(int volume) :
            _volume(volume)
        {
        }

        void execute() const
        {
            hw::audio::set_jingle_volume(_volume);
        }

    private:
        int _volume;
    };


    class play_dmg_music_command
    {

    public:
        play_dmg_music_command(const void* song, dmg_music_type type, bool loop, int speed) :
            _song(song),
            _speed(speed),
            _type(type),
            _loop(loop)
        {
        }

        void execute() const
        {
            hw::audio::play_dmg_music(_song, _type, _speed, _loop);
        }

    private:
        const void* _song;
        int _speed;
        dmg_music_type _type;
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


    class set_dmg_music_master_volume_command
    {

    public:
        explicit set_dmg_music_master_volume_command(int volume) :
            _volume(volume)
        {
        }

        void execute() const
        {
            hw::audio::set_dmg_music_master_volume(bn::dmg_music_master_volume(_volume));
        }

    private:
        int _volume;
    };


    class play_sound_command
    {

    public:
        play_sound_command(int priority, int id, uint16_t handle) :
            _id(id),
            _priority(int16_t(priority)),
            _handle(handle)
        {
        }

        void execute() const
        {
            if(sound_data_type* data = sound_data(_handle))
            {
                data->hw_handle = hw::audio::play_sound(_priority, _id);
            }
        }

    private:
        int _id;
        int16_t _priority;
        uint16_t _handle;
    };


    class play_sound_ex_command
    {

    public:
        play_sound_ex_command(int priority, int id, uint16_t handle, int volume, int speed, int panning) :
            _id(id),
            _priority(int16_t(priority)),
            _handle(handle),
            _speed(uint16_t(speed)),
            _volume(uint8_t(volume)),
            _panning(uint8_t(panning))
        {
        }

        void execute() const
        {
            if(sound_data_type* data = sound_data(_handle))
            {
                data->hw_handle = hw::audio::play_sound(_priority, _id, _volume, _speed, _panning);
            }
        }

    private:
        int _id;
        int16_t _priority;
        uint16_t _handle;
        uint16_t _speed;
        uint8_t _volume;
        uint8_t _panning;
    };


    class stop_sound_command
    {

    public:
        explicit stop_sound_command(uint16_t handle) :
            _handle(handle)
        {
        }

        void execute() const
        {
            if(sound_data_type* data = sound_data(uint16_t(_handle)))
            {
                if(const mm_sfxhand* hw_handle = data->hw_handle.get())
                {
                    hw::audio::stop_sound(*hw_handle);
                }
            }
        }

    private:
        int _handle;
    };


    class release_sound_command
    {

    public:
        explicit release_sound_command(uint16_t handle) :
            _handle(handle)
        {
        }

        void execute() const
        {
            if(sound_data_type* data = sound_data(uint16_t(_handle)))
            {
                if(const mm_sfxhand* hw_handle = data->hw_handle.get())
                {
                    hw::audio::release_sound(*hw_handle);
                }
            }
        }

    private:
        int _handle;
    };


    class set_sound_speed_command
    {

    public:
        set_sound_speed_command(uint16_t handle, int speed_scale) :
            _speed_scale(speed_scale),
            _handle(handle)
        {
        }

        void execute() const
        {
            if(sound_data_type* data = sound_data(_handle))
            {
                if(const mm_sfxhand* hw_handle = data->hw_handle.get())
                {
                    hw::audio::set_sound_speed(*hw_handle, _speed_scale);
                }
            }
        }

    private:
        int _speed_scale;
        uint16_t _handle;
    };


    class set_sound_panning_command
    {

    public:
        set_sound_panning_command(uint16_t handle, int panning) :
            _panning(panning),
            _handle(handle)
        {
        }

        void execute() const
        {
            if(sound_data_type* data = sound_data(_handle))
            {
                if(const mm_sfxhand* hw_handle = data->hw_handle.get())
                {
                    hw::audio::set_sound_panning(*hw_handle, _panning);
                }
            }
        }

    private:
        int _panning;
        uint16_t _handle;
    };


    class set_sound_master_volume_command
    {

    public:
        explicit set_sound_master_volume_command(int volume) :
            _volume(volume)
        {
        }

        void execute() const
        {
            hw::audio::set_sound_master_volume(_volume);
        }

    private:
        int _volume;
    };


    enum command_code : uint8_t
    {
        MUSIC_PLAY,
        MUSIC_STOP,
        MUSIC_PAUSE,
        MUSIC_RESUME,
        MUSIC_SET_POSITION,
        MUSIC_SET_VOLUME,
        MUSIC_SET_TEMPO,
        MUSIC_SET_PITCH,
        JINGLE_PLAY,
        JINGLE_SET_VOLUME,
        DMG_MUSIC_PLAY,
        DMG_MUSIC_STOP,
        DMG_MUSIC_PAUSE,
        DMG_MUSIC_RESUME,
        DMG_MUSIC_SET_POSITION,
        DMG_MUSIC_SET_VOLUME,
        DMG_MUSIC_SET_MASTER_VOLUME,
        SOUND_PLAY,
        SOUND_PLAY_EX,
        SOUND_STOP,
        SOUND_RELEASE,
        SOUND_SET_SPEED,
        SOUND_SET_PANNING,
        SOUND_STOP_ALL,
        SOUND_SET_MASTER_VOLUME
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
        unordered_map<unsigned, sound_data_type, max_sound_channels * 2, identity_hasher> sound_map;
        fixed music_volume;
        fixed music_tempo;
        fixed music_pitch;
        fixed jingle_volume;
        bn::dmg_music_position dmg_music_position;
        fixed dmg_music_left_volume;
        fixed dmg_music_right_volume;
        fixed sound_master_volume = 1;
        int commands_count = 0;
        int music_item_id = 0;
        int music_position = 0;
        int jingle_item_id = 0;
        const uint8_t* dmg_music_data = nullptr;
        uint16_t new_sound_handle = 0;
        command_code command_codes[max_commands];
        bn::dmg_music_type dmg_music_type = dmg_music_type::GBT_PLAYER;
        bn::dmg_music_master_volume dmg_music_master_volume = dmg_music_master_volume::QUARTER;
        bool music_playing = false;
        bool music_paused = false;
        bool jingle_playing = false;
        bool dmg_music_paused = false;
    };

    BN_DATA_EWRAM_BSS static_data data;
}

void init()
{
    new(&data) static_data();

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
    BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = MUSIC_PLAY;
    new(data.command_datas + commands) play_music_command(item.id(), loop, _hw_music_volume(volume));
    data.commands_count = commands + 1;

    data.music_item_id = item.id();
    data.music_position = 0;
    data.music_volume = volume;
    data.music_tempo = 1;
    data.music_pitch = 1;
    data.music_playing = true;
    data.music_paused = false;
}

void stop_music()
{
    if(data.music_playing)
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = MUSIC_STOP;
        data.commands_count = commands + 1;

        data.music_playing = false;
        data.music_paused = false;
    }
}

bool music_paused()
{
    return data.music_paused;
}

void pause_music()
{
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");
    BN_BASIC_ASSERT(! data.music_paused, "Music is already paused");

    int commands = data.commands_count;
    BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = MUSIC_PAUSE;
    data.commands_count = commands + 1;

    data.music_paused = true;
}

void resume_music()
{
    BN_BASIC_ASSERT(data.music_paused, "Music is not paused");

    int commands = data.commands_count;
    BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = MUSIC_RESUME;
    data.commands_count = commands + 1;

    data.music_paused = false;
}

int music_position()
{
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");

    return data.music_position;
}

void set_music_position(int position)
{
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");

    if(position != data.music_position)
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = MUSIC_SET_POSITION;
        new(data.command_datas + commands) set_music_position_command(position);
        data.commands_count = commands + 1;

        data.music_position = position;
    }
}

fixed music_volume()
{
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");

    return data.music_volume;
}

void set_music_volume(fixed volume)
{
    int hw_volume = _hw_music_volume(volume);
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");

    if(hw_volume != _hw_music_volume(data.music_volume))
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = MUSIC_SET_VOLUME;
        new(data.command_datas + commands) set_music_volume_command(hw_volume);
        data.commands_count = commands + 1;
    }

    data.music_volume = volume;
}

fixed music_tempo()
{
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");

    return data.music_tempo;
}

void set_music_tempo(fixed tempo)
{
    int hw_tempo = _hw_music_tempo(tempo);
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");

    if(hw_tempo != _hw_music_tempo(data.music_tempo))
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = MUSIC_SET_TEMPO;
        new(data.command_datas + commands) set_music_tempo_command(hw_tempo);
        data.commands_count = commands + 1;
    }

    data.music_tempo = tempo;
}

fixed music_pitch()
{
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");

    return data.music_pitch;
}

void set_music_pitch(fixed pitch)
{
    int hw_pitch = _hw_music_pitch(pitch);
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");

    if(hw_pitch != _hw_music_pitch(data.music_pitch))
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = MUSIC_SET_PITCH;
        new(data.command_datas + commands) set_music_pitch_command(hw_pitch);
        data.commands_count = commands + 1;
    }

    data.music_pitch = pitch;
}

bool jingle_playing()
{
    return data.jingle_playing;
}

optional<music_item> playing_jingle_item()
{
    optional<music_item> result;

    if(data.jingle_playing)
    {
        result = music_item(data.jingle_item_id);
    }

    return result;
}

void play_jingle(music_item item, fixed volume)
{
    int commands = data.commands_count;
    BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = JINGLE_PLAY;
    new(data.command_datas + commands) play_jingle_command(item.id(), _hw_music_volume(volume));
    data.commands_count = commands + 1;

    data.jingle_item_id = item.id();
    data.jingle_volume = volume;
    data.jingle_playing = true;
}

fixed jingle_volume()
{
    BN_BASIC_ASSERT(data.jingle_playing, "There's no jingle playing");

    return data.jingle_volume;
}

void set_jingle_volume(fixed volume)
{
    int hw_volume = _hw_music_volume(volume);
    BN_BASIC_ASSERT(data.jingle_playing, "There's no jingle playing");

    if(hw_volume != _hw_music_volume(data.jingle_volume))
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = JINGLE_SET_VOLUME;
        new(data.command_datas + commands) set_jingle_volume_command(hw_volume);
        data.commands_count = commands + 1;
    }

    data.jingle_volume = volume;
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
        result = dmg_music_item(*dmg_music_data, data.dmg_music_type);
    }

    return result;
}

void play_dmg_music(const dmg_music_item& item, int speed, bool loop)
{
    int commands = data.commands_count;
    BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = DMG_MUSIC_PLAY;
    new(data.command_datas + commands) play_dmg_music_command(item.data_ptr(), item.type(), loop, speed);
    data.commands_count = commands + 1;

    data.dmg_music_position = bn::dmg_music_position();
    data.dmg_music_left_volume = 1;
    data.dmg_music_right_volume = 1;
    data.dmg_music_data = item.data_ptr();
    data.dmg_music_type = item.type();
    data.dmg_music_paused = false;
}

void stop_dmg_music()
{
    if(data.dmg_music_data)
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = DMG_MUSIC_STOP;
        data.commands_count = commands + 1;

        data.dmg_music_data = nullptr;
        data.dmg_music_paused = false;
    }
}

bool dmg_music_paused()
{
    return data.dmg_music_paused;
}

void pause_dmg_music()
{
    BN_BASIC_ASSERT(data.dmg_music_data, "There's no DMG music playing");
    BN_BASIC_ASSERT(! data.dmg_music_paused, "DMG music is already paused");

    int commands = data.commands_count;
    BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = DMG_MUSIC_PAUSE;
    data.commands_count = commands + 1;

    data.dmg_music_paused = true;
}

void resume_dmg_music()
{
    BN_BASIC_ASSERT(data.dmg_music_paused, "DMG music is not paused");

    int commands = data.commands_count;
    BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = DMG_MUSIC_RESUME;
    data.commands_count = commands + 1;

    data.dmg_music_paused = false;
}

const bn::dmg_music_position& dmg_music_position()
{
    BN_BASIC_ASSERT(data.dmg_music_data, "There's no DMG music playing");

    return data.dmg_music_position;
}

void set_dmg_music_position(const bn::dmg_music_position& position)
{
    BN_BASIC_ASSERT(data.dmg_music_data, "There's no DMG music playing");

    if(position != data.dmg_music_position)
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = DMG_MUSIC_SET_POSITION;
        new(data.command_datas + commands) set_dmg_music_position_command(position.pattern(), position.row());
        data.commands_count = commands + 1;

        data.dmg_music_position = position;
    }
}

fixed dmg_music_left_volume()
{
    BN_BASIC_ASSERT(data.dmg_music_data, "There's no DMG music playing");

    return data.dmg_music_left_volume;
}

fixed dmg_music_right_volume()
{
    BN_BASIC_ASSERT(data.dmg_music_data, "There's no DMG music playing");

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
    int hw_left_volume = _hw_dmg_music_volume(left_volume);
    int hw_right_volume = _hw_dmg_music_volume(right_volume);
    BN_BASIC_ASSERT(data.dmg_music_data, "There's no DMG music playing");

    if(hw_left_volume != _hw_dmg_music_volume(data.dmg_music_left_volume) ||
            hw_right_volume != _hw_dmg_music_volume(data.dmg_music_right_volume))
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = DMG_MUSIC_SET_VOLUME;
        new(data.command_datas + commands) set_dmg_music_volume_command(hw_left_volume, hw_right_volume);
        data.commands_count = commands + 1;
    }

    data.dmg_music_left_volume = left_volume;
    data.dmg_music_right_volume = right_volume;
}

bn::dmg_music_master_volume dmg_music_master_volume()
{
    return data.dmg_music_master_volume;
}

void set_dmg_music_master_volume(bn::dmg_music_master_volume volume)
{
    if(volume != data.dmg_music_master_volume)
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = DMG_MUSIC_SET_MASTER_VOLUME;
        new(data.command_datas + commands) set_dmg_music_master_volume_command(int(volume));
        data.commands_count = commands + 1;

        data.dmg_music_master_volume = volume;
    }
}

sound_data_type* sound_data(uint16_t handle)
{
    auto it = data.sound_map.find(handle);

    if(it != data.sound_map.end())
    {
        return &it->second;
    }

    return nullptr;
}

uint16_t play_sound(int priority, bn::sound_item item)
{
    int commands = data.commands_count;
    BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");
    BN_BASIC_ASSERT(! data.sound_map.full(), "No more sound handles available");

    uint16_t handle = data.new_sound_handle;
    data.sound_map[handle].init(item, 1, 0);
    data.new_sound_handle = handle + 1;

    data.command_codes[commands] = SOUND_PLAY;
    new(data.command_datas + commands) play_sound_command(priority, item.id(), handle);
    data.commands_count = commands + 1;

    return handle;
}

uint16_t play_sound(int priority, bn::sound_item item, fixed volume, fixed speed, fixed panning)
{
    int commands = data.commands_count;
    BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");
    BN_BASIC_ASSERT(! data.sound_map.full(), "No more sound handles available");

    uint16_t handle = data.new_sound_handle;
    data.sound_map[handle].init(item, speed, panning);
    data.new_sound_handle = handle + 1;

    data.command_codes[commands] = SOUND_PLAY_EX;
    new(data.command_datas + commands) play_sound_ex_command(
            priority, item.id(), handle, _hw_sound_volume(volume), _hw_sound_speed(speed),
            _hw_sound_panning(panning));
    data.commands_count = commands + 1;

    return handle;
}

void stop_sound(uint16_t handle)
{
    if(data.sound_map.find(handle) != data.sound_map.end())
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = SOUND_STOP;
        new(data.command_datas + commands) stop_sound_command(handle);
        data.commands_count = commands + 1;
    }
}

void release_sound(uint16_t handle)
{
    if(data.sound_map.find(handle) != data.sound_map.end())
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = SOUND_RELEASE;
        new(data.command_datas + commands) release_sound_command(handle);
        data.commands_count = commands + 1;
    }
}

bn::sound_item sound_item(uint16_t handle)
{
    auto it = data.sound_map.find(handle);
    BN_BASIC_ASSERT(it != data.sound_map.end(), "Sound is not active: ", handle);

    return bn::sound_item(it->second.item_id);
}

fixed sound_speed(uint16_t handle)
{
    auto it = data.sound_map.find(handle);
    BN_BASIC_ASSERT(it != data.sound_map.end(), "Sound is not active: ", handle);

    return it->second.speed;
}

void set_sound_speed(uint16_t handle, fixed speed)
{
    auto it = data.sound_map.find(handle);
    BN_BASIC_ASSERT(it != data.sound_map.end(), "Sound is not active: ", handle);

    sound_data_type& sound_data = it->second;

    if(speed != sound_data.speed)
    {
        fixed_t<10> current_speed = bn::max(fixed_t<10>(sound_data.speed), fixed_t<10>::from_data(1));
        fixed_t<10> scale = fixed_t<10>(speed).unsafe_division(current_speed);

        if(scale != 1)
        {
            int hw_scale = scale.data();
            BN_BASIC_ASSERT(hw_scale < 65536, "Speed change is too high: ", sound_data.speed, " - ", speed);

            int commands = data.commands_count;
            BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

            data.command_codes[commands] = SOUND_SET_SPEED;
            new(data.command_datas + commands) set_sound_speed_command(handle, hw_scale);
            data.commands_count = commands + 1;
        }

        sound_data.speed = speed;
    }
}

fixed sound_panning(uint16_t handle)
{
    auto it = data.sound_map.find(handle);
    BN_BASIC_ASSERT(it != data.sound_map.end(), "Sound is not active: ", handle);

    return it->second.panning;
}

void set_sound_panning(uint16_t handle, fixed panning)
{
    auto it = data.sound_map.find(handle);
    BN_BASIC_ASSERT(it != data.sound_map.end(), "Sound is not active: ", handle);

    sound_data_type& sound_data = it->second;
    int hw_panning = _hw_sound_panning(panning);

    if(hw_panning != _hw_sound_panning(sound_data.panning))
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = SOUND_SET_PANNING;
        new(data.command_datas + commands) set_sound_panning_command(handle, hw_panning);
        data.commands_count = commands + 1;
    }

    sound_data.panning = panning;
}

void stop_all_sounds()
{
    int commands = data.commands_count;
    BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = SOUND_STOP_ALL;
    data.commands_count = commands + 1;

    data.sound_map.clear();
}

fixed sound_master_volume()
{
    return data.sound_master_volume;
}

void set_sound_master_volume(fixed volume)
{
    int hw_volume = _hw_sound_master_volume(volume);

    if(hw_volume != _hw_sound_master_volume(data.sound_master_volume))
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = SOUND_SET_MASTER_VOLUME;
        new(data.command_datas + commands) set_sound_master_volume_command(hw_volume);
        data.commands_count = commands + 1;
    }

    data.sound_master_volume = volume;
}

bool update_on_vblank()
{
    return hw::audio::update_on_vblank();
}

void set_update_on_vblank(bool update_on_vblank)
{
    hw::audio::set_update_on_vblank(update_on_vblank);
}

void update()
{
    hw::audio::update();
}

void execute_commands()
{
    const uint8_t* old_dmg_music_data = data.dmg_music_data;
    const uint8_t* dmg_music_data = old_dmg_music_data;
    bool dmg_music_paused = data.dmg_music_paused;
    bool music_playing = data.music_playing;
    bool music_paused = data.music_paused;
    bool jingle_playing = data.jingle_playing;

    if(music_playing && ! hw::audio::music_playing())
    {
        music_playing = false;
        music_paused = false;
    }

    if(jingle_playing && ! hw::audio::jingle_playing())
    {
        jingle_playing = false;
    }

    if(dmg_music_data && ! hw::audio::dmg_music_playing())
    {
        dmg_music_data = nullptr;
        dmg_music_paused = false;
    }

    hw::audio::update_sounds_queue();

    for(int index = 0, limit = data.commands_count; index < limit; ++index)
    {
        switch(data.command_codes[index])
        {

        case MUSIC_PLAY:
            reinterpret_cast<const play_music_command&>(data.command_datas[index].data).execute();
            music_playing = true;
            music_paused = false;
            break;

        case MUSIC_STOP:
            hw::audio::stop_music();
            music_playing = false;
            music_paused = false;
            break;

        case MUSIC_PAUSE:
            if(music_playing)
            {
                hw::audio::pause_music();
                music_paused = true;
            }
            break;

        case MUSIC_RESUME:
            if(music_playing)
            {
                hw::audio::resume_music();
                music_paused = false;
            }
            break;

        case MUSIC_SET_POSITION:
            if(music_playing)
            {
                reinterpret_cast<const set_music_position_command&>(data.command_datas[index].data).execute();
            }
            break;

        case MUSIC_SET_VOLUME:
            if(music_playing)
            {
                reinterpret_cast<const set_music_volume_command&>(data.command_datas[index].data).execute();
            }
            break;

        case MUSIC_SET_TEMPO:
            if(music_playing)
            {
                reinterpret_cast<const set_music_tempo_command&>(data.command_datas[index].data).execute();
            }
            break;

        case MUSIC_SET_PITCH:
            if(music_playing)
            {
                reinterpret_cast<const set_music_pitch_command&>(data.command_datas[index].data).execute();
            }
            break;

        case JINGLE_PLAY:
            reinterpret_cast<const play_jingle_command&>(data.command_datas[index].data).execute();
            jingle_playing = true;
            break;

        case JINGLE_SET_VOLUME:
            if(jingle_playing)
            {
                reinterpret_cast<const set_jingle_volume_command&>(data.command_datas[index].data).execute();
            }
            break;

        case DMG_MUSIC_PLAY:
            reinterpret_cast<const play_dmg_music_command&>(data.command_datas[index].data).execute();
            dmg_music_data = old_dmg_music_data;
            dmg_music_paused = false;
            break;

        case DMG_MUSIC_STOP:
            hw::audio::stop_dmg_music();
            dmg_music_data = nullptr;
            dmg_music_paused = false;
            break;

        case DMG_MUSIC_PAUSE:
            if(dmg_music_data)
            {
                hw::audio::pause_dmg_music();
                dmg_music_paused = true;
            }
            break;

        case DMG_MUSIC_RESUME:
            if(dmg_music_data)
            {
                hw::audio::resume_dmg_music();
                dmg_music_paused = false;
            }
            break;

        case DMG_MUSIC_SET_POSITION:
            if(dmg_music_data)
            {
                reinterpret_cast<const set_dmg_music_position_command&>(data.command_datas[index].data).execute();
            }
            break;

        case DMG_MUSIC_SET_VOLUME:
            if(dmg_music_data)
            {
                reinterpret_cast<const set_dmg_music_volume_command&>(data.command_datas[index].data).execute();
            }
            break;

        case DMG_MUSIC_SET_MASTER_VOLUME:
            reinterpret_cast<const set_dmg_music_master_volume_command&>(data.command_datas[index].data).execute();
            break;

        case SOUND_PLAY:
            reinterpret_cast<const play_sound_command&>(data.command_datas[index].data).execute();
            break;

        case SOUND_PLAY_EX:
            reinterpret_cast<const play_sound_ex_command&>(data.command_datas[index].data).execute();
            break;

        case SOUND_STOP:
            reinterpret_cast<const stop_sound_command&>(data.command_datas[index].data).execute();
            break;

        case SOUND_RELEASE:
            reinterpret_cast<const release_sound_command&>(data.command_datas[index].data).execute();
            break;

        case SOUND_SET_SPEED:
            reinterpret_cast<const set_sound_speed_command&>(data.command_datas[index].data).execute();
            break;

        case SOUND_SET_PANNING:
            reinterpret_cast<const set_sound_panning_command&>(data.command_datas[index].data).execute();
            break;

        case SOUND_STOP_ALL:
            hw::audio::stop_all_sounds();
            break;

        case SOUND_SET_MASTER_VOLUME:
            reinterpret_cast<const set_sound_master_volume_command&>(data.command_datas[index].data).execute();
            break;

        default:
            break;
        }
    }

    data.commands_count = 0;

    if(music_playing)
    {
        data.music_position = hw::audio::music_position();
    }

    if(dmg_music_data)
    {
        int pattern;
        int row;
        hw::audio::dmg_music_position(pattern, row);
        data.dmg_music_position = bn::dmg_music_position(pattern, row);
    }

    data.music_playing = music_playing;
    data.music_paused = music_paused;
    data.jingle_playing = jingle_playing;
    data.dmg_music_data = dmg_music_data;
    data.dmg_music_paused = dmg_music_paused;

    for(auto it = data.sound_map.begin(), end = data.sound_map.end(); it != end; )
    {
        const mm_sfxhand* hw_handle = it->second.hw_handle.get();

        if(hw_handle && hw::audio::sound_active(*hw_handle))
        {
            ++it;
        }
        else
        {
            it = data.sound_map.erase(it);
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
    data.sound_map.clear();

    hw::audio::stop();
}

}
