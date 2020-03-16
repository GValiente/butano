#ifndef BF_HERO_BOMB_H
#define BF_HERO_BOMB_H

#include "btn_regular_bg_actions.h"

namespace bf
{

class hero;

class hero_bomb
{

public:
    enum class status_type
    {
        INACTIVE,
        ACTIVE,
    };

    hero_bomb();

    [[nodiscard]] status_type status() const
    {
        return _status;
    }

    void update(hero& hero);

private:
    btn::regular_bg_move_by_action _bg_move_action;
    status_type _status = status_type::INACTIVE;
    int _counter = 0;
};

}

#endif
