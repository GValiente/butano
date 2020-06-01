#ifndef BF_GAME_STATUS_H
#define BF_GAME_STATUS_H

#include "bf_game_stage_1.h"

namespace bf::game
{

class status
{

public:
    status();

    [[nodiscard]] const stage& current_stage() const
    {
        return *_current_stage;
    }

    [[nodiscard]] int level() const
    {
        return _level;
    }

    [[nodiscard]] bool add_level();

    [[nodiscard]] int experience() const
    {
        return _experience;
    }

    [[nodiscard]] btn::fixed next_level_experience_ratio() const;

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

private:
    const stage* _current_stage = &stage_1::get();
    int _level = 0;
    int _experience = 0;
    int _bombs_count = 2;
    int _high_experience = 0;
};

}

#endif
