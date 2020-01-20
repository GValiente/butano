#ifndef BTN_SPRITE_TILES_BANK_H
#define BTN_SPRITE_TILES_BANK_H

#include "btn_vector.h"
#include "btn_span_fwd.h"
#include "btn_optional_fwd.h"
#include "btn_hash_map.h"
#include "btn_config_sprite_tiles.h"

namespace btn
{

class tile;

class sprite_tiles_bank
{

public:
    static constexpr const size_t max_items = BTN_CFG_SPRITE_TILES_MAX_BANK_ITEMS;

    enum class type
    {
        HIGH,
        LOW
    };

    void init(type bank_type);

    [[nodiscard]] optional<int> find(const span<const tile>& tiles_ref);

    [[nodiscard]] optional<int> create(const span<const tile>& tiles_ref);

    [[nodiscard]] optional<int> allocate(int tiles);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] int start_tile(int id) const
    {
        return _items.item(id).start_tile;
    }

    [[nodiscard]] int tiles_count(int id) const
    {
        return _items.item(id).tiles_count;
    }

    [[nodiscard]] optional<span<const tile>> tiles_ref(int id) const;

    void set_tiles_ref(int id, const span<const tile>& tiles_ref);

    void reload_tiles_ref(int id);

    [[nodiscard]] optional<span<tile>> vram(int id);

    [[nodiscard]] bool update();

    [[nodiscard]] bool commit();

    #if BTN_CFG_SPRITE_TILES_LOG_ENABLED
        void log();
    #endif

private:
    static constexpr const int _max_list_items = max_items + 1;

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
        uint16_t next_index = _max_list_items;
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
                _free_indices[index] = int16_t(index + 1);
            }
        }

        [[nodiscard]] int size() const
        {
            return int(_free_indices.size());
        }

        [[nodiscard]] bool full() const
        {
            return _free_indices.empty();
        }

        [[nodiscard]] const item_type& item(int index) const
        {
            return _items[index];
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
            return iterator(_max_list_items, *this);
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
        item_type _items[_max_list_items];
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

    items_list _items;
    hash_map<const tile*, uint16_t, max_items * 2> _items_map;
    items_list::iterator _biggest_free_iterator;
    int _free_tiles_count = 0;
    int _to_remove_tiles_count = 0;
    bool _check_commit = false;
    bool _delay_commit = false;

    [[nodiscard]] optional<int> _create_impl(const tile* tiles_data, int tiles_count);

    void _create_item(int id, const tile* tiles_data, int tiles_count);

    void _commit_item(int id, const tile& tiles_data, bool delay_commit);

    [[nodiscard]] bool _remove_adjacent_item(int adjacent_id, item_type& current_item);
};

}

#endif
