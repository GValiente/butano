/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_BACKGROUND_3D_H
#define FR_BACKGROUND_3D_H

#include "bn_fixed_point.h"
#include "bn_affine_bg_ptr.h"
#include "bn_bg_palette_ptr.h"

namespace fr
{

class stage;
class camera_3d;

class background_3d
{

public:
    explicit background_3d(const stage& stage);

    ~background_3d();

    void set_fade(bn::color color, bn::fixed intensity);

    void update(const stage& stage, const camera_3d& camera);

private:
    bn::affine_bg_ptr _ground_bg;
    bn::affine_bg_ptr _clouds_bg;
    bn::bg_palette_ptr _palette;
    bn::fixed_point _clouds_bg_pivot_diff;
};

}

#endif
