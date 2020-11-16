/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_PALETTES_MANAGER_H
#define BTN_PALETTES_MANAGER_H

#include "btn_common.h"

namespace btn
{

class palettes_bank;

namespace palettes_manager
{
    [[nodiscard]] palettes_bank& sprite_palettes_bank();

    [[nodiscard]] palettes_bank& bg_palettes_bank();

    void update();

    void commit();

    void stop();
}

}

#endif
