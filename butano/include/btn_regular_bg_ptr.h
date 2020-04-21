#ifndef BTN_REGULAR_BG_PTR_H
#define BTN_REGULAR_BG_PTR_H

#include "btn_utility.h"
#include "btn_fixed_fwd.h"
#include "btn_functional.h"
#include "btn_optional_fwd.h"

namespace btn
{

class size;
class window;
class sprite_ptr;
class fixed_point;
class bg_tiles_ptr;
class bg_tiles_item;
class bg_palette_ptr;
class bg_palette_item;
class regular_bg_item;
class regular_bg_builder;
class regular_bg_map_ptr;
class regular_bg_map_item;
class regular_bg_attributes;

class regular_bg_ptr
{

public:
    [[nodiscard]] static regular_bg_ptr create(fixed x, fixed y, const regular_bg_item& item);

    [[nodiscard]] static regular_bg_ptr create(const fixed_point& position, const regular_bg_item& item);

    [[nodiscard]] static regular_bg_ptr create(const regular_bg_builder& builder);

    [[nodiscard]] static regular_bg_ptr create(regular_bg_builder&& builder);

    [[nodiscard]] static optional<regular_bg_ptr> optional_create(fixed x, fixed y, const regular_bg_item& item);

    [[nodiscard]] static optional<regular_bg_ptr> optional_create(const fixed_point& position,
                                                                  const regular_bg_item& item);

    [[nodiscard]] static optional<regular_bg_ptr> optional_create(const regular_bg_builder& builder);

    [[nodiscard]] static optional<regular_bg_ptr> optional_create(regular_bg_builder&& builder);

    regular_bg_ptr(const regular_bg_ptr& other);

    regular_bg_ptr& operator=(const regular_bg_ptr& other);

    regular_bg_ptr(regular_bg_ptr&& other) :
        regular_bg_ptr(other._id)
    {
        other._id = -1;
    }

    regular_bg_ptr& operator=(regular_bg_ptr&& other)
    {
        btn::swap(_id, other._id);
        return *this;
    }

    ~regular_bg_ptr()
    {
        if(_id >= 0)
        {
            _destroy();
        }
    }

    [[nodiscard]] int id() const
    {
        return _id;
    }

    [[nodiscard]] size dimensions() const;

    [[nodiscard]] const bg_tiles_ptr& tiles() const;

    void set_tiles(const bg_tiles_ptr& tiles_ptr);

    void set_tiles(bg_tiles_ptr&& tiles_ptr);

    void set_tiles(const regular_bg_item& item);

    void set_tiles(const bg_tiles_item& tiles_item);

    [[nodiscard]] const bg_palette_ptr& palette() const;

    void set_palette(const bg_palette_ptr& palette_ptr);

    void set_palette(bg_palette_ptr&& palette_ptr);

    void set_palette(const regular_bg_item& item);

    void set_palette(const bg_palette_item& palette_item);

    void set_tiles_and_palette(bg_tiles_ptr tiles_ptr, bg_palette_ptr palette_ptr);

    [[nodiscard]] const regular_bg_map_ptr& map() const;

    void set_map(const regular_bg_map_ptr& map_ptr);

    void set_map(regular_bg_map_ptr&& map_ptr);

    void set_map(const regular_bg_item& item);

    void set_map(const regular_bg_map_item& map_item);

    void set_item(const regular_bg_item& item);

    [[nodiscard]] fixed x() const;

    [[nodiscard]] fixed y() const;

    [[nodiscard]] const fixed_point& position() const;

    void set_x(fixed x);

    void set_y(fixed y);

    void set_position(fixed x, fixed y);

    void set_position(const fixed_point& position);

    [[nodiscard]] int priority() const;

    void set_priority(int priority);

    [[nodiscard]] bool above(const regular_bg_ptr& other) const;

    [[nodiscard]] bool above(const sprite_ptr& sprite_ptr) const;

    void put_above(const regular_bg_ptr& other);

    void put_above(const sprite_ptr& sprite_ptr);

    [[nodiscard]] bool mosaic_enabled() const;

    void set_mosaic_enabled(bool mosaic_enabled);

    [[nodiscard]] regular_bg_attributes attributes() const;

    void set_attributes(const regular_bg_attributes& attributes);

    [[nodiscard]] bool blending_enabled() const;

    void set_blending_enabled(bool blending_enabled);

    [[nodiscard]] bool visible() const;

    void set_visible(bool visible);

    [[nodiscard]] bool visible_in_window(const window& window) const;

    void set_visible_in_window(bool visible, window& window);

    [[nodiscard]] bool ignore_camera() const;

    void set_ignore_camera(bool ignore_camera);

    void swap(regular_bg_ptr& other)
    {
        btn::swap(_id, other._id);
    }

    friend void swap(regular_bg_ptr& a, regular_bg_ptr& b)
    {
        btn::swap(a._id, b._id);
    }

    [[nodiscard]] friend bool operator==(const regular_bg_ptr& a, const regular_bg_ptr& b)
    {
        return a._id == b._id;
    }

    [[nodiscard]] friend bool operator!=(const regular_bg_ptr& a, const regular_bg_ptr& b)
    {
        return ! (a == b);
    }

private:
    int8_t _id;

    explicit regular_bg_ptr(int id) :
        _id(int8_t(id))
    {
    }

    void _destroy();
};


template<>
struct hash<regular_bg_ptr>
{
    [[nodiscard]] unsigned operator()(const regular_bg_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
