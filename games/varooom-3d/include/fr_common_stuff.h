/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
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

    [[nodiscard]] int finish_time() const
    {
        return _finish_time;
    }

    void set_finish_position_and_time(int position, int time)
    {
        _finish_position = position;
        _finish_time = time;
    }

    void update();

private:
    int _finish_position = 0;
    int _finish_time = 0;
    bool _reset_ready = false;
};

}

#endif
