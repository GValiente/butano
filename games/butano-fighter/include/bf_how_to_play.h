#ifndef BF_HOW_TO_PLAY_H
#define BF_HOW_TO_PLAY_H

#include "btn_music_actions.h"
#include "btn_sprite_actions.h"
#include "btn_blending_actions.h"
#include "btn_sprite_palette_actions.h"
#include "bf_scene.h"
#include "bf_scene_type.h"

namespace btn
{
    class sprite_text_generator;
}

namespace bf
{

class butano_background;

class how_to_play : public scene
{

public:
    how_to_play(scene_type next_scene, btn::sprite_text_generator& text_generator,
                butano_background& butano_background);

    [[nodiscard]] btn::optional<scene_type> update() final;

private:
    struct entry
    {
        btn::vector<btn::sprite_ptr, 6> text_sprites_1;
        btn::vector<btn::sprite_ptr, 6> text_sprites_2;
        btn::vector<btn::sprite_ptr, 6> text_sprites_3;
        btn::vector<btn::sprite_ptr, 6> text_sprites_4;
        btn::vector<btn::sprite_ptr, 5> graphic_sprites;
        btn::optional<btn::sprite_visible_toggle_action> toggle_action;
        btn::optional<btn::sprite_rotate_by_action> rotate_action;
        btn::optional<btn::sprite_palette_rotate_by_action> palette_rotate_action;
        btn::vector<btn::sprite_cached_animate_action<2>, 4> animate_actions;
        int move_x = 0;
        int move_counter = 0;

        void setup(int x, int y);

        void set_position(int x, int y);

        void animate();
    };

    scene_type _next_scene;
    btn::sprite_text_generator& _text_generator;
    btn::vector<btn::sprite_ptr, 3> _title_sprites;
    btn::vector<entry, 4> _entries;
    btn::optional<btn::blending_transparency_alpha_to_action> _blending_action;
    btn::optional<btn::music_volume_to_action> _music_volume_action;
    int _page_index = 0;

    [[nodiscard]] entry _create_hero_entry();

    [[nodiscard]] entry _create_move_entry();

    [[nodiscard]] entry _create_shoot_entry();

    [[nodiscard]] entry _create_bomb_entry();

    [[nodiscard]] entry _create_shield_entry();

    [[nodiscard]] entry _create_brightness_entry();

    [[nodiscard]] entry _create_experience_entry();

    [[nodiscard]] entry _create_weapon_entry();
};

}

#endif
