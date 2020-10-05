#include "btn_core.h"
#include "btn_keypad.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_regular_bg_ptr.h"
#include "btn_blending_actions.h"
#include "btn_blending_fade_alpha.h"
#include "btn_sprite_text_generator.h"
#include "btn_blending_fade_alpha_hblank_effect_ptr.h"
#include "btn_blending_transparency_attributes.h"
#include "btn_blending_transparency_attributes_hblank_effect_ptr.h"

#include "btn_sprite_items_dinosaur.h"
#include "btn_regular_bg_items_mountain.h"
#include "btn_sprite_items_variable_8x16_font_yellow.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    void transparency_scene(btn::regular_bg_ptr& mountain_bg, btn::sprite_ptr& dinosaur_sprite,
                            btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease transparency alpha",
            "RIGHT: increase transparency alpha",
            "",
            "START: go to next scene",
        };

        info info("Transparency", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(false);
        dinosaur_sprite.set_blending_enabled(true);

        btn::blending::set_transparency_alpha(0.5);

        while(! btn::keypad::start_pressed())
        {
            btn::fixed transparency_alpha = btn::blending::transparency_alpha();

            if(btn::keypad::left_held())
            {
                btn::blending::set_transparency_alpha(btn::max(transparency_alpha - 0.01, btn::fixed(0)));
            }
            else if(btn::keypad::right_held())
            {
                btn::blending::set_transparency_alpha(btn::min(transparency_alpha + 0.01, btn::fixed(1)));
            }

            info.update();
            btn::core::update();
        }

        btn::blending::set_transparency_alpha(1);
    }

    void intensity_scene(btn::regular_bg_ptr& mountain_bg, btn::sprite_ptr& dinosaur_sprite,
                         btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease intensity alpha",
            "RIGHT: increase intensity alpha",
            "",
            "START: go to next scene",
        };

        info info("Intensity", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(false);
        dinosaur_sprite.set_blending_enabled(true);

        btn::blending::set_intensity_alpha(0.5);

        while(! btn::keypad::start_pressed())
        {
            btn::fixed intensity_alpha = btn::blending::intensity_alpha();

            if(btn::keypad::left_held())
            {
                btn::blending::set_intensity_alpha(btn::max(intensity_alpha - 0.01, btn::fixed(0)));
            }
            else if(btn::keypad::right_held())
            {
                btn::blending::set_intensity_alpha(btn::min(intensity_alpha + 0.01, btn::fixed(1)));
            }

            info.update();
            btn::core::update();
        }

        btn::blending::set_intensity_alpha(0);
    }

    void fade_scene(btn::regular_bg_ptr& mountain_bg, btn::sprite_ptr& dinosaur_sprite,
                    btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "LEFT: decrease fade alpha",
            "RIGHT: increase fade alpha",
            "A: toggle fade color",
            "",
            "START: go to next scene",
        };

        info info("Fade", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(true);
        dinosaur_sprite.set_blending_enabled(true);

        btn::blending::set_fade_alpha(0.5);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::a_pressed())
            {
                if(btn::blending::fade_color() == btn::blending::fade_color_type::BLACK)
                {
                    btn::blending::set_fade_color(btn::blending::fade_color_type::WHITE);
                }
                else
                {
                    btn::blending::set_fade_color(btn::blending::fade_color_type::BLACK);
                }
            }

            btn::fixed fade_alpha = btn::blending::fade_alpha();

            if(btn::keypad::left_held())
            {
                btn::blending::set_fade_alpha(btn::max(fade_alpha - 0.01, btn::fixed(0)));
            }
            else if(btn::keypad::right_held())
            {
                btn::blending::set_fade_alpha(btn::min(fade_alpha + 0.01, btn::fixed(1)));
            }

            info.update();
            btn::core::update();
        }

        btn::blending::set_fade_color(btn::blending::fade_color_type::BLACK);
        btn::blending::set_fade_alpha(0);
    }

    void transparency_actions_scene(btn::regular_bg_ptr& mountain_bg, btn::sprite_ptr& dinosaur_sprite,
                                      btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Transparency actions", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(false);
        dinosaur_sprite.set_blending_enabled(true);

        btn::blending_transparency_alpha_loop_action action(120, 0);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }

        btn::blending::set_transparency_alpha(1);
    }

    void intensity_actions_scene(btn::regular_bg_ptr& mountain_bg, btn::sprite_ptr& dinosaur_sprite,
                                 btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("intensity actions", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(false);
        dinosaur_sprite.set_blending_enabled(true);

        btn::blending_intensity_alpha_loop_action action(120, 1);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }

        btn::blending::set_intensity_alpha(0);
    }

    void fade_actions_scene(btn::regular_bg_ptr& mountain_bg, btn::sprite_ptr& dinosaur_sprite,
                            btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Fade actions", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(true);
        dinosaur_sprite.set_blending_enabled(true);

        btn::blending_fade_alpha_loop_action action(120, 1);

        while(! btn::keypad::start_pressed())
        {
            action.update();
            info.update();
            btn::core::update();
        }

        btn::blending::set_fade_alpha(0);
    }

    void transparency_hblank_effect_scene(btn::regular_bg_ptr& mountain_bg, btn::sprite_ptr& dinosaur_sprite,
                                          btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Transparency H-Blank effect", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(false);
        dinosaur_sprite.set_blending_enabled(true);

        btn::array<btn::blending_transparency_attributes, btn::display::height()> transparency_attributes;

        for(int index = 0, amplitude = 32; index < amplitude; ++index)
        {
            btn::fixed transparency_alpha = btn::fixed(index) / amplitude;
            transparency_attributes[(btn::display::height() / 2) + index].set_transparency_alpha(transparency_alpha);
            transparency_attributes[(btn::display::height() / 2) - index - 1].set_transparency_alpha(transparency_alpha);
        }

        btn::blending_transparency_attributes_hblank_effect_ptr transparency_hblank_effect =
                btn::blending_transparency_attributes_hblank_effect_ptr::create(transparency_attributes);

        while(! btn::keypad::start_pressed())
        {
            info.update();
            btn::core::update();
        }
    }

    void fade_hblank_effect_scene(btn::regular_bg_ptr& mountain_bg, btn::sprite_ptr& dinosaur_sprite,
                                          btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Fade H-Blank effect", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(true);
        dinosaur_sprite.set_blending_enabled(true);

        btn::array<btn::blending_fade_alpha, btn::display::height()> fade_alphas;

        for(int index = 0, amplitude = btn::display::height() / 2; index < amplitude; ++index)
        {
            btn::fixed fade_alpha = btn::fixed(index) / amplitude;
            fade_alphas[(btn::display::height() / 2) + index].set_value(fade_alpha);
            fade_alphas[(btn::display::height() / 2) - index - 1].set_value(fade_alpha);
        }

        btn::blending_fade_alpha_hblank_effect_ptr fade_hblank_effect =
                btn::blending_fade_alpha_hblank_effect_ptr::create(fade_alphas);

        while(! btn::keypad::start_pressed())
        {
            info.update();
            btn::core::update();
        }
    }
}

int main()
{
    btn::core::init();

    btn::regular_bg_ptr mountain_bg = btn::regular_bg_items::mountain.create_bg(0, 0);
    btn::sprite_ptr dinosaur_sprite = btn::sprite_items::dinosaur.create_sprite(0, 0);

    btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    text_generator.set_palette_item(btn::sprite_items::variable_8x16_font_yellow.palette_item());

    while(true)
    {
        transparency_scene(mountain_bg, dinosaur_sprite, text_generator);
        btn::core::update();

        intensity_scene(mountain_bg, dinosaur_sprite, text_generator);
        btn::core::update();

        fade_scene(mountain_bg, dinosaur_sprite, text_generator);
        btn::core::update();

        transparency_actions_scene(mountain_bg, dinosaur_sprite, text_generator);
        btn::core::update();

        intensity_actions_scene(mountain_bg, dinosaur_sprite, text_generator);
        btn::core::update();

        fade_actions_scene(mountain_bg, dinosaur_sprite, text_generator);
        btn::core::update();

        transparency_hblank_effect_scene(mountain_bg, dinosaur_sprite, text_generator);
        btn::core::update();

        fade_hblank_effect_scene(mountain_bg, dinosaur_sprite, text_generator);
        btn::core::update();
    }
}
