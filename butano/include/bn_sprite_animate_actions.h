/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_ANIMATE_ACTIONS_H
#define BN_SPRITE_ANIMATE_ACTIONS_H

/**
 * @file
 * bn::sprite_ptr animate actions implementation header file.
 *
 * @ingroup sprite
 * @ingroup tile
 * @ingroup action
 */

#include "bn_array.h"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_tiles_item.h"
#include "bn_sprite_animate_actions_fwd.h"

namespace bn
{

// animation

/**
 * @brief Base class of bn::sprite_animate_action.
 *
 * Can be used as a reference type for all bn::sprite_animate_action objects.
 *
 * @ingroup sprite
 * @ingroup tile
 * @ingroup action
 */
class isprite_animate_action
{

public:
    isprite_animate_action(const isprite_animate_action& other) = delete;

    /**
     * @brief Copy assignment operator.
     * @param other isprite_animate_action to copy.
     * @return Reference to this.
     */
    isprite_animate_action& operator=(const isprite_animate_action& other);

    /**
     * @brief Move assignment operator.
     * @param other isprite_animate_action to move.
     * @return Reference to this.
     */
    isprite_animate_action& operator=(isprite_animate_action&& other) noexcept;

    /**
     * @brief Changes the tile set of the given sprite_ptr when the given amount of update calls are done.
     */
    void update();

    /**
     * @brief Indicates if the action must not be updated anymore.
     */
    [[nodiscard]] bool done() const
    {
        return _current_graphics_indexes_index == _graphics_indexes_ref->size();
    }

    /**
     * @brief Resets the action to its initial state.
     */
    void reset()
    {
        _current_graphics_indexes_index = 0;
        _current_wait_updates = 0;
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return *_sprite_ref;
    }

    /**
     * @brief Returns the number of times the action must be updated before changing the tiles
     * of the given sprite_ptr.
     */
    [[nodiscard]] int wait_updates() const
    {
        return _wait_updates;
    }

    /**
     * @brief Sets the number of times the action must be updated before changing the tiles
     * of the given sprite_ptr.
     */
    void set_wait_updates(int wait_updates);

    /**
     * @brief Returns the number of times the action must be updated before the next tiles change.
     */
    [[nodiscard]] int next_change_updates() const
    {
        return _current_wait_updates;
    }

    /**
     * @brief Returns the sprite_tiles_item used to create the new sprite tiles to use by the given sprite_ptr.
     */
    [[nodiscard]] const sprite_tiles_item& tiles_item() const
    {
        return *_tiles_item_ref;
    }

    /**
     * @brief Returns the indexes of the tile sets to reference in the given sprite_tiles_item.
     */
    [[nodiscard]] const ivector<uint16_t>& graphics_indexes() const
    {
        return *_graphics_indexes_ref;
    }

    /**
     * @brief Indicates if the action can be updated forever or not.
     */
    [[nodiscard]] bool update_forever() const
    {
        return _forever;
    }

    /**
     * @brief Returns the current index of the given graphics_indexes
     * (not the current index of the tile set to reference in the given tiles_item).
     */
    [[nodiscard]] int current_index() const
    {
        return _current_graphics_indexes_index;
    }

    /**
     * @brief Returns the current index of the tile set to reference in the given tiles_item.
     */
    [[nodiscard]] int current_graphics_index() const
    {
        return graphics_indexes()[_current_graphics_indexes_index];
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    isprite_animate_action() = default;

    void _set_refs(sprite_ptr& sprite, sprite_tiles_item& tiles_item, ivector<uint16_t>& graphics_indexes);

    void _assign(const isprite_animate_action& other);

    void _set_update_forever(bool forever)
    {
        _forever = forever;
    }

    void _assign_graphics_indexes(const span<const uint16_t>& graphics_indexes);

    void _assign_graphics_indexes(const ivector<uint16_t>& graphics_indexes);

    /// @endcond

private:
    sprite_ptr* _sprite_ref = nullptr;
    sprite_tiles_item* _tiles_item_ref = nullptr;
    ivector<uint16_t>* _graphics_indexes_ref = nullptr;
    uint16_t _wait_updates = 0;
    uint16_t _current_graphics_indexes_index = 0;
    uint16_t _current_wait_updates = 0;
    bool _forever = true;
};

template<int MaxSize>
class sprite_animate_action : public isprite_animate_action
{
    static_assert(MaxSize > 1);

public:
    /**
     * @brief Generates a sprite_animate_action which loops over the given sprite tile sets only once.
     * @param sprite sprite_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
     * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
     * @param graphics_indexes Indexes of the tile sets to reference in tiles_item.
     * @return The requested sprite_animate_action.
     */
    [[nodiscard]] static sprite_animate_action once(
            const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_animate_action(sprite, wait_updates, tiles_item, false, graphics_indexes);
    }

    /**
     * @brief Generates a sprite_animate_action which loops over the given sprite tile sets only once.
     * @param sprite sprite_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
     * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
     * @param graphics_indexes Indexes of the tile sets to reference in the given sprite_tiles_item.
     * @return The requested sprite_animate_action.
     */
    [[nodiscard]] static sprite_animate_action once(
            sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_animate_action(move(sprite), wait_updates, tiles_item, false, graphics_indexes);
    }

    /**
     * @brief Generates a sprite_animate_action which loops over the given sprite tile sets forever.
     * @param sprite sprite_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
     * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
     * @param graphics_indexes Indexes of the tile sets to reference in the given sprite_tiles_item.
     * @return The requested sprite_animate_action.
     */
    [[nodiscard]] static sprite_animate_action forever(
            const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_animate_action(sprite, wait_updates, tiles_item, true, graphics_indexes);
    }

    /**
     * @brief Generates a sprite_animate_action which loops over the given sprite tile sets forever.
     * @param sprite sprite_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
     * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
     * @param graphics_indexes Indexes of the tile sets to reference in the given sprite_tiles_item.
     * @return The requested sprite_animate_action.
     */
    [[nodiscard]] static sprite_animate_action forever(
            sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_animate_action(move(sprite), wait_updates, tiles_item, true, graphics_indexes);
    }

    /**
     * @brief Copy constructor.
     * @param other sprite_animate_action to copy.
     */
    sprite_animate_action(const sprite_animate_action& other) :
        _sprite(other._sprite),
        _tiles_item(other._tiles_item),
        _graphics_indexes(other._graphics_indexes)
    {
        this->_set_refs(_sprite, _tiles_item, _graphics_indexes);
        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other sprite_animate_action to move.
     */
    sprite_animate_action(sprite_animate_action&& other) noexcept :
        _sprite(move(other._sprite)),
        _tiles_item(other._tiles_item),
        _graphics_indexes(other._graphics_indexes)
    {
        this->_set_refs(_sprite, _tiles_item, _graphics_indexes);
        this->_assign(other);
    }

    /**
     * @brief Copy constructor.
     * @param other isprite_animate_action to copy.
     */
    sprite_animate_action(const isprite_animate_action& other) :
        _sprite(other.sprite()),
        _tiles_item(other.tiles_item()),
        _graphics_indexes(other.graphics_indexes())
    {
        BN_ASSERT(other.graphics_indexes().size() <= MaxSize,
                  "Too many graphics indexes: ", other.graphics_indexes().size(), " - ", MaxSize);

        this->_set_refs(_sprite, _tiles_item, _graphics_indexes);
        this->_assign(other);
    }

    /**
     * @brief Copy assignment operator.
     * @param other sprite_animate_action to copy.
     * @return Reference to this.
     */
    sprite_animate_action& operator=(const sprite_animate_action& other)
    {
        if(this != &other)
        {
            _sprite = other._sprite;
            _tiles_item = other._tiles_item;
            _graphics_indexes = other._graphics_indexes;
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other sprite_animate_action to move.
     * @return Reference to this.
     */
    sprite_animate_action& operator=(sprite_animate_action&& other) noexcept
    {
        if(this != &other)
        {
            _sprite = move(other._sprite);
            _tiles_item = other._tiles_item;
            _graphics_indexes = other._graphics_indexes;
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other isprite_animate_action to copy.
     * @return Reference to this.
     */
    sprite_animate_action& operator=(const isprite_animate_action& other)
    {
        static_cast<isprite_animate_action&>(*this) = other;
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other ivector to move.
     * @return Reference to this.
     */
    sprite_animate_action& operator=(isprite_animate_action&& other) noexcept
    {
        static_cast<isprite_animate_action&>(*this) = move(other);
        return *this;
    }

private:
    sprite_ptr _sprite;
    sprite_tiles_item _tiles_item;
    vector<uint16_t, MaxSize> _graphics_indexes;

    sprite_animate_action(const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
                          bool forever, const span<const uint16_t>& graphics_indexes) :
        _sprite(sprite),
        _tiles_item(tiles_item)
    {
        this->_set_refs(_sprite, _tiles_item, _graphics_indexes);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_graphics_indexes(graphics_indexes);
    }

    sprite_animate_action(sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, bool forever,
                          const span<const uint16_t>& graphics_indexes) :
        _sprite(move(sprite)),
        _tiles_item(tiles_item)
    {
        this->_set_refs(_sprite, _tiles_item, _graphics_indexes);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_graphics_indexes(graphics_indexes);
    }
};


/**
 * @brief Generates a sprite_animate_action which loops over the given sprite tile sets only once.
 * @param sprite sprite_ptr to copy.
 * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
 * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
 * @param graphics_indexes Indexes of the tile sets to reference in tiles_item.
 * @return The requested sprite_animate_action.
 *
 * @ingroup sprite
 */
template<typename ...Args>
[[nodiscard]] auto create_sprite_animate_action_once(
        const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_animate_action<sizeof...(Args)>::once(
                sprite, wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}


/**
 * @brief Generates a sprite_animate_action which loops over the given sprite tile sets only once.
 * @param sprite sprite_ptr to move.
 * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
 * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
 * @param graphics_indexes Indexes of the tile sets to reference in tiles_item.
 * @return The requested sprite_animate_action.
 *
 * @ingroup sprite
 */
template<typename ...Args>
[[nodiscard]] auto create_sprite_animate_action_once(
        sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_animate_action<sizeof...(Args)>::once(
                move(sprite), wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}


/**
 * @brief Generates a sprite_animate_action which loops over the given sprite tile sets forever.
 * @param sprite sprite_ptr to copy.
 * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
 * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
 * @param graphics_indexes Indexes of the tile sets to reference in tiles_item.
 * @return The requested sprite_animate_action.
 *
 * @ingroup sprite
 */
template<typename ...Args>
[[nodiscard]] auto create_sprite_animate_action_forever(
        const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_animate_action<sizeof...(Args)>::forever(
                sprite, wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}


/**
 * @brief Generates a sprite_animate_action which loops over the given sprite tile sets forever.
 * @param sprite sprite_ptr to move.
 * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
 * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
 * @param graphics_indexes Indexes of the tile sets to reference in tiles_item.
 * @return The requested sprite_animate_action.
 *
 * @ingroup sprite
 */
template<typename ...Args>
[[nodiscard]] auto create_sprite_animate_action_forever(
        sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_animate_action<sizeof...(Args)>::forever(
                move(sprite), wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}


// cached animation

/**
 * @brief Base class of bn::sprite_cached_animate_action.
 *
 * Can be used as a reference type for all bn::sprite_cached_animate_action objects.
 *
 * @ingroup sprite
 * @ingroup tile
 * @ingroup action
 */
class isprite_cached_animate_action
{

public:
    isprite_cached_animate_action(const isprite_cached_animate_action& other) = delete;

    /**
     * @brief Copy assignment operator.
     * @param other isprite_cached_animate_action to copy.
     * @return Reference to this.
     */
    isprite_cached_animate_action& operator=(const isprite_cached_animate_action& other);

    /**
     * @brief Move assignment operator.
     * @param other isprite_cached_animate_action to move.
     * @return Reference to this.
     */
    isprite_cached_animate_action& operator=(isprite_cached_animate_action&& other) noexcept;

    /**
     * @brief Changes the tile set of the given sprite_ptr when the given amount of update calls are done.
     */
    void update();

    /**
     * @brief Indicates if the action must not be updated anymore.
     */
    [[nodiscard]] bool done() const
    {
        return _current_tiles_list_index == _tiles_list_ref->size();
    }

    /**
     * @brief Resets the action to its initial state.
     */
    void reset()
    {
        _current_tiles_list_index = 0;
        _current_wait_updates = 0;
    }

    /**
     * @brief Returns the sprite_ptr to modify.
     */
    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return *_sprite_ref;
    }

    /**
     * @brief Returns the number of times the action must be updated before changing the tiles
     * of the given sprite_ptr.
     */
    [[nodiscard]] int wait_updates() const
    {
        return _wait_updates;
    }

    /**
     * @brief Sets the number of times the action must be updated before changing the tiles
     * of the given sprite_ptr.
     */
    void set_wait_updates(int wait_updates);

    /**
     * @brief Returns the number of times the action must be updated before the next tiles change.
     */
    [[nodiscard]] int next_change_updates() const
    {
        return _current_wait_updates;
    }

    /**
     * @brief Returns the tile sets to use by the given sprite_ptr.
     */
    [[nodiscard]] const ivector<sprite_tiles_ptr>& tiles_list() const
    {
        return *_tiles_list_ref;
    }

    /**
     * @brief Indicates if the action can be updated forever or not.
     */
    [[nodiscard]] bool update_forever() const
    {
        return _forever;
    }

    /**
     * @brief Returns the current index of the given tile sets.
     */
    [[nodiscard]] int current_index() const
    {
        return _current_tiles_list_index;
    }

    /**
     * @brief Returns the current tile set.
     */
    [[nodiscard]] const sprite_tiles_ptr& current_tiles() const
    {
        return tiles_list()[_current_tiles_list_index];
    }

protected:
    /// @cond DO_NOT_DOCUMENT

    isprite_cached_animate_action() = default;

    void _set_refs(sprite_ptr& sprite, ivector<sprite_tiles_ptr>& tiles_list);

    void _assign(const isprite_cached_animate_action& other);

    void _set_update_forever(bool forever)
    {
        _forever = forever;
    }

    void _assign_graphics_indexes(
            const sprite_tiles_item& tiles_item, const span<const uint16_t>& graphics_indexes);

    void _assign_tiles_list(span<sprite_tiles_ptr> tiles_list);

    void _assign_tiles_list(const ivector<sprite_tiles_ptr>& tiles_list);

    void _assign_tiles_list(ivector<sprite_tiles_ptr>&& tiles_list);

    /// @endcond

private:
    sprite_ptr* _sprite_ref = nullptr;
    ivector<sprite_tiles_ptr>* _tiles_list_ref = nullptr;
    uint16_t _wait_updates = 0;
    uint16_t _current_tiles_list_index = 0;
    uint16_t _current_wait_updates = 0;
    bool _forever = true;
};

template<int MaxSize>
class sprite_cached_animate_action : public isprite_cached_animate_action
{
    static_assert(MaxSize > 1);

public:
    /**
     * @brief Generates a sprite_cached_animate_action which loops over the given sprite tile sets only once.
     * @param sprite sprite_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
     * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
     * @param graphics_indexes Indexes of the tile sets to reference in tiles_item.
     * @return The requested sprite_cached_animate_action.
     */
    [[nodiscard]] static sprite_cached_animate_action once(
            const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_cached_animate_action(sprite, wait_updates, tiles_item, false, graphics_indexes);
    }

    /**
     * @brief Generates a sprite_cached_animate_action which loops over the given sprite tile sets only once.
     * @param sprite sprite_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
     * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
     * @param graphics_indexes Indexes of the tile sets to reference in the given sprite_tiles_item.
     * @return The requested sprite_cached_animate_action.
     */
    [[nodiscard]] static sprite_cached_animate_action once(
            sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_cached_animate_action(move(sprite), wait_updates, tiles_item, false, graphics_indexes);
    }

    /**
     * @brief Generates a sprite_cached_animate_action which loops over the given sprite tile sets only once.
     * @param sprite sprite_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
     * @param tiles_list List of tile sets to use by the given sprite_ptr.
     * @return The requested sprite_cached_animate_action.
     */
    [[nodiscard]] static sprite_cached_animate_action once(
            const sprite_ptr& sprite, int wait_updates, span<sprite_tiles_ptr> tiles_list)
    {
        return sprite_cached_animate_action(sprite, wait_updates, false, tiles_list);
    }

    /**
     * @brief Generates a sprite_cached_animate_action which loops over the given sprite tile sets only once.
     * @param sprite sprite_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
     * @param tiles_list List of tile sets to use by the given sprite_ptr.
     * @return The requested sprite_cached_animate_action.
     */
    [[nodiscard]] static sprite_cached_animate_action once(
            sprite_ptr&& sprite, int wait_updates, span<sprite_tiles_ptr> tiles_list)
    {
        return sprite_cached_animate_action(move(sprite), wait_updates, false, tiles_list);
    }

    /**
     * @brief Generates a sprite_cached_animate_action which loops over the given sprite tile sets forever.
     * @param sprite sprite_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
     * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
     * @param graphics_indexes Indexes of the tile sets to reference in tiles_item.
     * @return The requested sprite_cached_animate_action.
     */
    [[nodiscard]] static sprite_cached_animate_action forever(
            const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_cached_animate_action(sprite, wait_updates, tiles_item, true, graphics_indexes);
    }

    /**
     * @brief Generates a sprite_cached_animate_action which loops over the given sprite tile sets forever.
     * @param sprite sprite_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
     * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
     * @param graphics_indexes Indexes of the tile sets to reference in the given sprite_tiles_item.
     * @return The requested sprite_cached_animate_action.
     */
    [[nodiscard]] static sprite_cached_animate_action forever(
            sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_cached_animate_action(move(sprite), wait_updates, tiles_item, true, graphics_indexes);
    }

    /**
     * @brief Generates a sprite_cached_animate_action which loops over the given sprite tile sets forever.
     * @param sprite sprite_ptr to copy.
     * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
     * @param tiles_list List of tile sets to use by the given sprite_ptr.
     * @return The requested sprite_cached_animate_action.
     */
    [[nodiscard]] static sprite_cached_animate_action forever(
            const sprite_ptr& sprite, int wait_updates, span<sprite_tiles_ptr> tiles_list)
    {
        return sprite_cached_animate_action(sprite, wait_updates, true, tiles_list);
    }

    /**
     * @brief Generates a sprite_cached_animate_action which loops over the given sprite tile sets forever.
     * @param sprite sprite_ptr to move.
     * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
     * @param tiles_list List of tile sets to use by the given sprite_ptr.
     * @return The requested sprite_cached_animate_action.
     */
    [[nodiscard]] static sprite_cached_animate_action forever(
            sprite_ptr&& sprite, int wait_updates, span<sprite_tiles_ptr> tiles_list)
    {
        return sprite_cached_animate_action(move(sprite), wait_updates, true, tiles_list);
    }

    /**
     * @brief Copy constructor.
     * @param other sprite_cached_animate_action to copy.
     */
    sprite_cached_animate_action(const sprite_cached_animate_action& other) :
        _sprite(other._sprite),
        _tiles_list(other._tiles_list)
    {
        this->_set_refs(_sprite, _tiles_list);
        this->_assign(other);
    }

    /**
     * @brief Move constructor.
     * @param other sprite_cached_animate_action to move.
     */
    sprite_cached_animate_action(sprite_cached_animate_action&& other) noexcept :
        _sprite(move(other._sprite)),
        _tiles_list(move(other._tiles_list))
    {
        this->_set_refs(_sprite, _tiles_list);
        this->_assign(other);
    }

    /**
     * @brief Copy constructor.
     * @param other isprite_cached_animate_action to copy.
     */
    sprite_cached_animate_action(const isprite_cached_animate_action& other) :
        _sprite(other.sprite()),
        _tiles_list(other.tiles_list())
    {
        BN_ASSERT(other.tiles_list().size() <= MaxSize,
                  "Too many tiles: ", other.tiles_list().size(), " - ", MaxSize);

        this->_set_refs(_sprite, _tiles_list);
        this->_assign(other);
    }

    /**
     * @brief Copy assignment operator.
     * @param other sprite_cached_animate_action to copy.
     * @return Reference to this.
     */
    sprite_cached_animate_action& operator=(const sprite_cached_animate_action& other)
    {
        if(this != &other)
        {
            _sprite = other._sprite;
            _tiles_list = other._tiles_list;
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other sprite_cached_animate_action to move.
     * @return Reference to this.
     */
    sprite_cached_animate_action& operator=(sprite_cached_animate_action&& other) noexcept
    {
        if(this != &other)
        {
            _sprite = move(other._sprite);
            _tiles_list = move(other._tiles_list);
            this->_assign(other);
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param other isprite_cached_animate_action to copy.
     * @return Reference to this.
     */
    sprite_cached_animate_action& operator=(const isprite_cached_animate_action& other)
    {
        static_cast<isprite_cached_animate_action&>(*this) = other;
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param other ivector to move.
     * @return Reference to this.
     */
    sprite_cached_animate_action& operator=(isprite_cached_animate_action&& other) noexcept
    {
        static_cast<isprite_cached_animate_action&>(*this) = move(other);
        return *this;
    }

private:
    sprite_ptr _sprite;
    vector<sprite_tiles_ptr, MaxSize> _tiles_list;

    sprite_cached_animate_action(const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
                                 bool forever, const span<const uint16_t>& graphics_indexes) :
        _sprite(sprite)
    {
        this->_set_refs(_sprite, _tiles_list);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_graphics_indexes(tiles_item, graphics_indexes);
    }

    sprite_cached_animate_action(sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
                                 bool forever, const span<const uint16_t>& graphics_indexes) :
        _sprite(move(sprite))
    {
        this->_set_refs(_sprite, _tiles_list);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_graphics_indexes(tiles_item, graphics_indexes);
    }

    sprite_cached_animate_action(const sprite_ptr& sprite, int wait_updates, bool forever,
                                 span<sprite_tiles_ptr> tiles_list) :
        _sprite(sprite)
    {
        this->_set_refs(_sprite, _tiles_list);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_tiles_list(tiles_list);
    }

    sprite_cached_animate_action(sprite_ptr&& sprite, int wait_updates, bool forever,
                                 span<sprite_tiles_ptr> tiles_list) :
        _sprite(move(sprite))
    {
        this->_set_refs(_sprite, _tiles_list);
        this->_set_update_forever(forever);
        this->set_wait_updates(wait_updates);
        this->_assign_tiles_list(tiles_list);
    }
};


/**
 * @brief Generates a sprite_cached_animate_action which loops over the given sprite tile sets only once.
 * @param sprite sprite_ptr to copy.
 * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
 * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
 * @param graphics_indexes Indexes of the tile sets to reference in tiles_item.
 * @return The requested sprite_cached_animate_action.
 *
 * @ingroup sprite
 */
template<typename ...Args>
[[nodiscard]] auto create_sprite_cached_animate_action_once(
        const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_cached_animate_action<sizeof...(Args)>::once(
                sprite, wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}


/**
 * @brief Generates a sprite_cached_animate_action which loops over the given sprite tile sets only once.
 * @param sprite sprite_ptr to move.
 * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
 * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
 * @param graphics_indexes Indexes of the tile sets to reference in tiles_item.
 * @return The requested sprite_cached_animate_action.
 *
 * @ingroup sprite
 */
template<typename ...Args>
[[nodiscard]] auto create_sprite_cached_animate_action_once(
        sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_cached_animate_action<sizeof...(Args)>::once(
                move(sprite), wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}


/**
 * @brief Generates a sprite_cached_animate_action which loops over the given sprite tile sets forever.
 * @param sprite sprite_ptr to copy.
 * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
 * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
 * @param graphics_indexes Indexes of the tile sets to reference in tiles_item.
 * @return The requested sprite_cached_animate_action.
 *
 * @ingroup sprite
 */
template<typename ...Args>
[[nodiscard]] auto create_sprite_cached_animate_action_forever(
        const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_cached_animate_action<sizeof...(Args)>::forever(
                sprite, wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}


/**
 * @brief Generates a sprite_cached_animate_action which loops over the given sprite tile sets forever.
 * @param sprite sprite_ptr to move.
 * @param wait_updates Number of times the action must be updated before changing the tiles of the given sprite_ptr.
 * @param tiles_item It creates the new sprite tiles to use by the given sprite_ptr.
 * @param graphics_indexes Indexes of the tile sets to reference in tiles_item.
 * @return The requested sprite_cached_animate_action.
 *
 * @ingroup sprite
 */
template<typename ...Args>
[[nodiscard]] auto create_sprite_cached_animate_action_forever(
        sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_cached_animate_action<sizeof...(Args)>::forever(
                move(sprite), wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

}

#endif
