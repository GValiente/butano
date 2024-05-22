/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 jwellbelove, https://github.com/ETLCPP/etl, http://www.etlcpp.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "bn_generic_pool.h"

namespace bn
{

igeneric_pool_base::~igeneric_pool_base() noexcept
{
    BN_BASIC_ASSERT(empty(), "Pool is not empty");
}

igeneric_pool_base::igeneric_pool_base(char* buffer, size_type max_size) :
    _buffer(buffer),
    _next_ptr(buffer),
    _max_size(max_size)
{
}

char* igeneric_pool_base::_allocate()
{
    size_type size = _allocated_items_count;
    size_type max_size = _max_size;
    BN_BASIC_ASSERT(size != max_size, "Pool is full");

    char* result = _next_ptr;
    ++size;

    if(size != max_size)
    {
        _next_ptr = *reinterpret_cast<char**>(result);
    }
    else
    {
        _next_ptr = nullptr;
    }

    _allocated_items_count = size;
    return result;
}

void igeneric_pool_base::_free(char* ptr)
{
    auto typed_ptr = reinterpret_cast<uintptr_t*>(ptr);

    if(_next_ptr)
    {
        *typed_ptr = reinterpret_cast<uintptr_t>(_next_ptr);
    }
    else
    {
        *typed_ptr = 0;
    }

    _next_ptr = ptr;
    --_allocated_items_count;
}

}
