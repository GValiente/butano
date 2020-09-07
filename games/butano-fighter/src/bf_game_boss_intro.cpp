#include "bf_game_boss_intro.h"

#include "btn_colors.h"
#include "btn_display.h"
#include "btn_bg_palettes.h"
#include "btn_music_items.h"
#include "btn_sound_items.h"
#include "btn_sprite_builder.h"
#include "btn_sprite_palettes.h"
#include "btn_sprite_items_boss_intro.h"
#include "btn_sprite_affine_mat_attributes.h"

namespace bf::game
{

namespace
{
    constexpr const int amplitude = 32;
    constexpr const bool skip = false;

    [[nodiscard]] btn::sprite_ptr _create_sprite(int x, int graphics_index,
                                                 const btn::sprite_affine_mat_ptr& affine_mat)
    {
        btn::sprite_builder builder(btn::sprite_items::boss_intro, graphics_index);
        builder.set_x(x);
        builder.set_affine_mat(affine_mat);
        builder.set_bg_priority(2);
        builder.set_ignore_camera(true);
        return builder.release_build();
    }

    [[nodiscard]] constexpr btn::array<btn::sprite_affine_mat_attributes, btn::display::height()>
    _create_hblank_effect_attributes()
    {
        btn::sprite_affine_mat_attributes base_attributes;
        base_attributes.set_scale(0.001);

        btn::array<btn::sprite_affine_mat_attributes, btn::display::height()> result;
        result.fill(base_attributes);

        btn::fixed scale = 1;
        btn::fixed scale_step = 0.002;
        btn::fixed scale_dec = 0;

        for(int index = 0; index < amplitude; ++index)
        {
            int base_index = btn::display::height() / 2;
            result[base_index - index].set_scale(scale);
            result[base_index + index + 1].set_scale(scale);
            scale_dec += scale_step;
            scale -= scale_dec;
        }

        return result;
    }

    constexpr const btn::array<btn::sprite_affine_mat_attributes, btn::display::height()> _hblank_effect_attributes =
            _create_hblank_effect_attributes();
}

void boss_intro::enable()
{
    btn::sprite_affine_mat_ptr affine_mat = btn::sprite_affine_mat_ptr::create();
    int x = -84;
    _sprites.push_back(_create_sprite(x, 0, affine_mat));
    x += 31;
    _sprites.push_back(_create_sprite(x, 1, affine_mat));
    x += 30;
    _sprites.push_back(_create_sprite(x, 2, affine_mat));
    x += 30;
    _sprites.push_back(_create_sprite(x, 3, affine_mat));
    x += 24;
    _sprites.push_back(_create_sprite(x, 4, affine_mat));
    x += 24;
    _sprites.push_back(_create_sprite(x, 5, affine_mat));
    x += 30;
    _sprites.push_back(_create_sprite(x, 6, affine_mat));
    _music_volume_action.emplace(amplitude * 2, 0);
    _hblank_effect = btn::sprite_affine_mat_attributes_hblank_effect_ptr::create(
                btn::move(affine_mat), _hblank_effect_attributes);

    _state = state::ACTIVE;
    _loops = 5;
    _counter = 1;
}

void boss_intro::update()
{
    switch(_state)
    {

    case state::INIT:
        break;

    case state::ACTIVE:
        --_counter;

        if(! _counter)
        {
            --_loops;

            if(_loops && ! skip)
            {
                btn::sound_items::space_shooter_7.play();
                _counter = amplitude * 2;
            }
            else
            {
                btn::bg_palettes::set_contrast(0);
                btn::sprite_palettes::set_contrast(0);
                _sprites.clear();
                _hblank_effect.reset();
                btn::music_items::minor_boss_r.play(0.4);
                _loops = 0;
                _state = state::DONE;
            }
        }

        if(_counter)
        {
            btn::fixed y = _counter - amplitude;

            for(btn::sprite_ptr& sprite : _sprites)
            {
                sprite.set_y(y);
            }

            btn::fixed contrast = (amplitude - btn::abs(y)) / amplitude;
            btn::bg_palettes::set_contrast(contrast);
            btn::sprite_palettes::set_contrast(contrast);
        }

        if(_music_volume_action)
        {
            _music_volume_action->update();

            if(_music_volume_action->done())
            {
                _music_volume_action.reset();
            }
        }
        break;

    case state::DONE:
        break;

    default:
        BTN_ERROR("Invalid state: ", int(_state));
        break;
    }
}

}
