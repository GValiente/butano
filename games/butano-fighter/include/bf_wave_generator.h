/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_WAVE_GENERATOR_H
#define BF_WAVE_GENERATOR_H

#include "bn_math.h"
#include "bn_span.h"

namespace bf
{

class wave_generator
{

public:
    [[nodiscard]] constexpr int speed() const
    {
        return _speed;
    }

    constexpr void set_speed(int speed)
    {
        BN_ASSERT(speed >= 0, "Invalid speed: ", speed);

        _speed = speed;
    }

    [[nodiscard]] constexpr int amplitude() const
    {
        return _amplitude;
    }

    constexpr void set_amplitude(int amplitude)
    {
        BN_ASSERT(amplitude >= 1 && amplitude <= 4, "Invalid amplitude: ", amplitude);

        _amplitude = amplitude;
    }

    constexpr void generate(bn::span<bn::fixed> values) const
    {
        switch(_amplitude)
        {

        case 1:
            _generate_impl<1>(_speed, values);
            break;

        case 2:
            _generate_impl<2>(_speed, values);
            break;

        case 3:
            _generate_impl<3>(_speed, values);
            break;

        case 4:
            _generate_impl<4>(_speed, values);
            break;

        default:
            BN_ERROR("Invalid amplitude: ", _amplitude);
            break;
        }
    }

private:
    int _speed = 1024;
    int _amplitude = 4;

    template<int Amplitude>
    constexpr static void _generate_impl(int speed, bn::span<bn::fixed>& values)
    {
        int a = (4096 / (1 << Amplitude));
        int b = (1 << (Amplitude - 1));
        bn::fixed* values_data = values.data();

        for(int index = 0, limit = values.size(); index < limit; ++index)
        {
            int lut_angle = int(uint16_t(index * speed)) >> 5;
            int sin = bn::lut_sin(lut_angle).data();
            values_data[index] = (sin / a) - b;
        }
    }
};

}

#endif
