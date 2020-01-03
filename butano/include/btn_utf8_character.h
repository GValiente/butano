#ifndef BTN_UTF8_CHARACTER_H
#define BTN_UTF8_CHARACTER_H

#include "btn_assert.h"

namespace btn
{

class utf8_character
{

public:
    constexpr explicit utf8_character(const char* text)
    {
        // https://www.coranac.com/tonc/text/tte.htm

        auto src = text;
        auto ch8 = unsigned(*src);
        BTN_CONSTEXPR_ASSERT(text, "Text is null");

        if(ch8 < 0x80)
        {
            // 7bit
            _value = int(ch8);
        }
        else if(0xC0 <= ch8 && ch8 < 0xE0)
        {
            // 11bit
            _value = (*src++ & 0x1F) << 6;
            BTN_CONSTEXPR_ASSERT((*src >> 6) == 2, "Invalid utf8 character");

            _value |= (*src++ & 0x3F) << 0;
        }
        else if(0xE0 <= ch8 && ch8 < 0xF0)
        {
            // 16bit
            _value  = (*src++ & 0x0F) << 12;
            BTN_CONSTEXPR_ASSERT((*src >> 6) == 2, "Invalid utf8 character");

            _value |= (*src++ & 0x3F) <<  6;
            BTN_CONSTEXPR_ASSERT((*src >> 6) == 2, "Invalid utf8 character");

            _value |= (*src++ & 0x3F) <<  0;
        }
        else if(0xF0 <= ch8 && ch8 < 0xF8)
        {
            // 21bit
            _value  = (*src++ & 0x0F) << 18;
            BTN_CONSTEXPR_ASSERT((*src >> 6) == 2, "Invalid utf8 character");

            _value |= (*src++ & 0x3F) << 12;
            BTN_CONSTEXPR_ASSERT((*src >> 6) == 2, "Invalid utf8 character");

            _value |= (*src++ & 0x3F) <<  6;
            BTN_CONSTEXPR_ASSERT((*src >> 6) == 2, "Invalid utf8 character");

            _value |= (*src++ & 0x3F) <<  0;
        }
        else
        {
            BTN_CONSTEXPR_ASSERT(true, "Invalid utf8 character");
        }

        _size = src - text;
    }

    [[nodiscard]] constexpr int value() const
    {
        return _value;
    }

    [[nodiscard]] constexpr int size() const
    {
        return _size;
    }

private:
    int _value = 0;
    int _size = 0;
};

}

#endif
