/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_COMMON_STUFF_H
#define FR_COMMON_STUFF_H

#include "bn_sprite_text_generator.h"

#include "fr_sram_data.h"

namespace fr
{

class common_stuff
{

public:
    bn::sprite_text_generator big_text_generator;
    bn::sprite_text_generator small_fixed_text_generator;
    bn::sprite_text_generator small_variable_text_generator;
    sram_data storage;

    common_stuff();

    [[nodiscard]] int finish_position() const
    {
        return _finish_position;
    }

    void set_finish_position(int position)
    {
        _finish_position = position;
    }

    void update();

private:
    int _finish_position = 0;
    bool _reset_ready = false;
};

}

#endif
