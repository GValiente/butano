#include "btn_keypad_manager.h"

#include "../hw/include/btn_hw_keypad.h"

namespace btn::keypad_manager
{

namespace
{
    class static_data
    {

    public:
        unsigned held_keys = 0;
        unsigned pressed_keys = 0;
        unsigned released_keys = 0;
    };

    BTN_DATA_EWRAM static_data data;
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
    unsigned current_keys = hw::keypad::get();
    data.held_keys = current_keys;
    data.pressed_keys = current_keys & ~previous_keys;
    data.released_keys = ~current_keys & previous_keys;
}

void set_interrupt(const span<const key_type>& keys)
{
    BTN_ASSERT(! keys.empty(), "There's no keys");

    hw::keypad::set_interrupt(keys);
}

}
