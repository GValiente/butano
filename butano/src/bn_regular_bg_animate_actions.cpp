#include "bn_regular_bg_animate_actions.h"

#include "bn_limits.h"
#include "bn_bg_palette_ptr.h"
#include "bn_regular_bg_tiles_ptr.h"

namespace bn
{

iregular_bg_animate_action& iregular_bg_animate_action::operator=(const iregular_bg_animate_action& other)
{
    if(this != &other)
    {
        BN_ASSERT(other.map_indexes().size() <= map_indexes().max_size(),
                  "Too many map indexes: ", other.map_indexes().size(), " - ", map_indexes().max_size());

        *_regular_bg_ref = *other._regular_bg_ref;
        *_map_item_ref = *other._map_item_ref;
        *_map_indexes_ref = *other._map_indexes_ref;
        _assign(other);
    }

    return *this;
}

iregular_bg_animate_action& iregular_bg_animate_action::operator=(iregular_bg_animate_action&& other) noexcept
{
    if(this != &other)
    {
        BN_ASSERT(other.map_indexes().size() <= map_indexes().max_size(),
                  "Too many map indexes: ", other.map_indexes().size(), " - ", map_indexes().max_size());

        *_regular_bg_ref = move(*other._regular_bg_ref);
        *_map_item_ref = *other._map_item_ref;
        *_map_indexes_ref = *other._map_indexes_ref;
        _assign(other);
    }

    return *this;
}

void iregular_bg_animate_action::update()
{
    BN_ASSERT(! done(), "Action is done");

    if(_current_wait_updates)
    {
        --_current_wait_updates;
    }
    else
    {
        const ivector<uint16_t>& map_indexes = this->map_indexes();
        int current_map_indexes_index = _current_map_indexes_index;
        int current_map_index = map_indexes[current_map_indexes_index];
        _current_wait_updates = _wait_updates;

        if(current_map_indexes_index == 0 || map_indexes[current_map_indexes_index - 1] != current_map_index)
        {
            _regular_bg_ref->set_map(*_map_item_ref, current_map_index);
        }

        if(_forever && current_map_indexes_index == map_indexes.size() - 1)
        {
            _current_map_indexes_index = 0;
        }
        else
        {
            ++_current_map_indexes_index;
        }
    }
}

void iregular_bg_animate_action::set_wait_updates(int wait_updates)
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

void iregular_bg_animate_action::_set_refs(
        regular_bg_ptr& regular_bg, regular_bg_map_item& map_item, ivector<uint16_t>& map_indexes)
{
    _regular_bg_ref = &regular_bg;
    _map_item_ref = &map_item;
    _map_indexes_ref = &map_indexes;
}

void iregular_bg_animate_action::_assign(const iregular_bg_animate_action& other)
{
    _wait_updates = other._wait_updates;
    _current_map_indexes_index = other._current_map_indexes_index;
    _current_wait_updates = other._current_wait_updates;
    _forever = other._forever;
}

void iregular_bg_animate_action::_assign_map_indexes(const span<const uint16_t>& map_indexes)
{
    BN_ASSERT(map_indexes.size() > 1 && map_indexes.size() <= _map_indexes_ref->max_size(),
              "Invalid map indexes count: ", map_indexes.size());

    for(uint16_t map_index : map_indexes)
    {
        _map_indexes_ref->push_back(map_index);
    }
}

void iregular_bg_animate_action::_assign_map_indexes(const ivector<uint16_t>& map_indexes)
{
    BN_ASSERT(map_indexes.size() > 1 && map_indexes.size() <= _map_indexes_ref->max_size(),
              "Invalid map indexes count: ", map_indexes.size());

    *_map_indexes_ref = map_indexes;
}

iregular_bg_cached_animate_action& iregular_bg_cached_animate_action::operator=(
        const iregular_bg_cached_animate_action& other)
{
    if(this != &other)
    {
        BN_ASSERT(other.maps().size() <= maps().max_size(),
                  "Too many maps: ", other.maps().size(), " - ", maps().max_size());

        *_regular_bg_ref = *other._regular_bg_ref;
        *_maps_ref = *other._maps_ref;
        _assign(other);
    }

    return *this;
}

iregular_bg_cached_animate_action& iregular_bg_cached_animate_action::operator=(
        iregular_bg_cached_animate_action&& other) noexcept
{
    if(this != &other)
    {
        BN_ASSERT(other.maps().size() <= maps().max_size(),
                  "Too many maps: ", other.maps().size(), " - ", maps().max_size());

        *_regular_bg_ref = move(*other._regular_bg_ref);
        *_maps_ref = move(*other._maps_ref);
        _assign(other);
    }

    return *this;
}

void iregular_bg_cached_animate_action::update()
{
    BN_ASSERT(! done(), "Action is done");

    if(_current_wait_updates)
    {
        --_current_wait_updates;
    }
    else
    {
        _current_wait_updates = _wait_updates;
        _regular_bg_ref->set_map((*_maps_ref)[_current_map_index]);
        ++_current_map_index;

        if(_forever && _current_map_index == _maps_ref->size())
        {
            _current_map_index = 0;
        }
    }
}

void iregular_bg_cached_animate_action::set_wait_updates(int wait_updates)
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

void iregular_bg_cached_animate_action::_set_refs(regular_bg_ptr& regular_bg, ivector<regular_bg_map_ptr>& maps)
{
    _regular_bg_ref = &regular_bg;
    _maps_ref = &maps;
}

void iregular_bg_cached_animate_action::_assign(const iregular_bg_cached_animate_action& other)
{
    _wait_updates = other._wait_updates;
    _current_map_index = other._current_map_index;
    _current_wait_updates = other._current_wait_updates;
    _forever = other._forever;
}

void iregular_bg_cached_animate_action::_assign_map_indexes(
        const regular_bg_map_item& map_item, const span<const uint16_t>& map_indexes)
{
    BN_ASSERT(map_indexes.size() > 1 && map_indexes.size() <= _maps_ref->max_size(),
              "Invalid map indexes count: ", map_indexes.size(), " - ", _maps_ref->max_size());

    const regular_bg_tiles_ptr& tiles = _regular_bg_ref->tiles();
    const bg_palette_ptr& palette = _regular_bg_ref->palette();

    for(int map_index : map_indexes)
    {
        _maps_ref->push_back(regular_bg_map_ptr::create(map_item, tiles, palette, map_index));
    }
}

void iregular_bg_cached_animate_action::_assign_maps(span<regular_bg_map_ptr> maps)
{
    BN_ASSERT(maps.size() > 1 && maps.size() <= _maps_ref->max_size(),
              "Invalid maps count: ", maps.size(), " - ", _maps_ref->max_size());

    for(regular_bg_map_ptr& map : maps)
    {
        _maps_ref->push_back(move(map));
    }
}

void iregular_bg_cached_animate_action::_assign_maps(const ivector<regular_bg_map_ptr>& maps)
{
    BN_ASSERT(maps.size() > 1 && maps.size() <= _maps_ref->max_size(),
              "Invalid maps count: ", maps.size(), " - ", _maps_ref->max_size());

    *_maps_ref = maps;
}

void iregular_bg_cached_animate_action::_assign_maps(ivector<regular_bg_map_ptr>&& maps)
{
    BN_ASSERT(maps.size() > 1 && maps.size() <= _maps_ref->max_size(),
              "Invalid maps count: ", maps.size(), " - ", _maps_ref->max_size());

    *_maps_ref = move(maps);
}

}
