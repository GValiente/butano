/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_ANIMATE_ACTIONS_H
#define BTN_SPRITE_ANIMATE_ACTIONS_H

#include "btn_array.h"
#include "btn_vector.h"
#include "btn_limits.h"
#include "btn_sprite_ptr.h"
#include "btn_sprite_tiles_ptr.h"
#include "btn_sprite_tiles_item.h"

namespace btn
{

// animation

template<int Size>
class sprite_animate_action
{
    static_assert(Size > 1);

public:
    [[nodiscard]] static sprite_animate_action once(
            const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_animate_action(sprite, wait_updates, tiles_item, false, graphics_indexes);
    }

    [[nodiscard]] static sprite_animate_action once(
            sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_animate_action(move(sprite), wait_updates, tiles_item, false, graphics_indexes);
    }

    [[nodiscard]] static sprite_animate_action forever(
            const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_animate_action(sprite, wait_updates, tiles_item, true, graphics_indexes);
    }

    [[nodiscard]] static sprite_animate_action forever(
            sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_animate_action(move(sprite), wait_updates, tiles_item, true, graphics_indexes);
    }

    void reset()
    {
        _current_graphics_indexes_index = 0;
        _current_wait_updates = 0;
    }

    void update()
    {
        BTN_ASSERT(! done(), "Action is done");

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

    [[nodiscard]] bool done() const
    {
        return _current_graphics_indexes_index == _graphics_indexes.size();
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite;
    }

    [[nodiscard]] int wait_updates() const
    {
        return _wait_updates;
    }

    [[nodiscard]] const sprite_tiles_item& tiles_item() const
    {
        return _tiles_item;
    }

    [[nodiscard]] const vector<uint16_t, Size>& graphics_indexes() const
    {
        return _graphics_indexes;
    }

    [[nodiscard]] bool update_forever() const
    {
        return _forever;
    }

    [[nodiscard]] int current_index() const
    {
        return _current_graphics_indexes_index;
    }

private:
    bool _forever = true;
    uint16_t _wait_updates = 0;
    sprite_ptr _sprite;
    sprite_tiles_item _tiles_item;
    vector<uint16_t, Size> _graphics_indexes;
    uint16_t _current_graphics_indexes_index = 0;
    uint16_t _current_wait_updates = 0;

    sprite_animate_action(const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
                          bool forever, const span<const uint16_t>& graphics_indexes) :
        _forever(forever),
        _wait_updates(uint16_t(wait_updates)),
        _sprite(sprite),
        _tiles_item(tiles_item)
    {
        BTN_ASSERT(wait_updates >= 0, "Invalid wait updates: ", wait_updates);
        BTN_ASSERT(wait_updates <= numeric_limits<decltype(_wait_updates)>::max(),
                   "Too much wait updates: ", wait_updates);
        BTN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= Size,
                   "Invalid graphics indexes: ", graphics_indexes.size());

        for(int graphics_index : graphics_indexes)
        {
            _graphics_indexes.push_back(graphics_index);
        }
    }

    sprite_animate_action(sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, bool forever,
                          const span<const uint16_t>& graphics_indexes) :
        _forever(forever),
        _wait_updates(uint16_t(wait_updates)),
        _sprite(move(sprite)),
        _tiles_item(tiles_item)
    {
        BTN_ASSERT(wait_updates >= 0, "Invalid wait updates: ", wait_updates);
        BTN_ASSERT(wait_updates <= numeric_limits<decltype(_wait_updates)>::max(),
                   "Too much wait updates: ", wait_updates);
        BTN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= Size,
                   "Invalid graphics indexes: ", graphics_indexes.size());

        for(int graphics_index : graphics_indexes)
        {
            _graphics_indexes.push_back(graphics_index);
        }
    }
};

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_animate_action_once(
        const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_animate_action<sizeof...(Args)>::once(
                sprite, wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_animate_action_once(
        sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_animate_action<sizeof...(Args)>::once(
                move(sprite), wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_animate_action_forever(
        const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_animate_action<sizeof...(Args)>::forever(
                sprite, wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_animate_action_forever(
        sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_animate_action<sizeof...(Args)>::forever(
                move(sprite), wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}


// cached animation

template<int Size>
class sprite_cached_animate_action
{
    static_assert(Size > 1);

public:
    [[nodiscard]] static sprite_cached_animate_action once(
            const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_cached_animate_action(sprite, wait_updates, tiles_item, false, graphics_indexes);
    }

    [[nodiscard]] static sprite_cached_animate_action once(
            sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_cached_animate_action(move(sprite), wait_updates, tiles_item, false, graphics_indexes);
    }

    [[nodiscard]] static sprite_cached_animate_action once(
            const sprite_ptr& sprite, int wait_updates, span<sprite_tiles_ptr> tiles_list)
    {
        return sprite_cached_animate_action(sprite, wait_updates, false, tiles_list);
    }

    [[nodiscard]] static sprite_cached_animate_action once(
            sprite_ptr&& sprite, int wait_updates, span<sprite_tiles_ptr> tiles_list)
    {
        return sprite_cached_animate_action(move(sprite), wait_updates, false, tiles_list);
    }

    [[nodiscard]] static sprite_cached_animate_action forever(
            const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_cached_animate_action(sprite, wait_updates, tiles_item, true, graphics_indexes);
    }

    [[nodiscard]] static sprite_cached_animate_action forever(
            sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
            const span<const uint16_t>& graphics_indexes)
    {
        return sprite_cached_animate_action(move(sprite), wait_updates, tiles_item, true, graphics_indexes);
    }

    [[nodiscard]] static sprite_cached_animate_action forever(
            const sprite_ptr& sprite, int wait_updates, span<sprite_tiles_ptr> tiles_list)
    {
        return sprite_cached_animate_action(sprite, wait_updates, true, tiles_list);
    }

    [[nodiscard]] static sprite_cached_animate_action forever(
            sprite_ptr&& sprite, int wait_updates, span<sprite_tiles_ptr> tiles_list)
    {
        return sprite_cached_animate_action(move(sprite), wait_updates, true, tiles_list);
    }

    void reset()
    {
        _current_tiles_list_index = 0;
        _current_wait_updates = 0;
    }

    void update()
    {
        BTN_ASSERT(! done(), "Action is done");

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

    [[nodiscard]] bool done() const
    {
        return _current_tiles_list_index == _tiles_list.size();
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite;
    }

    [[nodiscard]] int wait_updates() const
    {
        return _wait_updates;
    }

    [[nodiscard]] const vector<sprite_tiles_ptr, Size>& tiles_list() const
    {
        return _tiles_list;
    }

    [[nodiscard]] bool update_forever() const
    {
        return _forever;
    }

    [[nodiscard]] int current_index() const
    {
        return _current_tiles_list_index;
    }

private:
    bool _forever = true;
    uint16_t _wait_updates = 0;
    sprite_ptr _sprite;
    vector<sprite_tiles_ptr, Size> _tiles_list;
    uint16_t _current_tiles_list_index = 0;
    uint16_t _current_wait_updates = 0;

    sprite_cached_animate_action(const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
                                 bool forever, const span<const uint16_t>& graphics_indexes) :
        _forever(forever),
        _wait_updates(uint16_t(wait_updates)),
        _sprite(sprite)
    {
        BTN_ASSERT(wait_updates >= 0, "Invalid wait updates: ", wait_updates);
        BTN_ASSERT(wait_updates <= numeric_limits<decltype(_wait_updates)>::max(),
                   "Too much wait updates: ", wait_updates);
        BTN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= Size,
                   "Invalid graphics indexes: ", graphics_indexes.size());

        for(int graphics_index : graphics_indexes)
        {
            _tiles_list.push_back(tiles_item.create_tiles(graphics_index));
        }
    }

    sprite_cached_animate_action(sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item,
                                 bool forever, const span<const uint16_t>& graphics_indexes) :
        _forever(forever),
        _wait_updates(uint16_t(wait_updates)),
        _sprite(move(sprite))
    {
        BTN_ASSERT(wait_updates >= 0, "Invalid wait updates: ", wait_updates);
        BTN_ASSERT(wait_updates <= numeric_limits<decltype(_wait_updates)>::max(),
                   "Too much wait updates: ", wait_updates);
        BTN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= Size,
                   "Invalid graphics indexes: ", graphics_indexes.size());

        for(int graphics_index : graphics_indexes)
        {
            _tiles_list.push_back(tiles_item.create_tiles(graphics_index));
        }
    }

    sprite_cached_animate_action(const sprite_ptr& sprite, int wait_updates, bool forever,
                                 span<sprite_tiles_ptr> tiles_list) :
        _forever(forever),
        _wait_updates(uint16_t(wait_updates)),
        _sprite(sprite)
    {
        BTN_ASSERT(wait_updates >= 0, "Invalid wait updates: ", wait_updates);
        BTN_ASSERT(wait_updates <= numeric_limits<decltype(_wait_updates)>::max(),
                   "Too much wait updates: ", wait_updates);
        BTN_ASSERT(tiles_list.size() > 1 && tiles_list.size() <= Size, "Invalid tiles ptrs: ", tiles_list.size());

        for(sprite_tiles_ptr& tiles : tiles_list)
        {
            _tiles_list.push_back(move(tiles));
        }
    }

    sprite_cached_animate_action(sprite_ptr&& sprite, int wait_updates, bool forever,
                                 span<sprite_tiles_ptr> tiles_list) :
        _forever(forever),
        _wait_updates(uint16_t(wait_updates)),
        _sprite(move(sprite))
    {
        BTN_ASSERT(wait_updates >= 0, "Invalid wait updates: ", wait_updates);
        BTN_ASSERT(wait_updates <= numeric_limits<decltype(_wait_updates)>::max(),
                   "Too much wait updates: ", wait_updates);
        BTN_ASSERT(tiles_list.size() > 1 && tiles_list.size() <= Size, "Invalid tiles ptrs: ", tiles_list.size());

        for(sprite_tiles_ptr& tiles : tiles_list)
        {
            _tiles_list.push_back(move(tiles));
        }
    }
};

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_cached_animate_action_once(
        const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_cached_animate_action<sizeof...(Args)>::once(
                sprite, wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_cached_animate_action_once(
        sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_cached_animate_action<sizeof...(Args)>::once(
                move(sprite), wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

template< typename ...Args>
[[nodiscard]] inline auto create_sprite_cached_animate_action_forever(
        const sprite_ptr& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_cached_animate_action<sizeof...(Args)>::forever(
                sprite, wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

template< typename ...Args>
[[nodiscard]] inline auto create_sprite_cached_animate_action_forever(
        sprite_ptr&& sprite, int wait_updates, const sprite_tiles_item& tiles_item, Args ...graphics_indexes)
{
    return sprite_cached_animate_action<sizeof...(Args)>::forever(
                move(sprite), wait_updates, tiles_item,
                array<uint16_t, sizeof...(Args)>{{ uint16_t(graphics_indexes)... }});
}

}

#endif
