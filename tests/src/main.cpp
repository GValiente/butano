#include "btn_core.h"
#include "btn_colors.h"
#include "btn_optional.h"
#include "btn_sprite_ptr.h"
#include "btn_bg_palettes.h"
#include "btn_config_assert.h"
#include "btn_sprite_text_generator.h"

#include "fixed_tests.h"
#include "math_tests.h"
#include "sqrt_tests.h"
#include "any_tests.h"
#include "sprite_sort_test.h"
#include "sram_tests.h"
#include "variable_8x16_sprite_font.h"

#if ! BTN_CFG_ASSERT_ENABLED
    static_assert(false, "Enable asserts in btn_config_assert.h to run tests");
#endif

int main()
{
    btn::core::init();

    btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    text_generator.set_alignment(btn::horizontal_alignment_type::CENTER);

    auto text = text_generator.generate<8>(0, 0, "Running tests...");
    btn::bg_palettes::set_transparent_color(btn::colors::gray);
    btn::core::update();

    fixed_tests();
    math_tests();
    sqrt_tests();
    any_tests();
    sprite_sort_tests();
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
        btn::core::update();
    }
}
