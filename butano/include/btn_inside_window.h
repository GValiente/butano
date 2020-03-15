#ifndef BTN_INSIDE_WINDOW_H
#define BTN_INSIDE_WINDOW_H

#include "btn_window.h"

namespace btn
{

class inside_window : public window
{

public:
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
