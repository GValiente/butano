#include "btn_bg_blocks_manager.h"

#include "btn_size.h"
#include "btn_span.h"
#include "btn_tile.h"
#include "btn_vector.h"
#include "btn_limits.h"
#include "btn_optional.h"
#include "btn_hash_map.h"
#include "btn_bgs_manager.h"
#include "btn_bg_tiles_ptr.h"
#include "btn_bg_palette_ptr.h"
#include "btn_palette_bpp_mode.h"
#include "btn_config_bg_blocks.h"
#include "../hw/include/btn_hw_bg_blocks.h"

#if BTN_CFG_BG_BLOCKS_LOG_ENABLED
    #include "btn_log.h"

    static_assert(BTN_CFG_LOG_ENABLED, "Log is not enabled");
#endif

namespace btn::bg_blocks_manager
{

namespace
{
    [[nodiscard]] constexpr int _tiles_to_half_words(int tiles)
    {
        return tiles * int(sizeof(tile) / 2);
    }

    [[nodiscard]] constexpr int _half_words_to_tiles(int half_words)
    {
        return half_words / int(sizeof(tile) / 2);
    }

    [[nodiscard]] constexpr int _blocks_to_half_words(int blocks)
    {
        return blocks * hw::bg_blocks::half_words_per_block();
    }

    [[nodiscard]] constexpr int _half_words_to_blocks(int half_words)
    {
        return half_words / hw::bg_blocks::half_words_per_block();
    }

    [[nodiscard]] constexpr int _blocks_to_tiles(int blocks)
    {
        int half_words = _blocks_to_half_words(blocks);
        return _half_words_to_tiles(half_words);
    }

    [[nodiscard]] constexpr int _tiles_to_blocks(int tiles)
    {
        int half_words = _tiles_to_half_words(tiles);
        return _half_words_to_blocks(half_words);
    }


    constexpr const int max_items = bg_tiles::blocks_count();
    constexpr const int max_list_items = max_items + 1;
    constexpr const int max_tiles_half_words = _blocks_to_half_words(hw::bg_blocks::max_blocks_per_tiles());


    class item_type
    {

    public:
        enum class status_type
        {
            FREE,
            USED,
            TO_REMOVE
        };

        const uint16_t* data = nullptr;
        unsigned usages = 0;
        optional<bg_tiles_ptr> tiles_ptr;
        optional<bg_palette_ptr> palette_ptr;
        uint16_t width = 0;
        uint8_t height = 0;
        uint8_t start_block = 0;
        uint8_t blocks_count = 0;
        uint8_t next_index = max_list_items;
        bool commit = false;

    private:
        uint8_t _status = uint8_t(status_type::FREE);

    public:
        [[nodiscard]] bool is_tiles() const
        {
            return ! palette_ptr;
        }

        [[nodiscard]] status_type status() const
        {
            return static_cast<status_type>(_status);
        }

        void set_status(status_type status)
        {
            _status = uint8_t(status);
        }

        [[nodiscard]] int half_words() const
        {
            return width * height;
        }

        [[nodiscard]] int tiles() const
        {
            return _half_words_to_tiles(half_words());
        }

        [[nodiscard]] int tiles_offset() const
        {
            int tiles_start_block = tiles_ptr->id();
            int offset_blocks_count = tiles_start_block % hw::bg_blocks::tiles_alignment_blocks_count();
            int result = _blocks_to_tiles(offset_blocks_count);

            if(palette_ptr->bpp_mode() == palette_bpp_mode::BPP_8)
            {
                result /= 2;
            }

            return result;
        }

        [[nodiscard]] int palette_offset() const
        {
            return palette_ptr ? palette_ptr->id() : 0;
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

            iterator& operator++()
            {
                item_type& item = _list->_items[_index];
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
                _free_indices[index] = int8_t(index + 1);
            }
        }

        [[nodiscard]] int size() const
        {
            return _free_indices.available();
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
        vector<int8_t, max_items> _free_indices;

        void _join(int index, int new_index)
        {
            _items[index].next_index = uint8_t(new_index);
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
            _free_indices.push_back(int8_t(next_index));

            auto next_next_index = int(_items[next_index].next_index);
            _join(index, next_next_index);
        }
    };


    struct create_data
    {
        const uint16_t* data_ptr;
        int blocks_count;
        int width;
        int height;
        optional<bg_tiles_ptr> tiles_ptr;
        optional<bg_palette_ptr> palette_ptr;

        static create_data tiles(const uint16_t* data_ptr, int half_words)
        {
            return create_data{ data_ptr, _ceil_half_words_to_blocks(half_words), half_words, 1, nullopt, nullopt };
        }

        static create_data map(const uint16_t* data_ptr, const size& dimensions, bg_tiles_ptr&& tiles_ptr,
                               bg_palette_ptr&& palette_ptr)
        {
            return create_data{ data_ptr, _ceil_half_words_to_blocks(dimensions.width() * dimensions.height()),
                        dimensions.width(), dimensions.height(), move(tiles_ptr), move(palette_ptr) };
        }

    private:
        [[nodiscard]] static int _ceil_half_words_to_blocks(int half_words)
        {
            int result = half_words / hw::bg_blocks::half_words_per_block();

            if(half_words % hw::bg_blocks::half_words_per_block())
            {
                ++result;
            }

            return result;
        }
    };


    class static_data
    {

    public:
        items_list items;
        hash_map<const uint16_t*, uint16_t, max_items * 2> items_map;
        int free_blocks_count = 0;
        int to_remove_blocks_count = 0;
        bool check_commit = false;
        bool delay_commit = false;
    };

    BTN_DATA_EWRAM static_data data;


    #if BTN_CFG_BG_BLOCKS_LOG_ENABLED
        void _log_status()
        {
            BTN_LOG("items: ", data.items.size());
            BTN_LOG('[');

            for(item_type& item : data.items)
            {
                if(item.status() == item_type::status_type::FREE)
                {
                    BTN_LOG("    ",
                            "free",
                            " - start_block: ", item.start_block,
                            " - blocks_count: ", item.blocks_count);
                }
                else if(item.is_tiles())
                {
                    BTN_LOG("    ",
                            (item.status() == item_type::status_type::USED ? "used" : "to_remove"),
                            "_tiles",
                            " - start_block: ", item.start_block,
                            " - blocks_count: ", item.blocks_count,
                            " - data: ", item.data,
                            " - usages: ", item.usages,
                            " - tiles: ", item.tiles(),
                            (item.commit ? " - commit" : " - no_commit"));
                }
                else
                {
                    BTN_LOG("    ",
                            (item.status() == item_type::status_type::USED ? "used" : "to_remove"),
                            "_map",
                            " - start_block: ", item.start_block,
                            " - blocks_count: ", item.blocks_count,
                            " - data: ", item.data,
                            " - usages: ", item.usages,
                            " - width: ", item.width,
                            " - height: ", item.height,
                            " - tiles: ", item.tiles_ptr->id(),
                            " - palette: ", item.palette_ptr->id(),
                            " - tiles_offset: ", item.tiles_offset(),
                            (item.commit ? " - commit" : " - no_commit"));
                }
            }

            BTN_LOG(']');

            BTN_LOG("items_map: ", data.items_map.size());
            BTN_LOG('[');

            for(const auto& items_map_item : data.items_map)
            {
                BTN_LOG("    data: ", items_map_item.first,
                        " - start_block: ", data.items.item(items_map_item.second).start_block);
            }

            BTN_LOG(']');

            BTN_LOG("free_blocks_count: ", data.free_blocks_count);
            BTN_LOG("to_remove_blocks_count: ", data.to_remove_blocks_count);
            BTN_LOG("check_commit: ", (data.check_commit ? "true" : "false"));
            BTN_LOG("delay_commit: ", (data.delay_commit ? "true" : "false"));
        }

        #define BTN_BG_BLOCKS_LOG BTN_LOG

        #define BTN_BG_BLOCKS_LOG_STATUS \
            _log_status
    #else
        #define BTN_BG_BLOCKS_LOG(...) \
            do \
            { \
            } while(false)

        #define BTN_BG_BLOCKS_LOG_STATUS(...) \
            do \
            { \
            } while(false)
    #endif


    void _commit_item(item_type& item)
    {
        if(item.is_tiles())
        {
            hw::bg_blocks::commit_tiles(item.data, item.start_block, item.half_words());
        }
        else
        {
            hw::bg_blocks::commit_map(item.data, item.start_block, item.half_words(), item.tiles_offset(),
                                      item.palette_offset());
        }
    }

    void _check_commit_item(int id, const uint16_t* data_ptr, bool delay_commit)
    {
        item_type& item = data.items.item(id);
        item.data = data_ptr;
        data.items_map.insert(data_ptr, id);

        if(delay_commit)
        {
            item.commit = true;
            data.check_commit = true;
        }
        else
        {
            _commit_item(item);
        }
    }

    [[nodiscard]] int _create_item(int id, int padding_blocks_count, create_data&& create_data)
    {
        item_type* item = &data.items.item(id);
        int blocks_count = create_data.blocks_count;

        if(padding_blocks_count)
        {
            BTN_ASSERT(! data.items.full(), "No more items allowed");

            int new_item_blocks_count = item->blocks_count - padding_blocks_count;
            item->blocks_count = uint8_t(padding_blocks_count);

            item_type new_item;
            new_item.start_block = item->start_block + item->blocks_count;
            new_item.blocks_count = uint8_t(new_item_blocks_count);

            auto new_item_iterator = data.items.insert_after(id, new_item);
            id = new_item_iterator.id();
            item = &data.items.item(id);
        }

        if(int new_item_blocks_count = item->blocks_count - blocks_count)
        {
            BTN_ASSERT(! data.items.full(), "No more items allowed");

            if(create_data.palette_ptr)
            {
                item->blocks_count -= blocks_count;

                item_type new_item;
                new_item.start_block = uint8_t(item->start_block + item->blocks_count);

                auto new_item_iterator = data.items.insert_after(id, new_item);
                id = new_item_iterator.id();
                item = &data.items.item(id);
            }
            else
            {
                item_type new_item;
                new_item.start_block = uint8_t(item->start_block + blocks_count);
                new_item.blocks_count = uint8_t(new_item_blocks_count);
                data.items.insert_after(id, new_item);
            }
        }

        const uint16_t* data_ptr = create_data.data_ptr;
        item->data = create_data.data_ptr;
        item->blocks_count = uint8_t(blocks_count);
        item->tiles_ptr = move(create_data.tiles_ptr);
        item->palette_ptr = move(create_data.palette_ptr);
        item->width = uint16_t(create_data.width);
        item->height = uint8_t(create_data.height);
        item->usages = 1;
        item->set_status(item_type::status_type::USED);
        item->commit = false;
        data.free_blocks_count -= blocks_count;

        if(data_ptr)
        {
            _check_commit_item(id, data_ptr, data.delay_commit);
        }

        return id;
    }

    bool _remove_adjacent_item(int adjacent_id, item_type& current_item)
    {
        const item_type& adjacent_item = data.items.item(adjacent_id);
        item_type::status_type adjacent_item_status = adjacent_item.status();
        bool remove = adjacent_item_status != item_type::status_type::USED;

        if(remove)
        {
            current_item.blocks_count += adjacent_item.blocks_count;

            if(adjacent_item_status == item_type::status_type::TO_REMOVE)
            {
                if(adjacent_item.data)
                {
                    data.items_map.erase(adjacent_item.data);
                }

                data.free_blocks_count += adjacent_item.blocks_count;
            }
        }

        return remove;
    }

    template<bool tiles>
    [[nodiscard]] int _create_impl(create_data&& create_data)
    {
        const uint16_t* data_ptr = create_data.data_ptr;

        if(! data_ptr && data.delay_commit)
        {
            return -1;
        }

        int blocks_count = create_data.blocks_count;

        if(blocks_count <= data.free_blocks_count)
        {
            auto smallest_iterator = data.items.end();
            int smallest_blocks_count = numeric_limits<int>::max();
            int smallest_padding_blocks_count = 0;

            for(auto iterator = data.items.begin(), end = data.items.end(); iterator != end; ++iterator)
            {
                const item_type& item = *iterator;

                if(item.status() == item_type::status_type::FREE)
                {
                    int padding_blocks_count = 0;

                    if(tiles)
                    {
                        int alignment_blocks_count = hw::bg_blocks::tiles_alignment_blocks_count();
                        int extra_blocks_count = item.start_block % alignment_blocks_count;

                        if(blocks_count + extra_blocks_count > hw::bg_blocks::max_blocks_per_tiles())
                        {
                            padding_blocks_count = alignment_blocks_count - extra_blocks_count;
                        }
                    }

                    if(item.blocks_count >= blocks_count + padding_blocks_count)
                    {
                        if(item.blocks_count < smallest_blocks_count)
                        {
                            smallest_iterator = iterator;
                            smallest_blocks_count = item.blocks_count;
                            smallest_padding_blocks_count = padding_blocks_count;
                        }
                    }
                }
            }

            if(smallest_iterator != data.items.end())
            {
                return _create_item(smallest_iterator.id(), smallest_padding_blocks_count, move(create_data));
            }
        }

        if(data_ptr && blocks_count <= data.to_remove_blocks_count)
        {
            update();
            data.delay_commit = true;
            return _create_impl<tiles>(move(create_data));
        }

        return -1;
    }
}

void init()
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - INIT");

    item_type new_item;
    new_item.blocks_count = max_items;
    data.items.init();
    data.items.push_front(new_item);
    data.free_blocks_count = new_item.blocks_count;

    BTN_BG_BLOCKS_LOG_STATUS();
}

int used_tiles_count()
{
    return _blocks_to_tiles(used_tile_blocks_count());
}

int available_tiles_count()
{
    return _blocks_to_tiles(available_tile_blocks_count());
}

int used_tile_blocks_count()
{
    int result = 0;

    for(const item_type& item : data.items)
    {
        if(item.status() != item_type::status_type::FREE && item.is_tiles())
        {
            result += item.blocks_count;
        }
    }

    return result;
}

int available_tile_blocks_count()
{
    return data.free_blocks_count;
}

int used_map_cells_count()
{
    return _blocks_to_half_words(used_map_blocks_count());
}

int available_map_cells_count()
{
    return _blocks_to_half_words(available_map_blocks_count());
}

int used_map_blocks_count()
{
    int result = 0;

    for(const item_type& item : data.items)
    {
        if(item.status() != item_type::status_type::FREE && ! item.is_tiles())
        {
            result += item.blocks_count;
        }
    }

    return result;
}

int available_map_blocks_count()
{
    return data.free_blocks_count;
}

int find_tiles(const span<const tile>& tiles_ref)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - FIND TILES: ", tiles_ref.data(), " - ", tiles_ref.size());

    auto data_ptr = reinterpret_cast<const uint16_t*>(tiles_ref.data());
    BTN_ASSERT(data_ptr, "Tiles ref is null");

    auto items_map_iterator = data.items_map.find(data_ptr);

    if(items_map_iterator != data.items_map.end())
    {
        auto id = int(items_map_iterator->second);
        item_type& item = data.items.item(id);
        BTN_ASSERT(_tiles_to_half_words(tiles_ref.size()) == item.half_words(),
                   "Tiles count does not match item tiles count: ",
                   _tiles_to_half_words(tiles_ref.size()), " - ", item.half_words());

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
            data.to_remove_blocks_count -= item.blocks_count;
            break;
        }

        BTN_BG_BLOCKS_LOG("FOUND. start_block: ", data.items.item(id).start_block);
        BTN_BG_BLOCKS_LOG_STATUS();

        return id;
    }

    BTN_BG_BLOCKS_LOG("NOT FOUND");
    return -1;
}

int find_regular_map(const regular_bg_map_cell& map_cells_ref, [[maybe_unused]] const size& map_dimensions,
                     [[maybe_unused]] const bg_tiles_ptr& tiles_ptr,
                     [[maybe_unused]] const bg_palette_ptr& palette_ptr)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - FIND REGULAR MAP: ", &map_cells_ref, " - ",
                      map_dimensions.width(), " - ", map_dimensions.height(), " - ", palette_ptr.id());

    auto data_ptr = reinterpret_cast<const uint16_t*>(&map_cells_ref);
    auto items_map_iterator = data.items_map.find(data_ptr);

    if(items_map_iterator != data.items_map.end())
    {
        auto id = int(items_map_iterator->second);
        item_type& item = data.items.item(id);
        BTN_ASSERT(map_dimensions.width() == item.width, "Width does not match item width: ",
                   map_dimensions.width(), " - ", item.width);
        BTN_ASSERT(map_dimensions.height() == item.height, "Height does not match item height: ",
                   map_dimensions.height(), " - ", item.height);
        BTN_ASSERT(tiles_ptr.id() == item.tiles_ptr->id(), "Tiles does not match item tiles: ",
                   tiles_ptr.id(), " - ", item.tiles_ptr->id());
        BTN_ASSERT(palette_ptr.id() == item.palette_ptr->id(), "Palette does not match item palette: ",
                   palette_ptr.id(), " - ", item.palette_ptr->id());

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
            data.to_remove_blocks_count -= item.blocks_count;
            break;
        }

        BTN_BG_BLOCKS_LOG("FOUND. start_block: ", data.items.item(id).start_block);
        BTN_BG_BLOCKS_LOG_STATUS();

        return id;
    }

    BTN_BG_BLOCKS_LOG("NOT FOUND");
    return -1;
}

int create_tiles(const span<const tile>& tiles_ref)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - CREATE TILES: ", tiles_ref.data(), " - ", tiles_ref.size());

    int half_words = _tiles_to_half_words(tiles_ref.size());
    BTN_ASSERT(half_words > 0 && half_words <= max_tiles_half_words,
               "Invalid tiles count: ", tiles_ref.size(), " - ", half_words);

    auto data_ptr = reinterpret_cast<const uint16_t*>(tiles_ref.data());
    BTN_ASSERT(data.items_map.find(data_ptr) == data.items_map.end(), "Multiple copies of the same data not supported");

    int result = _create_impl<true>(create_data::tiles(data_ptr, half_words));

    if(result >= 0)
    {
        BTN_BG_BLOCKS_LOG("CREATED. start_block: ", data.items.item(result).start_block);
        BTN_BG_BLOCKS_LOG_STATUS();
    }
    else
    {
        BTN_BG_BLOCKS_LOG("NOT CREATED");
    }

    return result;
}

int create_regular_map(const regular_bg_map_cell& map_cells_ref, const size& map_dimensions, bg_tiles_ptr&& tiles_ptr,
                       bg_palette_ptr&& palette_ptr)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - CREATE REGULAR MAP: ", &map_cells_ref, " - ",
                      map_dimensions.width(), " - ", map_dimensions.height(), " - ", tiles_ptr.id(), " - ",
                      palette_ptr.id());

    BTN_ASSERT(map_dimensions.width() == 32 || map_dimensions.width() == 64, "Invalid width: ", map_dimensions.width());
    BTN_ASSERT(map_dimensions.height() == 32 || map_dimensions.height() == 64, "Invalid height: ", map_dimensions.height());
    BTN_ASSERT(tiles_ptr.valid_tiles_count(palette_ptr.bpp_mode()), "Invalid tiles count: ", tiles_ptr.tiles_count());

    auto data_ptr = reinterpret_cast<const uint16_t*>(&map_cells_ref);
    BTN_ASSERT(data.items_map.find(data_ptr) == data.items_map.end(), "Multiple copies of the same data not supported");

    int result = _create_impl<false>(create_data::map(data_ptr, map_dimensions, move(tiles_ptr), move(palette_ptr)));

    if(result >= 0)
    {
        BTN_BG_BLOCKS_LOG("CREATED. start_block: ", data.items.item(result).start_block);
        BTN_BG_BLOCKS_LOG_STATUS();
    }
    else
    {
        BTN_BG_BLOCKS_LOG("NOT CREATED");
    }

    return result;
}

int allocate_tiles(int tiles_count)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - ALLOCATE TILES: ", tiles_count);

    int half_words = _tiles_to_half_words(tiles_count);
    BTN_ASSERT(half_words > 0 && half_words <= max_tiles_half_words,
               "Invalid tiles count: ", tiles_count, " - ", half_words);

    int result = _create_impl<true>(create_data::tiles(nullptr, half_words));

    if(result >= 0)
    {
        BTN_BG_BLOCKS_LOG("ALLOCATED. start_block: ", data.items.item(result).start_block);
        BTN_BG_BLOCKS_LOG_STATUS();
    }
    else
    {
        BTN_BG_BLOCKS_LOG("NOT ALLOCATED");
    }

    return result;
}

int allocate_regular_map(const size& map_dimensions, bg_tiles_ptr&& tiles_ptr, bg_palette_ptr&& palette_ptr)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - ALLOCATE REGULAR MAP: ", map_dimensions.width(), " - ",
                      map_dimensions.height(), " - ", tiles_ptr.id(), " - ", palette_ptr.id());

    BTN_ASSERT(map_dimensions.width() == 32 || map_dimensions.width() == 64, "Invalid width: ", map_dimensions.width());
    BTN_ASSERT(map_dimensions.height() == 32 || map_dimensions.height() == 64, "Invalid height: ", map_dimensions.height());
    BTN_ASSERT(tiles_ptr.valid_tiles_count(palette_ptr.bpp_mode()), "Invalid tiles count: ", tiles_ptr.tiles_count());

    int result = _create_impl<false>(create_data::map(nullptr, map_dimensions, move(tiles_ptr), move(palette_ptr)));

    if(result >= 0)
    {
        BTN_BG_BLOCKS_LOG("ALLOCATED. start_block: ", data.items.item(result).start_block);
        BTN_BG_BLOCKS_LOG_STATUS();
    }
    else
    {
        BTN_BG_BLOCKS_LOG("NOT ALLOCATED");
    }

    return result;
}

void increase_usages(int id)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - INCREASE_USAGES: ", id, " - ", data.items.item(id).start_block);

    item_type& item = data.items.item(id);
    ++item.usages;

    BTN_BG_BLOCKS_LOG_STATUS();
}

void decrease_usages(int id)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - DECREASE_USAGES: ", id, " - ", data.items.item(id).start_block);

    item_type& item = data.items.item(id);
    --item.usages;

    if(! item.usages)
    {
        item.set_status(item_type::status_type::TO_REMOVE);
        data.to_remove_blocks_count += item.blocks_count;
    }

    BTN_BG_BLOCKS_LOG_STATUS();
}

int hw_id(int id)
{
    const item_type& item = data.items.item(id);
    return item.start_block;
}

int hw_tiles_cbb(int id)
{
    return hw_id(id) / hw::bg_blocks::tiles_alignment_blocks_count();
}

int tiles_count(int id)
{
    const item_type& item = data.items.item(id);
    return item.tiles();
}

size map_dimensions(int id)
{
    const item_type& item = data.items.item(id);
    return size(item.width, item.height);
}

[[nodiscard]] optional<span<const tile>> tiles_ref(int id)
{
    const item_type& item = data.items.item(id);
    optional<span<const tile>> result;

    if(const uint16_t* data_ptr = item.data)
    {
        auto tiles_ptr = reinterpret_cast<const tile*>(data_ptr);
        result.emplace(tiles_ptr, item.tiles());
    }

    return result;
}

const regular_bg_map_cell* regular_map_cells_ref(int id)
{
    const item_type& item = data.items.item(id);
    return reinterpret_cast<const regular_bg_map_cell*>(item.data);
}

void set_tiles_ref(int id, const span<const tile>& tiles_ref)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - SET TILES REF: ", id, " - ", data.items.item(id).start_block, " - ",
                      tiles_ref.data(), " - ", tiles_ref.size());

    item_type& item = data.items.item(id);
    BTN_ASSERT(item.data, "Item has no data");
    BTN_ASSERT(_tiles_to_half_words(tiles_ref.size()) == item.half_words(),
               "Tiles count does not match item tiles count: ",
               _tiles_to_half_words(tiles_ref.size()), " - ", item.half_words());

    auto data_ptr = reinterpret_cast<const uint16_t*>(tiles_ref.data());

    if(item.data != data_ptr)
    {
        BTN_ASSERT(data.items_map.find(data_ptr) == data.items_map.end(),
                   "Multiple copies of the same data not supported");

        data.items_map.erase(item.data);
        _check_commit_item(id, data_ptr, true);

        BTN_BG_BLOCKS_LOG_STATUS();
    }
}

void set_regular_map_cells_ref(int id, const regular_bg_map_cell& map_cells_ref,
                               [[maybe_unused]] const size& map_dimensions)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - SET REGULAR MAP CELLS REF: ", id, " - ", data.items.item(id).start_block,
                      " - ", &map_cells_ref, " - ", map_dimensions.width(), " - ", map_dimensions.height());

    item_type& item = data.items.item(id);
    BTN_ASSERT(item.data, "Item has no data");
    BTN_ASSERT(map_dimensions.width() == item.width, "Width does not match item width: ",
               map_dimensions.width(), " - ", item.width);
    BTN_ASSERT(map_dimensions.height() == item.height, "Height does not match item height: ",
               map_dimensions.height(), " - ", item.height);

    auto data_ptr = reinterpret_cast<const uint16_t*>(&map_cells_ref);

    if(item.data != data_ptr)
    {
        BTN_ASSERT(data.items_map.find(data_ptr) == data.items_map.end(),
                   "Multiple copies of the same data not supported");

        data.items_map.erase(item.data);
        _check_commit_item(id, data_ptr, true);

        BTN_BG_BLOCKS_LOG_STATUS();
    }
}

void reload(int id)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - RELOAD: ", id, " - ", data.items.item(id).start_block);

    item_type& item = data.items.item(id);
    BTN_ASSERT(item.data, "Item has no data");

    item.commit = true;
    data.check_commit = true;

    BTN_BG_BLOCKS_LOG_STATUS();
}

const bg_tiles_ptr& map_tiles(int id)
{
    const item_type& item = data.items.item(id);
    return *item.tiles_ptr;
}

void set_map_tiles(int id, bg_tiles_ptr&& tiles_ptr)
{
    item_type& item = data.items.item(id);

    if(tiles_ptr != item.tiles_ptr)
    {
        BTN_ASSERT(tiles_ptr.valid_tiles_count(item.palette_ptr->bpp_mode()),
                   "Invalid tiles count: ", tiles_ptr.tiles_count());

        int cbb = tiles_ptr.cbb();

        if(cbb != item.tiles_ptr->cbb())
        {
            bgs_manager::update_map_tiles_cbb(item.start_block, cbb);
        }

        int old_tiles_offset = item.tiles_offset();
        item.tiles_ptr = move(tiles_ptr);

        if(item.tiles_offset() != old_tiles_offset)
        {
            item.commit = true;
            data.check_commit = true;
        }
    }
}

const bg_palette_ptr& map_palette(int id)
{
    const item_type& item = data.items.item(id);
    return *item.palette_ptr;
}

void set_map_palette(int id, bg_palette_ptr&& palette_ptr)
{
    item_type& item = data.items.item(id);

    if(palette_ptr != item.palette_ptr)
    {
        palette_bpp_mode new_bpp_mode = palette_ptr.bpp_mode();
        BTN_ASSERT(item.tiles_ptr->valid_tiles_count(new_bpp_mode),
                   "Invalid tiles count: ", item.tiles_ptr->tiles_count());

        if(new_bpp_mode != item.palette_ptr->bpp_mode())
        {
            bgs_manager::update_map_palette_bpp_mode(item.start_block, new_bpp_mode);
        }

        int old_tiles_offset = item.tiles_offset();
        int old_palette_offset = item.palette_offset();
        item.palette_ptr = move(palette_ptr);

        if(item.tiles_offset() != old_tiles_offset || item.palette_offset() != old_palette_offset)
        {
            item.commit = true;
            data.check_commit = true;
        }
    }
}

void set_map_tiles_and_palette(int id, bg_tiles_ptr&& tiles_ptr, bg_palette_ptr&& palette_ptr)
{
    item_type& item = data.items.item(id);
    palette_bpp_mode new_bpp_mode = palette_ptr.bpp_mode();
    BTN_ASSERT(tiles_ptr.valid_tiles_count(new_bpp_mode), "Invalid tiles count: ", tiles_ptr.tiles_count());

    int old_tiles_offset = item.tiles_offset();
    int old_palette_offset = item.palette_offset();

    if(tiles_ptr != item.tiles_ptr)
    {
        int cbb = tiles_ptr.cbb();

        if(cbb != item.tiles_ptr->cbb())
        {
            bgs_manager::update_map_tiles_cbb(item.start_block, cbb);
        }

        item.tiles_ptr = move(tiles_ptr);
    }

    if(palette_ptr != item.palette_ptr)
    {
        if(new_bpp_mode != item.palette_ptr->bpp_mode())
        {
            bgs_manager::update_map_palette_bpp_mode(item.start_block, new_bpp_mode);
        }

        item.palette_ptr = move(palette_ptr);
    }

    if(item.tiles_offset() != old_tiles_offset || item.palette_offset() != old_palette_offset)
    {
        item.commit = true;
        data.check_commit = true;
    }
}

optional<span<tile>> tiles_vram(int id)
{
    item_type& item = data.items.item(id);
    optional<span<tile>> result;

    if(! item.data)
    {
        auto vram_ptr = reinterpret_cast<tile*>(hw::bg_blocks::vram(item.start_block));
        result.emplace(vram_ptr, item.tiles());
    }

    return result;
}

optional<span<regular_bg_map_cell>> regular_map_vram(int id)
{
    item_type& item = data.items.item(id);
    optional<span<regular_bg_map_cell>> result;

    if(! item.data)
    {
        auto vram_ptr = reinterpret_cast<regular_bg_map_cell*>(hw::bg_blocks::vram(item.start_block));
        result.emplace(vram_ptr, item.half_words());
    }

    return result;
}

void update()
{
    if(data.to_remove_blocks_count)
    {
        BTN_BG_BLOCKS_LOG("bg_blocks_manager - UPDATE");

        for(item_type& item : data.items)
        {
            if(item.status() == item_type::status_type::TO_REMOVE)
            {
                item.tiles_ptr.reset();
                item.palette_ptr.reset();
            }
        }

        auto end = data.items.end();
        auto before_previous_iterator = end;
        auto previous_iterator = data.items.before_begin();
        auto iterator = previous_iterator;
        ++iterator;
        data.to_remove_blocks_count = 0;

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
                item.width = 0;
                item.height = 0;
                item.set_status(item_type::status_type::FREE);
                item.commit = false;
                data.free_blocks_count += item.blocks_count;

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
                        item.start_block = previous_iterator->start_block;
                        data.items.erase_after(before_previous_iterator.id());
                        previous_iterator = before_previous_iterator;
                    }
                }
            }

            before_previous_iterator = previous_iterator;
            previous_iterator = iterator;
            ++iterator;
        }

        BTN_BG_BLOCKS_LOG_STATUS();
    }
}

void commit()
{
    bool do_commit = data.check_commit;

    if(do_commit)
    {
        BTN_BG_BLOCKS_LOG("bg_blocks_manager - COMMIT");

        data.check_commit = false;

        for(item_type& item : data.items)
        {
            if(item.commit)
            {
                item.commit = false;

                if(item.status() == item_type::status_type::USED)
                {
                    _commit_item(item);
                }
            }
        }
    }

    data.delay_commit = false;

    if(do_commit)
    {
        BTN_BG_BLOCKS_LOG_STATUS();
    }
}

}
