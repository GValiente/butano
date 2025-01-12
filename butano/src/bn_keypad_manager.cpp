/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_keypad_manager.h"

#include "bn_config_keypad.h"
#include "../hw/include/bn_hw_keypad.h"

#include "bn_keypad.cpp.h"

#if BN_CFG_KEYPAD_LOG_ENABLED
    #include "bn_log.h"
    #include "bn_string.h"

    static_assert(BN_CFG_LOG_ENABLED, "Log is not enabled");
#endif

namespace bn::keypad_manager
{

namespace
{
    #if BN_CFG_KEYPAD_LOG_ENABLED
        class keypad_logger
        {

        public:
            void log(unsigned keys)
            {
                uint8_t low_part = keys & 0b11111;
                uint8_t high_part = (keys & 0b1111100000) >> 5;
                _buffer.append(char(low_part) + '0');
                _buffer.append(char(high_part) + '0');

                if(_buffer.available() < 2)
                {
                    flush();
                }
            }

            void flush()
            {
                if(! _buffer.empty())
                {
                    BN_LOG(_buffer);
                    _buffer.clear();
                }
            }

        private:
            string<min(BN_CFG_LOG_MAX_SIZE - 8, 128)> _buffer;
        };
    #endif

    class static_data
    {

    public:
        string_view commands;
        unsigned held_keys = 0;
        unsigned pressed_keys = 0;
        unsigned released_keys = 0;
        bool read_commands = false;

        #if BN_CFG_KEYPAD_LOG_ENABLED
            keypad_logger logger;
        #endif
    };

    BN_DATA_EWRAM_BSS static_data data;
}

void init(const string_view& commands)
{
    ::new(static_cast<void*>(&data)) static_data();

    BN_ASSERT(commands.size() % 2 == 0, "Invalid commands size: ", commands.size());

    data.commands = commands;
    data.read_commands = ! commands.empty();

    #if BN_CFG_KEYPAD_LOG_ENABLED
        BN_LOG("- KEYPAD LOGGER INIT -");
    #endif
}

bool held(key_type key)
{
    return data.held_keys & unsigned(key);
}

bool pressed(key_type key)
{
    return data.pressed_keys & unsigned(key);
}

bool released(key_type key)
{
    return data.released_keys & unsigned(key);
}

bool any_held()
{
    return data.held_keys;
}

bool any_pressed()
{
    return data.pressed_keys;
}

bool any_released()
{
    return data.released_keys;
}

void update()
{
    unsigned previous_keys = data.held_keys;
    unsigned current_keys;

    if(data.read_commands) [[unlikely]]
    {
        if(data.commands.empty()) [[unlikely]]
        {
            current_keys = hw::keypad::get();
            data.read_commands = false;
        }
        else
        {
            uint8_t low_part = data.commands[0] - '0';
            uint8_t high_part = data.commands[1] - '0';
            current_keys = (unsigned(high_part) << 5) + low_part;
            data.commands.remove_prefix(2);
        }
    }
    else
    {
        current_keys = hw::keypad::get();
    }

    data.held_keys = current_keys;
    data.pressed_keys = current_keys & ~previous_keys;
    data.released_keys = ~current_keys & previous_keys;

    #if BN_CFG_KEYPAD_LOG_ENABLED
        data.logger.log(current_keys);
    #endif
}

void set_interrupt(const span<const key_type>& keys)
{
    BN_BASIC_ASSERT(! keys.empty(), "There are no keys");

    hw::keypad::set_interrupt(keys);
}

void stop()
{
    #if BN_CFG_KEYPAD_LOG_ENABLED
        data.logger.flush();
    #endif
}

}
