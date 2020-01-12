#ifndef BTN_SPRITE_PTR_H
#define BTN_SPRITE_PTR_H

#include "btn_fixed_fwd.h"
#include "btn_functional.h"
#include "btn_create_mode.h"

namespace btn
{

class size;
class fixed_point;
class sprite_item;
class sprite_builder;
class sprite_tiles_ptr;
class sprite_tiles_item;
class sprite_palette_ptr;
class sprite_palette_item;

class sprite_ptr
{

public:
    [[nodiscard]] static sprite_ptr create(fixed x, fixed y, const sprite_item& item, int graphics_id = 0);

    [[nodiscard]] static sprite_ptr create(const fixed_point& position, const sprite_item& item, int graphics_id = 0);

    [[nodiscard]] static sprite_ptr create(const sprite_builder& builder);

    [[nodiscard]] static sprite_ptr create(sprite_builder&& builder);

    sprite_ptr(const sprite_ptr& other);

    sprite_ptr& operator=(const sprite_ptr& other);

    sprite_ptr(sprite_ptr&& other);

    sprite_ptr& operator=(sprite_ptr&& other);

    ~sprite_ptr()
    {
        _destroy();
    }

    [[nodiscard]] size dimensions() const;

    [[nodiscard]] const sprite_tiles_ptr& tiles() const;

    void set_tiles(sprite_tiles_ptr tiles_ptr);

    void set_tiles(const sprite_item& item, int graphics_id = 0, create_mode create_mode = create_mode::FIND_OR_CREATE);

    void set_tiles(const sprite_tiles_item& tiles_item, int graphics_id = 0,
                   create_mode create_mode = create_mode::FIND_OR_CREATE);

    [[nodiscard]] const sprite_palette_ptr& palette() const;

    void set_palette(sprite_palette_ptr palette_ptr);

    void set_palette(const sprite_item& item, create_mode create_mode = create_mode::FIND_OR_CREATE);

    void set_palette(const sprite_palette_item& palette_item, create_mode create_mode = create_mode::FIND_OR_CREATE);

    void set_tiles_and_palette(const sprite_item& item, int graphics_id = 0,
                               create_mode create_mode = create_mode::FIND_OR_CREATE);

    [[nodiscard]] fixed x() const;

    [[nodiscard]] fixed y() const;

    [[nodiscard]] const fixed_point& position() const;

    void set_x(fixed x);

    void set_y(fixed y);

    void set_position(fixed x, fixed y);

    void set_position(const fixed_point& position);

    [[nodiscard]] int bg_priority() const;

    void set_bg_priority(int bg_priority);

    [[nodiscard]] int z_order() const;

    void set_z_order(int z_order);

    [[nodiscard]] bool horizontal_flip() const;

    void set_horizontal_flip(bool horizontal_flip);

    [[nodiscard]] bool vertical_flip() const;

    void set_vertical_flip(bool vertical_flip);

    [[nodiscard]] bool mosaic_enabled() const;

    void set_mosaic_enabled(bool mosaic_enabled);

    [[nodiscard]] bool visible() const;

    void set_visible(bool visible);

    [[nodiscard]] bool ignore_camera() const;

    void set_ignore_camera(bool ignore_camera);

    [[nodiscard]] size_t hash() const
    {
        return make_hash(_id);
    }

    [[nodiscard]] friend bool operator==(const sprite_ptr& a, const sprite_ptr& b)
    {
        return a._id == b._id;
    }

    [[nodiscard]] friend bool operator!=(const sprite_ptr& a, const sprite_ptr& b)
    {
        return ! (a == b);
    }

private:
    using id_type = void*;

    id_type _id;

    explicit sprite_ptr(id_type id) :
        _id(id)
    {
    }

    void _destroy();
};


template<>
struct hash<sprite_ptr>
{
    [[nodiscard]] size_t operator()(const sprite_ptr& value) const
    {
        return value.hash();
    }
};

}

#endif
