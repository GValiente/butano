/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_POWER_OF_TWO_H
#define BTN_POWER_OF_TWO_H

#include "btn_common.h"

namespace btn
{

/**
 * @brief Indicates if the given value is a power of two or not.
 *
 * @ingroup math
 */
template<typename Type>
[[nodiscard]] constexpr bool power_of_two(Type value)
{
    if(value > 0)
    {
        while(value % 2 == 0)
        {
            value /= 2;
        }

        if(value == 1)
        {
            return true;
        }
    }

    return false;
}

}

#endif
