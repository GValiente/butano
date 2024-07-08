/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_log.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    struct custom_type
    {
        int data = 456;
    };

    bn::ostringstream& operator<<(bn::ostringstream& stream, const custom_type& custom_value)
    {
        stream.append("custom_type: ");
        stream.append(custom_value.data);
        return stream;
    }
}

__attribute__((noinline)) void log_example_c_function()
{
    bn::core::log_stacktrace();
}

__attribute__((noinline)) void log_example_b_function()
{
    log_example_c_function();
}

__attribute__((noinline)) void log_example_a_function()
{
    log_example_b_function();
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    bn::string_view info_text_lines[7];
    info_text_lines[2] = "Logs window is located in";
    info_text_lines[5] = "The current log backend";
    info_text_lines[6] = "is defined by BN_CFG_LOG_BACKEND";

    #if BN_CFG_LOG_BACKEND == BN_LOG_BACKEND_VBA
        info_text_lines[0] = "VisualBoyAdvance backend";
        info_text_lines[3] = "Tools -> Logging";
        info_text_lines[5] = "This backend crashes on hardware";
        info_text_lines[6] = "and on other emulators!";
    #elif BN_CFG_LOG_BACKEND == BN_LOG_BACKEND_NOCASHGBA
        info_text_lines[0] = "No$gba Debugger backend";
        info_text_lines[3] = "Window -> TTY debug messages";
    #elif BN_CFG_LOG_BACKEND == BN_LOG_BACKEND_MGBA
        info_text_lines[0] = "mGBA backend";
        info_text_lines[3] = "Tools -> View logs";
    #else
        static_assert(false, "Unknown log backend");
    #endif

    common::info info("Log", info_text_lines, text_generator);
    info.set_show_always(true);

    BN_LOG("Log example");
    BN_LOG("You can log characters: ", 'c');
    BN_LOG("Numbers: ", 123);
    BN_LOG("Bools: ", true);

    auto info_ptr = &info;
    BN_LOG("Pointers: ", info_ptr);

    info_ptr = nullptr;
    BN_LOG("Null pointers: ", info_ptr);
    BN_LOG("Always null pointers: ", nullptr);
    BN_LOG("Multiple arguments: ", 1, 'a', 2, 'b', 3, 'c');
    BN_LOG("Even fixed point numbers: ", bn::fixed(0.125));
    BN_LOG("Custom types too: ", custom_type());

    log_example_a_function();

    while(true)
    {
        info.update();
        bn::core::update();
    }
}
