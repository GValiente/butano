/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_JAM_INTRO_SCENE_H
#define FR_JAM_INTRO_SCENE_H

#include "bn_regular_bg_ptr.h"
#include "bn_bg_palettes_actions.h"
#include "bn_sprite_palettes_actions.h"

#include "fr_scene.h"
#include "fr_camera_3d.h"
#include "fr_models_3d.h"

namespace fr
{

class jam_intro_scene : public scene
{

public:
    jam_intro_scene();

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    bn::bg_palettes_fade_to_action _bgs_fade_in_action;
    bn::sprite_palettes_fade_to_action _sprites_fade_in_action;
    bn::optional<bn::bg_palettes_fade_to_action> _bgs_fade_out_action;
    bn::optional<bn::sprite_palettes_fade_to_action> _sprites_fade_out_action;
    bn::regular_bg_ptr _backdrop_bg;
    camera_3d _camera;
    models_3d _models;
    model_3d* _model = nullptr;
    int _counter = 60 * 4;
};

}

#endif
