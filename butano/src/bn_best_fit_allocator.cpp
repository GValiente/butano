/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_best_fit_allocator.h"

#include "bn_memory.h"
#include "bn_limits.h"
#include "bn_alignment.h"

#if BN_CFG_LOG_ENABLED
    #include "bn_log.h"
#endif

namespace bn
{

namespace
{
    constexpr best_fit_allocator::size_type alignment_bytes = sizeof(int);

    [[nodiscard]] best_fit_allocator::size_type _aligned_bytes(best_fit_allocator::size_type bytes)
    {
        if(best_fit_allocator::size_type extra_bytes = bytes % alignment_bytes)
        {
            bytes += alignment_bytes - extra_bytes;
        }

        return bytes;
    }
}

best_fit_allocator::~best_fit_allocator() noexcept
{
    BN_ASSERT(empty(), "Allocator is not empty");
}

void* best_fit_allocator::alloc(size_type bytes)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);

    bytes = _aligned_bytes(bytes) + _sizeof_item;

    if(bytes > _free_bytes_count)
    {
        return nullptr;
    }

    item_type* item = _best_free_item(bytes);

    if(! item)
    {
        return nullptr;
    }

    size_type new_item_size = item->size - bytes;

    if(new_item_size > _sizeof_item)
    {
        item->size = bytes;

        item_type* new_item = item->next();
        new_item->previous = item;
        new_item->previous_free = item->previous_free;
        new_item->next_free = item->next_free;
        new_item->size = new_item_size;
        new_item->used = false;

        item_type* new_next_item = new_item->next();

        if(new_next_item != _end_item())
        {
            new_next_item->previous = new_item;
        }

        if(item_type* previous_free_item = item->previous_free)
        {
            previous_free_item->next_free = new_item;
        }
        else
        {
            _first_free_item = new_item;
        }

        if(item_type* next_free_item = item->next_free)
        {
            next_free_item->previous_free = new_item;
        }
    }
    else
    {
        item_type* next_free_item = item->next_free;

        if(item_type* previous_free_item = item->previous_free)
        {
            previous_free_item->next_free = item->next_free;
        }
        else
        {
            _first_free_item = next_free_item;
        }

        if(next_free_item)
        {
            next_free_item->previous_free = item->previous_free;
        }
    }

    item->previous_free = nullptr;
    item->next_free = nullptr;
    item->used = true;
    _free_bytes_count -= item->size;

    return reinterpret_cast<uint8_t*>(item) + _sizeof_item;
}

void* best_fit_allocator::calloc(size_type num, size_type bytes)
{
    BN_ASSERT(num >= 0, "Invalid num: ", num);
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);

    bytes *= num;

    void* result = alloc(bytes);

    if(result)
    {
        auto int_result = reinterpret_cast<int*>(result);
        memory::clear(_aligned_bytes(bytes) / size_type(sizeof(int)), *int_result);
    }

    return result;
}

void* best_fit_allocator::realloc(void* ptr, size_type new_bytes)
{
    if(! ptr)
    {
        return alloc(new_bytes);
    }

    BN_ASSERT(new_bytes >= 0, "Invalid new bytes: ", new_bytes);

    uint8_t* item_ptr = static_cast<uint8_t*>(ptr) - _sizeof_item;
    auto item = reinterpret_cast<item_type*>(item_ptr);
    size_type old_bytes = item->size - _sizeof_item;

    if(new_bytes <= old_bytes)
    {
        return ptr;
    }

    void* new_ptr = alloc(new_bytes);

    if(! new_ptr)
    {
        return nullptr;
    }

    auto old_ptr_data = reinterpret_cast<const int*>(ptr);
    auto new_ptr_data = reinterpret_cast<int*>(new_ptr);
    memory::copy(*old_ptr_data, old_bytes / 4, *new_ptr_data);
    free(ptr);
    return new_ptr;
}

void best_fit_allocator::free(void* ptr)
{
    if(! ptr)
    {
        return;
    }

    uint8_t* item_ptr = static_cast<uint8_t*>(ptr) - _sizeof_item;
    auto item = reinterpret_cast<item_type*>(item_ptr);
    bool item_linked = false;
    item->used = false;
    _free_bytes_count += item->size;

    if(item_type* previous_item = item->previous)
    {
        if(! previous_item->used)
        {
            previous_item->size += item->size;
            item = previous_item;
            item_linked = true;
        }
    }

    item_type* next_item = item->next();
    item_type* end_item = _end_item();

    if(next_item != end_item)
    {
        if(! next_item->used)
        {
            item_linked = true;

            if(item_type* previous_free_item = next_item->previous_free)
            {
                if(previous_free_item == item)
                {
                    item->next_free = next_item->next_free;
                }
                else
                {
                    previous_free_item->next_free = item;
                    item->previous_free = previous_free_item;
                }
            }

            if(item_type* next_free_item = next_item->next_free)
            {
                next_free_item->previous_free = item;
                item->next_free = next_free_item;
            }

            if(next_item == _first_free_item)
            {
                _first_free_item = nullptr;
            }

            item->size += next_item->size;
            next_item = item->next();
        }

        if(next_item != end_item)
        {
            next_item->previous = item;
        }
    }

    if(item_type* first_free_item = _first_free_item)
    {
        if(! item_linked)
        {
            item_type* free_item = first_free_item;
            item_type* previous_free_item = nullptr;

            while(free_item)
            {
                if(free_item < item)
                {
                    previous_free_item = free_item;
                }
                else
                {
                    free_item->previous_free = item;
                    item->next_free = free_item;
                    break;
                }

                free_item = free_item->next_free;
            }

            if(previous_free_item)
            {
                previous_free_item->next_free = item;
                item->previous_free = previous_free_item;
            }
        }

        if(first_free_item > item)
        {
            _first_free_item = item;
        }
    }
    else
    {
        _first_free_item = item;
    }
}

void best_fit_allocator::reset(void* start, size_type bytes)
{
    BN_ASSERT(bytes >= 0 && bytes % size_type(sizeof(int)) == 0, "Invalid bytes: ", bytes);
    BN_ASSERT(empty(), "Allocator is not empty");

    if(bytes >= _sizeof_item)
    {
        BN_ASSERT(start, "Start is null");
        BN_ASSERT(aligned<alignment_bytes>(start), "Start is not aligned");

        auto first_item = reinterpret_cast<item_type*>(start);
        first_item->previous = nullptr;
        first_item->previous_free = nullptr;
        first_item->next_free = nullptr;
        first_item->size = bytes;
        first_item->used = false;

        _start_ptr = static_cast<uint8_t*>(start);
        _first_free_item = first_item;
        _total_bytes_count = bytes;
        _free_bytes_count = bytes;
    }
    else
    {
        _start_ptr = nullptr;
        _first_free_item = nullptr;
        _total_bytes_count = 0;
        _free_bytes_count = 0;
    }
}

#if BN_CFG_LOG_ENABLED
    void best_fit_allocator::log_status() const
    {
        BN_LOG("items: ");
        BN_LOG('[');

        const item_type* item = _begin_item();
        const item_type* end_item = _end_item();

        while(item != end_item)
        {
            BN_LOG("    ",
                   item->used ? "used" : "free",
                   " - size: ", item->size);

            item = item->next();
        }

        BN_LOG(']');
        BN_LOG("free_bytes_count: ", _free_bytes_count);
        BN_LOG("total_bytes_count: ", _total_bytes_count);
    }
#endif

best_fit_allocator::item_type* best_fit_allocator::_best_free_item(size_type bytes)
{
    item_type* free_item = _first_free_item;
    item_type* best_free_item = nullptr;
    size_type best_free_item_bytes = numeric_limits<size_type>::max();

    while(free_item)
    {
        size_type free_item_bytes = free_item->size;

        if(free_item_bytes == bytes)
        {
            return free_item;
        }

        if(free_item_bytes > bytes && free_item_bytes < best_free_item_bytes)
        {
            best_free_item = free_item;
            best_free_item_bytes = free_item_bytes;
        }

        free_item = free_item->next_free;
    }

    return best_free_item;
}

}
