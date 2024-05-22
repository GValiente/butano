/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef RANDOM_TESTS_H
#define RANDOM_TESTS_H

#include "bn_random.h"
#include "bn_seed_random.h"
#include "bn_unordered_set.h"
#include "tests.h"

class random_tests : public tests
{

public:
    random_tests() :
        tests("random")
    {
        constexpr int its = 256;

        bn::random random;
        bn::seed_random seed_random;
        bn::unordered_set<unsigned, its * 4> set;

        for(int it = 0; it < its; ++it)
        {
            BN_ASSERT(set.insert(random.get()) != set.end());
            BN_ASSERT(set.insert(seed_random.get()) != set.end());
        }
    }
};

#endif
