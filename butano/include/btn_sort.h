#ifndef BTN_SORT_H
#define BTN_SORT_H

#include "btn_utility.h"

namespace btn
{
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
}

#endif
