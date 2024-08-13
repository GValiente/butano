/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_hdma.h"

#include "bn_span.h"
#include "bn_assert.h"
#include "bn_display.h"
#include "bn_hdma_manager.h"

namespace bn::hdma
{

bool running()
{
    return hdma_manager::low_priority_running();
}

void start(const bn::span<const uint16_t>& source_ref, uint16_t& destination_ref)
{
    int source_size = source_ref.size();
    BN_ASSERT(source_size % bn::display::height() == 0, "Invalid source size: ", source_size);

    int elements = source_size / bn::display::height();
    BN_ASSERT(elements > 0, "Invalid elements: ", source_size);

    hdma_manager::low_priority_start(*source_ref.data(), elements, destination_ref);
}

void start(const uint16_t& source_ref, int elements, uint16_t& destination_ref)
{
    BN_ASSERT(elements > 0, "Invalid elements: ", elements);

    hdma_manager::low_priority_start(source_ref, elements, destination_ref);
}

void stop()
{
    hdma_manager::low_priority_stop();
}

bool high_priority_running()
{
    return hdma_manager::high_priority_running();
}

void high_priority_start(const bn::span<const uint16_t>& source_ref, uint16_t& destination_ref)
{
    int source_size = source_ref.size();
    BN_ASSERT(source_size % bn::display::height() == 0, "Invalid source size: ", source_size);

    int elements = source_size / bn::display::height();
    BN_ASSERT(elements > 0, "Invalid elements: ", source_size);

    hdma_manager::high_priority_start(*source_ref.data(), elements, destination_ref);
}

void high_priority_start(const uint16_t& source_ref, int elements, uint16_t& destination_ref)
{
    BN_ASSERT(elements > 0, "Invalid elements: ", elements);

    hdma_manager::high_priority_start(source_ref, elements, destination_ref);
}

void high_priority_stop()
{
    hdma_manager::high_priority_stop();
}

}
