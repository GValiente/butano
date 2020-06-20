#include "bf_game_stage_1.h"

#include "btn_display.h"
#include "btn_music_items.h"
#include "btn_sound_items.h"
#include "btn_regular_bg_items_clouds.h"
#include "btn_regular_bg_items_stage_1.h"
#include "btn_sprite_items_stage_1_intro.h"
#include "btn_sprite_items_stage_1_intro_alt.h"
#include "btn_sprite_items_stage_1_animals.h"
#include "btn_sprite_items_stage_1_monsters_1.h"
#include "btn_sprite_items_stage_1_monsters_2.h"
#include "btn_sprite_items_stage_1_characters_1.h"
#include "btn_sprite_items_stage_1_characters_2.h"
#include "btn_sprite_items_stage_1_characters_1_team.h"
#include "bf_constants.h"
#include "bf_game_bullet_util.h"

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

    constexpr const enemy_data man(btn::sprite_items::stage_1_characters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
                                   btn::sound_items::scream_4, btn::fixed_size(10, 10), 0, 1, 1, 3);

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
                                     btn::sound_items::enemy_death_ninja_14, btn::fixed_size(9, 10), 2, 3, 1, 3);

    constexpr const enemy_move_event woman_moves[] = {
        enemy_move_event(btn::fixed_point(0.75, move_y(0.25)),  75, false),
        enemy_move_event(btn::fixed_point(0,    move_y(-0.75)), 1,  false),
    };


    // knight:

    constexpr const enemy_data knight(btn::sprite_items::stage_1_characters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
                                      btn::sound_items::scream_3, btn::fixed_size(12, 10), 4, 5, 7, 6);

    constexpr const enemy_move_event knight_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0)), 1, false),
    };

    constexpr const enemy_move_event knight_flipped_moves[] = {
        knight_moves[0].flipped()
    };


    // outlaw:

    constexpr const enemy_data outlaw(btn::sprite_items::stage_1_characters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
                                      btn::sound_items::scream_5, btn::fixed_size(10, 10), 6, 7, 2, 5);

    constexpr const enemy_move_event outlaw_moves[] = {
        enemy_move_event(btn::fixed_point(0.5, move_y(0.25)), 60, false),
        enemy_move_event(btn::fixed_point(0,   move_y(0)),    1,  false),
    };

    constexpr const enemy_move_event outlaw_flipped_moves[] = {
        outlaw_moves[0].flipped(),
        outlaw_moves[1].flipped(),
    };

    constexpr const enemy_bullet_event outlaw_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0, move_y(0), 1), 30),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 60),
    };


    // bat:

    constexpr const enemy_data bat(btn::sprite_items::stage_1_monsters_1, enemy_data::death_anim_type::ROTATE,
                                   btn::sound_items::dinosaur_5, btn::fixed_size(14, 8), 0, 1, 1, 1);

    constexpr const enemy_move_event bat_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0.25)), 1, false),
    };

    constexpr const enemy_move_event bat_flipped_moves[] = {
        bat_moves[0].flipped(),
    };


    // goblin:

    constexpr const enemy_data goblin(btn::sprite_items::stage_1_monsters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
                                      btn::sound_items::enemy_death_ninja_15, btn::fixed_size(13, 10), 2, 3, 2, 7);

    constexpr const enemy_move_event goblin_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0.125)), 1, false),
    };

    constexpr const enemy_move_event goblin_flipped_moves[] = {
        goblin_moves[0].flipped(),
    };

    constexpr const enemy_bullet_event goblin_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,  move_y(0), 1), 45),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, move_y(0), 1), 1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,  move_y(0), 1), 90),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, move_y(0), 1), 1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,  move_y(0), 1), 90),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, move_y(0), 1), 1),
    };


    // characters_1_team:

    constexpr const enemy_data characters_1_team(
        btn::sprite_items::stage_1_characters_1_team, enemy_data::death_anim_type::EXPLOSION,
        btn::sound_items::boss_shoot, btn::fixed_size(47, 47), 0, 1, 50, 100);

    constexpr const enemy_move_event characters_1_team_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(-0.25)), 220, false),
        enemy_move_event(btn::fixed_point(0.25, 0),          320, false),
        enemy_move_event(btn::fixed_point(-0.25, 0),         320, false),
        enemy_move_event(btn::fixed_point(0, move_y(-0.25)), 1,   false),
    };

    constexpr const enemy_bullet_event characters_1_team_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::BIG,   1, 150),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::BIG,   1, 90),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::BIG,   1, 90),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::BIG,   1, 90),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::BIG,   1, 90),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::BIG,   1, 90),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::BIG,   1, 90),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 6),
    };


    // horse:

    constexpr const enemy_data horse(btn::sprite_items::stage_1_animals, enemy_data::death_anim_type::VERTICAL_SCALE,
                                     btn::sound_items::death_8, btn::fixed_size(16, 13), 4, 5, 8, 5);

    constexpr const enemy_move_event horse_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(-0.1)), 1, false),
    };

    constexpr const enemy_move_event horse_flipped_moves[] = {
        horse_moves[0].flipped(),
    };


    // wild_boar:

    constexpr const enemy_data wild_boar(btn::sprite_items::stage_1_animals, enemy_data::death_anim_type::VERTICAL_SCALE,
                                     btn::sound_items::bulls, btn::fixed_size(14, 8), 6, 7, 5, 4);

    constexpr const enemy_move_event wild_boar_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(-0.1)), 1, false),
    };

    constexpr const enemy_move_event wild_boar_flipped_moves[] = {
        wild_boar_moves[0].flipped(),
    };


    // cowboy:

    constexpr const enemy_data cowboy(btn::sprite_items::stage_1_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
                                      btn::sound_items::scream_5, btn::fixed_size(10, 10), 0, 1, 4, 6);

    constexpr const enemy_move_event cowboy_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0.25)), 60, false),
        enemy_move_event(btn::fixed_point(0, move_y(0)),    1,  false),
    };

    constexpr const enemy_move_event cowboy_flipped_moves[] = {
        cowboy_moves[0].flipped(),
        cowboy_moves[1].flipped(),
    };

    constexpr const enemy_bullet_event cowboy_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 45),
    };


    // thief:

    constexpr const enemy_data thief(btn::sprite_items::stage_1_characters_2, enemy_data::death_anim_type::ROTATE,
                                     btn::sound_items::scream_6, btn::fixed_size(10, 12), 2, 3, 2, 4);

    constexpr const enemy_move_event thief_moves[] = {
        enemy_move_event(btn::fixed_point(0.75, move_y(0.15)), 75, false),
        enemy_move_event(btn::fixed_point(0.75, move_y(-1.1)), 1,  false),
    };

    constexpr const enemy_move_event thief_flipped_moves[] = {
        thief_moves[0].flipped(),
        thief_moves[1].flipped(),
    };


    // chinese:

    constexpr const enemy_data chinese(btn::sprite_items::stage_1_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
                                       btn::sound_items::enemy_death_ninja_12, btn::fixed_size(12, 12), 4, 5, 5, 8);

    constexpr const enemy_move_event chinese_moves[] = {
        enemy_move_event(btn::fixed_point(0.5, move_y(0)), 60, false),
        enemy_move_event(btn::fixed_point(0,   move_y(0)), 1,  false),
    };

    constexpr const enemy_move_event chinese_flipped_moves[] = {
        chinese_moves[0].flipped(),
        chinese_moves[1].flipped(),
    };

    constexpr const enemy_bullet_event chinese_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,     move_y(0),   0.85), 60),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1,    move_y(0),   0.85), 1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,     move_y(0),   0.85), 1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-0.65, move_y(0.5), 0.85), 1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0.65,  move_y(0.5), 0.85), 1),
    };


    // blacksmith:

    constexpr const enemy_data blacksmith(btn::sprite_items::stage_1_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
                                          btn::sound_items::scream_2, btn::fixed_size(13, 12), 6, 7, 7, 9);

    constexpr const enemy_move_event blacksmith_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0)), 1, false),
    };

    constexpr const enemy_move_event blacksmith_flipped_moves[] = {
        blacksmith_moves[0].flipped()
    };

    constexpr const enemy_bullet_event blacksmith_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-0.65, move_y(0.5), 1), 60),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0.65,  move_y(0.5), 1), 60),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-0.65, move_y(0.5), 1), 60),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0.65,  move_y(0.5), 1), 60),
    };

    constexpr const enemy_bullet_event blacksmith_flipped_bullets[] = {
        blacksmith_bullets[0].flipped(),
        blacksmith_bullets[1].flipped(),
        blacksmith_bullets[2].flipped(),
        blacksmith_bullets[3].flipped(),
    };


    // skeleton:

    constexpr const enemy_data skeleton(btn::sprite_items::stage_1_monsters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
                                        btn::sound_items::hit_2, btn::fixed_size(13, 11), 0, 1, 14, 10);

    constexpr const enemy_move_event skeleton_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0)), 1, false),
    };

    constexpr const enemy_move_event skeleton_flipped_moves[] = {
        skeleton_moves[0].flipped()
    };


    // jelly:

    constexpr const enemy_data jelly(btn::sprite_items::stage_1_monsters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
                                     btn::sound_items::alert, btn::fixed_size(13, 7), 2, 3, 1, 2);

    constexpr const enemy_move_event jelly_moves[] = {
        enemy_move_event(btn::fixed_point(0, move_y(0.25)), 1, false),
    };

    constexpr const enemy_move_event jelly_flipped_moves[] = {
        jelly_moves[0].flipped()
    };


    // enemy events:

    constexpr const enemy_event enemy_events[] = {

        // level 0

        /*enemy_event(pig, btn::fixed_point(-45, start_y), pig_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(pig, btn::fixed_point(-60, start_y), pig_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(pig, btn::fixed_point(-30, start_y), pig_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(sheep, btn::fixed_point(40, start_y), sheep_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(sheep, btn::fixed_point(55, start_y), sheep_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(sheep, btn::fixed_point(25, start_y), sheep_flipped_moves, no_bullets, 120, enemy_drop_type::NONE),

        // level 1 slow

        enemy_event(man, btn::fixed_point(-45, start_y), man_moves, man_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(-60, start_y), man_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(-30, start_y), man_moves, man_bullets, 60, enemy_drop_type::NONE),

        enemy_event(knight, btn::fixed_point(40, start_y), knight_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(knight, btn::fixed_point(55, start_y), knight_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(knight, btn::fixed_point(25, start_y), knight_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(woman, btn::fixed_point(-30, start_y), woman_moves, no_bullets, 60, enemy_drop_type::GEM),
        enemy_event(pig, btn::fixed_point(-30, start_y), pig_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(sheep, btn::fixed_point(55, start_y), sheep_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(bat, btn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(bat, btn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(man, btn::fixed_point(40, start_y), man_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(55, start_y), man_flipped_moves, man_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(25, start_y), man_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(outlaw, btn::fixed_point(-25, start_y), outlaw_moves, outlaw_bullets, 60, enemy_drop_type::NONE),

        enemy_event(pig, btn::fixed_point(45, start_y), pig_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(man, btn::fixed_point(-45, start_y), man_moves, man_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(-60, start_y), man_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(-30, start_y), man_moves, man_bullets, 60, enemy_drop_type::NONE),
        enemy_event(outlaw, btn::fixed_point(25, start_y), outlaw_flipped_moves, outlaw_bullets, 60, enemy_drop_type::NONE),

        enemy_event(sheep, btn::fixed_point(-40, start_y), sheep_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(goblin, btn::fixed_point(25, start_y), goblin_flipped_moves, goblin_bullets, 60, enemy_drop_type::NONE),
        enemy_event(goblin, btn::fixed_point(40, start_y), goblin_flipped_moves, goblin_bullets, 60, enemy_drop_type::NONE),

        enemy_event(bat, btn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(goblin, btn::fixed_point(-25, start_y), goblin_moves, goblin_bullets, 60, enemy_drop_type::NONE),
        enemy_event(goblin, btn::fixed_point(-40, start_y), goblin_moves, goblin_bullets, 60, enemy_drop_type::NONE),

        // level 1 fast

        enemy_event(knight, btn::fixed_point(40, start_y), knight_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(knight, btn::fixed_point(55, start_y), knight_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(knight, btn::fixed_point(25, start_y), knight_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(woman, btn::fixed_point(-30, start_y), woman_moves, no_bullets, 45, enemy_drop_type::GEM),
        enemy_event(pig, btn::fixed_point(-30, start_y), pig_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(sheep, btn::fixed_point(55, start_y), sheep_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(man, btn::fixed_point(-45, start_y), man_moves, man_bullets, 45, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(-60, start_y), man_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(-30, start_y), man_moves, man_bullets, 45, enemy_drop_type::NONE),
        enemy_event(outlaw, btn::fixed_point(25, start_y), outlaw_flipped_moves, outlaw_bullets, 60, enemy_drop_type::NONE),

        enemy_event(bat, btn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(goblin, btn::fixed_point(25, start_y), goblin_flipped_moves, goblin_bullets, 45, enemy_drop_type::NONE),
        enemy_event(goblin, btn::fixed_point(55, start_y), goblin_flipped_moves, goblin_bullets, 60, enemy_drop_type::NONE),

        enemy_event(sheep, btn::fixed_point(-40, start_y), sheep_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(man, btn::fixed_point(40, start_y), man_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(55, start_y), man_flipped_moves, man_bullets, 45, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(25, start_y), man_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(outlaw, btn::fixed_point(-25, start_y), outlaw_moves, outlaw_bullets, 60, enemy_drop_type::NONE),

        enemy_event(bat, btn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(man, btn::fixed_point(-25, start_y), man_flipped_moves, man_bullets, 45, enemy_drop_type::NONE),
        enemy_event(outlaw, btn::fixed_point(55, start_y), outlaw_flipped_moves, outlaw_bullets, 60, enemy_drop_type::NONE),

        enemy_event(pig, btn::fixed_point(45, start_y), pig_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(sheep, btn::fixed_point(-40, start_y), sheep_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(goblin, btn::fixed_point(0, start_y), goblin_moves, goblin_bullets, 60, enemy_drop_type::NONE),
        enemy_event(goblin, btn::fixed_point(0, start_y), goblin_flipped_moves, goblin_bullets, 60, enemy_drop_type::NONE),
        enemy_event(goblin, btn::fixed_point(0, start_y), goblin_moves, goblin_bullets, 60, enemy_drop_type::NONE),
        enemy_event(goblin, btn::fixed_point(0, start_y), goblin_flipped_moves, goblin_bullets, 60, enemy_drop_type::NONE),

        enemy_event(characters_1_team, btn::fixed_point(-40, start_y - 16), characters_1_team_moves, characters_1_team_bullets, 0, enemy_drop_type::HERO_BOMB),*/

        // level 2

        enemy_event(wild_boar, btn::fixed_point(-30, start_y), wild_boar_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(wild_boar, btn::fixed_point(-45, start_y), wild_boar_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(horse, btn::fixed_point(25, start_y), horse_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(horse, btn::fixed_point(40, start_y), horse_flipped_moves, no_bullets, 90, enemy_drop_type::NONE),

        enemy_event(cowboy, btn::fixed_point(-45, start_y), cowboy_moves, cowboy_bullets, 60, enemy_drop_type::NONE),
        enemy_event(cowboy, btn::fixed_point(-30, start_y), cowboy_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(cowboy, btn::fixed_point(-60, start_y), cowboy_moves, cowboy_bullets, 60, enemy_drop_type::NONE),

        enemy_event(thief, btn::fixed_point(-60, start_y), thief_moves, no_bullets, 60, enemy_drop_type::GEM),

        enemy_event(chinese, btn::fixed_point(25, start_y), chinese_flipped_moves, chinese_bullets, 90, enemy_drop_type::NONE),
        enemy_event(chinese, btn::fixed_point(40, start_y), chinese_flipped_moves, chinese_bullets, 60, enemy_drop_type::NONE),

        enemy_event(thief, btn::fixed_point(55, start_y), thief_flipped_moves, no_bullets, 60, enemy_drop_type::GEM),

        enemy_event(horse, btn::fixed_point(25, start_y), horse_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(horse, btn::fixed_point(40, start_y), horse_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(blacksmith, btn::fixed_point(-45, start_y), blacksmith_moves, blacksmith_flipped_bullets, 90, enemy_drop_type::NONE),
        enemy_event(blacksmith, btn::fixed_point(-30, start_y), blacksmith_moves, blacksmith_flipped_bullets, 60, enemy_drop_type::NONE),

        enemy_event(wild_boar, btn::fixed_point(25, start_y), wild_boar_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(wild_boar, btn::fixed_point(40, start_y), wild_boar_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(skeleton, btn::fixed_point(0, start_y), skeleton_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(skeleton, btn::fixed_point(-30, start_y), skeleton_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(skeleton, btn::fixed_point(25, start_y), skeleton_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(skeleton, btn::fixed_point(-60, start_y), skeleton_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(skeleton, btn::fixed_point(55, start_y), skeleton_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(thief, btn::fixed_point(-60, start_y), thief_moves, no_bullets, 60, enemy_drop_type::GEM),

        // level 2 fast

        enemy_event(jelly, btn::fixed_point(-45, start_y), jelly_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-30, start_y), jelly_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-60, start_y), jelly_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(cowboy, btn::fixed_point(-45, start_y), cowboy_moves, cowboy_bullets, 45, enemy_drop_type::NONE),
        enemy_event(cowboy, btn::fixed_point(-60, start_y), cowboy_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(cowboy, btn::fixed_point(-30, start_y), cowboy_moves, cowboy_bullets, 45, enemy_drop_type::NONE),
        enemy_event(outlaw, btn::fixed_point(25, start_y), outlaw_flipped_moves, outlaw_bullets, 30, enemy_drop_type::NONE),

        enemy_event(jelly, btn::fixed_point(10, start_y), jelly_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(25, start_y), jelly_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(40, start_y), jelly_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(thief, btn::fixed_point(55, start_y), thief_flipped_moves, no_bullets, 30, enemy_drop_type::GEM),

        enemy_event(man, btn::fixed_point(-45, start_y), man_moves, man_bullets, 45, enemy_drop_type::NONE),
        enemy_event(man, btn::fixed_point(-60, start_y), man_moves, man_bullets, 45, enemy_drop_type::NONE),
        enemy_event(chinese, btn::fixed_point(25, start_y), chinese_flipped_moves, chinese_bullets, 45, enemy_drop_type::NONE),

        enemy_event(horse, btn::fixed_point(-60, start_y), horse_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-30, start_y), jelly_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(thief, btn::fixed_point(-60, start_y), thief_moves, no_bullets, 30, enemy_drop_type::GEM),

        enemy_event(blacksmith, btn::fixed_point(-60, start_y), blacksmith_moves, blacksmith_flipped_bullets, 1, enemy_drop_type::NONE),
        enemy_event(blacksmith, btn::fixed_point(-5, start_y), blacksmith_moves, blacksmith_flipped_bullets, 1, enemy_drop_type::NONE),
        enemy_event(blacksmith, btn::fixed_point(55, start_y), blacksmith_flipped_moves, blacksmith_bullets, 60, enemy_drop_type::NONE),

        enemy_event(jelly, btn::fixed_point(-30, start_y), jelly_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(skeleton, btn::fixed_point(0, start_y), skeleton_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(skeleton, btn::fixed_point(-30, start_y), skeleton_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(skeleton, btn::fixed_point(25, start_y), skeleton_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(skeleton, btn::fixed_point(-60, start_y), skeleton_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(skeleton, btn::fixed_point(55, start_y), skeleton_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(chinese, btn::fixed_point(25, start_y), chinese_flipped_moves, chinese_bullets, 30, enemy_drop_type::NONE),

        enemy_event(wild_boar, btn::fixed_point(-30, start_y), wild_boar_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(thief, btn::fixed_point(55, start_y), thief_flipped_moves, no_bullets, 45, enemy_drop_type::GEM),

        enemy_event(bat, btn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, btn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(jelly, btn::fixed_point(10, start_y), jelly_flipped_moves, no_bullets, 25, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(40, start_y), jelly_flipped_moves, no_bullets, 25, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(25, start_y), jelly_flipped_moves, no_bullets, 25, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(55, start_y), jelly_flipped_moves, no_bullets, 25, enemy_drop_type::NONE),

        enemy_event(jelly, btn::fixed_point(-15, start_y), jelly_moves, no_bullets, 25, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-45, start_y), jelly_moves, no_bullets, 25, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-30, start_y), jelly_moves, no_bullets, 25, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-60, start_y), jelly_moves, no_bullets, 25, enemy_drop_type::NONE),

        enemy_event(jelly, btn::fixed_point(10, start_y), jelly_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-45, start_y), jelly_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(40, start_y), jelly_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-30, start_y), jelly_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(25, start_y), jelly_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-60, start_y), jelly_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(55, start_y), jelly_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-15, start_y), jelly_moves, no_bullets, 20, enemy_drop_type::NONE),

        enemy_event(jelly, btn::fixed_point(10, start_y), jelly_flipped_moves, no_bullets, 15, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-45, start_y), jelly_moves, no_bullets, 15, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(40, start_y), jelly_flipped_moves, no_bullets, 15, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-30, start_y), jelly_moves, no_bullets, 15, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(25, start_y), jelly_flipped_moves, no_bullets, 15, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-60, start_y), jelly_moves, no_bullets, 15, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(55, start_y), jelly_flipped_moves, no_bullets, 15, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-15, start_y), jelly_moves, no_bullets, 15, enemy_drop_type::NONE),

        enemy_event(jelly, btn::fixed_point(10, start_y), jelly_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-45, start_y), jelly_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(40, start_y), jelly_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-30, start_y), jelly_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(25, start_y), jelly_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-60, start_y), jelly_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(55, start_y), jelly_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-15, start_y), jelly_moves, no_bullets, 10, enemy_drop_type::NONE),

        enemy_event(jelly, btn::fixed_point(10, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-45, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(40, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-30, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(25, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-60, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(55, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-15, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),

        enemy_event(jelly, btn::fixed_point(10, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-45, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(40, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-30, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(25, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-60, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(55, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, btn::fixed_point(-15, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),
    };


    // stage:

    constexpr const stage stage_1(
        btn::regular_bg_items::stage_1, btn::regular_bg_items::clouds, btn::sprite_items::stage_1_intro,
        btn::sprite_items::stage_1_intro_alt, "STAGE 1", "DWARF LAND", btn::music_items::cyberrid, enemy_events);
}

const stage& get()
{
    return stage_1;
}

}
