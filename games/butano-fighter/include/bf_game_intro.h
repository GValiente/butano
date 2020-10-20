#ifndef BF_GAME_INTRO_H
#define BF_GAME_INTRO_H

#include "btn_vector.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_sprite_actions.h"
#include "btn_rect_window_actions.h"
#include "btn_sprite_palette_actions.h"
#include "btn_sprite_third_attributes.h"
#include "btn_sprite_third_attributes_hblank_effect_ptr.h"

namespace btn
{
    class sprite_text_generator;
}

namespace bf
{
    class butano_background;
}

namespace bf::game
{

class stage;

class intro
{

public:
    intro(const stage& stage, btn::sprite_text_generator& text_generator);

    intro(const intro& other) = delete;

    intro& operator=(const intro& other) = delete;

    [[nodiscard]] bool active() const
    {
        return _state != state::DONE;
    }

    void update(const butano_background& butano_background);

private:
    enum class state
    {
        INIT,
        WAIT_1,
        IN,
        WAIT_2,
        WAIT_3,
        OUT,
        DONE
    };

    btn::vector<btn::sprite_ptr, 2> _background_sprites;
    btn::optional<btn::sprite_palette_ptr> _alt_palette;
    btn::vector<btn::sprite_ptr, 40> _text_sprites;
    btn::vector<btn::sprite_move_to_action, 2> _background_sprite_move_actions;
    btn::vector<btn::sprite_vertical_scale_to_action, 2> _background_sprite_vertical_scale_actions;
    btn::vector<btn::sprite_vertical_scale_to_action, 40> _text_sprite_vertical_scale_actions;
    btn::vector<btn::sprite_palette_fade_to_action, 2> _background_sprite_palette_actions;
    btn::vector<btn::sprite_third_attributes, btn::display::height()> _background_sprite_hblank_effect_attributes_1;
    btn::vector<btn::sprite_third_attributes, btn::display::height()> _background_sprite_hblank_effect_attributes_2;
    btn::vector<btn::sprite_third_attributes_hblank_effect_ptr, 2> _background_sprite_hblank_effects;
    btn::optional<btn::rect_window_move_top_to_action> _window_move_top_action;
    btn::optional<btn::rect_window_move_bottom_to_action> _window_move_bottom_action;
    state _state = state::INIT;
    int _counter = -1;
};

}

#endif
