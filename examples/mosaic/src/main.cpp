#include "btn_core.h"
#include "btn_keypad.h"
#include "btn_optional.h"
#include "btn_bg_palettes.h"
#include "btn_bgs_mosaic.h"
#include "btn_regular_bg_ptr.h"
#include "btn_sprites_mosaic.h"
#include "btn_sprite_text_generator.h"

#include "btn_sprite_items_blonde.h"
#include "btn_regular_bg_items_land.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    void sprites_mosaic_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease horizontal stretch",
            "RIGHT: increase horizontal stretch",
            "DOWN: decrease vertical stretch",
            "UP: increase vertical stretch",
            "",
            "START: go to next scene",
        };

        info info("Sprites mosaic", info_text_lines, text_generator);

        btn::sprite_ptr blonde_sprite = btn::sprite_items::blonde.create_sprite(0, 0);
        blonde_sprite.set_mosaic_enabled(true);

        while(! btn::keypad::start_pressed())
        {
            btn::fixed horizontal_stretch = btn::sprites_mosaic::horizontal_stretch();
            btn::fixed vertical_stretch = btn::sprites_mosaic::vertical_stretch();

            if(btn::keypad::left_held())
            {
                btn::sprites_mosaic::set_horizontal_stretch(btn::max(horizontal_stretch - 0.01, btn::fixed(0)));
            }
            else if(btn::keypad::right_held())
            {
                btn::sprites_mosaic::set_horizontal_stretch(btn::min(horizontal_stretch + 0.01, btn::fixed(1)));
            }

            if(btn::keypad::down_held())
            {
                btn::sprites_mosaic::set_vertical_stretch(btn::max(vertical_stretch - 0.01, btn::fixed(0)));
            }
            else if(btn::keypad::up_held())
            {
                btn::sprites_mosaic::set_vertical_stretch(btn::min(vertical_stretch + 0.01, btn::fixed(1)));
            }

            info.update();
            btn::core::update();
        }

        btn::sprites_mosaic::set_horizontal_stretch(0);
        btn::sprites_mosaic::set_vertical_stretch(0);
    }

    void bgs_mosaic_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease horizontal stretch",
            "RIGHT: increase horizontal stretch",
            "DOWN: decrease vertical stretch",
            "UP: increase vertical stretch",
            "",
            "START: go to next scene",
        };

        info info("Backgrounds mosaic", info_text_lines, text_generator);

        btn::regular_bg_ptr land_bg = btn::regular_bg_items::land.create_bg(0, 0);
        land_bg.set_mosaic_enabled(true);

        while(! btn::keypad::start_pressed())
        {
            btn::fixed horizontal_stretch = btn::bgs_mosaic::horizontal_stretch();
            btn::fixed vertical_stretch = btn::bgs_mosaic::vertical_stretch();

            if(btn::keypad::left_held())
            {
                btn::bgs_mosaic::set_horizontal_stretch(btn::max(horizontal_stretch - 0.01, btn::fixed(0)));
            }
            else if(btn::keypad::right_held())
            {
                btn::bgs_mosaic::set_horizontal_stretch(btn::min(horizontal_stretch + 0.01, btn::fixed(1)));
            }

            if(btn::keypad::down_held())
            {
                btn::bgs_mosaic::set_vertical_stretch(btn::max(vertical_stretch - 0.01, btn::fixed(0)));
            }
            else if(btn::keypad::up_held())
            {
                btn::bgs_mosaic::set_vertical_stretch(btn::min(vertical_stretch + 0.01, btn::fixed(1)));
            }

            info.update();
            btn::core::update();
        }

        btn::bgs_mosaic::set_horizontal_stretch(0);
        btn::bgs_mosaic::set_vertical_stretch(0);
    }
}

int main()
{
    btn::core::init();

    btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    btn::bg_palettes::set_transparent_color(btn::color(16, 16, 16));

    while(true)
    {
        sprites_mosaic_scene(text_generator);
        btn::core::update();

        bgs_mosaic_scene(text_generator);
        btn::core::update();
    }
}
