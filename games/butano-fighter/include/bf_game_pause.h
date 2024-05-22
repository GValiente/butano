/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_PAUSE_H
#define BF_GAME_PAUSE_H

#include "bn_regular_bg_ptr.h"

namespace bf
{
    class butano_background;
}

namespace bf::game
{

class rumble_manager;

class pause
{

public:
    [[nodiscard]] bool active() const
    {
        return _active;
    }

    void update(const butano_background& butano_background, rumble_manager& rumble_manager);

private:
    bn::optional<bn::regular_bg_ptr> _bg;
    bn::fixed _contrast;
    bn::fixed _fade_intensity;
    bool _active = false;
    bool _green_swap = false;
    bool _rumble = false;
};

}

#endif
