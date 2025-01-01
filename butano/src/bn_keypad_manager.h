/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_KEYPAD_MANAGER_H
#define BN_KEYPAD_MANAGER_H

#include "bn_span.h"
#include "bn_keypad.h"
#include "bn_string_view.h"

namespace bn::keypad_manager
{
    using key_type = keypad::key_type;

    void init(const string_view& commands);

    [[nodiscard]] bool held(key_type key);

    [[nodiscard]] bool pressed(key_type key);

    [[nodiscard]] bool released(key_type key);

    [[nodiscard]] bool any_held();

    [[nodiscard]] bool any_pressed();

    [[nodiscard]] bool any_released();

    void update();

    void set_interrupt(const span<const key_type>& keys);

    void stop();
}

#endif
