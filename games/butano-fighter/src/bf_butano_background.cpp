#include "bf_butano_background.h"

#include "btn_colors.h"
#include "btn_bg_palettes.h"
#include "btn_inside_window.h"
#include "btn_butano_background_bg_item.h"

namespace bf
{

butano_background::butano_background() :
    _move_action(btn::regular_bg_move_by_action(
                     btn::regular_bg_ptr::create(0, 0, btn::bg_items::butano_background), 1, -1))
{
    btn::bg_palettes::set_transparent_color(btn::colors::maroon);
    btn::window::sprites().set_show_bg(_move_action->bg(), false);
}

void butano_background::update()
{
    if(_move_action)
    {
        _move_action->update();
    }
}

}
