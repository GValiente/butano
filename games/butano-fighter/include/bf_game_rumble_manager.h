/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_RUMBLE_MANAGER_H
#define BF_GAME_RUMBLE_MANAGER_H

#include "bn_rumble.h"

namespace bf::game
{

class rumble_manager
{

public:
    explicit rumble_manager(bool rumble_enabled) :
        _rumble_enabled(rumble_enabled)
    {
    }

    [[nodiscard]] bool enabled()
    {
        return bn::rumble::enabled();
    }

    void set_enabled(bool enabled)
    {
        if(_rumble_enabled)
        {
            bn::rumble::set_enabled(enabled);
        }
    }

private:
    bool _rumble_enabled;
};

}

#endif
