/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_CAR_ENGINE_H
#define FR_CAR_ENGINE_H

#include "bn_fixed.h"

#include "fr_difficulty_level.h"

namespace fr
{

class car_engine
{

public:
    constexpr car_engine(bn::fixed max_power, bn::fixed max_turbo_power, bn::fixed acceleration) :
        _max_power(max_power),
        _max_turbo_power(max_turbo_power),
        _max_reverse_power(max_power / 4),
        _acceleration(acceleration),
        _turbo_acceleration(acceleration * 8),
        _reverse_acceleration(acceleration.unsafe_multiplication(bn::fixed(0.65)))
    {
    }

    [[nodiscard]] constexpr bn::fixed max_power() const
    {
        return _max_power;
    }

    [[nodiscard]] constexpr bn::fixed max_turbo_power() const
    {
        return _max_turbo_power;
    }

    [[nodiscard]] constexpr bn::fixed max_reverse_power() const
    {
        return _max_reverse_power;
    }

    [[nodiscard]] constexpr bn::fixed acceleration() const
    {
        return _acceleration;
    }

    [[nodiscard]] constexpr bn::fixed turbo_acceleration() const
    {
        return _turbo_acceleration;
    }

    [[nodiscard]] constexpr bn::fixed reverse_acceleration() const
    {
        return _reverse_acceleration;
    }

private:
    bn::fixed _max_power;
    bn::fixed _max_turbo_power;
    bn::fixed _max_reverse_power;
    bn::fixed _acceleration;
    bn::fixed _turbo_acceleration;
    bn::fixed _reverse_acceleration;
};

}

#endif
