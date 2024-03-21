/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_tiles_manager.h"

#include "bn_vector.h"
#include "bn_string_view.h"
#include "bn_unordered_map.h"
#include "bn_config_sprite_tiles.h"
#include "../hw/include/bn_hw_sprite_tiles.h"
#include "../hw/include/bn_hw_sprite_tiles_constants.h"

#include "bn_sprite_tiles.cpp.h"
#include "bn_sprite_tiles_ptr.cpp.h"
#include "bn_sprite_tiles_item.cpp.h"

#if BN_CFG_SPRITE_TILES_LOG_ENABLED
    #include "bn_log.h"
    #include "bn_tile.h"

    static_assert(BN_CFG_LOG_ENABLED, "Log is not enabled");
#elif BN_CFG_LOG_ENABLED
    #include "bn_log.h"
#endif

namespace bn::sprite_tiles_manager
{

namespace
{
    static_assert(BN_CFG_SPRITE_TILES_MAX_ITEMS > 0 &&
                  BN_CFG_SPRITE_TILES_MAX_ITEMS <= hw::sprite_tiles::tiles_count());
    static_assert(power_of_two(BN_CFG_SPRITE_TILES_MAX_ITEMS));


    #if BN_CFG_LOG_ENABLED
        constexpr string_view _status_log_message = "\nSprite VRAM status has been logged.";
    #else
        constexpr string_view _status_log_message = "";
    #endif


    constexpr int max_items = BN_CFG_SPRITE_TILES_MAX_ITEMS;
    constexpr int max_list_items = max_items + 2;


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
        uint8_t _status: 2 = uint8_t(status_type::FREE);
        uint8_t _compression: 2 = uint8_t(compression_type::NONE);

    public:
        bool commit: 1 = false;
        bool commit_if_recovered: 1 = false;

        [[nodiscard]] status_type status() const
        {
            return static_cast<status_type>(_status);
        }

        void set_status(status_type status)
        {
            _status = uint8_t(status);
        }

        [[nodiscard]] compression_type compression() const
        {
            return static_cast<compression_type>(_compression);
        }

        void set_compression(compression_type compression)
        {
            _compression = uint8_t(compression);
        }
    };


    class items_list
    {

    public:
        class iterator
        {

        public:
            friend class items_list;

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

        items_list()
        {
            for(int index = 0; index < max_items; ++index)
            {
                _free_indices_array[index] = int16_t(max_items - index);
            }

            _items[0].next_index = max_list_items - 1;
            _items[max_list_items - 1].prev_index = 0;
        }

        [[nodiscard]] int size() const
        {
            return _free_indices_size;
        }

        [[nodiscard]] int available() const
        {
            return _free_indices_size;
        }

        [[nodiscard]] bool full() const
        {
            return ! _free_indices_size;
        }

        [[nodiscard]] item_type& item(int index)
        {
            return _items[index];
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
            --_free_indices_size;

            int free_index = _free_indices_array[_free_indices_size];
            _items[free_index] = value;
            _insert_node(index, free_index);
            return iterator(free_index, *this);
        }

        iterator erase(int index)
        {
            int next_index = _items[index].next_index;
            _free_indices_array[_free_indices_size] = int16_t(index);
            ++_free_indices_size;
            _remove_node(index);
            return iterator(next_index, *this);
        }

    private:
        item_type _items[max_list_items];
        alignas(int) int16_t _free_indices_array[max_items];
        int _free_indices_size = max_items;

        void _insert_node(int position_index, int new_index)
        {
            node_type& position_node = _items[position_index];
            node_type& new_node = _items[new_index];
            int prev_index = position_node.prev_index;
            node_type& prev_node = _items[prev_index];
            prev_node.next_index = uint16_t(new_index);
            new_node.prev_index = uint16_t(prev_index);
            new_node.next_index = uint16_t(position_index);
            position_node.prev_index = uint16_t(new_index);
        }

        void _remove_node(int position_index)
        {
            const node_type& position_node = _items[position_index];
            int prev_index = position_node.prev_index;
            node_type& prev_node = _items[prev_index];
            int next_index = position_node.next_index;
            node_type& next_node = _items[next_index];
            prev_node.next_index = uint16_t(next_index);
            next_node.prev_index = uint16_t(prev_index);
        }
    };


    class static_data
    {

    public:
        items_list items;
        unordered_map<const tile*, int, max_items * 2> items_map;
        vector<uint16_t, max_items> free_items;
        vector<uint16_t, max_items> to_remove_items;
        vector<uint16_t, max_items> to_commit_uncompressed_items;
        vector<uint16_t, max_items> to_commit_compressed_items;
        uint16_t free_tiles_count = 0;
        uint16_t to_remove_tiles_count = 0;
        bool delay_commit = false;
    };

    BN_DATA_EWRAM_BSS static_data data;


    #if BN_CFG_SPRITE_TILES_LOG_ENABLED
        void _log_status()
        {
            BN_LOG("items: ", data.items.size());
            BN_LOG('[');

            for(const item_type& item : data.items)
            {
                BN_LOG("    ",
                        (item.status() == status_type::FREE ? "free" :
                                item.status() == status_type::USED ? "used" : "to_remove"),
                        " - data: ", item.data,
                        " - start_tile: ", item.start_tile,
                        " - tiles_count: ", item.tiles_count,
                        " - compression: ", int(item.compression()),
                        " - usages: ", item.usages,
                        (item.commit ? " - commit" : " - no_commit"));
            }

            BN_LOG(']');

            BN_LOG("items_map: ", data.items_map.size());
            BN_LOG('[');

            for(const auto& items_map_pair : data.items_map)
            {
                BN_LOG("    ",
                        "data: ", items_map_pair.first,
                        " - id: ", items_map_pair.second);
            }

            BN_LOG(']');

            BN_LOG("free_items: ", data.free_items.size());
            BN_LOG('[');

            for(int item_index : data.free_items)
            {
                const item_type& item = data.items.item(item_index);
                BN_LOG("    ",
                        "index: ", item_index,
                        " - data: ", item.data,
                        " - start_tile: ", item.start_tile,
                        " - tiles_count: ", item.tiles_count);
            }

            BN_LOG(']');

            BN_LOG("to_remove_items: ", data.to_remove_items.size());
            BN_LOG('[');

            for(int item_index : data.to_remove_items)
            {
                const item_type& item = data.items.item(item_index);
                BN_LOG("    ",
                        "index: ", item_index,
                        " - data: ", item.data,
                        " - start_tile: ", item.start_tile,
                        " - tiles_count: ", item.tiles_count);
            }

            BN_LOG(']');

            BN_LOG("free_tiles_count: ", data.free_tiles_count);
            BN_LOG("to_remove_tiles_count: ", data.to_remove_tiles_count);
            BN_LOG("delay_commit: ", (data.delay_commit ? "true" : "false"));
        }

        #define BN_SPRITE_TILES_LOG BN_LOG

        #define BN_SPRITE_TILES_LOG_STATUS \
            _log_status
    #else
        #define BN_SPRITE_TILES_LOG(...) \
            do \
            { \
            } while(false)

        #define BN_SPRITE_TILES_LOG_STATUS(...) \
            do \
            { \
            } while(false)
    #endif


    constexpr auto tiles_count_lower_bound_comparator = [](int item_index, unsigned tiles_count)
    {
        return data.items.item(item_index).tiles_count < tiles_count;
    };

    constexpr auto tiles_count_upper_bound_comparator = [](unsigned tiles_count, int item_index)
    {
        return tiles_count < data.items.item(item_index).tiles_count;
    };

    void _insert_free_item(int id, ivector<uint16_t>::iterator free_items_last)
    {
        const item_type& item = data.items.item(id);
        auto free_items_it = upper_bound(data.free_items.begin(), free_items_last, item.tiles_count,
                                         tiles_count_upper_bound_comparator);
        data.free_items.insert(free_items_it, uint16_t(id));
    }

    void _insert_free_item(int id)
    {
        _insert_free_item(id, data.free_items.end());
    }

    void _erase_free_item(int id)
    {
        const item_type& item = data.items.item(id);
        auto free_items_it = lower_bound(data.free_items.begin(), data.free_items.end(), item.tiles_count,
                                         tiles_count_lower_bound_comparator);

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
                                              item.tiles_count, tiles_count_upper_bound_comparator);
        data.to_remove_items.insert(to_remove_items_it, uint16_t(id));
    }

    void _erase_to_remove_item(int id)
    {
        const item_type& item = data.items.item(id);
        auto to_remove_items_it = lower_bound(data.to_remove_items.begin(),  data.to_remove_items.end(),
                                              item.tiles_count, tiles_count_lower_bound_comparator);

        while(*to_remove_items_it != id)
        {
            ++to_remove_items_it;
        }

        data.to_remove_items.erase(to_remove_items_it);
    }

    void _insert_to_commit_item(int id, item_type& item)
    {
        if(! item.commit)
        {
            item.commit = true;

            vector<uint16_t, max_items>& to_commit_items =
                    item.compression() == compression_type::NONE ?
                        data.to_commit_uncompressed_items : data.to_commit_compressed_items;
            to_commit_items.push_back(uint16_t(id));
        }
    }

    void _erase_to_commit_item(int id, item_type& item)
    {
        if(item.commit)
        {
            item.commit = false;

            vector<uint16_t, max_items>& to_commit_items =
                    item.compression() == compression_type::NONE ?
                        data.to_commit_uncompressed_items : data.to_commit_compressed_items;
            to_commit_items.erase(bn::find(to_commit_items.begin(), to_commit_items.end(), id));
        }
    }

    [[nodiscard]] int _find_impl(const tile* tiles_data, [[maybe_unused]] compression_type compression,
                                 [[maybe_unused]] int tiles_count)
    {
        auto items_map_iterator = data.items_map.find(tiles_data);

        if(items_map_iterator != data.items_map.end())
        {
            int id = items_map_iterator->second;
            item_type& item = data.items.item(id);

            BN_BASIC_ASSERT(tiles_data == item.data,
                            "Tiles data does not match item tiles data: ", tiles_data, " - ", item.data);
            BN_BASIC_ASSERT(compression == item.compression(),
                            "Tiles compression does not match item tiles compression: ",
                            int(compression), " - ", int(item.compression()));
            BN_BASIC_ASSERT(tiles_count == item.tiles_count,
                            "Tiles count does not match item tiles count: ", tiles_count, " - ", item.tiles_count);

            switch(item.status())
            {

            case status_type::FREE:
                BN_ERROR("Invalid item state");
                break;

            case status_type::USED:
                ++item.usages;
                break;

            case status_type::TO_REMOVE:
                item.usages = 1;
                item.set_status(status_type::USED);
                _erase_to_remove_item(id);
                data.to_remove_tiles_count -= item.tiles_count;

                if(item.commit_if_recovered)
                {
                    item.commit_if_recovered = false;
                    _insert_to_commit_item(id, item);
                }
                break;

            default:
                BN_ERROR("Invalid item status: ", int(item.status()));
                break;
            }

            BN_SPRITE_TILES_LOG("FOUND. start_tile: ", data.items.item(id).start_tile);
            BN_SPRITE_TILES_LOG_STATUS();

            return id;
        }

        BN_SPRITE_TILES_LOG("NOT FOUND");

        return -1;
    }

    __attribute__((noinline)) void _hw_commit(
            const tile* source_tiles_ptr, compression_type compression, int index, int count)
    {
        switch(compression)
        {

        case compression_type::NONE:
            hw::sprite_tiles::commit_with_cpu(source_tiles_ptr, index, count);
            break;

        case compression_type::LZ77:
            hw::decompress::lz77(source_tiles_ptr, hw::sprite_tiles::tile_vram(index));
            break;

        case compression_type::RUN_LENGTH:
            hw::decompress::rl_vram(source_tiles_ptr, hw::sprite_tiles::tile_vram(index));
            break;

        case compression_type::HUFFMAN:
            hw::decompress::huff(source_tiles_ptr, hw::sprite_tiles::tile_vram(index));
            break;

        default:
            BN_ERROR("Unknown compression type: ", int(compression));
            break;
        }
    }

    [[nodiscard]] int _create_item(
            int id, const tile* tiles_data, compression_type compression, int tiles_count, bool delay_commit)
    {
        item_type& item = data.items.item(id);
        int new_item_tiles_count = int(item.tiles_count) - tiles_count;

        switch(item.status())
        {

        case status_type::FREE:
            data.free_tiles_count -= tiles_count;
            break;

        case status_type::USED:
            BN_ERROR("Invalid item state");
            break;

        case status_type::TO_REMOVE:
            item.commit_if_recovered = false;
            data.items_map.erase(item.data);
            data.to_remove_tiles_count -= tiles_count;
            break;

        default:
            BN_ERROR("Invalid item status: ", int(item.status()));
            break;
        }

        item.data = tiles_data;
        item.set_compression(compression);
        item.tiles_count = uint16_t(tiles_count);
        item.usages = 1;
        item.set_status(status_type::USED);

        if(tiles_data)
        {
            if(delay_commit)
            {
                _insert_to_commit_item(id, item);
            }
            else
            {
                _hw_commit(tiles_data, compression, int(item.start_tile), tiles_count);
            }
        }

        int new_free_item_id;

        if(new_item_tiles_count)
        {
            BN_BASIC_ASSERT(! data.items.full(), "No more sprite tiles items available");

            item_type new_item;
            new_item.start_tile = item.start_tile + item.tiles_count;
            new_item.tiles_count = uint16_t(new_item_tiles_count);

            auto new_item_iterator = data.items.insert(item.next_index, new_item);
            new_free_item_id = new_item_iterator.id();
        }
        else
        {
            new_free_item_id = -1;
        }

        return new_free_item_id;
    }

    [[nodiscard]] int _create_impl(const tile* tiles_data, compression_type compression, int tiles_count)
    {
        int to_remove_tiles_count = data.to_remove_tiles_count;

        if(tiles_count <= to_remove_tiles_count &&
                (data.delay_commit || compression == compression_type::NONE))
        {
            auto to_remove_items_end = data.to_remove_items.end();
            auto to_remove_items_it = lower_bound(
                        data.to_remove_items.begin(), to_remove_items_end, tiles_count,
                        tiles_count_lower_bound_comparator);

            if(to_remove_items_it != to_remove_items_end)
            {
                int id = *to_remove_items_it;
                const item_type& item = data.items.item(id);

                if(int(item.tiles_count) == tiles_count)
                {
                    data.to_remove_items.erase(to_remove_items_it);

                    int new_free_item_id = _create_item(id, tiles_data, compression, tiles_count, true);

                    if(new_free_item_id >= 0)
                    {
                        _insert_free_item(new_free_item_id);
                    }

                    return id;
                }
            }
        }

        if(tiles_count <= data.free_tiles_count)
        {
            auto free_items_end = data.free_items.end();
            auto free_items_it = lower_bound(data.free_items.begin(), free_items_end, tiles_count,
                                             tiles_count_lower_bound_comparator);

            if(free_items_it != free_items_end)
            {
                int id = *free_items_it;
                int new_free_item_id = _create_item(id, tiles_data, compression, tiles_count, data.delay_commit);

                if(new_free_item_id >= 0)
                {
                    _insert_free_item(new_free_item_id, free_items_it);
                    ++free_items_it;
                }

                data.free_items.erase(free_items_it);
                return id;
            }
        }

        if(to_remove_tiles_count)
        {
            update();
            data.delay_commit = true;
            return _create_impl(tiles_data, compression, tiles_count);
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
                                             tiles_count_lower_bound_comparator);

            if(free_items_it != free_items_end)
            {
                int id = *free_items_it;
                int new_free_item_id = _create_item(id, nullptr, compression_type::NONE, tiles_count, false);

                if(new_free_item_id >= 0)
                {
                    _insert_free_item(new_free_item_id, free_items_it);
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
    new(&data) static_data();

    BN_SPRITE_TILES_LOG("sprite_tiles_manager - INIT");

    item_type new_item;
    new_item.tiles_count = hw::sprite_tiles::tiles_count();
    data.items.push_front(new_item);
    data.free_items.push_back(uint16_t(data.items.begin().id()));
    data.free_tiles_count = new_item.tiles_count;

    BN_SPRITE_TILES_LOG_STATUS();
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

#if BN_CFG_LOG_ENABLED
    void log_status()
    {
        #if BN_CFG_SPRITE_TILES_LOG_ENABLED
            BN_SPRITE_TILES_LOG_STATUS();
        #else
            BN_LOG("items: ", data.items.size());
            BN_LOG('[');

            for(const item_type& item : data.items)
            {
                BN_LOG("    ",
                        (item.status() == status_type::FREE ? "free" :
                                item.status() == status_type::USED ? "used" : "to_remove"),
                        " - data: ", item.data,
                        " - start_tile: ", item.start_tile,
                        " - tiles_count: ", item.tiles_count,
                        " - usages: ", item.usages);
            }

            BN_LOG(']');

            BN_LOG("free_tiles_count: ", data.free_tiles_count);
            BN_LOG("to_remove_tiles_count: ", data.to_remove_tiles_count);
        #endif
    }
#endif

int find(const span<const tile>& tiles_ref, compression_type compression)
{
    const tile* tiles_data = tiles_ref.data();
    int tiles_count = tiles_ref.size();

    BN_SPRITE_TILES_LOG("sprite_tiles_manager - FIND: ", tiles_data, " - ", tiles_count, " - ", int(compression));

    return _find_impl(tiles_data, compression, tiles_count);
}

int create(const span<const tile>& tiles_ref, compression_type compression)
{
    const tile* tiles_data = tiles_ref.data();
    int tiles_count = tiles_ref.size();

    BN_SPRITE_TILES_LOG("sprite_tiles_manager - CREATE: ", tiles_data, " - ", tiles_count, " - ", int(compression));

    int result = _find_impl(tiles_data, compression, tiles_count);

    if(result >= 0)
    {
        return result;
    }

    result = _create_impl(tiles_data, compression, tiles_count);

    if(result >= 0)
    {
        data.items_map.insert(tiles_data, result);

        BN_SPRITE_TILES_LOG("CREATED. start_tile: ", data.items.item(result).start_tile);
        BN_SPRITE_TILES_LOG_STATUS();
    }
    else
    {
        BN_SPRITE_TILES_LOG("NOT CREATED");

        #if BN_CFG_LOG_ENABLED
            log_status();
        #endif

        BN_ERROR("Sprite tiles create failed:",
                 "\n\tTiles data: ", tiles_data,
                 "\n\tTiles count: ", tiles_count,
                 "\n\nThere's no more available VRAM.",
                 _status_log_message);
    }

    return result;
}

int allocate(int tiles_count, bpp_mode bpp)
{
    BN_SPRITE_TILES_LOG("sprite_tiles_manager - ALLOCATE: ", tiles_count, " - ", int(bpp));

    BN_ASSERT(sprite_tiles_item::valid_tiles_count(tiles_count, bpp),
              "Invalid tiles count: ", tiles_count, " - ", int(bpp));

    int result = _allocate_impl(tiles_count);

    if(result >= 0)
    {
        BN_SPRITE_TILES_LOG("ALLOCATED. start_tile: ", data.items.item(result).start_tile);
        BN_SPRITE_TILES_LOG_STATUS();
    }
    else
    {
        BN_SPRITE_TILES_LOG("NOT ALLOCATED");

        #if BN_CFG_LOG_ENABLED
            log_status();
        #endif

        BN_ERROR("Sprite tiles allocate failed. Tiles count: ", tiles_count,
                 "\n\nThere's no more available VRAM.",
                 _status_log_message);
    }

    return result;
}

int create_optional(const span<const tile>& tiles_ref, compression_type compression)
{
    const tile* tiles_data = tiles_ref.data();
    int tiles_count = tiles_ref.size();

    BN_SPRITE_TILES_LOG("sprite_tiles_manager - CREATE OPTIONAL: ", tiles_data, " - ", tiles_count, " - ",
                        int(compression));

    int result = _find_impl(tiles_data, compression, tiles_count);

    if(result >= 0)
    {
        return result;
    }

    result = _create_impl(tiles_data, compression, tiles_count);

    if(result >= 0)
    {
        data.items_map.insert(tiles_data, result);

        BN_SPRITE_TILES_LOG("CREATED. start_tile: ", data.items.item(result).start_tile);
        BN_SPRITE_TILES_LOG_STATUS();
    }
    else
    {
        BN_SPRITE_TILES_LOG("NOT CREATED");
    }

    return result;
}

int allocate_optional(int tiles_count, bpp_mode bpp)
{
    BN_SPRITE_TILES_LOG("sprite_tiles_manager - ALLOCATE OPTIONAL: ", tiles_count, " - ", int(bpp));

    BN_ASSERT(sprite_tiles_item::valid_tiles_count(tiles_count, bpp),
              "Invalid tiles count: ", tiles_count, " - ", int(bpp));

    int result = _allocate_impl(tiles_count);

    if(result >= 0)
    {
        BN_SPRITE_TILES_LOG("ALLOCATED. start_tile: ", data.items.item(result).start_tile);
        BN_SPRITE_TILES_LOG_STATUS();
    }
    else
    {
        BN_SPRITE_TILES_LOG("NOT ALLOCATED");
    }

    return result;
}

void increase_usages(int id)
{
    item_type& item = data.items.item(id);

    BN_SPRITE_TILES_LOG("sprite_tiles_manager - INCREASE_USAGES: ", item.start_tile);

    ++item.usages;

    BN_SPRITE_TILES_LOG_STATUS();
}

void decrease_usages(int id)
{
    item_type& item = data.items.item(id);

    BN_SPRITE_TILES_LOG("sprite_tiles_manager - DECREASE_USAGES: ", item.start_tile);

    --item.usages;

    if(! item.usages)
    {
        item.set_status(status_type::TO_REMOVE);
        item.commit_if_recovered = item.commit;
        _erase_to_commit_item(id, item);
        _insert_to_remove_item(id);
        data.to_remove_tiles_count += item.tiles_count;
    }

    BN_SPRITE_TILES_LOG_STATUS();
}

int start_tile(int id)
{
    return int(data.items.item(id).start_tile);
}

int tiles_count(int id)
{
    return int(data.items.item(id).tiles_count);
}

compression_type compression(int id)
{
    return data.items.item(id).compression();
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

void set_tiles_ref(int id, const span<const tile>& tiles_ref, compression_type compression)
{
    item_type& item = data.items.item(id);
    const tile* old_tiles_data = item.data;
    const tile* new_tiles_data = tiles_ref.data();

    BN_SPRITE_TILES_LOG("sprite_tiles_manager - SET_TILES_REF: ", item.start_tile, " - ", new_tiles_data,
                        " - ", tiles_ref.size(), " - ", int(compression));

    BN_BASIC_ASSERT(int(item.tiles_count) == tiles_ref.size(),
                    "Tiles count does not match item tiles count: ", int(item.tiles_count), " - ", tiles_ref.size());

    compression_type item_compression = item.compression();

    if(old_tiles_data != new_tiles_data)
    {
        BN_BASIC_ASSERT(old_tiles_data, "Item has no data");
        BN_BASIC_ASSERT(data.items_map.find(new_tiles_data) == data.items_map.end(),
                        "Multiple copies of the same tiles data not supported");

        data.items_map.erase(old_tiles_data);
        data.items_map.insert(new_tiles_data, id);

        if(compression != item_compression)
        {
            if(compression == compression_type::NONE || item_compression == compression_type::NONE)
            {
                _erase_to_commit_item(id, item);
            }

            item.set_compression(compression);
        }

        item.data = new_tiles_data;
        _insert_to_commit_item(id, item);

        BN_SPRITE_TILES_LOG_STATUS();
    }
    else if(compression != item_compression)
    {
        if(compression == compression_type::NONE || item_compression == compression_type::NONE)
        {
            _erase_to_commit_item(id, item);
        }

        item.set_compression(compression);
        _insert_to_commit_item(id, item);

        BN_SPRITE_TILES_LOG_STATUS();
    }
}

void reload_tiles_ref(int id)
{
    item_type& item = data.items.item(id);

    BN_SPRITE_TILES_LOG("sprite_tiles_manager - RELOAD_TILES_REF: ", item.start_tile);

    BN_BASIC_ASSERT(item.data, "Item has no data");

    _insert_to_commit_item(id, item);

    BN_SPRITE_TILES_LOG_STATUS();
}

optional<span<tile>> vram(int id)
{
    const item_type& item = data.items.item(id);
    optional<span<tile>> result;

    if(! item.data)
    {
        result.emplace(hw::sprite_tiles::vram(int(item.start_tile)), int(item.tiles_count));
    }

    return result;
}

void update()
{
    if(data.to_remove_tiles_count)
    {
        BN_SPRITE_TILES_LOG("sprite_tiles_manager - UPDATE");

        auto begin = data.items.begin();
        auto end = data.items.end();

        for(int to_remove_item_index : data.to_remove_items)
        {
            auto iterator = data.items.it(to_remove_item_index);
            item_type& item = *iterator;

            if(item.data)
            {
                data.items_map.erase(item.data);
                item.data = nullptr;
            }

            item.set_status(status_type::FREE);
            item.commit_if_recovered = false;
            data.free_tiles_count += item.tiles_count;

            auto next_iterator = iterator;
            ++next_iterator;

            if(next_iterator != end)
            {
                const item_type& next_item = *next_iterator;

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

                const item_type& previous_item = *previous_iterator;

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

        BN_SPRITE_TILES_LOG_STATUS();
    }

    data.delay_commit = false;
}

void commit_uncompressed(bool use_dma)
{
    if(! data.to_commit_uncompressed_items.empty())
    {
        BN_SPRITE_TILES_LOG("sprite_tiles_manager - COMMIT UNCOMPRESSED");

        if(use_dma)
        {
            for(int item_index : data.to_commit_uncompressed_items)
            {
                item_type& item = data.items.item(item_index);
                hw::sprite_tiles::commit_with_dma(item.data, int(item.start_tile), int(item.tiles_count));
                item.commit = false;
            }
        }
        else
        {
            for(int item_index : data.to_commit_uncompressed_items)
            {
                item_type& item = data.items.item(item_index);
                hw::sprite_tiles::commit_with_cpu(item.data, int(item.start_tile), int(item.tiles_count));
                item.commit = false;
            }
        }

        data.to_commit_uncompressed_items.clear();

        BN_SPRITE_TILES_LOG_STATUS();
    }
}

void commit_compressed()
{
    if(! data.to_commit_compressed_items.empty())
    {
        BN_SPRITE_TILES_LOG("sprite_tiles_manager - COMMIT COMPRESSED");

        for(int item_index : data.to_commit_compressed_items)
        {
            item_type& item = data.items.item(item_index);
            _hw_commit(item.data, item.compression(), int(item.start_tile), int(item.tiles_count));
            item.commit = false;
        }

        data.to_commit_compressed_items.clear();

        BN_SPRITE_TILES_LOG_STATUS();
    }
}

}
