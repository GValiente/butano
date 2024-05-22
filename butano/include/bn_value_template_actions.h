/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_VALUE_TEMPLATE_ACTIONS_H
#define BN_VALUE_TEMPLATE_ACTIONS_H

/**
 * @file
 * Value template actions header file.
 *
 * @ingroup template_action
 */

#include "bn_assert.h"
#include "bn_limits.h"
#include "bn_utility.h"

namespace bn
{

/**
 * @brief Modifies the property of a value by a given delta.
 *
 * @tparam Value Value to modify.
 * @tparam Property Property of the value to modify.
 * @tparam PropertyManager Reads and writes the property of the value to modify.
 *
 * @ingroup template_action
 */
template<typename Value, typename Property, class PropertyManager>
class by_value_template_action
{

public:
    /**
     * @brief Resets the property to its initial state.
     */
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
    }

    /**
     * @brief Adds delta_property to the property.
     */
    void update()
    {
        PropertyManager::set(PropertyManager::get(_value) + _delta_property, _value);
    }

protected:
    /**
     * @brief Constructor.
     * @param value Value to copy.
     * @param delta_property How much to add to the property when the action is updated.
     */
    by_value_template_action(const Value& value, const Property& delta_property) :
        _value(value),
        _delta_property(delta_property),
        _initial_property(PropertyManager::get(_value))
    {
    }

    /**
     * @brief Constructor.
     * @param value Value to move.
     * @param delta_property How much to add to the property when the action is updated.
     */
    by_value_template_action(Value&& value, const Property& delta_property) :
        _value(move(value)),
        _delta_property(delta_property),
        _initial_property(PropertyManager::get(_value))
    {
    }

    /**
     * @brief Returns the value to modify.
     */
    [[nodiscard]] const Value& value() const
    {
        return _value;
    }

    /**
     * @brief Returns how much to add to the property when the action is updated.
     */
    [[nodiscard]] const Property& delta_property() const
    {
        return _delta_property;
    }

private:
    Value _value;
    Property _delta_property;
    Property _initial_property;
};


/**
 * @brief Modifies the property of a value by delta from a minimum to a maximum.
 * When the property is over the given maximum, it goes back to the given minimum and vice versa.
 *
 * @tparam Value Value to modify.
 * @tparam Property Property of the value to modify.
 * @tparam PropertyManager Reads and writes the property of the value to modify.
 *
 * @ingroup template_action
 */
template<typename Value, typename Property, class PropertyManager>
class cyclic_by_value_template_action
{

public:
    /**
     * @brief Resets the property to its initial state.
     */
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
    }

    /**
     * @brief Adds delta_property to the property.
     *
     * When the property reaches the maximum, it goes back to the minimum and vice versa.
     */
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
    /**
     * @brief Constructor.
     * @param value Value to copy.
     * @param delta_property How much to add to the property when the action is updated.
     * @param min_property Minimum property.
     * @param after_max_property Immediate amount after the maximum property.
     */
    cyclic_by_value_template_action(const Value& value, const Property& delta_property, const Property& min_property,
                                    const Property& after_max_property) :
        _value(value),
        _delta_property(delta_property),
        _min_property(min_property),
        _after_max_property(after_max_property),
        _initial_property(PropertyManager::get(_value))
    {
    }

    /**
     * @brief Constructor.
     * @param value Value to move.
     * @param delta_property How much to add to the property when the action is updated.
     * @param min_property Minimum property.
     * @param after_max_property Immediate amount after the maximum property.
     */
    cyclic_by_value_template_action(Value&& value, const Property& delta_property, const Property& min_property,
                                    const Property& after_max_property) :
        _value(move(value)),
        _delta_property(delta_property),
        _min_property(min_property),
        _after_max_property(after_max_property),
        _initial_property(PropertyManager::get(_value))
    {
    }

    /**
     * @brief Returns the value to modify.
     */
    [[nodiscard]] const Value& value() const
    {
        return _value;
    }

    /**
     * @brief Returns how much to add to the property when the action is updated.
     */
    [[nodiscard]] const Property& delta_property() const
    {
        return _delta_property;
    }

    /**
     * @brief Sets the minimum property.
     */
    void set_min_property(const Property& min_property)
    {
        _min_property = min_property;
    }

    /**
     * @brief Sets the immediate amount after the maximum property.
     */
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


/**
 * @brief Modifies the property of a value by delta when the action is updated a given number of times.
 *
 * @tparam Value Value to modify.
 * @tparam Property Property of the value to modify.
 * @tparam PropertyManager Reads and writes the property of the value to modify.
 *
 * @ingroup template_action
 */
template<typename Value, typename Property, class PropertyManager>
class duration_by_value_template_action
{

public:
    /**
     * @brief Resets the property to its initial state.
     */
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
        _current_update = 0;
    }

    /**
     * @brief Adds delta_property to the property when the given amount of update calls are done.
     */
    void update()
    {
        if(_current_update == _duration_updates - 1)
        {
            PropertyManager::set(PropertyManager::get(_value) + _delta_property, _value);
            _current_update = 0;
        }
        else
        {
            ++_current_update;
        }
    }

    /**
     * @brief Returns how much times the action has to be updated before updating the property.
     */
    [[nodiscard]] int duration_updates() const
    {
        return _duration_updates;
    }

protected:
    /**
     * @brief Constructor.
     * @param value Value to copy.
     * @param duration_updates How much times the action has to be updated before updating the property.
     * @param delta_property How much to add to the property when the action is updated duration_updates times.
     */
    duration_by_value_template_action(const Value& value, int duration_updates, const Property& delta_property) :
        _value(value),
        _delta_property(delta_property),
        _initial_property(PropertyManager::get(_value)),
        _duration_updates(duration_updates)
    {
        BN_ASSERT(duration_updates > 0, "Invalid duration updates: ", duration_updates);
    }

    /**
     * @brief Constructor.
     * @param value Value to move.
     * @param duration_updates How much times the action has to be updated before updating the property.
     * @param delta_property How much to add to the property when the action is updated duration_updates times.
     */
    duration_by_value_template_action(Value&& value, int duration_updates, const Property& delta_property) :
        _value(move(value)),
        _delta_property(delta_property),
        _initial_property(PropertyManager::get(_value)),
        _duration_updates(duration_updates)
    {
        BN_ASSERT(duration_updates > 0, "Invalid duration updates: ", duration_updates);
    }

    /**
     * @brief Returns the value to modify.
     */
    [[nodiscard]] const Value& value() const
    {
        return _value;
    }

    /**
     * @brief Returns how much to add to the property when the action is updated the given number of times.
     */
    [[nodiscard]] const Property& delta_property() const
    {
        return _delta_property;
    }

private:
    uint16_t _current_update = 0;
    Value _value;
    Property _delta_property;
    Property _initial_property;
    int _duration_updates;
};


/**
 * @brief Modifies the property of a value by delta when the action is updated a given number of times.
 * When the property is over the given maximum, it goes back to the given minimum and vice versa.
 *
 * @tparam Value Value to modify.
 * @tparam Property Property of the value to modify.
 * @tparam PropertyManager Reads and writes the property of the value to modify.
 *
 * @ingroup template_action
 */
template<typename Value, typename Property, class PropertyManager>
class cyclic_duration_by_value_template_action
{

public:
    /**
     * @brief Resets the property to its initial state.
     */
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
    }

    /**
     * @brief Adds delta_property to the property when the given amount of update calls are done.
     *
     * When the property reaches the maximum, it goes back to the minimum and vice versa.
     */
    void update()
    {
        if(_current_update == _duration_updates - 1)
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
            _current_update = 0;
        }
        else
        {
            ++_current_update;
        }
    }

    /**
     * @brief Returns how much times the action has to be updated before updating the property.
     */
    [[nodiscard]] int duration_updates() const
    {
        return _duration_updates;
    }

protected:
    /**
     * @brief Constructor.
     * @param value Value to copy.
     * @param duration_updates How much times the action has to be updated before updating the property.
     * @param delta_property How much to add to the property when the action is updated duration_updates times.
     * @param min_property Minimum property.
     * @param after_max_property Immediate amount after the maximum property.
     */
    cyclic_duration_by_value_template_action(const Value& value, int duration_updates, const Property& delta_property,
                                             const Property& min_property, const Property& after_max_property) :
        _value(value),
        _delta_property(delta_property),
        _min_property(min_property),
        _after_max_property(after_max_property),
        _initial_property(PropertyManager::get(_value)),
        _duration_updates(duration_updates)
    {
        BN_ASSERT(duration_updates > 0, "Invalid duration updates: ", duration_updates);
    }

    /**
     * @brief Constructor.
     * @param value Value to move.
     * @param duration_updates How much times the action has to be updated before updating the property.
     * @param delta_property How much to add to the property when the action is updated duration_updates times.
     * @param min_property Minimum property.
     * @param after_max_property Immediate amount after the maximum property.
     */
    cyclic_duration_by_value_template_action(Value&& value, int duration_updates, const Property& delta_property,
                                             const Property& min_property, const Property& after_max_property) :
        _value(move(value)),
        _delta_property(delta_property),
        _min_property(min_property),
        _after_max_property(after_max_property),
        _initial_property(PropertyManager::get(_value)),
        _duration_updates(duration_updates)
    {
        BN_ASSERT(duration_updates > 0, "Invalid duration updates: ", duration_updates);
    }

    /**
     * @brief Returns the value to modify.
     */
    [[nodiscard]] const Value& value() const
    {
        return _value;
    }

    /**
     * @brief Returns how much to add to the property when the action is updated the given number of times.
     */
    [[nodiscard]] const Property& delta_property() const
    {
        return _delta_property;
    }

    /**
     * @brief Sets the minimum property.
     */
    void set_min_property(const Property& min_property)
    {
        _min_property = min_property;
    }

    /**
     * @brief Sets the immediate amount after the maximum property.
     */
    void set_after_max_property(const Property& after_max_property)
    {
        _after_max_property = after_max_property;
    }

private:
    uint16_t _current_update = 0;
    Value _value;
    Property _delta_property;
    Property _min_property;
    Property _after_max_property;
    Property _initial_property;
    int _duration_updates;
};


/**
 * @brief Modifies the property of a value until it has a given state.
 *
 * @tparam Value Value to modify.
 * @tparam Property Property of the value to modify.
 * @tparam PropertyManager Reads and writes the property of the value to modify.
 *
 * @ingroup template_action
 */
template<typename Value, typename Property, class PropertyManager>
class to_value_template_action
{

public:
    /**
     * @brief Resets the property to its initial state.
     */
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
        _current_update = 0;
    }

    /**
     * @brief Adds ((final_property - initial_property) / duration_updates) to the property
     * until it is equal to final_property.
     */
    void update()
    {
        BN_ASSERT(! done(), "Action is done");

        ++_current_update;

        if(_current_update == _duration_updates)
        {
            PropertyManager::set(_final_property, _value);
        }
        else
        {
            PropertyManager::set(PropertyManager::get(_value) + _delta_property, _value);
        }
    }

    /**
     * @brief Indicates if the action must not be updated anymore.
     */
    [[nodiscard]] bool done() const
    {
        return _current_update == _duration_updates;
    }

    /**
     * @brief Returns the number of times that the action must be updated
     * until the property is equal to final_property.
     */
    [[nodiscard]] int duration_updates() const
    {
        return _duration_updates;
    }

protected:
    /**
     * @brief Constructor.
     * @param value Value to copy.
     * @param duration_updates Number of times that the action must be updated
     * until the property is equal to final_property.
     * @param final_property Property when the action is updated duration_updates times.
     */
    to_value_template_action(const Value& value, int duration_updates, const Property& final_property) :
        _value(value),
        _final_property(final_property),
        _initial_property(PropertyManager::get(_value)),
        _delta_property(_calculate_delta_property(duration_updates)),
        _duration_updates(duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param value Value to move.
     * @param duration_updates Number of times that the action must be updated
     * until the property is equal to final_property.
     * @param final_property Property when the action is updated duration_updates times.
     */
    to_value_template_action(Value&& value, int duration_updates, const Property& final_property) :
        _value(move(value)),
        _final_property(final_property),
        _initial_property(PropertyManager::get(_value)),
        _delta_property(_calculate_delta_property(duration_updates)),
        _duration_updates(duration_updates)
    {
    }

    /**
     * @brief Returns the value to modify.
     */
    [[nodiscard]] const Value& value() const
    {
        return _value;
    }

    /**
     * @brief Returns the state of property when the action is updated the given number of times.
     */
    [[nodiscard]] const Property& final_property() const
    {
        return _final_property;
    }

private:
    uint16_t _current_update = 0;
    Value _value;
    Property _final_property;
    Property _initial_property;
    Property _delta_property;
    int _duration_updates;

    [[nodiscard]] Property _calculate_delta_property(int duration_updates) const
    {
        BN_ASSERT(duration_updates > 0, "Invalid duration updates: ", duration_updates);
        BN_ASSERT(duration_updates <= numeric_limits<decltype(_current_update)>::max(),
                   "Too many duration updates: ", duration_updates);

        return (_final_property - _initial_property) / duration_updates;
    }
};


/**
 * @brief Modifies the property of a value by delta from a minimum to a maximum.
 * When the property is equal to the given final state, it goes back to its initial state and vice versa.
 *
 * @tparam Value Value to modify.
 * @tparam Property Property of the value to modify.
 * @tparam PropertyManager Reads and writes the property of the value to modify.
 *
 * @ingroup template_action
 */
template<typename Value, typename Property, class PropertyManager>
class loop_value_template_action
{

public:
    /**
     * @brief Resets the property to its initial state.
     */
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
        _current_update = 0;
        _reverse = false;
    }

    /**
     * @brief Adds or subtracts ((final_property - initial_property) / duration_updates) to the property.
     *
     * When the property is equal to final_property, it goes back to its initial state and vice versa.
     */
    void update()
    {
        ++_current_update;

        if(_current_update == _duration_updates)
        {
            _current_update = 0;

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

    /**
     * @brief Returns how much times the action has to be updated before changing the direction of the property delta.
     */
    [[nodiscard]] int duration_updates() const
    {
        return _duration_updates;
    }

protected:
    /**
     * @brief Constructor.
     * @param value Value to copy.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the property delta.
     * @param final_property When the property is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    loop_value_template_action(const Value& value, int duration_updates, const Property& final_property) :
        _value(value),
        _final_property(final_property),
        _initial_property(PropertyManager::get(_value)),
        _delta_property(_calculate_delta_property(duration_updates)),
        _duration_updates(duration_updates)
    {
    }

    /**
     * @brief Constructor.
     * @param value Value to move.
     * @param duration_updates How much times the action has to be updated
     * before changing the direction of the property delta.
     * @param final_property When the property is equal to this parameter,
     * it goes back to its initial state and vice versa.
     */
    loop_value_template_action(Value&& value, int duration_updates, const Property& final_property) :
        _value(move(value)),
        _final_property(final_property),
        _initial_property(PropertyManager::get(_value)),
        _delta_property(_calculate_delta_property(duration_updates)),
        _duration_updates(duration_updates)
    {
    }

    /**
     * @brief Returns the value to modify.
     */
    [[nodiscard]] const Value& value() const
    {
        return _value;
    }

    /**
     * @brief When the property is equal to the returned parameter,
     * it goes back to its initial state and vice versa.
     */
    [[nodiscard]] const Property& final_property() const
    {
        return _final_property;
    }

private:
    bool _reverse = false;
    uint16_t _current_update = 0;
    Value _value;
    Property _final_property;
    Property _initial_property;
    Property _delta_property;
    int _duration_updates;

    [[nodiscard]] Property _calculate_delta_property(int duration_updates) const
    {
        BN_ASSERT(duration_updates > 0, "Invalid duration updates: ", duration_updates);
        BN_ASSERT(duration_updates <= numeric_limits<decltype(_current_update)>::max(),
                   "Too many duration updates: ", duration_updates);

        return (_final_property - _initial_property) / duration_updates;
    }
};


/**
 * @brief Changes the property of a value when the action is updated a given number of times.
 *
 * @tparam Value Value to modify.
 * @tparam Property Property of the value to modify.
 * @tparam PropertyManager Reads and writes the property of the value to modify.
 *
 * @ingroup template_action
 */
template<typename Value, typename Property, class PropertyManager>
class toggle_value_template_action
{

public:
    /**
     * @brief Resets the property to its initial state.
     */
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
        _current_update = 0;
        _reverse = false;
    }

    /**
     * @brief Sets the property to new_property or to its initial state when the given amount of update calls are done.
     */
    void update()
    {
        ++_current_update;

        if(_current_update == _duration_updates)
        {
            _current_update = 0;

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

    /**
     * @brief Returns how much times the action has to be updated to change the property.
     */
    [[nodiscard]] int duration_updates() const
    {
        return _duration_updates;
    }

protected:
    /**
     * @brief Constructor.
     * @param value Value to copy.
     * @param duration_updates How much times the action has to be updated to change the property.
     * @param new_property New state for the property when the action is updated duration_updates times.
     */
    toggle_value_template_action(const Value& value, int duration_updates, const Property& new_property) :
        _value(value),
        _initial_property(PropertyManager::get(_value)),
        _new_property(new_property),
        _duration_updates(duration_updates)
    {
        BN_ASSERT(duration_updates > 0, "Invalid duration updates: ", duration_updates);
    }

    /**
     * @brief Constructor.
     * @param value Value to move.
     * @param duration_updates How much times the action has to be updated to change the property.
     * @param new_property New state for the property when the action is updated duration_updates times.
     */
    toggle_value_template_action(Value&& value, int duration_updates, const Property& new_property) :
        _value(move(value)),
        _initial_property(PropertyManager::get(_value)),
        _new_property(new_property),
        _duration_updates(duration_updates)
    {
        BN_ASSERT(duration_updates > 0, "Invalid duration updates: ", duration_updates);
    }

    /**
     * @brief Returns the value to modify.
     */
    [[nodiscard]] const Value& value() const
    {
        return _value;
    }

    /**
     * @brief Returns the new state for the property when the action is updated the given number of times.
     */
    [[nodiscard]] const Property& new_property() const
    {
        return _new_property;
    }

private:
    bool _reverse = false;
    uint16_t _current_update = 0;
    Value _value;
    Property _initial_property;
    Property _new_property;
    int _duration_updates;
};


/**
 * @brief Toggles a boolean property of a value when the action is updated a given number of times.
 *
 * @tparam Value Value to modify.
 * @tparam PropertyManager Reads and writes the property of the value to modify.
 *
 * @ingroup template_action
 */
template<typename Value, class PropertyManager>
class bool_toggle_value_template_action
{

public:
    /**
     * @brief Resets the property to its initial state.
     */
    void reset()
    {
        PropertyManager::set(_initial_property, _value);
        _current_update = 0;
        _reverse = false;
    }

    /**
     * @brief Toggles the property when the given amount of update calls are done.
     */
    void update()
    {
        ++_current_update;

        if(_current_update == _duration_updates)
        {
            _current_update = 0;

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

    /**
     * @brief Returns how much times the action has to be updated to toggle the property.
     */
    [[nodiscard]] int duration_updates() const
    {
        return _duration_updates;
    }

protected:
    /**
     * @brief Constructor.
     * @param value Value to copy.
     * @param duration_updates How much times the action has to be updated to toggle the property.
     */
    bool_toggle_value_template_action(const Value& value, int duration_updates) :
        _initial_property(PropertyManager::get(value)),
        _value(value),
        _duration_updates(duration_updates)
    {
        BN_ASSERT(duration_updates > 0, "Invalid duration updates: ", duration_updates);
    }

    /**
     * @brief Constructor.
     * @param value Value to move.
     * @param duration_updates How much times the action has to be updated to toggle the property.
     */
    bool_toggle_value_template_action(Value&& value, int duration_updates) :
        _initial_property(PropertyManager::get(value)),
        _value(move(value)),
        _duration_updates(duration_updates)
    {
        BN_ASSERT(duration_updates > 0, "Invalid duration updates: ", duration_updates);
    }

    /**
     * @brief Returns the value to modify.
     */
    [[nodiscard]] const Value& value() const
    {
        return _value;
    }

private:
    bool _reverse = false;
    bool _initial_property;
    uint16_t _current_update = 0;
    Value _value;
    int _duration_updates;
};

}

#endif
