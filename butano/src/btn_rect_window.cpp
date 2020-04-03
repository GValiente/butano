#include "btn_rect_window.h"

#include "btn_fixed_rect.h"
#include "btn_display_manager.h"

namespace btn
{

fixed rect_window::top() const
{
    return top_left().y();
}

fixed rect_window::left() const
{
    return top_left().x();
}

const fixed_point& rect_window::top_left() const
{
    return display_manager::rect_window_top_left(id());
}

fixed rect_window::bottom() const
{
    return bottom_right().y();
}

fixed rect_window::right() const
{
    return bottom_right().x();
}

const fixed_point& rect_window::bottom_right() const
{
    return display_manager::rect_window_bottom_right(id());
}

fixed_rect rect_window::boundaries() const
{
    const fixed_point& tl = top_left();
    const fixed_point& br = bottom_right();
    fixed_size dimensions(br.x() - tl.x(), br.y() - tl.y());
    return fixed_rect(tl + fixed_point(dimensions.width() / 2, dimensions.height() / 2), dimensions);
}

void rect_window::set_top(fixed top)
{
    BTN_ASSERT(top <= bottom_right().y(), "Invalid top: ", top, " - ", bottom_right().y());

    fixed_point tl = top_left();
    tl.set_y(top);
    display_manager::set_rect_window_top_left(id(), tl);
}

void rect_window::set_left(fixed left)
{
    BTN_ASSERT(left <= bottom_right().x(), "Invalid left: ", left, " - ", bottom_right().x());

    fixed_point tl = top_left();
    tl.set_x(left);
    display_manager::set_rect_window_top_left(id(), tl);
}

void rect_window::set_top_left(fixed top, fixed left)
{
    set_top_left(fixed_point(left, top));
}

void rect_window::set_top_left(const fixed_point& top_left)
{
    BTN_ASSERT(top_left.y() <= bottom_right().y(), "Invalid top: ", top_left.y(), " - ", bottom_right().y());
    BTN_ASSERT(top_left.x() <= bottom_right().x(), "Invalid left: ", top_left.x(), " - ", bottom_right().x());

    display_manager::set_rect_window_top_left(id(), top_left);
}

void rect_window::set_bottom(fixed bottom)
{
    BTN_ASSERT(bottom >= top_left().y(), "Invalid bottom: ", bottom, " - ", top_left().y());

    fixed_point br = bottom_right();
    br.set_y(bottom);
    display_manager::set_rect_window_bottom_right(id(), br);
}

void rect_window::set_right(fixed right)
{
    BTN_ASSERT(right >= top_left().x(), "Invalid right: ", right, " - ", top_left().x());

    fixed_point br = bottom_right();
    br.set_x(right);
    display_manager::set_rect_window_bottom_right(id(), br);
}

void rect_window::set_bottom_right(fixed bottom, fixed right)
{
    set_bottom_right(fixed_point(right, bottom));
}

void rect_window::set_bottom_right(const fixed_point& bottom_right)
{
    BTN_ASSERT(bottom_right.y() >= top_left().y(), "Invalid bottom: ", bottom_right.y(), " - ", top_left().y());
    BTN_ASSERT(bottom_right.x() >= top_left().x(), "Invalid right: ", bottom_right.x(), " - ", top_left().x());

    display_manager::set_rect_window_bottom_right(id(), bottom_right);
}

void rect_window::set_boundaries(fixed top, fixed left, fixed bottom, fixed right)
{
    set_boundaries(fixed_point(left, top), fixed_point(right, bottom));
}

void rect_window::set_boundaries(const fixed_point& top_left, const fixed_point& bottom_right)
{
    BTN_ASSERT(top_left.y() <= bottom_right.y(), "Invalid top or bottom: ", top_left.y(), " - ", bottom_right.y());
    BTN_ASSERT(top_left.x() <= bottom_right.x(), "Invalid left or right: ", top_left.x(), " - ", bottom_right.x());

    display_manager::set_rect_window_top_left(id(), top_left);
    display_manager::set_rect_window_bottom_right(id(), bottom_right);
}

void rect_window::set_boundaries(const fixed_rect& boundaries)
{
    fixed_point half_dimensions(boundaries.width() / 2, boundaries.height() / 2);
    display_manager::set_rect_window_top_left(id(), boundaries.position() - half_dimensions);
    display_manager::set_rect_window_bottom_right(id(), boundaries.position() + half_dimensions);
}

bool rect_window::ignore_camera() const
{
    return display_manager::rect_window_ignore_camera(id());
}

void rect_window::set_ignore_camera(bool ignore_camera)
{
    display_manager::set_rect_window_ignore_camera(id(), ignore_camera);
}

}
