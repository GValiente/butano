#ifndef BTN_BG_PTR_H
#define BTN_BG_PTR_H

#include "btn_utility.h"
#include "btn_fixed_fwd.h"
#include "btn_functional.h"
#include "btn_create_mode.h"
#include "btn_optional_fwd.h"

namespace btn
{

class size;
class bg_item;
class bg_builder;
class bg_map_ptr;
class bg_map_item;
class fixed_point;
class palette_item;
class bg_tiles_ptr;
class bg_tiles_item;
class bg_palette_ptr;

using bg_palette_item = palette_item;

class bg_ptr
{

public:
    [[nodiscard]] static bg_ptr create(fixed x, fixed y, const bg_item& item);

    [[nodiscard]] static bg_ptr create(const fixed_point& position, const bg_item& item);

    [[nodiscard]] static bg_ptr create(const bg_builder& builder);

    [[nodiscard]] static bg_ptr create(bg_builder&& builder);

    [[nodiscard]] static optional<bg_ptr> optional_create(fixed x, fixed y, const bg_item& item);

    [[nodiscard]] static optional<bg_ptr> optional_create(const fixed_point& position, const bg_item& item);

    [[nodiscard]] static optional<bg_ptr> optional_create(const bg_builder& builder);

    [[nodiscard]] static optional<bg_ptr> optional_create(bg_builder&& builder);

    bg_ptr(const bg_ptr& other);

    bg_ptr& operator=(const bg_ptr& other);

    bg_ptr(bg_ptr&& other) :
        bg_ptr(other._id)
    {
        other._id = -1;
    }

    bg_ptr& operator=(bg_ptr&& other)
    {
        swap(_id, other._id);
        return *this;
    }

    ~bg_ptr()
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

    void set_tiles(bg_tiles_ptr tiles_ptr);

    void set_tiles(const bg_item& item, create_mode create_mode = create_mode::FIND_OR_CREATE);

    void set_tiles(const bg_tiles_item& tiles_item, create_mode create_mode = create_mode::FIND_OR_CREATE);

    [[nodiscard]] const bg_map_ptr& map() const;

    void set_map(bg_map_ptr map_ptr);

    void set_map(const bg_item& item, create_mode create_mode = create_mode::FIND_OR_CREATE);

    void set_map(const bg_map_item& map_item, create_mode create_mode = create_mode::FIND_OR_CREATE);

    [[nodiscard]] const bg_palette_ptr& palette() const;

    void set_palette(bg_palette_ptr palette_ptr);

    void set_palette(const bg_item& item, create_mode create_mode = create_mode::FIND_OR_CREATE);

    void set_palette(const bg_palette_item& palette_item, create_mode create_mode = create_mode::FIND_OR_CREATE);

    void set_item(const bg_item& item, create_mode create_mode = create_mode::FIND_OR_CREATE);

    [[nodiscard]] fixed x() const;

    [[nodiscard]] fixed y() const;

    [[nodiscard]] const fixed_point& position() const;

    void set_x(fixed x);

    void set_y(fixed y);

    void set_position(fixed x, fixed y);

    void set_position(const fixed_point& position);

    [[nodiscard]] int priority() const;

    void set_priority(int priority);

    [[nodiscard]] bool mosaic_enabled() const;

    void set_mosaic_enabled(bool mosaic_enabled);

    [[nodiscard]] bool visible() const;

    void set_visible(bool visible);

    [[nodiscard]] bool ignore_camera() const;

    void set_ignore_camera(bool ignore_camera);

    [[nodiscard]] friend bool operator==(const bg_ptr& a, const bg_ptr& b)
    {
        return a._id == b._id;
    }

    [[nodiscard]] friend bool operator!=(const bg_ptr& a, const bg_ptr& b)
    {
        return ! (a == b);
    }

private:
    int8_t _id;

    explicit bg_ptr(int id) :
        _id(int8_t(id))
    {
    }

    void _destroy();
};


template<>
struct hash<bg_ptr>
{
    [[nodiscard]] size_t operator()(const bg_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
