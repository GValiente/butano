/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_LOSE_SCENE_H
#define FR_LOSE_SCENE_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_bg_palettes_actions.h"
#include "bn_sprite_palettes_actions.h"

#include "fr_scene.h"
#include "fr_announcer.h"
#include "fr_camera_3d.h"
#include "fr_models_3d.h"

namespace fr
{

class common_stuff;

class lose_scene : public scene
{

public:
    explicit lose_scene(common_stuff& common_stuff);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    bn::optional<bn::bg_palettes_fade_to_action> _bgs_fade_action;
    bn::optional<bn::sprite_palettes_fade_to_action> _sprites_fade_action;
    bn::sprite_ptr _cursor_sprite;
    bn::vector<bn::sprite_ptr, 4> _text_sprites;
    announcer _announcer;
    camera_3d _camera;
    models_3d _models;
    model_3d* _l_model = nullptr;
    model_3d* _o_model = nullptr;
    model_3d* _s_model = nullptr;
    model_3d* _e_model = nullptr;
    int _cursor_index = 0;
    int _wait_frames = 0;
    bool _animation_done = false;

    void _animate();
};

}

#endif
