#ifndef BTN_SPRITE_TILES_PTR_H
#define BTN_SPRITE_TILES_PTR_H

#include "btn_span_fwd.h"
#include "btn_functional.h"
#include "btn_optional_fwd.h"

namespace btn
{

class tile;

class sprite_tiles_ptr
{

public:
    [[nodiscard]] static optional<sprite_tiles_ptr> find(const span<const tile>& tiles_ref);

    [[nodiscard]] static sprite_tiles_ptr create(const span<const tile>& tiles_ref);

    [[nodiscard]] static sprite_tiles_ptr find_or_create(const span<const tile>& tiles_ref);

    [[nodiscard]] static sprite_tiles_ptr allocate(int tiles);

    [[nodiscard]] static optional<sprite_tiles_ptr> optional_create(const span<const tile>& tiles_ref);

    [[nodiscard]] static optional<sprite_tiles_ptr> optional_find_or_create(const span<const tile>& tiles_ref);

    [[nodiscard]] static optional<sprite_tiles_ptr> optional_allocate(int tiles);

    sprite_tiles_ptr(const sprite_tiles_ptr& other);

    sprite_tiles_ptr& operator=(const sprite_tiles_ptr& other);

    sprite_tiles_ptr(sprite_tiles_ptr&& other);

    sprite_tiles_ptr& operator=(sprite_tiles_ptr&& other);

    ~sprite_tiles_ptr()
    {
        _destroy();
    }

    [[nodiscard]] int id() const;

    [[nodiscard]] int tiles_count() const;

    [[nodiscard]] optional<span<const tile>> tiles_ref() const;

    void set_tiles_ref(const span<const tile>& tiles_ref);

    void reload_tiles_ref();

    [[nodiscard]] optional<span<tile>> vram();

    [[nodiscard]] size_t hash() const
    {
        return make_hash(_handle);
    }

    [[nodiscard]] friend bool operator==(const sprite_tiles_ptr& a, const sprite_tiles_ptr& b)
    {
        return a._handle == b._handle;
    }

    [[nodiscard]] friend bool operator!=(const sprite_tiles_ptr& a, const sprite_tiles_ptr& b)
    {
        return ! (a == b);
    }

private:
    int16_t _handle;

    explicit sprite_tiles_ptr(int handle) :
        _handle(int16_t(handle))
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
