#include "bf_title.h"

#include "btn_string.h"
#include "btn_keypad.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_blending.h"
#include "btn_music_items.h"
#include "btn_sound_items.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_text_generator.h"
#include "btn_sprite_items_hero_head.h"
#include "btn_sprite_first_attributes.h"
#include "btn_sprite_items_butano_font.h"
#include "btn_sprite_items_fighter_font.h"
#include "btn_sprite_affine_mat_attributes.h"
#include "btn_sprite_items_butano_big_sprite.h"
#include "bf_scene_type.h"
#include "bf_game_status.h"
#include "bf_wave_generator.h"
#include "bf_butano_background.h"

namespace bf
{

namespace
{
    [[nodiscard]] btn::sprite_ptr _create_butano_up_sprite()
    {
        btn::sprite_builder builder(btn::sprite_items::butano_big_sprite, 0);
        builder.set_y((btn::display::height() / 2) + 32);
        builder.set_blending_enabled(true);
        return builder.release_build();
    }

    [[nodiscard]] btn::sprite_ptr _create_butano_down_sprite()
    {
        btn::sprite_builder builder(btn::sprite_items::butano_big_sprite, 1);
        builder.set_y((btn::display::height() / 2) + 32 + 64);
        builder.set_blending_enabled(true);
        return builder.release_build();
    }

    [[nodiscard]] btn::vector<btn::sprite_ptr, 6> _create_butano_characters()
    {
        btn::vector<btn::sprite_ptr, 6> result;

        for(int index = 0; index < 6; ++index)
        {
            result.push_back(btn::sprite_items::butano_font.create_sprite((index * 20) - 50, -28, index));
            result.back().set_visible(false);
        }

        return result;
    }

    [[nodiscard]] btn::vector<btn::sprite_ptr, 7> _create_fighter_characters()
    {
        btn::vector<btn::sprite_ptr, 7> result;
        btn::fixed fighter_x = 78;

        for(int index = 6; index >= 0; --index)
        {
            result.push_back(btn::sprite_items::fighter_font.create_sprite(fighter_x, 4, index));
            result.back().set_visible(false);
            fighter_x -= index <= 2 ? 21 : 29;
        }

        return result;
    }

    [[nodiscard]] btn::sprite_regular_second_attributes_hblank_effect_ptr _create_x_hblank_effect(
        const btn::sprite_ptr& sprite, const btn::ivector<btn::sprite_regular_second_attributes>& attributes)
    {
        btn::span<const btn::sprite_regular_second_attributes> span(attributes.data(), attributes.size());
        return btn::sprite_regular_second_attributes_hblank_effect_ptr::create(sprite, span);
    }

    [[nodiscard]] constexpr btn::array<btn::sprite_first_attributes, btn::display::height()>
    _create_butano_character_hblank_effect_attributes()
    {
        btn::array<btn::sprite_first_attributes, btn::display::height()> result;
        int start = 64;

        for(int index = start; index < btn::display::height(); ++index)
        {
            result[index].set_y(index - start);
        }

        return result;
    }

    constexpr const btn::array<btn::sprite_first_attributes, btn::display::height()> butano_character_hblank_effect_attributes =
            _create_butano_character_hblank_effect_attributes();

    [[nodiscard]] constexpr btn::array<btn::sprite_affine_mat_attributes, btn::display::height()>
    _create_fighter_character_hblank_effect_attributes()
    {
        btn::array<btn::sprite_affine_mat_attributes, btn::display::height()> result;
        int start = 90;

        for(int index = start; index < btn::display::height(); ++index)
        {
            result[index].set_scale(1 + (btn::fixed(index - start) / 112));
            result[index].set_rotation_angle(index - start);
        }

        return result;
    }

    constexpr const btn::array<btn::sprite_affine_mat_attributes, btn::display::height()> fighter_character_hblank_effect_attributes =
            _create_fighter_character_hblank_effect_attributes();
}

title::title(const game::status& game_status, btn::sprite_text_generator& text_generator,
             butano_background& butano_background) :
    _butano_up_sprite(_create_butano_up_sprite()),
    _butano_down_sprite(_create_butano_down_sprite()),
    _butano_characters(_create_butano_characters()),
    _fighter_characters(_create_fighter_characters()),
    _cursor_sprite(btn::sprite_items::hero_head.create_sprite(0, 0)),
    _butano_x_hblank_effect_attributes(btn::display::height(), _butano_up_sprite.regular_second_attributes()),
    _butano_up_x_hblank_effect(_create_x_hblank_effect(_butano_up_sprite, _butano_x_hblank_effect_attributes)),
    _butano_down_x_hblank_effect(_create_x_hblank_effect(_butano_down_sprite, _butano_x_hblank_effect_attributes))
{
    btn::string<20> high_experience_text("HIGH EXP: ");
    high_experience_text.append(btn::to_string<8>(game_status.high_experience()));

    btn::horizontal_alignment_type old_alignment = text_generator.alignment();
    text_generator.set_alignment(btn::horizontal_alignment_type::CENTER);
    text_generator.generate(0, 12 - (btn::display::height() / 2), high_experience_text, _high_experience_text_sprites);
    text_generator.generate(0, 44, "START", _start_text_sprites);
    text_generator.generate(0, 44 + 12, "CREDITS", _credits_text_sprites);
    text_generator.set_alignment(old_alignment);
    _cursor_sprite.set_position(_credits_text_sprites[0].x() - 28, _start_text_sprites[0].y());
    _cursor_sprite.set_visible(false);

    for(btn::sprite_ptr& sprite : _high_experience_text_sprites)
    {
        sprite.set_visible(false);
    }

    for(btn::sprite_ptr& sprite : _start_text_sprites)
    {
        sprite.set_visible(false);
    }

    for(btn::sprite_ptr& sprite : _credits_text_sprites)
    {
        sprite.set_visible(false);
    }

    butano_background.put_under_all();
    btn::music_items::battle_clean.play(0.6);
}

btn::optional<scene_type> title::update()
{
    _animate_butano_x();
    _animate_butano_y();
    _animate_butano_characters();
    _animate_fighter_characters();
    return _menu();
}

void title::_animate_butano_x()
{
    if(_butano_x_hblank_effect_speed)
    {
        wave_generator generator;
        generator.set_speed(_butano_x_hblank_effect_speed);

        if(_butano_x_hblank_effect_speed > 32)
        {
            generator.set_amplitude(2);
            _butano_x_hblank_effect_speed -= 32;
        }
        else
        {
            generator.set_amplitude(1);
            _butano_x_hblank_effect_speed /= 2;
        }

        btn::fixed values[btn::display::height()];
        generator.generate(values);

        for(int index = 0; index < btn::display::height(); ++index)
        {
            _butano_x_hblank_effect_attributes[index].set_x(values[index]);
        }

        _butano_up_x_hblank_effect->reload_attributes_ref();
        _butano_down_x_hblank_effect->reload_attributes_ref();
    }
    else
    {
        if(_butano_up_x_hblank_effect)
        {
            _butano_up_x_hblank_effect.reset();
            _butano_down_x_hblank_effect.reset();
        }
    }
}

void title::_animate_butano_y()
{
    if(_butano_y_up)
    {
        _butano_y_inc += 0.05;

        if(_butano_y_inc >= 1)
        {
            _butano_y_inc = 1;
            _butano_y_up = false;
        }
        else
        {
            if(_butano_y_inc >= 0 && _state == state::START)
            {
                _state = state::BUTANO_CHARACTERS_SHOW;
            }
        }
    }
    else
    {
        _butano_y_inc -= 0.05;

        if(_butano_y_inc <= -1)
        {
            _butano_y_inc = -1;
            _butano_y_up = true;
        }
    }

    btn::fixed butano_up_sprite_y = _butano_up_sprite.y() + _butano_y_inc;
    _butano_up_sprite.set_y(butano_up_sprite_y);
    _butano_down_sprite.set_y(butano_up_sprite_y + 64);

    if(int(_state) <= int(state::MENU))
    {
        if(butano_up_sprite_y <= -26)
        {
            btn::fixed blending_intensity = -butano_up_sprite_y - 26;
            btn::blending::set_intensity_alpha(blending_intensity / 40);
        }
        else
        {
            btn::blending::set_intensity_alpha(0);
        }
    }
}

void title::_animate_butano_characters()
{
    if(_state != state::BUTANO_CHARACTERS_SHOW)
    {
        return;
    }

    if(_butano_character_move_action)
    {
        _butano_character_move_action->update();

        if(_butano_character_move_action->done())
        {
            _butano_character_move_action.reset();
            _butano_character_hblank_effect.reset();
        }
    }
    else
    {
        for(btn::sprite_ptr& butano_character : _butano_characters)
        {
            if(! butano_character.visible())
            {
                btn::fixed_point position = butano_character.position();
                butano_character.set_y(0);
                butano_character.set_visible(true);
                _butano_character_move_action.emplace(butano_character, 12, position);
                _butano_character_hblank_effect = btn::sprite_first_attributes_hblank_effect_ptr::create(
                            butano_character, butano_character_hblank_effect_attributes);
                return;
            }
        }

        _state = state::FIGHTER_CHARACTERS_SHOW;
    }
}

void title::_animate_fighter_characters()
{
    if(_state != state::FIGHTER_CHARACTERS_SHOW)
    {
        return;
    }

    if(_fighter_character_move_action)
    {
        _fighter_character_move_action->update();

        if(_fighter_character_move_action->done())
        {
            _fighter_character_move_action.reset();
            _fighter_character_hblank_effect.reset();

            for(btn::sprite_ptr& fighter_character : _fighter_characters)
            {
                if(fighter_character.affine_mat())
                {
                    fighter_character.set_affine_mat(btn::nullopt);
                }
            }
        }
    }
    else
    {
        for(btn::sprite_ptr& fighter_character : _fighter_characters)
        {
            if(! fighter_character.visible())
            {
                btn::fixed_point position = fighter_character.position();
                fighter_character.set_y((btn::display::height() + 32) / 2);
                fighter_character.set_visible(true);
                _fighter_character_move_action.emplace(fighter_character, 16, position);

                btn::sprite_affine_mat_ptr affine_mat = btn::sprite_affine_mat_ptr::create();
                fighter_character.set_affine_mat(affine_mat);
                fighter_character.set_double_size_mode(btn::sprite_double_size_mode::ENABLED);
                _fighter_character_hblank_effect = btn::sprite_affine_mat_attributes_hblank_effect_ptr::create(
                            affine_mat, fighter_character_hblank_effect_attributes);
                return;
            }
        }

        _state = state::MENU;
    }
}

btn::optional<scene_type> title::_menu()
{
    const int cursor_scale_frames = 20;
    const int sprites_hide_frames = 60;
    btn::optional<scene_type> result;

    if(_cursor_move_action)
    {
        _cursor_move_action->update();

        if(_cursor_move_action->done())
        {
            _cursor_move_action.reset();
        }
    }
    else if(_state == state::MENU)
    {
        if(! _cursor_sprite.visible())
        {
            _cursor_sprite.set_visible(true);
            _cursor_sprite.set_scale(0.01);
            _cursor_scale_action.emplace(_cursor_sprite, cursor_scale_frames, 1);

            for(btn::sprite_ptr& sprite : _high_experience_text_sprites)
            {
                sprite.set_visible(true);
            }

            for(btn::sprite_ptr& sprite : _start_text_sprites)
            {
                sprite.set_visible(true);
            }

            for(btn::sprite_ptr& sprite : _credits_text_sprites)
            {
                sprite.set_visible(true);
            }
        }

        if(btn::keypad::pressed(btn::keypad::key_type::A))
        {
            if(btn::blending::intensity_alpha() > 0)
            {
                _blending_intensity_action.emplace(cursor_scale_frames, 0);
            }

            _cursor_scale_action.emplace(_cursor_sprite, cursor_scale_frames, 0.01);
            _music_volume_action.emplace(cursor_scale_frames + sprites_hide_frames, 0);

            if(_start_selected)
            {
                btn::sound_items::start.play();
            }
            else
            {
                btn::sound_items::cure.play();
            }

            _state = state::HIDE_CURSOR;
        }
        else if(btn::keypad::pressed(btn::keypad::key_type::UP) || btn::keypad::pressed(btn::keypad::key_type::DOWN))
        {
            if(_start_selected)
            {
                _cursor_move_action.emplace(_cursor_sprite, 3, _cursor_sprite.x(), _credits_text_sprites[0].y());
            }
            else
            {
                _cursor_move_action.emplace(_cursor_sprite, 3, _cursor_sprite.x(), _start_text_sprites[0].y());
            }

            _start_selected = ! _start_selected;
            btn::sound_items::alert.play();
        }
    }

    if(_cursor_scale_action)
    {
        _cursor_scale_action->update();

        if(_cursor_scale_action->done())
        {
            _cursor_scale_action.reset();

            if(_state == state::HIDE_CURSOR)
            {
                for(btn::sprite_ptr& sprite : _butano_characters)
                {
                    sprite.set_blending_enabled(true);
                }

                for(btn::sprite_ptr& sprite : _fighter_characters)
                {
                    sprite.set_blending_enabled(true);
                }

                for(btn::sprite_ptr& sprite : _high_experience_text_sprites)
                {
                    sprite.set_blending_enabled(true);
                }

                for(btn::sprite_ptr& sprite : _start_text_sprites)
                {
                    sprite.set_blending_enabled(true);
                }

                for(btn::sprite_ptr& sprite : _credits_text_sprites)
                {
                    sprite.set_blending_enabled(true);
                }

                _cursor_sprite.set_visible(false);
                _blending_transparency_action.emplace(sprites_hide_frames, 0);
                _state = state::HIDE_SPRITES;
            }
        }
    }

    if(_music_volume_action)
    {
        _music_volume_action->update();

        if(_music_volume_action->done())
        {
            _music_volume_action.reset();
        }
    }

    if(_blending_intensity_action)
    {
        _blending_intensity_action->update();

        if(_blending_intensity_action->done())
        {
            _blending_intensity_action.reset();
        }
    }

    if(_blending_transparency_action)
    {
        _blending_transparency_action->update();

        if(_blending_transparency_action->done())
        {
            _blending_transparency_action.reset();
            _butano_up_sprite.set_visible(false);
            _butano_down_sprite.set_visible(false);
            _butano_characters.clear();
            _fighter_characters.clear();
            _high_experience_text_sprites.clear();
            _start_text_sprites.clear();
            _credits_text_sprites.clear();
            btn::blending::set_transparency_alpha(1);
            result = scene_type::GAME;
        }
    }

    return result;
}

}
