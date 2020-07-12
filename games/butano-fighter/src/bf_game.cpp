#include "bf_game.h"

#include "btn_music_items.h"
#include "btn_sprite_items_flash_palette.h"
#include "bf_status.h"
#include "bf_butano_background.h"

namespace bf::game
{

game::game(status& status, btn::sprite_text_generator& text_generator, butano_background& butano_background) :
    _background(status.current_stage()),
    _hero(status),
    _intro(status.current_stage(), text_generator),
    _enemies(status.current_stage(), btn::sprite_items::flash_palette.palette_item().create_palette()),
    _objects(btn::sprite_items::flash_palette.palette_item().create_palette()),
    _scoreboard(text_generator),
    _butano_background(butano_background)
{
    butano_background.hide(_hero.body_position());
    status.current_stage().music_item.play(0.4);
}

btn::optional<scene_type> game::update()
{
    btn::optional<scene_type> result;
    _pause.update(_butano_background);

    if(! _pause.active())
    {
        result = _hero.update(_hero_bomb, _enemies, _enemy_bullets, _objects, _background, _butano_background);

        if(result)
        {
            _background.reset();

            if(btn::music::playing())
            {
                btn::music::stop();
            }
        }
        else
        {
            _hero_bullets.update(_hero, _enemies, _objects);
            _hero_bomb.update(_intro, _boss_intro, _hero, _enemies, _enemy_bullets, _background);
            _background.update();
            _intro.update(_butano_background);
            _enemies.update(_hero, _hero_bomb, _intro, _enemy_bullets, _objects, _boss_intro, _scoreboard);
            _enemy_bullets.update();
            _objects.update();
            _scoreboard.update(_hero);
            _boss_intro.update();
        }
    }

    return result;
}

}
