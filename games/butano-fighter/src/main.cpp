#include "btn_core.h"
#include "btn_memory.h"
#include "btn_sprite_text_generator.h"

#include "bf_game.h"
#include "bf_stats.h"
#include "bf_intro.h"
#include "bf_title.h"
#include "bf_how_to_play.h"
#include "bf_game_status.h"
#include "bf_big_sprite_font.h"
#include "bf_keypad_shortcuts.h"
#include "bf_butano_background.h"
#include "bf_small_sprite_font.h"

int main()
{
    btn::core::init();

    btn::sprite_text_generator small_text_generator(bf::small_sprite_font);
    small_text_generator.set_bg_priority(1);

    btn::sprite_text_generator big_text_generator(bf::big_sprite_font);
    big_text_generator.set_bg_priority(1);

    bf::game::status game_status;
    bf::butano_background butano_background;
    btn::unique_ptr<bf::scene> scene(new bf::how_to_play(bf::scene_type::TITLE, big_text_generator, butano_background));
    bf::stats stats(small_text_generator);
    bf::keypad_shortcuts keypad_shortcuts;
    btn::optional<bf::scene_type> next_scene = bf::scene_type::INTRO;
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
        btn::core::update();

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
                    scene.reset(new bf::title(game_status, small_text_generator, butano_background));
                    break;

                case bf::scene_type::GAME:
                    scene.reset(new bf::game::game(game_status, small_text_generator, butano_background));
                    break;

                case bf::scene_type::HOW_TO_PLAY_AND_TITLE:
                    scene.reset(new bf::how_to_play(bf::scene_type::TITLE, big_text_generator, butano_background));
                    break;

                case bf::scene_type::HOW_TO_PLAY_AND_GAME:
                    scene.reset(new bf::how_to_play(bf::scene_type::GAME, big_text_generator, butano_background));
                    break;

                default:
                    BTN_ERROR("Invalid next scene: ", int(*next_scene));
                    break;
                }
            }
        }
    }
}
