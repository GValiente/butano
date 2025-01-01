/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_GPIO_MANAGER_H
#define BN_GPIO_MANAGER_H

#include "bn_optional_fwd.h"

namespace bn
{
    class date;
    class time;
}

namespace bn::gpio_manager
{
    [[nodiscard]] bool rtc_active();

    [[nodiscard]] optional<date> current_date();

    [[nodiscard]] optional<time> current_time();

    [[nodiscard]] bool rumble_enabled();

    void set_rumble_enabled(bool enabled);

    void sleep();

    void wake_up();

    inline void stop()
    {
        sleep();
    }
}

#endif
