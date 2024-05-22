/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_butano_intro_scene.h"

#include "bn_string.h"
#include "bn_keypad.h"
#include "bn_version.h"

#include "fr_scene_type.h"
#include "fr_common_stuff.h"

#include "bn_affine_bg_items_butano_backdrop.h"
#include "bn_affine_bg_items_butano_background.h"

#include "models/fr_model_3d_items_butano.h"

namespace fr
{

namespace
{
    constexpr bn::color fade_color(14, 0, 0);

    constexpr bn::color model_colors[] = {
        bn::color(10, 10, 10), bn::color(6, 6, 6), bn::color(23, 11, 6), bn::color(23, 9, 3),
    };

    [[nodiscard]] bn::bg_palettes_fade_to_action _create_bgs_fade_in_action()
    {
        bn::bg_palettes::remove_transparent_color();
        bn::bg_palettes::set_fade(fade_color, 1);
        return bn::bg_palettes_fade_to_action(60, 0);
    }

    [[nodiscard]] bn::sprite_palettes_fade_to_action _create_sprites_fade_in_action()
    {
        bn::sprite_palettes::set_fade(fade_color, 1);
        return bn::sprite_palettes_fade_to_action(60, 0);
    }
}

butano_intro_scene::butano_intro_scene(common_stuff& common_stuff) :
    _bgs_fade_in_action(_create_bgs_fade_in_action()),
    _sprites_fade_in_action(_create_sprites_fade_in_action()),
    _backdrop_bg(bn::affine_bg_items::butano_backdrop.create_bg(0, 0)),
    _background_bg(bn::affine_bg_items::butano_background.create_bg(0, 0)),
    _background_move_action(_background_bg, 1, -1)
{
    bn::string<32> middle_text("Butano ");
    middle_text.append(bn::to_string<4>(bn::version::major()));
    middle_text.append('.');
    middle_text.append(bn::to_string<4>(bn::version::minor()));
    middle_text.append('.');
    middle_text.append(bn::to_string<4>(bn::version::patch()));

    bn::sprite_text_generator& big_text_generator = common_stuff.big_text_generator;
    big_text_generator.set_center_alignment();
    big_text_generator.generate(0, -16, "Made with", _text_sprites);
    big_text_generator.generate(0, 0, middle_text, _text_sprites);
    big_text_generator.generate(0, 16, "github.com/GValiente/butano", _text_sprites);
    big_text_generator.set_left_alignment();

    _camera.set_position(point_3d(0, 128 + 16, 0));
    _models.load_colors(model_colors);
    _butano_model = &_models.create_dynamic_model(model_3d_items::butano);
    _butano_model->set_phi(0);
    _butano_model->set_theta(0);
    _butano_model->set_psi(32768 + 16384);
}

bn::optional<scene_type> butano_intro_scene::update()
{
    bn::optional<scene_type> result;

    if(_bgs_fade_in_action.done())
    {
        if(_counter)
        {
            if(bn::keypad::a_pressed())
            {
                _counter = 0;
            }
            else
            {
                --_counter;
            }
        }
        else
        {
            if(_bgs_fade_out_action)
            {
                if(_bgs_fade_out_action->done())
                {
                    if(_butano_model)
                    {
                        _text_sprites.clear();
                        _models.destroy_dynamic_model(*_butano_model);
                        _butano_model = nullptr;
                    }
                    else
                    {
                        result = scene_type::JAM_INTRO;
                    }
                }
                else
                {
                    _bgs_fade_out_action->update();
                    _sprites_fade_out_action->update();
                }
            }
            else
            {
                _bgs_fade_out_action.emplace(60, 1);
                _sprites_fade_out_action.emplace(60, 1);
            }
        }
    }
    else
    {
        _bgs_fade_in_action.update();
        _sprites_fade_in_action.update();
    }

    if(_butano_model)
    {
        _butano_model->set_phi(_butano_model->phi() - 128);
        _butano_model->set_theta(_butano_model->theta() + 128);
    }

    _background_move_action.update();
    _models.update(_camera);

    return result;
}

}
