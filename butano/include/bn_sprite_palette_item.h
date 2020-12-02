/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_PALETTE_ITEM_H
#define BN_SPRITE_PALETTE_ITEM_H

/**
 * @file
 * bn::sprite_palette_item header file.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup tool
 */

#include "bn_span.h"
#include "bn_color.h"
#include "bn_bpp_mode.h"
#include "bn_alignment.h"
#include "bn_optional_fwd.h"

namespace bn
{

class sprite_palette_ptr;

/**
 * @brief Contains the required information to generate sprite color palettes.
 *
 * The assets conversion tools generate an object of this type in the build folder for each *.bmp file
 * with `sprite` type.
 *
 * The colors are not copied but referenced, so they should outlive the sprite_palette_item
 * to avoid dangling references.
 *
 * @ingroup sprite
 * @ingroup palette
 * @ingroup tool
 */
class sprite_palette_item
{

public:
    /**
     * @brief Constructor.
     * @param colors_ref Reference to an array of multiples of 16 colors.
     *
     * The colors are not copied but referenced, so they should outlive the sprite_palette_item
     * to avoid dangling references.
     *
     * @param bpp Bits per pixel of the color palettes to create.
     */
    constexpr sprite_palette_item(const span<const color>& colors_ref, bpp_mode bpp) :
        _colors_ref(colors_ref),
        _bpp(bpp)
    {
        BN_ASSERT(aligned<alignof(int)>(colors_ref.data()), "Colors are not aligned");
        BN_ASSERT((bpp == bpp_mode::BPP_4 && colors_ref.size() == 16) ||
                  (bpp == bpp_mode::BPP_8 && colors_ref.size() >= 16 && colors_ref.size() <= 256 &&
                        colors_ref.size() % 16 == 0),
                  "Invalid colors count: ", colors_ref.size());
    }

    /**
     * @brief Returns the referenced array of multiples of 16 colors.
     *
     * The colors are not copied but referenced, so they should outlive the sprite_palette_item
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr const span<const color>& colors_ref() const
    {
        return _colors_ref;
    }

    /**
     * @brief Returns the bits per pixel of the color palettes to create.
     */
    [[nodiscard]] constexpr bpp_mode bpp() const
    {
        return _bpp;
    }

    /**
     * @brief Searches for a sprite_palette_ptr which contains the colors referenced by this item.
     * @return sprite_palette_ptr which contains the colors referenced by this item if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<sprite_palette_ptr> find_palette() const;

    /**
     * @brief Searches for a sprite_palette_ptr which contains the colors referenced by this item.
     * If it is not found, it creates a sprite_palette_ptr which contains them.
     * @return sprite_palette_ptr which contains the colors referenced by this item if it has been found;
     * otherwise it returns a new sprite_palette_ptr which contains them.
     */
    [[nodiscard]] sprite_palette_ptr create_palette() const;

    /**
     * @brief Creates a new sprite_palette_ptr which contains the colors referenced by this item.
     * @return sprite_palette_ptr which contains the colors referenced by this item.
     */
    [[nodiscard]] sprite_palette_ptr create_new_palette() const;

    /**
     * @brief Searches for a sprite_palette_ptr which contains the colors referenced by this item.
     * If it is not found, it creates a sprite_palette_ptr which contains them.
     * @return sprite_palette_ptr which contains the colors referenced by this item if it has been found;
     * otherwise it returns a new sprite_palette_ptr which contains them it if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<sprite_palette_ptr> create_palette_optional() const;

    /**
     * @brief Creates a new sprite_palette_ptr which contains the colors referenced by this item.
     * @return sprite_palette_ptr which contains the colors referenced by this item it if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] optional<sprite_palette_ptr> create_new_palette_optional() const;

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const sprite_palette_item& a,
                                                   const sprite_palette_item& b) = default;

private:
    span<const color> _colors_ref;
    bpp_mode _bpp;
};

}

#endif

