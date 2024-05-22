/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "common_info.h"

#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_sprite_text_generator.h"

namespace common
{

namespace
{
    constexpr bn::fixed y_inc = 14;
    constexpr bn::fixed start_y = (-bn::display::height() / 2) + y_inc + 1;
}

info::info(const bn::span<const bn::string_view>& text_lines, bn::sprite_text_generator& text_generator) :
    info("", text_lines, text_generator)
{
}

info::info(const bn::string_view& title, const bn::span<const bn::string_view>& text_lines,
           bn::sprite_text_generator& text_generator)
{
    bn::fixed y = start_y;
    text_generator.set_bg_priority(0);
    text_generator.set_alignment(bn::sprite_text_generator::alignment_type::CENTER);

    if(! title.empty())
    {
        text_generator.generate(0, y, title, _title_sprites);
        y += y_inc * 2;
    }

    if(! text_lines.empty())
    {
        bn::fixed info_y = start_y + (y_inc * 9);

        if(text_lines.size() == 1)
        {
            text_generator.generate(0, info_y, text_lines[0], _text_sprites);
            _enabled = true;
            _show_always = true;
        }
        else
        {
            for(const bn::string_view& text_line : text_lines)
            {
                text_generator.generate(0, y, text_line, _text_sprites);
                y += y_inc;
            }

            text_generator.generate(0, info_y, "SELECT: hide info", _hide_info_sprites);
            text_generator.generate(0, info_y, "SELECT: show info", _show_info_sprites);
        }
    }

    _update_sprites();
}

void info::set_show_always(bool show_always)
{
    _show_always = show_always;

    if(show_always && ! _enabled)
    {
        _enabled = true;
        _update_sprites();
    }
}

void info::update()
{
    if(! _show_always && bn::keypad::select_pressed())
    {
        _enabled = ! _enabled;
        _update_sprites();
    }
}

void info::_update_sprites()
{
    bool enabled = _enabled;
    bool show_show_info_sprites;
    bool show_hide_info_sprites;

    if(_show_always)
    {
        show_show_info_sprites = false;
        show_hide_info_sprites = false;
    }
    else
    {
        if(enabled)
        {
            show_show_info_sprites = false;
            show_hide_info_sprites = true;
        }
        else
        {
            show_show_info_sprites = true;
            show_hide_info_sprites = false;
        }
    }

    for(bn::sprite_ptr& sprite : _show_info_sprites)
    {
        sprite.set_visible(show_show_info_sprites);
    }

    for(bn::sprite_ptr& sprite : _hide_info_sprites)
    {
        sprite.set_visible(show_hide_info_sprites);
    }

    for(bn::sprite_ptr& sprite : _text_sprites)
    {
        sprite.set_visible(enabled);
    }
}

}
