#ifndef BTN_INPUT_STRING_STREAM_H
#define BTN_INPUT_STRING_STREAM_H

#include "btn_limits.h"
#include "btn_string.h"
#include "btn_fixed_fwd.h"

namespace btn
{

class string_view;

class input_string_stream
{

public:
    explicit input_string_stream(istring& string) :
        _string(string)
    {
    }

    [[nodiscard]] const istring& string() const
    {
        return _string;
    }

    [[nodiscard]] istring& string()
    {
        return _string;
    }

    [[nodiscard]] bool empty() const
    {
        return _string.empty();
    }

    [[nodiscard]] size_t size() const
    {
        return _string.size();
    }

    [[nodiscard]] size_t capacity() const
    {
        return _string.capacity();
    }

    [[nodiscard]] size_t available() const
    {
        return _string.available();
    }

    void append(char character)
    {
        _string += character;
    }

    void append(const string_view& str_view);

    void append(const istring& string)
    {
        _string.append(string);
    }

    void append(const char* char_array);

    void append(const char* char_array, size_t size);

    template<size_t Capacity>
    void append(char(&char_array)[Capacity])
    {
        append(char_array, Capacity);
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

    void append(float value);

    void append(double value);

    void append(long double value);

    void append(const void* ptr);

    template<size_t Precision>
    void append(fixed_t<Precision> value)
    {
        // https://stackoverflow.com/questions/57452174/how-to-correctly-print-a-2-30-fixed-point-variable

        append(value.integer());

        if(int fraction = value.fraction())
        {
            auto scale = unsigned(value.scale());
            unsigned digits = 10000;
            append('.');

            if(uint64_t(scale) * digits < numeric_limits<unsigned>::max())
            {
                append((unsigned(fraction) * digits) / scale);
            }
            else
            {
                append((uint64_t(fraction) * digits) / scale);
            }
        }
    }

    template<typename Type, typename... Args>
    void append_args(const Type& value, const Args&... args)
    {
        append(value);
        append_args(args...);
    }

    template<typename Type>
    void append_args(const Type& value)
    {
        append(value);
    }

    void append_args()
    {
    }

    void clear()
    {
        _string.clear();
    }

    void resize(size_t new_size, char character)
    {
        _string.resize(new_size, character);
    }

private:
    istring& _string;
};


inline input_string_stream& operator<<(input_string_stream& stream, char character)
{
    stream.append(character);
    return stream;
}

inline input_string_stream& operator<<(input_string_stream& stream, const string_view& string_view)
{
    stream.append(string_view);
    return stream;
}

inline input_string_stream& operator<<(input_string_stream& stream, const istring& string)
{
    stream.append(string);
    return stream;
}

inline input_string_stream& operator<<(input_string_stream& stream, const char* char_array)
{
    stream.append(char_array);
    return stream;
}

template<size_t Capacity>
inline input_string_stream& operator<<(input_string_stream& stream, char(&char_array)[Capacity])
{
    stream.append<Capacity>(char_array);
    return stream;
}

inline input_string_stream& operator<<(input_string_stream& stream, bool value)
{
    stream.append(value);
    return stream;
}

inline input_string_stream& operator<<(input_string_stream& stream, int value)
{
    stream.append(value);
    return stream;
}

inline input_string_stream& operator<<(input_string_stream& stream, long value)
{
    stream.append(value);
    return stream;
}

inline input_string_stream& operator<<(input_string_stream& stream, int64_t value)
{
    stream.append(value);
    return stream;
}

inline input_string_stream& operator<<(input_string_stream& stream, unsigned value)
{
    stream.append(value);
    return stream;
}

inline input_string_stream& operator<<(input_string_stream& stream, unsigned long value)
{
    stream.append(value);
    return stream;
}

inline input_string_stream& operator<<(input_string_stream& stream, uint64_t value)
{
    stream.append(value);
    return stream;
}

inline input_string_stream& operator<<(input_string_stream& stream, float value)
{
    stream.append(value);
    return stream;
}

inline input_string_stream& operator<<(input_string_stream& stream, double value)
{
    stream.append(value);
    return stream;
}

inline input_string_stream& operator<<(input_string_stream& stream, long double value)
{
    stream.append(value);
    return stream;
}

template<size_t Precision>
inline input_string_stream& operator<<(input_string_stream& stream, fixed_t<Precision> value)
{
    stream.append(value);
    return stream;
}

}

#endif
