#ifndef BTN_SPRITE_FONT_H
#define BTN_SPRITE_FONT_H

#include "btn_string_view.h"
#include "btn_sprite_item.h"
#include "btn_utf8_character.h"
#include "btn_config_sprite_text.h"

namespace btn
{

class sprite_font
{

public:
    static constexpr const int minimum_graphics = 94;

    constexpr sprite_font(const sprite_item& item, const span<const string_view>& utf8_characters) :
        sprite_font(item, utf8_characters, span<const int8_t>())
    {
    }

    constexpr sprite_font(const sprite_item& item, const span<const string_view>& utf8_characters,
                          const span<const int8_t>& character_widths) :
        _item(item),
        _utf8_characters(utf8_characters),
        _character_widths(character_widths)
    {
        BTN_ASSERT(item.shape_size() == sprite_shape_size(sprite_shape::SQUARE, sprite_size::SMALL) ||
                   item.shape_size() == sprite_shape_size(sprite_shape::TALL, sprite_size::SMALL),
                   "Invalid shape size");
        BTN_ASSERT(item.tiles_item().graphics_count() >= minimum_graphics + utf8_characters.size(),
                   "Invalid graphics count or utf8 characters count: ", item.tiles_item().graphics_count(), " - ",
                   utf8_characters.size(), " - ", minimum_graphics + utf8_characters.size());
        BTN_ASSERT(item.palette_item().bpp_mode() == palette_bpp_mode::BPP_4, "8BPP fonts not supported");
        BTN_ASSERT(utf8_characters.size() <= BTN_CFG_SPRITE_TEXT_MAX_UTF8_CHARACTERS,
                   "Invalid utf8 characters count: ", utf8_characters.size());
        BTN_ASSERT(_validate_utf8_characters(utf8_characters), "Utf8 characters validation failed");
        BTN_ASSERT(! _duplicated_utf8_characters(utf8_characters), "There's duplicated utf8 characters");
        BTN_ASSERT(character_widths.empty() || character_widths.size() == 1 + minimum_graphics + utf8_characters.size(),
                   "Invalid characters width count: ", character_widths.size(), " - ",
                   utf8_characters.size(), " - ", minimum_graphics + utf8_characters.size());
        BTN_ASSERT(_validate_character_widths(character_widths), "Character widths validation failed");
    }

    [[nodiscard]] constexpr const sprite_item& item() const
    {
        return _item;
    }

    [[nodiscard]] constexpr const span<const string_view>& utf8_characters() const
    {
        return _utf8_characters;
    }

    [[nodiscard]] constexpr const span<const int8_t>& character_widths() const
    {
        return _character_widths;
    }

    [[nodiscard]] constexpr friend bool operator==(const sprite_font& a, const sprite_font& b) = default;

private:
    sprite_item _item;
    span<const string_view> _utf8_characters;
    span<const int8_t> _character_widths;

    [[nodiscard]] static constexpr bool _validate_utf8_characters(const span<const string_view>& utf8_characters)
    {
        for(const string_view& utf8_character_text : utf8_characters)
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

    [[nodiscard]] static constexpr bool _duplicated_utf8_characters(const span<const string_view>& utf8_characters)
    {
        for(int i = 0, size = utf8_characters.size(); i < size; ++i)
        {
            const string_view& a = utf8_characters[i];

            for(int j = i + 1; j < size; ++j)
            {
                const string_view& b = utf8_characters[j];

                if(a == b)
                {
                    return true;
                }
            }
        }

        return false;
    }

    [[nodiscard]] static constexpr bool _validate_character_widths(const span<const int8_t>& character_widths)
    {
        for(int character_width : character_widths)
        {
            if(character_width < 0 || character_width > 8)
            {
                return false;
            }
        }

        return true;
    }
};

}

#endif
