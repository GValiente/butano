/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_CREDITS_SCENE_H
#define FR_CREDITS_SCENE_H

#include "bn_deque.h"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_music_actions.h"
#include "bn_regular_bg_ptr.h"
#include "bn_bg_palettes_actions.h"
#include "bn_sprite_palettes_actions.h"

#include "fr_scene.h"
#include "fr_camera_3d.h"
#include "fr_models_3d.h"

namespace fr
{

class common_stuff;

class credits_scene : public scene
{

public:
    explicit credits_scene(common_stuff& common_stuff);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    struct text_entry
    {
        bn::vector<bn::sprite_ptr, 12> sprites;
        bn::fixed y;
        bool logo;
    };

    common_stuff& _common_stuff;
    bn::regular_bg_ptr _backdrop_bg;
    bn::regular_bg_ptr _player_car_bg;
    bn::regular_bg_ptr _logo_bg;
    bn::bg_palettes_fade_to_action _bgs_fade_in_action;
    bn::sprite_palettes_fade_to_action _sprites_fade_in_action;
    bn::deque<text_entry, 16> _text_entries;
    bn::optional<bn::bg_palettes_fade_to_action> _bgs_fade_out_action;
    bn::optional<bn::sprite_palettes_fade_to_action> _sprites_fade_out_action;
    bn::optional<bn::music_volume_to_action> _music_volume_action;
    camera_3d _camera;
    models_3d _models;
    model_3d* _model = nullptr;
    unsigned _model_index = 0;
    int _text_index = 0;
    int _text_counter = 1;
    int _player_car_counter = 0;

    void _update_models();

    void _update_text();

    void _finish();
};

}

#endif
