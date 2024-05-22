/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_WIN_SCENE_H
#define FR_WIN_SCENE_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_music_actions.h"
#include "bn_bg_palettes_actions.h"
#include "bn_sprite_palettes_actions.h"

#include "fr_scene.h"
#include "fr_camera_3d.h"
#include "fr_models_3d.h"
#include "fr_announcer.h"

namespace fr
{

class common_stuff;

class win_scene : public scene
{

public:
    explicit win_scene(common_stuff& common_stuff);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    common_stuff& _common_stuff;
    bn::optional<bn::bg_palettes_fade_to_action> _bgs_fade_action;
    bn::optional<bn::sprite_palettes_fade_to_action> _sprites_fade_action;
    bn::optional<bn::music_volume_to_action> _volume_action;
    bn::vector<bn::sprite_ptr, 8> _position_text_sprites;
    bn::vector<bn::sprite_ptr, 8> _stages_text_sprites;
    bn::vector<bn::sprite_ptr, 16> _models_text_sprites;
    announcer _announcer;
    camera_3d _camera;
    models_3d _models;
    model_3d* _w_model = nullptr;
    model_3d* _i_model = nullptr;
    model_3d* _n_model = nullptr;
    int _wait_counter = 0;
    int _unlocked_stages = 0;
    int _unlocked_models = 0;
    int _total_updates = 0;
    bool _done = false;

    void _animate();
};

}

#endif
