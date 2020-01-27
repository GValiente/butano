#ifndef BTN_TEMPLATE_ACTIONS_H
#define BTN_TEMPLATE_ACTIONS_H

#include "btn_action.h"
#include "btn_assert.h"
#include "btn_utility.h"

namespace btn
{

template<typename Value, typename Property, class PropertyManager>
class by_template_action : public action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
    }

    void update() override
    {
        PropertyManager::set(PropertyManager::get(_value) + _delta_property, _value);
    }

    [[nodiscard]] bool done() const override
    {
        return false;
    }

protected:
    template<class ValueType>
    by_template_action(ValueType&& value, const Property& delta_property) :
        _value(forward<ValueType>(value)),
        _delta_property(delta_property),
        _initial_property(PropertyManager::get(_value))
    {
    }

    [[nodiscard]] const Value& value() const
    {
        return _value;
    }

    [[nodiscard]] const Property& delta_property() const
    {
        return _delta_property;
    }

private:
    Value _value;
    Property _delta_property;
    Property _initial_property;
};


template<typename Value, typename Property, class PropertyManager>
class to_template_action : public action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
        _current_frame = 0;
    }

    void update() override
    {
        BTN_ASSERT(! done(), "Action is done");

        ++_current_frame;

        if(_current_frame == _duration_frames)
        {
            PropertyManager::set(_final_property, _value);
        }
        else
        {
            PropertyManager::set(PropertyManager::get(_value) + _delta_property, _value);
        }
    }

    [[nodiscard]] bool done() const override
    {
        return _current_frame == _duration_frames;
    }

    [[nodiscard]] int duration_frames() const
    {
        return _duration_frames;
    }

protected:
    template<class ValueType>
    to_template_action(ValueType&& value, int duration_frames, const Property& final_property) :
        _value(forward<ValueType>(value)),
        _final_property(final_property),
        _initial_property(PropertyManager::get(_value)),
        _delta_property(_calculate_delta_property(duration_frames)),
        _duration_frames(duration_frames)
    {
    }

    [[nodiscard]] const Value& value() const
    {
        return _value;
    }

    [[nodiscard]] const Property& final_property() const
    {
        return _final_property;
    }

private:
    uint16_t _current_frame = 0;
    Value _value;
    Property _final_property;
    Property _initial_property;
    Property _delta_property;
    int _duration_frames;

    [[nodiscard]] Property _calculate_delta_property(int duration_frames) const
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
        BTN_ASSERT(duration_frames <= numeric_limits<decltype(_current_frame)>::max(),
                   "Too much duration frames: ", duration_frames);

        return (_final_property - _initial_property) / duration_frames;
    }
};


template<typename Value, typename Property, class PropertyManager>
class loop_template_action : public action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
        _current_frame = 0;
        _reverse = false;
    }

    void update() override
    {
        ++_current_frame;

        if(_current_frame == _duration_frames)
        {
            _current_frame = 0;

            if(_reverse)
            {
                PropertyManager::set(_initial_property, _value);
                _reverse = false;
            }
            else
            {
                PropertyManager::set(_final_property, _value);
                _reverse = true;
            }
        }
        else
        {
            if(_reverse)
            {
                PropertyManager::set(PropertyManager::get(_value) - _delta_property, _value);
            }
            else
            {
                PropertyManager::set(PropertyManager::get(_value) + _delta_property, _value);
            }
        }
    }

    [[nodiscard]] bool done() const override
    {
        return false;
    }

    [[nodiscard]] int duration_frames() const
    {
        return _duration_frames;
    }

protected:
    template<class ValueType>
    loop_template_action(ValueType&& value, int duration_frames, const Property& final_property) :
        _value(forward<ValueType>(value)),
        _final_property(final_property),
        _initial_property(PropertyManager::get(_value)),
        _delta_property(_calculate_delta_property(duration_frames)),
        _duration_frames(duration_frames)
    {
    }

    [[nodiscard]] const Value& value() const
    {
        return _value;
    }

    [[nodiscard]] const Property& final_property() const
    {
        return _final_property;
    }

private:
    bool _reverse = false;
    uint16_t _current_frame = 0;
    Value _value;
    Property _final_property;
    Property _initial_property;
    Property _delta_property;
    int _duration_frames;

    [[nodiscard]] Property _calculate_delta_property(int duration_frames) const
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
        BTN_ASSERT(duration_frames <= numeric_limits<decltype(_current_frame)>::max(),
                   "Too much duration frames: ", duration_frames);

        return (_final_property - _initial_property) / duration_frames;
    }
};

}

#endif
