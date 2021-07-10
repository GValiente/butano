/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_STAGES_STATUS_H
#define FR_STAGES_STATUS_H

#include "bn_optional.h"

#include "fr_difficulty_level.h"

namespace fr
{

class stages_status
{

public:
    constexpr void init()
    {
        _unlocked[_stage_index(difficulty_level::EASY, false)] = true;
    }

    [[nodiscard]] constexpr bool unlocked_stage(difficulty_level difficulty, bool reverse) const
    {
        return _unlocked[_stage_index(difficulty, reverse)];
    }

    [[nodiscard]] constexpr bool unlock_stage(difficulty_level difficulty, bool reverse)
    {
        int8_t& unlocked = _unlocked[_stage_index(difficulty, reverse)];
        bool result = ! unlocked;
        unlocked = 1;
        return result;
    }

    [[nodiscard]] constexpr bn::optional<int> best_position(difficulty_level difficulty, bool reverse) const
    {
        bn::optional<int> result;

        if(int value = _best_position[_stage_index(difficulty, reverse)])
        {
            result = value - 1;
        }

        return result;
    }

    constexpr void set_best_position(difficulty_level difficulty, bool reverse, int best_position)
    {
        BN_ASSERT(best_position >= 0 && best_position < 99, "Invalid best position: ", best_position);

        _best_position[_stage_index(difficulty, reverse)] = best_position + 1;
    }

private:
    int8_t _unlocked[6] = {};
    int8_t _best_position[6] = {};

    [[nodiscard]] constexpr static int _stage_index(difficulty_level difficulty, bool reverse)
    {
        int base_index = reverse ? 3 : 0;
        int result = base_index + int(difficulty);
        BN_ASSERT(result >= 0 && result < 6, "Invalid stage index: ", result);

        return result;
    }
};

}

#endif
