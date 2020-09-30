#ifndef BF_TITLE_H
#define BF_TITLE_H

#include "btn_vector.h"
#include "btn_display.h"
#include "btn_music_actions.h"
#include "btn_sprite_actions.h"
#include "btn_blending_actions.h"
#include "btn_sprite_position_hblank_effect_ptr.h"
#include "btn_sprite_affine_mat_attributes_hblank_effect_ptr.h"
#include "bf_scene.h"

namespace btn
{
    class sprite_text_generator;
}

namespace bf
{

class status;
class butano_background;

class title : public scene
{

public:
    title(const status& status, btn::sprite_text_generator& text_generator, butano_background& butano_background);

    title(const title& other) = delete;

    title& operator=(const title& other) = delete;

    [[nodiscard]] btn::optional<scene_type> update() final;

private:
    enum class state
    {
        START,
        BUTANO_CHARACTERS_SHOW,
        FIGHTER_CHARACTERS_SHOW,
        MENU,
        HIDE_CURSOR,
        HIDE_SPRITES,
        END
    };

    const status& _status;
    btn::sprite_ptr _butano_up_sprite;
    btn::sprite_ptr _butano_down_sprite;
    btn::vector<btn::sprite_ptr, 6> _butano_characters;
    btn::vector<btn::sprite_ptr, 7> _fighter_characters;
    btn::vector<btn::sprite_ptr, 4> _high_experience_text_sprites;
    btn::vector<btn::sprite_ptr, 2> _start_text_sprites;
    btn::vector<btn::sprite_ptr, 3> _how_to_play_sprites;
    btn::vector<btn::sprite_ptr, 2> _credits_text_sprites;
    btn::sprite_ptr _cursor_sprite;
    btn::fixed _butano_y_inc = -4.05;
    btn::array<btn::fixed, btn::display::height()> _butano_x_hblank_effect_deltas;
    btn::optional<btn::sprite_position_hblank_effect_ptr> _butano_up_x_hblank_effect;
    btn::optional<btn::sprite_position_hblank_effect_ptr> _butano_down_x_hblank_effect;
    btn::optional<btn::sprite_move_to_action> _butano_character_move_action;
    btn::optional<btn::sprite_position_hblank_effect_ptr> _butano_character_hblank_effect;
    btn::optional<btn::sprite_move_to_action> _fighter_character_move_action;
    btn::optional<btn::sprite_move_to_action> _cursor_move_action;
    btn::optional<btn::sprite_scale_to_action> _cursor_scale_action;
    btn::optional<btn::music_volume_to_action> _music_volume_action;
    btn::optional<btn::blending_intensity_alpha_to_action> _blending_intensity_action;
    btn::optional<btn::blending_transparency_alpha_to_action> _blending_transparency_action;
    state _state = state::START;
    int _menu_index = 0;
    int _butano_x_hblank_effect_speed = 64 * 32;
    btn::optional<btn::sprite_affine_mat_attributes_hblank_effect_ptr> _fighter_character_hblank_effect;
    bool _butano_y_up = true;

    void _animate_butano_x();

    void _animate_butano_y();

    void _animate_butano_characters();

    void _animate_fighter_characters();

    btn::optional<scene_type> _menu();
};

}

#endif
