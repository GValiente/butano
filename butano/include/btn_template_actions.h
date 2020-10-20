/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_TEMPLATE_ACTIONS_H
#define BTN_TEMPLATE_ACTIONS_H

#include "btn_assert.h"
#include "btn_limits.h"

namespace btn
{

template<typename Property, class PropertyManager>
class by_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property);
    }

    void update()
    {
        PropertyManager::set(PropertyManager::get() + _delta_property);
    }

protected:
    explicit by_template_action(const Property& delta_property) :
        _delta_property(delta_property),
        _initial_property(PropertyManager::get())
    {
    }

    [[nodiscard]] const Property& delta_property() const
    {
        return _delta_property;
    }

private:
    Property _delta_property;
    Property _initial_property;
};


template<typename Property, class PropertyManager>
class cyclic_by_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property);
    }

    void update()
    {
        Property new_property = PropertyManager::get() + _delta_property;

        if(new_property < _min_property)
        {
            new_property += _after_max_property - _min_property;
        }
        else if(new_property >= _after_max_property)
        {
            new_property -= _after_max_property - _min_property;
        }

        PropertyManager::set(new_property);
    }

protected:
    cyclic_by_template_action(const Property& delta_property, const Property& min_property,
                              const Property& after_max_property) :
        _delta_property(delta_property),
        _min_property(min_property),
        _after_max_property(after_max_property),
        _initial_property(PropertyManager::get())
    {
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
    Property _delta_property;
    Property _min_property;
    Property _after_max_property;
    Property _initial_property;
};


template<typename Property, class PropertyManager>
class duration_by_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property);
        _current_frame = 0;
    }

    void update()
    {
        if(_current_frame == _duration_frames - 1)
        {
            PropertyManager::set(PropertyManager::get() + _delta_property);
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
    duration_by_template_action(int duration_frames, const Property& delta_property) :
        _delta_property(delta_property),
        _initial_property(PropertyManager::get()),
        _duration_frames(duration_frames)
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
    }

    [[nodiscard]] const Property& delta_property() const
    {
        return _delta_property;
    }

private:
    uint16_t _current_frame = 0;
    Property _delta_property;
    Property _initial_property;
    int _duration_frames;
};


template<typename Property, class PropertyManager>
class cyclic_duration_by_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property);
    }

    void update()
    {
        if(_current_frame == _duration_frames - 1)
        {
            Property new_property = PropertyManager::get() + _delta_property;

            if(new_property < _min_property)
            {
                new_property += _after_max_property - _min_property;
            }
            else if(new_property >= _after_max_property)
            {
                new_property -= _after_max_property - _min_property;
            }

            PropertyManager::set(new_property);
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
    cyclic_duration_by_template_action(int duration_frames, const Property& delta_property,
                                       const Property& min_property, const Property& after_max_property) :
        _delta_property(delta_property),
        _min_property(min_property),
        _after_max_property(after_max_property),
        _initial_property(PropertyManager::get()),
        _duration_frames(duration_frames)
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
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
    Property _delta_property;
    Property _min_property;
    Property _after_max_property;
    Property _initial_property;
    int _duration_frames;
};


template<typename Property, class PropertyManager>
class to_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property);
        _current_frame = 0;
    }

    void update()
    {
        BTN_ASSERT(! done(), "Action is done");

        ++_current_frame;

        if(_current_frame == _duration_frames)
        {
            PropertyManager::set(_final_property);
        }
        else
        {
            PropertyManager::set(PropertyManager::get() + _delta_property);
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
    to_template_action(int duration_frames, const Property& final_property) :
        _final_property(final_property),
        _initial_property(PropertyManager::get()),
        _delta_property(_calculate_delta_property(duration_frames)),
        _duration_frames(duration_frames)
    {
    }

    [[nodiscard]] const Property& final_property() const
    {
        return _final_property;
    }

private:
    uint16_t _current_frame = 0;
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


template<typename Property, class PropertyManager>
class loop_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property);
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
                PropertyManager::set(_initial_property);
                _reverse = false;
            }
            else
            {
                PropertyManager::set(_final_property);
                _reverse = true;
            }
        }
        else
        {
            if(_reverse)
            {
                PropertyManager::set(PropertyManager::get() - _delta_property);
            }
            else
            {
                PropertyManager::set(PropertyManager::get() + _delta_property);
            }
        }
    }

    [[nodiscard]] int duration_frames() const
    {
        return _duration_frames;
    }

protected:
    loop_template_action(int duration_frames, const Property& final_property) :
        _final_property(final_property),
        _initial_property(PropertyManager::get()),
        _delta_property(_calculate_delta_property(duration_frames)),
        _duration_frames(duration_frames)
    {
    }

    [[nodiscard]] const Property& final_property() const
    {
        return _final_property;
    }

private:
    bool _reverse = false;
    uint16_t _current_frame = 0;
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


template<typename Property, class PropertyManager>
class toggle_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property);
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
                PropertyManager::set(_initial_property);
                _reverse = false;
            }
            else
            {
                PropertyManager::set(_new_property);
                _reverse = true;
            }
        }
    }

    [[nodiscard]] int duration_frames() const
    {
        return _duration_frames;
    }

protected:
    toggle_template_action(int duration_frames, const Property& new_property) :
        _initial_property(PropertyManager::get()),
        _new_property(new_property),
        _duration_frames(duration_frames)
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
    }

    [[nodiscard]] const Property& new_property() const
    {
        return _new_property;
    }

private:
    bool _reverse = false;
    uint16_t _current_frame = 0;
    Property _initial_property;
    Property _new_property;
    int _duration_frames;
};


template<class PropertyManager>
class bool_toggle_template_action
{

public:
    void reset()
    {
        PropertyManager::set(_initial_property);
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
                PropertyManager::set(_initial_property);
                _reverse = false;
            }
            else
            {
                PropertyManager::set(! _initial_property);
                _reverse = true;
            }
        }
    }

    [[nodiscard]] int duration_frames() const
    {
        return _duration_frames;
    }

protected:
    explicit bool_toggle_template_action(int duration_frames) :
        _initial_property(PropertyManager::get()),
        _duration_frames(duration_frames)
    {
        BTN_ASSERT(duration_frames > 0, "Invalid duration frames: ", duration_frames);
    }

private:
    bool _reverse = false;
    bool _initial_property;
    uint16_t _current_frame = 0;
    int _duration_frames;
};

}

#endif
