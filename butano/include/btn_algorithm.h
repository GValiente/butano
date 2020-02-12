#ifndef BTN_ALGORITHM_H
#define BTN_ALGORITHM_H

#include <algorithm>
#include "btn_common.h"

namespace btn
{
    using std::min;
    using std::max;
    using std::clamp;

    template<typename Iterator>
    [[nodiscard]] constexpr bool equal(Iterator first1, Iterator last1, Iterator first2)
    {
        if(! BTN_CONSTANT_EVALUATED())
        {
            if(first1 == first2)
            {
                return true;
            }
        }

        for(; first1 != last1; ++first1, ++first2)
        {
            if(*first1 != *first2)
            {
                return false;
            }
        }

        return true;
    }

    template<typename Iterator>
    [[nodiscard]] constexpr bool equal(Iterator first1, Iterator last1, Iterator first2, Iterator last2)
    {
        if(! BTN_CONSTANT_EVALUATED())
        {
            if(first1 == first2 && last1 == last2)
            {
                return true;
            }
        }

        for(; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
            if(*first1 != *first2)
            {
                return false;
            }
        }

        return first1 == last1 && first2 == last2;
    }

    template<typename Iterator>
    [[nodiscard]] constexpr bool lexicographical_compare(Iterator first1, Iterator last1,
                                                         Iterator first2, Iterator last2)
    {
        if(! BTN_CONSTANT_EVALUATED())
        {
            if(first1 == first2 && last1 == last2)
            {
                return false;
            }
        }

        for(; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
            if(*first1 < *first2)
            {
                return true;
            }

            if(*first2 < *first1)
            {
                return false;
            }
        }

        return first1 == last1 && first2 != last2;
    }

    template<class Iterator, typename Type>
    constexpr void fill(Iterator begin, Iterator end, const Type& value)
    {
        if(BTN_CONSTANT_EVALUATED())
        {
            for(Iterator it = begin; it != end; ++it)
            {
                *it = value;
            }
        }
        else
        {
            std::fill(begin, end, value);
        }
    }

    template<class Iterator, class Comparator>
    constexpr void insertion_sort(Iterator begin, Iterator end, const Comparator& comparator)
    {
        if(begin == end)
        {
            return;
        }

        Iterator i = begin;
        ++i;

        while(i != end)
        {
            Iterator k = i;

            while(k != begin)
            {
                Iterator before_k = k;
                --before_k;

                auto& k_value = *k;
                auto& before_k_value = *before_k;

                if(comparator(k_value, before_k_value))
                {
                    swap(k_value, before_k_value);
                    k = before_k;
                }
                else
                {
                    break;
                }
            }

            ++i;
        }
    }

    template<class Iterator>
    constexpr void insertion_sort(Iterator begin, Iterator end)
    {
        using value_type = decltype(*begin);

        insertion_sort(begin, end, [](const value_type& a, const value_type& b)
        {
            return a < b;
        });
    }

    template<class Iterator, class Comparator>
    constexpr void stable_sort(Iterator begin, Iterator end, const Comparator& comparator)
    {
        if(BTN_CONSTANT_EVALUATED())
        {
            insertion_sort(begin, end, comparator);
        }
        else
        {
            std::stable_sort(begin, end, comparator);
        }
    }

    template<class Iterator>
    constexpr void stable_sort(Iterator begin, Iterator end)
    {
        if(BTN_CONSTANT_EVALUATED())
        {
            insertion_sort(begin, end);
        }
        else
        {
            std::stable_sort(begin, end);
        }
    }
}

#endif
