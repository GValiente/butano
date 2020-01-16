#ifndef BTN_SPRITE_TILES_PTR_H
#define BTN_SPRITE_TILES_PTR_H

#include "btn_span_fwd.h"
#include "btn_functional.h"
#include "btn_optional_fwd.h"
#include "_btn_sprite_tiles_manager_item.h"

namespace btn
{

class sprite_tiles_ptr
{

public:
    [[nodiscard]] static optional<sprite_tiles_ptr> find(const span<const tile>& tiles_ref);

    [[nodiscard]] static sprite_tiles_ptr create(const span<const tile>& tiles_ref);

    [[nodiscard]] static optional<sprite_tiles_ptr> optional_create(const span<const tile>& tiles_ref);

    [[nodiscard]] static sprite_tiles_ptr find_or_create(const span<const tile>& tiles_ref);

    [[nodiscard]] static optional<sprite_tiles_ptr> optional_find_or_create(const span<const tile>& tiles_ref);

    [[nodiscard]] static sprite_tiles_ptr allocate(int tiles);

    [[nodiscard]] static optional<sprite_tiles_ptr> optional_allocate(int tiles);

    sprite_tiles_ptr(const sprite_tiles_ptr& other);

    sprite_tiles_ptr& operator=(const sprite_tiles_ptr& other);

    sprite_tiles_ptr(sprite_tiles_ptr&& other);

    sprite_tiles_ptr& operator=(sprite_tiles_ptr&& other);

    ~sprite_tiles_ptr()
    {
        _destroy();
    }

    [[nodiscard]] int id() const
    {
        return _iterator->start_tile;
    }

    [[nodiscard]] optional<span<const tile>> tiles_ref() const;

    void set_tiles_ref(const span<const tile>& tiles_ref);

    void reload_tiles_ref();

    [[nodiscard]] optional<span<tile>> vram();

    [[nodiscard]] int tiles_count() const
    {
        return _iterator->tiles_count;
    }

    [[nodiscard]] friend bool operator==(const sprite_tiles_ptr& a, const sprite_tiles_ptr& b)
    {
        return a._iterator == b._iterator;
    }

    [[nodiscard]] friend bool operator!=(const sprite_tiles_ptr& a, const sprite_tiles_ptr& b)
    {
        return ! (a == b);
    }

private:
    using item_type = _btn::sprite_tiles_manager_item;

    item_type::list_iterator _iterator;

    explicit sprite_tiles_ptr(item_type::list_iterator iterator) :
        _iterator(iterator)
    {
    }

    void _destroy();
};


template<>
struct hash<sprite_tiles_ptr>
{
    [[nodiscard]] size_t operator()(const sprite_tiles_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
