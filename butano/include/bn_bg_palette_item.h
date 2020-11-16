/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BG_PALETTE_ITEM_H
#define BTN_BG_PALETTE_ITEM_H

/**
 * @file
 * btn::bg_palette_item header file.
 *
 * @ingroup bg
 * @ingroup palette
 * @ingroup tool
 */

#include "btn_span.h"
#include "btn_color.h"
#include "btn_optional_fwd.h"
#include "btn_palette_bpp_mode.h"

namespace btn
{

class bg_palette_ptr;

/**
 * @brief Contains the required information to generate background color palettes.
 *
 * The assets conversion tools generate an object of this type in the build folder for each *.bmp file
 * with `regular_bg` type.
 *
 * The colors are not copied but referenced, so they should outlive the bg_palette_item
 * to avoid dangling references.
 *
 * @ingroup bg
 * @ingroup palette
 * @ingroup tool
 */
class bg_palette_item
{

public:
    /**
     * @brief Constructor.
     * @param colors_ref Reference to an array of multiples of 16 colors.
     *
     * The colors are not copied but referenced, so they should outlive the bg_palette_item
     * to avoid dangling references.
     *
     * @param bpp_mode Bits per pixel of the color palettes to create.
     */
    constexpr bg_palette_item(const span<const color>& colors_ref, palette_bpp_mode bpp_mode) :
        _colors_ref(colors_ref),
        _bpp_mode(bpp_mode)
    {
        BTN_ASSERT(colors_ref.size() >= 16 && colors_ref.size() <= 256 && colors_ref.size() % 16 == 0,
                   "Invalid colors count: ", colors_ref.size());
    }

    /**
     * @brief Returns the referenced array of multiples of 16 colors.
     *
     * The colors are not copied but referenced, so they should outlive the bg_palette_item
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr const span<const color>& colors_ref() const
    {
        return _colors_ref;
    }

    /**
     * @brief Returns the bits per pixel of the color palettes to create.
     */
    [[nodiscard]] constexpr palette_bpp_mode bpp_mode() const
    {
        return _bpp_mode;
    }

    /**
     * @brief Searches for a bg_palette_ptr which contains the colors referenced by this item.
     * @return bg_palette_ptr which contains the colors referenced by this item if it has been found;
     * `nullopt` otherwise.
     */
    [[nodiscard]] optional<bg_palette_ptr> find_palette() const;

    /**
     * @brief Searches for a bg_palette_ptr which contains the colors referenced by this item.
     * If it is not found, it creates a bg_palette_ptr which contains them.
     * @return bg_palette_ptr which contains the colors referenced by this item if it has been found;
     * otherwise it returns a new bg_palette_ptr which contains them.
     */
    [[nodiscard]] bg_palette_ptr create_palette() const;

    /**
     * @brief Creates a new bg_palette_ptr which contains the colors referenced by this item.
     * @return bg_palette_ptr which contains the colors referenced by this item.
     */
    [[nodiscard]] bg_palette_ptr create_new_palette() const;

    /**
     * @brief Searches for a bg_palette_ptr which contains the colors referenced by this item.
     * If it is not found, it creates a bg_palette_ptr which contains them.
     * @return bg_palette_ptr which contains the colors referenced by this item if it has been found;
     * otherwise it returns a new bg_palette_ptr which contains them it if it could be allocated;
     * `nullopt` otherwise.
     */
    [[nodiscard]] optional<bg_palette_ptr> create_palette_optional() const;

    /**
     * @brief Creates a new bg_palette_ptr which contains the colors referenced by this item.
     * @return bg_palette_ptr which contains the colors referenced by this item it if it could be allocated;
     * `nullopt` otherwise.
     */
    [[nodiscard]] optional<bg_palette_ptr> create_new_palette_optional() const;

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const bg_palette_item& a, const bg_palette_item& b) = default;

private:
    span<const color> _colors_ref;
    palette_bpp_mode _bpp_mode;
};

}

#endif

