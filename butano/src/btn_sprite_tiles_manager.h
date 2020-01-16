#ifndef BTN_SPRITE_TILES_MANAGER_H
#define BTN_SPRITE_TILES_MANAGER_H

#include "btn_span_fwd.h"
#include "btn_optional_fwd.h"
#include "_btn_sprite_tiles_manager_item.h"

namespace btn::sprite_tiles_manager
{
    using item_type = _btn::sprite_tiles_manager_item;

    void init();

    [[nodiscard]] optional<item_type::list_iterator> find(const span<const tile>& tiles_ref);

    [[nodiscard]] optional<item_type::list_iterator> create(const span<const tile>& tiles_ref);

    [[nodiscard]] optional<item_type::list_iterator> allocate(int tiles);

    [[nodiscard]] item_type::list_iterator invalid_iterator();

    void increase_usages(item_type::list_iterator iterator);

    void decrease_usages(item_type::list_iterator iterator);

    void set_tiles_ref(item_type::list_iterator iterator, const span<const tile>& tiles_ref);

    void reload_tiles_ref(item_type::list_iterator iterator);

    void update();

    void commit();
}

#endif
