/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_GREEN_SWAP_HBLANK_EFFECT_PTR_H
#define BTN_GREEN_SWAP_HBLANK_EFFECT_PTR_H

#include "btn_optional_fwd.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class green_swap_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static green_swap_hblank_effect_ptr create(const span<const bool>& states_ref);

    [[nodiscard]] static optional<green_swap_hblank_effect_ptr> create_optional(const span<const bool>& states_ref);

    green_swap_hblank_effect_ptr(const green_swap_hblank_effect_ptr& other) = default;

    green_swap_hblank_effect_ptr& operator=(const green_swap_hblank_effect_ptr& other) = default;

    green_swap_hblank_effect_ptr(green_swap_hblank_effect_ptr&& other) = default;

    green_swap_hblank_effect_ptr& operator=(green_swap_hblank_effect_ptr&& other) = default;

    [[nodiscard]] span<const bool> states_ref() const;

    void set_states_ref(const span<const bool>& states_ref);

    void reload_states_ref();

    void swap(green_swap_hblank_effect_ptr& other);

    friend void swap(green_swap_hblank_effect_ptr& a, green_swap_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    green_swap_hblank_effect_ptr(int id);
};

}

#endif
