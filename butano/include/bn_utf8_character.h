/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

/*
 * Copyright 2005-2009 J Vijn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
 * See utf8_decode_char() function in https://github.com/devkitPro/libtonc/blob/master/src/tte/tte_main.c.
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
    constexpr utf8_character(const char* text_ptr)
    {
        BN_BASIC_ASSERT(text_ptr, "Text is null");

        *this = utf8_character(*text_ptr);
    }

    /**
     * @brief Constructor.
     * @param text_ref Reference to the string to decode.
     */
    constexpr utf8_character(const char& text_ref)
    {
        const char* src = &text_ref;
        auto ch8 = unsigned(*src);

        if(ch8 < 0x80)
        {
            // 7bit
            _data = int(ch8);
            ++src;
        }
        else if(0xC0 <= ch8 && ch8 < 0xE0)
        {
            // 11bit
            _data = (*src++ & 0x1F) << 6;
            BN_BASIC_ASSERT((*src >> 6) == 2, "Invalid UTF-8 character");

            _data |= (*src++ & 0x3F) << 0;
        }
        else if(0xE0 <= ch8 && ch8 < 0xF0)
        {
            // 16bit
            _data  = (*src++ & 0x0F) << 12;
            BN_BASIC_ASSERT((*src >> 6) == 2, "Invalid UTF-8 character");

            _data |= (*src++ & 0x3F) <<  6;
            BN_BASIC_ASSERT((*src >> 6) == 2, "Invalid UTF-8 character");

            _data |= (*src++ & 0x3F) <<  0;
        }
        else if(0xF0 <= ch8 && ch8 < 0xF8)
        {
            // 21bit
            _data  = (*src++ & 0x0F) << 18;
            BN_BASIC_ASSERT((*src >> 6) == 2, "Invalid UTF-8 character");

            _data |= (*src++ & 0x3F) << 12;
            BN_BASIC_ASSERT((*src >> 6) == 2, "Invalid UTF-8 character");

            _data |= (*src++ & 0x3F) <<  6;
            BN_BASIC_ASSERT((*src >> 6) == 2, "Invalid UTF-8 character");

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
