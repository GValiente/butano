#ifndef BTN_UTILITY_H
#define BTN_UTILITY_H

#include <utility>
#include "btn_type_traits.h"

namespace btn
{
    using std::move;

    using std::forward;

    using std::swap;


    template<typename Type1, typename Type2>
    class pair
    {

    public:
        using first_type = Type1;
        using second_type = Type2;

        first_type first;
        second_type second;

        constexpr pair() = default;

        constexpr pair(const first_type& x, const second_type& y) :
            first(x),
            second(y)
        {
        }

        constexpr pair(first_type&& x, const second_type& y) :
            first(move(x)),
            second(y)
        {
        }

        constexpr pair(const first_type& x, second_type&& y) :
            first(x),
            second(move(y))
        {
        }

        constexpr pair(first_type&& x, second_type&& y) :
            first(move(x)),
            second(move(y))
        {
        }

        constexpr void swap(pair& other)
        {
            btn::swap(first, other.first);
            btn::swap(second, other.second);
        }

        constexpr friend void swap(pair& a, pair& b)
        {
            a.swap(b);
        }

        [[nodiscard]] constexpr friend bool operator==(const pair& a, const pair& b)
        {
            return a.first == b.first && a.second == b.second;
        }

        [[nodiscard]] constexpr friend bool operator!=(const pair& a, const pair& b)
        {
            return ! (a == b);
        }

        [[nodiscard]] constexpr friend bool operator<(const pair& a, const pair& b)
        {
            if(a.first < b.first)
            {
                return true;
            }

            if(b.first < a.first)
            {
                return false;
            }

            return a.second < b.second;
        }

        [[nodiscard]] constexpr friend bool operator>(const pair& a, const pair& b)
        {
            return b < a;
        }

        [[nodiscard]] constexpr friend bool operator<=(const pair& a, const pair& b)
        {
            return ! (a > b);
        }

        [[nodiscard]] constexpr friend bool operator>=(const pair& a, const pair& b)
        {
            return ! (a < b);
        }
    };


    template<typename Type1, typename Type2>
    constexpr pair<typename decay<Type1>::type, typename decay<Type2>::type> make_pair(Type1&& x, Type2&& y)
    {
        return pair<typename decay<Type1>::type, typename decay<Type2>::type>(forward<Type1>(x), forward<Type2>(y));
    }
}

#endif
