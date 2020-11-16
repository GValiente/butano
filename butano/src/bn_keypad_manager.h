/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_KEYPAD_MANAGER_H
#define BN_KEYPAD_MANAGER_H

#include "bn_keypad.h"
#include "bn_span_fwd.h"

namespace bn
{
    class string_view;
}

namespace bn::keypad_manager
{
    using key_type = keypad::key_type;

    void init(const string_view& commands);

    [[nodiscard]] bool held(key_type key);

    [[nodiscard]] bool pressed(key_type key);

    [[nodiscard]] bool released(key_type key);

    void update();

    void set_interrupt(const span<const key_type>& keys);

    void stop();
}

#endif
