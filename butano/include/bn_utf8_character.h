/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_UTF8_CHARACTER_H
#define BN_UTF8_CHARACTER_H

/**
 * @file
 * bn::utf8_character header file.
 *
 * @ingroup text
 */

#include "bn_assert.h"

namespace bn
{

/**
 * @brief Decodes a single UTF-8 character from a string.
 *
 * See https://www.coranac.com/tonc/text/tte.htm, UTF-8 section.
 *
 * @ingroup text
 */
class utf8_character
{

public:
    /**
     * @brief Constructor.
     * @param text_ptr Non null pointer to the string to decode.
     */
    constexpr explicit utf8_character(const char* text_ptr)
    {
        BN_ASSERT(text_ptr, "Text is null");

        *this = utf8_character(*text_ptr);
    }

    /**
     * @brief Constructor.
     * @param text_ref Reference to the string to decode.
     */
    constexpr explicit utf8_character(const char& text_ref)
    {
        const char* src = &text_ref;
        auto ch8 = unsigned(*src);

        if(ch8 < 0x80)
        {
            // 7bit
            _data = int(ch8);
        }
        else if(0xC0 <= ch8 && ch8 < 0xE0)
        {
            // 11bit
            _data = (*src++ & 0x1F) << 6;
            BN_ASSERT((*src >> 6) == 2, "Invalid UTF-8 character");

            _data |= (*src++ & 0x3F) << 0;
        }
        else if(0xE0 <= ch8 && ch8 < 0xF0)
        {
            // 16bit
            _data  = (*src++ & 0x0F) << 12;
            BN_ASSERT((*src >> 6) == 2, "Invalid UTF-8 character");

            _data |= (*src++ & 0x3F) <<  6;
            BN_ASSERT((*src >> 6) == 2, "Invalid UTF-8 character");

            _data |= (*src++ & 0x3F) <<  0;
        }
        else if(0xF0 <= ch8 && ch8 < 0xF8)
        {
            // 21bit
            _data  = (*src++ & 0x0F) << 18;
            BN_ASSERT((*src >> 6) == 2, "Invalid UTF-8 character");

            _data |= (*src++ & 0x3F) << 12;
            BN_ASSERT((*src >> 6) == 2, "Invalid UTF-8 character");

            _data |= (*src++ & 0x3F) <<  6;
            BN_ASSERT((*src >> 6) == 2, "Invalid UTF-8 character");

            _data |= (*src++ & 0x3F) <<  0;
        }
        else
        {
            BN_ERROR("Invalid UTF-8 character");
        }

        _size = src - &text_ref;
    }

    /**
     * @brief Returns the decoded UTF-8 character.
     */
    [[nodiscard]] constexpr int data() const
    {
        return _data;
    }

    /**
     * @brief Returns the size in bytes of the decoded UTF-8 character.
     */
    [[nodiscard]] constexpr int size() const
    {
        return _size;
    }

private:
    int _data = 0;
    int _size = 0;
};

}

#endif
