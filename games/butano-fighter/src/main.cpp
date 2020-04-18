#include "btn_core.h"
#include "btn_memory.h"
#include "btn_sprite_text_generator.h"

#include "bf_game.h"
#include "bf_stats.h"
#include "bf_intro.h"
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

    bf::butano_background butano_background;
    auto intro = btn::make_unique<bf::intro>(big_text_generator);
    // auto game = btn::make_unique<bf::game::game>(small_text_generator);
    bf::stats stats(small_text_generator);
    bf::keypad_shortcuts keypad_shortcuts;
    btn::core::update();

    while(true)
    {
        // game->update();
        intro->update();
        butano_background.update();
        stats.update();
        keypad_shortcuts.update();
        btn::core::update();
    }
}
