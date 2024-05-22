/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_PALETTE_TARGET_ID_H
#define BN_PALETTE_TARGET_ID_H

#include "bn_common.h"

namespace bn
{

struct palette_target_id
{
    union
    {
        struct
        {
            int16_t palette_id;
            int16_t final_color_index;
        } params;
        intptr_t target_id;
    };

    palette_target_id(int palette_id, int color_index) :
        params({ int16_t(palette_id), int16_t((palette_id * 16) + color_index) })
    {
    }

    explicit palette_target_id(int _target_id) :
        target_id(_target_id)
    {
    }
};

static_assert(sizeof(palette_target_id) == sizeof(int));
static_assert(alignof(palette_target_id) == alignof(int));

}

#endif
