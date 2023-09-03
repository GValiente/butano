/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_GAME_PAK_H
#define BN_HW_GAME_PAK_H

#include "bn_common.h"

namespace bn::hw::game_pak
{
    [[nodiscard]] BN_CODE_EWRAM int _slow_game_pak();

    [[nodiscard]] bool init();
}

#endif
