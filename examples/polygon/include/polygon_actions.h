#ifndef POLYGON_ACTIONS_H
#define POLYGON_ACTIONS_H

#include "btn_value_template_actions.h"
#include "polygon.h"

class polygon_vertex
{

public:
    polygon_vertex(int index, polygon& ref) :
        _ptr(&ref),
        _index(index)
    {
    }

    [[nodiscard]] const btn::fixed_point& get() const
    {
        return _ptr->vertex(_index);
    }

    void set(const btn::fixed_point& position)
    {
        _ptr->set_vertex(_index, position);
    }

private:
    polygon* _ptr;
    int _index;
};


// vertex position

class polygon_vertex_position_manager
{

public:
    [[nodiscard]] static const btn::fixed_point& get(const polygon_vertex& polygon_vertex)
    {
        return polygon_vertex.get();
    }

    static void set(const btn::fixed_point& position, polygon_vertex& polygon_vertex)
    {
        polygon_vertex.set(position);
    }
};


class polygon_vertex_move_to_action :
        public btn::to_value_template_action<polygon_vertex, btn::fixed_point, polygon_vertex_position_manager>
{

public:
    polygon_vertex_move_to_action(const polygon_vertex& polygon_vertex, int duration_frames,
                                  const btn::fixed_point& final_position) :
        to_value_template_action(polygon_vertex, duration_frames, final_position)
    {
    }

    [[nodiscard]] const polygon_vertex& vertex() const
    {
        return value();
    }

    [[nodiscard]] const btn::fixed_point& final_position() const
    {
        return final_property();
    }
};

#endif
