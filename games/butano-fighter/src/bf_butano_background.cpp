#include "bf_butano_background.h"

#include "btn_inside_window.h"
#include "btn_bg_items_butano_background.h"

namespace bf
{

butano_background::butano_background() :
    _move_action(btn::regular_bg_move_by_action(
                     btn::regular_bg_ptr::create(0, 0, btn::bg_items::butano_background), 1, -1)),
    _palette_hblank_effect(btn::bg_palettes_transparent_color_hblank_effect_ptr::create(_palette_hblank_effect_colors))
{
    btn::window::sprites().set_show_bg(_move_action->bg(), false);
    btn::color color(14, 0, 0);
    int redInc = 32;
    int greenInc = 16;
    int blueInc = 8;

    for(int index = 0; index < 160; ++index)
    {
        _palette_hblank_effect_colors[index] = color;
        --redInc;
        --greenInc;
        --blueInc;

        if(! redInc)
        {
            color.set_red(color.red() + 1);
            redInc = 32;
        }

        if(! greenInc)
        {
            color.set_green(color.green() + 1);
            greenInc = 16;
        }

        if(! blueInc)
        {
            color.set_blue(color.blue() + 1);
            blueInc = 16;
        }
    }
}

void butano_background::update()
{
    if(_move_action)
    {
        _move_action->update();
    }
}

}
