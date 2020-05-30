#include "bf_keypad_shortcuts.h"

#include "btn_span.h"
#include "btn_core.h"
#include "btn_fixed.h"
#include "btn_keypad.h"
#include "btn_bg_palettes.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_palettes.h"
#include "btn_sprite_items_brightness.h"

namespace bf
{

namespace
{
    constexpr const btn::fixed max_brightness(0.4);

    [[nodiscard]] btn::sprite_ptr _create_brightness_sprite()
    {
        btn::sprite_builder builder(btn::sprite_items::brightness);
        builder.set_position(0, -60);
        builder.set_bg_priority(0);
        builder.set_visible(false);
        builder.set_ignore_camera(true);
        return builder.release_build();
    }
}

keypad_shortcuts::keypad_shortcuts() :
    _brightness_sprite(_create_brightness_sprite())
{
}

void keypad_shortcuts::update()
{
    bool b_held = btn::keypad::b_held();
    bool a_held = btn::keypad::a_held();
    bool l_held = btn::keypad::l_held();
    bool r_held = btn::keypad::r_held();
    bool select_held = btn::keypad::select_held();
    bool start_held = btn::keypad::start_held();
    btn::fixed new_brightness = -1;

    if(l_held)
    {
        if(! select_held || ! r_held)
        {
            new_brightness = btn::max(btn::bg_palettes::brightness() - btn::fixed(0.005), btn::fixed(0));
            btn::bg_palettes::set_brightness(new_brightness);
            btn::sprite_palettes::set_brightness(new_brightness);
        }
    }

    if(r_held)
    {
        if(! select_held || ! l_held)
        {
            new_brightness = btn::min(btn::bg_palettes::brightness() + btn::fixed(0.005), max_brightness);
            btn::bg_palettes::set_brightness(new_brightness);
            btn::sprite_palettes::set_brightness(new_brightness);
        }
    }

    if(new_brightness >= 0)
    {
        int graphics_index = ((new_brightness * 8) / max_brightness).integer();
        _brightness_sprite.set_tiles(btn::sprite_items::brightness.tiles_item(), graphics_index);
        _brightness_sprite.set_visible(true);
        _brightness_counter = 60;
    }
    else
    {
        if(_brightness_counter)
        {
            --_brightness_counter;

            if(! _brightness_counter)
            {
                _brightness_sprite.set_visible(false);
            }
        }
    }

    if(_sleep_ready)
    {
        if(select_held && l_held && r_held)
        {
            const btn::keypad::key_type wake_up_keys[] = {
                btn::keypad::key_type::SELECT,
                btn::keypad::key_type::L,
                btn::keypad::key_type::R
            };

            _sleep_ready = false;
            btn::core::sleep(wake_up_keys);
        }
    }
    else
    {
        if(! select_held || ! l_held || ! r_held)
        {
            _sleep_ready = true;
        }
    }

    if(_reset_ready)
    {
        if(select_held && start_held && b_held && a_held)
        {
            btn::core::reset();
        }
    }
    else
    {
        if(! select_held || ! start_held || ! b_held || ! a_held)
        {
            _reset_ready = true;
        }
    }
}

}
