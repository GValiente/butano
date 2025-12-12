/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_PALETTE_BITMAP_BG_PAINTER_H
#define BN_PALETTE_BITMAP_BG_PAINTER_H

/**
 * @file
 * bn::palette_bitmap_bg_painter header file.
 *
 * @ingroup palette_bitmap_bg
 */

#include "bn_memory.h"
#include "bn_bitmap_bg.h"
#include "bn_clip_line.h"
#include "bn_palette_bitmap_bg_ptr.h"
#include "bn_palette_bitmap_pixels_item.h"

namespace bn
{

/**
 * @brief Performs low-level painting on palette bitmap backgrounds.
 *
 * Only one bitmap painter can be active at the same time.
 *
 * @ingroup palette_bitmap_bg
 */
class palette_bitmap_bg_painter
{

public:
    /**
     * @brief Class constructor.
     * @param bg Palette bitmap background to be modified.
     */
    explicit palette_bitmap_bg_painter(const palette_bitmap_bg_ptr& bg);

    /**
     * @brief Class constructor.
     * @param bg Palette bitmap background to be modified.
     */
    explicit palette_bitmap_bg_painter(palette_bitmap_bg_ptr&& bg);

    palette_bitmap_bg_painter(const palette_bitmap_bg_painter& other) = delete;

    palette_bitmap_bg_painter& operator=(const palette_bitmap_bg_painter& other) = delete;

    /**
     * @brief Move constructor.
     * @param other palette_bitmap_bg_painter to move.
     */
    palette_bitmap_bg_painter(palette_bitmap_bg_painter&& other) noexcept;

    /**
     * @brief Move assignment operator.
     * @param other palette_bitmap_bg_painter to move.
     * @return Reference to this.
     */
    palette_bitmap_bg_painter& operator=(palette_bitmap_bg_painter&& other) noexcept;

    /**
     * @brief Class destructor.
     */
    ~palette_bitmap_bg_painter();

    /**
     * @brief Returns the background modified by this painter.
     */
    [[nodiscard]] const palette_bitmap_bg_ptr& bg() const
    {
        return _bg;
    }

    /**
     * @brief Returns a pointer to the hidden frame buffer.
     */
    [[nodiscard]] const uint16_t* page() const
    {
        return _page;
    }

    /**
     * @brief Returns a pointer to the hidden frame buffer.
     */
    [[nodiscard]] uint16_t* page()
    {
        return _page;
    }

    /**
     * @brief Swaps the hidden frame buffer in the next core::update call.
     */
    void flip_page()
    {
        _bg.flip_page();
    }

    /**
     * @brief Swaps the hidden frame buffer without waiting to the next core::update call.
     *
     * Expect lack of vsync issues (screen tearing).
     */
    void flip_page_now();

    /**
     * @brief Fills the current page with the transparent color.
     */
    inline void clear()
    {
        _bn::memory::unsafe_clear_words(_page_size / 4, _page);
    }

    /**
     * @brief Fills the current page with the given color.
     * @param color_index Palette index of the color to fill the current page.
     */
    inline void fill(int color_index)
    {
        _bn::memory::unsafe_set_words(_quad8(color_index), _page_size / 4, _page);
    }

    /**
     * @brief Returns the referenced color palette index without bounds checking.
     * @param x Horizontal position of the color palette index [0..bitmap_bg::palette_width()).
     * @param y Vertical position of the color palette index [0..bitmap_bg::palette_height()).
     * @return The referenced color palette index.
     */
    [[nodiscard]] inline int unsafe_get(int x, int y) const
    {
        auto data = reinterpret_cast<const uint8_t*>(_page);
        return data[(y * _page_width) + x];
    }

    /**
     * @brief Returns the referenced color palette index with bounds checking.
     * @param x Horizontal position of the color palette index [0..bitmap_bg::palette_width()).
     * @param y Vertical position of the color palette index [0..bitmap_bg::palette_height()).
     * @return The referenced color palette index.
     */
    [[nodiscard]] inline optional<int> get(int x, int y) const
    {
        optional<int> result;

        if(x >= 0 && x < _page_width && y >= 0 && y < _page_height)
        {
            result = unsafe_get(x, y);
        }

        return result;
    }

    /**
     * @brief Plots a pixel on the current page without bounds checking.
     * @param x Horizontal position of the pixel to plot [0..bitmap_bg::palette_width()).
     * @param y Vertical position of the pixel to plot [0..bitmap_bg::palette_height()).
     * @param color_index Palette index of the color to plot.
     */
    inline void unsafe_plot(int x, int y, int color_index)
    {
        uint16_t* dst = _page + (((y * _page_width) + x) >> 1);

        if(x & 1)
        {
            *dst = (*dst & 0xFF) | (color_index << 8);
        }
        else
        {
            *dst = (*dst & ~0xFF) | color_index;
        }
    }

    /**
     * @brief Plots a pixel on the current page with bounds checking.
     * @param x Horizontal position of the pixel to plot [0..bitmap_bg::palette_width()).
     * @param y Vertical position of the pixel to plot [0..bitmap_bg::palette_height()).
     * @param color_index Palette index of the color to plot.
     */
    inline void plot(int x, int y, int color_index)
    {
        if(x >= 0 && x < _page_width && y >= 0 && y < _page_height)
        {
            unsafe_plot(x, y, color_index);
        }
    }

    /**
     * @brief Draws an horizontal line in the current page without bounds checking.
     * @param x1 Horizontal position of the first pixel [0..bitmap_bg::palette_width()).
     * @param x2 Horizontal position of the last pixel [0..bitmap_bg::palette_width()).
     * @param y Vertical position of the horizontal line [0..bitmap_bg::palette_height()).
     * @param color_index Color palette index.
     */
    inline void unsafe_horizontal_line(int x1, int x2, int y, int color_index)
    {
        if(x2 == x1)
        {
            unsafe_plot(x1, y, color_index);
            return;
        }

        if(x2 < x1)
        {
            bn::swap(x1, x2);
        }

        auto dst_base = reinterpret_cast<uint8_t*>(_page);
        auto dst = reinterpret_cast<uint16_t*>(dst_base + (y * _page_width) + (x1 &~ 1));
        int width = x2 - x1 + 1;

        // Left unaligned pixel:
        if(x1 & 1)
        {
            *dst = (*dst & 0xFF) + (color_index << 8);
            --width;
            ++dst;
        }

        // Right unaligned pixel:
        if(width & 1)
        {
            dst[width / 2] = (dst[width / 2] &~ 0xFF) + color_index;
        }

        width /= 2;

        // Aligned line:
        if(width)
        {
            _bn::memory::unsafe_set_half_words(_dup8(color_index), width, dst);
        }
    }

    /**
     * @brief Draws an horizontal line in the current page with bounds checking.
     * @param x1 Horizontal position of the first pixel [0..bitmap_bg::palette_width()).
     * @param x2 Horizontal position of the last pixel [0..bitmap_bg::palette_width()).
     * @param y Vertical position of the horizontal line [0..bitmap_bg::palette_height()).
     * @param color_index Color palette index.
     */
    inline void horizontal_line(int x1, int x2, int y, int color_index)
    {
        if(y >= 0 && y < _page_height)
        {
            bool outside = (x1 < 0 && x2 < 0) || (x1 >= _page_width && x2 >= _page_width);

            if(! outside)
            {
                x1 = bn::clamp(x1, 0, _page_width - 1);
                x2 = bn::clamp(x2, 0, _page_width - 1);
                unsafe_horizontal_line(x1, x2, y, color_index);
            }
        }
    }

    /**
     * @brief Draws a vertical line in the current page without bounds checking.
     * @param x Horizontal position of the vertical line [0..bitmap_bg::palette_width()).
     * @param y1 Vertical position of the first pixel [0..bitmap_bg::palette_height()).
     * @param y2 Vertical position of the last pixel [0..bitmap_bg::palette_height()).
     * @param color_index Color palette index.
     */
    inline void unsafe_vertical_line(int x, int y1, int y2, int color_index)
    {
        if(y2 == y1)
        {
            unsafe_plot(x, y1, color_index);
            return;
        }

        if(y2 < y1)
        {
            bn::swap(y1, y2);
        }

        auto dst_base = reinterpret_cast<uint8_t*>(_page);
        auto dst = reinterpret_cast<uint16_t*>(dst_base + (y1 * _page_width) + (x &~ 1));
        int height = y2 - y1 + 1;

        if(x & 1)
        {
            int clr = color_index << 8;

            while(height--)
            {
                *dst = (*dst & 0xFF) + clr;
                dst += _half_page_width;
            }
        }
        else
        {
            while(height--)
            {
                *dst = (*dst &~ 0xFF) + color_index;
                dst += _half_page_width;
            }
        }
    }

    /**
     * @brief Draws a vertical line in the current page with bounds checking.
     * @param x Horizontal position of the vertical line [0..bitmap_bg::palette_width()).
     * @param y1 Vertical position of the first pixel [0..bitmap_bg::palette_height()).
     * @param y2 Vertical position of the last pixel [0..bitmap_bg::palette_height()).
     * @param color_index Color palette index.
     */
    inline void vertical_line(int x, int y1, int y2, int color_index)
    {
        if(x >= 0 && x < _page_width)
        {
            bool outside = (y1 < 0 && y2 < 0) || (y1 >= _page_height && y2 >= _page_height);

            if(! outside)
            {
                y1 = bn::clamp(y1, 0, _page_height - 1);
                y2 = bn::clamp(y2, 0, _page_height - 1);
                unsafe_vertical_line(x, y1, y2, color_index);
            }
        }
    }

    /**
     * @brief Draws a line in the current page without bounds checking.
     * @param x1 Horizontal position of the first pixel [0..bitmap_bg::palette_width()).
     * @param y1 Vertical position of the first pixel [0..bitmap_bg::palette_height()).
     * @param x2 Horizontal position of the last pixel [0..bitmap_bg::palette_width()).
     * @param y2 Vertical position of the last pixel [0..bitmap_bg::palette_height()).
     * @param color_index Color palette index.
     */
    inline void unsafe_line(int x1, int y1, int x2, int y2, int color_index)
    {
        if(y1 == y2)
        {
            unsafe_horizontal_line(x1, x2, y1, color_index);
            return;
        }

        if(x1 == x2)
        {
            unsafe_vertical_line(x1, y1, y2, color_index);
            return;
        }

        auto dst_base = reinterpret_cast<uint8_t*>(_page);
        auto addr = reinterpret_cast<unsigned>(dst_base + (y1 * _page_width) + x1);
        unsigned mask = 255;
        unsigned clr = color_index & mask;
        clr |= clr << 8;

        if(x1 & 1)
        {
            mask = ~mask;
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
        if(dx >= dy)
        {
            // Diagonal, slope <= 1:
            int dd = (2 * dy) - dx;

            for(int ii = dx; ii >= 0; --ii)
            {
                auto dst_l = reinterpret_cast<uint16_t*>(addr - (mask >> 31));
                *dst_l = (*dst_l &~ mask) | (clr & mask);

                if(dd >= 0)
                {
                    dd -= 2 * dx;
                    addr += ystep;
                }

                dd += 2 * dy;
                addr += xstep;
                mask = ~mask;
            }
        }
        else
        {
            // Diagonal, slope > 1:
            int dd = (2 * dx) - dy;

            for(int ii = dy; ii >= 0; --ii)
            {
                auto dst_l = reinterpret_cast<uint16_t*>(addr - (mask >> 31));
                *dst_l = (*dst_l &~ mask) | (clr & mask);

                if(dd >= 0)
                {
                    dd -= 2 * dy;
                    addr += xstep;
                    mask = ~mask;
                }

                dd += 2 * dx;
                addr += ystep;
            }
        }
    }

    /**
     * @brief Draws a line in the current page with bounds checking.
     * @param x1 Horizontal position of the first pixel [0..bitmap_bg::palette_width()).
     * @param y1 Vertical position of the first pixel [0..bitmap_bg::palette_height()).
     * @param x2 Horizontal position of the last pixel [0..bitmap_bg::palette_width()).
     * @param y2 Vertical position of the last pixel [0..bitmap_bg::palette_height()).
     * @param color_index Color palette index.
     */
    inline void line(int x1, int y1, int x2, int y2, int color_index)
    {
        optional<array<int, 4>> clip_result = clip_line(x1, y1, x2, y2, 0, 0, _page_width - 1, _page_height - 1);

        if(const array<int, 4>* clip_coords_ptr = clip_result.get())
        {
            const array<int, 4>& clip_coords = *clip_coords_ptr;
            unsafe_line(clip_coords[0], clip_coords[1], clip_coords[2], clip_coords[3], color_index);
        }
    }

    /**
     * @brief Draws a filled rectangle in the current page without bounds checking.
     * @param x1 Horizontal position of the first corner [0..bitmap_bg::palette_width()).
     * @param y1 Vertical position of the first corner [0..bitmap_bg::palette_height()).
     * @param x2 Horizontal position of the last corner [0..bitmap_bg::palette_width()).
     * @param y2 Vertical position of the last corner [0..bitmap_bg::palette_height()).
     * @param color_index Color palette index.
     */
    inline void unsafe_rectangle(int x1, int y1, int x2, int y2, int color_index)
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

        unsigned width = x2 - x1;
        unsigned height = y2 - y1;
        auto dst_base = reinterpret_cast<uint8_t*>(_page);
        auto dst = reinterpret_cast<uint16_t*>(dst_base + (y1 * _page_width) + (x1 &~ 1));

        // Unaligned left:
        if(x1 & 1)
        {
            uint16_t* dst_l = dst;

            for(unsigned iy = 0; iy < height; ++iy)
            {
                *dst_l = (*dst_l & 0xFF) | (color_index << 8);
                dst_l += _page_width / 2;
            }

            --width;
            ++dst;
        }

        // Unaligned right:
        if(width & 1)
        {
            uint16_t* dst_l = dst + (width / 2);

            for(unsigned iy = 0; iy < height; ++iy)
            {
                *dst_l = (*dst_l &~ 0xFF) | color_index;
                dst_l += _page_width / 2;
            }

            --width;
        }

        if(width)
        {
            // Center:
            uint16_t* dst_l = dst;
            color_index += color_index << 8;

            for(unsigned iy = 0; iy < height; ++iy)
            {
                _bn::memory::unsafe_set_half_words(color_index, width / 2, dst_l);
                dst_l += _page_width / 2;
            }
        }
    }

    /**
     * @brief Draws a filled rectangle in the current page with bounds checking.
     * @param x1 Horizontal position of the first corner [0..bitmap_bg::palette_width()).
     * @param y1 Vertical position of the first corner [0..bitmap_bg::palette_height()).
     * @param x2 Horizontal position of the last corner [0..bitmap_bg::palette_width()).
     * @param y2 Vertical position of the last corner [0..bitmap_bg::palette_height()).
     * @param color_index Color palette index.
     */
    inline void rectangle(int x1, int y1, int x2, int y2, int color_index)
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
            unsafe_rectangle(x1, y1, x2, y2, color_index);
        }
    }

    inline void unsafe_blit(const palette_bitmap_pixels_item& item, int x, int y)
    {
        BN_BASIC_ASSERT(item.compression() == compression_type::NONE, "Item is compressed");

        _blit_impl(item, x, y, 0, 0, item.dimensions().width(), item.dimensions().height());
    }

    inline void blit(const palette_bitmap_pixels_item& item, int x, int y)
    {
        BN_BASIC_ASSERT(item.compression() == compression_type::NONE, "Item is compressed");

        int item_x = 0;
        int item_y = 0;
        int blit_width = item.dimensions().width();
        int blit_height = item.dimensions().height();
        _blit_impl(item, x, y, item_x, item_y, blit_width, blit_height);
    }

private:
    static constexpr int _page_width = bitmap_bg::palette_width();
    static constexpr int _page_height = bitmap_bg::palette_height();
    static constexpr int _page_size = _page_width * _page_height;
    static constexpr int _half_page_width = _page_width / 2;
    static constexpr int _half_page_height = _page_height / 2;

    palette_bitmap_bg_ptr _bg;
    uint16_t* _page;

    [[nodiscard]] static inline int _dup8(int x)
    {
        return x | (x << 8);
    }

    [[nodiscard]] static inline int _quad8(int x)
    {
        return x * 0x01010101;
    }

    inline void _blit_impl(const palette_bitmap_pixels_item& item, int page_x, int page_y, int item_x, int item_y,
                           int blit_width, int blit_height)
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
};

}

#endif
