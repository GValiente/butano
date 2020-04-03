#ifndef BTN_RECT_WINDOW_H
#define BTN_RECT_WINDOW_H

#include "btn_fixed_fwd.h"
#include "btn_inside_window.h"

namespace btn
{

class fixed_rect;
class fixed_point;

class rect_window : public inside_window
{

public:
    [[nodiscard]] fixed top() const;

    [[nodiscard]] fixed left() const;

    [[nodiscard]] const fixed_point& top_left() const;

    [[nodiscard]] fixed bottom() const;

    [[nodiscard]] fixed right() const;

    [[nodiscard]] const fixed_point& bottom_right() const;

    [[nodiscard]] fixed_rect boundaries() const;

    void set_top(fixed top);

    void set_left(fixed left);

    void set_top_left(fixed top, fixed left);

    void set_top_left(const fixed_point& top_left);

    void set_bottom(fixed bottom);

    void set_right(fixed right);

    void set_bottom_right(fixed bottom, fixed right);

    void set_bottom_right(const fixed_point& bottom_right);

    void set_boundaries(fixed top, fixed left, fixed bottom, fixed right);

    void set_boundaries(const fixed_point& top_left, const fixed_point& bottom_right);

    void set_boundaries(const fixed_rect& boundaries);

    [[nodiscard]] bool ignore_camera() const;

    void set_ignore_camera(bool ignore_camera);

private:
    friend class window;

    explicit rect_window(int id) :
        inside_window(id)
    {
    }
};

}

#endif
