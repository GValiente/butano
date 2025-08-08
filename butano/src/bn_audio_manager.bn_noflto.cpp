/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_audio_manager.h"

#include "bn_config_audio.h"
#include "bn_unordered_map.h"
#include "bn_identity_hasher.h"
#include "bn_dmg_music_position.h"
#include "../hw/include/bn_hw_audio.h"
#include "../hw/include/bn_hw_dmg_audio.h"

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
    optional<uint16_t> hw_handle;

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


    class set_audio_mixing_rate
    {

    public:
        explicit set_audio_mixing_rate(audio_mixing_rate mixing_rate) :
            _mixing_rate(int(mixing_rate))
        {
        }

        void execute() const
        {
            hw::audio::set_mixing_rate(audio_mixing_rate(_mixing_rate));
        }

    private:
        int _mixing_rate;
    };


    class play_music_command
    {

    public:
        play_music_command(int id, bool loop, fixed volume) :
            _id(id),
            _volume(volume),
            _loop(loop)
        {
        }

        void execute() const
        {
            hw::audio::play_music(_id, _loop);
            hw::audio::set_music_volume(_volume);
            hw::audio::set_music_tempo(1);
            hw::audio::set_music_pitch(1);
        }

    private:
        int _id;
        fixed _volume;
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
        explicit set_music_volume_command(fixed volume) :
            _volume(volume)
        {
        }

        void execute() const
        {
            hw::audio::set_music_volume(_volume);
        }

    private:
        fixed _volume;
    };


    class set_music_tempo_command
    {

    public:
        explicit set_music_tempo_command(fixed tempo) :
            _tempo(tempo)
        {
        }

        void execute() const
        {
            hw::audio::set_music_tempo(_tempo);
        }

    private:
        fixed _tempo;
    };


    class set_music_pitch_command
    {

    public:
        explicit set_music_pitch_command(fixed pitch) :
            _pitch(pitch)
        {
        }

        void execute() const
        {
            hw::audio::set_music_pitch(_pitch);
        }

    private:
        fixed _pitch;
    };


    class play_jingle_command
    {

    public:
        play_jingle_command(int id, fixed volume) :
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
        fixed _volume;
    };


    class set_jingle_volume_command
    {

    public:
        explicit set_jingle_volume_command(fixed volume) :
            _volume(volume)
        {
        }

        void execute() const
        {
            hw::audio::set_jingle_volume(_volume);
        }

    private:
        fixed _volume;
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
            hw::dmg_audio::play_music(_song, _type, _speed, _loop);
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
            hw::dmg_audio::set_music_position(_pattern, _row);
        }

    private:
        int _pattern;
        int _row;
    };


    class set_dmg_music_volume_command
    {

    public:
        set_dmg_music_volume_command(fixed left_volume, fixed right_volume) :
            _left_volume(left_volume),
            _right_volume(right_volume)
        {
        }

        void execute() const
        {
            hw::dmg_audio::set_music_volume(_left_volume, _right_volume);
        }

    private:
        fixed _left_volume;
        fixed _right_volume;
    };


    class set_dmg_music_master_volume_command
    {

    public:
        explicit set_dmg_music_master_volume_command(bn::dmg_music_master_volume volume) :
            _volume(int(volume))
        {
        }

        void execute() const
        {
            hw::dmg_audio::set_music_master_volume(bn::dmg_music_master_volume(_volume));
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
        play_sound_ex_command(int priority, int id, uint16_t handle, fixed volume, fixed speed, fixed panning) :
            _id(id),
            _priority(int16_t(priority)),
            _handle(handle),
            _speed(speed),
            _volume(volume),
            _panning(panning)
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
        fixed _speed;
        fixed _volume;
        fixed _panning;
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
                if(const uint16_t* hw_handle = data->hw_handle.get())
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
                if(const uint16_t* hw_handle = data->hw_handle.get())
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
        set_sound_speed_command(uint16_t handle, fixed current_speed, fixed new_speed) :
            _current_speed(current_speed),
            _new_speed(new_speed),
            _handle(handle)
        {
        }

        void execute() const
        {
            if(sound_data_type* data = sound_data(_handle))
            {
                if(const uint16_t* hw_handle = data->hw_handle.get())
                {
                    hw::audio::set_sound_speed(*hw_handle, _current_speed, _new_speed);
                }
            }
        }

    private:
        fixed _current_speed;
        fixed _new_speed;
        uint16_t _handle;
    };


    class set_sound_panning_command
    {

    public:
        set_sound_panning_command(uint16_t handle, fixed panning) :
            _panning(panning),
            _handle(handle)
        {
        }

        void execute() const
        {
            if(sound_data_type* data = sound_data(_handle))
            {
                if(const uint16_t* hw_handle = data->hw_handle.get())
                {
                    hw::audio::set_sound_panning(*hw_handle, _panning);
                }
            }
        }

    private:
        fixed _panning;
        uint16_t _handle;
    };


    class set_sound_master_volume_command
    {

    public:
        explicit set_sound_master_volume_command(fixed volume) :
            _volume(volume)
        {
        }

        void execute() const
        {
            hw::audio::set_sound_master_volume(_volume);
        }

    private:
        fixed _volume;
    };


    enum command_code : uint8_t
    {
        AUDIO_SET_MIXING_RATE,
        MUSIC_PLAY,
        MUSIC_STOP,
        MUSIC_PAUSE,
        MUSIC_RESUME,
        MUSIC_SET_POSITION,
        MUSIC_SET_VOLUME,
        MUSIC_SET_TEMPO,
        MUSIC_SET_PITCH,
        JINGLE_PLAY,
        JINGLE_STOP,
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
        int data[5];
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
        audio_mixing_rate mixing_rate = audio_mixing_rate(BN_CFG_AUDIO_MIXING_RATE);
        bool music_playing = false;
        bool music_paused = false;
        bool jingle_playing = false;
        bool dmg_music_paused = false;
        bool update_on_vblank = false;
        bool event_handler_enabled = false;
        bool delay_commit = true;
    };

    BN_DATA_EWRAM_BSS static_data data;
}

void init()
{
    ::new(static_cast<void*>(&data)) static_data();

    hw::dmg_audio::init();
    hw::audio::init();
}

void enable()
{
    hw::audio::enable();
    hw::dmg_audio::enable();
}

void disable()
{
    hw::audio::disable();
    hw::dmg_audio::disable();
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
    ::new(static_cast<void*>(data.command_datas + commands)) play_music_command(item.id(), loop, volume);
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
        data.music_playing = false;
        data.music_paused = false;

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = MUSIC_STOP;
        data.commands_count = commands + 1;
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
        data.music_position = position;

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = MUSIC_SET_POSITION;
        ::new(static_cast<void*>(data.command_datas + commands)) set_music_position_command(position);
        data.commands_count = commands + 1;
    }
}

fixed music_volume()
{
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");

    return data.music_volume;
}

void set_music_volume(fixed volume)
{
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");

    if(volume != data.music_volume)
    {
        data.music_volume = volume;

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = MUSIC_SET_VOLUME;
        ::new(static_cast<void*>(data.command_datas + commands)) set_music_volume_command(volume);
        data.commands_count = commands + 1;
    }
}

fixed music_tempo()
{
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");

    return data.music_tempo;
}

void set_music_tempo(fixed tempo)
{
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");

    if(tempo != data.music_tempo)
    {
        data.music_tempo = tempo;

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = MUSIC_SET_TEMPO;
        ::new(static_cast<void*>(data.command_datas + commands)) set_music_tempo_command(tempo);
        data.commands_count = commands + 1;
    }
}

fixed music_pitch()
{
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");

    return data.music_pitch;
}

void set_music_pitch(fixed pitch)
{
    BN_BASIC_ASSERT(data.music_playing, "There's no music playing");

    if(pitch != data.music_pitch)
    {
        data.music_pitch = pitch;

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = MUSIC_SET_PITCH;
        ::new(static_cast<void*>(data.command_datas + commands)) set_music_pitch_command(pitch);
        data.commands_count = commands + 1;
    }
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
    ::new(static_cast<void*>(data.command_datas + commands)) play_jingle_command(item.id(), volume);
    data.commands_count = commands + 1;

    data.jingle_item_id = item.id();
    data.jingle_volume = volume;
    data.jingle_playing = true;
}

void stop_jingle()
{
    if(data.jingle_playing)
    {
        data.jingle_playing = false;

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = JINGLE_STOP;
        data.commands_count = commands + 1;
    }
}

fixed jingle_volume()
{
    BN_BASIC_ASSERT(data.jingle_playing, "There's no jingle playing");

    return data.jingle_volume;
}

void set_jingle_volume(fixed volume)
{
    BN_BASIC_ASSERT(data.jingle_playing, "There's no jingle playing");

    if(volume != data.jingle_volume)
    {
        data.jingle_volume = volume;

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = JINGLE_SET_VOLUME;
        ::new(static_cast<void*>(data.command_datas + commands)) set_jingle_volume_command(volume);
        data.commands_count = commands + 1;
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
        result = dmg_music_item(*dmg_music_data, data.dmg_music_type);
    }

    return result;
}

void play_dmg_music(const dmg_music_item& item, int speed, bool loop)
{
    int commands = data.commands_count;
    BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

    data.command_codes[commands] = DMG_MUSIC_PLAY;
    ::new(static_cast<void*>(data.command_datas + commands)) play_dmg_music_command(
            item.data_ptr(), item.type(), loop, speed);
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
        data.dmg_music_data = nullptr;
        data.dmg_music_paused = false;

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = DMG_MUSIC_STOP;
        data.commands_count = commands + 1;
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
        data.dmg_music_position = position;

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = DMG_MUSIC_SET_POSITION;
        ::new(static_cast<void*>(data.command_datas + commands)) set_dmg_music_position_command(
                position.pattern(), position.row());
        data.commands_count = commands + 1;
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
    BN_BASIC_ASSERT(data.dmg_music_data, "There's no DMG music playing");

    if(left_volume != data.dmg_music_left_volume || right_volume != data.dmg_music_right_volume)
    {
        data.dmg_music_left_volume = left_volume;
        data.dmg_music_right_volume = right_volume;

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = DMG_MUSIC_SET_VOLUME;
        ::new(static_cast<void*>(data.command_datas + commands)) set_dmg_music_volume_command(left_volume, right_volume);
        data.commands_count = commands + 1;
    }
}

bn::dmg_music_master_volume dmg_music_master_volume()
{
    return data.dmg_music_master_volume;
}

void set_dmg_music_master_volume(bn::dmg_music_master_volume volume)
{
    if(volume != data.dmg_music_master_volume)
    {
        data.dmg_music_master_volume = volume;

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = DMG_MUSIC_SET_MASTER_VOLUME;
        ::new(static_cast<void*>(data.command_datas + commands)) set_dmg_music_master_volume_command(volume);
        data.commands_count = commands + 1;
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
    ::new(static_cast<void*>(data.command_datas + commands)) play_sound_command(priority, item.id(), handle);
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
    ::new(static_cast<void*>(data.command_datas + commands)) play_sound_ex_command(
            priority, item.id(), handle, volume, speed, panning);
    data.commands_count = commands + 1;

    return handle;
}

void stop_sound(uint16_t handle)
{
    if(sound_data(handle))
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = SOUND_STOP;
        ::new(static_cast<void*>(data.command_datas + commands)) stop_sound_command(handle);
        data.commands_count = commands + 1;
    }
}

void release_sound(uint16_t handle)
{
    if(sound_data(handle))
    {
        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = SOUND_RELEASE;
        ::new(static_cast<void*>(data.command_datas + commands)) release_sound_command(handle);
        data.commands_count = commands + 1;
    }
}

bn::sound_item sound_item(uint16_t handle)
{
    sound_data_type* handle_sound_data = sound_data(handle);
    BN_BASIC_ASSERT(handle_sound_data, "Sound is not active: ", handle);

    return bn::sound_item(handle_sound_data->item_id);
}

fixed sound_speed(uint16_t handle)
{
    sound_data_type* handle_sound_data = sound_data(handle);
    BN_BASIC_ASSERT(handle_sound_data, "Sound is not active: ", handle);

    return handle_sound_data->speed;
}

void set_sound_speed(uint16_t handle, fixed speed)
{
    sound_data_type* handle_sound_data = sound_data(handle);
    BN_BASIC_ASSERT(handle_sound_data, "Sound is not active: ", handle);

    fixed handle_speed = handle_sound_data->speed;

    if(speed != handle_speed)
    {
        handle_sound_data->speed = speed;

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = SOUND_SET_SPEED;
        ::new(static_cast<void*>(data.command_datas + commands)) set_sound_speed_command(handle, handle_speed, speed);
        data.commands_count = commands + 1;
    }
}

fixed sound_panning(uint16_t handle)
{
    sound_data_type* handle_sound_data = sound_data(handle);
    BN_BASIC_ASSERT(handle_sound_data, "Sound is not active: ", handle);

    return handle_sound_data->panning;
}

void set_sound_panning(uint16_t handle, fixed panning)
{
    sound_data_type* handle_sound_data = sound_data(handle);
    BN_BASIC_ASSERT(handle_sound_data, "Sound is not active: ", handle);

    if(panning != handle_sound_data->panning)
    {
        handle_sound_data->panning = panning;

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = SOUND_SET_PANNING;
        ::new(static_cast<void*>(data.command_datas + commands)) set_sound_panning_command(handle, panning);
        data.commands_count = commands + 1;
    }
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
    if(volume != data.sound_master_volume)
    {
        data.sound_master_volume = volume;

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = SOUND_SET_MASTER_VOLUME;
        ::new(static_cast<void*>(data.command_datas + commands)) set_sound_master_volume_command(volume);
        data.commands_count = commands + 1;
    }
}

span<const audio_mixing_rate> available_mixing_rates()
{
    return hw::audio::available_mixing_rates();
}

audio_mixing_rate mixing_rate()
{
    return data.mixing_rate;
}

void set_mixing_rate(audio_mixing_rate mixing_rate)
{
    if(mixing_rate != data.mixing_rate)
    {
        data.mixing_rate = mixing_rate;

        BN_BASIC_ASSERT(! data.music_playing, "There's music playing");
        BN_BASIC_ASSERT(! data.jingle_playing, "There's a jingle playing");
        BN_BASIC_ASSERT(data.sound_map.empty(), "There are sounds playing");

        int commands = data.commands_count;
        BN_BASIC_ASSERT(commands < max_commands, "No more audio commands available");

        data.command_codes[commands] = AUDIO_SET_MIXING_RATE;
        ::new(static_cast<void*>(data.command_datas + commands)) set_audio_mixing_rate(mixing_rate);
        data.commands_count = commands + 1;
    }
}

bool update_on_vblank()
{
    return data.update_on_vblank;
}

void set_update_on_vblank(bool update_on_vblank)
{
    data.update_on_vblank = update_on_vblank;
}

bool event_handler_enabled()
{
    return data.event_handler_enabled;
}

void set_event_handler_enabled(bool enabled)
{
    data.event_handler_enabled = enabled;
}

span<uint8_t> event_ids()
{
    BN_BASIC_ASSERT(data.event_handler_enabled, "Event handler is disabled");

    return hw::audio::event_ids();
}

void update()
{
    data.delay_commit = ! data.update_on_vblank;
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

    if(dmg_music_data && ! hw::dmg_audio::music_playing())
    {
        dmg_music_data = nullptr;
        dmg_music_paused = false;
    }

    hw::audio::update_sounds_queue();

    for(int index = 0, limit = data.commands_count; index < limit; ++index)
    {
        switch(data.command_codes[index])
        {

        case AUDIO_SET_MIXING_RATE:
            reinterpret_cast<const set_audio_mixing_rate&>(data.command_datas[index].data).execute();
            break;

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

        case JINGLE_STOP:
            hw::audio::stop_jingle();
            jingle_playing = false;
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
            hw::dmg_audio::stop_music();
            dmg_music_data = nullptr;
            dmg_music_paused = false;
            break;

        case DMG_MUSIC_PAUSE:
            if(dmg_music_data)
            {
                hw::dmg_audio::pause_music();
                dmg_music_paused = true;
            }
            break;

        case DMG_MUSIC_RESUME:
            if(dmg_music_data)
            {
                hw::dmg_audio::resume_music();
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
        hw::dmg_audio::music_position(pattern, row);
        data.dmg_music_position = bn::dmg_music_position(pattern, row);
    }

    data.music_playing = music_playing;
    data.music_paused = music_paused;
    data.jingle_playing = jingle_playing;
    data.dmg_music_data = dmg_music_data;
    data.dmg_music_paused = dmg_music_paused;

    for(auto it = data.sound_map.begin(), end = data.sound_map.end(); it != end; )
    {
        const uint16_t* hw_handle = it->second.hw_handle.get();

        if(hw_handle && hw::audio::sound_active(*hw_handle))
        {
            ++it;
        }
        else
        {
            it = data.sound_map.erase(it);
        }
    }

    hw::audio::update_events(data.event_handler_enabled);
}

void vblank_commit()
{
    if(! data.delay_commit)
    {
        hw::audio::commit();
        hw::dmg_audio::commit();
    }
}

void delayed_commit()
{
    if(data.delay_commit)
    {
        hw::audio::commit();
        hw::dmg_audio::commit();
        hw::dmg_audio::check_commit_result();
        data.delay_commit = false;
    }
}

void stop()
{
    data.commands_count = 0;
    data.sound_map.clear();

    hw::dmg_audio::stop();
    hw::audio::stop();
}

}
