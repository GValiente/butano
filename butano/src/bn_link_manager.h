/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_LINK_MANAGER_H
#define BN_LINK_MANAGER_H

#include "bn_vector_fwd.h"

namespace bn
{
    class link_player;
}

namespace bn::link_manager
{
    void init();

    void send(int data_to_send);

    [[nodiscard]] bool get(int& current_player_id, vector<link_player, 3>& other_players);

    void enable();

    void disable();

    void update();
}

#endif
