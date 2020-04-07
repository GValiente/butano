#include "bf_game.h"

#include "btn_music.h"
#include "btn_music_items.h"
#include "btn_flash_palette_sprite_item.h"

namespace bf::game
{

game::game(btn::sprite_text_generator& text_generator) :
    _enemies(btn::sprite_items::flash_palette.palette_item().create_palette()),
    _objects(btn::sprite_items::flash_palette.palette_item().create_palette()),
    _scoreboard(text_generator)
{
    btn::music::play(btn::music_items::cyberrid, 0.4);
}

void game::update()
{
    _hero.update(_objects);
    _hero_bullets.update(_hero, _enemies, _objects);
    _hero_bomb.update(_hero, _enemies, _background);
    _background.update();
    _enemies.update(_hero, _hero_bomb, _enemy_bullets);
    _enemy_bullets.update();
    _objects.update();
    _scoreboard.update(_hero);
}

}
