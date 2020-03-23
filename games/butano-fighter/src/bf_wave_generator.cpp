#include "bf_wave_generator.h"

#include "btn_math.h"
#include "btn_span.h"
#include "btn_fixed.h"

namespace bf
{

void wave_generator::set_speed(int speed)
{
    BTN_ASSERT(speed >= 0, "Invalid speed: ", speed);

    _speed = speed;
}

void wave_generator::set_amplitude(int amplitude)
{
    BTN_ASSERT(amplitude >= 0, "Invalid amplitude: ", amplitude);

    _amplitude = amplitude;
}

void wave_generator::generate(btn::span<btn::fixed> values) const
{
    int speed = _speed;
    int amplitude = _amplitude;

    for(int index = 0, limit = values.size(); index < limit; ++index)
    {
        int sin = btn::lut_sin((index * speed) % 65536).value();
        values[index] = (sin / (4096 / (1 << amplitude))) - (1 << (amplitude - 1));
    }
}

}
