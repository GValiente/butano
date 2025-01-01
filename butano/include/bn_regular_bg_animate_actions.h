/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_REGULAR_BG_ANIMATE_ACTIONS_H
#define BN_REGULAR_BG_ANIMATE_ACTIONS_H

/**
 * @file
 * bn::regular_bg_ptr animate actions implementation header file.
 *
 * @ingroup regular_bg
 * @ingroup bg_map
 * @ingroup action
 */

#include "bn_array.h"
#include "bn_vector.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_item.h"
#include "bn_regular_bg_animate_actions_fwd.h"

namespace bn
{

// animation

/**
 * @brief Base class of bn::regular_bg_animate_action.
 *
 * Can be used as a reference type for all bn::regular_bg_animate_action objects.
 *
 * @ingroup regular_bg
 * @ingroup bg_map
 * @ingroup action
 */
class iregular_bg_animate_action
{

public:
    iregular_bg_animate_action(const iregular_bg_animate_action& other) = delete;

    /**
     * @brief Copy assignment operator.
     * @param other iregular_bg_animate_action to copy.
     * @return Reference to this.
     */
    iregular_bg_animate_action& operator=(const iregular_bg_animate_action& other);

    /**
     * @brief Move assignment operator.
     * @param other iregular_bg_animate_action to move.
     * @return Reference to this.
     */
    iregular_bg_animate_action& operator=(iregular_bg_animate_action&& other) noexcept;

    /**
     * @brief Changes the map of the given regular_bg_ptr when the given amount of update calls are done.
     */
    void update();

    /**
     * @brief Indicates if the action must not be updated anymore.
     */
    [[nodiscard]] bool done() const
    {
        return _current_map_indexes_index == _map_indexes_ref->size();
    }

    /**
     * @brief Resets the action to its initial state.
     */
    void reset()
    {
        _current_map_indexes_index = 0;
        _current_wait_updates = 0;
    }

    /**
     * @brief Returns the regular_bg_ptr to modify.
     */
    [[nodiscard]] const regular_bg_ptr& regular_bg() const
    {
        return *_regular_bg_ref;
    }

    /**
     * @brief Returns the number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     */
    [[nodiscard]] int wait_updates() const
    {
        return _wait_updates;
    }

    /**
     * @brief Sets the number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     */
    void set_wait_updates(int wait_updates);

    /**
     * @brief Returns the number of times the action must be updated before the next map change.
     */
    [[nodiscard]] int next_change_updates() const
    {
        return _current_wait_updates;
    }

    /**
     * @brief Returns the regular_bg_map_item used to create the new map to use by the given regular_bg_ptr.
     */
    [[nodiscard]] const regular_bg_map_item& map_item() const
    {
        return *_map_item_ref;
    }

    /**
     * @brief Returns the indexes of the maps to reference in the given regular_bg_map_item.
     */
    [[nodiscard]] const ivector<uint16_t>& map_indexes() const
    {
        return *_map_indexes_ref;
    }

    /**
     * @brief Indicates if the action can be updated forever or not.
     */
    [[nodiscard]] bool update_forever() const
    {
        return _forever;
    }

    /**
     * @brief Returns the current index of the given map_indexes
     * (not the current index of the map to reference in the given map_item).
     */
    [[nodiscard]] int current_index() const
    {
        return _current_map_indexes_index;
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    iregular_bg_animate_action() = default;

    void _set_refs(regular_bg_ptr& regular_bg, regular_bg_map_item& map_item, ivector<uint16_t>& map_indexes);

    void _assign(const iregular_bg_animate_action& other);

    void _set_update_forever(bool forever)
    {
        _forever = forever;
    }

    void _assign_map_indexes(const span<const uint16_t>& map_indexes);

    void _assign_map_indexes(const ivector<uint16_t>& map_indexes);

    /// @endcond

private:
    regular_bg_ptr* _regular_bg_ref = nullptr;
    regular_bg_map_item* _map_item_ref = nullptr;
    ivector<uint16_t>* _map_indexes_ref = nullptr;
    uint16_t _wait_updates = 0;
    uint16_t _current_map_indexes_index = 0;
    uint16_t _current_wait_updates = 0;
    bool _forever = true;
};

template<int MaxSize>
class regular_bg_animate_action : public iregular_bg_animate_action
{
    static_assert(MaxSize > 1);

public:
    /**
     * @brief Generates a regular_bg_animate_action which loops over the given regular_bg maps only once.
     * @param regular_bg regular_bg_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     * @param map_item It creates the new map to use by the given regular_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in map_item.
     * @return The requested regular_bg_animate_action.
     */
    [[nodiscard]] static regular_bg_animate_action once(
            const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return regular_bg_animate_action(regular_bg, wait_updates, map_item, false, map_indexes);
    }

    /**
     * @brief Generates a regular_bg_animate_action which loops over the given regular_bg maps only once.
     * @param regular_bg regular_bg_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     * @param map_item It creates the new map to use by the given regular_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in the given regular_bg_map_item.
     * @return The requested regular_bg_animate_action.
     */
    [[nodiscard]] static regular_bg_animate_action once(
            regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return regular_bg_animate_action(move(regular_bg), wait_updates, map_item, false, map_indexes);
    }

    /**
     * @brief Generates a regular_bg_animate_action which loops over the given regular_bg maps forever.
     * @param regular_bg regular_bg_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     * @param map_item It creates the new map to use by the given regular_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in the given regular_bg_map_item.
     * @return The requested regular_bg_animate_action.
     */
    [[nodiscard]] static regular_bg_animate_action forever(
            const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return regular_bg_animate_action(regular_bg, wait_updates, map_item, true, map_indexes);
    }

    /**
     * @brief Generates a regular_bg_animate_action which loops over the given regular_bg maps forever.
     * @param regular_bg regular_bg_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     * @param map_item It creates the new map to use by the given regular_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in the given regular_bg_map_item.
     * @return The requested regular_bg_animate_action.
     */
    [[nodiscard]] static regular_bg_animate_action forever(
            regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return regular_bg_animate_action(move(regular_bg), wait_updates, map_item, true, map_indexes);
    }

    /**
     * @brief Copy constructor.
     * @param other regular_bg_animate_action to copy.
     */
    regular_bg_animate_action(const regular_bg_animate_action& other) :
        _regular_bg(other._regular_bg),
        _map_item(other._map_item),
        _map_indexes(other._map_indexes)
    {
        this->_set_refs(_regular_bg, _map_item, _map_indexes);
        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other regular_bg_animate_action to move.
     */
    regular_bg_animate_action(regular_bg_animate_action&& other) noexcept :
        _regular_bg(move(other._regular_bg)),
        _map_item(other._map_item),
        _map_indexes(other._map_indexes)
    {
        this->_set_refs(_regular_bg, _map_item, _map_indexes);
        this->_assign(other);
    }

    /**
     * @brief Copy constructor.
     * @param other iregular_bg_animate_action to copy.
     */
    regular_bg_animate_action(const iregular_bg_animate_action& other) :
        _regular_bg(other.regular_bg()),
        _map_item(other.map_item()),
        _map_indexes(other.map_indexes())
    {
        BN_ASSERT(other.map_indexes().size() <= MaxSize,
                  "Too many map indexes: ", other.map_indexes().size(), " - ", MaxSize);

        this->_set_refs(_regular_bg, _map_item, _map_indexes);
        this->_assign(other);
    }

    /**
     * @brief Copy assignment operator.
     * @param other regular_bg_animate_action to copy.
     * @return Reference to this.
     */
    regular_bg_animate_action& operator=(const regular_bg_animate_action& other)
    {
        if(this != &other)
        {
            _regular_bg = other._regular_bg;
            _map_item = other._map_item;
            _map_indexes = other._map_indexes;
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other regular_bg_animate_action to move.
     * @return Reference to this.
     */
    regular_bg_animate_action& operator=(regular_bg_animate_action&& other) noexcept
    {
        if(this != &other)
        {
            _regular_bg = move(other._regular_bg);
            _map_item = other._map_item;
            _map_indexes = other._map_indexes;
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other iregular_bg_animate_action to copy.
     * @return Reference to this.
     */
    regular_bg_animate_action& operator=(const iregular_bg_animate_action& other)
    {
        static_cast<iregular_bg_animate_action&>(*this) = other;
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other ivector to move.
     * @return Reference to this.
     */
    regular_bg_animate_action& operator=(iregular_bg_animate_action&& other) noexcept
    {
        static_cast<iregular_bg_animate_action&>(*this) = move(other);
        return *this;
    }

private:
    regular_bg_ptr _regular_bg;
    regular_bg_map_item _map_item;
    vector<uint16_t, MaxSize> _map_indexes;

    regular_bg_animate_action(
            const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item,
            bool forever, const span<const uint16_t>& map_indexes) :
        _regular_bg(regular_bg),
        _map_item(map_item)
    {
        this->_set_refs(_regular_bg, _map_item, _map_indexes);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_map_indexes(map_indexes);
    }

    regular_bg_animate_action(
            regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item,
            bool forever, const span<const uint16_t>& map_indexes) :
        _regular_bg(move(regular_bg)),
        _map_item(map_item)
    {
        this->_set_refs(_regular_bg, _map_item, _map_indexes);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_map_indexes(map_indexes);
    }
};


/**
 * @brief Generates a regular_bg_animate_action which loops over the given regular_bg maps only once.
 * @param regular_bg regular_bg_ptr to copy.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given regular_bg_ptr.
 * @param map_item It creates the new map to use by the given regular_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested regular_bg_animate_action.
 *
 * @ingroup regular_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_regular_bg_animate_action_once(
        const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)
{
    return regular_bg_animate_action<sizeof...(Args)>::once(
                regular_bg, wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}


/**
 * @brief Generates a regular_bg_animate_action which loops over the given regular_bg maps only once.
 * @param regular_bg regular_bg_ptr to move.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given regular_bg_ptr.
 * @param map_item It creates the new map to use by the given regular_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested regular_bg_animate_action.
 *
 * @ingroup regular_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_regular_bg_animate_action_once(
        regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)
{
    return regular_bg_animate_action<sizeof...(Args)>::once(
                move(regular_bg), wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}


/**
 * @brief Generates a regular_bg_animate_action which loops over the given regular_bg maps forever.
 * @param regular_bg regular_bg_ptr to copy.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given regular_bg_ptr.
 * @param map_item It creates the new map to use by the given regular_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested regular_bg_animate_action.
 *
 * @ingroup regular_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_regular_bg_animate_action_forever(
        const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)
{
    return regular_bg_animate_action<sizeof...(Args)>::forever(
                regular_bg, wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}


/**
 * @brief Generates a regular_bg_animate_action which loops over the given regular_bg maps forever.
 * @param regular_bg regular_bg_ptr to move.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given regular_bg_ptr.
 * @param map_item It creates the new map to use by the given regular_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested regular_bg_animate_action.
 *
 * @ingroup regular_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_regular_bg_animate_action_forever(
        regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)
{
    return regular_bg_animate_action<sizeof...(Args)>::forever(
                move(regular_bg), wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}


// cached animation

/**
 * @brief Base class of bn::regular_bg_cached_animate_action.
 *
 * Can be used as a reference type for all bn::regular_bg_cached_animate_action objects.
 *
 * @ingroup regular_bg
 * @ingroup bg_map
 * @ingroup action
 */
class iregular_bg_cached_animate_action
{

public:
    iregular_bg_cached_animate_action(const iregular_bg_cached_animate_action& other) = delete;

    /**
     * @brief Copy assignment operator.
     * @param other iregular_bg_cached_animate_action to copy.
     * @return Reference to this.
     */
    iregular_bg_cached_animate_action& operator=(const iregular_bg_cached_animate_action& other);

    /**
     * @brief Move assignment operator.
     * @param other iregular_bg_cached_animate_action to move.
     * @return Reference to this.
     */
    iregular_bg_cached_animate_action& operator=(iregular_bg_cached_animate_action&& other) noexcept;

    /**
     * @brief Changes the map of the given regular_bg_ptr when the given amount of update calls are done.
     */
    void update();

    /**
     * @brief Indicates if the action must not be updated anymore.
     */
    [[nodiscard]] bool done() const
    {
        return _current_map_index == _maps_ref->size();
    }

    /**
     * @brief Resets the action to its initial state.
     */
    void reset()
    {
        _current_map_index = 0;
        _current_wait_updates = 0;
    }

    /**
     * @brief Returns the regular_bg_ptr to modify.
     */
    [[nodiscard]] const regular_bg_ptr& regular_bg() const
    {
        return *_regular_bg_ref;
    }

    /**
     * @brief Returns the number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     */
    [[nodiscard]] int wait_updates() const
    {
        return _wait_updates;
    }

    /**
     * @brief Sets the number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     */
    void set_wait_updates(int wait_updates);

    /**
     * @brief Returns the number of times the action must be updated before the next map change.
     */
    [[nodiscard]] int next_change_updates() const
    {
        return _current_wait_updates;
    }

    /**
     * @brief Returns the maps to use by the given regular_bg_ptr.
     */
    [[nodiscard]] const ivector<regular_bg_map_ptr>& maps() const
    {
        return *_maps_ref;
    }

    /**
     * @brief Indicates if the action can be updated forever or not.
     */
    [[nodiscard]] bool update_forever() const
    {
        return _forever;
    }

    /**
     * @brief Returns the current index of the given maps.
     */
    [[nodiscard]] int current_index() const
    {
        return _current_map_index;
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    iregular_bg_cached_animate_action() = default;

    void _set_refs(regular_bg_ptr& regular_bg, ivector<regular_bg_map_ptr>& maps);

    void _assign(const iregular_bg_cached_animate_action& other);

    void _set_update_forever(bool forever)
    {
        _forever = forever;
    }

    void _assign_map_indexes(const regular_bg_map_item& map_item, const span<const uint16_t>& map_indexes);

    void _assign_maps(span<regular_bg_map_ptr> maps);

    void _assign_maps(const ivector<regular_bg_map_ptr>& maps);

    void _assign_maps(ivector<regular_bg_map_ptr>&& maps);

    /// @endcond

private:
    regular_bg_ptr* _regular_bg_ref = nullptr;
    ivector<regular_bg_map_ptr>* _maps_ref = nullptr;
    uint16_t _wait_updates = 0;
    uint16_t _current_map_index = 0;
    uint16_t _current_wait_updates = 0;
    bool _forever = true;
};

template<int MaxSize>
class regular_bg_cached_animate_action : public iregular_bg_cached_animate_action
{
    static_assert(MaxSize > 1);

public:
    /**
     * @brief Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps only once.
     * @param regular_bg regular_bg_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     * @param map_item It creates the new map to use by the given regular_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in map_item.
     * @return The requested regular_bg_cached_animate_action.
     */
    [[nodiscard]] static regular_bg_cached_animate_action once(
            const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return regular_bg_cached_animate_action(regular_bg, wait_updates, map_item, false, map_indexes);
    }

    /**
     * @brief Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps only once.
     * @param regular_bg regular_bg_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     * @param map_item It creates the new map to use by the given regular_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in the given regular_bg_map_item.
     * @return The requested regular_bg_cached_animate_action.
     */
    [[nodiscard]] static regular_bg_cached_animate_action once(
            regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return regular_bg_cached_animate_action(move(regular_bg), wait_updates, map_item, false, map_indexes);
    }

    /**
     * @brief Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps only once.
     * @param regular_bg regular_bg_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     * @param maps List of maps to use by the given regular_bg_ptr.
     * @return The requested regular_bg_cached_animate_action.
     */
    [[nodiscard]] static regular_bg_cached_animate_action once(
            const regular_bg_ptr& regular_bg, int wait_updates, span<regular_bg_map_ptr> maps)
    {
        return regular_bg_cached_animate_action(regular_bg, wait_updates, false, maps);
    }

    /**
     * @brief Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps only once.
     * @param regular_bg regular_bg_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     * @param maps List of maps to use by the given regular_bg_ptr.
     * @return The requested regular_bg_cached_animate_action.
     */
    [[nodiscard]] static regular_bg_cached_animate_action once(
            regular_bg_ptr&& regular_bg, int wait_updates, span<regular_bg_map_ptr> maps)
    {
        return regular_bg_cached_animate_action(move(regular_bg), wait_updates, false, maps);
    }

    /**
     * @brief Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps forever.
     * @param regular_bg regular_bg_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     * @param map_item It creates the new map to use by the given regular_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in map_item.
     * @return The requested regular_bg_cached_animate_action.
     */
    [[nodiscard]] static regular_bg_cached_animate_action forever(
            const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return regular_bg_cached_animate_action(regular_bg, wait_updates, map_item, true, map_indexes);
    }

    /**
     * @brief Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps forever.
     * @param regular_bg regular_bg_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     * @param map_item It creates the new map to use by the given regular_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in the given regular_bg_map_item.
     * @return The requested regular_bg_cached_animate_action.
     */
    [[nodiscard]] static regular_bg_cached_animate_action forever(
            regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return regular_bg_cached_animate_action(move(regular_bg), wait_updates, map_item, true, map_indexes);
    }

    /**
     * @brief Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps forever.
     * @param regular_bg regular_bg_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     * @param maps List of maps to use by the given regular_bg_ptr.
     * @return The requested regular_bg_cached_animate_action.
     */
    [[nodiscard]] static regular_bg_cached_animate_action forever(
            const regular_bg_ptr& regular_bg, int wait_updates, span<regular_bg_map_ptr> maps)
    {
        return regular_bg_cached_animate_action(regular_bg, wait_updates, true, maps);
    }

    /**
     * @brief Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps forever.
     * @param regular_bg regular_bg_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given regular_bg_ptr.
     * @param maps List of maps to use by the given regular_bg_ptr.
     * @return The requested regular_bg_cached_animate_action.
     */
    [[nodiscard]] static regular_bg_cached_animate_action forever(
            regular_bg_ptr&& regular_bg, int wait_updates, span<regular_bg_map_ptr> maps)
    {
        return regular_bg_cached_animate_action(move(regular_bg), wait_updates, true, maps);
    }

    /**
     * @brief Copy constructor.
     * @param other regular_bg_cached_animate_action to copy.
     */
    regular_bg_cached_animate_action(const regular_bg_cached_animate_action& other) :
        _regular_bg(other._regular_bg),
        _maps(other._maps)
    {
        this->_set_refs(_regular_bg, _maps);
        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other regular_bg_cached_animate_action to move.
     */
    regular_bg_cached_animate_action(regular_bg_cached_animate_action&& other) noexcept :
        _regular_bg(move(other._regular_bg)),
        _maps(move(other._maps))
    {
        this->_set_refs(_regular_bg, _maps);
        this->_assign(other);
    }

    /**
     * @brief Copy constructor.
     * @param other iregular_bg_cached_animate_action to copy.
     */
    regular_bg_cached_animate_action(const iregular_bg_cached_animate_action& other) :
        _regular_bg(other.regular_bg()),
        _maps(other.maps())
    {
        BN_ASSERT(other.maps().size() <= MaxSize, "Too many maps: ", other.maps().size(), " - ", MaxSize);

        this->_set_refs(_regular_bg, _maps);
        this->_assign(other);
    }

    /**
     * @brief Copy assignment operator.
     * @param other regular_bg_cached_animate_action to copy.
     * @return Reference to this.
     */
    regular_bg_cached_animate_action& operator=(const regular_bg_cached_animate_action& other)
    {
        if(this != &other)
        {
            _regular_bg = other._regular_bg;
            _maps = other._maps;
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other regular_bg_cached_animate_action to move.
     * @return Reference to this.
     */
    regular_bg_cached_animate_action& operator=(regular_bg_cached_animate_action&& other) noexcept
    {
        if(this != &other)
        {
            _regular_bg = move(other._regular_bg);
            _maps = move(other._maps);
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other iregular_bg_cached_animate_action to copy.
     * @return Reference to this.
     */
    regular_bg_cached_animate_action& operator=(const iregular_bg_cached_animate_action& other)
    {
        static_cast<iregular_bg_cached_animate_action&>(*this) = other;
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other ivector to move.
     * @return Reference to this.
     */
    regular_bg_cached_animate_action& operator=(iregular_bg_cached_animate_action&& other) noexcept
    {
        static_cast<iregular_bg_cached_animate_action&>(*this) = move(other);
        return *this;
    }

private:
    regular_bg_ptr _regular_bg;
    vector<regular_bg_map_ptr, MaxSize> _maps;

    regular_bg_cached_animate_action(
            const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item,
            bool forever, const span<const uint16_t>& map_indexes) :
        _regular_bg(regular_bg)
    {
        this->_set_refs(_regular_bg, _maps);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_map_indexes(map_item, map_indexes);
    }

    regular_bg_cached_animate_action(
            regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item,
            bool forever, const span<const uint16_t>& map_indexes) :
        _regular_bg(move(regular_bg))
    {
        this->_set_refs(_regular_bg, _maps);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_map_indexes(map_item, map_indexes);
    }

    regular_bg_cached_animate_action(
            const regular_bg_ptr& regular_bg, int wait_updates, bool forever, span<regular_bg_map_ptr> maps) :
        _regular_bg(regular_bg)
    {
        this->_set_refs(_regular_bg, _maps);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_maps(maps);
    }

    regular_bg_cached_animate_action(
            regular_bg_ptr&& regular_bg, int wait_updates, bool forever, span<regular_bg_map_ptr> maps) :
        _regular_bg(move(regular_bg))
    {
        this->_set_refs(_regular_bg, _maps);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_maps(maps);
    }
};


/**
 * @brief Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps only once.
 * @param regular_bg regular_bg_ptr to copy.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given regular_bg_ptr.
 * @param map_item It creates the new map to use by the given regular_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested regular_bg_cached_animate_action.
 *
 * @ingroup regular_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_regular_bg_cached_animate_action_once(
        const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)
{
    return regular_bg_cached_animate_action<sizeof...(Args)>::once(
                regular_bg, wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}


/**
 * @brief Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps only once.
 * @param regular_bg regular_bg_ptr to move.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given regular_bg_ptr.
 * @param map_item It creates the new map to use by the given regular_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested regular_bg_cached_animate_action.
 *
 * @ingroup regular_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_regular_bg_cached_animate_action_once(
        regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)
{
    return regular_bg_cached_animate_action<sizeof...(Args)>::once(
                move(regular_bg), wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}


/**
 * @brief Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps forever.
 * @param regular_bg regular_bg_ptr to copy.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given regular_bg_ptr.
 * @param map_item It creates the new map to use by the given regular_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested regular_bg_cached_animate_action.
 *
 * @ingroup regular_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_regular_bg_cached_animate_action_forever(
        const regular_bg_ptr& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)
{
    return regular_bg_cached_animate_action<sizeof...(Args)>::forever(
                regular_bg, wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}


/**
 * @brief Generates a regular_bg_cached_animate_action which loops over the given regular_bg maps forever.
 * @param regular_bg regular_bg_ptr to move.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given regular_bg_ptr.
 * @param map_item It creates the new map to use by the given regular_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested regular_bg_cached_animate_action.
 *
 * @ingroup regular_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_regular_bg_cached_animate_action_forever(
        regular_bg_ptr&& regular_bg, int wait_updates, const regular_bg_map_item& map_item, Args ...map_indexes)
{
    return regular_bg_cached_animate_action<sizeof...(Args)>::forever(
                move(regular_bg), wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}

}

#endif
