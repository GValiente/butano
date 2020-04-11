#ifndef BF_BUTANO_BACKGROUND_H
#define BF_BUTANO_BACKGROUND_H

#include "btn_optional.h"
#include "btn_regular_bg_actions.h"

namespace bf
{

class butano_background
{

public:
    butano_background();

    void update();

private:
    btn::optional<btn::regular_bg_move_by_action> _move_action;
};

}

#endif
