#include "btn_inside_window.h"

#include "btn_display_manager.h"

namespace btn
{

bool inside_window::visible() const
{
    return display_manager::inside_window_enabled(id());
}

void inside_window::set_visible(bool visible)
{
    return display_manager::set_inside_window_enabled(id(), visible);
}

}
