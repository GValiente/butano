#include "btn_size.h"
#include "btn_core.h"
#include "btn_colors.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_sprite_ptr.h"
#include "btn_bg_palettes.h"
#include "btn_config_assert.h"
#include "btn_sprite_text_generator.h"
#include "sprite_font.h"

#include "fixed_tests.h"

int main()
{
    btn::core::init();

    btn::bg_palettes::set_transparent_color(btn::colors::gray);

    int text_x = btn::display::width() / 2;
    int text_y = btn::display::height() / 2;
    btn::sprite_text_generator text_generator(sprite_font);
    text_generator.set_alignment(btn::horizontal_alignment_type::CENTER);

    #if BTN_CFG_ASSERT_ENABLED
        fixed_tests();

        auto text = text_generator.generate<8>(text_x, text_y, "All tests passed :D");
    #else
        auto text1 = text_generator.generate<8>(text_x, text_y - 16, "Enable asserts in");
        auto text2 = text_generator.generate<8>(text_x, text_y, "btn_config_assert.h");
        auto text3 = text_generator.generate<8>(text_x, text_y + 16, "to run tests");
    #endif

    while(true)
    {
        btn::core::update();
    }
}
