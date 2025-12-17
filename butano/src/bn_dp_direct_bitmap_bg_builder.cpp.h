/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_dp_direct_bitmap_bg_builder.h"

#include "bn_bgs.h"
#include "bn_bitmap_bg.h"
#include "bn_top_left_utils.h"
#include "bn_dp_direct_bitmap_bg_ptr.h"

namespace bn
{

fixed dp_direct_bitmap_bg_builder::top_left_x() const
{
    return to_top_left_x(x(), bitmap_bg::dp_direct_width());
}

dp_direct_bitmap_bg_builder& dp_direct_bitmap_bg_builder::set_top_left_x(fixed top_left_x)
{
    return set_x(from_top_left_x(top_left_x, bitmap_bg::dp_direct_width()));
}

fixed dp_direct_bitmap_bg_builder::top_left_y() const
{
    return to_top_left_y(y(), bitmap_bg::dp_direct_height());
}

dp_direct_bitmap_bg_builder& dp_direct_bitmap_bg_builder::set_top_left_y(fixed top_left_y)
{
    return set_y(from_top_left_y(top_left_y, bitmap_bg::dp_direct_height()));
}

fixed_point dp_direct_bitmap_bg_builder::top_left_position() const
{
    return to_top_left_position(position(), bitmap_bg::dp_direct_size());
}

dp_direct_bitmap_bg_builder& dp_direct_bitmap_bg_builder::set_top_left_position(fixed top_left_x, fixed top_left_y)
{
    return set_position(from_top_left_position(fixed_point(top_left_x, top_left_y), bitmap_bg::dp_direct_size()));
}

dp_direct_bitmap_bg_builder& dp_direct_bitmap_bg_builder::set_top_left_position(const fixed_point& top_left_position)
{
    return set_position(from_top_left_position(top_left_position, bitmap_bg::dp_direct_size()));
}

dp_direct_bitmap_bg_builder& dp_direct_bitmap_bg_builder::set_priority(int priority)
{
    BN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

    _priority = priority;
    return *this;
}

optional<camera_ptr> dp_direct_bitmap_bg_builder::release_camera()
{
    optional<camera_ptr> result = move(_camera);
    _camera.reset();
    return result;
}

dp_direct_bitmap_bg_ptr dp_direct_bitmap_bg_builder::build() const
{
    return dp_direct_bitmap_bg_ptr::create(*this);
}

dp_direct_bitmap_bg_ptr dp_direct_bitmap_bg_builder::release_build()
{
    return dp_direct_bitmap_bg_ptr::create(move(*this));
}

optional<dp_direct_bitmap_bg_ptr> dp_direct_bitmap_bg_builder::build_optional() const
{
    return dp_direct_bitmap_bg_ptr::create_optional(*this);
}

optional<dp_direct_bitmap_bg_ptr> dp_direct_bitmap_bg_builder::release_build_optional()
{
    return dp_direct_bitmap_bg_ptr::create_optional(move(*this));
}

}
