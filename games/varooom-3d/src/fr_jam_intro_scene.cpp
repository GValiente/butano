/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_jam_intro_scene.h"

#include "bn_keypad.h"
#include "bn_colors.h"

#include "fr_scene_type.h"

#include "bn_regular_bg_items_jam_backdrop.h"

#include "models/fr_model_3d_items_jam_logo.h"

namespace fr
{

namespace
{
    constexpr bn::color model_colors[] = {
        bn::color(31, 31, 31), bn::color(13, 11, 20),
    };
}

jam_intro_scene::jam_intro_scene() :
    _bgs_fade_in_action(bn::bg_palettes_fade_to_action(60, 0)),
    _sprites_fade_in_action(bn::sprite_palettes_fade_to_action(60, 0)),
    _backdrop_bg(bn::regular_bg_items::jam_backdrop.create_bg(
                     (256 - bn::display::width()) / 2, (256 - bn::display::height()) / 2))
{
    _camera.set_position(point_3d(0, 66, 7));
    _models.load_colors(model_colors);
    _model = &_models.create_dynamic_model(model_3d_items::jam_logo);
    _model->set_phi(16384);
}

bn::optional<scene_type> jam_intro_scene::update()
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
                    if(_model)
                    {
                        _models.destroy_dynamic_model(*_model);
                        _model = nullptr;
                    }
                    else
                    {
                        result = scene_type::TITLE;
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
                bn::bg_palettes::set_fade_color(bn::colors::black);
                bn::sprite_palettes::set_fade_color(bn::colors::black);
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

    if(_model)
    {
        _model->set_phi(_model->phi() - 92);
    }

    _models.update(_camera);

    return result;
}

}
