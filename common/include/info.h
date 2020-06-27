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

    void update();

private:
    btn::sprite_text_generator& _text_generator;
    btn::vector<btn::sprite_ptr, 48> _sprites;
    btn::vector<btn::sprite_ptr, 8> _down_sprites;
    bool _enabled = false;

    void _update_sprites();
};

#endif
