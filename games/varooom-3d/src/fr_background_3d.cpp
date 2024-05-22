/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_background_3d.h"

#include "bn_blending.h"
#include "bn_affine_bg_item.h"
#include "bn_affine_bg_mat_attributes.h"

#include "fr_stage.h"
#include "fr_div_lut.h"
#include "fr_camera_3d.h"

namespace fr
{

background_3d::background_3d(const stage& stage) :
    _ground_bg(stage.ground_bg_item().create_bg(0, 0)),
    _clouds_bg(stage.clouds_bg_item().create_bg(0, 0)),
    _palette(_ground_bg.palette())
{
    _ground_bg.set_mosaic_enabled(true);

    _clouds_bg.set_priority(2);
    _clouds_bg.set_mosaic_enabled(true);
    _clouds_bg.set_blending_enabled(true);

    bn::blending::set_transparency_alpha(stage.clouds_bg_transparency_alpha());
}

background_3d::~background_3d()
{
    bn::blending::set_transparency_alpha(1);
}

void background_3d::set_fade(bn::color color, bn::fixed intensity)
{
    _palette.set_fade(color, intensity);
}

void background_3d::update(const stage& stage, const camera_3d& camera)
{
    point_3d camera_position = camera.position();
    bn::fixed_point pivot_position(camera_position.x() - 512, camera_position.z() - 512);
    _ground_bg.set_pivot_position(pivot_position);
    _clouds_bg.set_pivot_position(pivot_position + _clouds_bg_pivot_diff);
    _clouds_bg_pivot_diff += stage.clouds_bg_pivot_inc();

    // bn::fixed scale = bn::fixed(256).division(camera_position.y());
    bn::fixed scale = bn::fixed::from_data(div_lut_ptr[camera_position.y().data() >> 10] >> 2);
    _ground_bg.set_scale(scale);

    int degrees = camera.phi().right_shift_integer() * 360;
    bn::fixed rotation_angle = bn::fixed::from_data(degrees >> 4);
    _ground_bg.set_rotation_angle(rotation_angle);

    _clouds_bg.set_mat_attributes(_ground_bg.mat_attributes().mat_attributes());
}

}
