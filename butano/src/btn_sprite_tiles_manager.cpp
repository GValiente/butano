/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_sprite_tiles_manager.h"

#include "btn_vector.h"
#include "btn_unordered_map.h"
#include "btn_config_sprite_tiles.h"
#include "../hw/include/btn_hw_sprite_tiles.h"
#include "../hw/include/btn_hw_sprite_tiles_constants.h"

#include "btn_sprite_tiles.cpp.h"
#include "btn_sprite_tiles_ptr.cpp.h"
#include "btn_sprite_tiles_item.cpp.h"

#if BTN_CFG_SPRITE_TILES_LOG_ENABLED
    #include "btn_log.h"
    #include "btn_tile.h"

    static_assert(BTN_CFG_LOG_ENABLED, "Log is not enabled");
#endif

namespace btn::sprite_tiles_manager
{

namespace
{
    static_assert(BTN_CFG_SPRITE_TILES_MAX_ITEMS > 0 &&
                  BTN_CFG_SPRITE_TILES_MAX_ITEMS <= hw::sprite_tiles::tiles_count());
    static_assert(power_of_two(BTN_CFG_SPRITE_TILES_MAX_ITEMS));


    constexpr const int max_items = BTN_CFG_SPRITE_TILES_MAX_ITEMS;
    constexpr const int max_list_items = max_items + 2;


    enum class status_type
    {
        FREE,
        USED,
        TO_REMOVE
    };


    class node_type
    {

    public:
        uint16_t prev_index = max_list_items;
        uint16_t next_index = max_list_items;
    };


    class item_type : public node_type
    {

    public:
        const tile* data = nullptr;
        unsigned usages = 0;
        unsigned start_tile: 12 = 0;
        unsigned tiles_count: 12 = 0;

    private:
        unsigned _status: 2 = unsigned(status_type::FREE);

    public:
        bool commit: 1 = false;

        [[nodiscard]] status_type status() const
        {
            return static_cast<status_type>(_status);
        }

        void set_status(status_type status)
        {
            _status = unsigned(status);
        }
    };


    class items_list
    {

    public:
        class iterator
        {

        public:
            friend class items_list;

            iterator() = default;

            [[nodiscard]] int id() const
            {
                return _index;
            }

            iterator& operator--()
            {
                const item_type& item = _list->_items[_index];
                _index = item.prev_index;
                return *this;
            }

            iterator& operator++()
            {
                const item_type& item = _list->_items[_index];
                _index = item.next_index;
                return *this;
            }

            [[nodiscard]] item_type& operator*()
            {
                return _list->_items[_index];
            }

            item_type* operator->()
            {
                return _list->_items + _index;
            }

            [[nodiscard]] friend bool operator==(const iterator& a, const iterator& b)
            {
                return a._index == b._index;
            }

            [[nodiscard]] friend bool operator!=(const iterator& a, const iterator& b)
            {
                return ! (a == b);
            }

        private:
            int _index;
            items_list* _list;

            iterator(int index, items_list& list) :
                _index(index),
                _list(&list)
            {
            }
        };

        void init()
        {
            _free_indices.resize(max_items);

            for(int index = 0; index < max_items; ++index)
            {
                _free_indices[index] = int16_t(max_items - index);
            }

            _items[0].next_index = max_list_items - 1;
            _items[max_list_items - 1].prev_index = 0;
        }

        [[nodiscard]] int size() const
        {
            return _free_indices.available();
        }

        [[nodiscard]] int available() const
        {
            return _free_indices.size();
        }

        [[nodiscard]] bool full() const
        {
            return _free_indices.empty();
        }

        [[nodiscard]] item_type& item(int index)
        {
            return _items[index];
        }

        [[nodiscard]] int index(const item_type& item) const
        {
            return &item - _items;
        }

        [[nodiscard]] iterator it(int index)
        {
            return iterator(index, *this);
        }

        [[nodiscard]] iterator begin()
        {
            return iterator(_items[0].next_index, *this);
        }

        [[nodiscard]] iterator end()
        {
            return iterator(max_list_items - 1, *this);
        }

        void push_front(const item_type& value)
        {
            insert(_items[0].next_index, value);
        }

        iterator insert(int index, const item_type& value)
        {
            int free_index = _free_indices.back();
            _free_indices.pop_back();
            _items[free_index] = value;
            _insert_node(index, free_index);
            return iterator(free_index, *this);
        }

        iterator erase(int index)
        {
            int next_index = _items[index].next_index;
            _free_indices.push_back(index);
            _remove_node(index);
            return iterator(next_index, *this);
        }

    private:
        item_type _items[max_list_items];
        vector<int16_t, max_items> _free_indices;

        void _insert_node(int position_index, int new_index)
        {
            node_type& position_node = _items[position_index];
            node_type& new_node = _items[new_index];
            int prev_index = position_node.prev_index;
            node_type& prev_node = _items[prev_index];
            prev_node.next_index = new_index;
            new_node.prev_index = prev_index;
            new_node.next_index = position_index;
            position_node.prev_index = new_index;
        }

        void _remove_node(int position_index)
        {
            node_type& position_node = _items[position_index];
            int prev_index = position_node.prev_index;
            node_type& prev_node = _items[prev_index];
            int next_index = position_node.next_index;
            node_type& next_node = _items[next_index];
            prev_node.next_index = next_index;
            next_node.prev_index = prev_index;
        }
    };


    class static_data
    {

    public:
        items_list items;
        unordered_map<const tile*, int, max_items * 2> items_map;
        vector<uint16_t, max_items> free_items;
        vector<uint16_t, max_items> to_remove_items;
        int free_tiles_count = 0;
        int to_remove_tiles_count = 0;
        bool check_commit = false;
        bool delay_commit = false;
    };

    BTN_DATA_EWRAM static_data data;


    #if BTN_CFG_SPRITE_TILES_LOG_ENABLED
        void _log_status()
        {
            BTN_LOG("items: ", data.items.size());
            BTN_LOG('[');

            for(const item_type& item : data.items)
            {
                BTN_LOG("    ",
                        (item.status() == status_type::FREE ? "free" :
                                item.status() == status_type::USED ? "used" : "to_remove"),
                        " - data: ", item.data,
                        " - start_tile: ", item.start_tile,
                        " - tiles_count: ", item.tiles_count,
                        " - usages: ", item.usages,
                        (item.commit ? " - commit" : " - no_commit"));
            }

            BTN_LOG(']');

            BTN_LOG("items_map: ", data.items_map.size());
            BTN_LOG('[');

            for(const auto& items_map_pair : data.items_map)
            {
                BTN_LOG("    ",
                        "data: ", items_map_pair.first,
                        " - id: ", items_map_pair.second);
            }

            BTN_LOG(']');

            BTN_LOG("free_items: ", data.free_items.size());
            BTN_LOG('[');

            for(int item_index : data.free_items)
            {
                const item_type& item = data.items.item(item_index);
                BTN_LOG("    ",
                        "index: ", item_index,
                        " - data: ", item.data,
                        " - start_tile: ", item.start_tile,
                        " - tiles_count: ", item.tiles_count);
            }

            BTN_LOG(']');

            BTN_LOG("to_remove_items: ", data.to_remove_items.size());
            BTN_LOG('[');

            for(int item_index : data.to_remove_items)
            {
                const item_type& item = data.items.item(item_index);
                BTN_LOG("    ",
                        "index: ", item_index,
                        " - data: ", item.data,
                        " - start_tile: ", item.start_tile,
                        " - tiles_count: ", item.tiles_count);
            }

            BTN_LOG(']');

            BTN_LOG("free_tiles_count: ", data.free_tiles_count);
            BTN_LOG("to_remove_tiles_count: ", data.to_remove_tiles_count);
            BTN_LOG("check_commit: ", (data.check_commit ? "true" : "false"));
            BTN_LOG("delay_commit: ", (data.delay_commit ? "true" : "false"));
        }

        #define BTN_SPRITE_TILES_LOG BTN_LOG

        #define BTN_SPRITE_TILES_LOG_STATUS \
            _log_status
    #else
        #define BTN_SPRITE_TILES_LOG(...) \
            do \
            { \
            } while(false)

        #define BTN_SPRITE_TILES_LOG_STATUS(...) \
            do \
            { \
            } while(false)
    #endif


    constexpr const auto lower_bound_comparator = [](int item_index, int tiles_count)
    {
        return data.items.item(item_index).tiles_count < tiles_count;
    };

    constexpr const auto upper_bound_comparator = [](int tiles_count, int item_index)
    {
        return tiles_count < data.items.item(item_index).tiles_count;
    };

    void _insert_free_item(int id, ivector<uint16_t>::iterator free_items_last)
    {
        const item_type& item = data.items.item(id);
        auto free_items_it = upper_bound(data.free_items.begin(), free_items_last, item.tiles_count,
                                         upper_bound_comparator);
        data.free_items.insert(free_items_it, id);
    }

    void _insert_free_item(int id)
    {
        _insert_free_item(id, data.free_items.end());
    }

    void _erase_free_item(int id)
    {
        const item_type& item = data.items.item(id);
        auto free_items_end = data.free_items.end();
        auto free_items_it = lower_bound(data.free_items.begin(), free_items_end, item.tiles_count,
                                         lower_bound_comparator);
        BTN_ASSERT(free_items_it != free_items_end, "Free item not found: ", id);

        while(*free_items_it != id)
        {
            ++free_items_it;
        }

        data.free_items.erase(free_items_it);
    }

    void _insert_to_remove_item(int id)
    {
        const item_type& item = data.items.item(id);
        auto to_remove_items_it = upper_bound(data.to_remove_items.begin(), data.to_remove_items.end(),
                                              item.tiles_count, upper_bound_comparator);
        data.to_remove_items.insert(to_remove_items_it, id);
    }

    void _erase_to_remove_item(int id)
    {
        const item_type& item = data.items.item(id);
        auto to_remove_items_end = data.to_remove_items.end();
        auto to_remove_items_it = lower_bound(data.to_remove_items.begin(), to_remove_items_end, item.tiles_count,
                                              lower_bound_comparator);
        BTN_ASSERT(to_remove_items_it != to_remove_items_end, "To remove item not found: ", id);

        while(*to_remove_items_it != id)
        {
            ++to_remove_items_it;
        }

        data.to_remove_items.erase(to_remove_items_it);
    }

    [[maybe_unused]] bool _valid_tiles_count(int tiles_count)
    {
        return tiles_count == 1 || tiles_count == 2 || tiles_count == 4 || tiles_count == 8 || tiles_count == 16 ||
                tiles_count == 32 || tiles_count == 64 || tiles_count == 128;
    }

    [[nodiscard]] int _find_impl(const tile* tiles_data, [[maybe_unused]] int tiles_count)
    {
        BTN_ASSERT(tiles_data, "Tiles ref is null");

        auto items_map_iterator = data.items_map.find(tiles_data);

        if(items_map_iterator != data.items_map.end())
        {
            int id = items_map_iterator->second;
            item_type& item = data.items.item(id);

            BTN_ASSERT(tiles_data == item.data, "Tiles data does not match item tiles data: ",
                       tiles_data, " - ", item.data);
            BTN_ASSERT(tiles_count == item.tiles_count, "Tiles count does not match item tiles count: ",
                       tiles_count, " - ", item.tiles_count);

            switch(item.status())
            {

            case status_type::FREE:
                BTN_ERROR("Invalid item state");
                break;

            case status_type::USED:
                ++item.usages;
                break;

            case status_type::TO_REMOVE:
                item.usages = 1;
                item.set_status(status_type::USED);
                _erase_to_remove_item(id);
                data.to_remove_tiles_count -= item.tiles_count;
                break;

            default:
                BTN_ERROR("Invalid item status: ", int(item.status()));
                break;
            }

            BTN_SPRITE_TILES_LOG("FOUND. start_tile: ", data.items.item(id).start_tile);
            BTN_SPRITE_TILES_LOG_STATUS();

            return id;
        }

        BTN_SPRITE_TILES_LOG("NOT FOUND");

        return -1;
    }

    void _commit_item(const tile* tiles_data, bool delay_commit, item_type& item)
    {
        item.data = tiles_data;

        if(delay_commit)
        {
            item.commit = true;
            data.check_commit = true;
        }
        else
        {
            hw::sprite_tiles::commit(tiles_data, item.start_tile, item.tiles_count);
        }
    }

    [[nodiscard]] optional<int> _create_item(int id, const tile* tiles_data, int tiles_count, bool delay_commit)
    {
        item_type& item = data.items.item(id);
        int new_item_tiles_count = item.tiles_count - tiles_count;

        switch(item.status())
        {

        case status_type::FREE:
            data.free_tiles_count -= tiles_count;
            break;

        case status_type::USED:
            BTN_ERROR("Invalid item state");
            break;

        case status_type::TO_REMOVE:
            data.items_map.erase(item.data);
            data.to_remove_tiles_count -= tiles_count;
            break;

        default:
            BTN_ERROR("Invalid item status: ", int(item.status()));
            break;
        }

        item.data = tiles_data;
        item.tiles_count = uint16_t(tiles_count);
        item.usages = 1;
        item.commit = false;
        item.set_status(status_type::USED);

        if(tiles_data)
        {
            _commit_item(tiles_data, delay_commit, item);
        }

        optional<int> new_free_item_id;

        if(new_item_tiles_count)
        {
            BTN_ASSERT(! data.items.full(), "No more items allowed");

            item_type new_item;
            new_item.start_tile = item.start_tile + item.tiles_count;
            new_item.tiles_count = uint16_t(new_item_tiles_count);

            auto new_item_iterator = data.items.insert(item.next_index, new_item);
            new_free_item_id = new_item_iterator.id();
        }

        return new_free_item_id;
    }

    [[nodiscard]] int _create_impl(const tile* tiles_data, int tiles_count)
    {
        bool check_to_remove_tiles = tiles_count <= data.to_remove_tiles_count;

        if(check_to_remove_tiles)
        {
            auto to_remove_items_end = data.to_remove_items.end();
            auto to_remove_items_it = lower_bound(data.to_remove_items.begin(), to_remove_items_end, tiles_count,
                                                  lower_bound_comparator);

            if(to_remove_items_it != to_remove_items_end)
            {
                int id = *to_remove_items_it;
                const item_type& item = data.items.item(id);

                if(item.tiles_count == tiles_count)
                {
                    data.to_remove_items.erase(to_remove_items_it);

                    if(optional<int> new_free_item_id = _create_item(id, tiles_data, tiles_count, true))
                    {
                        _insert_free_item(*new_free_item_id);
                    }

                    return id;
                }
            }
        }

        if(tiles_count <= data.free_tiles_count)
        {
            auto free_items_end = data.free_items.end();
            auto free_items_it = lower_bound(data.free_items.begin(), free_items_end, tiles_count,
                                             lower_bound_comparator);

            if(free_items_it != free_items_end)
            {
                int id = *free_items_it;

                if(optional<int> new_free_item_id = _create_item(id, tiles_data, tiles_count, data.delay_commit))
                {
                    _insert_free_item(*new_free_item_id, free_items_it);
                    ++free_items_it;
                }

                data.free_items.erase(free_items_it);
                return id;
            }
        }

        if(check_to_remove_tiles && ! data.delay_commit)
        {
            update();
            data.delay_commit = true;
            return _create_impl(tiles_data, tiles_count);
        }

        return -1;
    }

    [[nodiscard]] int _allocate_impl(int tiles_count)
    {
        if(data.delay_commit)
        {
            return -1;
        }

        if(tiles_count <= data.free_tiles_count)
        {
            auto free_items_end = data.free_items.end();
            auto free_items_it = lower_bound(data.free_items.begin(), free_items_end, tiles_count,
                                             lower_bound_comparator);

            if(free_items_it != free_items_end)
            {
                int id = *free_items_it;

                if(optional<int> new_free_item_id = _create_item(id, nullptr, tiles_count, false))
                {
                    _insert_free_item(*new_free_item_id, free_items_it);
                    ++free_items_it;
                }

                data.free_items.erase(free_items_it);
                return id;
            }
        }

        return -1;
    }
}

void init()
{
    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - INIT");

    item_type new_item;
    new_item.tiles_count = hw::sprite_tiles::tiles_count();
    data.items.init();
    data.items.push_front(new_item);
    data.free_items.push_back(data.items.begin().id());
    data.free_tiles_count = new_item.tiles_count;

    BTN_SPRITE_TILES_LOG_STATUS();
}

int used_tiles_count()
{
    return hw::sprite_tiles::tiles_count() - data.free_tiles_count;
}

int available_tiles_count()
{
    return data.free_tiles_count;
}

int used_items_count()
{
    return data.items.size();
}

int available_items_count()
{
    return data.items.available();
}

int find(const span<const tile>& tiles_ref)
{
    const tile* tiles_data = tiles_ref.data();
    int tiles_count = tiles_ref.size();

    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - FIND: ", tiles_data, " - ", tiles_count);

    return _find_impl(tiles_data, tiles_count);
}

int create(const span<const tile>& tiles_ref)
{
    const tile* tiles_data = tiles_ref.data();
    int tiles_count = tiles_ref.size();

    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - CREATE: ", tiles_data, " - ", tiles_count);

    int result = _find_impl(tiles_data, tiles_count);

    if(result != -1)
    {
        return result;
    }

    BTN_ASSERT(_valid_tiles_count(tiles_count), "Invalid tiles ref count: ", tiles_count);

    result = _create_impl(tiles_data, tiles_count);

    if(result != -1)
    {
        data.items_map.insert(tiles_data, result);

        BTN_SPRITE_TILES_LOG("CREATED. start_tile: ", data.items.item(result).start_tile);
        BTN_SPRITE_TILES_LOG_STATUS();
    }
    else
    {
        BTN_SPRITE_TILES_LOG("NOT CREATED");
    }

    return result;
}

int create_new(const span<const tile>& tiles_ref)
{
    const tile* tiles_data = tiles_ref.data();
    int tiles_count = tiles_ref.size();

    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - CREATE NEW: ", tiles_data, " - ", tiles_count);

    BTN_ASSERT(_valid_tiles_count(tiles_count), "Invalid tiles ref count: ", tiles_count);
    BTN_ASSERT(data.items_map.find(tiles_data) == data.items_map.end(),
               "Multiple copies of the same tiles data not supported");

    int result = _create_impl(tiles_data, tiles_count);

    if(result != -1)
    {
        data.items_map.insert(tiles_data, result);

        BTN_SPRITE_TILES_LOG("CREATED. start_tile: ", data.items.item(result).start_tile);
        BTN_SPRITE_TILES_LOG_STATUS();
    }
    else
    {
        BTN_SPRITE_TILES_LOG("NOT CREATED");
    }

    return result;
}

int allocate(int tiles_count)
{
    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - ALLOCATE: ", tiles_count);

    BTN_ASSERT(_valid_tiles_count(tiles_count), "Invalid tiles count: ", tiles_count);

    int result = _allocate_impl(tiles_count);

    if(result >= 0)
    {
        BTN_SPRITE_TILES_LOG("ALLOCATED. start_tile: ", data.items.item(result).start_tile);
        BTN_SPRITE_TILES_LOG_STATUS();
    }
    else
    {
        BTN_SPRITE_TILES_LOG("NOT ALLOCATED");
    }

    return result;
}

void increase_usages(int id)
{
    item_type& item = data.items.item(id);

    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - INCREASE_USAGES: ", item.start_tile);

    ++item.usages;

    BTN_SPRITE_TILES_LOG_STATUS();
}

void decrease_usages(int id)
{
    item_type& item = data.items.item(id);

    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - DECREASE_USAGES: ", item.start_tile);

    --item.usages;

    if(! item.usages)
    {
        item.set_status(status_type::TO_REMOVE);
        _insert_to_remove_item(id);
        data.to_remove_tiles_count += item.tiles_count;
    }

    BTN_SPRITE_TILES_LOG_STATUS();
}

int start_tile(int id)
{
    return data.items.item(id).start_tile;
}

int tiles_count(int id)
{
    return data.items.item(id).tiles_count;
}

optional<span<const tile>> tiles_ref(int id)
{
    const item_type& item = data.items.item(id);
    optional<span<const tile>> result;

    if(item.data)
    {
        result.emplace(item.data, item.tiles_count);
    }

    return result;
}

void set_tiles_ref(int id, const span<const tile>& tiles_ref)
{
    item_type& item = data.items.item(id);
    const tile* old_tiles_data = item.data;
    [[maybe_unused]] int old_tiles_count = item.tiles_count;
    const tile* new_tiles_data = tiles_ref.data();
    [[maybe_unused]] int new_tiles_count = tiles_ref.size();

    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - SET_TILES_REF: ", item.start_tile, " - ", new_tiles_data,
                         " - ", new_tiles_count);

    BTN_ASSERT(old_tiles_data, "Item has no data");

    if(old_tiles_data != new_tiles_data)
    {
        BTN_ASSERT(data.items_map.find(new_tiles_data) == data.items_map.end(),
                   "Multiple copies of the same tiles data not supported");
        BTN_ASSERT(old_tiles_count == new_tiles_count, "Tiles count does not match item tiles count: ",
                   old_tiles_count, " - ", new_tiles_count);

        data.items_map.erase(old_tiles_data);
        _commit_item(new_tiles_data, true, item);
        data.items_map.insert(new_tiles_data, id);

        BTN_SPRITE_TILES_LOG_STATUS();
    }
}

void reload_tiles_ref(int id)
{
    item_type& item = data.items.item(id);

    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - RELOAD_TILES_REF: ", item.start_tile);

    BTN_ASSERT(item.data, "Item has no data");

    item.commit = true;
    data.check_commit = true;

    BTN_SPRITE_TILES_LOG_STATUS();
}

optional<span<tile>> vram(int id)
{
    const item_type& item = data.items.item(id);
    optional<span<tile>> result;

    if(! item.data)
    {
        result.emplace(hw::sprite_tiles::vram(item.start_tile), item.tiles_count);
    }

    return result;
}

void update()
{
    if(data.to_remove_tiles_count)
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - UPDATE");

        auto begin = data.items.begin();
        auto end = data.items.end();

        for(int to_remove_item_index : data.to_remove_items)
        {
            auto iterator = data.items.it(to_remove_item_index);
            item_type& item = *iterator;

            if(item.data)
            {
                data.items_map.erase(item.data);
            }

            item.data = nullptr;
            item.set_status(status_type::FREE);
            item.commit = false;
            data.free_tiles_count += item.tiles_count;

            auto next_iterator = iterator;
            ++next_iterator;

            if(next_iterator != end)
            {
                item_type& next_item = *next_iterator;

                if(next_item.status() == status_type::FREE)
                {
                    int next_id = next_iterator.id();
                    item.tiles_count += next_item.tiles_count;
                    _erase_free_item(next_id);
                    data.items.erase(next_id);
                }
            }

            if(iterator != begin)
            {
                auto previous_iterator = iterator;
                --previous_iterator;

                item_type& previous_item = *previous_iterator;

                if(previous_item.status() == status_type::FREE)
                {
                    int previous_id = previous_iterator.id();
                    item.start_tile = previous_item.start_tile;
                    item.tiles_count += previous_item.tiles_count;
                    _erase_free_item(previous_id);
                    data.items.erase(previous_id);
                }
            }

            _insert_free_item(to_remove_item_index);
        }

        data.to_remove_items.clear();
        data.to_remove_tiles_count = 0;

        BTN_SPRITE_TILES_LOG_STATUS();
    }
}

void commit()
{
    if(data.check_commit)
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - COMMIT");

        data.check_commit = false;

        for(item_type& item : data.items)
        {
            if(item.commit)
            {
                item.commit = false;

                if(item.status() == status_type::USED)
                {
                    hw::sprite_tiles::commit(item.data, item.start_tile, item.tiles_count);
                }
            }
        }

        BTN_SPRITE_TILES_LOG_STATUS();
    }

    data.delay_commit = false;
}

}
