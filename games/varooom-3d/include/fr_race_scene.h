/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_RACE_SCENE_H
#define FR_RACE_SCENE_H

#include "fr_scene.h"
#include "fr_pause.h"
#include "fr_announcer.h"
#include "fr_camera_3d.h"
#include "fr_models_3d.h"
#include "fr_scoreboard.h"
#include "fr_player_car.h"
#include "fr_race_intro.h"
#include "fr_race_outro.h"
#include "fr_race_state.h"
#include "fr_rival_cars.h"
#include "fr_background_3d.h"
#include "fr_foreground_3d.h"

namespace fr
{

class race_scene : public scene
{

public:
    explicit race_scene(common_stuff& common_stuff);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    const stage& _stage;
    common_stuff& _common_stuff;
    announcer _announcer;
    camera_3d _camera;
    background_3d _background;
    models_3d _models;
    race_state _state;
    player_car _player_car;
    rival_cars _rival_cars;
    foreground_3d _foreground;
    scoreboard _scoreboard;
    pause _pause;
    race_intro _intro;
    race_outro _outro;
    int _frame_index = 0;
};

}

#endif
