#ifndef BTN_SPRITE_PTR_H
#define BTN_SPRITE_PTR_H

#include "btn_utility.h"
#include "btn_fixed_fwd.h"
#include "btn_functional.h"
#include "btn_create_mode.h"
#include "btn_optional_fwd.h"

namespace btn
{

class size;
class bg_ptr;
class fixed_point;
class sprite_item;
class sprite_builder;
class sprite_tiles_ptr;
class sprite_tiles_item;
class sprite_palette_ptr;
class sprite_palette_item;
class sprite_affine_mat_ptr;
enum class sprite_double_size_mode;

class sprite_ptr
{

public:
    [[nodiscard]] static sprite_ptr create(fixed x, fixed y, const sprite_item& item, int graphics_index = 0);

    [[nodiscard]] static sprite_ptr create(const fixed_point& position, const sprite_item& item,
                                           int graphics_index = 0);

    [[nodiscard]] static sprite_ptr create(const sprite_builder& builder);

    [[nodiscard]] static sprite_ptr create(sprite_builder&& builder);

    [[nodiscard]] static optional<sprite_ptr> optional_create(fixed x, fixed y, const sprite_item& item,
                                                              int graphics_index = 0);

    [[nodiscard]] static optional<sprite_ptr> optional_create(const fixed_point& position, const sprite_item& item,
                                                              int graphics_index = 0);

    [[nodiscard]] static optional<sprite_ptr> optional_create(const sprite_builder& builder);

    [[nodiscard]] static optional<sprite_ptr> optional_create(sprite_builder&& builder);

    sprite_ptr(const sprite_ptr& other);

    sprite_ptr& operator=(const sprite_ptr& other);

    sprite_ptr(sprite_ptr&& other) :
        sprite_ptr(other._handle)
    {
        other._handle = nullptr;
    }

    sprite_ptr& operator=(sprite_ptr&& other)
    {
        swap(_handle, other._handle);
        return *this;
    }

    ~sprite_ptr()
    {
        if(_handle)
        {
            _destroy();
        }
    }

    [[nodiscard]] size dimensions() const;

    [[nodiscard]] const sprite_tiles_ptr& tiles() const;

    void set_tiles(const sprite_tiles_ptr& tiles_ptr);

    void set_tiles(sprite_tiles_ptr&& tiles_ptr);

    void set_tiles(const sprite_item& item, int graphics_index = 0,
                   create_mode create_mode = create_mode::FIND_OR_CREATE);

    void set_tiles(const sprite_tiles_item& tiles_item, int graphics_index = 0,
                   create_mode create_mode = create_mode::FIND_OR_CREATE);

    [[nodiscard]] const sprite_palette_ptr& palette() const;

    void set_palette(const sprite_palette_ptr& palette_ptr);

    void set_palette(sprite_palette_ptr&& palette_ptr);

    void set_palette(const sprite_item& item, create_mode create_mode = create_mode::FIND_OR_CREATE);

    void set_palette(const sprite_palette_item& palette_item, create_mode create_mode = create_mode::FIND_OR_CREATE);

    void set_item(const sprite_item& item, int graphics_index = 0,
                  create_mode create_mode = create_mode::FIND_OR_CREATE);

    [[nodiscard]] fixed x() const;

    [[nodiscard]] fixed y() const;

    [[nodiscard]] const fixed_point& position() const;

    void set_x(fixed x);

    void set_y(fixed y);

    void set_position(fixed x, fixed y);

    void set_position(const fixed_point& position);

    [[nodiscard]] fixed rotation_angle() const;

    void set_rotation_angle(fixed rotation_angle);

    [[nodiscard]] fixed scale_x() const;

    void set_scale_x(fixed scale_x);

    [[nodiscard]] fixed scale_y() const;

    void set_scale_y(fixed scale_y);

    [[nodiscard]] int bg_priority() const;

    void set_bg_priority(int bg_priority);

    [[nodiscard]] int z_order() const;

    void set_z_order(int z_order);

    void put_above(const sprite_ptr& other);

    void put_above(const bg_ptr& bg_ptr);

    [[nodiscard]] bool horizontal_flip() const;

    void set_horizontal_flip(bool horizontal_flip);

    [[nodiscard]] bool vertical_flip() const;

    void set_vertical_flip(bool vertical_flip);

    [[nodiscard]] bool mosaic_enabled() const;

    void set_mosaic_enabled(bool mosaic_enabled);

    [[nodiscard]] bool double_size() const;

    [[nodiscard]] sprite_double_size_mode double_size_mode() const;

    void set_double_size_mode(sprite_double_size_mode double_size_mode);

    [[nodiscard]] bool visible() const;

    void set_visible(bool visible);

    [[nodiscard]] bool ignore_camera() const;

    void set_ignore_camera(bool ignore_camera);

    [[nodiscard]] const optional<sprite_affine_mat_ptr>& affine_mat() const;

    void set_affine_mat(const optional<sprite_affine_mat_ptr>& affine_mat_ptr);

    void set_affine_mat(optional<sprite_affine_mat_ptr>&& affine_mat_ptr);

    [[nodiscard]] bool remove_affine_mat_when_not_needed() const;

    void set_remove_affine_mat_when_not_needed(bool remove_when_not_needed);

    [[nodiscard]] size_t hash() const
    {
        return make_hash(_handle);
    }

    [[nodiscard]] friend bool operator==(const sprite_ptr& a, const sprite_ptr& b)
    {
        return a._handle == b._handle;
    }

    [[nodiscard]] friend bool operator!=(const sprite_ptr& a, const sprite_ptr& b)
    {
        return ! (a == b);
    }

private:
    using handle_type = void*;

    handle_type _handle;

    explicit sprite_ptr(handle_type handle) :
        _handle(handle)
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
