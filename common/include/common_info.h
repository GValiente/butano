/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef COMMON_INFO_H
#define COMMON_INFO_H

#include "bn_span.h"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_string_view.h"

namespace bn
{
    class sprite_text_generator;
}

namespace common
{

class info
{

public:
    info(const bn::span<const bn::string_view>& text_lines, bn::sprite_text_generator& text_generator);

    info(const bn::string_view& title, const bn::span<const bn::string_view>& text_lines,
         bn::sprite_text_generator& text_generator);

    [[nodiscard]] bool show_always() const
    {
        return _show_always;
    }

    void set_show_always(bool show_always);

    void update();

private:
    bn::vector<bn::sprite_ptr, 8> _title_sprites;
    bn::vector<bn::sprite_ptr, 48> _text_sprites;
    bn::vector<bn::sprite_ptr, 4> _hide_info_sprites;
    bn::vector<bn::sprite_ptr, 4> _show_info_sprites;
    bool _show_always = false;
    bool _enabled = false;

    void _update_sprites();
};

}

#endif
