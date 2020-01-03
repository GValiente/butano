#ifndef BTN_SPRITE_TILES_BANK_H
#define BTN_SPRITE_TILES_BANK_H

#include "btn_span_fwd.h"
#include "btn_optional_fwd.h"
#include "btn_hash_map.h"
#include "btn_config_sprite_tiles.h"
#include "_btn_sprite_tiles_manager_item.h"

namespace btn
{

class sprite_tiles_bank
{

public:
    using item_type = _btn::sprite_tiles_manager_item;

    void init(item_type::bank_type bank_type);

    optional<item_type::list_iterator> find(const span<const tile>& tiles_ref);

    optional<item_type::list_iterator> create(const span<const tile>& tiles_ref);

    optional<item_type::list_iterator> allocate(int tiles);

    item_type::list_iterator invalid_iterator()
    {
        return _items.end();
    }

    void increase_usages(item_type::list_iterator iterator);

    void decrease_usages(item_type::list_iterator iterator);

    void reload_tiles_ref(item_type::list_iterator iterator);

    void set_tiles_ref(item_type::list_iterator iterator, const span<const tile>& tiles_ref);

    bool update();

    bool commit();

    #if BTN_CFG_SPRITE_TILES_LOG_ENABLED
        void log() const;
    #endif

private:
    static constexpr const size_t _max_items = BTN_CFG_SPRITE_TILES_MAX_BANK_ITEMS;

    forward_list<item_type, _max_items> _items;
    hash_map<const tile*, item_type::list_iterator, _max_items * 2> _items_map;
    item_type::list_iterator _biggest_free_iterator;
    item_type::bank_type _bank_type = item_type::bank_type::LOW;
    int _free_tiles_count = 0;
    int _to_remove_tiles_count = 0;
    bool _check_commit = false;
    bool _delay_commit = false;

    optional<item_type::list_iterator> _create_impl(const tile* tiles_data, int tiles_count);

    void _create_item(item_type::list_iterator iterator, const tile* tiles_data, int tiles_count);

    void _commit_item(item_type::list_iterator iterator, const tile* tiles_data, bool delay_commit);

    bool _remove_adjacent_item(item_type::list_iterator adjacent_iterator, item_type& current_item);
};

}

#endif
