#ifndef POLYGON_H
#define POLYGON_H

#include "bn_span.h"
#include "bn_vector.h"
#include "bn_fixed_point.h"

class polygon
{

public:
    explicit polygon(const bn::span<const bn::fixed_point>& vertices)
    {
        for(const bn::fixed_point& vertex : vertices)
        {
            _vertices.push_back(vertex);
        }
    }

    [[nodiscard]] const bn::ivector<bn::fixed_point>& vertices() const
    {
        return _vertices;
    }

    [[nodiscard]] bn::ivector<bn::fixed_point>& vertices()
    {
        return _vertices;
    }

private:
    bn::vector<bn::fixed_point, 4> _vertices;
};

#endif
