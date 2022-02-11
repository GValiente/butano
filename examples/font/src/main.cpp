/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_sprite_ptr.h"
#include "bn_bg_palettes.h"
#include "bn_string_view.h"
#include "bn_sprite_text_generator.h"

#include "user_variable_32x32_sprite_font.h"

namespace
{
    void user_font_text_scene()
    {
        bn::sprite_text_generator user_font_text_generator(user_variable_32x32_sprite_font);
        user_font_text_generator.set_center_alignment();

        bn::vector<bn::sprite_ptr, 32> user_font_text_sprites;
        user_font_text_generator.generate(0, -48, "这是用户自定义", user_font_text_sprites);
        user_font_text_generator.generate(0, -16, "字体。", user_font_text_sprites);
        user_font_text_generator.generate(0, 16, "ユーザー設定の", user_font_text_sprites);
        user_font_text_generator.generate(0, 48, "フォントです。", user_font_text_sprites);

        while(! bn::keypad::start_pressed())
        {
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while(true)
    {
        user_font_text_scene();
        bn::core::update();
    }
}
