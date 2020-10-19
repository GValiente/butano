#ifndef BTN_BG_TILES_PTR_H
#define BTN_BG_TILES_PTR_H

#include "btn_utility.h"
#include "btn_span_fwd.h"
#include "btn_functional.h"
#include "btn_optional_fwd.h"

namespace btn
{

class tile;
class bg_tiles_item;
enum class palette_bpp_mode;

class bg_tiles_ptr
{

public:
    [[nodiscard]] static optional<bg_tiles_ptr> find(const span<const tile>& tiles_ref);

    [[nodiscard]] static optional<bg_tiles_ptr> find(const bg_tiles_item& tiles_item);

    [[nodiscard]] static bg_tiles_ptr create(const span<const tile>& tiles_ref);

    [[nodiscard]] static bg_tiles_ptr create(const bg_tiles_item& tiles_item);

    [[nodiscard]] static bg_tiles_ptr find_or_create(const span<const tile>& tiles_ref);

    [[nodiscard]] static bg_tiles_ptr find_or_create(const bg_tiles_item& tiles_item);

    [[nodiscard]] static bg_tiles_ptr allocate(int tiles_count);

    [[nodiscard]] static optional<bg_tiles_ptr> create_optional(const span<const tile>& tiles_ref);

    [[nodiscard]] static optional<bg_tiles_ptr> create_optional(const bg_tiles_item& tiles_item);

    [[nodiscard]] static optional<bg_tiles_ptr> find_or_create_optional(const span<const tile>& tiles_ref);

    [[nodiscard]] static optional<bg_tiles_ptr> find_or_create_optional(const bg_tiles_item& tiles_item);

    [[nodiscard]] static optional<bg_tiles_ptr> allocate_optional(int tiles_count);

    bg_tiles_ptr(const bg_tiles_ptr& other);

    bg_tiles_ptr& operator=(const bg_tiles_ptr& other);

    bg_tiles_ptr(bg_tiles_ptr&& other) noexcept :
        bg_tiles_ptr(other._handle)
    {
        other._handle = -1;
    }

    bg_tiles_ptr& operator=(bg_tiles_ptr&& other) noexcept
    {
        btn::swap(_handle, other._handle);
        return *this;
    }

    ~bg_tiles_ptr()
    {
        if(_handle >= 0)
        {
            _destroy();
        }
    }

    [[nodiscard]] int id() const;

    [[nodiscard]] int cbb() const;

    [[nodiscard]] int tiles_count() const;

    [[nodiscard]] bool valid_tiles_count(palette_bpp_mode bpp_mode) const;

    [[nodiscard]] optional<span<const tile>> tiles_ref() const;

    void set_tiles_ref(const span<const tile>& tiles_ref);

    void set_tiles_ref(const bg_tiles_item& tiles_item);

    void reload_tiles_ref();

    [[nodiscard]] optional<span<tile>> vram();

    [[nodiscard]] unsigned hash() const
    {
        return make_hash(_handle);
    }

    void swap(bg_tiles_ptr& other)
    {
        btn::swap(_handle, other._handle);
    }

    friend void swap(bg_tiles_ptr& a, bg_tiles_ptr& b)
    {
        btn::swap(a._handle, b._handle);
    }

    [[nodiscard]] friend bool operator==(const bg_tiles_ptr& a, const bg_tiles_ptr& b) = default;

private:
    int8_t _handle;

    explicit bg_tiles_ptr(int handle) :
        _handle(int8_t(handle))
    {
    }

    void _destroy();
};


template<>
struct hash<bg_tiles_ptr>
{
    [[nodiscard]] unsigned operator()(const bg_tiles_ptr& value) const
    {
        return value.hash();
    }
};

}

#endif
