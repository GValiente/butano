#include "btn_core.h"
#include "btn_memory.h"
#include "btn_sprite_text_generator.h"

#include "bf_game.h"
#include "bf_stats.h"
#include "bf_small_sprite_font.h"
#include "bf_keypad_shortcuts.h"
#include "bf_butano_background.h"

int main()
{
    btn::core::init();

    btn::sprite_text_generator text_generator(bf::small_sprite_font);
    text_generator.set_bg_priority(1);

    bf::butano_background butano_background;
    auto game = btn::make_unique<bf::game::game>(text_generator);
    bf::stats stats(text_generator);
    bf::keypad_shortcuts keypad_shortcuts;
    btn::core::update();

    while(true)
    {
        game->update();
        butano_background.update();
        stats.update();
        keypad_shortcuts.update();
        btn::core::update();
    }
}
