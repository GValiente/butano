/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_scoreboard.h"

#include "bn_string.h"
#include "bn_display.h"
#include "bn_sprite_builder.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_items_experience_bar.h"
#include "bn_sprite_items_experience_frame_back.h"
#include "bn_sprite_items_experience_frame_front.h"
#include "bn_sprite_items_hero_bomb_icon.h"
#include "bf_game_hero.h"

namespace bf::game
{

namespace
{
    constexpr int level_text_x = 19 - (bn::display::width() / 2);
    constexpr int level_text_y = (bn::display::height() / 2) - 12;
    constexpr int experience_bar_x = (bn::display::width() / 2) - 22;
    constexpr int experience_bar_y = (bn::display::height() / 2) - 16;
    constexpr int experience_text_x = experience_bar_x - 8;
    constexpr int experience_text_y = (bn::display::height() / 2) - 32;

    void _set_visible(bool visible, bn::ivector<bn::sprite_ptr>& sprites)
    {
        for(bn::sprite_ptr& sprite : sprites)
        {
            sprite.set_visible(visible);
        }
    }
}

scoreboard::scoreboard(bn::sprite_text_generator& text_generator) :
    _text_generator(text_generator),
    _bombs_affine_mat(bn::sprite_affine_mat_ptr::create())
{
    _text_generator.set_center_alignment();
    _text_generator.generate(level_text_x, level_text_y - 12, "LVL", _level_label_sprites);
    _text_generator.generate(experience_text_x, experience_text_y - 12, "EXP", _experience_label_sprites);

    {
        bn::sprite_builder builder(bn::sprite_items::experience_frame_back);
        builder.set_position(bn::fixed_point(experience_bar_x - 8, experience_bar_y));
        builder.set_bg_priority(_text_generator.bg_priority());
        builder.set_z_order(_text_generator.z_order());
        _experience_bar_sprites.push_back(builder.release_build());
    }

    {
        bn::sprite_builder builder(bn::sprite_items::experience_bar);
        builder.set_position(_experience_bar_sprites[0].position());
        builder.set_bg_priority(_text_generator.bg_priority());
        builder.set_horizontal_scale(0.5);
        builder.set_z_order(_text_generator.z_order());
        _experience_bar_sprites.push_back(builder.release_build());
    }

    {
        bn::sprite_builder builder(bn::sprite_items::experience_frame_front);
        builder.set_position(bn::fixed_point(experience_bar_x - 2, experience_bar_y));
        builder.set_bg_priority(_text_generator.bg_priority());
        builder.set_z_order(_text_generator.z_order());
        builder.set_horizontal_flip(true);
        _experience_bar_sprites.push_back(builder.build());

        builder.set_position(bn::fixed_point(experience_bar_x - 13, experience_bar_y));
        builder.set_horizontal_flip(false);
        _experience_bar_sprites.push_back(builder.release_build());
    }

    _experience_bar_palette_action.emplace(_experience_bar_sprites[1].palette(), 2, 1);
}

void scoreboard::set_visible(bool visible)
{
    _set_visible(visible, _level_label_sprites);
    _set_visible(visible, _level_number_sprites);
    _set_visible(visible, _experience_label_sprites);
    _set_visible(visible, _experience_number_sprites);
    _set_visible(visible, _experience_bar_sprites);
    _set_visible(visible, _bomb_sprites);
}

void scoreboard::update(const hero& hero)
{
    int level = hero.level();
    int experience = hero.experience();
    int bombs_count = hero.bombs_count();
    bool visible = _level_label_sprites.front().visible();
    _experience_bar_palette_action->update();

    if(_bombs_affine_mat_scale_action)
    {
        _bombs_affine_mat_scale_action->update();

        if(_bombs_affine_mat_scale_action->done())
        {
            _bombs_affine_mat_scale_action.reset();

            if(_bomb_sprites.size() > _last_bombs_count)
            {
                _bomb_sprites.shrink(_last_bombs_count);
            }
            else
            {
                for(bn::sprite_ptr& bomb_sprite : _bomb_sprites)
                {
                    bomb_sprite.remove_affine_mat();
                }
            }
        }
    }

    if(level != _last_level)
    {
        _last_level = level;
        _last_experience = -1;

        bn::string<4> text;

        if(level < constants::hero_bullet_levels - 1)
        {
            text = bn::to_string<4>(level + 1);
        }
        else
        {
            text = "MAX";
        }

        _level_number_sprites.clear();
        _text_generator.set_center_alignment();
        _text_generator.generate(level_text_x, level_text_y, text, _level_number_sprites);
        _set_visible(visible, _level_number_sprites);
    }

    if(experience != _last_experience)
    {
        _last_experience = experience;

        bn::sprite_ptr& experience_bar_sprite = _experience_bar_sprites[1];
        bn::fixed next_level_experience_ratio = hero.next_level_experience_ratio();

        if(next_level_experience_ratio < 1)
        {
            next_level_experience_ratio *= bn::fixed(0.935);
        }

        if(next_level_experience_ratio == 0)
        {
            experience_bar_sprite.set_visible(false);
        }
        else
        {
            experience_bar_sprite.set_horizontal_scale(next_level_experience_ratio);
            experience_bar_sprite.set_visible(true);
        }

        bn::string<8> text = bn::to_string<8>(experience);
        _experience_number_sprites.clear();
        _text_generator.set_center_alignment();
        _text_generator.generate(experience_text_x, experience_text_y, text, _experience_number_sprites);
        _set_visible(visible, _experience_number_sprites);
    }

    if(bombs_count != _last_bombs_count)
    {
        int last_bombs_count = _last_bombs_count;
        _last_bombs_count = bombs_count;
        _bomb_sprites.clear();

        for(int index = 0, limit = bn::max(bombs_count, last_bombs_count); index < limit; ++index)
        {
            bn::sprite_builder builder(bn::sprite_items::hero_bomb_icon);
            builder.set_x((bn::display::width() / 2) - ((index + 1) * 16) + 1);
            builder.set_y(16 - (bn::display::height() / 2));
            builder.set_bg_priority(_text_generator.bg_priority());
            builder.set_z_order(_text_generator.z_order());
            builder.set_visible(visible);
            _bomb_sprites.push_back(builder.release_build());
        }

        if(bombs_count > last_bombs_count)
        {
            if(last_bombs_count != -1)
            {
                _bombs_affine_mat.set_scale(0.01);
                _bombs_affine_mat_scale_action.emplace(_bombs_affine_mat, 16, 1);

                for(int index = last_bombs_count; index < bombs_count; ++index)
                {
                    _bomb_sprites[index].set_affine_mat(_bombs_affine_mat);
                }
            }
        }
        else
        {
            _bombs_affine_mat.set_scale(1);
            _bombs_affine_mat_scale_action.emplace(_bombs_affine_mat, 16, bn::fixed(0.01));

            for(int index = bombs_count; index < last_bombs_count; ++index)
            {
                _bomb_sprites[index].set_affine_mat(_bombs_affine_mat);
            }
        }
    }
}

}
