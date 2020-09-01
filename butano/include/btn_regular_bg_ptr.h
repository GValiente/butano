#ifndef BTN_REGULAR_BG_PTR_H
#define BTN_REGULAR_BG_PTR_H

#include "btn_utility.h"
#include "btn_fixed_fwd.h"
#include "btn_functional.h"
#include "btn_optional_fwd.h"
#include "btn_config_camera.h"

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

    [[nodiscard]] static optional<regular_bg_ptr> create_optional(fixed x, fixed y, const regular_bg_item& item);

    [[nodiscard]] static optional<regular_bg_ptr> create_optional(const fixed_point& position,
                                                                  const regular_bg_item& item);

    [[nodiscard]] static optional<regular_bg_ptr> create_optional(const regular_bg_builder& builder);

    [[nodiscard]] static optional<regular_bg_ptr> create_optional(regular_bg_builder&& builder);

    regular_bg_ptr(const regular_bg_ptr& other);

    regular_bg_ptr& operator=(const regular_bg_ptr& other);

    regular_bg_ptr(regular_bg_ptr&& other) noexcept :
        regular_bg_ptr(other._handle)
    {
        other._handle = nullptr;
    }

    regular_bg_ptr& operator=(regular_bg_ptr&& other) noexcept
    {
        btn::swap(_handle, other._handle);
        return *this;
    }

    ~regular_bg_ptr()
    {
        if(_handle)
        {
            _destroy();
        }
    }

    [[nodiscard]] size dimensions() const;

    [[nodiscard]] const bg_tiles_ptr& tiles() const;

    void set_tiles(const bg_tiles_ptr& tiles);

    void set_tiles(bg_tiles_ptr&& tiles);

    void set_tiles(const bg_tiles_item& tiles_item);

    [[nodiscard]] const bg_palette_ptr& palette() const;

    void set_palette(const bg_palette_ptr& palette);

    void set_palette(bg_palette_ptr&& palette);

    void set_palette(const bg_palette_item& palette_item);

    void set_tiles_and_palette(bg_tiles_ptr tiles, bg_palette_ptr palette);

    void set_tiles_and_palette(const bg_tiles_item& tiles_item, const bg_palette_item& palette_item);

    [[nodiscard]] const regular_bg_map_ptr& map() const;

    void set_map(const regular_bg_map_ptr& map);

    void set_map(regular_bg_map_ptr&& map);

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

    [[nodiscard]] int z_order() const;

    void set_z_order(int z_order);

    [[nodiscard]] bool above(const regular_bg_ptr& other) const;

    [[nodiscard]] bool above(const sprite_ptr& sprite_ptr) const;

    void put_above(const regular_bg_ptr& other);

    void put_above(const sprite_ptr& sprite_ptr);

    [[nodiscard]] bool mosaic_enabled() const;

    void set_mosaic_enabled(bool mosaic_enabled);

    [[nodiscard]] bool blending_enabled() const;

    void set_blending_enabled(bool blending_enabled);

    [[nodiscard]] bool visible() const;

    void set_visible(bool visible);

    [[nodiscard]] bool visible_in_window(const window& window) const;

    void set_visible_in_window(bool visible, window& window);

    #if BTN_CFG_CAMERA_ENABLED
        [[nodiscard]] bool ignore_camera() const;

        void set_ignore_camera(bool ignore_camera);
    #endif

    [[nodiscard]] regular_bg_attributes attributes() const;

    void set_attributes(const regular_bg_attributes& attributes);

    [[nodiscard]] const void* handle() const
    {
        return _handle;
    }

    [[nodiscard]] void* handle()
    {
        return _handle;
    }

    void swap(regular_bg_ptr& other)
    {
        btn::swap(_handle, other._handle);
    }

    friend void swap(regular_bg_ptr& a, regular_bg_ptr& b)
    {
        btn::swap(a._handle, b._handle);
    }

    [[nodiscard]] friend bool operator==(const regular_bg_ptr& a, const regular_bg_ptr& b) = default;

private:
    using handle_type = void*;

    handle_type _handle;

    explicit regular_bg_ptr(handle_type handle) :
        _handle(handle)
    {
    }

    void _destroy();
};


template<>
struct hash<regular_bg_ptr>
{
    [[nodiscard]] unsigned operator()(const regular_bg_ptr& value) const
    {
        return make_hash(value.handle());
    }
};

}

#endif
