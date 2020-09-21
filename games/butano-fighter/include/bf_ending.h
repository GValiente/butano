#ifndef BF_ENDING_H
#define BF_ENDING_H

#include "btn_vector.h"
#include "btn_optional.h"
#include "btn_sprite_ptr.h"
#include "btn_regular_bg_ptr.h"
#include "btn_blending_actions.h"
#include "bf_scene.h"

namespace btn
{
    class sprite_text_generator;
}

namespace bf
{

class butano_background;

class ending : public scene
{

public:
    ending(btn::sprite_text_generator& text_generator, butano_background& butano_background);

    [[nodiscard]] btn::optional<scene_type> update() final;

private:
    butano_background& _butano_background;
    btn::optional<btn::regular_bg_ptr> _bg;
    btn::blending_transparency_alpha_to_action _blending_action;
    btn::vector<btn::sprite_ptr, 20> _text_sprites;
    int _counter = 10000;
    bool _updated = false;
};

}

#endif
