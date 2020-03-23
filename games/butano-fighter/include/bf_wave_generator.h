#ifndef BF_WAVE_GENERATOR_H
#define BF_WAVE_GENERATOR_H

#include "btn_span_fwd.h"
#include "btn_fixed_fwd.h"

namespace bf
{

class wave_generator
{

public:
    [[nodiscard]] int speed() const
    {
        return _speed;
    }

    void set_speed(int speed);

    [[nodiscard]] int amplitude() const
    {
        return _amplitude;
    }

    void set_amplitude(int amplitude);

    void generate(btn::span<btn::fixed> values) const;

private:
    int _speed = 1024;
    int _amplitude = 4;
};

}

#endif
