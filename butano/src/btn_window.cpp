#include "btn_window.h"

#include "btn_regular_bg_ptr.h"
#include "btn_display_manager.h"

namespace btn
{

bool window::show_bg(const regular_bg_ptr& regular_bg) const
{
    return display_manager::show_bg_in_window(_id, regular_bg.id());
}

void window::set_show_bg(const regular_bg_ptr& regular_bg, bool show)
{
    return display_manager::set_show_bg_in_window(_id, regular_bg.id(), show);
}

bool window::show_sprites() const
{
    return display_manager::show_sprites_in_window(_id);
}

void window::set_show_sprites(bool show)
{
    return display_manager::set_show_sprites_in_window(_id, show);
}

bool window::show_blending() const
{
    return display_manager::show_blending_in_window(_id);
}

void window::set_show_blending(bool show)
{
    return display_manager::set_show_blending_in_window(_id, show);
}

}
