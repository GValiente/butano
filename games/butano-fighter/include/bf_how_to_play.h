/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_HOW_TO_PLAY_H
#define BF_HOW_TO_PLAY_H

#include "bn_music_actions.h"
#include "bn_sprite_actions.h"
#include "bn_blending_actions.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_palette_actions.h"
#include "bf_scene.h"
#include "bf_scene_type.h"

namespace bn
{
    class sprite_text_generator;
}

namespace bf
{

class status;
class butano_background;

class how_to_play : public scene
{

public:
    how_to_play(scene_type next_scene, status& status, bn::sprite_text_generator& text_generator,
                butano_background& butano_background);

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    struct entry_type
    {
        bn::vector<bn::sprite_ptr, 30> sprites;
        bn::optional<bn::sprite_visible_toggle_action> toggle_action;
        bn::optional<bn::sprite_rotate_by_action> rotate_action;
        bn::optional<bn::sprite_palette_fade_loop_action> palette_fade_action;
        bn::optional<bn::sprite_palette_rotate_by_action> palette_rotate_action;
        bn::vector<bn::sprite_cached_animate_action<4>, 4> animate_actions;
        bn::optional<bn::sprite_palette_ptr> flash_palette;
        int move_x = 0;
        int move_counter = 0;
        int flash_palette_counter = 1;

        void setup(int x, int y);

        void set_position(int x, int y);

        void animate();
    };

    scene_type _next_scene;
    status& _status;
    bn::sprite_text_generator& _text_generator;
    bn::vector<bn::sprite_ptr, 3> _title_sprites;
    bn::vector<entry_type, 2> _entries;
    bn::optional<bn::blending_transparency_alpha_to_action> _blending_action;
    bn::optional<bn::music_volume_to_action> _music_volume_action;
    int _page_index = 0;

    [[nodiscard]] entry_type _create_hero_entry();

    [[nodiscard]] entry_type _create_move_entry();

    [[nodiscard]] entry_type _create_shoot_entry();

    [[nodiscard]] entry_type _create_bomb_entry();

    [[nodiscard]] entry_type _create_shield_entry();

    [[nodiscard]] entry_type _create_brightness_entry();

    [[nodiscard]] entry_type _create_experience_entry();

    [[nodiscard]] entry_type _create_weapon_entry();

    [[nodiscard]] entry_type _create_gem_entry();

    [[nodiscard]] entry_type _create_bomb_item_entry();
};

}

#endif
