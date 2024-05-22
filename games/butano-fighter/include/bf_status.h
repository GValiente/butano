/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_STATUS_H
#define BF_STATUS_H

#include "bf_game_stage.h"

namespace bf
{

class status
{

public:
    status();

    [[nodiscard]] const game::stage& current_stage() const
    {
        return *_current_stage;
    }

    [[nodiscard]] bool go_to_next_stage();

    [[nodiscard]] int level() const
    {
        return _level;
    }

    [[nodiscard]] bool add_level();

    [[nodiscard]] int experience() const
    {
        return _experience;
    }

    [[nodiscard]] bn::fixed next_level_experience_ratio() const;

    [[nodiscard]] bool add_experience(int experience);

    [[nodiscard]] int bombs_count() const
    {
        return _bombs_count;
    }

    [[nodiscard]] bool add_bomb();

    [[nodiscard]] bool throw_bomb();

    [[nodiscard]] bool throw_shield();

    [[nodiscard]] int high_experience() const
    {
        return _high_experience;
    }

    void update_high_experience();

    [[nodiscard]] bool rumble_enabled() const
    {
        return _rumble_enabled;
    }

    void set_rumble_enabled(bool rumble_enabled);

    [[nodiscard]] bool how_to_play_viewed() const
    {
        return _how_to_play_viewed;
    }

    void mark_how_to_play_as_viewed();

private:
    const game::stage* _current_stage;
    int _level = 0;
    int _experience = 0;
    int _bombs_count = 2;
    int _high_experience = 0;
    bool _rumble_enabled = true;
    bool _how_to_play_viewed = false;
};

}

#endif
