#ifndef BTN_ALGORITHM_H
#define BTN_ALGORITHM_H

#include "../3rd_party/etl/include/etl/algorithm.h"

namespace btn
{
    template<typename Type>
    [[nodiscard]] constexpr const Type& min(const Type& a, const Type& b)
    {
        return (a < b) ? a : b;
    }

    template<typename Type>
    [[nodiscard]] constexpr const Type& max(const Type& a, const Type& b)
    {
        return (a < b) ? b : a;
    }

    template<typename Type>
    [[nodiscard]] constexpr const Type& clamp(const Type& value, const Type& min_value, const Type& max_value)
    {
        return min(max(value, min_value), max_value);
    }

    template<typename Iterator>
    void sort(Iterator first, Iterator last)
    {
        etl::sort(first, last);
    }

    template<typename Iterator, typename Comparator>
    void sort(Iterator first, Iterator last, Comparator comparator)
    {
        etl::sort(first, last, comparator);
    }
}

#endif
