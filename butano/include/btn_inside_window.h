#ifndef BTN_INSIDE_WINDOW_H
#define BTN_INSIDE_WINDOW_H

#include "btn_window.h"

namespace btn
{

class inside_window : public window
{

public:
    [[nodiscard]] static inside_window internal()
    {
        return inside_window(window::internal().id());
    }

    [[nodiscard]] static inside_window external()
    {
        return inside_window(window::external().id());
    }

    [[nodiscard]] static inside_window sprites()
    {
        return inside_window(window::sprites().id());
    }

    [[nodiscard]] bool visible() const;

    void set_visible(bool visible);

protected:
    explicit inside_window(int id) :
        window(id)
    {
    }

private:
    friend class window;
};

}

#endif
