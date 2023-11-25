#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_vector.h"
#include "bn_sprites.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_affine_mat_ptr.h"

#include "bn_sprite_items_red.h"
#include "bn_sprite_items_blue.h"

int main()
{
    bn::core::init();

    bn::vector<bn::sprite_ptr, 32> sprites;
    bn::sprites::set_reserved_handles_count(92);

    bn::sprite_affine_mat_ptr affine_mat = bn::sprite_affine_mat_ptr::create();

    for(int index = 0; index < 32; ++index)
    {
        bn::sprite_ptr sprite = bn::sprite_items::blue.create_sprite(80 - (index * 8), -48 + ((index % 4) * 32));
        sprite.set_affine_mat(affine_mat);
        sprites.push_back(bn::move(sprite));
    }

    sprites[10].set_item(bn::sprite_items::red);

    while(true)
    {
        if(bn::keypad::a_pressed())
        {
            if(bn::sprites::reserved_handles_count())
            {
                bn::sprites::set_reserved_handles_count(0);
            }
            else
            {
                bn::sprites::set_reserved_handles_count(92);
            }
        }

        bn::core::update();
    }
}
