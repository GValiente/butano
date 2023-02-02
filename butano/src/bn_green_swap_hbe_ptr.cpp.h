/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_green_swap_hbe_ptr.h"

#include "bn_display.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

green_swap_hbe_ptr green_swap_hbe_ptr::create(const span<const bool>& states_ref)
{
    int id = hblank_effects_manager::create(states_ref.data(), states_ref.size(), 0,
                                            hblank_effects_manager::handler_type::GREEN_SWAP);
    return green_swap_hbe_ptr(id);
}

optional<green_swap_hbe_ptr> green_swap_hbe_ptr::create_optional(const span<const bool>& states_ref)
{
    int id = hblank_effects_manager::create_optional(states_ref.data(), states_ref.size(), 0,
                                                     hblank_effects_manager::handler_type::GREEN_SWAP);
    optional<green_swap_hbe_ptr> result;

    if(id >= 0)
    {
        result = green_swap_hbe_ptr(id);
    }

    return result;
}

span<const bool> green_swap_hbe_ptr::states_ref() const
{
    auto values_ptr = reinterpret_cast<const bool*>(hblank_effects_manager::values_ref(id()));
    return span<const bool>(values_ptr, display::height());
}

void green_swap_hbe_ptr::set_states_ref(const span<const bool>& states_ref)
{
    hblank_effects_manager::set_values_ref(id(), states_ref.data(), states_ref.size());
}

void green_swap_hbe_ptr::reload_states_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void green_swap_hbe_ptr::swap(green_swap_hbe_ptr& other)
{
    hbe_ptr::swap(other);
}

green_swap_hbe_ptr::green_swap_hbe_ptr(int id) :
    hbe_ptr(id)
{
}

}
