/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_unsafe_palette_bitmap_bg_painter.h"

#include "bn_display_manager.h"

namespace bn
{

unsafe_palette_bitmap_bg_painter::unsafe_palette_bitmap_bg_painter(const palette_bitmap_bg_ptr& bg) :
    _bg(bg),
    _page(display_manager::bitmap_page())
{
    display_manager::on_bitmap_painter_created(reinterpret_cast<void**>(&_page));
}

unsafe_palette_bitmap_bg_painter::unsafe_palette_bitmap_bg_painter(palette_bitmap_bg_ptr&& bg) :
    _bg(move(bg)),
    _page(display_manager::bitmap_page())
{
    display_manager::on_bitmap_painter_created(reinterpret_cast<void**>(&_page));
}

unsafe_palette_bitmap_bg_painter::unsafe_palette_bitmap_bg_painter(unsafe_palette_bitmap_bg_painter&& other) noexcept :
    _bg(move(other._bg)),
    _page(other._page)
{
    other._page = nullptr;

    display_manager::on_bitmap_painter_destroyed();
    display_manager::on_bitmap_painter_created(reinterpret_cast<void**>(&_page));
}

unsafe_palette_bitmap_bg_painter& unsafe_palette_bitmap_bg_painter::operator=(unsafe_palette_bitmap_bg_painter&& other) noexcept
{
    if(this != &other)
    {
        _bg = move(other._bg);
        _page = other._page;
        other._page = nullptr;

        display_manager::on_bitmap_painter_destroyed();
        display_manager::on_bitmap_painter_created(reinterpret_cast<void**>(&_page));
    }

    return *this;
}

unsafe_palette_bitmap_bg_painter::~unsafe_palette_bitmap_bg_painter()
{
    if(_page)
    {
        display_manager::on_bitmap_painter_destroyed();
    }
}

void unsafe_palette_bitmap_bg_painter::flip_page_now()
{
    _bg.flip_page_now();
    _page = display_manager::bitmap_page();
}

}
