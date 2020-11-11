/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_keypad_manager.h"

#include "btn_string_view.h"
#include "btn_config_keypad.h"
#include "../hw/include/btn_hw_keypad.h"

#include "btn_keypad.cpp.h"

#if BTN_CFG_KEYPAD_LOG_ENABLED
    #include "btn_log.h"
    #include "btn_string.h"

    static_assert(BTN_CFG_LOG_ENABLED, "Log is not enabled");
#endif

namespace btn::keypad_manager
{

namespace
{
    #if BTN_CFG_KEYPAD_LOG_ENABLED
        class keypad_logger
        {

        public:
            keypad_logger()
            {
                BTN_LOG("-- KEYPAD LOGGER INIT ---");
            }

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
                    BTN_LOG(_buffer);
                    _buffer.clear();
                }
            }

        private:
            string<BTN_CFG_LOG_MAX_SIZE - 8> _buffer;
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

        #if BTN_CFG_KEYPAD_LOG_ENABLED
            keypad_logger logger;
        #endif
    };

    BTN_DATA_EWRAM static_data data;
}

void init(const string_view& commands)
{
    BTN_ASSERT(commands.empty() || commands.size() % 2 == 0, "Invalid commands size: ", commands.size());

    data.commands = commands;
    data.read_commands = ! commands.empty();
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

void update()
{
    unsigned previous_keys = data.held_keys;
    unsigned current_keys;

    if(data.read_commands)
    {
        if(data.commands.empty())
        {
            current_keys = 0;
        }
        else
        {
            uint8_t low_part = data.commands[0] - '0';
            uint8_t high_part = data.commands[1] - '0';
            current_keys = (high_part << 5) + low_part;
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

    #if BTN_CFG_KEYPAD_LOG_ENABLED
        data.logger.log(current_keys);
    #endif
}

void set_interrupt(const span<const key_type>& keys)
{
    BTN_ASSERT(! keys.empty(), "There's no keys");

    hw::keypad::set_interrupt(keys);
}

void stop()
{
    #if BTN_CFG_KEYPAD_LOG_ENABLED
        data.logger.flush();
    #endif
}

}
