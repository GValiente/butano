/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_best_fit_allocator.h"

#include "bn_memory.h"
#include "bn_alignment.h"

namespace bn
{

namespace
{
    [[nodiscard]] ibest_fit_allocator::size_type _aligned_bytes(ibest_fit_allocator::size_type bytes)
    {
        ibest_fit_allocator::size_type alignment_bytes = sizeof(int);

        if(ibest_fit_allocator::size_type extra_bytes = bytes % alignment_bytes)
        {
            bytes += alignment_bytes - extra_bytes;
        }

        return bytes;
    }
}

ibest_fit_allocator::~ibest_fit_allocator() noexcept
{
    BN_ASSERT(empty(), "Pool is not empty");
}

void* ibest_fit_allocator::alloc(size_type bytes)
{
    BN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);

    bytes = _aligned_bytes(bytes) + size_type(sizeof(items_iterator));

    if(bytes > _free_bytes_count)
    {
        return nullptr;
    }

    auto free_items_end = _free_items.end();
    auto free_items_it = lower_bound(_free_items.begin(), free_items_end, bytes, _lower_bound_comparator);

    if(free_items_it == free_items_end)
    {
        return nullptr;
    }

    items_iterator items_it = *free_items_it;
    item_type& item = *items_it;

    if(size_type new_item_size = item.size - bytes)
    {
        if(_items.full())
        {
            return nullptr;
        }

        item_type new_item;
        new_item.data = item.data;
        new_item.size = new_item_size;

        items_iterator new_items_it = _items.insert(items_it, new_item);
        _insert_free_item(new_items_it, free_items_it);
        ++free_items_it;
        item.data += new_item_size;
        item.size = bytes;
    }

    item.used = true;
    _free_items.erase(free_items_it);

    items_iterator* items_it_ptr = reinterpret_cast<items_iterator*>(item.data);
    *items_it_ptr = items_it;
    _free_bytes_count -= bytes;
    return items_it_ptr + 1;
}

void* ibest_fit_allocator::calloc(size_type num, size_type bytes)
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

void* ibest_fit_allocator::realloc(void* ptr, size_type new_bytes)
{
    if(! ptr)
    {
        return alloc(new_bytes);
    }

    BN_ASSERT(new_bytes >= 0, "Invalid new bytes: ", new_bytes);

    items_iterator* items_it_ptr = reinterpret_cast<items_iterator*>(ptr) - 1;
    items_iterator items_it = *items_it_ptr;
    item_type& item = *items_it;
    size_type old_bytes = item.size - size_type(sizeof(items_iterator));

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

void ibest_fit_allocator::free(void* ptr)
{
    if(! ptr)
    {
        return;
    }

    items_iterator* items_it_ptr = reinterpret_cast<items_iterator*>(ptr) - 1;
    items_iterator items_it = *items_it_ptr;
    item_type& item = *items_it;
    item.used = false;
    _free_bytes_count += item.size;

    if(items_it != _items.begin())
    {
        items_iterator previous_items_it = items_it;
        --previous_items_it;

        item_type& previous_item = *previous_items_it;

        if(! previous_item.used && previous_item.data + previous_item.size == item.data)
        {
            item.data = previous_item.data;
            item.size += previous_item.size;
            _erase_free_item(previous_items_it);
            _items.erase(previous_items_it);
        }
    }

    items_iterator next_items_it = items_it;
    ++next_items_it;

    if(next_items_it != _items.end())
    {
        item_type& next_item = *next_items_it;

        if(! next_item.used && item.data + item.size == next_item.data)
        {
            item.size += next_item.size;
            _erase_free_item(next_items_it);
            _items.erase(next_items_it);
        }
    }

    _insert_free_item(items_it, _free_items.end());
}

ibest_fit_allocator::ibest_fit_allocator(ilist<item_type>& items, ivector<items_iterator>& free_items) :
    _items(items),
    _free_items(free_items)
{
}

void ibest_fit_allocator::_init(void* start, size_type bytes)
{
    BN_ASSERT(start, "Start is null");
    BN_ASSERT(aligned<sizeof(int)>(start), "Start is not aligned");
    BN_ASSERT(bytes > 0 && bytes % size_type(sizeof(int)) == 0, "Invalid bytes: ", bytes);

    _total_bytes_count = bytes;
    _free_bytes_count = bytes;

    item_type new_item;
    new_item.data = static_cast<char*>(start);
    new_item.size = bytes;
    _items.push_front(new_item);
    _free_items.push_back(_items.begin());
}

void ibest_fit_allocator::_insert_free_item(
        items_iterator items_it, ivector<items_iterator>::iterator free_items_last)
{
    BN_ASSERT(! _free_items.full(), "Free items is full");

    auto free_items_it = upper_bound(_free_items.begin(), free_items_last, items_it->size,
                                     _upper_bound_comparator);
    _free_items.insert(free_items_it, items_it);
}

void ibest_fit_allocator::_erase_free_item(items_iterator items_it)
{
    auto free_items_end = _free_items.end();
    auto free_items_it = lower_bound(_free_items.begin(), free_items_end, items_it->size,
                                     _lower_bound_comparator);

    while(*free_items_it != items_it)
    {
        ++free_items_it;
    }

    _free_items.erase(free_items_it);
}

}
