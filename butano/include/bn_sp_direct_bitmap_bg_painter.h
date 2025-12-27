/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SP_DIRECT_BITMAP_BG_PAINTER_H
#define BN_SP_DIRECT_BITMAP_BG_PAINTER_H

/**
 * @file
 * bn::sp_direct_bitmap_bg_painter header file.
 *
 * @ingroup sp_direct_bitmap_bg
 */

#include "bn_point.h"
#include "bn_memory.h"
#include "bn_bitmap_bg.h"
#include "bn_clip_line.h"
#include "bn_direct_bitmap_roi.h"
#include "bn_sp_direct_bitmap_bg_ptr.h"

namespace bn
{

/**
 * @brief Performs low-level painting on single page direct bitmap backgrounds.
 * @ingroup sp_direct_bitmap_bg
 */
class sp_direct_bitmap_bg_painter
{

public:
    /**
     * @brief Class constructor.
     * @param bg Single page direct bitmap background to be modified.
     */
    explicit sp_direct_bitmap_bg_painter(const sp_direct_bitmap_bg_ptr& bg) :
        _bg(bg)
    {
    }

    /**
     * @brief Class constructor.
     * @param bg Single page direct bitmap background to be modified.
     */
    explicit sp_direct_bitmap_bg_painter(sp_direct_bitmap_bg_ptr&& bg) :
        _bg(move(bg))
    {
    }

    /**
     * @brief Returns the background modified by this painter.
     */
    [[nodiscard]] const sp_direct_bitmap_bg_ptr& bg() const
    {
        return _bg;
    }

    /**
     * @brief Returns a span of the hidden frame buffer.
     */
    [[nodiscard]] span<const color> page() const
    {
        auto page = reinterpret_cast<const color*>(_page);
        return span<const color>(page, _page_width * _page_height);
    }

    /**
     * @brief Returns a span of the hidden frame buffer.
     */
    [[nodiscard]] span<color> page()
    {
        auto page = reinterpret_cast<color*>(_page);
        return span<color>(page, _page_width * _page_height);
    }

    /**
     * @brief Fills the current page with the given color.
     */
    inline void fill(color color)
    {
        _bn::memory::unsafe_set_words(_dup16(color.data()), _page_size / 2, _page());
    }

    /**
     * @brief Returns the referenced color without bounds checking.
     * @param x Horizontal position in the current page [0..bitmap_bg::sp_direct_width()).
     * @param y Vertical position in the current page [0..bitmap_bg::sp_direct_height()).
     * @return The referenced color.
     */
    [[nodiscard]] inline color unsafe_get(int x, int y) const
    {
        return _page()[(y * _page_width) + x];
    }

    /**
     * @brief Returns the referenced color without bounds checking.
     * @param position Position in the current page.
     * @return The referenced color.
     */
    [[nodiscard]] inline color unsafe_get(const point& position) const
    {
        return unsafe_get(position.x(), position.y());
    }

    /**
     * @brief Returns the referenced color with bounds checking.
     * @param x Horizontal position in the current page [0..bitmap_bg::sp_direct_width()).
     * @param y Vertical position in the current page [0..bitmap_bg::sp_direct_height()).
     * @return The referenced color.
     */
    [[nodiscard]] inline optional<color> get(int x, int y) const
    {
        optional<color> result;

        if(x >= 0 && x < _page_width && y >= 0 && y < _page_height)
        {
            result = unsafe_get(x, y);
        }

        return result;
    }

    /**
     * @brief Returns the referenced color with bounds checking.
     * @param position Position in the current page.
     * @return The referenced color.
     */
    [[nodiscard]] inline optional<color> get(const point& position) const
    {
        return get(position.x(), position.y());
    }

    /**
     * @brief Plots a pixel on the current page without bounds checking.
     * @param x Horizontal position of the pixel to plot [0..bitmap_bg::sp_direct_width()).
     * @param y Vertical position of the pixel to plot [0..bitmap_bg::sp_direct_height()).
     * @param color Color to plot.
     */
    inline void unsafe_plot(int x, int y, color color)
    {
        _page()[(y * _page_width) + x] = color;
    }

    /**
     * @brief Plots a pixel on the current page without bounds checking.
     * @param position Position of the pixel to plot.
     * @param color Color to plot.
     */
    inline void unsafe_plot(const point& position, color color)
    {
        unsafe_plot(position.x(), position.y(), color);
    }

    /**
     * @brief Plots a pixel on the current page with bounds checking.
     * @param x Horizontal position of the pixel to plot [0..bitmap_bg::sp_direct_width()).
     * @param y Vertical position of the pixel to plot [0..bitmap_bg::sp_direct_height()).
     * @param color Color to plot.
     */
    inline void plot(int x, int y, color color)
    {
        if(x >= 0 && x < _page_width && y >= 0 && y < _page_height)
        {
            unsafe_plot(x, y, color);
        }
    }

    /**
     * @brief Plots a pixel on the current page with bounds checking.
     * @param position Position of the pixel to plot.
     * @param color Color to plot.
     */
    inline void plot(const point& position, color color)
    {
        plot(position.x(), position.y(), color);
    }

    /**
     * @brief Draws an horizontal line in the current page without bounds checking.
     * @param x1 Horizontal position of the first pixel [0..bitmap_bg::sp_direct_width()).
     * @param x2 Horizontal position of the last pixel [0..bitmap_bg::sp_direct_width()).
     * @param y Vertical position of the horizontal line [0..bitmap_bg::sp_direct_height()).
     * @param color Color to plot.
     */
    inline void unsafe_horizontal_line(int x1, int x2, int y, color color)
    {
        if(x2 < x1)
        {
            bn::swap(x1, x2);
        }

        bn::color* dst = _page() + (y * _page_width) + x1;
        _bn::memory::unsafe_set_half_words(color.data(), x2 - x1 + 1, dst);
    }

    /**
     * @brief Draws an horizontal line in the current page with bounds checking.
     * @param x1 Horizontal position of the first pixel [0..bitmap_bg::sp_direct_width()).
     * @param x2 Horizontal position of the last pixel [0..bitmap_bg::sp_direct_width()).
     * @param y Vertical position of the horizontal line [0..bitmap_bg::sp_direct_height()).
     * @param color Color to plot.
     */
    inline void horizontal_line(int x1, int x2, int y, color color)
    {
        if(y >= 0 && y < _page_height)
        {
            bool outside = (x1 < 0 && x2 < 0) || (x1 >= _page_width && x2 >= _page_width);

            if(! outside)
            {
                x1 = bn::clamp(x1, 0, _page_width - 1);
                x2 = bn::clamp(x2, 0, _page_width - 1);
                unsafe_horizontal_line(x1, x2, y, color);
            }
        }
    }

    /**
     * @brief Draws a vertical line in the current page without bounds checking.
     * @param x Horizontal position of the vertical line [0..bitmap_bg::sp_direct_width()).
     * @param y1 Vertical position of the first pixel [0..bitmap_bg::sp_direct_height()).
     * @param y2 Vertical position of the last pixel [0..bitmap_bg::sp_direct_height()).
     * @param color Color to plot.
     */
    inline void unsafe_vertical_line(int x, int y1, int y2, color color)
    {
        if(y2 < y1)
        {
            bn::swap(y1, y2);
        }

        bn::color* dst = _page() + (y1 * _page_width) + x;
        int height = y2 - y1 + 1;

        while(height--)
        {
            *dst = color;
            dst += _page_width;
        }
    }

    /**
     * @brief Draws a vertical line in the current page with bounds checking.
     * @param x Horizontal position of the vertical line [0..bitmap_bg::sp_direct_width()).
     * @param y1 Vertical position of the first pixel [0..bitmap_bg::sp_direct_height()).
     * @param y2 Vertical position of the last pixel [0..bitmap_bg::sp_direct_height()).
     * @param color Color to plot.
     */
    inline void vertical_line(int x, int y1, int y2, color color)
    {
        if(x >= 0 && x < _page_width)
        {
            bool outside = (y1 < 0 && y2 < 0) || (y1 >= _page_height && y2 >= _page_height);

            if(! outside)
            {
                y1 = bn::clamp(y1, 0, _page_height - 1);
                y2 = bn::clamp(y2, 0, _page_height - 1);
                unsafe_vertical_line(x, y1, y2, color);
            }
        }
    }

    /**
     * @brief Draws a line in the current page without bounds checking.
     * @param x1 Horizontal position of the first pixel [0..bitmap_bg::sp_direct_width()).
     * @param y1 Vertical position of the first pixel [0..bitmap_bg::sp_direct_height()).
     * @param x2 Horizontal position of the last pixel [0..bitmap_bg::sp_direct_width()).
     * @param y2 Vertical position of the last pixel [0..bitmap_bg::sp_direct_height()).
     * @param color Color to plot.
     */
    inline void unsafe_line(int x1, int y1, int x2, int y2, color color)
    {
        if(y1 == y2)
        {
            unsafe_horizontal_line(x1, x2, y1, color);
            return;
        }

        if(x1 == x2)
        {
            unsafe_vertical_line(x1, y1, y2, color);
            return;
        }

        // Normalization:
        int dx, xstep;

        if(x1 > x2)
        {
            xstep = -1;
            dx = x1 - x2;
        }
        else
        {
            xstep = 1;
            dx = x2 - x1;
        }

        int dy, ystep;

        if(y1 > y2)
        {
            ystep = -_page_width;
            dy = y1 - y2;
        }
        else
        {
            ystep = _page_width;
            dy = y2 - y1;
        }

        // Drawing:
        bn::color* dst = _page() + (y1 * _page_width) + x1;

        if(dx >= dy)
        {
            // Diagonal, slope <= 1:
            int dd = 2 * dy - dx;

            for(int ii = 0; ii <= dx; ++ii)
            {
                *dst = color;

                if(dd >= 0)
                {
                    dd -= 2 * dx;
                    dst += ystep;
                }

                dd += 2 * dy;
                dst += xstep;
            }
        }
        else
        {
            // Diagonal, slope > 1:
            int dd = 2 * dx - dy;

            for(int ii = 0; ii <= dy; ++ii)
            {
                *dst = color;

                if(dd >= 0)
                {
                    dd -= 2 * dy;
                    dst += xstep;
                }

                dd += 2 * dx;
                dst += ystep;
            }
        }
    }

    /**
     * @brief Draws a line in the current page without bounds checking.
     * @param first Position of the first pixel.
     * @param last Position of the last pixel.
     * @param color Color to plot.
     */
    inline void unsafe_line(const point& first, const point& last, color color)
    {
        unsafe_line(first.x(), first.y(), last.x(), last.y(), color);
    }

    /**
     * @brief Draws a line in the current page with bounds checking.
     * @param x1 Horizontal position of the first pixel [0..bitmap_bg::sp_direct_width()).
     * @param y1 Vertical position of the first pixel [0..bitmap_bg::sp_direct_height()).
     * @param x2 Horizontal position of the last pixel [0..bitmap_bg::sp_direct_width()).
     * @param y2 Vertical position of the last pixel [0..bitmap_bg::sp_direct_height()).
     * @param color Color to plot.
     */
    inline void line(int x1, int y1, int x2, int y2, color color)
    {
        optional<array<int, 4>> clip_result = clip_line(x1, y1, x2, y2, 0, 0, _page_width - 1, _page_height - 1);

        if(const array<int, 4>* clip_coords_ptr = clip_result.get())
        {
            const array<int, 4>& clip_coords = *clip_coords_ptr;
            unsafe_line(clip_coords[0], clip_coords[1], clip_coords[2], clip_coords[3], color);
        }
    }

    /**
     * @brief Draws a line in the current page with bounds checking.
     * @param first Position of the first pixel.
     * @param last Position of the last pixel.
     * @param color Color to plot.
     */
    inline void line(const point& first, const point& last, color color)
    {
        line(first.x(), first.y(), last.x(), last.y(), color);
    }

    /**
     * @brief Draws a filled rectangle in the current page without bounds checking.
     * @param x1 Horizontal position of the first corner [0..bitmap_bg::sp_direct_width()).
     * @param y1 Vertical position of the first corner [0..bitmap_bg::sp_direct_height()).
     * @param x2 Horizontal position of the last corner [0..bitmap_bg::sp_direct_width()).
     * @param y2 Vertical position of the last corner [0..bitmap_bg::sp_direct_height()).
     * @param color Color to plot.
     */
    inline void unsafe_rectangle(int x1, int y1, int x2, int y2, color color)
    {
        // Normalization:
        if(x2 < x1)
        {
            bn::swap(x1, x2);
        }

        if(y2 < y1)
        {
            bn::swap(y1, y2);
        }

        bn::color* dst = _page() + (y1 * _page_width) + x1;
        int width = x2 - x1 + 1;
        int height = y2 - y1 + 1;

        // Draw:
        while(height--)
        {
            _bn::memory::unsafe_set_half_words(color.data(), width, dst);
            dst += _page_width;
        }
    }

    /**
     * @brief Draws a filled rectangle in the current page without bounds checking.
     * @param first Position of the first corner.
     * @param last Position of the last corner.
     * @param color Color to plot.
     */
    inline void unsafe_rectangle(const point& first, const point& last, color color)
    {
        unsafe_rectangle(first.x(), first.y(), last.x(), last.y(), color);
    }

    /**
     * @brief Draws a filled rectangle in the current page with bounds checking.
     * @param x1 Horizontal position of the first corner [0..bitmap_bg::sp_direct_width()).
     * @param y1 Vertical position of the first corner [0..bitmap_bg::sp_direct_height()).
     * @param x2 Horizontal position of the last corner [0..bitmap_bg::sp_direct_width()).
     * @param y2 Vertical position of the last corner [0..bitmap_bg::sp_direct_height()).
     * @param color Color to plot.
     */
    inline void rectangle(int x1, int y1, int x2, int y2, color color)
    {
        bool outside =
                (x1 < 0 && x2 < 0) || (x1 >= _page_width && x2 >= _page_width) ||
                (y1 < 0 && y2 < 0) || (y1 >= _page_height && y2 >= _page_height);

        if(! outside)
        {
            x1 = bn::clamp(x1, 0, _page_width - 1);
            x2 = bn::clamp(x2, 0, _page_width - 1);
            y1 = bn::clamp(y1, 0, _page_height - 1);
            y2 = bn::clamp(y2, 0, _page_height - 1);
            unsafe_rectangle(x1, y1, x2, y2, color);
        }
    }

    /**
     * @brief Draws a filled rectangle in the current page with bounds checking.
     * @param first Position of the first corner.
     * @param last Position of the last corner.
     * @param color Color to plot.
     */
    inline void rectangle(const point& first, const point& last, color color)
    {
        rectangle(first.x(), first.y(), last.x(), last.y(), color);
    }

    /**
     * @brief Copies the given item to the current page without bounds checking. Transparent pixels are also copied,
     * so it should be faster than sp_direct_bitmap_bg_painter::unsafe_draw.
     * @param x Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).
     * @param y Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).
     * @param item Item to copy to the current page.
     */
    inline void unsafe_blit(int x, int y, const direct_bitmap_item& item)
    {
        BN_BASIC_ASSERT(item.compression() == compression_type::NONE, "Item is compressed");

        _blit_impl(x, y, 0, 0, item.dimensions().width(), item.dimensions().height(), item);
    }

    /**
     * @brief Copies the given item to the current page without bounds checking. Transparent pixels are also copied,
     * so it should be faster than sp_direct_bitmap_bg_painter::unsafe_draw.
     * @param position Position of the current page top-left corner.
     * @param item Item to copy to the current page.
     */
    inline void unsafe_blit(const point& position, const direct_bitmap_item& item)
    {
        unsafe_blit(position.x(), position.y(), item);
    }

    /**
     * @brief Copies the given region of interest to the current page without bounds checking.
     * Transparent pixels are also copied, so it should be faster than sp_direct_bitmap_bg_painter::unsafe_draw.
     * @param x Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).
     * @param y Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).
     * @param roi Region of interest to copy to the current page.
     */
    inline void unsafe_blit(int x, int y, const direct_bitmap_roi& roi)
    {
        const direct_bitmap_item& item_ref = roi.item_ref();
        BN_BASIC_ASSERT(item_ref.compression() == compression_type::NONE, "Item is compressed");

        _blit_impl(x, y, roi.x(), roi.y(), roi.width(), roi.height(), item_ref);
    }

    /**
     * @brief Copies the given region of interest to the current page without bounds checking.
     * Transparent pixels are also copied, so it should be faster than sp_direct_bitmap_bg_painter::unsafe_draw.
     * @param position Position of the current page top-left corner.
     * @param roi Region of interest to copy to the current page.
     */
    inline void unsafe_blit(const point& position, const direct_bitmap_roi& roi)
    {
        unsafe_blit(position.x(), position.y(), roi);
    }

    /**
     * @brief Copies the given item to the current page with bounds checking. Transparent pixels are also copied,
     * so it should be faster than sp_direct_bitmap_bg_painter::draw.
     * @param x Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).
     * @param y Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).
     * @param item Item to copy to the current page.
     */
    inline void blit(int x, int y, const direct_bitmap_item& item)
    {
        int item_x = 0;
        int item_y = 0;
        int blit_width = item.dimensions().width();
        int blit_height = item.dimensions().height();

        if(_crop(x, y, item_x, item_y, blit_width, blit_height))
        {
            BN_BASIC_ASSERT(item.compression() == compression_type::NONE, "Item is compressed");

            _blit_impl(x, y, item_x, item_y, blit_width, blit_height, item);
        }
    }

    /**
     * @brief Copies the given item to the current page with bounds checking. Transparent pixels are also copied,
     * so it should be faster than sp_direct_bitmap_bg_painter::draw.
     * @param position Position of the current page top-left corner.
     * @param item Item to copy to the current page.
     */
    inline void blit(const point& position, const direct_bitmap_item& item)
    {
        blit(position.x(), position.y(), item);
    }

    /**
     * @brief Copies the given region of interest to the current page with bounds checking.
     * Transparent pixels are also copied, so it should be faster than sp_direct_bitmap_bg_painter::draw.
     * @param x Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).
     * @param y Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).
     * @param roi Region of interest to copy to the current page.
     */
    inline void blit(int x, int y, const direct_bitmap_roi& roi)
    {
        int item_x = roi.x();
        int item_y = roi.y();
        int blit_width = roi.width();
        int blit_height = roi.height();

        if(_crop(x, y, item_x, item_y, blit_width, blit_height))
        {
            const direct_bitmap_item& item_ref = roi.item_ref();
            BN_BASIC_ASSERT(item_ref.compression() == compression_type::NONE, "Item is compressed");

            _blit_impl(x, y, item_x, item_y, blit_width, blit_height, item_ref);
        }
    }

    /**
     * @brief Copies the given region of interest to the current page with bounds checking.
     * Transparent pixels are also copied, so it should be faster than sp_direct_bitmap_bg_painter::draw.
     * @param position Position of the current page top-left corner.
     * @param roi Region of interest to copy to the current page.
     */
    inline void blit(const point& position, const direct_bitmap_roi& roi)
    {
        blit(position.x(), position.y(), roi);
    }

    /**
     * @brief Copies the given item to the current page without bounds checking. Transparent pixels are skipped,
     * so it should be slower than sp_direct_bitmap_bg_painter::unsafe_blit.
     * @param x Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).
     * @param y Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).
     * @param item Item to copy to the current page.
     * @param transparent_color Color of the transparent pixels.
     */
    inline void unsafe_draw(int x, int y, const direct_bitmap_item& item, bn::color transparent_color)
    {
        BN_BASIC_ASSERT(item.compression() == compression_type::NONE, "Item is compressed");

        _draw_impl(x, y, 0, 0, item.dimensions().width(), item.dimensions().height(), item, transparent_color);
    }

    /**
     * @brief Copies the given item to the current page without bounds checking. Transparent pixels are skipped,
     * so it should be slower than sp_direct_bitmap_bg_painter::unsafe_blit.
     * @param position Position of the current page top-left corner.
     * @param item Item to copy to the current page.
     * @param transparent_color Color of the transparent pixels.
     */
    inline void unsafe_draw(const point& position, const direct_bitmap_item& item, bn::color transparent_color)
    {
        unsafe_draw(position.x(), position.y(), item, transparent_color);
    }

    /**
     * @brief Copies the given region of interest to the current page without bounds checking.
     * Transparent pixels are skipped, so it should be slower than sp_direct_bitmap_bg_painter::unsafe_blit.
     * @param x Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).
     * @param y Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).
     * @param roi Region of interest to copy to the current page.
     * @param transparent_color Color of the transparent pixels.
     */
    inline void unsafe_draw(int x, int y, const direct_bitmap_roi& roi, bn::color transparent_color)
    {
        const direct_bitmap_item& item_ref = roi.item_ref();
        BN_BASIC_ASSERT(item_ref.compression() == compression_type::NONE, "Item is compressed");

        _draw_impl(x, y, roi.x(), roi.y(), roi.width(), roi.height(), item_ref, transparent_color);
    }

    /**
     * @brief Copies the given region of interest to the current page without bounds checking.
     * Transparent pixels are skipped, so it should be slower than sp_direct_bitmap_bg_painter::unsafe_blit.
     * @param position Position of the current page top-left corner.
     * @param roi Region of interest to copy to the current page.
     * @param transparent_color Color of the transparent pixels.
     */
    inline void unsafe_draw(const point& position, const direct_bitmap_roi& roi, bn::color transparent_color)
    {
        unsafe_draw(position.x(), position.y(), roi, transparent_color);
    }

    /**
     * @brief Copies the given item to the current page with bounds checking. Transparent pixels are skipped,
     * so it should be slower than sp_direct_bitmap_bg_painter::blit.
     * @param x Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).
     * @param y Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).
     * @param item Item to copy to the current page.
     * @param transparent_color Color of the transparent pixels.
     */
    inline void draw(int x, int y, const direct_bitmap_item& item, bn::color transparent_color)
    {
        int item_x = 0;
        int item_y = 0;
        int blit_width = item.dimensions().width();
        int blit_height = item.dimensions().height();

        if(_crop(x, y, item_x, item_y, blit_width, blit_height))
        {
            BN_BASIC_ASSERT(item.compression() == compression_type::NONE, "Item is compressed");

            _draw_impl(x, y, item_x, item_y, blit_width, blit_height, item, transparent_color);
        }
    }

    /**
     * @brief Copies the given item to the current page with bounds checking. Transparent pixels are skipped,
     * so it should be slower than sp_direct_bitmap_bg_painter::blit.
     * @param position Position of the current page top-left corner.
     * @param item Item to copy to the current page.
     * @param transparent_color Color of the transparent pixels.
     */
    inline void draw(const point& position, const direct_bitmap_item& item, bn::color transparent_color)
    {
        draw(position.x(), position.y(), item, transparent_color);
    }

    /**
     * @brief Copies the given region of interest to the current page with bounds checking.
     * Transparent pixels are skipped, so it should be slower than sp_direct_bitmap_bg_painter::blit.
     * @param x Horizontal position of the current page top-left corner [0..bitmap_bg::sp_direct_width()).
     * @param y Vertical position of the current page top-left corner [0..bitmap_bg::sp_direct_height()).
     * @param roi Region of interest to copy to the current page.
     * @param transparent_color Color of the transparent pixels.
     */
    inline void draw(int x, int y, const direct_bitmap_roi& roi, bn::color transparent_color)
    {
        int item_x = roi.x();
        int item_y = roi.y();
        int blit_width = roi.width();
        int blit_height = roi.height();

        if(_crop(x, y, item_x, item_y, blit_width, blit_height))
        {
            const direct_bitmap_item& item_ref = roi.item_ref();
            BN_BASIC_ASSERT(item_ref.compression() == compression_type::NONE, "Item is compressed");

            _draw_impl(x, y, item_x, item_y, blit_width, blit_height, item_ref, transparent_color);
        }
    }

    /**
     * @brief Copies the given region of interest to the current page with bounds checking.
     * Transparent pixels are skipped, so it should be slower than sp_direct_bitmap_bg_painter::blit.
     * @param position Position of the current page top-left corner.
     * @param roi Region of interest to copy to the current page.
     * @param transparent_color Color of the transparent pixels.
     */
    inline void draw(const point& position, const direct_bitmap_roi& roi, bn::color transparent_color)
    {
        draw(position.x(), position.y(), roi, transparent_color);
    }

private:
    static constexpr int _page_width = bitmap_bg::sp_direct_width();
    static constexpr int _page_height = bitmap_bg::sp_direct_height();
    static constexpr int _page_size = _page_width * _page_height;
    static constexpr int _half_page_width = _page_width / 2;
    static constexpr int _half_page_height = _page_height / 2;

    sp_direct_bitmap_bg_ptr _bg;

    [[nodiscard]] static inline color* _page()
    {
        return hw::bitmap_bg::sp_direct_vram();
    }

    [[nodiscard]] static inline int _dup16(int x)
    {
        return x | (x << 16);
    }

    [[nodiscard]] static inline bool _crop(
            int& x, int& y, int& item_x, int& item_y, int& blit_width, int& blit_height)
    {
        int x2 = x + blit_width - 1;
        int y2 = y + blit_height - 1;
        bool outside =
                (x < 0 && x2 < 0) || (x >= _page_width && x2 >= _page_width) ||
                (y < 0 && y2 < 0) || (y >= _page_height && y2 >= _page_height);

        if(outside)
        {
            return false;
        }

        if(x < 0)
        {
            item_x -= x;
            blit_width += x;
            x = 0;
        }

        int right = x + blit_width - _page_width;

        if(right > 0)
        {
            blit_width -= right;
        }

        if(y < 0)
        {
            item_y -= y;
            blit_height += y;
            y = 0;
        }

        int bottom = y + blit_height - _page_height;

        if(bottom > 0)
        {
            blit_height -= bottom;
        }

        return true;
    }

    inline void _blit_impl(int page_x, int page_y, int item_x, int item_y, int blit_width, int blit_height,
                           const direct_bitmap_item& item)
    {
        int item_width = item.dimensions().width();
        const bn::color* src = item.colors_ptr() + (item_y * item_width) + item_x;
        bn::color* dst = _page() + (page_y * _page_width) + page_x;

        while(blit_height--)
        {
            _bn::memory::unsafe_copy_half_words(src, blit_width, dst);
            src += item_width;
            dst += _page_width;
        }
    }

    inline void _draw_impl(int page_x, int page_y, int item_x, int item_y, int blit_width, int blit_height,
                           const direct_bitmap_item& item, bn::color transparent_color)
    {
        int item_width = item.dimensions().width();
        const bn::color* src = item.colors_ptr() + (item_y * item_width) + item_x;
        bn::color* dst = _page() + (page_y * _page_width) + page_x;

        while(blit_height--)
        {
            for(int x = 0; x < blit_width; ++x)
            {
                bn::color color = src[x];

                if(color != transparent_color)
                {
                    dst[x] = color;
                }
            }

            src += item_width;
            dst += _page_width;
        }
    }
};

}

#endif
