#ifndef BTN_VALUE_TEMPLATE_ACTIONS_H
#define BTN_VALUE_TEMPLATE_ACTIONS_H

#include "btn_assert.h"
#include "btn_utility.h"

namespace btn
{

template<typename Value, typename Property, class PropertyManager>
class by_value_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
    }

    void update()
    {
        PropertyManager::set(PropertyManager::get(_value) + _delta_property, _value);
    }

protected:
    by_value_template_action(const Value& value, const Property& delta_property) :
        _value(value),
        _delta_property(delta_property),
        _initial_property(PropertyManager::get(_value))
    {
    }

    by_value_template_action(Value&& value, const Property& delta_property) :
        _value(move(value)),
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
class cyclic_by_value_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
    }

    void update()
    {
        Property new_property = PropertyManager::get(_value) + _delta_property;

        if(new_property < _min_property)
        {
            new_property += _after_max_property - _min_property;
        }
        else if(new_property >= _after_max_property)
        {
            new_property -= _after_max_property - _min_property;
        }

        PropertyManager::set(new_property, _value);
    }

protected:
    cyclic_by_value_template_action(const Value& value, const Property& delta_property, const Property& min_property,
                                    const Property& after_max_property) :
        _value(value),
        _delta_property(delta_property),
        _min_property(min_property),
        _after_max_property(after_max_property),
        _initial_property(PropertyManager::get(_value))
    {
    }

    cyclic_by_value_template_action(Value&& value, const Property& delta_property, const Property& min_property,
                                    const Property& after_max_property) :
        _value(move(value)),
        _delta_property(delta_property),
        _min_property(min_property),
        _after_max_property(after_max_property),
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

    void set_min_property(const Property& min_property)
    {
        _min_property = min_property;
    }

    void set_after_max_property(const Property& after_max_property)
    {
        _after_max_property = after_max_property;
    }

private:
    Value _value;
    Property _delta_property;
    Property _min_property;
    Property _after_max_property;
    Property _initial_property;
};


template<typename Value, typename Property, class PropertyManager>
class duration_by_value_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
        _current_frame = 0;
    }

    void update()
    {
        if(_current_frame == _duration_frames - 1)
        {
            PropertyManager::set(PropertyManager::get(_value) + _delta_property, _value);
            _current_frame = 0;
        }
        else
        {
            ++_current_frame;
        }
    }

    [[nodiscard]] int duration_frames() const
    {
        return _duration_frames;
    }

protected:
    duration_by_value_template_action(const Value& value, int duration_frames, const Property& delta_property) :
        _value(value),
        _delta_property(delta_property),
        _initial_property(PropertyManager::get(_value)),
        _duration_frames(duration_frames)
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
    }

    duration_by_value_template_action(Value&& value, int duration_frames, const Property& delta_property) :
        _value(move(value)),
        _delta_property(delta_property),
        _initial_property(PropertyManager::get(_value)),
        _duration_frames(duration_frames)
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
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
    uint16_t _current_frame = 0;
    Value _value;
    Property _delta_property;
    Property _initial_property;
    int _duration_frames;
};


template<typename Value, typename Property, class PropertyManager>
class cyclic_duration_by_value_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
    }

    void update()
    {
        if(_current_frame == _duration_frames - 1)
        {
            Property new_property = PropertyManager::get(_value) + _delta_property;

            if(new_property < _min_property)
            {
                new_property += _after_max_property - _min_property;
            }
            else if(new_property >= _after_max_property)
            {
                new_property -= _after_max_property - _min_property;
            }

            PropertyManager::set(new_property, _value);
            _current_frame = 0;
        }
        else
        {
            ++_current_frame;
        }
    }

    [[nodiscard]] int duration_frames() const
    {
        return _duration_frames;
    }

protected:
    cyclic_duration_by_value_template_action(const Value& value, int duration_frames, const Property& delta_property,
                                             const Property& min_property, const Property& after_max_property) :
        _value(value),
        _delta_property(delta_property),
        _min_property(min_property),
        _after_max_property(after_max_property),
        _initial_property(PropertyManager::get(_value)),
        _duration_frames(duration_frames)
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
    }

    cyclic_duration_by_value_template_action(Value&& value, int duration_frames, const Property& delta_property,
                                             const Property& min_property, const Property& after_max_property) :
        _value(move(value)),
        _delta_property(delta_property),
        _min_property(min_property),
        _after_max_property(after_max_property),
        _initial_property(PropertyManager::get(_value)),
        _duration_frames(duration_frames)
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
    }

    [[nodiscard]] const Value& value() const
    {
        return _value;
    }

    [[nodiscard]] const Property& delta_property() const
    {
        return _delta_property;
    }

    void set_min_property(const Property& min_property)
    {
        _min_property = min_property;
    }

    void set_after_max_property(const Property& after_max_property)
    {
        _after_max_property = after_max_property;
    }

private:
    uint16_t _current_frame = 0;
    Value _value;
    Property _delta_property;
    Property _min_property;
    Property _after_max_property;
    Property _initial_property;
    int _duration_frames;
};


template<typename Value, typename Property, class PropertyManager>
class to_value_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
        _current_frame = 0;
    }

    void update()
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

    [[nodiscard]] bool done() const
    {
        return _current_frame == _duration_frames;
    }

    [[nodiscard]] int duration_frames() const
    {
        return _duration_frames;
    }

protected:
    to_value_template_action(const Value& value, int duration_frames, const Property& final_property) :
        _value(value),
        _final_property(final_property),
        _initial_property(PropertyManager::get(_value)),
        _delta_property(_calculate_delta_property(duration_frames)),
        _duration_frames(duration_frames)
    {
    }

    to_value_template_action(Value&& value, int duration_frames, const Property& final_property) :
        _value(move(value)),
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
class loop_value_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
        _current_frame = 0;
        _reverse = false;
    }

    void update()
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

    [[nodiscard]] int duration_frames() const
    {
        return _duration_frames;
    }

protected:
    loop_value_template_action(const Value& value, int duration_frames, const Property& final_property) :
        _value(value),
        _final_property(final_property),
        _initial_property(PropertyManager::get(_value)),
        _delta_property(_calculate_delta_property(duration_frames)),
        _duration_frames(duration_frames)
    {
    }

    loop_value_template_action(Value&& value, int duration_frames, const Property& final_property) :
        _value(move(value)),
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


template<typename Value, typename Property, class PropertyManager>
class toggle_value_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
        _current_frame = 0;
        _reverse = false;
    }

    void update()
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
                PropertyManager::set(_new_property, _value);
                _reverse = true;
            }
        }
    }

    [[nodiscard]] int duration_frames() const
    {
        return _duration_frames;
    }

protected:
    toggle_value_template_action(const Value& value, int duration_frames, const Property& new_property) :
        _value(value),
        _initial_property(PropertyManager::get(_value)),
        _new_property(new_property),
        _duration_frames(duration_frames)
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
    }

    toggle_value_template_action(Value&& value, int duration_frames, const Property& new_property) :
        _value(move(value)),
        _initial_property(PropertyManager::get(_value)),
        _new_property(new_property),
        _duration_frames(duration_frames)
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
    }

    [[nodiscard]] const Value& value() const
    {
        return _value;
    }

    [[nodiscard]] const Property& new_property() const
    {
        return _new_property;
    }

private:
    bool _reverse = false;
    uint16_t _current_frame = 0;
    Value _value;
    Property _initial_property;
    Property _new_property;
    int _duration_frames;
};


template<typename Value, class PropertyManager>
class bool_toggle_value_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
        _current_frame = 0;
        _reverse = false;
    }

    void update()
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
                PropertyManager::set(! _initial_property, _value);
                _reverse = true;
            }
        }
    }

    [[nodiscard]] int duration_frames() const
    {
        return _duration_frames;
    }

protected:
    bool_toggle_value_template_action(const Value& value, int duration_frames) :
        _initial_property(PropertyManager::get(value)),
        _value(value),
        _duration_frames(duration_frames)
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
    }

    bool_toggle_value_template_action(Value&& value, int duration_frames) :
        _initial_property(PropertyManager::get(value)),
        _value(move(value)),
        _duration_frames(duration_frames)
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
    }

    [[nodiscard]] const Value& value() const
    {
        return _value;
    }

private:
    bool _reverse = false;
    bool _initial_property;
    uint16_t _current_frame = 0;
    Value _value;
    int _duration_frames;
};

}

#endif
