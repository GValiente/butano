/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_RANDOM_CHOICE_H
#define FR_RANDOM_CHOICE_H

#include "bn_random.h"

namespace fr
{

class random_choice
{

public:
    template<unsigned NumChoices>
    [[nodiscard]] constexpr unsigned get()
    {
        static_assert(NumChoices > 1);

        unsigned result = _random.get() % NumChoices;
        unsigned last_result = _last_result;

        while(result == last_result)
        {
            result = _random.get() % NumChoices;
        }

        _last_result = result;
        return result;
    }

private:
    bn::random _random;
    unsigned _last_result = 0;
};

}

#endif
