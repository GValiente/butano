#ifndef BF_TITLE_H
#define BF_TITLE_H

#include "btn_fixed.h"
#include "btn_sprite_ptr.h"
#include "bf_scene.h"

namespace btn
{
    class sprite_text_generator;
}

namespace bf
{

class title : public scene
{

public:
    title(btn::sprite_text_generator& text_generator);

    [[nodiscard]] btn::optional<scene_type> update() final;

private:
    btn::sprite_ptr _butano_up_sprite;
    btn::sprite_ptr _butano_down_sprite;
    btn::fixed _butano_y_inc = -4;
    bool _butano_y_up = true;
};

}

#endif
