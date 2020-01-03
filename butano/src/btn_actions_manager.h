#ifndef BTN_ACTIONS_MANAGER_H
#define BTN_ACTIONS_MANAGER_H

#include "btn_common.h"

namespace btn
{

class action;

namespace actions_manager
{
    using action_type = action;

    void run(action_type& action);

    void stop(action_type& action);

    void update();
}

}

#endif
