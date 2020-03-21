#ifndef BF_GAME_BACKGROUND_H
#define BF_GAME_BACKGROUND_H

#include "btn_regular_bg_actions.h"

namespace bf
{

class game_background
{

public:
    game_background();

    void update();

private:
    btn::regular_bg_move_by_action _ground_bg_move_action;
    btn::regular_bg_move_by_action _clouds_bg_move_action;
};

}

#endif
