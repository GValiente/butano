#include "info.h"

#include "btn_keypad.h"
#include "btn_display.h"
#include "btn_sprite_text_generator.h"

info::info(const btn::span<const btn::string_view>& text_lines, btn::sprite_text_generator& text_generator)
{
    btn::fixed y_inc = 12;
    btn::fixed y = (-btn::display::height() / 2) + (y_inc * 3);
    text_generator.set_bg_priority(0);
    text_generator.set_alignment(btn::horizontal_alignment_type::CENTER);
    text_generator.generate(0, (btn::display::height() / 2) - 12, "SELECT: hide info", _hide_info_sprites);
    text_generator.generate(0, (btn::display::height() / 2) - 12, "SELECT: show info", _show_info_sprites);

    for(const btn::string_view& text_line : text_lines)
    {
        text_generator.generate(0, y, text_line, _sprites);
        y += y_inc;
    }

    _update_sprites();
}

void info::update()
{
    if(btn::keypad::select_pressed())
    {
        _enabled = ! _enabled;
        _update_sprites();
    }
}

void info::_update_sprites()
{
    bool enabled = _enabled;

    if(enabled)
    {
        for(btn::sprite_ptr& sprite : _show_info_sprites)
        {
            sprite.set_visible(false);
        }

        for(btn::sprite_ptr& sprite : _hide_info_sprites)
        {
            sprite.set_visible(true);
        }
    }
    else
    {
        for(btn::sprite_ptr& sprite : _hide_info_sprites)
        {
            sprite.set_visible(false);
        }

        for(btn::sprite_ptr& sprite : _show_info_sprites)
        {
            sprite.set_visible(true);
        }
    }

    for(btn::sprite_ptr& sprite : _sprites)
    {
        sprite.set_visible(enabled);
    }
}
