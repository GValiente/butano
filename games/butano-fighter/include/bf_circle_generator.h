#ifndef BF_CIRCLE_GENERATOR_H
#define BF_CIRCLE_GENERATOR_H

#include "btn_fixed.h"
#include "btn_memory.h"
#include "btn_span_fwd.h"

namespace bf
{

class circle_generator
{

public:
    [[nodiscard]] btn::fixed origin_y() const
    {
        return _origin_y;
    }

    void set_origin_y(btn::fixed origin_y)
    {
        _origin_y = origin_y;
    }

    [[nodiscard]] btn::fixed radius() const
    {
        return _radius;
    }

    void set_radius(btn::fixed radius);

    void generate(btn::span<btn::pair<btn::fixed, btn::fixed>> values) const;

private:
    btn::fixed _origin_y;
    btn::fixed _radius = 1;
};

}

#endif
