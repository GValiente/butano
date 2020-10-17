#include "btn_core.h"
#include "btn_fixed.h"
#include "btn_vector.h"
#include "btn_display.h"
#include "btn_regular_bg_ptr.h"
#include "btn_regular_bg_attributes.h"
#include "btn_regular_bg_attributes_hblank_effect_ptr.h"

#include "btn_regular_bg_items_red.h"
#include "btn_regular_bg_items_green.h"

int main()
{
    btn::core::init();

    btn::regular_bg_ptr red_bg = btn::regular_bg_items::red.create_bg(0, 0);
    btn::regular_bg_map_ptr green_map = btn::regular_bg_items::green.create_bg(0, 0).map();
    btn::regular_bg_attributes red_attributes = red_bg.attributes();
    btn::regular_bg_attributes green_attributes = red_attributes;
    green_attributes.set_map(green_map);

    btn::vector<btn::regular_bg_attributes, btn::display::height()> attributes;

    for(int index = 0; index < btn::display::height(); ++index)
    {
        if(index % 2)
        {
            attributes.push_back(green_attributes);
        }
        else
        {
            attributes.push_back(red_attributes);
        }
    }

    btn::span<const btn::regular_bg_attributes> attributes_ref(attributes.data(), attributes.size());
    btn::regular_bg_attributes_hblank_effect_ptr hblank_effect =
            btn::regular_bg_attributes_hblank_effect_ptr::create(red_bg, attributes_ref);

    while(true)
    {
        btn::core::update();
    }
}
