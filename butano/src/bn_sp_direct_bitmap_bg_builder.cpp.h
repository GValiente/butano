/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sp_direct_bitmap_bg_builder.h"

#include "bn_bgs.h"
#include "bn_bitmap_bg.h"
#include "bn_top_left_utils.h"
#include "bn_sp_direct_bitmap_bg_ptr.h"

namespace bn
{

fixed sp_direct_bitmap_bg_builder::top_left_x() const
{
    return to_top_left_x(x(), bitmap_bg::sp_direct_width());
}

sp_direct_bitmap_bg_builder& sp_direct_bitmap_bg_builder::set_top_left_x(fixed top_left_x)
{
    return set_x(from_top_left_x(top_left_x, bitmap_bg::sp_direct_width()));
}

fixed sp_direct_bitmap_bg_builder::top_left_y() const
{
    return to_top_left_y(y(), bitmap_bg::sp_direct_height());
}

sp_direct_bitmap_bg_builder& sp_direct_bitmap_bg_builder::set_top_left_y(fixed top_left_y)
{
    return set_y(from_top_left_y(top_left_y, bitmap_bg::sp_direct_height()));
}

fixed_point sp_direct_bitmap_bg_builder::top_left_position() const
{
    return to_top_left_position(position(), bitmap_bg::sp_direct_size());
}

sp_direct_bitmap_bg_builder& sp_direct_bitmap_bg_builder::set_top_left_position(fixed top_left_x, fixed top_left_y)
{
    return set_position(from_top_left_position(fixed_point(top_left_x, top_left_y), bitmap_bg::sp_direct_size()));
}

sp_direct_bitmap_bg_builder& sp_direct_bitmap_bg_builder::set_top_left_position(const fixed_point& top_left_position)
{
    return set_position(from_top_left_position(top_left_position, bitmap_bg::sp_direct_size()));
}

sp_direct_bitmap_bg_builder& sp_direct_bitmap_bg_builder::expand()
{
    fixed horizontal_scale = fixed(display::width()) / bitmap_bg::sp_direct_width();
    fixed vertical_scale = fixed(display::height()) / bitmap_bg::sp_direct_height();
    _mat_attributes.set_scale(horizontal_scale, vertical_scale);
    return *this;
}

sp_direct_bitmap_bg_builder& sp_direct_bitmap_bg_builder::set_priority(int priority)
{
    BN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

    _priority = priority;
    return *this;
}

optional<camera_ptr> sp_direct_bitmap_bg_builder::release_camera()
{
    optional<camera_ptr> result = move(_camera);
    _camera.reset();
    return result;
}

sp_direct_bitmap_bg_ptr sp_direct_bitmap_bg_builder::build() const
{
    return sp_direct_bitmap_bg_ptr::create(*this);
}

sp_direct_bitmap_bg_ptr sp_direct_bitmap_bg_builder::release_build()
{
    return sp_direct_bitmap_bg_ptr::create(move(*this));
}

optional<sp_direct_bitmap_bg_ptr> sp_direct_bitmap_bg_builder::build_optional() const
{
    return sp_direct_bitmap_bg_ptr::create_optional(*this);
}

optional<sp_direct_bitmap_bg_ptr> sp_direct_bitmap_bg_builder::release_build_optional()
{
    return sp_direct_bitmap_bg_ptr::create_optional(move(*this));
}

}
