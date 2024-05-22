/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_sprite_text_generator.h"

#include "bf_game.h"
#include "bf_stats.h"
#include "bf_intro.h"
#include "bf_title.h"
#include "bf_status.h"
#include "bf_ending.h"
#include "bf_credits.h"
#include "bf_how_to_play.h"
#include "bf_big_sprite_font.h"
#include "bf_keypad_shortcuts.h"
#include "bf_butano_background.h"
#include "bf_small_sprite_font.h"

int main()
{
    bn::core::init();

    bn::sprite_text_generator small_text_generator(bf::small_sprite_font);
    small_text_generator.set_bg_priority(1);

    bn::sprite_text_generator big_text_generator(bf::big_sprite_font);
    big_text_generator.set_bg_priority(1);

    bf::status status;
    bf::butano_background butano_background;
    bn::unique_ptr<bf::scene> scene(new bf::intro(big_text_generator, butano_background));
    bf::stats stats(small_text_generator);
    bf::keypad_shortcuts keypad_shortcuts;
    bn::optional<bf::scene_type> next_scene = bf::scene_type::INTRO;
    int wait_frames = 0;

    while(true)
    {
        if(scene)
        {
            next_scene = scene->update();
        }

        butano_background.update();
        stats.update();
        keypad_shortcuts.update();
        bn::core::update();

        if(next_scene)
        {
            if(scene)
            {
                scene.reset();
                wait_frames = 30;
            }

            --wait_frames;

            if(! wait_frames)
            {
                switch(*next_scene)
                {

                case bf::scene_type::INTRO:
                    scene.reset(new bf::intro(big_text_generator, butano_background));
                    break;

                case bf::scene_type::TITLE:
                    scene.reset(new bf::title(status, small_text_generator, butano_background));
                    break;

                case bf::scene_type::GAME:
                    scene.reset(new bf::game::game(status, small_text_generator, butano_background));
                    break;

                case bf::scene_type::HOW_TO_PLAY_AND_TITLE:
                    scene.reset(new bf::how_to_play(bf::scene_type::TITLE, status, big_text_generator,
                                                    butano_background));
                    break;

                case bf::scene_type::HOW_TO_PLAY_AND_GAME:
                    scene.reset(new bf::how_to_play(bf::scene_type::GAME, status, big_text_generator,
                                                    butano_background));
                    break;

                case bf::scene_type::CREDITS:
                    scene.reset(new bf::credits(big_text_generator, butano_background));
                    break;

                case bf::scene_type::ENDING:
                    scene.reset(new bf::ending(big_text_generator, butano_background));
                    break;

                default:
                    BN_ERROR("Invalid next scene: ", int(*next_scene));
                    break;
                }
            }
        }
    }
}
