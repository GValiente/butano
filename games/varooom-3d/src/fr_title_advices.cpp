/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_title_advices.h"

#include "bn_display.h"

#include "fr_common_stuff.h"

namespace fr
{

namespace
{
    constexpr bn::fixed text_y = (bn::display::height() / 2) - 16;
    constexpr bn::fixed scale_inc = bn::fixed(1) / 8;

    constexpr bn::string_view text_items[] = {
        "DON'T FORGET YOUR HEADPHONES!",
        "USE TURBO BOOSTS! YOU ARE NOT GOING TO GET FAR WITHOUT THEM!",
        "YOU CAN USE TURBO BOOSTS EVEN WHEN THERE'S NO MORE TIME LEFT!",
        "INSTEAD OF BUMPING A RIVAL, STOP ACCELERATING UNTIL YOU KNOW YOU CAN OVERTAKE IT!",
        "DON'T WASTE A TURBO BOOST IF YOU KNOW A RIVAL IS NEAR. USE THE BOOST TO OVERTAKE IT!",
        "YOU CAN USE TURBO BOOSTS FOR OVERTAKINGS BY DRIVING OUTSIDE OF THE TRACK WITHOUT SLOWING DOWN TOO MUCH!",
        "IF YOU HAVE A TURBO BOOST YOU CAN BUMP ANOTHER CAR, TURN A BIT, FIRE THE BOOST AND OVERTAKE IT!",
        "IF YOU ARE FAST ENOUGH, YOU CAN WIN A DOLLAR!",
    };

    constexpr int text_items_count = sizeof(text_items) / sizeof(text_items[0]);
}

title_advices::title_advices(common_stuff& common_stuff) :
    _affine_mat(bn::sprite_affine_mat_ptr::create()),
    _item_index(common_stuff.storage.advice_index())
{
    if(_item_index < 0 || _item_index >= text_items_count)
    {
        _item_index = 0;
    }
}

void title_advices::set_visible(bool visible)
{
    if(visible)
    {
        _intro_done = false;
    }
    else
    {
        _outro_done = false;
    }
}

void title_advices::update(common_stuff& common_stuff)
{
    if(! _intro_done)
    {
        _update_intro();
    }
    else if(! _outro_done)
    {
        _update_outro();
    }

    if(_vertical_scale > 0)
    {
        for(message_type& message : _messages)
        {
            bn::ideque<bn::sprite_ptr>& sprites = message.sprites;

            for(bn::sprite_ptr& sprite : sprites)
            {
                sprite.set_x(sprite.x() - 1);
            }

            if(! sprites.empty() && sprites.front().x() < -(bn::display::width() / 2) - 32)
            {
                sprites.pop_front();
            }
        }

        if(_wait_frames)
        {
            --_wait_frames;
        }
        else
        {
            if(_messages.full())
            {
                _messages.pop_front();
            }

            _messages.push_back(message_type());

            message_type& message = _messages.back();
            bn::sprite_text_generator& text_generator = common_stuff.small_variable_text_generator;
            const bn::string_view& text_item = text_items[_item_index];
            bn::vector<bn::sprite_ptr, 32> sprites;
            bn::fixed text_x = bn::display::width() / 2;
            text_generator.generate(text_x, text_y, text_item, sprites);

            for(bn::sprite_ptr& sprite : sprites)
            {
                sprite.set_affine_mat(_affine_mat);
                message.sprites.push_back(bn::move(sprite));
            }

            _wait_frames = text_generator.width(text_item) + 64;
            _item_index = (_item_index + 1) % text_items_count;
            common_stuff.storage.set_advice_index(_item_index);
        }
    }
}

void title_advices::_set_sprites_visible(bool visible)
{
    for(message_type& message : _messages)
    {
        for(bn::sprite_ptr& sprite : message.sprites)
        {
            sprite.set_visible(visible);
        }
    }
}

void title_advices::_set_sprites_vertical_scale(bn::fixed vertical_scale)
{
    _vertical_scale = vertical_scale;

    if(vertical_scale > 0)
    {
        _affine_mat.set_vertical_scale(vertical_scale);
    }
}

void title_advices::_update_intro()
{
    bn::fixed vertical_scale = _vertical_scale;

    if(vertical_scale > 0)
    {
        vertical_scale += scale_inc;

        if(vertical_scale >= 1)
        {
            vertical_scale = 1;
            _intro_done = true;
        }
    }
    else
    {
        _set_sprites_visible(true);
        vertical_scale = scale_inc;
    }

    _set_sprites_vertical_scale(vertical_scale);
}

void title_advices::_update_outro()
{
    bn::fixed vertical_scale = _vertical_scale - scale_inc;

    if(vertical_scale < scale_inc)
    {
        _set_sprites_visible(false);
        vertical_scale = 0;
        _outro_done = true;
    }

    _set_sprites_vertical_scale(vertical_scale);
}

}
