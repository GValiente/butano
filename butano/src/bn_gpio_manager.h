/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_GPIO_MANAGER_H
#define BN_GPIO_MANAGER_H

#include "bn_common.h"

namespace bn::gpio_manager
{
    [[nodiscard]] bool rumble_enabled();

    void set_rumble_enabled(bool enabled);

    void commit();

    void sleep();

    void wake_up();

    void stop();
}

#endif
