#include "btn_rect_window.h"

#include "btn_display.h"
#include "btn_fixed_point.h"
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

void rect_window::set_top(fixed top)
{
    BTN_ASSERT(top >= 0 && top <= display::height(), "Invalid top: ", top);

    const fixed_point& br = bottom_right();
    BTN_ASSERT(top <= br.y(), "Top is below bottom: ", top, " - ", br.y());

    fixed_point tl = top_left();
    tl.set_y(top);
    display_manager::set_rect_window_boundaries(id(), tl, br);
}

void rect_window::set_left(fixed left)
{
    BTN_ASSERT(left >= 0 && left <= display::width(), "Invalid left: ", left);

    const fixed_point& br = bottom_right();
    BTN_ASSERT(left <= br.x(), "Left is higher than right: ", left, " - ", br.x());

    fixed_point tl = top_left();
    tl.set_x(left);
    display_manager::set_rect_window_boundaries(id(), tl, br);
}

void rect_window::set_top_left(fixed top, fixed left)
{
    set_top_left(fixed_point(left, top));
}

void rect_window::set_top_left(const fixed_point& top_left)
{
    BTN_ASSERT(top_left.y() >= 0 && top_left.y() <= display::height(), "Invalid top: ", top_left.y());
    BTN_ASSERT(top_left.x() >= 0 && top_left.x() <= display::width(), "Invalid left: ", top_left.x());

    const fixed_point& br = bottom_right();
    BTN_ASSERT(top_left.y() <= br.y(), "Top is below bottom: ", top_left.y(), " - ", br.y());
    BTN_ASSERT(top_left.x() <= br.x(), "Left is higher than right: ", top_left.x(), " - ", br.x());

    display_manager::set_rect_window_boundaries(id(), top_left, br);
}

void rect_window::set_bottom(fixed bottom)
{
    BTN_ASSERT(bottom >= 0 && bottom <= display::height(), "Invalid bottom: ", bottom);

    const fixed_point& tl = top_left();
    BTN_ASSERT(bottom >= tl.y(), "Bottom is above top: ", bottom, " - ", tl.y());

    fixed_point br = bottom_right();
    br.set_y(bottom);
    display_manager::set_rect_window_boundaries(id(), tl, br);
}

void rect_window::set_right(fixed right)
{
    BTN_ASSERT(right >= 0 && right <= display::width(), "Invalid right: ", right);

    const fixed_point& tl = top_left();
    BTN_ASSERT(right >= tl.x(), "Right is lower than left: ", right, " - ", tl.x());

    fixed_point br = bottom_right();
    br.set_x(right);
    display_manager::set_rect_window_boundaries(id(), tl, br);
}

void rect_window::set_bottom_right(fixed bottom, fixed right)
{
    set_bottom_right(fixed_point(right, bottom));
}

void rect_window::set_bottom_right(const fixed_point& bottom_right)
{
    BTN_ASSERT(bottom_right.y() >= 0 && bottom_right.y() <= display::height(), "Invalid bottom: ", bottom_right.y());
    BTN_ASSERT(bottom_right.x() >= 0 && bottom_right.x() <= display::width(), "Invalid right: ", bottom_right.x());

    const fixed_point& tl = top_left();
    BTN_ASSERT(bottom_right.y() >= tl.y(), "Bottom is above top: ", bottom_right.y(), " - ", tl.y());
    BTN_ASSERT(bottom_right.x() >= tl.x(), "Right is lower than left: ", bottom_right.x(), " - ", tl.x());

    display_manager::set_rect_window_boundaries(id(), tl, bottom_right);
}

void rect_window::set_boundaries(fixed top, fixed left, fixed bottom, fixed right)
{
    set_boundaries(fixed_point(left, top), fixed_point(right, bottom));
}

void rect_window::set_boundaries(const fixed_point& top_left, const fixed_point& bottom_right)
{
    BTN_ASSERT(top_left.y() >= 0 && top_left.y() <= display::height(), "Invalid top: ", top_left.y());
    BTN_ASSERT(top_left.x() >= 0 && top_left.x() <= display::width(), "Invalid left: ", top_left.x());
    BTN_ASSERT(bottom_right.y() >= 0 && bottom_right.y() <= display::height(), "Invalid bottom: ", bottom_right.y());
    BTN_ASSERT(bottom_right.x() >= 0 && bottom_right.x() <= display::width(), "Invalid right: ", bottom_right.x());
    BTN_ASSERT(top_left.y() <= bottom_right.y(), "Top is below bottom: ", top_left.y(), " - ", bottom_right.y());
    BTN_ASSERT(top_left.x() <= bottom_right.x(), "Left is higher than right: ", top_left.x(), " - ", bottom_right.x());

    display_manager::set_rect_window_boundaries(id(), top_left, bottom_right);
}

}
