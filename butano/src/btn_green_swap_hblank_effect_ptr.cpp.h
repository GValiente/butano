/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_green_swap_hblank_effect_ptr.h"

#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

green_swap_hblank_effect_ptr green_swap_hblank_effect_ptr::create(const span<const bool>& states_ref)
{
    int id = hblank_effects_manager::create(states_ref.data(), states_ref.size(), 0,
                                            hblank_effects_manager::handler_type::GREEN_SWAP);
    return green_swap_hblank_effect_ptr(id);
}

optional<green_swap_hblank_effect_ptr> green_swap_hblank_effect_ptr::create_optional(const span<const bool>& states_ref)
{
    int id = hblank_effects_manager::create_optional(states_ref.data(), states_ref.size(), 0,
                                                     hblank_effects_manager::handler_type::GREEN_SWAP);
    optional<green_swap_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = green_swap_hblank_effect_ptr(id);
    }

    return result;
}

span<const bool> green_swap_hblank_effect_ptr::states_ref() const
{
    auto values_ptr = reinterpret_cast<const bool*>(hblank_effects_manager::values_ref(id()));
    return span<const bool>(values_ptr, display::height());
}

void green_swap_hblank_effect_ptr::set_states_ref(const span<const bool>& states_ref)
{
    hblank_effects_manager::set_values_ref(id(), states_ref.data(), states_ref.size());
}

void green_swap_hblank_effect_ptr::reload_states_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void green_swap_hblank_effect_ptr::swap(green_swap_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
}

green_swap_hblank_effect_ptr::green_swap_hblank_effect_ptr(int id) :
    hblank_effect_ptr(id)
{
}

}
