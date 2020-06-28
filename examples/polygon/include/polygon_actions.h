#ifndef POLYGON_ACTIONS_H
#define POLYGON_ACTIONS_H

#include "btn_value_template_actions.h"
#include "polygon.h"

class polygon_ptr
{

public:
    polygon_ptr(polygon& ref) :
        _ptr(&ref)
    {
    }

    [[nodiscard]] const polygon& operator*() const
    {
        return *_ptr;
    }

    [[nodiscard]] polygon& operator*()
    {
        return *_ptr;
    }

    [[nodiscard]] const polygon* operator->() const
    {
        return _ptr;
    }

    [[nodiscard]] polygon* operator->()
    {
        return _ptr;
    }

private:
    polygon* _ptr;
};


// panning

class polygon_panning_manager
{

public:
    [[nodiscard]] static btn::fixed get(const polygon_ptr& polygon_ptr)
    {
        return polygon_ptr->panning();
    }

    static void set(btn::fixed panning, polygon_ptr& polygon_ptr)
    {
        polygon_ptr->set_panning(panning);
    }
};


class polygon_panning_to_action :
        public btn::to_value_template_action<polygon_ptr, btn::fixed, polygon_panning_manager>
{

public:
    polygon_panning_to_action(const polygon_ptr& polygon, int duration_frames, btn::fixed final_panning) :
        to_value_template_action(polygon, duration_frames, final_panning)
    {
    }

    [[nodiscard]] const polygon_ptr& polygon() const
    {
        return value();
    }

    [[nodiscard]] btn::fixed final_panning() const
    {
        return final_property();
    }
};


// scale_x

class polygon_scale_x_manager
{

public:
    [[nodiscard]] static btn::fixed get(const polygon_ptr& polygon_ptr)
    {
        return polygon_ptr->scale_x();
    }

    static void set(btn::fixed scale_x, polygon_ptr& polygon_ptr)
    {
        polygon_ptr->set_scale_x(scale_x);
    }
};


class polygon_scale_x_to_action :
        public btn::to_value_template_action<polygon_ptr, btn::fixed, polygon_scale_x_manager>
{

public:
    polygon_scale_x_to_action(const polygon_ptr& polygon, int duration_frames, btn::fixed final_scale_x) :
        to_value_template_action(polygon, duration_frames, final_scale_x)
    {
    }

    [[nodiscard]] const polygon_ptr& polygon() const
    {
        return value();
    }

    [[nodiscard]] btn::fixed final_scale_x() const
    {
        return final_property();
    }
};


// scale_y

class polygon_scale_y_manager
{

public:
    [[nodiscard]] static btn::fixed get(const polygon_ptr& polygon_ptr)
    {
        return polygon_ptr->scale_y();
    }

    static void set(btn::fixed scale_y, polygon_ptr& polygon_ptr)
    {
        polygon_ptr->set_scale_y(scale_y);
    }
};


class polygon_scale_y_to_action :
        public btn::to_value_template_action<polygon_ptr, btn::fixed, polygon_scale_y_manager>
{

public:
    polygon_scale_y_to_action(const polygon_ptr& polygon, int duration_frames, btn::fixed final_scale_y) :
        to_value_template_action(polygon, duration_frames, final_scale_y)
    {
    }

    [[nodiscard]] const polygon_ptr& polygon() const
    {
        return value();
    }

    [[nodiscard]] btn::fixed final_scale_y() const
    {
        return final_property();
    }
};

#endif
