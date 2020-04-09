#ifndef BTN_ALGORITHM_H
#define BTN_ALGORITHM_H

#include <algorithm>
#include "btn_utility.h"

namespace btn
{
    using std::min;
    using std::max;
    using std::clamp;

    using std::distance;
    using std::advance;

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

    template<class Iterator, typename Type>
    constexpr void fill_n(Iterator first, int count, const Type& value)
    {
        if(BTN_CONSTANT_EVALUATED())
        {
            for(int index = 0; index < count; index++)
            {
                *first++ = value;
            }
        }
        else
        {
            std::fill_n(first, count, value);
        }
    }

    template<typename InputIt, typename OutputIt>
    constexpr OutputIt copy(InputIt inputFirst, InputIt inputLast, OutputIt outputFirst)
    {
        if(BTN_CONSTANT_EVALUATED())
        {
            while(inputFirst != inputLast)
            {
                *outputFirst++ = *inputFirst++;
            }

            return outputFirst;
        }
        else
        {
            return std::copy(inputFirst, inputLast, outputFirst);
        }
    }

    template<typename InputIt, typename OutputIt>
    constexpr OutputIt copy_backward(InputIt inputFirst, InputIt inputLast, OutputIt outputLast)
    {
        if(BTN_CONSTANT_EVALUATED())
        {
            while(inputFirst != inputLast)
            {
                *(--outputLast) = *(--inputLast);
            }

            return outputLast;
        }
        else
        {
            return std::copy_backward(inputFirst, inputLast, outputLast);
        }
    }

    template<typename Iterator, typename Type>
    [[nodiscard]] constexpr Iterator find(Iterator first, Iterator last, const Type& value)
    {
        if(BTN_CONSTANT_EVALUATED())
        {
            for(; first != last; ++first)
            {
                if(*first == value)
                {
                    return first;
                }
            }

            return last;
        }
        else
        {
            return std::find(first, last, value);
        }
    }

    template<typename Iterator, class Pred>
    [[nodiscard]] constexpr Iterator find_if(Iterator first, Iterator last, Pred pred)
    {
        if(BTN_CONSTANT_EVALUATED())
        {
            for(; first != last; ++first)
            {
                if(pred(*first))
                {
                    return first;
                }
            }

            return last;
        }
        else
        {
            return std::find_if(first, last, pred);
        }
    }

    template<typename Iterator, typename Type>
    [[nodiscard]] constexpr Iterator lower_bound(Iterator first, Iterator last, const Type& value)
    {
        if(BTN_CONSTANT_EVALUATED())
        {
            auto count = distance(first, last);

            while(count > 0)
            {
                auto step = count / 2;
                Iterator it = first;
                advance(it, step);

                if(*it < value)
                {
                    first = ++it;
                    count -= step + 1;
                }
                else
                {
                    count = step;
                }
            }

            return first;
        }
        else
        {
            return std::lower_bound(first, last, value);
        }
    }

    template<typename Iterator, typename Type, class Comparator>
    [[nodiscard]] constexpr Iterator lower_bound(Iterator first, Iterator last, const Type& value,
                                                 const Comparator& comparator)
    {
        if(BTN_CONSTANT_EVALUATED())
        {
            auto count = distance(first, last);

            while(count > 0)
            {
                auto step = count / 2;
                Iterator it = first;
                advance(it, step);

                if(comparator(*it, value))
                {
                    first = ++it;
                    count -= step + 1;
                }
                else
                {
                    count = step;
                }
            }

            return first;
        }
        else
        {
            return std::lower_bound(first, last, value, comparator);
        }
    }

    template<typename Iterator, typename Type>
    constexpr Iterator remove(Iterator first, Iterator last, const Type& value)
    {
        if(BTN_CONSTANT_EVALUATED())
        {
            first = find(first, last, value);

            if(first != last)
            {
                for(Iterator it = first; ++it != last; )
                {
                    if(! (*it == value))
                    {
                        *first++ = move(*it);
                    }
                }
            }

            return first;
        }
        else
        {
            return std::remove(first, last, value);
        }
    }

    template<typename Iterator, class Pred>
    Iterator remove_if(Iterator first, Iterator last, Pred pred)
    {
        if(BTN_CONSTANT_EVALUATED())
        {
            first = find_if(first, last, pred);

            if(first != last)
            {
                for(Iterator it = first; ++it != last; )
                {
                    if(! pred(*it))
                    {
                        *first++ = move(*it);
                    }
                }
            }

            return first;
        }
        else
        {
            return std::remove_if(first, last, pred);
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
