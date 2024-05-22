/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_date.h"
#include "bn_time.h"
#include "bn_keypad.h"
#include "bn_string.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    void date_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view week_days[] = {
            "Sunday",
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday",
            "Saturday",
        };

        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Date", info_text_lines, text_generator);

        bn::vector<bn::sprite_ptr, 16> text_sprites;

        while(! bn::keypad::start_pressed())
        {
            bn::string<64> text;
            text_sprites.clear();

            if(bn::date::active())
            {
                if(bn::optional<bn::date> date = bn::date::current())
                {
                    text += week_days[date->week_day()];
                    text += ' ';
                    text += bn::to_string<4>(date->year());
                    text += '/';
                    text += bn::to_string<4>(date->month());
                    text += '/';
                    text += bn::to_string<4>(date->month_day());
                }
                else
                {
                    text = "Invalid RTC date";
                }
            }
            else
            {
                text = "RTC not present";
            }

            text_generator.generate(0, 0, text, text_sprites);

            info.update();
            bn::core::update();
        }
    }

    void time_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Time", info_text_lines, text_generator);

        bn::vector<bn::sprite_ptr, 8> text_sprites;

        while(! bn::keypad::start_pressed())
        {
            bn::string<32> text;
            text_sprites.clear();

            if(bn::time::active())
            {
                if(bn::optional<bn::time> time = bn::time::current())
                {
                    bn::string<4> hour = bn::to_string<4>(time->hour());
                    bn::string<4> minute = bn::to_string<4>(time->minute());
                    bn::string<4> second = bn::to_string<4>(time->second());

                    if(hour.size() == 1)
                    {
                        hour = "0" + hour;
                    }

                    if(minute.size() == 1)
                    {
                        minute = "0" + minute;
                    }

                    if(second.size() == 1)
                    {
                        second = "0" + second;
                    }

                    text += hour;
                    text += ':';
                    text += minute;
                    text += ':';
                    text += second;
                }
                else
                {
                    text = "Invalid RTC time";
                }
            }
            else
            {
                text = "RTC not present";
            }

            text_generator.generate(0, 0, text, text_sprites);

            info.update();
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while(true)
    {
        date_scene(text_generator);
        bn::core::update();

        time_scene(text_generator);
        bn::core::update();
    }
}
