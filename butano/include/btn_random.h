#ifndef BTN_RANDOM_H
#define BTN_RANDOM_H

#include "btn_common.h"

namespace btn
{

/**
 * @brief Fast, deterministic random number generator.
 *
 * https://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
 */
class random
{

public:
    constexpr random() :
        _x(123456789),
        _y(362436069),
        _z(521288629)
    {
    }

    [[nodiscard]] constexpr unsigned get()
    {
        _x ^= _x << 16;
        _x ^= _x >> 5;
        _x ^= _x << 1;

        unsigned t = _x;
        _x = _y;
        _y = _z;
        _z = t ^ _x ^ _y;
        return _z;
    }

private:
    unsigned _x;
    unsigned _y;
    unsigned _z;
};

}

#endif
