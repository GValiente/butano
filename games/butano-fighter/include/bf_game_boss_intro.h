/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_BOSS_INTRO_H
#define BF_GAME_BOSS_INTRO_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_music_actions.h"
#include "bn_sprite_affine_mat_attributes_hbe_ptr.h"

namespace bf::game
{

class boss_intro
{

public:
    [[nodiscard]] bool active() const
    {
        return _state == state::ACTIVE;
    }

    [[nodiscard]] bool done() const
    {
        return _state == state::DONE;
    }

    void enable();

    void update();

private:
    enum class state
    {
        INIT,
        ACTIVE,
        DONE
    };

    bn::vector<bn::sprite_ptr, 7> _sprites;
    bn::optional<bn::music_volume_to_action> _music_volume_action;
    bn::optional<bn::sprite_affine_mat_attributes_hbe_ptr> _hbe;
    state _state = state::INIT;
    int _loops = 0;
    int _counter = 0;
};

}

#endif
