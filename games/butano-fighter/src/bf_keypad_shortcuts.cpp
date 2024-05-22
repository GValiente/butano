/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_keypad_shortcuts.h"

#include "bn_core.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_palettes.h"
#include "bn_sprite_items_brightness.h"

namespace bf
{

namespace
{
    constexpr bn::fixed max_brightness(0.4);

    [[nodiscard]] bn::sprite_ptr _create_brightness_sprite()
    {
        bn::sprite_builder builder(bn::sprite_items::brightness);
        builder.set_position(0, -60);
        builder.set_bg_priority(0);
        builder.set_visible(false);
        return builder.release_build();
    }
}

keypad_shortcuts::keypad_shortcuts() :
    _brightness_sprite(_create_brightness_sprite())
{
}

void keypad_shortcuts::update()
{
    bool b_held = bn::keypad::b_held();
    bool a_held = bn::keypad::a_held();
    bool l_held = bn::keypad::l_held();
    bool r_held = bn::keypad::r_held();
    bool select_held = bn::keypad::select_held();
    bool start_held = bn::keypad::start_held();
    bn::fixed new_brightness = -1;

    if(l_held)
    {
        if(! select_held || ! r_held)
        {
            new_brightness = bn::max(bn::bg_palettes::brightness() - bn::fixed(0.005), bn::fixed(0));
            bn::bg_palettes::set_brightness(new_brightness);
            bn::sprite_palettes::set_brightness(new_brightness);
        }
    }

    if(r_held)
    {
        if(! select_held || ! l_held)
        {
            new_brightness = bn::min(bn::bg_palettes::brightness() + bn::fixed(0.005), max_brightness);
            bn::bg_palettes::set_brightness(new_brightness);
            bn::sprite_palettes::set_brightness(new_brightness);
        }
    }

    if(new_brightness >= 0)
    {
        int graphics_index = ((new_brightness * 8) / max_brightness).right_shift_integer();
        _brightness_sprite.set_tiles(bn::sprite_items::brightness.tiles_item(), graphics_index);
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
            const bn::keypad::key_type wake_up_keys[] = {
                bn::keypad::key_type::SELECT,
                bn::keypad::key_type::L,
                bn::keypad::key_type::R
            };

            _sleep_ready = false;
            bn::core::sleep(wake_up_keys);
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
            bn::core::reset();
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
