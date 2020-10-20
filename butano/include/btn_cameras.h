/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CAMERAS_H
#define BTN_CAMERAS_H

#include "btn_common.h"

namespace btn::cameras
{
    [[nodiscard]] int used_items_count();

    [[nodiscard]] int available_items_count();
}

#endif
