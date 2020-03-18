#ifndef BF_HERO_BOMB_H
#define BF_HERO_BOMB_H

#include "btn_bgs_mosaic_actions.h"
#include "btn_regular_bg_actions.h"
#include "btn_regular_bg_attributes.h"

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

    hero_bomb(const hero_bomb& other) = delete;

    hero_bomb& operator=(const hero_bomb& other) = delete;

    [[nodiscard]] status_type status() const
    {
        return _status;
    }

    void update(hero& hero);

private:
    btn::regular_bg_ptr _bg;
    btn::regular_bg_move_by_action _bg_move_action;
    btn::bgs_mosaic_stretch_loop_action _bgs_mosaic_action;
    btn::fixed _old_blending_transparency_alpha;
    btn::regular_bg_attributes _bg_attributes[160];
    status_type _status = status_type::INACTIVE;
    int _counter = 0;
};

}

#endif
