#include "btn_memory_manager.h"

#include "btn_list.h"
#include "btn_vector.h"
#include "btn_hash_map.h"
#include "btn_config_memory.h"
#include "../hw/include/btn_hw_memory.h"

namespace btn::memory_manager
{

namespace
{
    static_assert(power_of_two(BTN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS));


    constexpr const int max_items = BTN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS;


    class item_type
    {

    public:
        char* data = nullptr;
        int size: 24 = 0;
        bool used = false;
    };


    using items_list = list<item_type, max_items>;
    using items_iterator = items_list::iterator;


    class static_data
    {

    public:
        items_list items;
        hash_map<void*, items_iterator, max_items * 2> used_items;
        vector<items_iterator, max_items> free_items;
        int total_bytes_count = 0;
        int free_bytes_count = 0;
    };

    BTN_DATA_EWRAM static_data data;


    void _create_item(items_iterator items_it, int bytes)
    {
        item_type& item = *items_it;
        BTN_ASSERT(! item.used, "Item is not free: ", item.size);

        item.used = true;

        if(int new_item_size = item.size - bytes)
        {
            BTN_ASSERT(! data.items.full(), "No more items allowed");

            item_type new_item;
            new_item.data = item.data;
            new_item.size = new_item_size;

            items_iterator new_items_it = data.items.insert(items_it, new_item);
            data.free_items.push_back(new_items_it);
            item.data += new_item_size;
            item.size = bytes;
        }

        data.used_items.insert(item.data, items_it);
        data.free_bytes_count -= bytes;
    }

    void _erase_free_items_it(items_iterator items_it)
    {
        for(auto free_items_it = data.free_items.begin(), free_items_end = data.free_items.end();
            free_items_it != free_items_end; ++free_items_it)
        {
            if(*free_items_it == items_it)
            {
                data.free_items.erase(free_items_it);
                return;
            }
        }
    }
}

void init()
{
    char* start = hw::memory::ewram_heap_start();
    char* end = hw::memory::ewram_heap_end();
    data.total_bytes_count = end - start;
    BTN_ASSERT(data.total_bytes_count >= 0, "Invalid heap size: ",
               static_cast<void*>(start), " - ", static_cast<void*>(end));

    item_type new_item;
    new_item.data = start;
    new_item.size = data.total_bytes_count;
    data.items.push_front(new_item);
    data.free_items.push_back(data.items.begin());
    data.free_bytes_count = data.total_bytes_count;
}

void* ewram_alloc(int bytes)
{
    BTN_ASSERT(bytes >= 0, "Invalid bytes: ", bytes);

    bytes = max(bytes, int(sizeof(int)));

    if(bytes <= data.free_bytes_count)
    {
        auto free_items_end = data.free_items.end();
        auto smallest_free_items_it = free_items_end;
        int smallest_size = numeric_limits<int>::max();

        for(auto free_items_it = data.free_items.begin(); free_items_it != free_items_end; ++free_items_it)
        {
            items_iterator items_it = *free_items_it;
            item_type& item = *items_it;

            if(item.size > bytes)
            {
                if(item.size < smallest_size)
                {
                    smallest_free_items_it = free_items_it;
                    smallest_size = item.size;
                }
            }
            else if(item.size == bytes)
            {
                _create_item(items_it, bytes);
                data.free_items.erase(free_items_it);
                return items_it->data;
            }
        }

        if(smallest_free_items_it != free_items_end)
        {
            items_iterator items_it = *smallest_free_items_it;
            _create_item(items_it, bytes);
            data.free_items.erase(smallest_free_items_it);
            return items_it->data;
        }
    }

    return nullptr;
}

void ewram_free(void* ptr)
{
    if(ptr)
    {
        auto used_items_it = data.used_items.find(ptr);
        BTN_ASSERT(used_items_it != data.used_items.end(), "Ptr not found: ", ptr);

        items_iterator items_it = used_items_it->second;
        item_type& item = *items_it;
        BTN_ASSERT(item.used, "Item is not used: ", item.size);

        item.used = false;
        data.used_items.erase(used_items_it);
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
                _erase_free_items_it(previous_items_it);
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
                _erase_free_items_it(next_items_it);
                data.items.erase(next_items_it);
            }
        }

        data.free_items.push_back(items_it);
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

}
