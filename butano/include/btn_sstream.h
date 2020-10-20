/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SSTREAM_H
#define BTN_SSTREAM_H

#include "btn_cstddef.h"
#include "btn_fixed_fwd.h"
#include "btn_string_fwd.h"
#include "btn_istring_base.h"

namespace btn
{

class string_view;

class ostringstream
{

public:
    explicit ostringstream(istring_base& string) :
        _string(&string)
    {
    }

    [[nodiscard]] const istring& str() const;

    [[nodiscard]] istring& str();

    [[nodiscard]] const istring_base* rdbuf() const
    {
        return _string;
    }

    [[nodiscard]] istring_base* rdbuf()
    {
        return _string;
    }

    istring_base* rdbuf(istring_base* sb);

    void set_rdbuf(istring_base& sb)
    {
        _string = &sb;
    }

    [[nodiscard]] int precision() const
    {
        return _precision;
    }

    int precision(int new_precision);

    void set_precision(int precision);

    [[nodiscard]] string_view view() const;

    [[nodiscard]] int size() const
    {
        return _string->size();
    }

    [[nodiscard]] int length() const
    {
        return _string->length();
    }

    [[nodiscard]] int capacity() const
    {
        return _string->capacity();
    }

    [[nodiscard]] int max_size() const
    {
        return _string->max_size();
    }

    [[nodiscard]] int available() const
    {
        return _string->available();
    }

    [[nodiscard]] bool empty() const
    {
        return _string->empty();
    }

    [[nodiscard]] bool full() const
    {
        return _string->full();
    }

    void append(char character);

    ostringstream& put(char character)
    {
        append(character);
        return *this;
    }

    void append(const string_view& string_view);

    void append(const istring& istring);

    void append(const char* char_array);

    void append(const char* char_array, int size);

    ostringstream& write(const char* char_array, int size)
    {
        append(char_array, size);
        return *this;
    }

    template<int MaxSize>
    void append(char(&char_array)[MaxSize])
    {
        append(char_array, MaxSize);
    }

    void append(bool value)
    {
        append(value ? "true" : "false");
    }

    void append(int value);

    void append(long value);

    void append(int64_t value);

    void append(unsigned value);

    void append(unsigned long value);

    void append(uint64_t value);

    void append(const void* ptr);

    void append(const nullptr_t& null_ptr);

    template<int Precision>
    void append(fixed_t<Precision> value)
    {
        // https://stackoverflow.com/questions/57452174/how-to-correctly-print-a-2-30-fixed-point-variable

        if(value < 0)
        {
            append('-');
            append(0 - value);
        }
        else
        {
            int old_size = size();
            append(value.integer());

            int integer_digits = size() - old_size;
            int fraction_digits = precision() - integer_digits;

            if(fraction_digits > 0)
            {
                if(int fraction = value.fraction())
                {
                    unsigned zeros = 1;

                    for(int index = 0; index < fraction_digits; ++index)
                    {
                        zeros *= 10;
                    }

                    unsigned fraction_result = (uint64_t(fraction) * zeros) / unsigned(value.scale());

                    if(fraction_result)
                    {
                        _append_fraction(fraction_result, fraction_digits);
                    }
                }
            }
        }
    }

    template<typename Type, typename... Args>
    void append_args(const Type& value, const Args&... args)
    {
        *this << value;
        append_args(args...);
    }

    template<typename Type>
    void append_args(const Type& value)
    {
        *this << value;
    }

    void append_args()
    {
    }

    void swap(ostringstream& other);

    friend void swap(ostringstream& a, ostringstream& b)
    {
        a.swap(b);
    }

private:
    istring_base* _string;
    int _precision = 6;

    void _append_fraction(unsigned fraction_result, int fraction_digits);
};


using stringstream = ostringstream;


inline ostringstream& operator<<(ostringstream& stream, char character)
{
    stream.append(character);
    return stream;
}

inline ostringstream& operator<<(ostringstream& stream, const string_view& string_view)
{
    stream.append(string_view);
    return stream;
}

inline ostringstream& operator<<(ostringstream& stream, const istring& string)
{
    stream.append(string);
    return stream;
}

inline ostringstream& operator<<(ostringstream& stream, const char* char_array)
{
    stream.append(char_array);
    return stream;
}

template<int MaxSize>
inline ostringstream& operator<<(ostringstream& stream, char(&char_array)[MaxSize])
{
    stream.append<MaxSize>(char_array);
    return stream;
}

inline ostringstream& operator<<(ostringstream& stream, bool value)
{
    stream.append(value);
    return stream;
}

inline ostringstream& operator<<(ostringstream& stream, int value)
{
    stream.append(value);
    return stream;
}

inline ostringstream& operator<<(ostringstream& stream, long value)
{
    stream.append(value);
    return stream;
}

inline ostringstream& operator<<(ostringstream& stream, int64_t value)
{
    stream.append(value);
    return stream;
}

inline ostringstream& operator<<(ostringstream& stream, unsigned value)
{
    stream.append(value);
    return stream;
}

inline ostringstream& operator<<(ostringstream& stream, unsigned long value)
{
    stream.append(value);
    return stream;
}

inline ostringstream& operator<<(ostringstream& stream, uint64_t value)
{
    stream.append(value);
    return stream;
}

inline ostringstream& operator<<(ostringstream& stream, const void* ptr)
{
    stream.append(ptr);
    return stream;
}

inline ostringstream& operator<<(ostringstream& stream, const nullptr_t& null_ptr)
{
    stream.append(null_ptr);
    return stream;
}

template<int Precision>
ostringstream& operator<<(ostringstream& stream, fixed_t<Precision> value)
{
    stream.append(value);
    return stream;
}

}

#endif
