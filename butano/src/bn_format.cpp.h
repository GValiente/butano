/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_format.h"

namespace _bn
{

void format(bn::ostringstream& stream, const char* format_begin, const char* format_end)
{
    while(format_begin != format_end)
    {
        char character = *format_begin;
        ++format_begin;

        if(character == '{') [[unlikely]]
        {
            BN_BASIC_ASSERT(format_begin != format_end, "Format contains a single '{' character");

            char next_character = *format_begin;
            ++format_begin;

            if(next_character == '{') [[unlikely]]
            {
                stream.append(next_character);
            }
            else
            {
                BN_BASIC_ASSERT(next_character == '}', "Format contains a single '{' character");
                BN_ERROR("Not enough arguments");
            }
        }
        else if(character == '}') [[unlikely]]
        {
            BN_BASIC_ASSERT(format_begin != format_end, "Format contains a single '}' character");
            BN_BASIC_ASSERT(*format_begin == '}', "Format contains a single '}' character");

            ++format_begin;
            stream.append('}');
        }
        else
        {
            stream.append(character);
        }
    }
}

}
