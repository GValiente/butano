#ifndef BF_BACKGROUND_H
#define BF_BACKGROUND_H

#include "btn_regular_bg_actions.h"

namespace bf
{

class background
{

public:
    background();

    void update();

private:
    btn::regular_bg_move_by_action _ground_bg_move_action;
    btn::regular_bg_move_by_action _clouds_bg_move_action;
};

}

#endif
