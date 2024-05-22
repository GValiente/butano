/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_string.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "external_tool_file.h"

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    bn::string_view info_text_lines[] = {
        "Date generated with",
        "python-script.py:",
        "",
        etf::current_date,
    };

    common::info info("External tool", info_text_lines, text_generator);
    info.set_show_always(true);

    while(true)
    {
        info.update();
        bn::core::update();
    }
}
