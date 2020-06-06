#ifndef SPRITE_SORT_TESTS_H
#define SPRITE_SORT_TESTS_H

#include "btn_vector.h"
#include "btn_random.h"
#include "btn_sort_key.h"
#include "btn_sprite_ptr.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_items_fixed_8x8_font.h"
#include "tests.h"

#include "btn_core.h"

class sprite_sort_tests : public tests
{

public:
    sprite_sort_tests() :
        tests("sprite_sort")
    {
        BTN_ASSERT(btn::sort_key(1, 0) > btn::sort_key(0, 0));
        BTN_ASSERT(btn::sort_key(2, 0) > btn::sort_key(1, 0));
        BTN_ASSERT(btn::sort_key(2, 0) > btn::sort_key(0, 0));

        BTN_ASSERT(btn::sort_key(0, 1) > btn::sort_key(0, 0));
        BTN_ASSERT(btn::sort_key(0, 2) > btn::sort_key(0, 1));
        BTN_ASSERT(btn::sort_key(0, 2) > btn::sort_key(0, 0));

        BTN_ASSERT(btn::sort_key(0, -1) < btn::sort_key(0, 0));
        BTN_ASSERT(btn::sort_key(0, -1) < btn::sort_key(0, 0));
        BTN_ASSERT(btn::sort_key(0, -2) < btn::sort_key(0, -1));
        BTN_ASSERT(btn::sort_key(0, -2) < btn::sort_key(0, 0));

        BTN_ASSERT(btn::sort_key(0, -1) < btn::sort_key(0, 1));
        BTN_ASSERT(btn::sort_key(0, -1) < btn::sort_key(0, 1));
        BTN_ASSERT(btn::sort_key(0, -2) < btn::sort_key(0, 2));
        BTN_ASSERT(btn::sort_key(0, -2) < btn::sort_key(0, 1));

        BTN_ASSERT(btn::sort_key(1, 0) > btn::sort_key(0, 1));
        BTN_ASSERT(btn::sort_key(1, 0) > btn::sort_key(0, 2));

        BTN_ASSERT(btn::sort_key(1, 0) > btn::sort_key(0, -1));
        BTN_ASSERT(btn::sort_key(1, 0) > btn::sort_key(0, -2));

        btn::random random;
        btn::vector<btn::sprite_ptr, 64> sprites;

        for(int i = 0; i < 1000; ++i)
        {
            int command = random.get() % 8;

            switch(command)
            {

            case 0:
            case 1:
            case 2:
                if(sprites.full())
                {
                    int new_sprites_size = random.get() % 64;

                    while(sprites.size() > new_sprites_size)
                    {
                        int index = random.get() % sprites.size();
                        sprites.erase(sprites.begin() + index);
                    }
                }
                else
                {
                    int bg_priority = random.get() % 4;
                    int z_order = random.get() % 4;
                    btn::sprite_builder builder(btn::sprite_items::fixed_8x8_font);
                    builder.set_bg_priority(bg_priority);
                    builder.set_z_order(z_order);
                    builder.set_position(-64, -64);
                    sprites.push_back(builder.release_build());
                    BTN_ASSERT(sprites.back().bg_priority() == bg_priority);
                    BTN_ASSERT(sprites.back().z_order() == z_order);
                }
                break;

            case 3:
            case 4:
                if(! sprites.empty())
                {
                    int index = random.get() % sprites.size();
                    sprites.erase(sprites.begin() + index);
                }
                break;

            case 5:
                if(! sprites.empty())
                {
                    btn::sprite_ptr& sprite = sprites[random.get() % sprites.size()];
                    int bg_priority = random.get() % 4;
                    sprite.set_bg_priority(bg_priority);
                    BTN_ASSERT(sprite.bg_priority() == bg_priority);
                }
                break;

            case 6:
                if(! sprites.empty())
                {
                    btn::sprite_ptr& sprite = sprites[random.get() % sprites.size()];
                    int z_order = random.get() % 4;
                    sprite.set_z_order(z_order);
                    BTN_ASSERT(sprite.z_order() == z_order);
                }
                break;

            case 7:
                if(! sprites.empty())
                {
                    btn::sprite_ptr& first_sprite = sprites[random.get() % sprites.size()];
                    btn::sprite_ptr& second_sprite = sprites[random.get() % sprites.size()];

                    if(first_sprite != second_sprite)
                    {
                        first_sprite.put_above(second_sprite);
                        BTN_ASSERT(first_sprite.above(second_sprite).value_or(true));
                    }
                }
                break;

            default:
                BTN_ERROR("Invalid command: ", command);
                break;
            }
        }
    }
};

#endif
