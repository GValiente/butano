/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_MODEL_VIEWER_SCENE_H
#define FR_MODEL_VIEWER_SCENE_H

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
#include "fr_menu_keypad.h"

namespace fr
{

class common_stuff;

class model_viewer_scene : public scene
{

public:
    explicit model_viewer_scene(common_stuff& common_stuff);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    struct menu_entry
    {
        bn::vector<bn::sprite_ptr, 4> text_sprites;
    };

    common_stuff& _common_stuff;
    bn::bg_palettes_fade_to_action _bgs_fade_in_action;
    bn::sprite_palettes_fade_to_action _sprites_fade_in_action;
    bn::optional<bn::bg_palettes_fade_to_action> _bgs_fade_out_action;
    bn::optional<bn::sprite_palettes_fade_to_action> _sprites_fade_out_action;
    bn::optional<bn::music_volume_to_action> _music_volume_action;
    bn::regular_bg_ptr _backdrop_bg;
    bn::regular_bg_ptr _menu_bg;
    bn::vector<bn::sprite_ptr, 4> _main_title_sprites;
    bn::vector<bn::sprite_ptr, 4> _menu_title_sprites;
    bn::sprite_ptr _cursor_sprite;
    bn::deque<menu_entry, 16> _menu_entries;
    camera_3d _camera;
    models_3d _models;
    menu_keypad _menu_keypad;
    model_3d* _model = nullptr;
    bn::fixed _interrogation_phi = 16384;
    int _item_index = 0;
    int _menu_entry_index = 0;
    int _showing_menu_frames = false;
    int _hiding_menu_frames = false;
    bool _menu_active = true;

    void _load_menu();

    void _add_new_menu_entry(int item_index, int menu_entry_index, bool back);

    void _load_model();

    void _show_menu();

    void _hide_menu();

    void _update_menu();

    void _update_model();

    void _increase_item_index();

    void _decrease_item_index();

    void _update_menu_cursor();

    void _exit();
};

}

#endif
