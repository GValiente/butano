/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_TITLE_SCENE_H
#define FR_TITLE_SCENE_H

#include "bn_music_actions.h"
#include "bn_regular_bg_actions.h"
#include "bn_bg_palettes_actions.h"
#include "bn_sprite_palettes_actions.h"

#include "fr_scene.h"
#include "fr_announcer.h"
#include "fr_camera_3d.h"
#include "fr_models_3d.h"
#include "fr_title_flag.h"
#include "fr_title_advices.h"
#include "fr_title_base_menu.h"
#include "fr_title_race_menu.h"
#include "fr_title_options_menu.h"

namespace fr
{

class common_stuff;

class title_scene : public scene
{

public:
    explicit title_scene(common_stuff& common_stuff);

    ~title_scene();

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    common_stuff& _common_stuff;
    title_base_menu _base_menu;
    title_race_menu _race_menu;
    title_options_menu _options_menu;
    title_advices _advices;
    bn::optional<title_base_menu::option> _base_menu_selected_option;
    bn::optional<bn::regular_bg_ptr> _back_backdrop_bg;
    bn::optional<bn::regular_bg_ptr> _middle_backdrop_bg;
    bn::optional<bn::regular_bg_move_by_action> _middle_backdrop_action;
    bn::optional<bn::regular_bg_ptr> _front_backdrop_bg;
    bn::optional<bn::bg_palettes_fade_to_action> _bgs_fade_action;
    bn::optional<bn::bg_palettes_fade_to_action> _bgs_fade_out_action;
    bn::optional<bn::sprite_palettes_fade_to_action> _sprites_fade_out_action;
    bn::optional<bn::music_volume_to_action> _music_volume_action;
    camera_3d _camera;
    models_3d _models;
    announcer _announcer;
    title_flag _flag;
    model_3d* _model = nullptr;
    bn::fixed _model_inc_x;
    int _intro_index = -1;
    int _wait_frames = 0;
    bool _intro_scene_started = false;
    bool _intro_done = false;

    void _update_intro();

    void _update_intro_minus_1();

    void _update_intro_0();

    void _update_intro_1();

    void _update_intro_2();

    void _update_intro_3();

    void _update_intro_4();

    void _update_intro_5();

    void _update_intro_6();

    void _update_intro_7();

    void _update_intro_8();

    void _update_intro_9();

    void _update_intro_10();

    void _update_intro_11();

    void _exit();
};

}

#endif
