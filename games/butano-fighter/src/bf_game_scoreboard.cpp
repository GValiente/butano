#include "bf_game_scoreboard.h"

#include "btn_string.h"
#include "btn_display.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_text_generator.h"
#include "btn_sprite_items_experience_bar.h"
#include "btn_sprite_items_experience_frame_back.h"
#include "btn_sprite_items_experience_frame_front.h"
#include "btn_sprite_items_hero_bomb_icon.h"
#include "bf_game_hero.h"

namespace bf::game
{

namespace
{
    constexpr const int text_y = (btn::display::height() / 2) - 16;

    void _set_visible(bool visible, btn::ivector<btn::sprite_ptr>& sprites)
    {
        for(btn::sprite_ptr& sprite : sprites)
        {
            sprite.set_visible(visible);
        }
    }
}

scoreboard::scoreboard(btn::sprite_text_generator& text_generator) :
    _text_generator(text_generator),
    _bombs_affine_mat(btn::sprite_affine_mat_ptr::create())
{
    int experience_bar_x = (btn::display::width() / 2) - 22;
    _text_generator.generate(8 - (btn::display::width() / 2), text_y, "LVL:", _level_label_sprites);
    _text_generator.generate(0, text_y, "EXP:", _experience_label_sprites);

    {
        btn::sprite_builder builder(btn::sprite_items::experience_frame_back);
        builder.set_position(btn::fixed_point(experience_bar_x - 8, text_y));
        builder.set_bg_priority(_text_generator.bg_priority());
        builder.set_z_order(_text_generator.z_order());
        builder.set_ignore_camera(true);
        _experience_bar_sprites.push_back(builder.release_build());
    }

    {
        btn::sprite_builder builder(btn::sprite_items::experience_bar);
        builder.set_position(_experience_bar_sprites[0].position());
        builder.set_bg_priority(_text_generator.bg_priority());
        builder.set_scale_x(0.5);
        builder.set_z_order(_text_generator.z_order());
        builder.set_ignore_camera(true);
        _experience_bar_sprites.push_back(builder.release_build());
    }

    {
        btn::sprite_builder builder(btn::sprite_items::experience_frame_front);
        builder.set_position(btn::fixed_point(experience_bar_x - 2, text_y));
        builder.set_bg_priority(_text_generator.bg_priority());
        builder.set_z_order(_text_generator.z_order());
        builder.set_ignore_camera(true);
        builder.set_horizontal_flip(true);
        _experience_bar_sprites.push_back(builder.build());

        builder.set_position(btn::fixed_point(experience_bar_x - 13, text_y));
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
                for(btn::sprite_ptr& bomb_sprite : _bomb_sprites)
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

        btn::string<2> text = btn::to_string<2>(level + 1);
        _level_number_sprites.clear();
        _text_generator.generate(39 - (btn::display::width() / 2), text_y, text, _level_number_sprites);
        _set_visible(visible, _level_number_sprites);
    }

    if(experience != _last_experience)
    {
        _last_experience = experience;

        btn::sprite_ptr& experience_bar_sprite = _experience_bar_sprites[1];
        btn::fixed next_level_experience_ratio = hero.next_level_experience_ratio();

        if(next_level_experience_ratio < 1)
        {
            next_level_experience_ratio *= btn::fixed(0.935);
        }

        if(next_level_experience_ratio == 0)
        {
            experience_bar_sprite.set_visible(false);
        }
        else
        {
            experience_bar_sprite.set_scale_x(next_level_experience_ratio);
            experience_bar_sprite.set_visible(true);
        }

        btn::string<8> text = btn::to_string<8>(experience);
        _text_generator.set_alignment(btn::horizontal_alignment_type::RIGHT);
        _experience_number_sprites.clear();
        _text_generator.generate(_experience_bar_sprites[2].x() - 36, text_y, text, _experience_number_sprites);
        _experience_label_sprites[0].set_x(_experience_number_sprites[0].x() - 30);
        _set_visible(visible, _experience_number_sprites);
        _text_generator.set_alignment(btn::horizontal_alignment_type::LEFT);
    }

    if(bombs_count != _last_bombs_count)
    {
        int last_bombs_count = _last_bombs_count;
        _last_bombs_count = bombs_count;
        _bomb_sprites.clear();

        for(int index = 0, limit = btn::max(bombs_count, last_bombs_count); index < limit; ++index)
        {
            btn::sprite_builder builder(btn::sprite_items::hero_bomb_icon);
            builder.set_x((btn::display::width() / 2) - ((index + 1) * 16));
            builder.set_y(16 - (btn::display::height() / 2));
            builder.set_bg_priority(_text_generator.bg_priority());
            builder.set_z_order(_text_generator.z_order());
            builder.set_visible(visible);
            builder.set_ignore_camera(true);
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
            _bombs_affine_mat_scale_action.emplace(_bombs_affine_mat, 16, 0.01);

            for(int index = bombs_count; index < last_bombs_count; ++index)
            {
                _bomb_sprites[index].set_affine_mat(_bombs_affine_mat);
            }
        }
    }
}

}
