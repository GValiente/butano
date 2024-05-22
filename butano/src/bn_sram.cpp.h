/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sram.h"

#include "bn_span.h"
#include "../hw/include/bn_hw_sram.h"

namespace _bn::sram
{

void unsafe_read(void* destination, int size, int offset)
{
    bn::hw::sram::read(destination, size, offset);
}

void unsafe_write(const void* source, int size, int offset)
{
    bn::hw::sram::write(source, size, offset);
}

}


namespace bn::sram
{

void read_span(span<uint8_t>& destination)
{
    int destination_size = destination.size();
    BN_ASSERT(destination_size <= size(), "Destination size is too high: ", destination_size);

    _bn::sram::unsafe_read(destination.data(), destination_size, 0);
}

void read_span_offset(span<uint8_t>& destination, int offset)
{
    int destination_size = destination.size();
    BN_ASSERT(offset >= 0, "Invalid offset: ", offset);
    BN_ASSERT(destination_size + offset <= size(),
              "Destination size and offset are too high: ", destination_size, " - ", offset);

    _bn::sram::unsafe_read(destination.data(), destination_size, offset);
}

void write_span(const span<const uint8_t>& source)
{
    int source_size = source.size();
    BN_ASSERT(source_size <= size(), "Source size is too high: ", source_size);

    _bn::sram::unsafe_write(source.data(), source_size, 0);
}

void write_span_offset(const span<const uint8_t>& source, int offset)
{
    int source_size = source.size();
    BN_ASSERT(offset >= 0, "Invalid offset: ", offset);
    BN_ASSERT(source_size + offset <= size(),
              "Source size and offset are too high: ", source_size, " - ", offset);

    _bn::sram::unsafe_write(source.data(), source_size, offset);
}

void clear(int bytes)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BN_ASSERT(bytes <= size(), "Bytes is too high: ", bytes);

    hw::sram::set_bytes(0, bytes, 0);
}

void clear(int bytes, int offset)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BN_ASSERT(offset >= 0, "Invalid offset: ", offset);
    BN_ASSERT(bytes + offset <= size(), "Bytes and offset are too high: ", bytes, " - ", offset);

    hw::sram::set_bytes(0, bytes, offset);
}

void set_bytes(uint8_t value, int bytes)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BN_ASSERT(bytes <= size(), "Bytes is too high: ", bytes);

    hw::sram::set_bytes(value, bytes, 0);
}

void set_bytes(uint8_t value, int bytes, int offset)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);
    BN_ASSERT(offset >= 0, "Invalid offset: ", offset);
    BN_ASSERT(bytes + offset <= size(), "Bytes and offset are too high: ", bytes, " - ", offset);

    hw::sram::set_bytes(value, bytes, offset);
}

}
