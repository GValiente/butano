/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_memory_manager.h"

#include "bn_list.h"
#include "bn_vector.h"
#include "bn_config_memory.h"
#include "../hw/include/bn_hw_memory.h"

#include "bn_memory.cpp.h"
#include "bn_cstdlib.cpp.h"
#include "bn_cstring.cpp.h"

namespace bn::memory_manager
{

namespace
{
    static_assert(BN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS > 0);


    constexpr int max_items = BN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS;


    class item_type
    {

    public:
        char* data = nullptr;
        int size: 24 = 0;
        bool used = false;
    };


    using items_list = list<item_type, max_items>;
    using items_iterator = items_list::iterator;

    static_assert(sizeof(items_iterator) == sizeof(int));
    static_assert(alignof(items_iterator) == alignof(int));


    class static_data
    {

    public:
        items_list items;
        vector<items_iterator, max_items> free_items;
        int total_bytes_count = 0;
        int free_bytes_count = 0;
    };

    BN_DATA_EWRAM static_data data;

    constexpr auto lower_bound_comparator = [](const items_iterator& items_it, int size)
    {
        return items_it->size < size;
    };

    constexpr auto upper_bound_comparator = [](int size, const items_iterator& items_it)
    {
        return size < items_it->size;
    };

    [[nodiscard]] int _aligned_bytes(int bytes)
    {
        int alignment_bytes = sizeof(int);

        if(int extra_bytes = bytes % alignment_bytes)
        {
            bytes += alignment_bytes - extra_bytes;
        }

        return bytes;
    }

    void _insert_free_item(items_iterator items_it, ivector<items_iterator>::iterator free_items_last)
    {
        auto free_items_it = upper_bound(data.free_items.begin(), free_items_last, items_it->size,
                                         upper_bound_comparator);
        data.free_items.insert(free_items_it, items_it);
    }

    void _insert_free_item(items_iterator items_it)
    {
        _insert_free_item(items_it, data.free_items.end());
    }

    void _erase_free_item(items_iterator items_it)
    {
        auto free_items_end = data.free_items.end();
        auto free_items_it = lower_bound(data.free_items.begin(), free_items_end, items_it->size,
                                         lower_bound_comparator);

        while(*free_items_it != items_it)
        {
            ++free_items_it;
        }

        data.free_items.erase(free_items_it);
    }
}

void init()
{
    char* start = hw::memory::ewram_heap_start();
    char* end = hw::memory::ewram_heap_end();
    data.total_bytes_count = end - start;

    item_type new_item;
    new_item.data = start;
    new_item.size = data.total_bytes_count;
    data.items.push_front(new_item);
    data.free_items.push_back(data.items.begin());
    data.free_bytes_count = data.total_bytes_count;
}

void* ewram_alloc(int bytes)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);

    bytes = _aligned_bytes(bytes) + sizeof(items_iterator);

    if(bytes > data.free_bytes_count)
    {
        return nullptr;
    }

    auto free_items_end = data.free_items.end();
    auto free_items_it = lower_bound(data.free_items.begin(), free_items_end, bytes, lower_bound_comparator);

    if(free_items_it == free_items_end)
    {
        return nullptr;
    }

    items_iterator items_it = *free_items_it;
    item_type& item = *items_it;

    if(int new_item_size = item.size - bytes)
    {
        if(data.items.full())
        {
            return nullptr;
        }

        item_type new_item;
        new_item.data = item.data;
        new_item.size = new_item_size;

        items_iterator new_items_it = data.items.insert(items_it, new_item);
        _insert_free_item(new_items_it, free_items_it);
        ++free_items_it;
        item.data += new_item_size;
        item.size = bytes;
    }

    item.used = true;
    data.free_items.erase(free_items_it);

    items_iterator* items_it_ptr = reinterpret_cast<items_iterator*>(item.data);
    *items_it_ptr = items_it;
    data.free_bytes_count -= bytes;
    return items_it_ptr + 1;
}

void* ewram_calloc(int bytes)
{
    void* result = ewram_alloc(bytes);

    if(result)
    {
        auto int_result = reinterpret_cast<int*>(result);
        memory::clear(_aligned_bytes(bytes) / sizeof(int), *int_result);
    }

    return result;
}

void* ewram_realloc(void* ptr, int new_bytes)
{
    if(! ptr)
    {
        return ewram_alloc(new_bytes);
    }

    items_iterator* items_it_ptr = reinterpret_cast<items_iterator*>(ptr) - 1;
    items_iterator items_it = *items_it_ptr;
    item_type& item = *items_it;
    int old_bytes = item.size - sizeof(items_iterator);

    if(new_bytes <= old_bytes)
    {
        return ptr;
    }

    void* new_ptr = ewram_alloc(new_bytes);

    if(! new_ptr)
    {
        return nullptr;
    }

    auto old_ptr_data = reinterpret_cast<const int*>(ptr);
    auto new_ptr_data = reinterpret_cast<int*>(new_ptr);
    memory::copy(*old_ptr_data, old_bytes / 4, *new_ptr_data);
    ewram_free(ptr);
    return new_ptr;
}

void ewram_free(void* ptr)
{
    if(ptr)
    {
        items_iterator* items_it_ptr = reinterpret_cast<items_iterator*>(ptr) - 1;
        items_iterator items_it = *items_it_ptr;
        item_type& item = *items_it;

        item.used = false;
        data.free_bytes_count += item.size;

        if(items_it != data.items.begin())
        {
            items_iterator previous_items_it = items_it;
            --previous_items_it;

            item_type& previous_item = *previous_items_it;

            if(! previous_item.used && previous_item.data + previous_item.size == item.data)
            {
                item.data = previous_item.data;
                item.size += previous_item.size;
                _erase_free_item(previous_items_it);
                data.items.erase(previous_items_it);
            }
        }

        items_iterator next_items_it = items_it;
        ++next_items_it;

        if(next_items_it != data.items.end())
        {
            item_type& next_item = *next_items_it;

            if(! next_item.used && item.data + item.size == next_item.data)
            {
                item.size += next_item.size;
                _erase_free_item(next_items_it);
                data.items.erase(next_items_it);
            }
        }

        _insert_free_item(items_it);
    }
}

int used_alloc_ewram()
{
    return data.total_bytes_count - data.free_bytes_count;
}

int available_alloc_ewram()
{
    return data.free_bytes_count;
}

int used_items_ewram()
{
    return data.items.size();
}

int available_items_ewram()
{
    return data.items.available();
}

}
