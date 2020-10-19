#ifndef BTN_SPRITE_TILES_PTR_H
#define BTN_SPRITE_TILES_PTR_H

#include "btn_utility.h"
#include "btn_span_fwd.h"
#include "btn_functional.h"
#include "btn_optional_fwd.h"

namespace btn
{

class tile;
class sprite_tiles_item;

class sprite_tiles_ptr
{

public:
    [[nodiscard]] static optional<sprite_tiles_ptr> find(const span<const tile>& tiles_ref);

    [[nodiscard]] static optional<sprite_tiles_ptr> find(const sprite_tiles_item& tiles_item);

    [[nodiscard]] static optional<sprite_tiles_ptr> find(const sprite_tiles_item& tiles_item, int graphics_index);

    [[nodiscard]] static sprite_tiles_ptr create(const span<const tile>& tiles_ref);

    [[nodiscard]] static sprite_tiles_ptr create(const sprite_tiles_item& tiles_item);

    [[nodiscard]] static sprite_tiles_ptr create(const sprite_tiles_item& tiles_item, int graphics_index);

    [[nodiscard]] static sprite_tiles_ptr find_or_create(const span<const tile>& tiles_ref);

    [[nodiscard]] static sprite_tiles_ptr find_or_create(const sprite_tiles_item& tiles_item);

    [[nodiscard]] static sprite_tiles_ptr find_or_create(const sprite_tiles_item& tiles_item, int graphics_index);

    [[nodiscard]] static sprite_tiles_ptr allocate(int tiles_count);

    [[nodiscard]] static optional<sprite_tiles_ptr> create_optional(const span<const tile>& tiles_ref);

    [[nodiscard]] static optional<sprite_tiles_ptr> create_optional(const sprite_tiles_item& tiles_item);

    [[nodiscard]] static optional<sprite_tiles_ptr> create_optional(const sprite_tiles_item& tiles_item,
                                                                    int graphics_index);

    [[nodiscard]] static optional<sprite_tiles_ptr> find_or_create_optional(const span<const tile>& tiles_ref);

    [[nodiscard]] static optional<sprite_tiles_ptr> find_or_create_optional(const sprite_tiles_item& tiles_item);

    [[nodiscard]] static optional<sprite_tiles_ptr> find_or_create_optional(const sprite_tiles_item& tiles_item,
                                                                            int graphics_index);

    [[nodiscard]] static optional<sprite_tiles_ptr> allocate_optional(int tiles_count);

    sprite_tiles_ptr(const sprite_tiles_ptr& other);

    sprite_tiles_ptr& operator=(const sprite_tiles_ptr& other);

    sprite_tiles_ptr(sprite_tiles_ptr&& other) noexcept :
        sprite_tiles_ptr(other._handle)
    {
        other._handle = -1;
    }

    sprite_tiles_ptr& operator=(sprite_tiles_ptr&& other) noexcept
    {
        btn::swap(_handle, other._handle);
        return *this;
    }

    ~sprite_tiles_ptr()
    {
        if(_handle >= 0)
        {
            _destroy();
        }
    }

    [[nodiscard]] int id() const;

    [[nodiscard]] int tiles_count() const;

    [[nodiscard]] optional<span<const tile>> tiles_ref() const;

    void set_tiles_ref(const span<const tile>& tiles_ref);

    void set_tiles_ref(const sprite_tiles_item& tiles_item);

    void set_tiles_ref(const sprite_tiles_item& tiles_item, int graphics_index);

    void reload_tiles_ref();

    [[nodiscard]] optional<span<tile>> vram();

    [[nodiscard]] unsigned hash() const
    {
        return make_hash(_handle);
    }

    void swap(sprite_tiles_ptr& other)
    {
        btn::swap(_handle, other._handle);
    }

    friend void swap(sprite_tiles_ptr& a, sprite_tiles_ptr& b)
    {
        btn::swap(a._handle, b._handle);
    }

    [[nodiscard]] friend bool operator==(const sprite_tiles_ptr& a, const sprite_tiles_ptr& b) = default;

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
    [[nodiscard]] unsigned operator()(const sprite_tiles_ptr& value) const
    {
        return value.hash();
    }
};

}

#endif
