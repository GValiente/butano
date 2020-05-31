#ifndef BTN_RECT_WINDOW_H
#define BTN_RECT_WINDOW_H

#include "btn_fixed_fwd.h"
#include "btn_inside_window.h"
#include "btn_config_camera.h"

namespace btn
{

class fixed_rect;
class fixed_point;

class rect_window : public inside_window
{

public:
    [[nodiscard]] static rect_window internal()
    {
        return rect_window(window::internal().id());
    }

    [[nodiscard]] static window external()
    {
        return rect_window(window::external().id());
    }

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

    #if BTN_CFG_CAMERA_ENABLED
        [[nodiscard]] bool ignore_camera() const;

        void set_ignore_camera(bool ignore_camera);
    #endif

private:
    friend class window;

    explicit rect_window(int id) :
        inside_window(id)
    {
    }
};

}

#endif
