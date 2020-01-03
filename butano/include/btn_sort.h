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

                if(comparator(k, before_k))
                {
                    swap(*k, *before_k);
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
        insertion_sort(begin, end, [](const Iterator& a, const Iterator& b)
        {
            return *a < *b;
        });
    }

    template<class Iterator, class Comparator, int increment = 3>
    constexpr void shell_sort(Iterator begin, Iterator end, const Comparator& comparator)
    {
        static_assert(increment > 1, "");

        auto distance = int(end - begin);

        if(distance < 2)
        {
            return;
        }

        int shell = 1;

        while(shell < distance / increment)
        {
            shell = shell * increment + 1;
        }

        while(shell > 0)
        {
            Iterator shell_iterator = begin + shell;

            for(Iterator index = shell_iterator; index != end; ++index)
            {
                for(Iterator insertion = index; insertion >= shell_iterator; insertion -= shell)
                {
                    Iterator insertion_ref = insertion - shell;

                    if(comparator(insertion, insertion_ref))
                    {
                        swap(*insertion, *insertion_ref);
                    }
                    else
                    {
                        break;
                    }
                }
            }

            shell /= increment;
        }
    }

    template<class Iterator, int increment = 3>
    constexpr void shell_sort(Iterator begin, Iterator end)
    {
        auto comparator = [](const Iterator& a, const Iterator& b)
        {
            return *a < *b;
        };

        shell_sort<Iterator, decltype(comparator), increment>(begin, end, comparator);
    }
}

#endif
