/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_GREEN_SWAP_ACTIONS_H
#define BTN_GREEN_SWAP_ACTIONS_H

#include "btn_green_swap.h"
#include "btn_template_actions.h"

namespace btn
{

class green_swap_manager
{

public:
    [[nodiscard]] static bool get()
    {
        return green_swap::enabled();
    }

    static void set(bool enabled)
    {
        green_swap::set_enabled(enabled);
    }
};


class green_swap_toggle_action : public bool_toggle_template_action<green_swap_manager>
{

public:
    explicit green_swap_toggle_action(int duration_updates) :
        bool_toggle_template_action(duration_updates)
    {
    }
};

}

#endif
