#include "bf_wave_generator.h"

#include "btn_math.h"
#include "btn_span.h"
#include "btn_fixed.h"

namespace bf
{

namespace
{
    template<int Amplitude>
    void _generate_impl(int speed, btn::span<btn::fixed>& values)
    {
        for(int index = 0, limit = values.size(); index < limit; ++index)
        {
            int sin = btn::lut_sin((index * speed) % 65536).value();
            values[index] = (sin / (4096 / (1 << Amplitude))) - (1 << (Amplitude - 1));
        }
    }
}

void wave_generator::set_speed(int speed)
{
    BTN_ASSERT(speed >= 0, "Invalid speed: ", speed);

    _speed = speed;
}

void wave_generator::set_amplitude(int amplitude)
{
    BTN_ASSERT(amplitude >= 1 && amplitude <= 4, "Invalid amplitude: ", amplitude);

    _amplitude = amplitude;
}

void wave_generator::generate(btn::span<btn::fixed> values) const
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
    }
}

}
