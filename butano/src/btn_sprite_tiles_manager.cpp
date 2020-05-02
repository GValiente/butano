#include "btn_sprite_tiles_manager.h"

#include "btn_span.h"
#include "btn_tile.h"
#include "btn_vector.h"
#include "btn_optional.h"
#include "btn_hash_map.h"
#include "btn_sprite_tiles.h"
#include "../hw/include/btn_hw_sprite_tiles.h"

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
                  BTN_CFG_SPRITE_TILES_MAX_ITEMS <= sprite_tiles::tiles_count());
    static_assert(power_of_two(BTN_CFG_SPRITE_TILES_MAX_ITEMS));


    static constexpr const int max_items = BTN_CFG_SPRITE_TILES_MAX_ITEMS;
    static constexpr const int max_list_items = max_items + 1;


    class item_type
    {

    public:
        enum class status_type
        {
            FREE,
            USED,
            TO_REMOVE
        };

        const tile* data = nullptr;
        unsigned usages = 0;
        uint16_t start_tile = 0;
        uint16_t tiles_count = 0;
        uint16_t next_index = max_list_items;
        bool commit = false;

    private:
        uint8_t _status = uint8_t(status_type::FREE);

    public:
        [[nodiscard]] status_type status() const
        {
            return static_cast<status_type>(_status);
        }

        void set_status(status_type status)
        {
            _status = uint8_t(status);
        }
    };


    class items_list
    {

    public:
        enum class status_type
        {
            FREE,
            USED,
            TO_REMOVE
        };

        class iterator
        {

        public:
            friend class items_list;

            iterator() = default;

            [[nodiscard]] int id() const
            {
                return _index;
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
                _free_indices[index] = int16_t(index + 1);
            }
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

        [[nodiscard]] iterator before_begin()
        {
            return iterator(0, *this);
        }

        [[nodiscard]] iterator begin()
        {
            return iterator(_items[0].next_index, *this);
        }

        [[nodiscard]] iterator end()
        {
            return iterator(max_list_items, *this);
        }

        void push_front(const item_type& value)
        {
            insert_after(0, value);
        }

        iterator insert_after(int index, const item_type& value)
        {
            int free_index = _free_indices.back();
            _free_indices.pop_back();
            _items[free_index] = value;
            _insert_node_after(index, free_index);
            return iterator(free_index, *this);
        }

        iterator erase_after(int index)
        {
            _remove_node_after(index);
            return iterator(_items[index].next_index, *this);
        }

    private:
        item_type _items[max_list_items];
        vector<int16_t, max_items> _free_indices;

        void _join(int index, int new_index)
        {
            _items[index].next_index = uint16_t(new_index);
        }

        void _insert_node_after(int index, int new_index)
        {
            auto next_index = int(_items[index].next_index);
            _join(new_index, next_index);
            _join(index, new_index);
        }

        void _remove_node_after(int index)
        {
            auto next_index = int(_items[index].next_index);
            _free_indices.push_back(int16_t(next_index));

            auto next_next_index = int(_items[next_index].next_index);
            _join(index, next_next_index);
        }
    };


    class static_data
    {

    public:
        items_list items;
        hash_map<const tile*, uint16_t, max_items * 2> items_map;
        items_list::iterator biggest_free_iterator;
        int free_tiles_count = 0;
        int to_remove_tiles_count = 0;
        bool check_commit = false;
        bool delay_commit = false;
    };

    BTN_DATA_EWRAM static_data data;


    [[maybe_unused]] bool _valid_tiles_count(int tiles_count)
    {
        return tiles_count == 1 || tiles_count == 2 || tiles_count == 4 || tiles_count == 8 || tiles_count == 16 ||
                tiles_count == 32 || tiles_count == 64 || tiles_count == 128;
    }

    void _commit_item(int id, const tile* tiles_data, bool delay_commit)
    {
        item_type& item = data.items.item(id);
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

    void _create_item(int id, const tile* tiles_data, int tiles_count)
    {
        item_type& item = data.items.item(id);
        int new_item_tiles_count = item.tiles_count - tiles_count;
        item.data = tiles_data;
        item.tiles_count = uint16_t(tiles_count);
        item.usages = 1;
        item.set_status(item_type::status_type::USED);
        item.commit = false;
        data.free_tiles_count -= tiles_count;

        if(data.biggest_free_iterator.id() == id)
        {
            data.biggest_free_iterator = data.items.end();
        }

        if(tiles_data)
        {
            _commit_item(id, tiles_data, data.delay_commit);
        }

        if(new_item_tiles_count)
        {
            BTN_ASSERT(! data.items.full(), "No more items allowed");

            item_type new_item;
            new_item.start_tile = item.start_tile + item.tiles_count;
            new_item.tiles_count = uint16_t(new_item_tiles_count);

            auto new_item_iterator = data.items.insert_after(id, new_item);

            if(data.biggest_free_iterator != data.items.end())
            {
                const item_type& biggest_free_item = *data.biggest_free_iterator;

                if(biggest_free_item.tiles_count < new_item_tiles_count)
                {
                    data.biggest_free_iterator = new_item_iterator;
                }
            }
            else
            {
                data.biggest_free_iterator = new_item_iterator;
            }
        }
    }

    int _create_impl(const tile* tiles_data, int tiles_count)
    {
        if(! tiles_data && data.delay_commit)
        {
            return -1;
        }

        if(tiles_count <= data.free_tiles_count)
        {
            for(auto iterator = data.biggest_free_iterator, end = data.items.end(); iterator != end; ++iterator)
            {
                const item_type& item = *iterator;

                if(item.status() == item_type::status_type::FREE && item.tiles_count >= tiles_count)
                {
                    int id = iterator.id();
                    _create_item(id, tiles_data, tiles_count);
                    return id;
                }
            }

            for(auto iterator = data.items.begin(), end = data.biggest_free_iterator; iterator != end; ++iterator)
            {
                const item_type& item = *iterator;

                if(item.status() == item_type::status_type::FREE && item.tiles_count >= tiles_count)
                {
                    int id = iterator.id();
                    _create_item(id, tiles_data, tiles_count);
                    return id;
                }
            }
        }

        if(tiles_data && tiles_count <= data.to_remove_tiles_count)
        {
            update();
            data.delay_commit = true;
            return _create_impl(tiles_data, tiles_count);
        }

        return -1;
    }

    bool _remove_adjacent_item(int adjacent_id, item_type& current_item)
    {
        const item_type& adjacent_item = data.items.item(adjacent_id);
        item_type::status_type adjacent_item_status = adjacent_item.status();
        bool remove = adjacent_item_status != item_type::status_type::USED;

        if(remove)
        {
            current_item.tiles_count += adjacent_item.tiles_count;

            if(adjacent_item_status == item_type::status_type::TO_REMOVE)
            {
                if(adjacent_item.data)
                {
                    data.items_map.erase(adjacent_item.data);
                }

                data.free_tiles_count += adjacent_item.tiles_count;
            }
            else
            {
                if(data.biggest_free_iterator.id() == adjacent_id)
                {
                    data.biggest_free_iterator = data.items.end();
                }
            }
        }

        return remove;
    }


    #if BTN_CFG_SPRITE_TILES_LOG_ENABLED
        void _log_status()
        {
            BTN_LOG("items: ", data.items.size());
            BTN_LOG('[');

            for(const item_type& item : data.items)
            {
                BTN_LOG("    ",
                        (item.status() == item_type::status_type::FREE ? "free" :
                                item.status() == item_type::status_type::USED ? "used" : "to_remove"),
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
                        " - data: ", items_map_pair.first,
                        " - id: ", items_map_pair.second);
            }

            BTN_LOG(']');

            if(data.biggest_free_iterator == data.items.end())
            {
                BTN_LOG("biggest_free_iterator: invalid");
            }
            else
            {
                BTN_LOG("biggest_free_iterator start_tile: ", data.biggest_free_iterator->start_tile);
                BTN_LOG("biggest_free_iterator tiles_count: ", data.biggest_free_iterator->tiles_count);
            }

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
}

void init()
{
    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - INIT");

    item_type new_item;
    new_item.tiles_count = sprite_tiles::tiles_count();
    data.items.init();
    data.items.push_front(new_item);
    data.biggest_free_iterator = data.items.begin();
    data.free_tiles_count = new_item.tiles_count;

    BTN_SPRITE_TILES_LOG_STATUS();
}

int used_tiles_count()
{
    return sprite_tiles::tiles_count() - data.free_tiles_count;
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
    [[maybe_unused]] int tiles_size = tiles_ref.size();

    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - FIND: ", tiles_data, " - ", tiles_size);

    BTN_ASSERT(tiles_data, "Tiles ref is null");

    auto items_map_iterator = data.items_map.find(tiles_data);

    if(items_map_iterator != data.items_map.end())
    {
        int id = items_map_iterator->second;
        item_type& item = data.items.item(id);

        BTN_ASSERT(tiles_size == item.tiles_count, "Tiles count does not match item tiles count: ",
                   tiles_size, " - ", item.tiles_count);

        switch(item.status())
        {

        case item_type::status_type::FREE:
            BTN_ERROR("Invalid item state");
            break;

        case item_type::status_type::USED:
            ++item.usages;
            break;

        case item_type::status_type::TO_REMOVE:
            item.usages = 1;
            item.set_status(item_type::status_type::USED);
            data.to_remove_tiles_count -= item.tiles_count;
            break;
        }

        BTN_SPRITE_TILES_LOG("FOUND. start_tile: ", data.items.item(id).start_tile);
        BTN_SPRITE_TILES_LOG_STATUS();

        return id;
    }

    BTN_SPRITE_TILES_LOG("NOT FOUND");

    return -1;
}

int create(const span<const tile>& tiles_ref)
{
    const tile* tiles_data = tiles_ref.data();
    int tiles_size = tiles_ref.size();

    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - CREATE: ", tiles_data, " - ", tiles_size);

    BTN_ASSERT(_valid_tiles_count(tiles_size), "Invalid tiles ref size: ", tiles_size);
    BTN_ASSERT(data.items_map.find(tiles_data) == data.items_map.end(),
               "Multiple copies of the same tiles data not supported");

    int result = _create_impl(tiles_data, tiles_size);

    if(result >= 0)
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

    int result = _create_impl(nullptr, tiles_count);

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
        item.set_status(item_type::status_type::TO_REMOVE);
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
    const item_type& item = data.items.item(id);
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
        _commit_item(id, new_tiles_data, true);
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

        auto end = data.items.end();
        auto before_previous_iterator = end;
        auto previous_iterator = data.items.before_begin();
        auto iterator = previous_iterator;
        ++iterator;
        data.to_remove_tiles_count = 0;

        while(iterator != end)
        {
            item_type& item = *iterator;

            if(item.status() == item_type::status_type::TO_REMOVE)
            {
                if(item.data)
                {
                    data.items_map.erase(item.data);
                }

                item.data = nullptr;
                item.set_status(item_type::status_type::FREE);
                item.commit = false;
                data.free_tiles_count += item.tiles_count;

                auto next_iterator = iterator;
                ++next_iterator;

                while(next_iterator != end)
                {
                    if(_remove_adjacent_item(next_iterator.id(), item))
                    {
                        next_iterator = data.items.erase_after(iterator.id());
                    }
                    else
                    {
                        break;
                    }
                }

                if(before_previous_iterator != end)
                {
                    if(_remove_adjacent_item(previous_iterator.id(), item))
                    {
                        item.start_tile = previous_iterator->start_tile;
                        data.items.erase_after(before_previous_iterator.id());
                        previous_iterator = before_previous_iterator;
                    }
                }

                if(data.biggest_free_iterator != end)
                {
                    if(data.biggest_free_iterator->tiles_count < item.tiles_count)
                    {
                        data.biggest_free_iterator = iterator;
                    }
                }
                else
                {
                    data.biggest_free_iterator = iterator;
                }
            }

            before_previous_iterator = previous_iterator;
            previous_iterator = iterator;
            ++iterator;
        }

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

                if(item.status() == item_type::status_type::USED)
                {
                    hw::sprite_tiles::commit(item.data, item.start_tile, item.tiles_count);
                }
            }
        }

        BTN_SPRITE_TILES_LOG_STATUS();
    }
}

}
