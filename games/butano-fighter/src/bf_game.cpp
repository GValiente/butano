#include "bf_game.h"

#include "btn_music.h"
#include "btn_music_items.h"
#include "btn_sprite_items_flash_palette.h"

namespace bf::game
{

game::game(btn::sprite_text_generator& text_generator) :
    _enemies(btn::sprite_items::flash_palette.palette_item().create_palette()),
    _objects(btn::sprite_items::flash_palette.palette_item().create_palette()),
    _scoreboard(text_generator)
{
    btn::music::play(btn::music_items::cyberrid, 0.4);
}

btn::optional<scene_type> game::update()
{
    btn::optional<scene_type> result;

    _hero.update(_hero_bomb, _enemies, _enemy_bullets, _objects, _background);
    _hero_bullets.update(_hero, _enemies, _objects);
    _hero_bomb.update(_hero, _enemies, _enemy_bullets, _background);
    _background.update();
    _enemies.update(_hero, _hero_bomb, _enemy_bullets);
    _enemy_bullets.update();
    _objects.update();
    _scoreboard.update(_hero);

    return result;
}

}
