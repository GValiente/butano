/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_FORMAT_H
#define BN_FORMAT_H

/**
 * @file
 * bn::format and bn::format_ref header file.
 *
 * @ingroup string
 */

#include "bn_string.h"

/// @cond DO_NOT_DOCUMENT

namespace _bn
{
    void format(bn::ostringstream& stream, const char* format_begin, const char* format_end);

    template<typename Type>
    void format(bn::ostringstream& stream, const char* format_begin, const char* format_end, const Type& value)
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

                    stream << value;
                    format(stream, format_begin, format_end);
                    return;
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

    template<typename Type, typename... Args>
    void format(bn::ostringstream& stream, const char* format_begin, const char* format_end, const Type& value,
                const Args&... args)
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

                    stream << value;
                    format(stream, format_begin, format_end, args...);
                    return;
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

/// @endcond


namespace bn
{

/**
 * @brief Format the given arguments according to the given format string, and return the result as a string.
 * @tparam MaxSize Maximum number of characters that can be stored in the output string.
 * @tparam Args Types of the arguments to be formatted.
 * @param format string_view representing the format string.
 *
 * The format string consists of:
 * * Ordinary characters (except `{` and `}`), which are copied unchanged to the output.
 * * Escape sequences `{{` and `}}`, which are replaced with `{` and `}` respectively in the output.
 * * Replacement fields, with the following format: `{}`.
 *
 * @param args Arguments to be formatted. They are used in order when processing the format string.
 * @return A string holding the formatted result.
 *
 * @ingroup string
 */
template<int MaxSize, class... Args>
[[nodiscard]] string<MaxSize> format(const string_view& format, const Args&... args)
{
    string<MaxSize> result;
    ostringstream stream(result);
    _bn::format(stream, format.begin(), format.end(), args...);
    return result;
}

/**
 * @brief Format the given arguments according to the given format string, storing the result in the given string.
 * @param string The result of the formatting is stored in this string.
 * @param format string_view representing the format string.
 *
 * The format string consists of:
 * * Ordinary characters (except `{` and `}`), which are copied unchanged to the output.
 * * Escape sequences `{{` and `}}`, which are replaced with `{` and `}` respectively in the output.
 * * Replacement fields, with the following format: `{}`.
 *
 * @param args Arguments to be formatted. They are used in order when processing the format string.
 *
 * @ingroup string
 */
template<class... Args>
void format_ref(istring_base& string, const string_view& format, const Args&... args)
{
    ostringstream stream(string);
    _bn::format(stream, format.begin(), format.end(), args...);
}

}

#endif
