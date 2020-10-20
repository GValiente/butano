/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_PALETTE_PTR_H
#define BTN_SPRITE_PALETTE_PTR_H

#include "btn_utility.h"
#include "btn_span_fwd.h"
#include "btn_fixed_fwd.h"
#include "btn_functional.h"
#include "btn_optional_fwd.h"

namespace btn
{

class color;
class sprite_palette_item;
enum class palette_bpp_mode;

class sprite_palette_ptr
{

public:
    [[nodiscard]] static optional<sprite_palette_ptr> find(const span<const color>& colors, palette_bpp_mode bpp_mode);

    [[nodiscard]] static optional<sprite_palette_ptr> find(const sprite_palette_item& palette_item);

    [[nodiscard]] static sprite_palette_ptr create(const span<const color>& colors, palette_bpp_mode bpp_mode);

    [[nodiscard]] static sprite_palette_ptr create(const sprite_palette_item& palette_item);

    [[nodiscard]] static sprite_palette_ptr find_or_create(const span<const color>& colors, palette_bpp_mode bpp_mode);

    [[nodiscard]] static sprite_palette_ptr find_or_create(const sprite_palette_item& palette_item);

    [[nodiscard]] static optional<sprite_palette_ptr> create_optional(const span<const color>& colors,
                                                                      palette_bpp_mode bpp_mode);

    [[nodiscard]] static optional<sprite_palette_ptr> create_optional(const sprite_palette_item& palette_item);

    [[nodiscard]] static optional<sprite_palette_ptr> find_or_create_optional(const span<const color>& colors,
                                                                              palette_bpp_mode bpp_mode);

    [[nodiscard]] static optional<sprite_palette_ptr> find_or_create_optional(const sprite_palette_item& palette_item);

    sprite_palette_ptr(const sprite_palette_ptr& other);

    sprite_palette_ptr& operator=(const sprite_palette_ptr& other);

    sprite_palette_ptr(sprite_palette_ptr&& other) noexcept :
        _id(other._id)
    {
        other._id = -1;
    }

    sprite_palette_ptr& operator=(sprite_palette_ptr&& other) noexcept
    {
        btn::swap(_id, other._id);
        return *this;
    }

    ~sprite_palette_ptr()
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

    [[nodiscard]] span<const color> colors() const;

    void set_colors(const span<const color>& colors);

    void set_colors(const sprite_palette_item& palette_item);

    [[nodiscard]] int colors_count() const;

    [[nodiscard]] palette_bpp_mode bpp_mode() const;

    [[nodiscard]] bool inverted() const;

    void set_inverted(bool inverted);

    [[nodiscard]] fixed grayscale_intensity() const;

    void set_grayscale_intensity(fixed intensity);

    [[nodiscard]] color fade_color() const;

    [[nodiscard]] fixed fade_intensity() const;

    void set_fade_color(color color);

    void set_fade_intensity(fixed intensity);

    void set_fade(color color, fixed intensity);

    [[nodiscard]] int rotate_count() const;

    void set_rotate_count(int count);

    void swap(sprite_palette_ptr& other)
    {
        btn::swap(_id, other._id);
    }

    friend void swap(sprite_palette_ptr& a, sprite_palette_ptr& b)
    {
        btn::swap(a._id, b._id);
    }

    [[nodiscard]] friend bool operator==(const sprite_palette_ptr& a, const sprite_palette_ptr& b) = default;

private:
    int8_t _id;

    explicit sprite_palette_ptr(int id) :
        _id(int8_t(id))
    {
    }

    void _destroy();
};


template<>
struct hash<sprite_palette_ptr>
{
    [[nodiscard]] unsigned operator()(const sprite_palette_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
