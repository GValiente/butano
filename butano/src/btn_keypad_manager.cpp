#include "btn_keypad_manager.h"

#include "../hw/include/btn_hw_keypad.h"

namespace btn::keypad_manager
{

namespace
{
    class static_data
    {

    public:
        unsigned down_keys = 0;
        unsigned up_keys = 0;
        unsigned pressed_keys = 0;
        unsigned released_keys = 0;
    };

    BTN_DATA_EWRAM static_data data;
}

bool down(key_type key)
{
    return data.down_keys & unsigned(key);
}

bool up(key_type key)
{
    return data.up_keys & unsigned(key);
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
    unsigned previous_keys = data.down_keys;
    unsigned current_keys = hw::keypad::get();
    data.down_keys = current_keys;
    data.up_keys = ~current_keys;
    data.pressed_keys = current_keys & ~previous_keys;
    data.released_keys = ~current_keys & previous_keys;
}

void set_interrupt(const span<const key_type>& keys)
{
    BTN_ASSERT(! keys.empty(), "There's no keys");

    hw::keypad::set_interrupt(keys);
}

}
