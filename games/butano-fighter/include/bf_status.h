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

    [[nodiscard]] bool how_to_play_viewed() const
    {
        return _how_to_play_viewed;
    }

    void mark_how_to_play_as_viewed();

private:
    const game::stage* _current_stage;
    int _level = 6;
    int _experience = 3500;
    int _bombs_count = 2;
    int _high_experience = 0;
    bool _how_to_play_viewed = false;
};

}

#endif
