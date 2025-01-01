/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_FONT_H
#define BN_SPRITE_FONT_H

/**
 * @file
 * bn::sprite_font header file.
 *
 * @ingroup sprite
 * @ingroup text
 */

#include "bn_sprite_item.h"
#include "bn_utf8_characters_map_ref.h"

namespace bn
{

/**
 * @brief Contains the required information to generate text sprites.
 *
 * Currently, it supports 4 bits per pixel (16 colors) fixed width AND variable width characters.
 *
 * Also, UTF-8 characters are supported.
 *
 * UTF-8 characters map and character widths are not copied but referenced,
 * so they should outlive the sprite_font to avoid dangling references.
 *
 * @ingroup sprite
 * @ingroup text
 */
class sprite_font
{

public:
    static constexpr int minimum_graphics = 94; //!< Minimum number of sprite tile sets that must contain the sprite_tiles_item of a sprite_font.

    /**
     * @brief Constructor.
     * @param item sprite_item used to generate text sprites.
     */
    constexpr explicit sprite_font(const sprite_item& item) :
        sprite_font(item, utf8_characters_map_ref(), span<const int8_t>(), 0)
    {
    }

    /**
     * @brief Constructor.
     * @param item sprite_item used to generate text sprites.
     * @param utf8_characters_ref Reference to the map of the supported UTF-8 characters
     * and their position in the tile sets.
     *
     * UTF-8 characters should appear in the tile sets referenced by item just after ASCII characters.
     *
     * UTF-8 characters map is not copied but referenced, so it should outlive the sprite_font
     * to avoid dangling references.
     */
    constexpr sprite_font(const sprite_item& item, const utf8_characters_map_ref& utf8_characters_ref) :
        sprite_font(item, utf8_characters_ref, span<const int8_t>(), 0)
    {
    }

    /**
     * @brief Constructor.
     * @param item sprite_item used to generate text sprites.
     * @param utf8_characters_ref Reference to the map of the supported UTF-8 characters
     * and their position in the tile sets.
     *
     * UTF-8 characters should appear in the tile sets referenced by item just after ASCII characters.
     *
     * UTF-8 characters map is not copied but referenced, so it should outlive the sprite_font
     * to avoid dangling references.
     *
     * @param character_widths_ref Reference to the width in pixels of each supported character.
     *
     * Character widths are not copied but referenced, so they should outlive the sprite_font
     * to avoid dangling references.
     */
    constexpr sprite_font(const sprite_item& item, const utf8_characters_map_ref& utf8_characters_ref,
                          const span<const int8_t>& character_widths_ref) :
        sprite_font(item, utf8_characters_ref, character_widths_ref, 0)
    {
    }

    /**
     * @brief Constructor.
     * @param item sprite_item used to generate text sprites.
     * @param utf8_characters_ref Reference to the map of the supported UTF-8 characters
     * and their position in the tile sets.
     *
     * UTF-8 characters should appear in the tile sets referenced by item just after ASCII characters.
     *
     * UTF-8 characters map is not copied but referenced, so it should outlive the sprite_font
     * to avoid dangling references.
     *
     * @param character_widths_ref Reference to the width in pixels of each supported character.
     *
     * Character widths are not copied but referenced, so they should outlive the sprite_font
     * to avoid dangling references.
     *
     * @param space_between_characters Space between two consecutive characters in pixels (it can be negative).
     */
    constexpr sprite_font(const sprite_item& item, const utf8_characters_map_ref& utf8_characters_ref,
                          const span<const int8_t>& character_widths_ref, int space_between_characters) :
        _item(item),
        _utf8_characters_ref(utf8_characters_ref),
        _character_widths_ref(character_widths_ref),
        _space_between_characters(space_between_characters)
    {
        BN_ASSERT(item.tiles_item().compression() == compression_type::NONE, "Compressed tiles not supported");
        BN_ASSERT(item.tiles_item().graphics_count() >= minimum_graphics + utf8_characters_ref.size(),
                   "Invalid graphics count or UTF-8 characters count: ", item.tiles_item().graphics_count(), " - ",
                   utf8_characters_ref.size(), " - ", minimum_graphics + utf8_characters_ref.size());
        BN_ASSERT(item.palette_item().bpp() == bpp_mode::BPP_4, "8BPP fonts not supported");
        BN_ASSERT(character_widths_ref.empty() ||
                  character_widths_ref.size() == 1 + minimum_graphics + utf8_characters_ref.size(),
                  "Invalid character widths count: ", character_widths_ref.size(), " - ",
                  utf8_characters_ref.size(), " - ", minimum_graphics + utf8_characters_ref.size());
        BN_BASIC_ASSERT(_validate_character_widths(), "Character widths validation failed");
        BN_BASIC_ASSERT(_validate_space_between_characters(), "Space between characters validation failed");
    }

    /**
     * @brief Returns the sprite_item used to generate text sprites.
     */
    [[nodiscard]] constexpr const sprite_item& item() const
    {
        return _item;
    }

    /**
     * @brief Returns the reference to the map of the supported UTF-8 characters
     * and their position in the tile sets.
     *
     * UTF-8 characters should appear in the tile sets referenced by item just after ASCII characters.
     *
     * UTF-8 characters map is not copied but referenced, so it should outlive the sprite_font
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr const utf8_characters_map_ref& utf8_characters_ref() const
    {
        return _utf8_characters_ref;
    }

    /**
     * @brief Returns the reference to the width in pixels of each supported character.
     *
     * Character widths are not copied but referenced, so they should outlive the sprite_font
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr const span<const int8_t>& character_widths_ref() const
    {
        return _character_widths_ref;
    }

    /**
     * @brief Returns the space between two consecutive characters in pixels (it can be negative).
     */
    [[nodiscard]] int space_between_characters() const
    {
        return _space_between_characters;
    }

private:
    sprite_item _item;
    utf8_characters_map_ref _utf8_characters_ref;
    span<const int8_t> _character_widths_ref;
    int _space_between_characters;

    [[nodiscard]] constexpr bool _validate_character_widths() const
    {
        int max_character_width = _item.shape_size().width();

        for(int character_width : _character_widths_ref)
        {
            if(character_width < 0 || character_width > max_character_width)
            {
                return false;
            }
        }

        return true;
    }

    [[nodiscard]] constexpr bool _validate_space_between_characters() const
    {
        int space_between_characters = _space_between_characters;

        if(space_between_characters < 0)
        {
            space_between_characters = -space_between_characters;

            if(_character_widths_ref.empty())
            {
                int max_character_width = _item.shape_size().width();

                if(max_character_width <= space_between_characters)
                {
                    return false;
                }
            }
            else
            {
                for(int character_width : _character_widths_ref)
                {
                    if(character_width <= space_between_characters)
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }
};

}

#endif
