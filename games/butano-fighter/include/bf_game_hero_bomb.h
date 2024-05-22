/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_HERO_BOMB_H
#define BF_GAME_HERO_BOMB_H

#include "bn_display.h"
#include "bn_blending_actions.h"
#include "bn_regular_bg_actions.h"
#include "bn_rect_window_actions.h"
#include "bn_regular_bg_position_hbe_ptr.h"
#include "bn_rect_window_boundaries_hbe_ptr.h"
#include "bf_circle_generator.h"

namespace bf::game
{

class hero;
class intro;
class enemies;
class background;
class boss_intro;
class enemy_bullets;
class rumble_manager;

class hero_bomb
{

public:
    hero_bomb() = default;

    hero_bomb(const hero_bomb& other) = delete;

    hero_bomb& operator=(const hero_bomb& other) = delete;

    [[nodiscard]] bool active() const
    {
        return _status != status_type::INACTIVE;
    }

    [[nodiscard]] bool closing() const
    {
        return _status == status_type::CLOSE;
    }

    void update(const intro& intro, const boss_intro& boss_intro, const bn::camera_ptr& camera, hero& hero,
                enemies& enemies, enemy_bullets& enemy_bullets, background& background,
                rumble_manager& rumble_manager);

private:
    enum class status_type
    {
        INACTIVE,
        OPEN,
        CLOSE
    };

    bn::point _center;
    bn::optional<bn::regular_bg_move_by_action> _bg_move_action;
    bn::optional<bn::rect_window_move_top_by_action> _move_window_top_action;
    bn::optional<bn::rect_window_move_bottom_by_action> _move_window_bottom_action;
    bn::optional<bn::blending_transparency_alpha_to_action> _transparency_blending_action;
    bn::optional<bn::blending_intensity_alpha_to_action> _intensity_blending_action;
    bn::pair<bn::fixed, bn::fixed> _circle_hbe_deltas[bn::display::height()];
    circle_generator _circle_generator;
    bn::optional<bn::rect_window_boundaries_hbe_ptr> _circle_hbe;
    bn::optional<bn::regular_bg_position_hbe_ptr> _wave_hbe;
    status_type _status = status_type::INACTIVE;
    int _counter = 0;
    int _flame_sound_counter = 0;

    void _play_flame_sound();
};

}

#endif
