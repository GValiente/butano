#include "btn_bg_blocks_manager.h"

#include "btn_span.h"
#include "btn_vector.h"
#include "btn_bg_block.h"
#include "btn_optional.h"
#include "btn_hash_map.h"
#include "../hw/include/btn_hw_bg_blocks.h"

namespace btn::bg_blocks_manager
{

namespace
{
    constexpr const size_t max_items = hw::bg_blocks::count();
    constexpr const size_t max_list_items = max_items + 1;


    class item_type
    {

    public:
        enum class status_type
        {
            FREE,
            USED,
            TO_REMOVE
        };

        const bg_block* data = nullptr;
        unsigned usages = 0;
        uint8_t start_block = 0;
        uint8_t blocks_count = 0;
        uint8_t next_index = max_list_items;
        unsigned commit: 1;

    private:
        unsigned _status: 2;

    public:
        item_type()
        {
            commit = false;
            _status = uint8_t(status_type::FREE);
        }

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
            _items[0].next_index = 1;
            _free_indices.resize(max_items);

            for(size_t index = 0; index < max_items; ++index)
            {
                _free_indices[index] = int8_t(index);
            }
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
        hash_map<const bg_block*, uint16_t, max_items * 2> items_map;
        int free_blocks_count = 0;
        int to_remove_blocks_count = 0;
        bool check_commit = false;
        bool delay_commit = false;
    };

    BTN_DATA_EWRAM static_data data;

    void _commit_item(int id, const bg_block& blocks_data, bool delay_commit)
    {
        item_type& item = data.items.item(id);
        item.data = &blocks_data;
        data.items_map.insert(&blocks_data, id);

        if(delay_commit)
        {
            item.commit = true;
            data.check_commit = true;
        }
        else
        {
            hw::bg_blocks::commit(blocks_data, item.start_block, item.blocks_count);
        }
    }

    template<bool aligned>
    [[nodiscard]] int _create_item(int id, const bg_block* blocks_data, int blocks_count)
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
        item.data = blocks_data;
        item.blocks_count = uint8_t(blocks_count);
        item.usages = 1;
        item.set_status(item_type::status_type::USED);
        item.commit = false;
        data.free_blocks_count -= blocks_count;

        if(blocks_data)
        {
            _commit_item(id, *blocks_data, data.delay_commit);
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
    optional<int> _create_impl(const bg_block* blocks_data, int blocks_count)
    {
        if(! blocks_data && data.delay_commit)
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

                    if(item.blocks_count <= required_blocks_count)
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
                return _create_item<aligned>(smallest_iterator.id(), blocks_data, blocks_count);
            }
        }

        if(blocks_data && blocks_count <= data.to_remove_blocks_count)
        {
            update();
            data.delay_commit = true;
            return _create_impl<aligned>(blocks_data, blocks_count);
        }

        return nullopt;
    }
}

void init()
{
    item_type new_item;
    new_item.blocks_count = hw::bg_blocks::count();
    data.items.init();
    data.items.push_front(new_item);
    data.free_blocks_count = new_item.blocks_count;
}

optional<int> find(const span<const bg_block>& blocks_ref)
{
    BTN_ASSERT(blocks_ref.size() > 0 && blocks_ref.size() < max_items, "Invalid blocks ref size: ", blocks_ref.size());

    auto items_map_iterator = data.items_map.find(blocks_ref.data());
    optional<int> result;

    if(items_map_iterator != data.items_map.end())
    {
        auto id = int(items_map_iterator->second);
        item_type& item = data.items.item(id);
        result.emplace(id);

        BTN_ASSERT(int(blocks_ref.size()) == item.blocks_count, "Blocks count does not match item blocks count: ",
                   blocks_ref.size(), " - ", item.blocks_count);

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

    return result;
}

optional<int> create(const span<const bg_block>& blocks_ref, bool aligned)
{
    BTN_ASSERT(blocks_ref.size() > 0 && blocks_ref.size() < max_items, "Invalid blocks ref size: ", blocks_ref.size());
    BTN_ASSERT(data.items_map.find(blocks_ref.data()) == data.items_map.end(),
               "Multiple copies of the same blocks data not supported");

    if(aligned)
    {
        return _create_impl<true>(blocks_ref.data(), int(blocks_ref.size()));
    }
    else
    {
        return _create_impl<false>(blocks_ref.data(), int(blocks_ref.size()));
    }
}

optional<int> allocate(int blocks, bool aligned)
{
    BTN_ASSERT(blocks > 0 && blocks < int(max_items), "Invalid blocks: ", blocks);

    if(aligned)
    {
        return _create_impl<true>(nullptr, blocks);
    }
    else
    {
        return _create_impl<false>(nullptr, blocks);
    }
}

void increase_usages(int id)
{
    item_type& item = data.items.item(id);
    ++item.usages;
}

void decrease_usages(int id)
{
    item_type& item = data.items.item(id);
    --item.usages;

    if(! item.usages)
    {
        item.set_status(item_type::status_type::TO_REMOVE);
        data.to_remove_blocks_count += item.blocks_count;
    }
}

int start_block(int id)
{
    item_type& item = data.items.item(id);
    return item.start_block;
}

int blocks_count(int id)
{
    item_type& item = data.items.item(id);
    return item.blocks_count;
}

optional<span<const bg_block>> blocks_ref(int id)
{
    const item_type& item = data.items.item(id);
    optional<span<const bg_block>> result;

    if(item.data)
    {
        result.emplace(item.data, item.blocks_count);
    }

    return result;
}

void set_blocks_ref(int id, const span<const bg_block>& blocks_ref)
{
    BTN_ASSERT(blocks_ref.size() > 0 && blocks_ref.size() < max_items, "Invalid blocks ref size: ", blocks_ref.size());

    item_type& item = data.items.item(id);
    BTN_ASSERT(item.data, "Blocks item has no data");
    BTN_ASSERT(int(blocks_ref.size()) == item.blocks_count, "Blocks count does not match item blocks count: ",
               blocks_ref.size(), " - ", item.blocks_count);

    const bg_block* blocks_data = blocks_ref.data();

    if(item.data != blocks_data)
    {
        BTN_ASSERT(data.items_map.find(blocks_data) == data.items_map.end(),
                   "Multiple copies of the same blocks data not supported");

        data.items_map.erase(item.data);
        _commit_item(id, *blocks_data, true);
    }
}

void reload_blocks_ref(int id)
{
    item_type& item = data.items.item(id);
    BTN_ASSERT(item.data, "Blocks item has no data");

    item.commit = true;
    data.check_commit = true;
}

optional<span<bg_block>> vram(int id)
{
    item_type& item = data.items.item(id);
    optional<span<bg_block>> result;

    if(! item.data)
    {
        result.emplace(&hw::bg_blocks::vram(item.start_block), item.blocks_count);
    }

    return result;
}

void update()
{
    if(data.to_remove_blocks_count)
    {
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
    }
}

void commit()
{
    if(data.check_commit)
    {
        data.check_commit = false;

        for(item_type& item : data.items)
        {
            if(item.commit)
            {
                item.commit = false;

                if(item.status() == item_type::status_type::USED)
                {
                    hw::bg_blocks::commit(*item.data, item.start_block, item.blocks_count);
                }
            }
        }
    }

    data.delay_commit = false;
}

}
