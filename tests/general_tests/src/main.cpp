/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_colors.h"
#include "bn_sprite_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_config_assert.h"
#include "bn_sprite_text_generator.h"

#include "common_variable_8x16_sprite_font.h"

#include "string_tests.h"
#include "fixed_tests.h"
#include "math_tests.h"
#include "sqrt_tests.h"
#include "random_tests.h"
#include "optional_tests.h"
#include "any_tests.h"
#include "format_tests.h"
#include "memory_tests.h"
#include "sram_tests.h"

#if ! BN_CFG_ASSERT_ENABLED
    static_assert(false, "Enable asserts in bn_config_assert.h to run tests");
#endif

int main()
{
    bn::core::init();

    int used_stack_iwram = bn::memory::used_stack_iwram();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    text_generator.set_center_alignment();

    auto text = text_generator.generate<8>(0, 0, "Running tests...");
    bn::bg_palettes::set_transparent_color(bn::colors::gray);
    bn::core::update();

    string_tests();
    fixed_tests();
    math_tests();
    sqrt_tests();
    random_tests();
    optional_tests();
    any_tests();
    format_tests();
    memory_tests memory_tests(used_stack_iwram);
    sram_tests sram_tests;

    if(sram_tests.again())
    {
        text = text_generator.generate<8>(0, 0, "Run tests again to check SRAM");
    }
    else
    {
        text = text_generator.generate<8>(0, 0, "All tests passed :D");
    }

    while(true)
    {
        bn::core::update();
    }
}
