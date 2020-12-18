/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
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

#include "bn_string_view.h"
#include "bn_sprite_item.h"
#include "bn_utf8_character.h"
#include "bn_config_sprite_text.h"

namespace bn
{

/**
 * @brief Contains the required information to generate text sprites.
 *
 * Currently, it supports 8x8 and 8x16 fixed width AND variable width characters of 16 colors (4 bits per pixel).
 *
 * Also, UTF-8 characters are supported.
 *
 * UTF-8 characters and character widths are not copied but referenced,
 * so they should outlive the sprite_font to avoid dangling references.
 *
 * @ingroup sprite
 * @ingroup text
 */
class sprite_font
{

public:
    static constexpr const int minimum_graphics = 94; //!< Minimum number of sprite tile sets that must contain a sprite_tiles_item of a sprite_font.

    /**
     * @brief Constructor.
     * @param item sprite_item used to generate text sprites.
     * @param utf8_characters_ref Reference to a list of supported UTF-8 characters.
     *
     * They should appear in the tile sets referenced by item just after ASCII characters.
     *
     * UTF-8 characters are not copied but referenced, so they should outlive the sprite_font
     * to avoid dangling references.
     */
    constexpr sprite_font(const sprite_item& item, const span<const string_view>& utf8_characters_ref) :
        sprite_font(item, utf8_characters_ref, span<const int8_t>(), 0)
    {
    }

    /**
     * @brief Constructor.
     * @param item sprite_item used to generate text sprites.
     * @param utf8_characters_ref Reference to a list of supported UTF-8 characters.
     *
     * They should appear in the tile sets referenced by item just after ASCII characters.
     *
     * UTF-8 characters are not copied but referenced, so they should outlive the sprite_font
     * to avoid dangling references.
     *
     * @param character_widths_ref Reference to the width in pixels of each supported character.
     *
     * Character widths are not copied but referenced, so they should outlive the sprite_font
     * to avoid dangling references.
     */
    constexpr sprite_font(const sprite_item& item, const span<const string_view>& utf8_characters_ref,
                          const span<const int8_t>& character_widths_ref) :
        sprite_font(item, utf8_characters_ref, character_widths_ref, 0)
    {
    }

    /**
     * @brief Constructor.
     * @param item sprite_item used to generate text sprites.
     * @param utf8_characters_ref Reference to a list of supported UTF-8 characters.
     *
     * They should appear in the tile sets referenced by item just after ASCII characters.
     *
     * UTF-8 characters are not copied but referenced, so they should outlive the sprite_font
     * to avoid dangling references.
     *
     * @param character_widths_ref Reference to the width in pixels of each supported character.
     *
     * Character widths are not copied but referenced, so they should outlive the sprite_font
     * to avoid dangling references.
     *
     * @param space_between_characters Space between two consecutive characters in pixels (it can be negative).
     */
    constexpr sprite_font(const sprite_item& item, const span<const string_view>& utf8_characters_ref,
                          const span<const int8_t>& character_widths_ref, int space_between_characters) :
        _item(item),
        _utf8_characters_ref(utf8_characters_ref),
        _character_widths_ref(character_widths_ref),
        _space_between_characters(space_between_characters)
    {
        BN_ASSERT(item.shape_size() == sprite_shape_size(sprite_shape::SQUARE, sprite_size::SMALL) ||
                   item.shape_size() == sprite_shape_size(sprite_shape::TALL, sprite_size::SMALL),
                   "Invalid shape size");
        BN_ASSERT(item.tiles_item().compression() == compression_type::NONE, "Compressed tiles not supported");
        BN_ASSERT(item.tiles_item().graphics_count() >= minimum_graphics + utf8_characters_ref.size(),
                   "Invalid graphics count or UTF-8 characters count: ", item.tiles_item().graphics_count(), " - ",
                   utf8_characters_ref.size(), " - ", minimum_graphics + utf8_characters_ref.size());
        BN_ASSERT(item.palette_item().bpp() == bpp_mode::BPP_4, "8BPP fonts not supported");
        BN_ASSERT(utf8_characters_ref.size() <= BN_CFG_SPRITE_TEXT_MAX_UTF8_CHARACTERS,
                   "Invalid UTF-8 characters count: ", utf8_characters_ref.size());
        BN_ASSERT(_validate_utf8_characters(), "UTF-8 characters validation failed");
        BN_ASSERT(! _duplicated_utf8_characters(), "There's duplicated UTF-8 characters");
        BN_ASSERT(character_widths_ref.empty() ||
                   character_widths_ref.size() == 1 + minimum_graphics + utf8_characters_ref.size(),
                   "Invalid character widths count: ", character_widths_ref.size(), " - ",
                   utf8_characters_ref.size(), " - ", minimum_graphics + utf8_characters_ref.size());
        BN_ASSERT(_validate_character_widths(), "Character widths validation failed");
        BN_ASSERT(_validate_space_between_characters(), "Space between characters validation failed");
    }

    /**
     * @brief Returns the sprite_item used to generate text sprites.
     */
    [[nodiscard]] constexpr const sprite_item& item() const
    {
        return _item;
    }

    /**
     * @brief Returns the reference to the list of supported UTF-8 characters.
     *
     * They should appear in the tile sets referenced by item just after ASCII characters.
     *
     * UTF-8 characters are not copied but referenced, so they should outlive the sprite_font
     * to avoid dangling references.
     */
    [[nodiscard]] constexpr const span<const string_view>& utf8_characters_ref() const
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
    span<const string_view> _utf8_characters_ref;
    span<const int8_t> _character_widths_ref;
    int _space_between_characters;

    [[nodiscard]] constexpr bool _validate_utf8_characters() const
    {
        for(const string_view& utf8_character_text : _utf8_characters_ref)
        {
            utf8_character utf8_char(utf8_character_text.data());

            if(utf8_char.data() <= '~')
            {
                return false;
            }

            if(utf8_char.size() != utf8_character_text.size())
            {
                return false;
            }
        }

        return true;
    }

    [[nodiscard]] constexpr bool _duplicated_utf8_characters() const
    {
        const string_view* utf8_characters_data = _utf8_characters_ref.data();

        for(int i = 0, size = _utf8_characters_ref.size(); i < size; ++i)
        {
            const string_view& a = utf8_characters_data[i];

            for(int j = i + 1; j < size; ++j)
            {
                const string_view& b = utf8_characters_data[j];

                if(a == b)
                {
                    return true;
                }
            }
        }

        return false;
    }

    [[nodiscard]] constexpr bool _validate_character_widths() const
    {
        for(int character_width : _character_widths_ref)
        {
            if(character_width < 0 || character_width > 8)
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
            for(int character_width : _character_widths_ref)
            {
                if(character_width <= space_between_characters)
                {
                    return false;
                }
            }
        }

        return true;
    }
};

}

#endif
