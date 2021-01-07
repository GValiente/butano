/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_hdma_manager.h"

#include "../hw/include/bn_hw_hdma.h"

#include "bn_hdma.cpp.h"

namespace bn::hdma_manager
{

namespace
{
    class state
    {

    public:
        const uint16_t* source_ptr = nullptr;
        uint16_t* destination_ptr = nullptr;
        int elements = 0;
    };

    class static_data
    {

    public:
        state states[2];
        int8_t current_state_index = 0;
        bool updated = false;
    };

    BN_DATA_EWRAM static_data data;

    [[nodiscard]] state& _current_state()
    {
        return data.states[data.current_state_index];
    }

    [[nodiscard]] state& _next_state()
    {
        return data.states[(data.current_state_index + 1) % 2];
    }
}

void enable()
{
    commit();
}

void disable()
{
    hw::hdma::stop();
}

void start(const uint16_t& source_ref, int elements, uint16_t& destination_ref)
{
    state& next_state = _next_state();
    next_state.source_ptr = &source_ref;
    next_state.destination_ptr = &destination_ref;
    next_state.elements = elements;
    data.updated = true;
}

void stop()
{
    state& next_state = _next_state();
    next_state.elements = 0;
    data.updated = true;
}

void update()
{
    if(data.updated)
    {
        data.updated = false;

        if(data.current_state_index)
        {
            data.current_state_index = 0;
            data.states[1] = data.states[0];
        }
        else
        {
            data.current_state_index = 1;
            data.states[0] = data.states[1];
        }
    }
}

void commit()
{
    const state& current_state = _current_state();

    if(int elements = current_state.elements)
    {
        hw::hdma::start(current_state.source_ptr, elements, current_state.destination_ptr);
    }
    else
    {
        hw::hdma::stop();
    }
}

}
