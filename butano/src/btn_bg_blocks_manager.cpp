#include "btn_bg_blocks_manager.h"

#include "btn_size.h"
#include "btn_span.h"
#include "btn_vector.h"
#include "btn_limits.h"
#include "btn_optional.h"
#include "btn_hash_map.h"
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
    constexpr const size_t max_items = hw::bg_blocks::count();
    constexpr const size_t max_list_items = max_items + 1;
    constexpr const int max_half_words = max_items * hw::bg_blocks::half_words_per_block();


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
        optional<bg_palette_ptr> palette_ptr;
        uint16_t width = 0;
        uint8_t height = 0;
        uint8_t start_block = 0;
        unsigned blocks_count: 6;
        unsigned next_index: 6;
        unsigned commit: 1;

    private:
        unsigned _status: 2;

    public:
        item_type()
        {
            blocks_count = 0;
            next_index = max_list_items;
            commit = false;
            set_status(status_type::FREE);
        }

        [[nodiscard]] status_type status() const
        {
            return static_cast<status_type>(_status);
        }

        void set_status(status_type status)
        {
            _status = uint8_t(status);
        }

        [[nodiscard]] int palette_offset() const
        {
            return palette_ptr ? palette_ptr->id() : 0;
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

            for(size_t index = 0; index < max_items; ++index)
            {
                _free_indices[index] = int8_t(index + 1);
            }
        }

        [[nodiscard]] size_t size() const
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
                BTN_LOG("    ",
                        (item.status() == item_type::status_type::FREE ? "free" :
                                item.status() == item_type::status_type::USED ? "used" : "to_remove"),
                        " - data: ", item.data,
                        " - start_block: ", item.start_block,
                        " - blocks_count: ", item.blocks_count,
                        " - width: ", item.width,
                        " - height: ", item.height,
                        " - palette: ", item.palette_ptr ? item.palette_ptr->id() : -1,
                        " - usages: ", item.usages,
                        " - next_index: ", item.next_index,
                        (item.commit ? " - commit" : " - no_commit"));
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


    [[nodiscard]] int _blocks(int half_words)
    {
        int result = half_words / hw::bg_blocks::half_words_per_block();

        if(half_words % hw::bg_blocks::half_words_per_block())
        {
            ++result;
        }

        return result;
    }

    void _commit_item(int id, const uint16_t& data_ref, bool delay_commit)
    {
        item_type& item = data.items.item(id);
        item.data = &data_ref;
        data.items_map.insert(&data_ref, id);

        if(delay_commit)
        {
            item.commit = true;
            data.check_commit = true;
        }
        else
        {
            hw::bg_blocks::commit(data_ref, item.start_block, item.width * item.height, item.palette_offset());
        }
    }

    template<bool aligned>
    [[nodiscard]] int _create_item(int id, const uint16_t* data_ptr, int blocks_count, const size& dimensions,
                                   optional<bg_palette_ptr>&& palette_ptr)
    {
        if(aligned)
        {
            item_type& item = data.items.item(id);

            if(int extra_blocks_count = item.start_block % hw::bg_blocks::alignment())
            {
                BTN_ASSERT(! data.items.full(), "No more items allowed");

                int padding_blocks_count = hw::bg_blocks::alignment() - extra_blocks_count;
                int new_item_blocks_count = item.blocks_count - padding_blocks_count;
                item.blocks_count = uint8_t(padding_blocks_count);

                item_type new_item;
                new_item.start_block = item.start_block + item.blocks_count;
                new_item.blocks_count = uint8_t(new_item_blocks_count);

                auto new_item_iterator = data.items.insert_after(id, new_item);
                id = new_item_iterator.id();
            }
        }

        item_type& item = data.items.item(id);
        int new_item_blocks_count = item.blocks_count - blocks_count;
        item.data = data_ptr;
        item.blocks_count = uint8_t(blocks_count);
        item.palette_ptr = move(palette_ptr);
        item.width = uint16_t(dimensions.width());
        item.height = uint8_t(dimensions.height());
        item.usages = 1;
        item.set_status(item_type::status_type::USED);
        item.commit = false;
        data.free_blocks_count -= blocks_count;

        if(data_ptr)
        {
            _commit_item(id, *data_ptr, data.delay_commit);
        }

        if(new_item_blocks_count)
        {
            BTN_ASSERT(! data.items.full(), "No more items allowed");

            item_type new_item;
            new_item.start_block = item.start_block + item.blocks_count;
            new_item.blocks_count = uint8_t(new_item_blocks_count);
            data.items.insert_after(id, new_item);
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

    template<bool aligned>
    optional<int> _create_impl(const uint16_t* data_ptr, int blocks_count, const size& dimensions,
                               optional<bg_palette_ptr>&& palette_ptr)
    {
        if(! data_ptr && data.delay_commit)
        {
            return nullopt;
        }

        if(blocks_count <= data.free_blocks_count)
        {
            auto smallest_iterator = data.items.end();
            int smallest_blocks_count = numeric_limits<int>::max();

            for(auto iterator = data.items.begin(), end = data.items.end(); iterator != end; ++iterator)
            {
                const item_type& item = *iterator;

                if(item.status() == item_type::status_type::FREE)
                {
                    int required_blocks_count = blocks_count;

                    if(aligned)
                    {
                        if(int extra_blocks_count = item.start_block % hw::bg_blocks::alignment())
                        {
                            required_blocks_count += hw::bg_blocks::alignment() - extra_blocks_count;
                        }
                    }

                    if(item.blocks_count >= required_blocks_count)
                    {
                        if(item.blocks_count < smallest_blocks_count)
                        {
                            smallest_iterator = iterator;
                            smallest_blocks_count = item.blocks_count;
                        }
                    }
                }
            }

            if(smallest_iterator != data.items.end())
            {
                return _create_item<aligned>(smallest_iterator.id(), data_ptr, blocks_count, dimensions,
                                             move(palette_ptr));
            }
        }

        if(data_ptr && blocks_count <= data.to_remove_blocks_count)
        {
            update();
            data.delay_commit = true;
            return _create_impl<aligned>(data_ptr, blocks_count, dimensions, move(palette_ptr));
        }

        return nullopt;
    }
}

void init()
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - INIT");

    item_type new_item;
    new_item.blocks_count = hw::bg_blocks::count();
    data.items.init();
    data.items.push_front(new_item);
    data.free_blocks_count = new_item.blocks_count;

    BTN_BG_BLOCKS_LOG_STATUS();
}

optional<int> find(const uint16_t& data_ref, [[maybe_unused]] const size& dimensions,
                   [[maybe_unused]] const bg_palette_ptr* palette_ptr)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - FIND: ", &data_ref, " - ", dimensions.width(), " - ", dimensions.height(),
                      " - ", palette_ptr ? palette_ptr->id() : -1);

    auto items_map_iterator = data.items_map.find(&data_ref);
    optional<int> result;

    if(items_map_iterator != data.items_map.end())
    {
        auto id = int(items_map_iterator->second);
        item_type& item = data.items.item(id);
        result.emplace(id);
        BTN_ASSERT(dimensions.width() == item.width, "Width does not match item width: ",
                   dimensions.width(), " - ", item.width);
        BTN_ASSERT(dimensions.height() == item.height, "Height does not match item height: ",
                   dimensions.height(), " - ", item.height);
        BTN_ASSERT(! palette_ptr == ! item.palette_ptr || palette_ptr->id() == item.palette_ptr->id(),
                   "Palette does not match item palette");

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
    }

    if(result)
    {
        BTN_BG_BLOCKS_LOG("FOUND. start_block: ", data.items.item(*result).start_block);
    }
    else
    {
        BTN_BG_BLOCKS_LOG("NOT FOUND");
    }

    BTN_BG_BLOCKS_LOG_STATUS();

    return result;
}

optional<int> create(const uint16_t& data_ref, const size& dimensions, optional<bg_palette_ptr>&& palette_ptr,
                     bool aligned)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - CREATE: ", &data_ref, " - ", dimensions.width(), " - ",
                      dimensions.height(), " - ", palette_ptr ? palette_ptr->id() : -1, " - ", aligned);

    BTN_ASSERT(dimensions.width() > 0, "Invalid width: ", dimensions.width());
    BTN_ASSERT(dimensions.height() > 0, "Invalid height: ", dimensions.height());

    int half_words = dimensions.width() * dimensions.height();
    BTN_ASSERT(half_words > 0 && half_words < max_half_words, "Invalid data size: ", half_words);
    BTN_ASSERT(data.items_map.find(&data_ref) == data.items_map.end(), "Multiple copies of the same data not supported");

    optional<int> result;

    if(aligned)
    {
        result = _create_impl<true>(&data_ref, _blocks(half_words), dimensions, move(palette_ptr));
    }
    else
    {
        result = _create_impl<false>(&data_ref, _blocks(half_words), dimensions, move(palette_ptr));
    }

    if(result)
    {
        BTN_BG_BLOCKS_LOG("CREATED. start_block: ", data.items.item(*result).start_block);
    }
    else
    {
        BTN_BG_BLOCKS_LOG("NOT CREATED");
    }

    BTN_BG_BLOCKS_LOG_STATUS();

    return result;
}

optional<int> allocate(const size& dimensions, optional<bg_palette_ptr>&& palette_ptr, bool aligned)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - ALLOCATE: ", dimensions.width(), " - ", dimensions.height(), " - ",
                      palette_ptr ? palette_ptr->id() : -1, " - ", aligned);

    BTN_ASSERT(dimensions.width() > 0, "Invalid width: ", dimensions.width());
    BTN_ASSERT(dimensions.height() > 0, "Invalid height: ", dimensions.height());

    int half_words = dimensions.width() * dimensions.height();
    BTN_ASSERT(half_words > 0 && half_words < max_half_words, "Invalid data size: ", half_words);

    optional<int> result;

    if(aligned)
    {
        result = _create_impl<true>(nullptr, _blocks(half_words), dimensions, move(palette_ptr));
    }
    else
    {
        result = _create_impl<false>(nullptr, _blocks(half_words), dimensions, move(palette_ptr));
    }

    if(result)
    {
        BTN_BG_BLOCKS_LOG("ALLOCATED. start_block: ", data.items.item(*result).start_block);
    }
    else
    {
        BTN_BG_BLOCKS_LOG("NOT ALLOCATED");
    }

    BTN_BG_BLOCKS_LOG_STATUS();

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

int hw_id(int id, bool aligned)
{
    const item_type& item = data.items.item(id);
    int result = item.start_block;

    if(aligned)
    {
        result /= hw::bg_blocks::alignment();
    }

    return result;
}

size dimensions(int id)
{
    const item_type& item = data.items.item(id);
    return size(item.width, item.height);
}

const uint16_t* data_ref(int id)
{
    const item_type& item = data.items.item(id);
    return item.data;
}

void set_data_ref(int id, const uint16_t& data_ref, [[maybe_unused]] const size& dimensions)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - SET_DATA_REF: ", id, " - ", data.items.item(id).start_block, " - ",
                      &data_ref, " - ", dimensions.width(), " - ", dimensions.height());

    item_type& item = data.items.item(id);
    BTN_ASSERT(item.data, "Item has no data");
    BTN_ASSERT(dimensions.width() == item.width, "Width does not match item width: ",
               dimensions.width(), " - ", item.width);
    BTN_ASSERT(dimensions.height() == item.height, "Height does not match item height: ",
               dimensions.height(), " - ", item.height);

    if(item.data != &data_ref)
    {
        BTN_ASSERT(data.items_map.find(&data_ref) == data.items_map.end(),
                   "Multiple copies of the same data not supported");

        data.items_map.erase(item.data);
        _commit_item(id, data_ref, true);
    }

    BTN_BG_BLOCKS_LOG_STATUS();
}

void reload_data_ref(int id)
{
    BTN_BG_BLOCKS_LOG("bg_blocks_manager - RELOAD_DATA_REF: ", id, " - ", data.items.item(id).start_block);

    item_type& item = data.items.item(id);
    BTN_ASSERT(item.data, "Item has no data");

    item.commit = true;
    data.check_commit = true;

    BTN_BG_BLOCKS_LOG_STATUS();
}

const bg_palette_ptr* palette(int id)
{
    const item_type& item = data.items.item(id);
    return item.palette_ptr ? &item.palette_ptr.value() : nullptr;
}

void set_palette(int id, const optional<bg_palette_ptr>& palette_ptr)
{
    item_type& item = data.items.item(id);

    if(palette_ptr != item.palette_ptr)
    {
        BTN_ASSERT(! palette_ptr || palette_ptr->eight_bits_per_pixel() == item.palette_ptr->eight_bits_per_pixel(),
                   "Palette colors bpp mode mismatch: ", palette_ptr->eight_bits_per_pixel(), " - ",
                   item.palette_ptr->eight_bits_per_pixel());

        item.palette_ptr = palette_ptr;
        item.commit = true;
        data.check_commit = true;
    }
}

void set_palette(int id, optional<bg_palette_ptr>&& palette_ptr)
{
    item_type& item = data.items.item(id);

    if(palette_ptr != item.palette_ptr)
    {
        item.palette_ptr = move(palette_ptr);
        item.commit = true;
        data.check_commit = true;
    }
}

optional<span<uint16_t>> vram(int id)
{
    item_type& item = data.items.item(id);
    optional<span<uint16_t>> result;

    if(! item.data)
    {
        result.emplace(&hw::bg_blocks::vram(item.start_block), item.width * item.height);
    }

    return result;
}

void update()
{
    if(data.to_remove_blocks_count)
    {
        BTN_BG_BLOCKS_LOG("bg_blocks_manager - UPDATE");

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
                    hw::bg_blocks::commit(*item.data, item.start_block, item.width * item.height,
                                          item.palette_offset());
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
