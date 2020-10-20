/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_sstream.h"

#include "btn_array.h"
#include "btn_string.h"
#include "btn_string_view.h"
#include "../hw/include/btn_hw_text.h"

namespace btn
{

const istring& ostringstream::str() const
{
    return static_cast<const istring&>(*_string);
}

istring& ostringstream::str()
{
    return static_cast<istring&>(*_string);
}

istring_base* ostringstream::rdbuf(istring_base* sb)
{
    BTN_ASSERT(sb, "Sb is null");

    _string = sb;
    return sb;
}

int ostringstream::precision(int new_precision)
{
    BTN_ASSERT(new_precision >= 0, "Invalid precision: ", new_precision);

    _precision = new_precision;
    return new_precision;
}

void ostringstream::set_precision(int precision)
{
    BTN_ASSERT(precision >= 0, "Invalid precision: ", precision);

    _precision = precision;
}

string_view ostringstream::view() const
{
    return string_view(*_string);
}

void ostringstream::append(char character)
{
    str().push_back(character);
}

void ostringstream::append(const string_view& string_view)
{
    str().append(string_view.data(), string_view.size());
}

void ostringstream::append(const istring& istring)
{
    str().append(istring);
}

void ostringstream::append(const char* char_array)
{
    str().append(char_array);
}

void ostringstream::append(const char* char_array, int size)
{
    str().append(char_array, size);
}

void ostringstream::append(int value)
{
    array<char, 32> buffer;
    int size = hw::text::parse(value, buffer);
    str().append(buffer.data(), size);
}

void ostringstream::append(long value)
{
    array<char, 32> buffer;
    int size = hw::text::parse(value, buffer);
    str().append(buffer.data(), size);
}

void ostringstream::append(int64_t value)
{
    array<char, 32> buffer;
    int size = hw::text::parse(value, buffer);
    str().append(buffer.data(), size);
}

void ostringstream::append(unsigned value)
{
    array<char, 32> buffer;
    int size = hw::text::parse(value, buffer);
    str().append(buffer.data(), size);
}

void ostringstream::append(unsigned long value)
{
    array<char, 32> buffer;
    int size = hw::text::parse(value, buffer);
    str().append(buffer.data(), size);
}

void ostringstream::append(uint64_t value)
{
    array<char, 32> buffer;
    int size = hw::text::parse(value, buffer);
    str().append(buffer.data(), size);
}

void ostringstream::append(const void* ptr)
{
    if(ptr)
    {
        array<char, 32> buffer;
        int size = hw::text::parse(ptr, buffer);
        str().append(buffer.data(), size);
    }
    else
    {
        append(nullptr);
    }
}

void ostringstream::append(const nullptr_t&)
{
    str().append("nullptr");
}

void ostringstream::swap(ostringstream& other)
{
    btn::swap(_string, other._string);
    btn::swap(_precision, other._precision);
}

void ostringstream::_append_fraction(unsigned fraction_result, int fraction_digits)
{
    array<char, 32> buffer;
    int fraction_size = hw::text::parse(fraction_result, buffer);
    istring& string = ostringstream::str();
    string.append('.');

    for(int index = fraction_size; index < fraction_digits; ++index)
    {
        string.append('0');
    }

    string.append(buffer.data(), fraction_size);
}

}
