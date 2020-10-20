/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_pause.h"

#include "btn_music.h"
#include "btn_keypad.h"
#include "btn_green_swap.h"
#include "btn_sound_items.h"
#include "btn_bg_palettes.h"
#include "btn_sprite_palettes.h"
#include "btn_regular_bg_builder.h"
#include "btn_regular_bg_items_pause.h"
#include "bf_butano_background.h"

namespace bf::game
{

void pause::update(const butano_background& butano_background)
{
    if(_active)
    {
        if(btn::keypad::start_pressed())
        {
            _bg.reset();
            btn::bg_palettes::set_contrast(_contrast);
            btn::bg_palettes::set_grayscale_intensity(0);
            btn::sprite_palettes::set_contrast(_contrast);
            btn::sprite_palettes::set_fade_intensity(_fade_intensity);
            btn::sprite_palettes::set_grayscale_intensity(0);
            btn::green_swap::set_enabled(_green_swap);
            btn::sound_items::pause_out.play();

            if(btn::music::paused())
            {
                btn::music::resume();
            }

            _active = false;
        }
    }
    else
    {
        if(! butano_background.silhouette_visible() && btn::keypad::start_pressed())
        {
            btn::regular_bg_builder builder(btn::regular_bg_items::pause);
            builder.set_priority(0);
            _bg = builder.release_build();
            _contrast = btn::bg_palettes::contrast();
            _fade_intensity = btn::bg_palettes::fade_intensity();
            _green_swap = btn::green_swap::enabled();
            btn::bg_palettes::set_contrast(0);
            btn::bg_palettes::set_grayscale_intensity(1);
            btn::sprite_palettes::set_contrast(0);
            btn::sprite_palettes::set_fade_intensity(0);
            btn::sprite_palettes::set_grayscale_intensity(1);
            btn::green_swap::set_enabled(false);
            btn::sound_items::pause_in.play();

            if(btn::music::playing())
            {
                btn::music::pause();
            }

            _active = true;
        }
    }
}

}
