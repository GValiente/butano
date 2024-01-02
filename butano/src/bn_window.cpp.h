/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_window.h"

#include "bn_bgs_manager.h"
#include "bn_affine_bg_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_display_manager.h"

namespace bn
{

bool window::show_bg(const regular_bg_ptr& regular_bg) const
{
    auto bg_handle = const_cast<void*>(regular_bg.handle());
    return bgs_manager::show_in_window(bg_handle, _id);
}

bool window::show_bg(const affine_bg_ptr& affine_bg) const
{
    auto bg_handle = const_cast<void*>(affine_bg.handle());
    return bgs_manager::show_in_window(bg_handle, _id);
}

void window::set_show_bg(const regular_bg_ptr& regular_bg, bool show)
{
    auto bg_handle = const_cast<void*>(regular_bg.handle());
    bgs_manager::set_show_in_window(bg_handle, _id, show);
}

void window::set_show_bg(const affine_bg_ptr& affine_bg, bool show)
{
    auto bg_handle = const_cast<void*>(affine_bg.handle());
    bgs_manager::set_show_in_window(bg_handle, _id, show);
}

bool window::show_sprites() const
{
    return display_manager::show_sprites_in_window(_id);
}

void window::set_show_sprites(bool show)
{
    display_manager::set_show_sprites_in_window(_id, show);
}

bool window::show_blending() const
{
    return display_manager::show_blending_in_window(_id);
}

void window::set_show_blending(bool show)
{
    display_manager::set_show_blending_in_window(_id, show);
}

bool window::show_all() const
{
    return display_manager::show_all_in_window(_id);
}

void window::set_show_all()
{
    int id = _id;
    bgs_manager::set_show_all_in_window(id, true);
    display_manager::set_show_all_in_window(id);
}

bool window::show_nothing() const
{
    return display_manager::show_nothing_in_window(_id);
}

void window::set_show_nothing()
{
    int id = _id;
    bgs_manager::set_show_all_in_window(id, false);
    display_manager::set_show_nothing_in_window(id);
}

void window::restore()
{
    int id = _id;

    switch(id)
    {

    case 0:
    case 1:
        display_manager::set_rect_window_top_left(id, fixed_point());
        display_manager::set_rect_window_bottom_right(id, fixed_point());
        display_manager::remove_rect_window_camera(id);
        [[fallthrough]];

    case 2:
        display_manager::set_inside_window_enabled(id, true);
        [[fallthrough]];

    default:
        bgs_manager::set_show_all_in_window(id, true);
        display_manager::set_show_all_in_window(id);
        break;
    }
}

}
