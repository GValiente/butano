#ifndef BTN_ISTRING_BASE_H
#define BTN_ISTRING_BASE_H

#include "btn_common.h"

namespace btn
{

class istring_base
{

public:
    using size_type = int;
    using reference = char&;
    using const_reference = const char&;
    using pointer = char*;
    using const_pointer = const char*;

public:
    template<int MaxSize>
    constexpr istring_base(char (&text)[MaxSize]) :
        _data(text),
        _size(0),
        _max_size(MaxSize - 1)
    {
        static_assert(MaxSize > 1);

        _data[0] = 0;
    }

    [[nodiscard]] constexpr size_type size() const
    {
        return _size;
    }

    [[nodiscard]] constexpr size_type length() const
    {
        return _size;
    }

    [[nodiscard]] constexpr size_type capacity() const
    {
        return _max_size;
    }

    [[nodiscard]] constexpr size_type max_size() const
    {
        return _max_size;
    }

    [[nodiscard]] constexpr size_type available() const
    {
        return _max_size - _size;
    }

    [[nodiscard]] constexpr bool empty() const
    {
        return _size == 0;
    }

    [[nodiscard]] constexpr bool full() const
    {
        return _size == _max_size;
    }

    [[nodiscard]] constexpr const_pointer data() const
    {
        return _data;
    }

    [[nodiscard]] constexpr pointer data()
    {
        return _data;
    }

protected:
    pointer _data;
    size_type _size;
    size_type _max_size;

    constexpr istring_base(pointer data, size_type size, size_type max_size) :
        _data(data),
        _size(size),
        _max_size(max_size)
    {
    }
};

}

#endif
