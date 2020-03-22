#ifndef BTN_INPUT_STRING_STREAM_H
#define BTN_INPUT_STRING_STREAM_H

#include "btn_limits.h"
#include "btn_fixed_fwd.h"
#include "btn_string_fwd.h"
#include "btn_istring_base.h"

namespace btn
{

class string_view;

class input_string_stream
{

public:
    explicit input_string_stream(istring_base& string) :
        _string(string)
    {
    }

    [[nodiscard]] const istring& string() const;

    [[nodiscard]] istring& string();

    [[nodiscard]] int size() const
    {
        return _string.size();
    }

    [[nodiscard]] int length() const
    {
        return _string.length();
    }

    [[nodiscard]] int capacity() const
    {
        return _string.capacity();
    }

    [[nodiscard]] int max_size() const
    {
        return _string.max_size();
    }

    [[nodiscard]] int available() const
    {
        return _string.available();
    }

    [[nodiscard]] bool empty() const
    {
        return _string.empty();
    }

    [[nodiscard]] bool full() const
    {
        return _string.full();
    }

    void append(char character);

    void append(const string_view& str_view);

    void append(const istring& string);

    void append(const char* char_array);

    void append(const char* char_array, int size);

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

    void append(float value);

    void append(double value);

    void append(long double value);

    void append(const void* ptr);

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

    void clear();

private:
    istring_base& _string;
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

template<int MaxSize>
inline input_string_stream& operator<<(input_string_stream& stream, char(&char_array)[MaxSize])
{
    stream.append<MaxSize>(char_array);
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

template<int Precision>
input_string_stream& operator<<(input_string_stream& stream, fixed_t<Precision> value)
{
    stream.append(value);
    return stream;
}

}

#endif
