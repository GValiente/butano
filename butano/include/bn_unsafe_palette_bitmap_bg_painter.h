/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_UNSAFE_PALETTE_BITMAP_BG_PAINTER_H
#define BN_UNSAFE_PALETTE_BITMAP_BG_PAINTER_H

/**
 * @file
 * bn::unsafe_palette_bitmap_bg_painter header file.
 *
 * @ingroup palette_bitmap_bg
 */

#include "bn_memory.h"
#include "bn_bitmap_bg.h"
#include "bn_palette_bitmap_bg_ptr.h"

namespace bn
{

/**
 * @brief Performs low-level painting on palette bitmap backgrounds without validating the input parameters.
 *
 * Only one bitmap painter can be active at the same time.
 *
 * @ingroup palette_bitmap_bg
 */
class unsafe_palette_bitmap_bg_painter
{

public:
    /**
     * @brief Class constructor.
     * @param bg Palette bitmap background to be modified.
     */
    explicit unsafe_palette_bitmap_bg_painter(const palette_bitmap_bg_ptr& bg);

    /**
     * @brief Class constructor.
     * @param bg Palette bitmap background to be modified.
     */
    explicit unsafe_palette_bitmap_bg_painter(palette_bitmap_bg_ptr&& bg);

    unsafe_palette_bitmap_bg_painter(const unsafe_palette_bitmap_bg_painter& other) = delete;

    unsafe_palette_bitmap_bg_painter& operator=(const unsafe_palette_bitmap_bg_painter& other) = delete;

    /**
     * @brief Move constructor.
     * @param other unsafe_palette_bitmap_bg_painter to move.
     */
    unsafe_palette_bitmap_bg_painter(unsafe_palette_bitmap_bg_painter&& other) noexcept;

    /**
     * @brief Move assignment operator.
     * @param other unsafe_palette_bitmap_bg_painter to move.
     * @return Reference to this.
     */
    unsafe_palette_bitmap_bg_painter& operator=(unsafe_palette_bitmap_bg_painter&& other) noexcept;

    /**
     * @brief Class destructor.
     */
    ~unsafe_palette_bitmap_bg_painter();

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
     * Expect lack of vsync issues.
     */
    void flip_page_now();

    /**
     * @brief Fills the current page with the transparent color.
     */
    inline void clear()
    {
        _bn::memory::unsafe_clear_half_words(_page_size / 2, _page);
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
     * @brief Plots a pixel on the current page.
     * @param x Horizontal position of the pixel to plot.
     * @param y Vertical position of the pixel to plot.
     * @param color_index Palette index of the color to plot.
     */
    inline void plot(int x, int y, int color_index)
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
     * @brief Draws an horizontal line in the current page.
     * @param x1 Horizontal position of the first pixel.
     * @param x2 Horizontal position of the last pixel.
     * @param y Vertical position of the horizontal line.
     * @param color_index Color palette index.
     */
    inline void horizontal_line(int x1, int x2, int y, int color_index)
    {
        if(x2 == x1)
        {
            plot(x1, y, color_index);
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
     * @brief Draws a vertical line in the current page.
     * @param x Horizontal position of the vertical line.
     * @param y1 Vertical position of the first pixel.
     * @param y2 Vertical position of the last pixel.
     * @param color_index Color palette index.
     */
    inline void vertical_line(int x, int y1, int y2, int color_index)
    {
        if(y2 == y1)
        {
            plot(x, y1, color_index);
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

    inline void line(int x1, int y1, int x2, int y2, int color_index)
    {
        if(y1 == y2)
        {
            horizontal_line(x1, x2, y1, color_index);
            return;
        }

        if(x1 == x2)
        {
            vertical_line(x1, y1, y2, color_index);
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

            for(int ii = dx; ii >= 0; ii--)
            {
                auto dstL = reinterpret_cast<uint16_t*>(addr - (mask >> 31));
                *dstL = (*dstL &~ mask) | (clr & mask);

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

            for(int ii = dy; ii >= 0; ii--)
            {
                auto dstL = reinterpret_cast<uint16_t*>(addr - (mask >> 31));
                *dstL = (*dstL &~ mask) | (clr & mask);

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
};

}

#endif
