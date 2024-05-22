/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_RACE_INTRO_H
#define FR_RACE_INTRO_H

#include "bn_bg_palettes_actions.h"
#include "bn_sprite_palettes_actions.h"

#include "fr_constants_3d.h"

namespace fr
{

class stage;
class model_3d;
class models_3d;
class camera_3d;

class race_intro
{

public:
    explicit race_intro(models_3d& models);

    ~race_intro();

    [[nodiscard]] bool done() const
    {
        return _index >= 9;
    }

    void update(const stage& stage, camera_3d& camera);

private:
    models_3d& _models;
    bn::optional<bn::bg_palettes_fade_to_action> _bg_fade_action;
    bn::optional<bn::sprite_palettes_fade_to_action> _sprite_fade_action;
    model_3d* _character_model_1 = nullptr;
    model_3d* _character_model_2 = nullptr;
    bn::fixed _camera_y = 128 - constants_3d::camera_diff_y;
    int _index = 0;
    int _counter = 0;

    void _update_camera(camera_3d& camera);
};

}

#endif
