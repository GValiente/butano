/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DP_DIRECT_BITMAP_BG_PAINTER_H
#define BN_DP_DIRECT_BITMAP_BG_PAINTER_H

/**
 * @file
 * bn::dp_direct_bitmap_bg_painter header file.
 *
 * @ingroup dp_direct_bitmap_bg
 */

#include "bn_point.h"
#include "bn_memory.h"
#include "bn_bitmap_bg.h"
#include "bn_clip_line.h"
#include "bn_palette_bitmap_roi.h"
#include "bn_dp_direct_bitmap_bg_ptr.h"

#include "bn_color.h"

namespace bn
{

/**
 * @brief Performs low-level painting on dual page direct bitmap backgrounds.
 *
 * Only one bitmap painter can be active at the same time.
 *
 * @ingroup dp_direct_bitmap_bg
 */
class dp_direct_bitmap_bg_painter
{

public:
    /**
     * @brief Class constructor.
     * @param bg Dual page direct bitmap background to be modified.
     */
    explicit dp_direct_bitmap_bg_painter(const dp_direct_bitmap_bg_ptr& bg);

    /**
     * @brief Class constructor.
     * @param bg Dual page direct bitmap background to be modified.
     */
    explicit dp_direct_bitmap_bg_painter(dp_direct_bitmap_bg_ptr&& bg);

    dp_direct_bitmap_bg_painter(const dp_direct_bitmap_bg_painter& other) = delete;

    dp_direct_bitmap_bg_painter& operator=(const dp_direct_bitmap_bg_painter& other) = delete;

    /**
     * @brief Move constructor.
     * @param other dp_direct_bitmap_bg_painter to move.
     */
    dp_direct_bitmap_bg_painter(dp_direct_bitmap_bg_painter&& other) noexcept;

    /**
     * @brief Move assignment operator.
     * @param other dp_direct_bitmap_bg_painter to move.
     * @return Reference to this.
     */
    dp_direct_bitmap_bg_painter& operator=(dp_direct_bitmap_bg_painter&& other) noexcept;

    /**
     * @brief Class destructor.
     */
    ~dp_direct_bitmap_bg_painter();

    /**
     * @brief Returns the background modified by this painter.
     */
    [[nodiscard]] const dp_direct_bitmap_bg_ptr& bg() const
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
     * @brief Swaps the hidden frame buffer in the next core::update call.
     */
    void flip_page_later()
    {
        _bg.flip_page_later();
    }

    /**
     * @brief Swaps the hidden frame buffer without waiting to the next core::update call.
     *
     * Expect lack of vsync issues like screen tearing.
     */
    void flip_page_now();

    /**
     * @brief Fills the current page with the given color.
     */
    inline void fill(color color)
    {
        _bn::memory::unsafe_set_words(_dup16(color.data()), _page_size / 2, _page);
    }

    /**
     * @brief Returns the referenced color without bounds checking.
     * @param x Horizontal position in the current page [0..bitmap_bg::dp_direct_width()).
     * @param y Vertical position in the current page [0..bitmap_bg::dp_direct_height()).
     * @return The referenced color.
     */
    [[nodiscard]] inline color unsafe_get(int x, int y) const
    {
        return _page[(y * _page_width) + x];
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
     * @param x Horizontal position in the current page [0..bitmap_bg::dp_direct_width()).
     * @param y Vertical position in the current page [0..bitmap_bg::dp_direct_height()).
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
     * @param x Horizontal position of the pixel to plot [0..bitmap_bg::dp_direct_width()).
     * @param y Vertical position of the pixel to plot [0..bitmap_bg::dp_direct_height()).
     * @param color Color to plot.
     */
    inline void unsafe_plot(int x, int y, color color)
    {
        _page[(y * _page_width) + x] = color;
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
     * @param x Horizontal position of the pixel to plot [0..bitmap_bg::dp_direct_width()).
     * @param y Vertical position of the pixel to plot [0..bitmap_bg::dp_direct_height()).
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
     * @param x1 Horizontal position of the first pixel [0..bitmap_bg::dp_direct_width()).
     * @param x2 Horizontal position of the last pixel [0..bitmap_bg::dp_direct_width()).
     * @param y Vertical position of the horizontal line [0..bitmap_bg::dp_direct_height()).
     * @param color Color to plot.
     */
    inline void unsafe_horizontal_line(int x1, int x2, int y, color color)
    {
        if(x2 < x1)
        {
            bn::swap(x1, x2);
        }

        bn::color* dst = _page + (y * _page_width) + x1;
        _bn::memory::unsafe_set_half_words(color.data(), x2 - x1 + 1, dst);
    }

    /**
     * @brief Draws an horizontal line in the current page with bounds checking.
     * @param x1 Horizontal position of the first pixel [0..bitmap_bg::dp_direct_width()).
     * @param x2 Horizontal position of the last pixel [0..bitmap_bg::dp_direct_width()).
     * @param y Vertical position of the horizontal line [0..bitmap_bg::dp_direct_height()).
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
     * @param x Horizontal position of the vertical line [0..bitmap_bg::dp_direct_width()).
     * @param y1 Vertical position of the first pixel [0..bitmap_bg::dp_direct_height()).
     * @param y2 Vertical position of the last pixel [0..bitmap_bg::dp_direct_height()).
     * @param color Color to plot.
     */
    inline void unsafe_vertical_line(int x, int y1, int y2, color color)
    {
        if(y2 < y1)
        {
            bn::swap(y1, y2);
        }

        bn::color* dst = _page + (y1 * _page_width) + x;
        int height = y2 - y1 + 1;

        while(height--)
        {
            *dst = color;
            dst += _page_width;
        }
    }

    /**
     * @brief Draws a vertical line in the current page with bounds checking.
     * @param x Horizontal position of the vertical line [0..bitmap_bg::dp_direct_width()).
     * @param y1 Vertical position of the first pixel [0..bitmap_bg::dp_direct_height()).
     * @param y2 Vertical position of the last pixel [0..bitmap_bg::dp_direct_height()).
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
     * @param x1 Horizontal position of the first pixel [0..bitmap_bg::dp_direct_width()).
     * @param y1 Vertical position of the first pixel [0..bitmap_bg::dp_direct_height()).
     * @param x2 Horizontal position of the last pixel [0..bitmap_bg::dp_direct_width()).
     * @param y2 Vertical position of the last pixel [0..bitmap_bg::dp_direct_height()).
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
        bn::color* dst = _page + (y1 * _page_width) + x1;

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
     * @param x1 Horizontal position of the first pixel [0..bitmap_bg::dp_direct_width()).
     * @param y1 Vertical position of the first pixel [0..bitmap_bg::dp_direct_height()).
     * @param x2 Horizontal position of the last pixel [0..bitmap_bg::dp_direct_width()).
     * @param y2 Vertical position of the last pixel [0..bitmap_bg::dp_direct_height()).
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
     * @param x1 Horizontal position of the first corner [0..bitmap_bg::dp_direct_width()).
     * @param y1 Vertical position of the first corner [0..bitmap_bg::dp_direct_height()).
     * @param x2 Horizontal position of the last corner [0..bitmap_bg::dp_direct_width()).
     * @param y2 Vertical position of the last corner [0..bitmap_bg::dp_direct_height()).
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

        bn::color* dst = _page + (y1 * _page_width) + x1;
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
     * @param x1 Horizontal position of the first corner [0..bitmap_bg::dp_direct_width()).
     * @param y1 Vertical position of the first corner [0..bitmap_bg::dp_direct_height()).
     * @param x2 Horizontal position of the last corner [0..bitmap_bg::dp_direct_width()).
     * @param y2 Vertical position of the last corner [0..bitmap_bg::dp_direct_height()).
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

private:
    static constexpr int _page_width = bitmap_bg::dp_direct_width();
    static constexpr int _page_height = bitmap_bg::dp_direct_height();
    static constexpr int _page_size = _page_width * _page_height;
    static constexpr int _half_page_width = _page_width / 2;
    static constexpr int _half_page_height = _page_height / 2;

    dp_direct_bitmap_bg_ptr _bg;
    color* _page;

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
                           const palette_bitmap_pixels_item& item)
    {
        int item_width = item.dimensions().width();
        const uint8_t* src = item.pixels_ptr() + (item_y * item_width) + item_x;
        uint8_t* dst = reinterpret_cast<uint8_t*>(_page) + (page_y * _page_width) + page_x;

        while(blit_height--)
        {
            _bn::memory::unsafe_copy_bytes_vram(src, blit_width, dst);
            src += item_width;
            dst += _page_width;
        }
    }

    inline void _draw_impl(int page_x, int page_y, int item_x, int item_y, int blit_width, int blit_height,
                           const palette_bitmap_pixels_item& item)
    {
        auto dst_base = reinterpret_cast<uint8_t*>(_page);
        auto dst = reinterpret_cast<uint16_t*>(dst_base + (page_y * _page_width) + (page_x &~ 1));
        int item_width = item.dimensions().width();

        // Unaligned left:
        if(page_x & 1)
        {
            const uint8_t* src = item.pixels_ptr() + (item_y * item_width) + item_x;
            uint16_t* dst_l = dst;

            for(int iy = 0; iy < blit_height; ++iy)
            {
                if(uint8_t src_x = *src)
                {
                    *dst_l = (*dst_l & 0xFF) | (src_x << 8);
                }

                src += item_width;
                dst_l += _page_width / 2;
            }

            ++item_x;
            --blit_width;
            ++dst;
        }

        // Unaligned right:
        if(blit_width & 1)
        {
            const uint8_t* src = item.pixels_ptr() + (item_y * item_width) + item_x + blit_width - 1;
            uint16_t* dst_l = dst + (blit_width / 2);

            for(int iy = 0; iy < blit_height; ++iy)
            {
                if(uint8_t src_x = *src)
                {
                    *dst_l = (*dst_l &~ 0xFF) | src_x;
                }

                src += item_width;
                dst_l += _page_width / 2;
            }

            --blit_width;
        }

        if(blit_width)
        {
            // Center:
            const uint8_t* src = item.pixels_ptr() + (item_y * item_width) + item_x;
            uint16_t* dst_l = dst;
            int ixl = blit_width / 2;

            for(int iy = 0; iy < blit_height; ++iy)
            {
                const uint8_t* src_x = src;
                uint16_t* dst_x = dst_l;

                for(int ix = 0; ix < ixl; ++ix)
                {
                    uint8_t src_a = *src_x;
                    ++src_x;

                    uint8_t src_b = *src_x;
                    ++src_x;

                    uint8_t dst_a = src_a ? src_a : *dst_x & 0xFF;
                    uint8_t dst_b = src_b ? src_b : *dst_x >> 8;
                    *dst_x = dst_a | (dst_b << 8);
                    ++dst_x;
                }

                src += item_width;
                dst_l += _page_width / 2;
            }
        }
    }
};

}

#endif
