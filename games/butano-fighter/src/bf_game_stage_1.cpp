#include "bf_game_stage_1.h"

#include "btn_display.h"
#include "btn_sound_items.h"
#include "btn_sprite_items_stage_1_animals.h"
#include "btn_sprite_items_stage_1_monsters_1.h"
#include "btn_sprite_items_stage_1_characters_1.h"
#include "bf_constants.h"

namespace bf::game::stage_1
{

namespace
{
    constexpr const int start_y = -(btn::display::height() + 16) / 2;

    constexpr const btn::span<const enemy_bullet_event> no_bullets;

    [[nodiscard]] constexpr btn::fixed move_y(btn::fixed y)
    {
        return y + constants::background_speed;
    }


    // pig:

    constexpr const enemy_data pig(btn::sprite_items::stage_1_animals, enemy_data::death_anim_type::VERTICAL_SCALE,
                                   btn::sound_items::dinosaur_2, btn::fixed_size(16, 13), 2, 3, 1, 1);

    constexpr const enemy_move_event pig_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(-0.1)), 1, false),
    };

    constexpr const enemy_move_event pig_flipped_moves[] = {
        pig_moves[0].flipped(),
    };


    // sheep:

    constexpr const enemy_data sheep(btn::sprite_items::stage_1_animals, enemy_data::death_anim_type::VERTICAL_SCALE,
                                     btn::sound_items::kill, btn::fixed_size(16, 12), 0, 1, 2, 2);

    constexpr const enemy_move_event sheep_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(-0.1)), 1, false),
    };

    constexpr const enemy_move_event sheep_flipped_moves[] = {
        sheep_moves[0].flipped(),
    };


    // man:

    constexpr const enemy_data man(btn::sprite_items::stage_1_characters_1, enemy_data::death_anim_type::HORIZONTAL_SCALE,
                                   btn::sound_items::scream_4, btn::fixed_size(10, 10), 0, 1, 2, 3);

    constexpr const enemy_move_event man_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0.25)), 60, false),
        enemy_move_event(btn::fixed_point(0, move_y(0)),    1,  false),
    };

    constexpr const enemy_move_event man_flipped_moves[] = {
        man_moves[0].flipped(),
        man_moves[1].flipped(),
    };

    constexpr const enemy_bullet_event man_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 60),
    };


    // woman:

    constexpr const enemy_data woman(btn::sprite_items::stage_1_characters_1, enemy_data::death_anim_type::ROTATE,
                                     btn::sound_items::enemy_death_ninja_14, btn::fixed_size(9, 10), 2, 3, 2, 3);

    constexpr const enemy_move_event woman_moves[] = {
        enemy_move_event(btn::fixed_point(0.75, move_y(0.25)),  90, false),
        enemy_move_event(btn::fixed_point(0,    move_y(-0.75)), 1,  false),
    };

    constexpr const enemy_move_event woman_flipped_moves[] = {
        woman_moves[0].flipped(),
        woman_moves[1].flipped(),
    };


    // bat:

    constexpr const enemy_data bat(btn::sprite_items::stage_1_monsters_1, enemy_data::death_anim_type::ROTATE,
                                   btn::sound_items::dinosaur_5, btn::fixed_size(14, 8), 0, 1, 1, 1);

    constexpr const enemy_move_event bat_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0.25)), 1, true),
    };

    constexpr const enemy_move_event bat_flipped_moves[] = {
        bat_moves[0].flipped(),
    };

    constexpr const enemy_bullet_event bat_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL,    1,     30),
        enemy_bullet_event(enemy_bullet_type::SMALL,    1,     60),
    };


    // enemy events:

    constexpr const enemy_event enemy_events[] = {

        // level 0

        enemy_event(pig, btn::fixed_point(-45, start_y), pig_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(pig, btn::fixed_point(-60, start_y), pig_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(pig, btn::fixed_point(-30, start_y), pig_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(sheep, btn::fixed_point(40, start_y), sheep_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(sheep, btn::fixed_point(55, start_y), sheep_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(sheep, btn::fixed_point(25, start_y), sheep_flipped_moves, no_bullets, 120, enemy_drop_type::NONE),

        // level 1

        enemy_event(man, btn::fixed_point(-45, start_y), man_moves, man_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(-60, start_y), man_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(-30, start_y), man_moves, man_bullets, 60, enemy_drop_type::NONE),

        enemy_event(man, btn::fixed_point(40, start_y), man_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(55, start_y), man_flipped_moves, man_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(25, start_y), man_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(woman, btn::fixed_point(-40, start_y), woman_moves, no_bullets, 60, enemy_drop_type::GEM),
        enemy_event(woman, btn::fixed_point(40, start_y), woman_flipped_moves, no_bullets, 60, enemy_drop_type::GEM),

    };


    // stage:

    constexpr const stage stage_1(enemy_events);
}

const stage& get()
{
    return stage_1;
}

}
