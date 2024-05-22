/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sstream.h"

#include "bn_array.h"
#include "bn_string.h"
#include "bn_string_view.h"
#include "../hw/include/bn_hw_text.h"

namespace bn
{

ostringstream::ostringstream(istring_base& string) :
    _string(static_cast<istring*>(&string))
{
}

istring* ostringstream::rdbuf(istring_base* sb)
{
    BN_BASIC_ASSERT(sb, "Sb is null");

    _string = static_cast<istring*>(sb);
    return _string;
}

void ostringstream::set_rdbuf(istring_base& sb)
{
    _string = static_cast<istring*>(&sb);
}

int ostringstream::precision(int new_precision)
{
    BN_ASSERT(new_precision >= 0, "Invalid new precision: ", new_precision);

    _precision = new_precision;
    return new_precision;
}

void ostringstream::set_precision(int precision)
{
    BN_ASSERT(precision >= 0, "Invalid precision: ", precision);

    _precision = precision;
}

string_view ostringstream::view() const
{
    return string_view(*_string);
}

int ostringstream::size() const
{
    return _string->size();
}

int ostringstream::length() const
{
    return _string->length();
}

int ostringstream::max_size() const
{
    return _string->max_size();
}

int ostringstream::available() const
{
    return _string->available();
}

bool ostringstream::empty() const
{
    return _string->empty();
}

bool ostringstream::full() const
{
    return _string->full();
}

void ostringstream::append(char character)
{
    _string->push_back(character);
}

void ostringstream::append(const string_view& string_view)
{
    _string->append(string_view.data(), string_view.size());
}

void ostringstream::append(const istring_base& istring)
{
    _string->append(istring);
}

void ostringstream::append(const char* char_array_ptr)
{
    _string->append(char_array_ptr);
}

void ostringstream::append(const char* char_array_ptr, int char_array_size)
{
    _string->append(char_array_ptr, char_array_size);
}

void ostringstream::append(int value)
{
    array<char, 32> buffer;
    int size = hw::text::parse(value, buffer);
    _string->append(buffer.data(), size);
}

void ostringstream::append(long value)
{
    array<char, 32> buffer;
    int size = hw::text::parse(value, buffer);
    _string->append(buffer.data(), size);
}

void ostringstream::append(int64_t value)
{
    array<char, 32> buffer;
    int size = hw::text::parse(value, buffer);
    _string->append(buffer.data(), size);
}

void ostringstream::append(unsigned value)
{
    array<char, 32> buffer;
    int size = hw::text::parse(value, buffer);
    _string->append(buffer.data(), size);
}

void ostringstream::append(unsigned long value)
{
    array<char, 32> buffer;
    int size = hw::text::parse(value, buffer);
    _string->append(buffer.data(), size);
}

void ostringstream::append(uint64_t value)
{
    array<char, 32> buffer;
    int size = hw::text::parse(value, buffer);
    _string->append(buffer.data(), size);
}

void ostringstream::append(const void* ptr)
{
    if(ptr)
    {
        array<char, 32> buffer;
        int size = hw::text::parse(ptr, buffer);
        _string->append(buffer.data(), size);
    }
    else
    {
        append(nullptr);
    }
}

void ostringstream::swap(ostringstream& other)
{
    bn::swap(_string, other._string);
    bn::swap(_precision, other._precision);
}

void ostringstream::_append_fraction(unsigned fraction_result, int fraction_digits)
{
    array<char, 32> buffer;
    int fraction_size = hw::text::parse(fraction_result, buffer);
    istring& string = *_string;
    string.append('.');

    for(int index = fraction_size; index < fraction_digits; ++index)
    {
        string.append('0');
    }

    string.append(buffer.data(), fraction_size);
}

}
