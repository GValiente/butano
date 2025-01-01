/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_ANIMATE_ACTIONS_H
#define BN_AFFINE_BG_ANIMATE_ACTIONS_H

/**
 * @file
 * bn::affine_bg_ptr animate actions implementation header file.
 *
 * @ingroup affine_bg
 * @ingroup bg_map
 * @ingroup action
 */

#include "bn_array.h"
#include "bn_vector.h"
#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_map_ptr.h"
#include "bn_affine_bg_map_item.h"
#include "bn_affine_bg_animate_actions_fwd.h"

namespace bn
{

// animation

/**
 * @brief Base class of bn::affine_bg_animate_action.
 *
 * Can be used as a reference type for all bn::affine_bg_animate_action objects.
 *
 * @ingroup affine_bg
 * @ingroup bg_map
 * @ingroup action
 */
class iaffine_bg_animate_action
{

public:
    iaffine_bg_animate_action(const iaffine_bg_animate_action& other) = delete;

    /**
     * @brief Copy assignment operator.
     * @param other iaffine_bg_animate_action to copy.
     * @return Reference to this.
     */
    iaffine_bg_animate_action& operator=(const iaffine_bg_animate_action& other);

    /**
     * @brief Move assignment operator.
     * @param other iaffine_bg_animate_action to move.
     * @return Reference to this.
     */
    iaffine_bg_animate_action& operator=(iaffine_bg_animate_action&& other) noexcept;

    /**
     * @brief Changes the map of the given affine_bg_ptr when the given amount of update calls are done.
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
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& affine_bg() const
    {
        return *_affine_bg_ref;
    }

    /**
     * @brief Returns the number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
     */
    [[nodiscard]] int wait_updates() const
    {
        return _wait_updates;
    }

    /**
     * @brief Sets the number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
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
     * @brief Returns the affine_bg_map_item used to create the new map to use by the given affine_bg_ptr.
     */
    [[nodiscard]] const affine_bg_map_item& map_item() const
    {
        return *_map_item_ref;
    }

    /**
     * @brief Returns the indexes of the maps to reference in the given affine_bg_map_item.
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

    iaffine_bg_animate_action() = default;

    void _set_refs(affine_bg_ptr& affine_bg, affine_bg_map_item& map_item, ivector<uint16_t>& map_indexes);

    void _assign(const iaffine_bg_animate_action& other);

    void _set_update_forever(bool forever)
    {
        _forever = forever;
    }

    void _assign_map_indexes(const span<const uint16_t>& map_indexes);

    void _assign_map_indexes(const ivector<uint16_t>& map_indexes);

    /// @endcond

private:
    affine_bg_ptr* _affine_bg_ref = nullptr;
    affine_bg_map_item* _map_item_ref = nullptr;
    ivector<uint16_t>* _map_indexes_ref = nullptr;
    uint16_t _wait_updates = 0;
    uint16_t _current_map_indexes_index = 0;
    uint16_t _current_wait_updates = 0;
    bool _forever = true;
};

template<int MaxSize>
class affine_bg_animate_action : public iaffine_bg_animate_action
{
    static_assert(MaxSize > 1);

public:
    /**
     * @brief Generates an affine_bg_animate_action which loops over the given affine_bg maps only once.
     * @param affine_bg affine_bg_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
     * @param map_item It creates the new map to use by the given affine_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in map_item.
     * @return The requested affine_bg_animate_action.
     */
    [[nodiscard]] static affine_bg_animate_action once(
            const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return affine_bg_animate_action(affine_bg, wait_updates, map_item, false, map_indexes);
    }

    /**
     * @brief Generates an affine_bg_animate_action which loops over the given affine_bg maps only once.
     * @param affine_bg affine_bg_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
     * @param map_item It creates the new map to use by the given affine_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in the given affine_bg_map_item.
     * @return The requested affine_bg_animate_action.
     */
    [[nodiscard]] static affine_bg_animate_action once(
            affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return affine_bg_animate_action(move(affine_bg), wait_updates, map_item, false, map_indexes);
    }

    /**
     * @brief Generates an affine_bg_animate_action which loops over the given affine_bg maps forever.
     * @param affine_bg affine_bg_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
     * @param map_item It creates the new map to use by the given affine_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in the given affine_bg_map_item.
     * @return The requested affine_bg_animate_action.
     */
    [[nodiscard]] static affine_bg_animate_action forever(
            const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return affine_bg_animate_action(affine_bg, wait_updates, map_item, true, map_indexes);
    }

    /**
     * @brief Generates an affine_bg_animate_action which loops over the given affine_bg maps forever.
     * @param affine_bg affine_bg_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
     * @param map_item It creates the new map to use by the given affine_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in the given affine_bg_map_item.
     * @return The requested affine_bg_animate_action.
     */
    [[nodiscard]] static affine_bg_animate_action forever(
            affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return affine_bg_animate_action(move(affine_bg), wait_updates, map_item, true, map_indexes);
    }

    /**
     * @brief Copy constructor.
     * @param other affine_bg_animate_action to copy.
     */
    affine_bg_animate_action(const affine_bg_animate_action& other) :
        _affine_bg(other._affine_bg),
        _map_item(other._map_item),
        _map_indexes(other._map_indexes)
    {
        this->_set_refs(_affine_bg, _map_item, _map_indexes);
        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other affine_bg_animate_action to move.
     */
    affine_bg_animate_action(affine_bg_animate_action&& other) noexcept :
        _affine_bg(move(other._affine_bg)),
        _map_item(other._map_item),
        _map_indexes(other._map_indexes)
    {
        this->_set_refs(_affine_bg, _map_item, _map_indexes);
        this->_assign(other);
    }

    /**
     * @brief Copy constructor.
     * @param other iaffine_bg_animate_action to copy.
     */
    affine_bg_animate_action(const iaffine_bg_animate_action& other) :
        _affine_bg(other.affine_bg()),
        _map_item(other.map_item()),
        _map_indexes(other.map_indexes())
    {
        BN_ASSERT(other.map_indexes().size() <= MaxSize,
                  "Too many map indexes: ", other.map_indexes().size(), " - ", MaxSize);

        this->_set_refs(_affine_bg, _map_item, _map_indexes);
        this->_assign(other);
    }

    /**
     * @brief Copy assignment operator.
     * @param other affine_bg_animate_action to copy.
     * @return Reference to this.
     */
    affine_bg_animate_action& operator=(const affine_bg_animate_action& other)
    {
        if(this != &other)
        {
            _affine_bg = other._affine_bg;
            _map_item = other._map_item;
            _map_indexes = other._map_indexes;
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other affine_bg_animate_action to move.
     * @return Reference to this.
     */
    affine_bg_animate_action& operator=(affine_bg_animate_action&& other) noexcept
    {
        if(this != &other)
        {
            _affine_bg = move(other._affine_bg);
            _map_item = other._map_item;
            _map_indexes = other._map_indexes;
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other iaffine_bg_animate_action to copy.
     * @return Reference to this.
     */
    affine_bg_animate_action& operator=(const iaffine_bg_animate_action& other)
    {
        static_cast<iaffine_bg_animate_action&>(*this) = other;
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other ivector to move.
     * @return Reference to this.
     */
    affine_bg_animate_action& operator=(iaffine_bg_animate_action&& other) noexcept
    {
        static_cast<iaffine_bg_animate_action&>(*this) = move(other);
        return *this;
    }

private:
    affine_bg_ptr _affine_bg;
    affine_bg_map_item _map_item;
    vector<uint16_t, MaxSize> _map_indexes;

    affine_bg_animate_action(
            const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item,
            bool forever, const span<const uint16_t>& map_indexes) :
        _affine_bg(affine_bg),
        _map_item(map_item)
    {
        this->_set_refs(_affine_bg, _map_item, _map_indexes);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_map_indexes(map_indexes);
    }

    affine_bg_animate_action(
            affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item,
            bool forever, const span<const uint16_t>& map_indexes) :
        _affine_bg(move(affine_bg)),
        _map_item(map_item)
    {
        this->_set_refs(_affine_bg, _map_item, _map_indexes);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_map_indexes(map_indexes);
    }
};


/**
 * @brief Generates an affine_bg_animate_action which loops over the given affine_bg maps only once.
 * @param affine_bg affine_bg_ptr to copy.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given affine_bg_ptr.
 * @param map_item It creates the new map to use by the given affine_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested affine_bg_animate_action.
 *
 * @ingroup affine_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_affine_bg_animate_action_once(
        const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)
{
    return affine_bg_animate_action<sizeof...(Args)>::once(
                affine_bg, wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}


/**
 * @brief Generates an affine_bg_animate_action which loops over the given affine_bg maps only once.
 * @param affine_bg affine_bg_ptr to move.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given affine_bg_ptr.
 * @param map_item It creates the new map to use by the given affine_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested affine_bg_animate_action.
 *
 * @ingroup affine_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_affine_bg_animate_action_once(
        affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)
{
    return affine_bg_animate_action<sizeof...(Args)>::once(
                move(affine_bg), wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}


/**
 * @brief Generates an affine_bg_animate_action which loops over the given affine_bg maps forever.
 * @param affine_bg affine_bg_ptr to copy.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given affine_bg_ptr.
 * @param map_item It creates the new map to use by the given affine_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested affine_bg_animate_action.
 *
 * @ingroup affine_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_affine_bg_animate_action_forever(
        const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)
{
    return affine_bg_animate_action<sizeof...(Args)>::forever(
                affine_bg, wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}


/**
 * @brief Generates an affine_bg_animate_action which loops over the given affine_bg maps forever.
 * @param affine_bg affine_bg_ptr to move.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given affine_bg_ptr.
 * @param map_item It creates the new map to use by the given affine_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested affine_bg_animate_action.
 *
 * @ingroup affine_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_affine_bg_animate_action_forever(
        affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)
{
    return affine_bg_animate_action<sizeof...(Args)>::forever(
                move(affine_bg), wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}


// cached animation

/**
 * @brief Base class of bn::affine_bg_cached_animate_action.
 *
 * Can be used as a reference type for all bn::affine_bg_cached_animate_action objects.
 *
 * @ingroup affine_bg
 * @ingroup bg_map
 * @ingroup action
 */
class iaffine_bg_cached_animate_action
{

public:
    iaffine_bg_cached_animate_action(const iaffine_bg_cached_animate_action& other) = delete;

    /**
     * @brief Copy assignment operator.
     * @param other iaffine_bg_cached_animate_action to copy.
     * @return Reference to this.
     */
    iaffine_bg_cached_animate_action& operator=(const iaffine_bg_cached_animate_action& other);

    /**
     * @brief Move assignment operator.
     * @param other iaffine_bg_cached_animate_action to move.
     * @return Reference to this.
     */
    iaffine_bg_cached_animate_action& operator=(iaffine_bg_cached_animate_action&& other) noexcept;

    /**
     * @brief Changes the map of the given affine_bg_ptr when the given amount of update calls are done.
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
     * @brief Returns the affine_bg_ptr to modify.
     */
    [[nodiscard]] const affine_bg_ptr& affine_bg() const
    {
        return *_affine_bg_ref;
    }

    /**
     * @brief Returns the number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
     */
    [[nodiscard]] int wait_updates() const
    {
        return _wait_updates;
    }

    /**
     * @brief Sets the number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
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
     * @brief Returns the maps to use by the given affine_bg_ptr.
     */
    [[nodiscard]] const ivector<affine_bg_map_ptr>& maps() const
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

    iaffine_bg_cached_animate_action() = default;

    void _set_refs(affine_bg_ptr& affine_bg, ivector<affine_bg_map_ptr>& maps);

    void _assign(const iaffine_bg_cached_animate_action& other);

    void _set_update_forever(bool forever)
    {
        _forever = forever;
    }

    void _assign_map_indexes(const affine_bg_map_item& map_item, const span<const uint16_t>& map_indexes);

    void _assign_maps(span<affine_bg_map_ptr> maps);

    void _assign_maps(const ivector<affine_bg_map_ptr>& maps);

    void _assign_maps(ivector<affine_bg_map_ptr>&& maps);

    /// @endcond

private:
    affine_bg_ptr* _affine_bg_ref = nullptr;
    ivector<affine_bg_map_ptr>* _maps_ref = nullptr;
    uint16_t _wait_updates = 0;
    uint16_t _current_map_index = 0;
    uint16_t _current_wait_updates = 0;
    bool _forever = true;
};

template<int MaxSize>
class affine_bg_cached_animate_action : public iaffine_bg_cached_animate_action
{
    static_assert(MaxSize > 1);

public:
    /**
     * @brief Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps only once.
     * @param affine_bg affine_bg_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
     * @param map_item It creates the new map to use by the given affine_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in map_item.
     * @return The requested affine_bg_cached_animate_action.
     */
    [[nodiscard]] static affine_bg_cached_animate_action once(
            const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return affine_bg_cached_animate_action(affine_bg, wait_updates, map_item, false, map_indexes);
    }

    /**
     * @brief Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps only once.
     * @param affine_bg affine_bg_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
     * @param map_item It creates the new map to use by the given affine_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in the given affine_bg_map_item.
     * @return The requested affine_bg_cached_animate_action.
     */
    [[nodiscard]] static affine_bg_cached_animate_action once(
            affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return affine_bg_cached_animate_action(move(affine_bg), wait_updates, map_item, false, map_indexes);
    }

    /**
     * @brief Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps only once.
     * @param affine_bg affine_bg_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
     * @param maps List of maps to use by the given affine_bg_ptr.
     * @return The requested affine_bg_cached_animate_action.
     */
    [[nodiscard]] static affine_bg_cached_animate_action once(
            const affine_bg_ptr& affine_bg, int wait_updates, span<affine_bg_map_ptr> maps)
    {
        return affine_bg_cached_animate_action(affine_bg, wait_updates, false, maps);
    }

    /**
     * @brief Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps only once.
     * @param affine_bg affine_bg_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
     * @param maps List of maps to use by the given affine_bg_ptr.
     * @return The requested affine_bg_cached_animate_action.
     */
    [[nodiscard]] static affine_bg_cached_animate_action once(
            affine_bg_ptr&& affine_bg, int wait_updates, span<affine_bg_map_ptr> maps)
    {
        return affine_bg_cached_animate_action(move(affine_bg), wait_updates, false, maps);
    }

    /**
     * @brief Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps forever.
     * @param affine_bg affine_bg_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
     * @param map_item It creates the new map to use by the given affine_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in map_item.
     * @return The requested affine_bg_cached_animate_action.
     */
    [[nodiscard]] static affine_bg_cached_animate_action forever(
            const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return affine_bg_cached_animate_action(affine_bg, wait_updates, map_item, true, map_indexes);
    }

    /**
     * @brief Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps forever.
     * @param affine_bg affine_bg_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
     * @param map_item It creates the new map to use by the given affine_bg_ptr.
     * @param map_indexes Indexes of the maps to reference in the given affine_bg_map_item.
     * @return The requested affine_bg_cached_animate_action.
     */
    [[nodiscard]] static affine_bg_cached_animate_action forever(
            affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item,
            const span<const uint16_t>& map_indexes)
    {
        return affine_bg_cached_animate_action(move(affine_bg), wait_updates, map_item, true, map_indexes);
    }

    /**
     * @brief Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps forever.
     * @param affine_bg affine_bg_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
     * @param maps List of maps to use by the given affine_bg_ptr.
     * @return The requested affine_bg_cached_animate_action.
     */
    [[nodiscard]] static affine_bg_cached_animate_action forever(
            const affine_bg_ptr& affine_bg, int wait_updates, span<affine_bg_map_ptr> maps)
    {
        return affine_bg_cached_animate_action(affine_bg, wait_updates, true, maps);
    }

    /**
     * @brief Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps forever.
     * @param affine_bg affine_bg_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the map
     * of the given affine_bg_ptr.
     * @param maps List of maps to use by the given affine_bg_ptr.
     * @return The requested affine_bg_cached_animate_action.
     */
    [[nodiscard]] static affine_bg_cached_animate_action forever(
            affine_bg_ptr&& affine_bg, int wait_updates, span<affine_bg_map_ptr> maps)
    {
        return affine_bg_cached_animate_action(move(affine_bg), wait_updates, true, maps);
    }

    /**
     * @brief Copy constructor.
     * @param other affine_bg_cached_animate_action to copy.
     */
    affine_bg_cached_animate_action(const affine_bg_cached_animate_action& other) :
        _affine_bg(other._affine_bg),
        _maps(other._maps)
    {
        this->_set_refs(_affine_bg, _maps);
        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other affine_bg_cached_animate_action to move.
     */
    affine_bg_cached_animate_action(affine_bg_cached_animate_action&& other) noexcept :
        _affine_bg(move(other._affine_bg)),
        _maps(move(other._maps))
    {
        this->_set_refs(_affine_bg, _maps);
        this->_assign(other);
    }

    /**
     * @brief Copy constructor.
     * @param other iaffine_bg_cached_animate_action to copy.
     */
    affine_bg_cached_animate_action(const iaffine_bg_cached_animate_action& other) :
        _affine_bg(other.affine_bg()),
        _maps(other.maps())
    {
        BN_ASSERT(other.maps().size() <= MaxSize, "Too many maps: ", other.maps().size(), " - ", MaxSize);

        this->_set_refs(_affine_bg, _maps);
        this->_assign(other);
    }

    /**
     * @brief Copy assignment operator.
     * @param other affine_bg_cached_animate_action to copy.
     * @return Reference to this.
     */
    affine_bg_cached_animate_action& operator=(const affine_bg_cached_animate_action& other)
    {
        if(this != &other)
        {
            _affine_bg = other._affine_bg;
            _maps = other._maps;
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other affine_bg_cached_animate_action to move.
     * @return Reference to this.
     */
    affine_bg_cached_animate_action& operator=(affine_bg_cached_animate_action&& other) noexcept
    {
        if(this != &other)
        {
            _affine_bg = move(other._affine_bg);
            _maps = move(other._maps);
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other iaffine_bg_cached_animate_action to copy.
     * @return Reference to this.
     */
    affine_bg_cached_animate_action& operator=(const iaffine_bg_cached_animate_action& other)
    {
        static_cast<iaffine_bg_cached_animate_action&>(*this) = other;
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other ivector to move.
     * @return Reference to this.
     */
    affine_bg_cached_animate_action& operator=(iaffine_bg_cached_animate_action&& other) noexcept
    {
        static_cast<iaffine_bg_cached_animate_action&>(*this) = move(other);
        return *this;
    }

private:
    affine_bg_ptr _affine_bg;
    vector<affine_bg_map_ptr, MaxSize> _maps;

    affine_bg_cached_animate_action(
            const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item,
            bool forever, const span<const uint16_t>& map_indexes) :
        _affine_bg(affine_bg)
    {
        this->_set_refs(_affine_bg, _maps);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_map_indexes(map_item, map_indexes);
    }

    affine_bg_cached_animate_action(
            affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item,
            bool forever, const span<const uint16_t>& map_indexes) :
        _affine_bg(move(affine_bg))
    {
        this->_set_refs(_affine_bg, _maps);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_map_indexes(map_item, map_indexes);
    }

    affine_bg_cached_animate_action(
            const affine_bg_ptr& affine_bg, int wait_updates, bool forever, span<affine_bg_map_ptr> maps) :
        _affine_bg(affine_bg)
    {
        this->_set_refs(_affine_bg, _maps);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_maps(maps);
    }

    affine_bg_cached_animate_action(
            affine_bg_ptr&& affine_bg, int wait_updates, bool forever, span<affine_bg_map_ptr> maps) :
        _affine_bg(move(affine_bg))
    {
        this->_set_refs(_affine_bg, _maps);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_maps(maps);
    }
};


/**
 * @brief Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps only once.
 * @param affine_bg affine_bg_ptr to copy.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given affine_bg_ptr.
 * @param map_item It creates the new map to use by the given affine_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested affine_bg_cached_animate_action.
 *
 * @ingroup affine_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_affine_bg_cached_animate_action_once(
        const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)
{
    return affine_bg_cached_animate_action<sizeof...(Args)>::once(
                affine_bg, wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}


/**
 * @brief Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps only once.
 * @param affine_bg affine_bg_ptr to move.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given affine_bg_ptr.
 * @param map_item It creates the new map to use by the given affine_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested affine_bg_cached_animate_action.
 *
 * @ingroup affine_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_affine_bg_cached_animate_action_once(
        affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)
{
    return affine_bg_cached_animate_action<sizeof...(Args)>::once(
                move(affine_bg), wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}


/**
 * @brief Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps forever.
 * @param affine_bg affine_bg_ptr to copy.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given affine_bg_ptr.
 * @param map_item It creates the new map to use by the given affine_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested affine_bg_cached_animate_action.
 *
 * @ingroup affine_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_affine_bg_cached_animate_action_forever(
        const affine_bg_ptr& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)
{
    return affine_bg_cached_animate_action<sizeof...(Args)>::forever(
                affine_bg, wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}


/**
 * @brief Generates an affine_bg_cached_animate_action which loops over the given affine_bg maps forever.
 * @param affine_bg affine_bg_ptr to move.
 * @param wait_updates Number of times the action must be updated before changing the map
 * of the given affine_bg_ptr.
 * @param map_item It creates the new map to use by the given affine_bg_ptr.
 * @param map_indexes Indexes of the maps to reference in map_item.
 * @return The requested affine_bg_cached_animate_action.
 *
 * @ingroup affine_bg
 */
template<typename ...Args>
[[nodiscard]] auto create_affine_bg_cached_animate_action_forever(
        affine_bg_ptr&& affine_bg, int wait_updates, const affine_bg_map_item& map_item, Args ...map_indexes)
{
    return affine_bg_cached_animate_action<sizeof...(Args)>::forever(
                move(affine_bg), wait_updates, map_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(map_indexes)... }});
}

}

#endif
