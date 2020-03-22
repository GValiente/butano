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
    _clouds_bg_move_action(_create_clouds_bg(), -1.0 / 16, 0.5)
{
    btn::blending::set_transparency_alpha(blending_transparency);
    btn::window::internal().set_show_bg(_clouds_bg_move_action.bg(), false);
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

void game_background::reset_blending()
{
    btn::blending::set_transparency_alpha(0);
    _blending_action.emplace(20, blending_transparency);
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
}

}
