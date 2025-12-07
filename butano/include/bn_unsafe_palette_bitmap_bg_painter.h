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
    void clear()
    {
        _bn::memory::unsafe_clear_half_words((_page_width * _page_height) / 2, _page);
    }

    /**
     * @brief Plots a pixel on the current page.
     * @param x Horizontal position of the pixel to plot.
     * @param y Vertical position of the pixel to plot.
     * @param color_index Palette index of the color to plot.
     */
    void plot(int x, int y, uint8_t color_index)
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
     * @brief Draws a colored horizontal line in the current page.
     * @param x Horizontal position of the left pixel.
     * @param y Vertical position of the horizontal line.
     * @param width Horizontal line width in pixels.
     * @param color_index Palette index of the color to plot.
     */
    void horizontal_line(int x, int y, int width, uint8_t color_index)
    {
        auto dst_base = reinterpret_cast<uint8_t*>(_page);
        auto dst = reinterpret_cast<uint16_t*>(dst_base + (y * _page_width) + (x &~ 1));

        // Left unaligned pixel:
        if(x & 1)
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

private:
    static constexpr int _page_width = bitmap_bg::palette_width();
    static constexpr int _page_height = bitmap_bg::palette_height();

    palette_bitmap_bg_ptr _bg;
    uint16_t* _page;

    [[nodiscard]] static uint16_t _dup8(uint8_t x)
    {
        return x | (x << 8);
    }
};

}

#endif
