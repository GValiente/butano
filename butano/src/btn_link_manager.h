/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_LINK_MANAGER_H
#define BTN_LINK_MANAGER_H

#include "btn_vector_fwd.h"

namespace btn
{
    class link_player;
}

namespace btn::link_manager
{
    void init();

    [[nodiscard]] bool tick(int data_to_send, int& current_player_id, vector<link_player, 3>& other_players);

    void enable();

    void disable();
}

#endif
