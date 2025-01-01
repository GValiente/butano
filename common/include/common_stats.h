/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef COMMON_STATS_H
#define COMMON_STATS_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"

namespace bn
{
    class sprite_text_generator;
}

namespace common
{

class stats
{

public:
    enum class mode_type
    {
        DISABLED,
        SIMPLE,
        DETAILED
    };

    explicit stats(bn::sprite_text_generator& text_generator);

    [[nodiscard]] mode_type mode() const
    {
        return _mode;
    }

    void set_mode(mode_type mode);

    void update();

private:
    bn::sprite_text_generator& _text_generator;
    bn::vector<bn::sprite_ptr, 8> _static_text_sprites;
    bn::vector<bn::sprite_ptr, 2> _text_sprites;
    bn::fixed_point _text_position;
    bn::fixed _max_cpu_usage;
    mode_type _mode = mode_type::SIMPLE;
    int _counter = 0;
};

}

#endif
