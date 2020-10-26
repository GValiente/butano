/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef INFO_H
#define INFO_H

#include "btn_vector.h"
#include "btn_span_fwd.h"
#include "btn_sprite_ptr.h"

namespace btn
{
    class string_view;
    class sprite_text_generator;
}

class info
{

public:
    info(const btn::span<const btn::string_view>& text_lines, btn::sprite_text_generator& text_generator);

    info(const btn::string_view& title, const btn::span<const btn::string_view>& text_lines,
         btn::sprite_text_generator& text_generator);

    [[nodiscard]] bool show_always() const
    {
        return _show_always;
    }

    void set_show_always(bool show_always);

    void update();

private:
    btn::vector<btn::sprite_ptr, 8> _title_sprites;
    btn::vector<btn::sprite_ptr, 48> _text_sprites;
    btn::vector<btn::sprite_ptr, 4> _hide_info_sprites;
    btn::vector<btn::sprite_ptr, 4> _show_info_sprites;
    bool _show_always = false;
    bool _enabled = false;

    void _update_sprites();
};

#endif
