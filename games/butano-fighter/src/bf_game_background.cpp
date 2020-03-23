#include "bf_game_background.h"

#include "btn_blending.h"
#include "btn_rect_window.h"
#include "btn_regular_bg_builder.h"
#include "btn_clouds_bg_item.h"
#include "btn_stage_1_bg_item.h"

namespace bf
{

namespace
{
    constexpr const btn::fixed blending_transparency = 0.4;
    constexpr const int hblank_effect_multiplier = 32;

    btn::regular_bg_ptr _create_ground_bg()
    {
        btn::regular_bg_builder builder(btn::bg_items::stage_1);
        builder.set_mosaic_enabled(true);
        return builder.release_build();
    }

    btn::regular_bg_ptr _create_clouds_bg()
    {
        btn::regular_bg_builder builder(btn::bg_items::clouds);
        builder.set_priority(2);
        builder.set_blending_enabled(true);
        builder.set_mosaic_enabled(true);
        return builder.release_build();
    }
}

game_background::game_background() :
    _ground_bg_move_action(_create_ground_bg(), 0, 0.5),
    _clouds_bg_move_action(_create_clouds_bg(), -1.0 / 16, 0.5),
    _hblank_effect(btn::regular_bg_position_hblank_effect_ptr::create_horizontal(
                       _ground_bg_move_action.bg(), _hblank_effect_deltas))
{
    btn::blending::set_transparency_alpha(blending_transparency);
    btn::window::internal().set_show_bg(_clouds_bg_move_action.bg(), false);
    _wave_generator.set_amplitude(3);
    _hblank_effect.set_visible(false);
}

void game_background::show_mosaic()
{
    btn::bgs_mosaic::set_stretch(0.25);
    _mosaic_action = btn::bgs_mosaic_stretch_loop_action(4, 0.5);
}

void game_background::hide_mosaic()
{
    btn::bgs_mosaic::set_stretch(0);
    _mosaic_action.reset();
}

void game_background::show_blending()
{
    hide_blending();
    _blending_action.emplace(20, blending_transparency);
}

void game_background::hide_blending()
{
    btn::blending::set_transparency_alpha(0);
    _blending_action.reset();
}

void game_background::show_hblank_effect(int frames)
{
    BTN_ASSERT(frames > 0, "Invalid frames: ", frames);

    _hblank_effect.set_visible(true);
    _hblank_effect_frames = frames;
}

void game_background::update()
{
    _ground_bg_move_action.update();
    _clouds_bg_move_action.update();

    if(_mosaic_action)
    {
        _mosaic_action->update();
    }

    if(_blending_action)
    {
        _blending_action->update();

        if(_blending_action->done())
        {
            _blending_action.reset();
        }
    }

    if(_hblank_effect_frames)
    {
        --_hblank_effect_frames;

        if(_hblank_effect_frames)
        {
            _wave_generator.set_speed(_hblank_effect_frames * hblank_effect_multiplier);
            _wave_generator.generate(_hblank_effect_deltas);
            _hblank_effect.reload_deltas_ref();
        }
        else
        {
            _hblank_effect.set_visible(false);
        }
    }
}

}
