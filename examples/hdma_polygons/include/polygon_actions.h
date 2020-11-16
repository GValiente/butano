#ifndef POLYGON_ACTIONS_H
#define POLYGON_ACTIONS_H

#include "bn_value_template_actions.h"
#include "polygon.h"

class polygon_vertex
{

public:
    polygon_vertex(int index, polygon& ref) :
        _ptr(&ref),
        _index(index)
    {
    }

    [[nodiscard]] const bn::fixed_point& get() const
    {
        return _ptr->vertices()[_index];
    }

    void set(const bn::fixed_point& position)
    {
        _ptr->vertices()[_index] = position;
    }

private:
    polygon* _ptr;
    int _index;
};


// vertex position

class polygon_vertex_position_manager
{

public:
    [[nodiscard]] static const bn::fixed_point& get(const polygon_vertex& polygon_vertex)
    {
        return polygon_vertex.get();
    }

    static void set(const bn::fixed_point& position, polygon_vertex& polygon_vertex)
    {
        polygon_vertex.set(position);
    }
};


class polygon_vertex_move_to_action :
        public bn::to_value_template_action<polygon_vertex, bn::fixed_point, polygon_vertex_position_manager>
{

public:
    polygon_vertex_move_to_action(const polygon_vertex& polygon_vertex, int duration_frames,
                                  const bn::fixed_point& final_position) :
        to_value_template_action(polygon_vertex, duration_frames, final_position)
    {
    }

    [[nodiscard]] const polygon_vertex& vertex() const
    {
        return value();
    }

    [[nodiscard]] const bn::fixed_point& final_position() const
    {
        return final_property();
    }
};

#endif
