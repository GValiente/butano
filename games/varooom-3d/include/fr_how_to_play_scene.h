/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_HOW_TO_PLAY_SCENE_H
#define FR_HOW_TO_PLAY_SCENE_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_actions.h"
#include "bn_bg_palettes_actions.h"
#include "bn_sprite_palettes_actions.h"

#include "fr_scene.h"
#include "fr_camera_3d.h"
#include "fr_models_3d.h"

namespace fr
{

class common_stuff;

class how_to_play_scene : public scene
{

public:
    how_to_play_scene(scene_type next_scene, common_stuff& common_stuff);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    common_stuff& _common_stuff;
    bn::bg_palettes_fade_to_action _bgs_fade_in_action;
    bn::sprite_palettes_fade_to_action _sprites_fade_in_action;
    bn::optional<bn::bg_palettes_fade_to_action> _bgs_fade_out_action;
    bn::optional<bn::sprite_palettes_fade_to_action> _sprites_fade_out_action;
    bn::regular_bg_ptr _backdrop_bg;
    bn::regular_bg_ptr _background_bg;
    bn::regular_bg_move_by_action _background_move_action;
    bn::vector<bn::sprite_ptr, 4> _title_sprites;
    bn::vector<bn::sprite_ptr, 4> _left_right_text_sprites;
    bn::vector<bn::sprite_ptr, 4> _a_text_sprites;
    bn::vector<bn::sprite_ptr, 4> _b_text_sprites;
    bn::vector<bn::sprite_ptr, 12> _r_text_sprites;
    bn::vector<bn::sprite_ptr, 4> _start_text_sprites;
    camera_3d _camera;
    models_3d _models;
    model_3d* _gba_model = nullptr;
    bn::fixed _model_x_inc;
    bn::fixed _model_z_inc;
    bn::fixed _model_phi_inc;
    bn::fixed _model_psi_inc;
    scene_type _next_scene;
    int _animation_index = 0;
    int _animation_frames = 0;

    void _setup_animation();

    void _animate();
};

}

#endif
