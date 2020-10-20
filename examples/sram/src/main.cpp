/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_core.h"
#include "btn_sram.h"
#include "btn_string.h"
#include "btn_optional.h"
#include "btn_bg_palettes.h"
#include "btn_sprite_text_generator.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    struct sram_data
    {
        btn::array<char, 32> format_tag;
        int reads_count = 0;
    };
}

int main()
{
    btn::core::init();

    btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    btn::bg_palettes::set_transparent_color(btn::color(16, 16, 16));

    btn::string_view info_text_lines[4];

    sram_data cart_sram_data;
    btn::sram::read(cart_sram_data);

    btn::array<char, 32> expected_format_tag;
    btn::istring_base expected_format_tag_istring(expected_format_tag._data);
    btn::ostringstream expected_format_tag_stream(expected_format_tag_istring);
    expected_format_tag_stream.append("SRAM example");

    btn::string<32> sram_reads_count;

    if(cart_sram_data.format_tag == expected_format_tag)
    {
        ++cart_sram_data.reads_count;

        sram_reads_count = btn::to_string<32>(cart_sram_data.reads_count);

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
    }

    btn::sram::write(cart_sram_data);

    info info("SRAM", info_text_lines, text_generator);
    info.set_show_always(true);

    while(true)
    {
        info.update();
        btn::core::update();
    }
}
