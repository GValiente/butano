/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bf_game_stage_1.h"

#include "bn_display.h"
#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_regular_bg_items_clouds.h"
#include "bn_regular_bg_items_stage_1.h"
#include "bn_sprite_items_stage_1_intro.h"
#include "bn_sprite_items_stage_1_intro_alt.h"
#include "bn_sprite_items_stage_1_animals.h"
#include "bn_sprite_items_stage_1_monsters_1.h"
#include "bn_sprite_items_stage_1_monsters_2.h"
#include "bn_sprite_items_stage_1_characters_1.h"
#include "bn_sprite_items_stage_1_characters_2.h"
#include "bn_sprite_items_stage_1_characters_1_team.h"
#include "bf_constants.h"
#include "bf_game_bullet_util.h"

namespace bf::game::stage_1
{

namespace
{
    constexpr int start_y = -(bn::display::height() + 16) / 2;

    constexpr bn::span<const enemy_bullet_event> no_bullets;

    [[nodiscard]] constexpr bn::fixed move_y(bn::fixed y)
    {
        return y + constants::background_speed;
    }


    // pig:

    constexpr enemy_data pig(bn::sprite_items::stage_1_animals, enemy_data::death_anim_type::VERTICAL_SCALE,
                                   bn::sound_items::dinosaur_2, bn::fixed_size(16, 13), 2, 3, 1, 1);

    constexpr enemy_move_event pig_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(-0.1)), 1, false),
    };

    constexpr enemy_move_event pig_flipped_moves[] = {
        pig_moves[0].flipped(),
    };


    // sheep:

    constexpr enemy_data sheep(bn::sprite_items::stage_1_animals, enemy_data::death_anim_type::VERTICAL_SCALE,
                                     bn::sound_items::kill, bn::fixed_size(16, 12), 0, 1, 2, 2);

    constexpr enemy_move_event sheep_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(-0.1)), 1, false),
    };

    constexpr enemy_move_event sheep_flipped_moves[] = {
        sheep_moves[0].flipped(),
    };


    // man:

    constexpr enemy_data man(bn::sprite_items::stage_1_characters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
                                   bn::sound_items::scream_4, bn::fixed_size(10, 10), 0, 1, 1, 3);

    constexpr enemy_move_event man_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.25)), 60, false),
        enemy_move_event(bn::fixed_point(0, move_y(0)),    1,  false),
    };

    constexpr enemy_move_event man_flipped_moves[] = {
        man_moves[0].flipped(),
        man_moves[1].flipped(),
    };

    constexpr enemy_bullet_event man_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 60),
    };


    // woman:

    constexpr enemy_data woman(bn::sprite_items::stage_1_characters_1, enemy_data::death_anim_type::ROTATE,
                                     bn::sound_items::enemy_death_ninja_14, bn::fixed_size(9, 10), 2, 3, 1, 3);

    constexpr enemy_move_event woman_moves[] = {
        enemy_move_event(bn::fixed_point(0.75, move_y(0.25)),  75, false),
        enemy_move_event(bn::fixed_point(0,    move_y(-0.75)), 1,  false),
    };


    // knight:

    constexpr enemy_data knight(bn::sprite_items::stage_1_characters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
                                      bn::sound_items::scream_3, bn::fixed_size(12, 10), 4, 5, 6, 6);

    constexpr enemy_move_event knight_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0)), 1, false),
    };

    constexpr enemy_move_event knight_flipped_moves[] = {
        knight_moves[0].flipped()
    };


    // outlaw:

    constexpr enemy_data outlaw(bn::sprite_items::stage_1_characters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
                                      bn::sound_items::scream_5, bn::fixed_size(10, 10), 6, 7, 2, 5);

    constexpr enemy_move_event outlaw_moves[] = {
        enemy_move_event(bn::fixed_point(0.5, move_y(0.25)), 60, false),
        enemy_move_event(bn::fixed_point(0,   move_y(0)),    1,  false),
    };

    constexpr enemy_move_event outlaw_flipped_moves[] = {
        outlaw_moves[0].flipped(),
        outlaw_moves[1].flipped(),
    };

    constexpr enemy_bullet_event outlaw_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0, move_y(0), 1), 30),
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 60),
    };


    // bat:

    constexpr enemy_data bat(bn::sprite_items::stage_1_monsters_1, enemy_data::death_anim_type::ROTATE,
                                   bn::sound_items::dinosaur_5, bn::fixed_size(14, 8), 0, 1, 1, 1);

    constexpr enemy_move_event bat_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.25)), 1, false),
    };

    constexpr enemy_move_event bat_flipped_moves[] = {
        bat_moves[0].flipped(),
    };


    // goblin:

    constexpr enemy_data goblin(bn::sprite_items::stage_1_monsters_1, enemy_data::death_anim_type::VERTICAL_SCALE,
                                      bn::sound_items::enemy_death_ninja_15, bn::fixed_size(13, 10), 2, 3, 2, 7);

    constexpr enemy_move_event goblin_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.125)), 1, false),
    };

    constexpr enemy_move_event goblin_flipped_moves[] = {
        goblin_moves[0].flipped(),
    };

    constexpr enemy_bullet_event goblin_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,  move_y(0), 1), 45),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, move_y(0), 1), 1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,  move_y(0), 1), 90),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, move_y(0), 1), 1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,  move_y(0), 1), 90),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1, move_y(0), 1), 1),
    };


    // characters_1_team:

    constexpr enemy_data characters_1_team(
        bn::sprite_items::stage_1_characters_1_team, enemy_data::death_anim_type::EXPLOSION,
        bn::sound_items::boss_shoot, bn::fixed_size(47, 47), 0, 1, 50, 50);

    constexpr enemy_move_event characters_1_team_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(-0.25)), 220, false),
        enemy_move_event(bn::fixed_point(0.25, 0),          320, false),
        enemy_move_event(bn::fixed_point(-0.25, 0),         320, true),
        enemy_move_event(bn::fixed_point(0, move_y(-0.25)), 1,   false),
    };

    constexpr enemy_bullet_event characters_1_team_bullets[] = {
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

    constexpr enemy_data horse(bn::sprite_items::stage_1_animals, enemy_data::death_anim_type::VERTICAL_SCALE,
                                     bn::sound_items::death_8, bn::fixed_size(16, 13), 4, 5, 8, 5);

    constexpr enemy_move_event horse_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(-0.1)), 1, false),
    };

    constexpr enemy_move_event horse_flipped_moves[] = {
        horse_moves[0].flipped(),
    };


    // wild_boar:

    constexpr enemy_data wild_boar(bn::sprite_items::stage_1_animals, enemy_data::death_anim_type::VERTICAL_SCALE,
                                     bn::sound_items::bulls, bn::fixed_size(14, 8), 6, 7, 5, 4);

    constexpr enemy_move_event wild_boar_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(-0.1)), 1, false),
    };

    constexpr enemy_move_event wild_boar_flipped_moves[] = {
        wild_boar_moves[0].flipped(),
    };


    // cowboy:

    constexpr enemy_data cowboy(bn::sprite_items::stage_1_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
                                      bn::sound_items::scream_5, bn::fixed_size(10, 10), 0, 1, 4, 6);

    constexpr enemy_move_event cowboy_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.25)), 60, false),
        enemy_move_event(bn::fixed_point(0, move_y(0)),    1,  false),
    };

    constexpr enemy_bullet_event cowboy_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, 1, 45),
    };


    // thief:

    constexpr enemy_data thief(bn::sprite_items::stage_1_characters_2, enemy_data::death_anim_type::ROTATE,
                                     bn::sound_items::scream_6, bn::fixed_size(10, 12), 2, 3, 2, 4);

    constexpr enemy_move_event thief_moves[] = {
        enemy_move_event(bn::fixed_point(0.75, move_y(0.15)), 75, false),
        enemy_move_event(bn::fixed_point(0.75, move_y(-1.1)), 1,  false),
    };

    constexpr enemy_move_event thief_flipped_moves[] = {
        thief_moves[0].flipped(),
        thief_moves[1].flipped(),
    };


    // chinese:

    constexpr enemy_data chinese(bn::sprite_items::stage_1_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
                                       bn::sound_items::enemy_death_ninja_12, bn::fixed_size(12, 12), 4, 5, 5, 8);

    constexpr enemy_move_event chinese_moves[] = {
        enemy_move_event(bn::fixed_point(0.5, move_y(0)), 60, false),
        enemy_move_event(bn::fixed_point(0,   move_y(0)), 1,  false),
    };

    constexpr enemy_move_event chinese_flipped_moves[] = {
        chinese_moves[0].flipped(),
        chinese_moves[1].flipped(),
    };

    constexpr enemy_bullet_event chinese_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0,     move_y(0),   0.85), 60),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-1,    move_y(0),   0.85), 1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(1,     move_y(0),   0.85), 1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-0.65, move_y(0.5), 0.85), 1),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0.65,  move_y(0.5), 0.85), 1),
    };


    // blacksmith:

    constexpr enemy_data blacksmith(bn::sprite_items::stage_1_characters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
                                          bn::sound_items::scream_2, bn::fixed_size(13, 12), 6, 7, 7, 9);

    constexpr enemy_move_event blacksmith_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0)), 1, false),
    };

    constexpr enemy_move_event blacksmith_flipped_moves[] = {
        blacksmith_moves[0].flipped()
    };

    constexpr enemy_bullet_event blacksmith_bullets[] = {
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-0.65, move_y(0.5), 1), 60),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0.65,  move_y(0.5), 1), 60),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(-0.65, move_y(0.5), 1), 60),
        enemy_bullet_event(enemy_bullet_type::SMALL, direction_vector(0.65,  move_y(0.5), 1), 60),
    };

    constexpr enemy_bullet_event blacksmith_flipped_bullets[] = {
        blacksmith_bullets[0].flipped(),
        blacksmith_bullets[1].flipped(),
        blacksmith_bullets[2].flipped(),
        blacksmith_bullets[3].flipped(),
    };


    // skeleton:

    constexpr enemy_data skeleton(bn::sprite_items::stage_1_monsters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
                                        bn::sound_items::hit_2, bn::fixed_size(13, 11), 0, 1, 10, 10);

    constexpr enemy_move_event skeleton_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0)), 1, false),
    };

    constexpr enemy_move_event skeleton_flipped_moves[] = {
        skeleton_moves[0].flipped()
    };


    // jelly:

    constexpr enemy_data jelly(bn::sprite_items::stage_1_monsters_2, enemy_data::death_anim_type::VERTICAL_SCALE,
                                     bn::sound_items::alert, bn::fixed_size(13, 7), 2, 3, 1, 2);

    constexpr enemy_move_event jelly_moves[] = {
        enemy_move_event(bn::fixed_point(0, move_y(0.25)), 1, false),
    };

    constexpr enemy_move_event jelly_flipped_moves[] = {
        jelly_moves[0].flipped()
    };


    // enemy events:

    constexpr enemy_event enemy_events[] = {

        // level 0

        enemy_event(pig, bn::fixed_point(-45, start_y), pig_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(pig, bn::fixed_point(-60, start_y), pig_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(pig, bn::fixed_point(-30, start_y), pig_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(sheep, bn::fixed_point(40, start_y), sheep_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(sheep, bn::fixed_point(55, start_y), sheep_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(sheep, bn::fixed_point(25, start_y), sheep_flipped_moves, no_bullets, 120, enemy_drop_type::NONE),

        // level 1 slow

        enemy_event(man, bn::fixed_point(-45, start_y), man_moves, man_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, bn::fixed_point(-60, start_y), man_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, bn::fixed_point(-30, start_y), man_moves, man_bullets, 60, enemy_drop_type::NONE),

        enemy_event(knight, bn::fixed_point(40, start_y), knight_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(knight, bn::fixed_point(55, start_y), knight_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(knight, bn::fixed_point(25, start_y), knight_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(woman, bn::fixed_point(-30, start_y), woman_moves, no_bullets, 60, enemy_drop_type::GEM),
        enemy_event(pig, bn::fixed_point(-30, start_y), pig_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(sheep, bn::fixed_point(55, start_y), sheep_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(bat, bn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(bat, bn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(man, bn::fixed_point(40, start_y), man_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, bn::fixed_point(55, start_y), man_flipped_moves, man_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, bn::fixed_point(25, start_y), man_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(outlaw, bn::fixed_point(-25, start_y), outlaw_moves, outlaw_bullets, 60, enemy_drop_type::NONE),

        enemy_event(pig, bn::fixed_point(45, start_y), pig_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(man, bn::fixed_point(-45, start_y), man_moves, man_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, bn::fixed_point(-60, start_y), man_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(man, bn::fixed_point(-30, start_y), man_moves, man_bullets, 60, enemy_drop_type::NONE),
        enemy_event(outlaw, bn::fixed_point(25, start_y), outlaw_flipped_moves, outlaw_bullets, 60, enemy_drop_type::NONE),

        enemy_event(sheep, bn::fixed_point(-40, start_y), sheep_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(goblin, bn::fixed_point(25, start_y), goblin_flipped_moves, goblin_bullets, 60, enemy_drop_type::NONE),
        enemy_event(goblin, bn::fixed_point(40, start_y), goblin_flipped_moves, goblin_bullets, 60, enemy_drop_type::NONE),

        enemy_event(bat, bn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(goblin, bn::fixed_point(-25, start_y), goblin_moves, goblin_bullets, 60, enemy_drop_type::NONE),
        enemy_event(goblin, bn::fixed_point(-40, start_y), goblin_moves, goblin_bullets, 60, enemy_drop_type::NONE),

        // level 1 fast

        enemy_event(knight, bn::fixed_point(40, start_y), knight_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(knight, bn::fixed_point(55, start_y), knight_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(knight, bn::fixed_point(25, start_y), knight_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(woman, bn::fixed_point(-30, start_y), woman_moves, no_bullets, 45, enemy_drop_type::GEM),
        enemy_event(pig, bn::fixed_point(-30, start_y), pig_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(sheep, bn::fixed_point(55, start_y), sheep_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(man, bn::fixed_point(-45, start_y), man_moves, man_bullets, 45, enemy_drop_type::NONE),
        enemy_event(man, bn::fixed_point(-60, start_y), man_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(man, bn::fixed_point(-30, start_y), man_moves, man_bullets, 45, enemy_drop_type::NONE),
        enemy_event(outlaw, bn::fixed_point(25, start_y), outlaw_flipped_moves, outlaw_bullets, 60, enemy_drop_type::NONE),

        enemy_event(bat, bn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(goblin, bn::fixed_point(25, start_y), goblin_flipped_moves, goblin_bullets, 45, enemy_drop_type::NONE),
        enemy_event(goblin, bn::fixed_point(55, start_y), goblin_flipped_moves, goblin_bullets, 60, enemy_drop_type::NONE),

        enemy_event(sheep, bn::fixed_point(-40, start_y), sheep_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(man, bn::fixed_point(40, start_y), man_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(man, bn::fixed_point(55, start_y), man_flipped_moves, man_bullets, 45, enemy_drop_type::NONE),
        enemy_event(man, bn::fixed_point(25, start_y), man_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(outlaw, bn::fixed_point(-25, start_y), outlaw_moves, outlaw_bullets, 60, enemy_drop_type::NONE),

        enemy_event(bat, bn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(-40, start_y), bat_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(man, bn::fixed_point(-25, start_y), man_flipped_moves, man_bullets, 45, enemy_drop_type::NONE),
        enemy_event(outlaw, bn::fixed_point(55, start_y), outlaw_flipped_moves, outlaw_bullets, 60, enemy_drop_type::NONE),

        enemy_event(pig, bn::fixed_point(45, start_y), pig_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(sheep, bn::fixed_point(-40, start_y), sheep_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(goblin, bn::fixed_point(0, start_y), goblin_moves, goblin_bullets, 60, enemy_drop_type::NONE),
        enemy_event(goblin, bn::fixed_point(0, start_y), goblin_flipped_moves, goblin_bullets, 60, enemy_drop_type::NONE),
        enemy_event(goblin, bn::fixed_point(0, start_y), goblin_moves, goblin_bullets, 60, enemy_drop_type::NONE),
        enemy_event(goblin, bn::fixed_point(0, start_y), goblin_flipped_moves, goblin_bullets, 60, enemy_drop_type::NONE),

        enemy_event(characters_1_team, bn::fixed_point(-40, start_y - 16), characters_1_team_moves, characters_1_team_bullets, 0, enemy_drop_type::HERO_BOMB),

        // level 2

        enemy_event(wild_boar, bn::fixed_point(-30, start_y), wild_boar_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(wild_boar, bn::fixed_point(-45, start_y), wild_boar_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(horse, bn::fixed_point(25, start_y), horse_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(horse, bn::fixed_point(40, start_y), horse_flipped_moves, no_bullets, 90, enemy_drop_type::NONE),

        enemy_event(cowboy, bn::fixed_point(-45, start_y), cowboy_moves, cowboy_bullets, 60, enemy_drop_type::NONE),
        enemy_event(cowboy, bn::fixed_point(-30, start_y), cowboy_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(cowboy, bn::fixed_point(-60, start_y), cowboy_moves, cowboy_bullets, 60, enemy_drop_type::NONE),

        enemy_event(thief, bn::fixed_point(-60, start_y), thief_moves, no_bullets, 60, enemy_drop_type::GEM),

        enemy_event(chinese, bn::fixed_point(25, start_y), chinese_flipped_moves, chinese_bullets, 90, enemy_drop_type::NONE),
        enemy_event(chinese, bn::fixed_point(40, start_y), chinese_flipped_moves, chinese_bullets, 60, enemy_drop_type::NONE),

        enemy_event(thief, bn::fixed_point(55, start_y), thief_flipped_moves, no_bullets, 60, enemy_drop_type::GEM),

        enemy_event(horse, bn::fixed_point(25, start_y), horse_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(horse, bn::fixed_point(40, start_y), horse_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(blacksmith, bn::fixed_point(-45, start_y), blacksmith_moves, blacksmith_flipped_bullets, 90, enemy_drop_type::NONE),
        enemy_event(blacksmith, bn::fixed_point(-30, start_y), blacksmith_moves, blacksmith_flipped_bullets, 60, enemy_drop_type::NONE),

        enemy_event(wild_boar, bn::fixed_point(25, start_y), wild_boar_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(wild_boar, bn::fixed_point(40, start_y), wild_boar_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(skeleton, bn::fixed_point(0, start_y), skeleton_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(skeleton, bn::fixed_point(-30, start_y), skeleton_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(skeleton, bn::fixed_point(25, start_y), skeleton_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),
        enemy_event(skeleton, bn::fixed_point(-60, start_y), skeleton_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(skeleton, bn::fixed_point(55, start_y), skeleton_flipped_moves, no_bullets, 60, enemy_drop_type::NONE),

        enemy_event(thief, bn::fixed_point(-60, start_y), thief_moves, no_bullets, 60, enemy_drop_type::GEM),

        // level 2 fast

        enemy_event(jelly, bn::fixed_point(-45, start_y), jelly_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-30, start_y), jelly_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-60, start_y), jelly_moves, no_bullets, 45, enemy_drop_type::NONE),

        enemy_event(cowboy, bn::fixed_point(-45, start_y), cowboy_moves, cowboy_bullets, 45, enemy_drop_type::NONE),
        enemy_event(cowboy, bn::fixed_point(-60, start_y), cowboy_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(cowboy, bn::fixed_point(-30, start_y), cowboy_moves, cowboy_bullets, 45, enemy_drop_type::NONE),
        enemy_event(outlaw, bn::fixed_point(25, start_y), outlaw_flipped_moves, outlaw_bullets, 30, enemy_drop_type::NONE),

        enemy_event(jelly, bn::fixed_point(10, start_y), jelly_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(25, start_y), jelly_flipped_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(40, start_y), jelly_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(thief, bn::fixed_point(55, start_y), thief_flipped_moves, no_bullets, 30, enemy_drop_type::GEM),

        enemy_event(man, bn::fixed_point(-45, start_y), man_moves, man_bullets, 45, enemy_drop_type::NONE),
        enemy_event(man, bn::fixed_point(-60, start_y), man_moves, man_bullets, 45, enemy_drop_type::NONE),
        enemy_event(chinese, bn::fixed_point(25, start_y), chinese_flipped_moves, chinese_bullets, 45, enemy_drop_type::NONE),

        enemy_event(horse, bn::fixed_point(-60, start_y), horse_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-30, start_y), jelly_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(thief, bn::fixed_point(-60, start_y), thief_moves, no_bullets, 60, enemy_drop_type::GEM),

        enemy_event(blacksmith, bn::fixed_point(-60, start_y), blacksmith_moves, blacksmith_flipped_bullets, 1, enemy_drop_type::NONE),
        enemy_event(blacksmith, bn::fixed_point(55, start_y), blacksmith_flipped_moves, blacksmith_bullets, 60, enemy_drop_type::NONE),

        enemy_event(blacksmith, bn::fixed_point(0, start_y), blacksmith_moves, blacksmith_flipped_bullets, 60, enemy_drop_type::NONE),

        enemy_event(jelly, bn::fixed_point(-30, start_y), jelly_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(skeleton, bn::fixed_point(0, start_y), skeleton_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(skeleton, bn::fixed_point(-30, start_y), skeleton_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(skeleton, bn::fixed_point(25, start_y), skeleton_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(skeleton, bn::fixed_point(-60, start_y), skeleton_moves, no_bullets, 1, enemy_drop_type::NONE),
        enemy_event(skeleton, bn::fixed_point(55, start_y), skeleton_flipped_moves, no_bullets, 45, enemy_drop_type::NONE),
        enemy_event(chinese, bn::fixed_point(25, start_y), chinese_flipped_moves, chinese_bullets, 30, enemy_drop_type::NONE),

        enemy_event(wild_boar, bn::fixed_point(-30, start_y), wild_boar_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(thief, bn::fixed_point(55, start_y), thief_flipped_moves, no_bullets, 45, enemy_drop_type::GEM),

        enemy_event(bat, bn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),
        enemy_event(bat, bn::fixed_point(0, start_y), bat_flipped_moves, no_bullets, 30, enemy_drop_type::NONE),

        enemy_event(jelly, bn::fixed_point(10, start_y), jelly_flipped_moves, no_bullets, 25, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(40, start_y), jelly_flipped_moves, no_bullets, 25, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(25, start_y), jelly_flipped_moves, no_bullets, 25, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(55, start_y), jelly_flipped_moves, no_bullets, 25, enemy_drop_type::NONE),

        enemy_event(jelly, bn::fixed_point(-15, start_y), jelly_moves, no_bullets, 25, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-45, start_y), jelly_moves, no_bullets, 25, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-30, start_y), jelly_moves, no_bullets, 25, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-60, start_y), jelly_moves, no_bullets, 25, enemy_drop_type::NONE),

        enemy_event(jelly, bn::fixed_point(10, start_y), jelly_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-45, start_y), jelly_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(40, start_y), jelly_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-30, start_y), jelly_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(25, start_y), jelly_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-60, start_y), jelly_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(55, start_y), jelly_flipped_moves, no_bullets, 20, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-15, start_y), jelly_moves, no_bullets, 20, enemy_drop_type::NONE),

        enemy_event(jelly, bn::fixed_point(10, start_y), jelly_flipped_moves, no_bullets, 15, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-45, start_y), jelly_moves, no_bullets, 15, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(40, start_y), jelly_flipped_moves, no_bullets, 15, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-30, start_y), jelly_moves, no_bullets, 15, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(25, start_y), jelly_flipped_moves, no_bullets, 15, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-60, start_y), jelly_moves, no_bullets, 15, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(55, start_y), jelly_flipped_moves, no_bullets, 15, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-15, start_y), jelly_moves, no_bullets, 15, enemy_drop_type::NONE),

        enemy_event(jelly, bn::fixed_point(10, start_y), jelly_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-45, start_y), jelly_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(40, start_y), jelly_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-30, start_y), jelly_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(25, start_y), jelly_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-60, start_y), jelly_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(55, start_y), jelly_flipped_moves, no_bullets, 10, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-15, start_y), jelly_moves, no_bullets, 10, enemy_drop_type::NONE),

        enemy_event(jelly, bn::fixed_point(10, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-45, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(40, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-30, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(25, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-60, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(55, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-15, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),

        enemy_event(jelly, bn::fixed_point(10, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-45, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(40, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-30, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(25, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-60, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(55, start_y), jelly_flipped_moves, no_bullets, 5, enemy_drop_type::NONE),
        enemy_event(jelly, bn::fixed_point(-15, start_y), jelly_moves, no_bullets, 5, enemy_drop_type::NONE),
    };


    // stage (start with level 0, exp 0):

    constexpr stage stage_1(
        bn::regular_bg_items::stage_1, bn::fixed_point(0, constants::background_speed),
        bn::regular_bg_items::clouds, bn::fixed_point(-1.0 / 16, constants::background_speed),
        bn::sprite_items::stage_1_intro, bn::sprite_items::stage_1_intro_alt, "STAGE 1", "DWARF LAND",
        bn::music_items::cyberrid, 0.4, enemy_events, boss::type::TANK, false);
}

const stage& get()
{
    return stage_1;
}

}
