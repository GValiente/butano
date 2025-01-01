/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_rect_window.h"

#include "bn_camera_ptr.h"
#include "bn_display_manager.h"

namespace bn
{

fixed rect_window::top() const
{
    return top_left().y();
}

void rect_window::set_top(fixed top)
{
    BN_ASSERT(top <= bottom_right().y(), "Invalid top: ", top, " - ", bottom_right().y());

    fixed_point tl = top_left();
    tl.set_y(top);
    display_manager::set_rect_window_top_left(id(), tl);
}

fixed rect_window::left() const
{
    return top_left().x();
}

void rect_window::set_left(fixed left)
{
    BN_ASSERT(left <= bottom_right().x(), "Invalid left: ", left, " - ", bottom_right().x());

    fixed_point tl = top_left();
    tl.set_x(left);
    display_manager::set_rect_window_top_left(id(), tl);
}

const fixed_point& rect_window::top_left() const
{
    return display_manager::rect_window_top_left(id());
}

void rect_window::set_top_left(fixed top, fixed left)
{
    set_top_left(fixed_point(left, top));
}

void rect_window::set_top_left(const fixed_point& top_left)
{
    BN_ASSERT(top_left.y() <= bottom_right().y(), "Invalid top: ", top_left.y(), " - ", bottom_right().y());
    BN_ASSERT(top_left.x() <= bottom_right().x(), "Invalid left: ", top_left.x(), " - ", bottom_right().x());

    display_manager::set_rect_window_top_left(id(), top_left);
}

fixed rect_window::bottom() const
{
    return bottom_right().y();
}

void rect_window::set_bottom(fixed bottom)
{
    BN_ASSERT(bottom >= top_left().y(), "Invalid bottom: ", bottom, " - ", top_left().y());

    fixed_point br = bottom_right();
    br.set_y(bottom);
    display_manager::set_rect_window_bottom_right(id(), br);
}

fixed rect_window::right() const
{
    return bottom_right().x();
}

void rect_window::set_right(fixed right)
{
    BN_ASSERT(right >= top_left().x(), "Invalid right: ", right, " - ", top_left().x());

    fixed_point br = bottom_right();
    br.set_x(right);
    display_manager::set_rect_window_bottom_right(id(), br);
}

const fixed_point& rect_window::bottom_right() const
{
    return display_manager::rect_window_bottom_right(id());
}

void rect_window::set_bottom_right(fixed bottom, fixed right)
{
    set_bottom_right(fixed_point(right, bottom));
}

void rect_window::set_bottom_right(const fixed_point& bottom_right)
{
    BN_ASSERT(bottom_right.y() >= top_left().y(), "Invalid bottom: ", bottom_right.y(), " - ", top_left().y());
    BN_ASSERT(bottom_right.x() >= top_left().x(), "Invalid right: ", bottom_right.x(), " - ", top_left().x());

    display_manager::set_rect_window_bottom_right(id(), bottom_right);
}

fixed_rect rect_window::boundaries() const
{
    const fixed_point& tl = top_left();
    const fixed_point& br = bottom_right();
    fixed_size dimensions(br.x() - tl.x(), br.y() - tl.y());
    return fixed_rect(tl + fixed_point(dimensions.width() / 2, dimensions.height() / 2), dimensions);
}

top_left_fixed_rect rect_window::top_left_boundaries() const
{
    const fixed_point& tl = top_left();
    const fixed_point& br = bottom_right();
    fixed_size dimensions(br.x() - tl.x(), br.y() - tl.y());
    return top_left_fixed_rect(tl, dimensions);
}

void rect_window::set_boundaries(fixed top, fixed left, fixed bottom, fixed right)
{
    set_boundaries(fixed_point(left, top), fixed_point(right, bottom));
}

void rect_window::set_boundaries(const fixed_point& top_left, const fixed_point& bottom_right)
{
    BN_ASSERT(top_left.y() <= bottom_right.y(), "Invalid top or bottom: ", top_left.y(), " - ", bottom_right.y());
    BN_ASSERT(top_left.x() <= bottom_right.x(), "Invalid left or right: ", top_left.x(), " - ", bottom_right.x());

    int window_id = id();
    display_manager::set_rect_window_top_left(window_id, top_left);
    display_manager::set_rect_window_bottom_right(window_id, bottom_right);
}

void rect_window::set_boundaries(const fixed_rect& boundaries)
{
    int window_id = id();
    fixed_point half_dimensions(boundaries.width() / 2, boundaries.height() / 2);
    display_manager::set_rect_window_top_left(window_id, boundaries.position() - half_dimensions);
    display_manager::set_rect_window_bottom_right(window_id, boundaries.position() + half_dimensions);
}

void rect_window::set_boundaries(const top_left_fixed_rect& boundaries)
{
    int window_id = id();
    display_manager::set_rect_window_top_left(window_id, boundaries.top_left());
    display_manager::set_rect_window_bottom_right(window_id, boundaries.bottom_right());
}

void rect_window::restore_boundaries()
{
    int window_id = id();
    display_manager::set_rect_window_top_left(window_id, fixed_point());
    display_manager::set_rect_window_bottom_right(window_id, fixed_point());
}

const optional<camera_ptr>& rect_window::camera() const
{
    return display_manager::rect_window_camera(id());
}

void rect_window::set_camera(const camera_ptr& camera)
{
    display_manager::set_rect_window_camera(id(), camera_ptr(camera));
}

void rect_window::set_camera(camera_ptr&& camera)
{
    display_manager::set_rect_window_camera(id(), move(camera));
}

void rect_window::set_camera(const optional<camera_ptr>& camera)
{
    if(const camera_ptr* camera_ref = camera.get())
    {
        display_manager::set_rect_window_camera(id(), camera_ptr(*camera_ref));
    }
    else
    {
        display_manager::remove_rect_window_camera(id());
    }
}

void rect_window::set_camera(optional<camera_ptr>&& camera)
{
    if(camera_ptr* camera_ref = camera.get())
    {
        display_manager::set_rect_window_camera(id(), move(*camera_ref));
    }
    else
    {
        display_manager::remove_rect_window_camera(id());
    }
}

void rect_window::remove_camera()
{
    display_manager::remove_rect_window_camera(id());
}

}
