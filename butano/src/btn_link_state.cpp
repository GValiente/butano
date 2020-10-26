/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_link_state.h"

#include "btn_optional.h"
#include "btn_link_manager.h"

namespace btn
{

optional<link_state> link_state::get(int data_to_send)
{
    BTN_ASSERT(data_to_send >= 0 && data_to_send <= 32767, "Invalid data to send: ", data_to_send);

    optional<link_state> result = link_state();
    link_state& result_value = result.value();

    if(! link_manager::tick(data_to_send, result_value._current_player_id, result_value._other_players))
    {
        result.reset();
    }

    return result;
}

}
