#ifndef BF_CREDITS_H
#define BF_CREDITS_H

#include "btn_deque.h"
#include "btn_random.h"
#include "btn_music_actions.h"
#include "btn_sprite_actions.h"
#include "btn_blending_actions.h"
#include "btn_sprite_affine_mat_hblank_effects.h"
#include "bf_scene.h"

namespace btn
{
    class sprite_text_generator;
}

namespace bf
{

class butano_background;

class credits : public scene
{

public:
    credits(btn::sprite_text_generator& text_generator, butano_background& butano_background);

    [[nodiscard]] btn::optional<scene_type> update() final;

private:
    struct background_sprite
    {
        btn::sprite_move_to_action move_action;
        btn::sprite_animate_action<4> animate_action;
    };

    btn::sprite_text_generator& _text_generator;
    btn::sprite_affine_mat_attributes_hblank_effect_ptr _hblank_effect;
    btn::deque<background_sprite, 8> _background_sprites;
    btn::optional<btn::blending_transparency_alpha_to_action> _blending_action;
    btn::optional<btn::music_volume_to_action> _music_volume_action;
    btn::random _random;
    int _background_sprite_counter = 1;
};

}

#endif
