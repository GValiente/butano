#ifndef POLYGON_H
#define POLYGON_H

#include "btn_span.h"
#include "btn_vector.h"
#include "btn_fixed_point.h"

class polygon
{

public:
    explicit polygon(const btn::span<const btn::fixed_point>& vertices)
    {
        for(const btn::fixed_point& vertex : vertices)
        {
            _vertices.push_back(vertex);
        }
    }

    [[nodiscard]] const btn::ivector<btn::fixed_point>& vertices() const
    {
        return _vertices;
    }

    [[nodiscard]] btn::ivector<btn::fixed_point>& vertices()
    {
        return _vertices;
    }

private:
    btn::vector<btn::fixed_point, 4> _vertices;
};

#endif
