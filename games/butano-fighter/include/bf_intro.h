#ifndef BF_INTRO_H
#define BF_INTRO_H

#include "btn_vector.h"
#include "btn_sprite_ptr.h"
#include "btn_blending_actions.h"
#include "btn_bg_palettes_actions.h"
#include "btn_sprite_palettes_actions.h"
#include "bf_scene.h"

namespace btn
{
    class sprite_text_generator;
}

namespace bf
{

class butano_background;

class intro : public scene
{

public:
    explicit intro(btn::sprite_text_generator& text_generator, butano_background& butano_background);

    [[nodiscard]] btn::optional<scene_type> update() final;

private:
    btn::bg_palettes_fade_to_action _bg_fade_action;
    btn::sprite_palettes_fade_to_action _sprite_fade_action;
    btn::blending_transparency_alpha_to_action _blending_action;
    btn::vector<btn::sprite_ptr, 4> _up_text_sprites;
    btn::vector<btn::sprite_ptr, 8> _middle_text_sprites;
    btn::vector<btn::sprite_ptr, 8> _down_text_sprites;
    int _counter = 60 * 4;
};

}

#endif
