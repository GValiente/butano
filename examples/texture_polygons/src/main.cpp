/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_size.h"
#include "bn_color.h"
#include "bn_point.h"
#include "bn_keypad.h"
#include "bn_fixed_point.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_affine_mat_ptr.h"
#include "bn_affine_mat_attributes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_double_size_mode.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_sprite_items_bullet.h"
#include "bn_sprite_items_texture.h"

namespace
{
    constexpr bn::string_view info_text_lines[] = {
        "A: choose vertex to move",
        "PAD: move vertex",
    };

    class triangle
    {

    public:
        triangle(const bn::sprite_item& sprite_item, const bn::point& p0, const bn::point& p1, const bn::point& p2) :
            _sprite(sprite_item.create_sprite(0, 0)),
            _sprite_affine_mat(bn::sprite_affine_mat_ptr::create()),
            _p0(p0),
            _p1(p1),
            _p2(p2),
            _half_size(sprite_item.shape_size().width() / 2)
        {
            _sprite.set_affine_mat(_sprite_affine_mat);
            _update();
        }

        void set_p0(const bn::point& p0)
        {
            _p0 = p0;
            _update();
        }

        void set_p1(const bn::point& p1)
        {
            _p1 = p1;
            _update();
        }

        void set_p2(const bn::point& p2)
        {
            _p2 = p2;
            _update();
        }

    private:
        bn::sprite_ptr _sprite;
        bn::sprite_affine_mat_ptr _sprite_affine_mat;
        bn::point _p0;
        bn::point _p1;
        bn::point _p2;
        int _half_size;

        void _update()
        {
            switch(_half_size)
            {

            case 4:
                _update_impl<4>();
                break;

            case 8:
                _update_impl<8>();
                break;

            case 16:
                _update_impl<16>();
                break;

            case 32:
                _update_impl<32>();
                break;

            default:
                BN_ERROR("Invalid half size: ", _half_size);
                break;
            }
        }

        template<int half_size>
        void _update_impl()
        {
            int x0 = _p0.x();
            int y0 = _p0.y();
            int x1 = _p1.x();
            int y1 = _p1.y();
            int x2 = _p2.x();
            int y2 = _p2.y();
            int affine_divisor = x0*y1 - x0*y2 - x1*y0 + x1*y2 + x2*y0 - x2*y1;

            if(! affine_divisor)
            {
                _sprite.set_visible(false);
                return;
            }

            int pa = (-512*half_size*y1 + 512*half_size*y2 + 256*y1 - 256*y2) / affine_divisor;
            int pb = (256*(2*half_size*x1 - 2*half_size*x2 - x1 + x2)) / affine_divisor;
            int pc = (-512*half_size*y0 + 512*half_size*y1 + 256*y0 - 256*y1) / affine_divisor;
            int pd = (256*(2*half_size*x0 - 2*half_size*x1 - x0 + x1)) / affine_divisor;
            bn::affine_mat_attributes attributes;
            attributes.unsafe_set_register_values(pa, pb, pc, pd);
            _sprite_affine_mat.set_attributes(attributes);

            int u0 = -half_size;
            int v0 = half_size;

            int u1 = half_size;
            int v1 = half_size;

            int u2 = half_size;
            int v2 = -half_size;

            int u0v1 = u0 * v1;
            int u0v2 = u0 * v2;

            int u1v0 = u1 * v0;
            int u1v2 = u1 * v2;

            int u2v0 = u2 * v0;
            int u2v1 = u2 * v1;

            int delta_x = (u0v1 * x2) + (u2v0 * x1) + (x0 * u1v2) - (x0 * u2v1) - (u1v0 * x2) - (u0v2 * x1);
            int delta_y = (u0v1 * y2) + (y1 * u2v0) + (y0 * u1v2) - (y0 * u2v1) - (u1v0 * y2) - (u0v2 * y1);
            int position_divisor = u0v1 + u2v0 + u1v2 - u2v1 - u1v0 - u0v2;
            _sprite.set_position(delta_x / position_divisor, delta_y / position_divisor);
            _sprite.set_visible(true);
        }
    };
}

int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::color(8, 8, 8));

    bn::sprite_text_generator big_text_generator(common::variable_8x16_sprite_font);
    common::info info("Textured polygons with sprites", info_text_lines, big_text_generator);

    bn::point p0(-32, 32);
    bn::point p1(32, 24);
    bn::point p2(32, -24);
    bn::point p3(-32, -32);
    triangle top_triangle(bn::sprite_items::texture, p2, p3, p0);
    triangle bottom_triangle(bn::sprite_items::texture, p0, p1, p2);
    bn::sprite_ptr p0_sprite = bn::sprite_items::bullet.create_sprite(p0);
    bn::sprite_ptr p1_sprite = bn::sprite_items::bullet.create_sprite(p1);
    bn::sprite_ptr p2_sprite = bn::sprite_items::bullet.create_sprite(p2, 1);
    bn::sprite_ptr p3_sprite = bn::sprite_items::bullet.create_sprite(p3);
    int p_index = 2;

    while(true)
    {
        if(bn::keypad::a_pressed())
        {
            switch(p_index)
            {

            case 0:
                p0_sprite.set_tiles(bn::sprite_items::bullet.tiles_item());
                p1_sprite.set_tiles(bn::sprite_items::bullet.tiles_item(), 1);
                ++p_index;
                break;

            case 1:
                p1_sprite.set_tiles(bn::sprite_items::bullet.tiles_item());
                p2_sprite.set_tiles(bn::sprite_items::bullet.tiles_item(), 1);
                ++p_index;
                break;

            case 2:
                p2_sprite.set_tiles(bn::sprite_items::bullet.tiles_item());
                p3_sprite.set_tiles(bn::sprite_items::bullet.tiles_item(), 1);
                ++p_index;
                break;

            default:
                p3_sprite.set_tiles(bn::sprite_items::bullet.tiles_item());
                p0_sprite.set_tiles(bn::sprite_items::bullet.tiles_item(), 1);
                p_index = 0;
                break;
            }
        }

        bn::point position_inc;

        if(bn::keypad::up_held())
        {
            position_inc.set_y(-1);
        }
        else if(bn::keypad::down_held())
        {
            position_inc.set_y(1);
        }

        if(bn::keypad::left_held())
        {
            position_inc.set_x(-1);
        }
        else if(bn::keypad::right_held())
        {
            position_inc.set_x(1);
        }

        if(position_inc != bn::point())
        {
            switch(p_index)
            {

            case 0:
                p0 += position_inc;
                top_triangle.set_p2(p0);
                bottom_triangle.set_p0(p0);
                p0_sprite.set_position(p0);
                break;

            case 1:
                p1 += position_inc;
                bottom_triangle.set_p1(p1);
                p1_sprite.set_position(p1);
                break;

            case 2:
                p2 += position_inc;
                top_triangle.set_p0(p2);
                bottom_triangle.set_p2(p2);
                p2_sprite.set_position(p2);
                break;

            default:
                p3 += position_inc;
                top_triangle.set_p1(p3);
                p3_sprite.set_position(p3);
                break;
            }
        }

        info.update();
        bn::core::update();
    }
}
