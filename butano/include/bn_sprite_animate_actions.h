/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_ANIMATE_ACTIONS_H
#define BN_SPRITE_ANIMATE_ACTIONS_H

/**
 * @file
 * bn::sprite_ptr animate actions header file.
 *
 * @ingroup sprite
 * @ingroup tile
 * @ingroup action
 */

#include "bn_array.h"
#include "bn_vector.h"
#include "bn_limits.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_tiles_item.h"

namespace bn
{

// animation

/**
 * @brief Changes the tile set of a sprite_ptr when the action is updated a given number of times.
 *
 * Each time the tile set of a sprite_ptr must be changed, it is searched for and created if it has not been found,
 * so the tile sets are not cached.
 *
 * @tparam MaxSize Maximum number of indexes to sprite tile sets to store.
 *
 * @ingroup sprite
 * @ingroup tile
 * @ingroup action
 */
template<int MaxSize>
class sprite_animate_action
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
     * @brief Changes the tile set of the given sprite_ptr when the given amount of update calls are done.
     */
    void update()
    {
        BN_ASSERT(! done(), "Action is done");

        if(_current_wait_updates)
        {
            --_current_wait_updates;
        }
        else
        {
            int current_graphics_indexes_index = _current_graphics_indexes_index;
            int current_graphics_index = _graphics_indexes[current_graphics_indexes_index];
            _current_wait_updates = _wait_updates;

            if(current_graphics_indexes_index == 0 ||
                    _graphics_indexes[current_graphics_indexes_index - 1] != current_graphics_index)
            {
                _sprite.set_tiles(_tiles_item, current_graphics_index);
            }

            if(_forever && current_graphics_indexes_index == _graphics_indexes.size() - 1)
            {
                _current_graphics_indexes_index = 0;
            }
            else
            {
                ++_current_graphics_indexes_index;
            }
        }
    }

    /**
     * @brief Indicates if the action must not be updated anymore.
     */
    [[nodiscard]] bool done() const
    {
        return _current_graphics_indexes_index == _graphics_indexes.size();
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
        return _sprite;
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
    void set_wait_updates(int wait_updates)
    {
        BN_ASSERT(wait_updates >= 0, "Invalid wait updates: ", wait_updates);
        BN_ASSERT(wait_updates <= numeric_limits<decltype(_wait_updates)>::max(),
                   "Too much wait updates: ", wait_updates);

        _wait_updates = uint16_t(wait_updates);

        if(wait_updates < _current_wait_updates)
        {
            _current_wait_updates = wait_updates;
        }
    }

    /**
     * @brief Returns the sprite_tiles_item used to create the new sprite tiles to use by the given sprite_ptr.
     */
    [[nodiscard]] const sprite_tiles_item& tiles_item() const
    {
        return _tiles_item;
    }

    /**
     * @brief Returns the indexes of the tile sets to reference in the given sprite_tiles_item.
     */
    [[nodiscard]] const ivector<uint16_t>& graphics_indexes() const
    {
        return _graphics_indexes;
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

private:
    sprite_ptr _sprite;
    sprite_tiles_item _tiles_item;
    vector<uint16_t, MaxSize> _graphics_indexes;
    uint16_t _wait_updates = 0;
    uint16_t _current_graphics_indexes_index = 0;
    uint16_t _current_wait_updates = 0;
    bool _forever = true;

    sprite_animate_action(const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
                          bool forever, const span<const uint16_t>& graphics_indexes) :
        _sprite(sprite),
        _tiles_item(tiles_item),
        _forever(forever)
    {
        BN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= MaxSize,
                   "Invalid graphics indexes: ", graphics_indexes.size());

        set_wait_updates(wait_updates);

        for(uint16_t graphics_index : graphics_indexes)
        {
            _graphics_indexes.push_back(graphics_index);
        }
    }

    sprite_animate_action(sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, bool forever,
                          const span<const uint16_t>& graphics_indexes) :
        _sprite(move(sprite)),
        _tiles_item(tiles_item),
        _forever(forever)
    {
        BN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= MaxSize,
                   "Invalid graphics indexes: ", graphics_indexes.size());

        set_wait_updates(wait_updates);

        for(uint16_t graphics_index : graphics_indexes)
        {
            _graphics_indexes.push_back(graphics_index);
        }
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
 * @brief Changes the tile set of a sprite_ptr when the action is updated a given number of times.
 *
 * This action differs from sprite_animate_action in that in this action all sprite tile sets to use are cached,
 * so it is faster but takes way more VRAM.
 *
 * @tparam MaxSize Maximum number of sprite tile sets to store.
 *
 * @ingroup sprite
 * @ingroup tile
 * @ingroup action
 */
template<int MaxSize>
class sprite_cached_animate_action
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
     * @param tiles_list Tile sets to use by the given sprite_ptr.
     * @return The requested sprite_cached_animate_action.
     */
    [[nodiscard]] static sprite_cached_animate_action forever(
            sprite_ptr&& sprite, int wait_updates, span<sprite_tiles_ptr> tiles_list)
    {
        return sprite_cached_animate_action(move(sprite), wait_updates, true, tiles_list);
    }

    /**
     * @brief Changes the tile set of the given sprite_ptr when the given amount of update calls are done.
     */
    void update()
    {
        BN_ASSERT(! done(), "Action is done");

        if(_current_wait_updates)
        {
            --_current_wait_updates;
        }
        else
        {
            _current_wait_updates = _wait_updates;
            _sprite.set_tiles(_tiles_list[_current_tiles_list_index]);
            ++_current_tiles_list_index;

            if(_forever && _current_tiles_list_index == _tiles_list.size())
            {
                _current_tiles_list_index = 0;
            }
        }
    }

    /**
     * @brief Indicates if the action must not be updated anymore.
     */
    [[nodiscard]] bool done() const
    {
        return _current_tiles_list_index == _tiles_list.size();
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
        return _sprite;
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
    void set_wait_updates(int wait_updates)
    {
        BN_ASSERT(wait_updates >= 0, "Invalid wait updates: ", wait_updates);
        BN_ASSERT(wait_updates <= numeric_limits<decltype(_wait_updates)>::max(),
                   "Too much wait updates: ", wait_updates);

        _wait_updates = uint16_t(wait_updates);

        if(wait_updates < _current_wait_updates)
        {
            _current_wait_updates = wait_updates;
        }
    }

    /**
     * @brief Returns the tile sets to use by the given sprite_ptr.
     */
    [[nodiscard]] const ivector<sprite_tiles_ptr>& tiles_list() const
    {
        return _tiles_list;
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

private:
    sprite_ptr _sprite;
    vector<sprite_tiles_ptr, MaxSize> _tiles_list;
    uint16_t _wait_updates = 0;
    uint16_t _current_tiles_list_index = 0;
    uint16_t _current_wait_updates = 0;
    bool _forever = true;

    sprite_cached_animate_action(const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
                                 bool forever, const span<const uint16_t>& graphics_indexes) :
        _sprite(sprite),
        _forever(forever)
    {
        BN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= MaxSize,
                   "Invalid graphics indexes: ", graphics_indexes.size());

        set_wait_updates(wait_updates);

        for(int graphics_index : graphics_indexes)
        {
            _tiles_list.push_back(tiles_item.create_tiles(graphics_index));
        }
    }

    sprite_cached_animate_action(sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
                                 bool forever, const span<const uint16_t>& graphics_indexes) :
        _sprite(move(sprite)),
        _forever(forever)
    {
        BN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= MaxSize,
                   "Invalid graphics indexes: ", graphics_indexes.size());

        set_wait_updates(wait_updates);

        for(int graphics_index : graphics_indexes)
        {
            _tiles_list.push_back(tiles_item.create_tiles(graphics_index));
        }
    }

    sprite_cached_animate_action(const sprite_ptr& sprite, int wait_updates, bool forever,
                                 span<sprite_tiles_ptr> tiles_list) :
        _sprite(sprite),
        _forever(forever)
    {
        BN_ASSERT(tiles_list.size() > 1 && tiles_list.size() <= MaxSize, "Invalid tiles ptrs: ", tiles_list.size());

        set_wait_updates(wait_updates);

        for(sprite_tiles_ptr& tiles : tiles_list)
        {
            _tiles_list.push_back(move(tiles));
        }
    }

    sprite_cached_animate_action(sprite_ptr&& sprite, int wait_updates, bool forever,
                                 span<sprite_tiles_ptr> tiles_list) :
        _sprite(move(sprite)),
        _forever(forever)
    {
        BN_ASSERT(tiles_list.size() > 1 && tiles_list.size() <= MaxSize, "Invalid tiles ptrs: ", tiles_list.size());

        set_wait_updates(wait_updates);

        for(sprite_tiles_ptr& tiles : tiles_list)
        {
            _tiles_list.push_back(move(tiles));
        }
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
