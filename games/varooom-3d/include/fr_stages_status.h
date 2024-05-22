/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
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

    [[nodiscard]] constexpr bn::optional<int> best_time(difficulty_level difficulty, bool reverse) const
    {
        bn::optional<int> result;

        if(int value = _best_time[_stage_index(difficulty, reverse)])
        {
            result = value - 1;
        }

        return result;
    }

    [[nodiscard]] constexpr static bool dollar_time(difficulty_level difficulty, bool reverse, int time)
    {
        int seconds = 0;
        int hundredths = 0;

        if(reverse)
        {
            switch(difficulty)
            {

            case difficulty_level::EASY:
                seconds = 16 + 3;
                hundredths = 13;
                break;

            case difficulty_level::NORMAL:
                seconds = 28 + 3;
                hundredths = 60;
                break;

            case difficulty_level::HARD:
                seconds = 5 + 3;
                hundredths = 56;
                break;

            default:
                BN_ERROR("Invalid difficulty: ", int(difficulty));
                break;
            }
        }
        else
        {
            switch(difficulty)
            {

            case difficulty_level::EASY:
                seconds = 5 + 3;
                hundredths = 86;
                break;

            case difficulty_level::NORMAL:
                seconds = 25 + 3;
                hundredths = 6;
                break;

            case difficulty_level::HARD:
                seconds = 33 + 3;
                hundredths = 63;
                break;

            default:
                BN_ERROR("Invalid difficulty: ", int(difficulty));
                break;
            }
        }

        int frames = (hundredths * 60) / 100;
        frames += seconds * 60;
        frames += 3600;

        return time <= frames;
    }

    constexpr void set_best_position_and_time(difficulty_level difficulty, bool reverse,
                                              int best_position, int best_time)
    {
        BN_ASSERT(best_position >= 0 && best_position < 99, "Invalid best position: ", best_position);
        BN_ASSERT(best_time >= 0, "Invalid best time: ", best_time);

        int index = _stage_index(difficulty, reverse);
        _best_position[index] = best_position + 1;
        _best_time[index] = best_time + 1;
    }

private:
    int _best_time[6] = {};
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
