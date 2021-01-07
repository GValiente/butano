/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_PAUSE_H
#define BF_GAME_PAUSE_H

#include "bn_fixed.h"
#include "bn_optional.h"
#include "bn_regular_bg_ptr.h"

namespace bf
{
    class butano_background;
}

namespace bf::game
{

class pause
{

public:
    [[nodiscard]] bool active() const
    {
        return _active;
    }

    void update(const butano_background& butano_background);

private:
    bn::optional<bn::regular_bg_ptr> _bg;
    bn::fixed _contrast;
    bn::fixed _fade_intensity;
    bool _active = false;
    bool _green_swap = false;
};

}

#endif
