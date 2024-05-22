/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_pause.h"

#include "bn_music.h"
#include "bn_keypad.h"
#include "bn_green_swap.h"
#include "bn_sound_items.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_palettes.h"
#include "bn_regular_bg_builder.h"
#include "bn_regular_bg_items_pause.h"
#include "bf_butano_background.h"
#include "bf_game_rumble_manager.h"

namespace bf::game
{

void pause::update(const butano_background& butano_background, rumble_manager& rumble_manager)
{
    if(_active)
    {
        if(bn::keypad::start_pressed())
        {
            _bg.reset();
            bn::bg_palettes::set_contrast(_contrast);
            bn::bg_palettes::set_grayscale_intensity(0);
            bn::sprite_palettes::set_contrast(_contrast);
            bn::sprite_palettes::set_fade_intensity(_fade_intensity);
            bn::sprite_palettes::set_grayscale_intensity(0);
            bn::green_swap::set_enabled(_green_swap);
            rumble_manager.set_enabled(_rumble);
            bn::sound_items::pause_out.play();

            if(bn::music::paused())
            {
                bn::music::resume();
            }

            _active = false;
        }
    }
    else
    {
        if(! butano_background.silhouette_visible() && bn::keypad::start_pressed())
        {
            bn::regular_bg_builder builder(bn::regular_bg_items::pause);
            builder.set_priority(0);
            _bg = builder.release_build();
            _contrast = bn::bg_palettes::contrast();
            _fade_intensity = bn::bg_palettes::fade_intensity();
            _green_swap = bn::green_swap::enabled();
            _rumble = rumble_manager.enabled();
            bn::bg_palettes::set_contrast(0);
            bn::bg_palettes::set_grayscale_intensity(1);
            bn::sprite_palettes::set_contrast(0);
            bn::sprite_palettes::set_fade_intensity(0);
            bn::sprite_palettes::set_grayscale_intensity(1);
            bn::green_swap::set_enabled(false);
            rumble_manager.set_enabled(false);
            bn::sound_items::pause_in.play();

            if(bn::music::playing())
            {
                bn::music::pause();
            }

            _active = true;
        }
    }
}

}
