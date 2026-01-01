/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_palette_bitmap_bg_builder.h"

#include "bn_bgs.h"
#include "bn_bitmap_bg.h"
#include "bn_top_left_utils.h"
#include "bn_palette_bitmap_bg_ptr.h"

namespace bn
{

palette_bitmap_bg_builder::palette_bitmap_bg_builder(const bg_palette_item& palette_item) :
    _palette_item(palette_item)
{
    BN_ASSERT(palette_item.bpp() == bpp_mode::BPP_8, "4BPP bitmap backgrounds not supported");
}

palette_bitmap_bg_builder::palette_bitmap_bg_builder(const bg_palette_ptr& palette) :
    _palette(palette)
{
    BN_ASSERT(palette.bpp() == bpp_mode::BPP_8, "4BPP bitmap backgrounds not supported");
}

palette_bitmap_bg_builder::palette_bitmap_bg_builder(bg_palette_ptr&& palette) :
    _palette(move(palette))
{
    BN_ASSERT(_palette->bpp() == bpp_mode::BPP_8, "4BPP bitmap backgrounds not supported");
}

fixed palette_bitmap_bg_builder::top_left_x() const
{
    return to_top_left_x(x(), bitmap_bg::palette_width());
}

palette_bitmap_bg_builder& palette_bitmap_bg_builder::set_top_left_x(fixed top_left_x)
{
    return set_x(from_top_left_x(top_left_x, bitmap_bg::palette_width()));
}

fixed palette_bitmap_bg_builder::top_left_y() const
{
    return to_top_left_y(y(), bitmap_bg::palette_height());
}

palette_bitmap_bg_builder& palette_bitmap_bg_builder::set_top_left_y(fixed top_left_y)
{
    return set_y(from_top_left_y(top_left_y, bitmap_bg::palette_height()));
}

fixed_point palette_bitmap_bg_builder::top_left_position() const
{
    return to_top_left_position(position(), bitmap_bg::palette_size());
}

palette_bitmap_bg_builder& palette_bitmap_bg_builder::set_top_left_position(fixed top_left_x, fixed top_left_y)
{
    return set_position(from_top_left_position(fixed_point(top_left_x, top_left_y), bitmap_bg::palette_size()));
}

palette_bitmap_bg_builder& palette_bitmap_bg_builder::set_top_left_position(const fixed_point& top_left_position)
{
    return set_position(from_top_left_position(top_left_position, bitmap_bg::palette_size()));
}

palette_bitmap_bg_builder& palette_bitmap_bg_builder::set_priority(int priority)
{
    BN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

    _priority = priority;
    return *this;
}

optional<camera_ptr> palette_bitmap_bg_builder::release_camera()
{
    optional<camera_ptr> result = move(_camera);
    _camera.reset();
    return result;
}

palette_bitmap_bg_ptr palette_bitmap_bg_builder::build() const
{
    return palette_bitmap_bg_ptr::create(*this);
}

palette_bitmap_bg_ptr palette_bitmap_bg_builder::release_build()
{
    return palette_bitmap_bg_ptr::create(move(*this));
}

optional<palette_bitmap_bg_ptr> palette_bitmap_bg_builder::build_optional() const
{
    return palette_bitmap_bg_ptr::create_optional(*this);
}

optional<palette_bitmap_bg_ptr> palette_bitmap_bg_builder::release_build_optional()
{
    return palette_bitmap_bg_ptr::create_optional(move(*this));
}

bg_palette_ptr palette_bitmap_bg_builder::palette() const
{
    if(const bg_palette_item* palette_item = _palette_item.get())
    {
        return bg_palette_ptr::create(*palette_item);
    }

    return *_palette;
}

optional<bg_palette_ptr> palette_bitmap_bg_builder::palette_optional() const
{
    optional<bg_palette_ptr> result;

    if(const bg_palette_item* palette_item = _palette_item.get())
    {
        result = bg_palette_ptr::create_optional(*palette_item);
    }
    else
    {
        result = _palette;
    }

    return result;
}

bg_palette_ptr palette_bitmap_bg_builder::release_palette()
{
    const bg_palette_item* palette_item = _palette_item.get();
    bg_palette_ptr result = palette_item ? bg_palette_ptr::create(*palette_item) : move(*_palette);
    _palette.reset();
    return result;
}

optional<bg_palette_ptr> palette_bitmap_bg_builder::release_palette_optional()
{
    optional<bg_palette_ptr> result;

    if(const bg_palette_item* palette_item = _palette_item.get())
    {
        result = bg_palette_ptr::create_optional(*palette_item);
    }
    else
    {
        result = move(_palette);
        _palette.reset();
    }

    return result;
}

}
