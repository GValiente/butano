/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_sram.h"
#include "bn_string.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    struct sram_data
    {
        bn::array<char, 32> format_tag;
        int reads_count = 0;
    };
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    bn::string_view info_text_lines[4];

    sram_data cart_sram_data;
    bn::sram::read(cart_sram_data);

    bn::array<char, 32> expected_format_tag;
    bn::istring_base expected_format_tag_istring(expected_format_tag._data);
    bn::ostringstream expected_format_tag_stream(expected_format_tag_istring);
    expected_format_tag_stream.append("SRAM example");

    bn::string<32> sram_reads_count;

    if(cart_sram_data.format_tag == expected_format_tag)
    {
        ++cart_sram_data.reads_count;

        sram_reads_count = bn::to_string<32>(cart_sram_data.reads_count);

        info_text_lines[0] = "SRAM is formatted!";
        info_text_lines[1] = "";
        info_text_lines[2] = "SRAM reads count:";
        info_text_lines[3] = sram_reads_count;
    }
    else
    {
        cart_sram_data.format_tag = expected_format_tag;
        cart_sram_data.reads_count = 1;

        info_text_lines[0] = "SRAM is not formatted";
        info_text_lines[1] = "";
        info_text_lines[2] = "If you see this message again,";
        info_text_lines[3] = "SRAM is not working";

        bn::sram::clear(bn::sram::size());
    }

    bn::sram::write(cart_sram_data);

    common::info info("SRAM", info_text_lines, text_generator);
    info.set_show_always(true);

    while(true)
    {
        info.update();
        bn::core::update();
    }
}
