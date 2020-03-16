#include "bf_background.h"

#include "btn_blending.h"
#include "btn_regular_bg_builder.h"
#include "btn_clouds_bg_item.h"
#include "btn_stage_1_bg_item.h"

namespace bf
{

namespace
{
    btn::regular_bg_ptr _create_clouds_bg()
    {
        btn::regular_bg_builder builder(btn::bg_items::clouds);
        builder.set_priority(2);
        builder.set_blending_enabled(true);
        return builder.release_build();
    }
}

background::background() :
    _ground_bg_move_action(btn::regular_bg_ptr::create(0, 0, btn::bg_items::stage_1), 0, 0.5),
    _clouds_bg_move_action(_create_clouds_bg(), -1.0 / 16, 0.5)
{
    btn::blending::set_transparency_alpha(0.4);
}

void background::update()
{
    _ground_bg_move_action.update();
    _clouds_bg_move_action.update();
}

}
