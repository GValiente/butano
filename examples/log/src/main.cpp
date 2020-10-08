#include "btn_log.h"
#include "btn_core.h"
#include "btn_keypad.h"
#include "btn_optional.h"
#include "btn_bg_palettes.h"
#include "btn_sprite_text_generator.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    struct custom_type
    {
        int data = 456;
    };

    btn::ostringstream& operator<<(btn::ostringstream& stream, const custom_type& custom_value)
    {
        stream.append("custom_type: ");
        stream.append(custom_value.data);
        return stream;
    }
}

int main()
{
    btn::core::init();

    btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    btn::bg_palettes::set_transparent_color(btn::color(16, 16, 16));

    btn::string_view info_text_lines[7];
    info_text_lines[2] = "Logs window is located in";

    #if BTN_CFG_LOG_BACKEND == BTN_LOG_BACKEND_VBA
        info_text_lines[0] = "VisualBoyAdvance backend";
        info_text_lines[3] = "Tools -> Logging";
        info_text_lines[5] = "This backend crashes on hardware";
        info_text_lines[6] = "and on other emulators!";
    #elif BTN_CFG_LOG_BACKEND == BTN_LOG_BACKEND_NOCASHGBA
        info_text_lines[0] = "No$gba Debugger backend";
        info_text_lines[3] = "Window -> TTY debug messages";
    #elif BTN_CFG_LOG_BACKEND == BTN_LOG_BACKEND_MGBA
        info_text_lines[0] = "mGBA backend";
        info_text_lines[3] = "Tools -> View logs";
    #else
        static_assert(false, "Unknown log backend");
    #endif

    info info("Log", info_text_lines, text_generator);
    info.set_show_always(true);

    BTN_LOG("Log example");
    BTN_LOG("You can log characters: ", 'c');
    BTN_LOG("Numbers: ", 123);
    BTN_LOG("Bools: ", true);

    auto info_ptr = &info;
    BTN_LOG("Pointers: ", info_ptr);

    info_ptr = nullptr;
    BTN_LOG("Null pointers: ", info_ptr);
    BTN_LOG("Always null pointers: ", nullptr);
    BTN_LOG("Multiple arguments: ", 1, 'a', 2, 'b', 3, 'c');
    BTN_LOG("Even fixed point numbers: ", btn::fixed(0.125));
    BTN_LOG("Custom types too: ", custom_type());

    while(true)
    {
        info.update();
        btn::core::update();
    }
}
