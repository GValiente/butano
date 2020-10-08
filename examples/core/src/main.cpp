#include "btn_core.h"
#include "btn_music.h"
#include "btn_string.h"
#include "btn_keypad.h"
#include "btn_optional.h"
#include "btn_bg_palettes.h"
#include "btn_sprite_text_generator.h"

#include "btn_music_items.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    void cpu_usage_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("CPU usage", info_text_lines, text_generator);

        btn::vector<btn::sprite_ptr, 2> text_sprites;
        btn::fixed max_cpu_usage;
        int counter = 1;
        btn::music_items::soda7_xcopy_ohc.play(0.5);

        while(! btn::keypad::start_pressed())
        {
            max_cpu_usage = btn::max(max_cpu_usage, btn::core::cpu_usage());
            --counter;

            if(! counter)
            {
                btn::string<32> text;
                btn::ostringstream text_stream(text);
                text_stream.append((max_cpu_usage * 100).integer());
                text_stream.append("%");
                text_sprites.clear();
                text_generator.generate(0, 0, text, text_sprites);

                max_cpu_usage = 0;
                counter = 60;
            }

            info.update();
            btn::core::update();
        }

        btn::music::stop();
    }

    void sleep_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "A: sleep",
            "B: awake",
            "",
            "",
            "",
            "",
            "",
            "START: go to next scene",
        };

        info info("Sleep", info_text_lines, text_generator);
        info.set_show_always(true);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::a_pressed())
            {
                btn::core::sleep(btn::keypad::key_type::B);
            }

            info.update();
            btn::core::update();
        }
    }

    [[noreturn]] void reset_scene(btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: reset",
        };

        info info("Reset", info_text_lines, text_generator);

        while(true)
        {
            if(btn::keypad::start_pressed())
            {
                btn::core::reset();
            }

            info.update();
            btn::core::update();
        }
    }
}

int main()
{
    btn::core::init();

    btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    btn::bg_palettes::set_transparent_color(btn::color(16, 16, 16));

    cpu_usage_scene(text_generator);
    btn::core::update();

    sleep_scene(text_generator);
    btn::core::update();

    reset_scene(text_generator);
}
