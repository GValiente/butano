/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_PAUSE_H
#define BF_GAME_PAUSE_H

#include "btn_fixed.h"
#include "btn_optional.h"
#include "btn_regular_bg_ptr.h"

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
    btn::optional<btn::regular_bg_ptr> _bg;
    btn::fixed _contrast;
    btn::fixed _fade_intensity;
    bool _active = false;
    bool _green_swap = false;
};

}

#endif
