#include "bn_sprite_animate_actions.h"

#include "bn_limits.h"

namespace bn
{

isprite_animate_action& isprite_animate_action::operator=(const isprite_animate_action& other)
{
    if(this != &other)
    {
        BN_ASSERT(other.graphics_indexes().size() <= graphics_indexes().max_size(),
                  "Too many graphics indexes: ", other.graphics_indexes().size(), " - ",
                  graphics_indexes().max_size());

        *_sprite_ref = *other._sprite_ref;
        *_tiles_item_ref = *other._tiles_item_ref;
        *_graphics_indexes_ref = *other._graphics_indexes_ref;
        _assign(other);
    }

    return *this;
}

isprite_animate_action& isprite_animate_action::operator=(isprite_animate_action&& other) noexcept
{
    if(this != &other)
    {
        BN_ASSERT(other.graphics_indexes().size() <= graphics_indexes().max_size(),
                  "Too many graphics indexes: ", other.graphics_indexes().size(), " - ",
                  graphics_indexes().max_size());

        *_sprite_ref = move(*other._sprite_ref);
        *_tiles_item_ref = *other._tiles_item_ref;
        *_graphics_indexes_ref = *other._graphics_indexes_ref;
        _assign(other);
    }

    return *this;
}

void isprite_animate_action::update()
{
    BN_ASSERT(! done(), "Action is done");

    if(_current_wait_updates)
    {
        --_current_wait_updates;
    }
    else
    {
        const ivector<uint16_t>& graphics_indexes = this->graphics_indexes();
        int current_graphics_indexes_index = _current_graphics_indexes_index;
        int current_graphics_index = graphics_indexes[current_graphics_indexes_index];
        _current_wait_updates = _wait_updates;

        if(current_graphics_indexes_index == 0 ||
                graphics_indexes[current_graphics_indexes_index - 1] != current_graphics_index)
        {
            _sprite_ref->set_tiles(*_tiles_item_ref, current_graphics_index);
        }

        if(_forever && current_graphics_indexes_index == graphics_indexes.size() - 1)
        {
            _current_graphics_indexes_index = 0;
        }
        else
        {
            ++_current_graphics_indexes_index;
        }
    }
}

void isprite_animate_action::set_wait_updates(int wait_updates)
{
    BN_ASSERT(wait_updates >= 0, "Invalid wait updates: ", wait_updates);
    BN_ASSERT(wait_updates <= numeric_limits<decltype(_wait_updates)>::max(),
              "Too many wait updates: ", wait_updates);

    _wait_updates = uint16_t(wait_updates);

    if(wait_updates < _current_wait_updates)
    {
        _current_wait_updates = uint16_t(wait_updates);
    }
}

void isprite_animate_action::_set_refs(
        sprite_ptr& sprite, sprite_tiles_item& tiles_item, ivector<uint16_t>& graphics_indexes)
{
    _sprite_ref = &sprite;
    _tiles_item_ref = &tiles_item;
    _graphics_indexes_ref = &graphics_indexes;
}

void isprite_animate_action::_assign(const isprite_animate_action& other)
{
    _wait_updates = other._wait_updates;
    _current_graphics_indexes_index = other._current_graphics_indexes_index;
    _current_wait_updates = other._current_wait_updates;
    _forever = other._forever;
}

void isprite_animate_action::_assign_graphics_indexes(const span<const uint16_t>& graphics_indexes)
{
    BN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= _graphics_indexes_ref->max_size(),
              "Invalid graphics indexes count: ", graphics_indexes.size());

    for(uint16_t graphics_index : graphics_indexes)
    {
        _graphics_indexes_ref->push_back(graphics_index);
    }
}

void isprite_animate_action::_assign_graphics_indexes(const ivector<uint16_t>& graphics_indexes)
{
    BN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= _graphics_indexes_ref->max_size(),
              "Invalid graphics indexes count: ", graphics_indexes.size());

    *_graphics_indexes_ref = graphics_indexes;
}

isprite_cached_animate_action& isprite_cached_animate_action::operator=(
        const isprite_cached_animate_action& other)
{
    if(this != &other)
    {
        BN_ASSERT(other.tiles_list().size() <= tiles_list().max_size(),
                  "Too many tiles: ", other.tiles_list().size(), " - ", tiles_list().max_size());

        *_sprite_ref = *other._sprite_ref;
        *_tiles_list_ref = *other._tiles_list_ref;
        _assign(other);
    }

    return *this;
}

isprite_cached_animate_action& isprite_cached_animate_action::operator=(
        isprite_cached_animate_action&& other) noexcept
{
    if(this != &other)
    {
        BN_ASSERT(other.tiles_list().size() <= tiles_list().max_size(),
                  "Too many tiles: ", other.tiles_list().size(), " - ", tiles_list().max_size());

        *_sprite_ref = move(*other._sprite_ref);
        *_tiles_list_ref = move(*other._tiles_list_ref);
        _assign(other);
    }

    return *this;
}

void isprite_cached_animate_action::update()
{
    BN_ASSERT(! done(), "Action is done");

    if(_current_wait_updates)
    {
        --_current_wait_updates;
    }
    else
    {
        _current_wait_updates = _wait_updates;
        _sprite_ref->set_tiles((*_tiles_list_ref)[_current_tiles_list_index]);
        ++_current_tiles_list_index;

        if(_forever && _current_tiles_list_index == _tiles_list_ref->size())
        {
            _current_tiles_list_index = 0;
        }
    }
}

void isprite_cached_animate_action::set_wait_updates(int wait_updates)
{
    BN_ASSERT(wait_updates >= 0, "Invalid wait updates: ", wait_updates);
    BN_ASSERT(wait_updates <= numeric_limits<decltype(_wait_updates)>::max(),
              "Too many wait updates: ", wait_updates);

    _wait_updates = uint16_t(wait_updates);

    if(wait_updates < _current_wait_updates)
    {
        _current_wait_updates = uint16_t(wait_updates);
    }
}

void isprite_cached_animate_action::_set_refs(sprite_ptr& sprite, ivector<sprite_tiles_ptr>& tiles_list)
{
    _sprite_ref = &sprite;
    _tiles_list_ref = &tiles_list;
}

void isprite_cached_animate_action::_assign(const isprite_cached_animate_action& other)
{
    _wait_updates = other._wait_updates;
    _current_tiles_list_index = other._current_tiles_list_index;
    _current_wait_updates = other._current_wait_updates;
    _forever = other._forever;
}

void isprite_cached_animate_action::_assign_graphics_indexes(
        const sprite_tiles_item& tiles_item, const span<const uint16_t>& graphics_indexes)
{
    BN_ASSERT(graphics_indexes.size() > 1 && graphics_indexes.size() <= _tiles_list_ref->max_size(),
              "Invalid graphics indexes count: ", graphics_indexes.size(), " - ", _tiles_list_ref->max_size());

    for(int graphics_index : graphics_indexes)
    {
        _tiles_list_ref->push_back(sprite_tiles_ptr::create(tiles_item, graphics_index));
    }
}

void isprite_cached_animate_action::_assign_tiles_list(span<sprite_tiles_ptr> tiles_list)
{
    BN_ASSERT(tiles_list.size() > 1 && tiles_list.size() <= _tiles_list_ref->max_size(),
              "Invalid tiles count: ", tiles_list.size(), " - ", _tiles_list_ref->max_size());

    for(sprite_tiles_ptr& tiles : tiles_list)
    {
        _tiles_list_ref->push_back(move(tiles));
    }
}

void isprite_cached_animate_action::_assign_tiles_list(const ivector<sprite_tiles_ptr>& tiles_list)
{
    BN_ASSERT(tiles_list.size() > 1 && tiles_list.size() <= _tiles_list_ref->max_size(),
              "Invalid tiles count: ", tiles_list.size(), " - ", _tiles_list_ref->max_size());

    *_tiles_list_ref = tiles_list;
}

void isprite_cached_animate_action::_assign_tiles_list(ivector<sprite_tiles_ptr>&& tiles_list)
{
    BN_ASSERT(tiles_list.size() > 1 && tiles_list.size() <= _tiles_list_ref->max_size(),
              "Invalid tiles count: ", tiles_list.size(), " - ", _tiles_list_ref->max_size());

    *_tiles_list_ref = move(tiles_list);
}

}
