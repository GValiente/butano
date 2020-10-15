#include "bf_game.h"

#include "btn_music_items.h"
#include "btn_sprite_items_flash_palette.h"
#include "bf_status.h"
#include "bf_butano_background.h"

namespace bf::game
{

game::game(status& status, btn::sprite_text_generator& text_generator, butano_background& butano_background) :
    _camera(btn::camera_ptr::create(0, 0)),
    _background(status.current_stage(), _camera),
    _hero(_camera, status),
    _intro(status.current_stage(), text_generator),
    _enemies(status.current_stage(), btn::sprite_items::flash_palette.palette_item().create_palette()),
    _objects(btn::sprite_items::flash_palette.palette_item().create_palette()),
    _scoreboard(text_generator),
    _butano_background(butano_background)
{
    const stage& current_stage = status.current_stage();
    butano_background.hide(_hero.body_position(), _camera);
    current_stage.music_item.play(current_stage.music_volume);
}

btn::optional<scene_type> game::update()
{
    btn::optional<scene_type> result;
    _pause.update(_butano_background);

    if(! _pause.active())
    {
        result = _hero.update(_hero_bomb, _enemies, _enemy_bullets, _objects, _background, _butano_background, _camera);

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
            _hero_bullets.update(_camera, _hero, _enemies, _objects);
            _hero_bomb.update(_intro, _boss_intro, _camera, _hero, _enemies, _enemy_bullets, _background);
            _background.update();
            _intro.update(_butano_background);
            _enemies.update(_hero_bomb, _intro, _camera, _hero, _enemy_bullets, _objects, _boss_intro, _scoreboard,
                            _background);
            _enemy_bullets.update();
            _objects.update();
            _scoreboard.update(_hero);
            _boss_intro.update();
        }
    }

    return result;
}

}
