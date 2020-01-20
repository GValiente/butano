#include "btn_sprite_tiles_bank.h"

#include "btn_span.h"
#include "btn_tile.h"
#include "btn_optional.h"
#include "../hw/include/btn_hw_sprite_tiles.h"

#if BTN_CFG_SPRITE_TILES_LOG_ENABLED
    #include "btn_log.h"
#endif

namespace btn
{

namespace
{
    static_assert(BTN_CFG_SPRITE_TILES_MAX_BANK_ITEMS > 0 &&
                  BTN_CFG_SPRITE_TILES_MAX_BANK_ITEMS <= hw::sprite_tiles::count_per_bank());
    static_assert(power_of_two(BTN_CFG_SPRITE_TILES_MAX_BANK_ITEMS));

    constexpr bool valid_tiles(int tiles)
    {
        return tiles == 1 || tiles == 2 || tiles == 4 || tiles == 8 || tiles == 16 || tiles == 32 ||
                tiles == 64 || tiles == 128;

        /*
        0000: 8  x 8    64      16      8
        0001: 16 x 16   256     64      32
        0010: 32 x 32   1024    256     128
        0011: 64 x 64   4096    1024    512
        0100: 16 x 8    128     32      16
        0101: 32 x 8    256     64      32
        0110: 32 x 16   512     128     64
        0111: 64 x 32   2048    512     256
        1000: 8  x 16   128     32      16
        1001: 8  x 32   256     64      32
        1010: 16 x 32   512     128     64
        1011: 32 x 64   2048    512     256
        */
    }
}

void sprite_tiles_bank::init(type bank_type)
{
    item_type new_item;
    new_item.tiles_count = hw::sprite_tiles::count_per_bank();

    if(bank_type == type::HIGH)
    {
        new_item.start_tile = hw::sprite_tiles::count_per_bank();
    }

    _items.init();
    _items.push_front(new_item);
    _biggest_free_iterator = _items.begin();
    _free_tiles_count = new_item.tiles_count;
}

optional<int> sprite_tiles_bank::find(const span<const tile>& tiles_ref)
{
    BTN_ASSERT(valid_tiles(int(tiles_ref.size())), "Invalid tiles ref size: ", tiles_ref.size());

    auto items_map_iterator = _items_map.find(tiles_ref.data());
    optional<int> result;

    if(items_map_iterator != _items_map.end())
    {
        auto id = int(items_map_iterator->second);
        item_type& item = _items.item(id);
        result.emplace(id);

        BTN_ASSERT(int(tiles_ref.size()) == item.tiles_count, "Tiles count does not match item tiles count: ",
                   tiles_ref.size(), " - ", item.tiles_count);

        switch(item.status())
        {

        case item_type::status_type::FREE:
            BTN_ERROR("Invalid item state");
            break;

        case item_type::status_type::USED:
            ++item.usages;
            break;

        case item_type::status_type::TO_REMOVE:
            item.usages = 1;
            item.set_status(item_type::status_type::USED);
            _to_remove_tiles_count -= item.tiles_count;
            break;
        }
    }

    return result;
}

optional<int> sprite_tiles_bank::create(const span<const tile>& tiles_ref)
{
    BTN_ASSERT(valid_tiles(int(tiles_ref.size())), "Invalid tiles ref size: ", tiles_ref.size());
    BTN_ASSERT(_items_map.find(tiles_ref.data()) == _items_map.end(),
               "Multiple copies of the same tiles data not supported");

    return _create_impl(tiles_ref.data(), int(tiles_ref.size()));
}

optional<int> sprite_tiles_bank::allocate(int tiles)
{
    BTN_ASSERT(valid_tiles(tiles), "Invalid tiles: ", tiles);

    return _create_impl(nullptr, tiles);
}

void sprite_tiles_bank::increase_usages(int id)
{
    item_type& item = _items.item(id);
    ++item.usages;
}

void sprite_tiles_bank::decrease_usages(int id)
{
    item_type& item = _items.item(id);
    --item.usages;

    if(! item.usages)
    {
        item.set_status(item_type::status_type::TO_REMOVE);
        _to_remove_tiles_count += item.tiles_count;
    }
}

optional<span<const tile>> sprite_tiles_bank::tiles_ref(int id) const
{
    const item_type& item = _items.item(id);
    optional<span<const tile>> result;

    if(item.data)
    {
        result.emplace(item.data, item.tiles_count);
    }

    return result;
}

void sprite_tiles_bank::set_tiles_ref(int id, const span<const tile>& tiles_ref)
{
    BTN_ASSERT(valid_tiles(int(tiles_ref.size())), "Invalid tiles ref size: ", tiles_ref.size());

    item_type& item = _items.item(id);
    BTN_ASSERT(item.data, "Item has no data");
    BTN_ASSERT(int(tiles_ref.size()) == item.tiles_count, "Tiles count does not match item tiles count: ",
               tiles_ref.size(), " - ", item.tiles_count);

    const tile* tiles_data = tiles_ref.data();

    if(item.data != tiles_data)
    {
        BTN_ASSERT(_items_map.find(tiles_data) == _items_map.end(),
                   "Multiple copies of the same tiles data not supported");

        _items_map.erase(item.data);
        _commit_item(id, *tiles_data, true);
    }
}

void sprite_tiles_bank::reload_tiles_ref(int id)
{
    item_type& item = _items.item(id);
    BTN_ASSERT(item.data, "Item has no data");

    item.commit = true;
    _check_commit = true;
}

optional<span<tile>> sprite_tiles_bank::vram(int id)
{
    item_type& item = _items.item(id);
    optional<span<tile>> result;

    if(! item.data)
    {
        result.emplace(&hw::sprite_tiles::vram(item.start_tile), item.tiles_count);
    }

    return result;
}

bool sprite_tiles_bank::update()
{
    bool to_update = _to_remove_tiles_count;

    if(to_update)
    {
        auto end = _items.end();
        auto before_previous_iterator = end;
        auto previous_iterator = _items.before_begin();
        auto iterator = previous_iterator;
        ++iterator;
        _to_remove_tiles_count = 0;

        while(iterator != end)
        {
            item_type& item = *iterator;

            if(item.status() == item_type::status_type::TO_REMOVE)
            {
                if(item.data)
                {
                    _items_map.erase(item.data);
                }

                item.data = nullptr;
                item.set_status(item_type::status_type::FREE);
                item.commit = false;
                _free_tiles_count += item.tiles_count;

                auto next_iterator = iterator;
                ++next_iterator;

                while(next_iterator != end)
                {
                    if(_remove_adjacent_item(next_iterator.id(), item))
                    {
                        next_iterator = _items.erase_after(iterator.id());
                    }
                    else
                    {
                        break;
                    }
                }

                if(before_previous_iterator != end)
                {
                    if(_remove_adjacent_item(previous_iterator.id(), item))
                    {
                        item.start_tile = previous_iterator->start_tile;
                        _items.erase_after(before_previous_iterator.id());
                        previous_iterator = before_previous_iterator;
                    }
                }

                if(_biggest_free_iterator != end)
                {
                    const item_type& biggest_free_item = *_biggest_free_iterator;

                    if(biggest_free_item.tiles_count < item.tiles_count)
                    {
                        _biggest_free_iterator = iterator;
                    }
                }
                else
                {
                    _biggest_free_iterator = iterator;
                }
            }

            before_previous_iterator = previous_iterator;
            previous_iterator = iterator;
            ++iterator;
        }
    }

    return to_update;
}

bool sprite_tiles_bank::commit()
{
    bool to_commit = _check_commit;

    if(to_commit)
    {
        _check_commit = false;

        for(item_type& item : _items)
        {
            if(item.commit)
            {
                item.commit = false;

                if(item.status() == item_type::status_type::USED)
                {
                    hw::sprite_tiles::commit(*item.data, item.start_tile, item.tiles_count);
                }
            }
        }
    }

    _delay_commit = false;
    return to_commit;
}

#if BTN_CFG_SPRITE_TILES_LOG_ENABLED
    void sprite_tiles_bank::log()
    {
        BTN_LOG("items: ", _items.size());
        BTN_LOG('[');

        for(item_type& item : _items)
        {
            BTN_LOG("    ",
                    (item.status() == item_type::status_type::FREE ? "free" :
                            item.status() == item_type::status_type::USED ? "used" : "to_remove"),
                    " - data: ", item.data,
                    " - start_tile: ", item.start_tile,
                    " - tiles_count: ", item.tiles_count,
                    " - usages: ", item.usages,
                    (item.commit ? " - commit" : " - no_commit"));
        }

        BTN_LOG(']');

        BTN_LOG("items_map: ", _items_map.size());
        BTN_LOG('[');

        for(const auto& items_map_item : _items_map)
        {
            BTN_LOG("    data: ", items_map_item.first,
                    " - start_tile: ", _items.item(items_map_item.second).start_tile);
        }

        BTN_LOG(']');

        if(_biggest_free_iterator == _items.end())
        {
            BTN_LOG("biggest_free_iterator: invalid");
        }
        else
        {
            BTN_LOG("biggest_free_iterator start_tile: ", _biggest_free_iterator->start_tile);
            BTN_LOG("biggest_free_iterator tiles_count: ", _biggest_free_iterator->tiles_count);
        }

        BTN_LOG("free_tiles_count: ", _free_tiles_count);
        BTN_LOG("to_remove_tiles_count: ", _to_remove_tiles_count);
        BTN_LOG("check_commit: ", (_check_commit ? "true" : "false"));
        BTN_LOG("delay_commit: ", (_delay_commit ? "true" : "false"));
    }
#endif

optional<int> sprite_tiles_bank::_create_impl(const tile* tiles_data, int tiles_count)
{
    if(! tiles_data && _delay_commit)
    {
        return nullopt;
    }

    if(tiles_count <= _free_tiles_count)
    {
        for(auto iterator = _biggest_free_iterator, end = _items.end(); iterator != end; ++iterator)
        {
            const item_type& item = *iterator;

            if(item.status() == item_type::status_type::FREE && item.tiles_count >= tiles_count)
            {
                int id = iterator.id();
                _create_item(id, tiles_data, tiles_count);
                return id;
            }
        }

        for(auto iterator = _items.begin(), end = _biggest_free_iterator; iterator != end; ++iterator)
        {
            const item_type& item = *iterator;

            if(item.status() == item_type::status_type::FREE && item.tiles_count >= tiles_count)
            {
                int id = iterator.id();
                _create_item(id, tiles_data, tiles_count);
                return id;
            }
        }
    }

    if(tiles_data && tiles_count <= _to_remove_tiles_count)
    {
        [[maybe_unused]] bool updated = update();
        _delay_commit = true;
        return _create_impl(tiles_data, tiles_count);
    }

    return nullopt;
}

void sprite_tiles_bank::_create_item(int id, const tile* tiles_data, int tiles_count)
{
    item_type& item = _items.item(id);
    int new_item_tiles_count = item.tiles_count - tiles_count;
    item.data = tiles_data;
    item.tiles_count = uint16_t(tiles_count);
    item.usages = 1;
    item.set_status(item_type::status_type::USED);
    item.commit = false;
    _free_tiles_count -= tiles_count;

    if(_biggest_free_iterator.id() == id)
    {
        _biggest_free_iterator = _items.end();
    }

    if(tiles_data)
    {
        _commit_item(id, *tiles_data, _delay_commit);
    }

    if(new_item_tiles_count)
    {
        BTN_ASSERT(! _items.full(), "No more items allowed");

        item_type new_item;
        new_item.start_tile = item.start_tile + item.tiles_count;
        new_item.tiles_count = uint16_t(new_item_tiles_count);

        auto new_item_iterator = _items.insert_after(id, new_item);

        if(_biggest_free_iterator != _items.end())
        {
            const item_type& biggest_free_item = *_biggest_free_iterator;

            if(biggest_free_item.tiles_count < new_item_tiles_count)
            {
                _biggest_free_iterator = new_item_iterator;
            }
        }
        else
        {
            _biggest_free_iterator = new_item_iterator;
        }
    }
}

void sprite_tiles_bank::_commit_item(int id, const tile& tiles_data, bool delay_commit)
{
    item_type& item = _items.item(id);
    item.data = &tiles_data;
    _items_map.insert(&tiles_data, id);

    if(delay_commit)
    {
        item.commit = true;
        _check_commit = true;
    }
    else
    {
        hw::sprite_tiles::commit(tiles_data, item.start_tile, item.tiles_count);
    }
}

bool sprite_tiles_bank::_remove_adjacent_item(int adjacent_id, item_type& current_item)
{
    const item_type& adjacent_item = _items.item(adjacent_id);
    item_type::status_type adjacent_item_status = adjacent_item.status();
    bool remove = adjacent_item_status != item_type::status_type::USED;

    if(remove)
    {
        current_item.tiles_count += adjacent_item.tiles_count;

        if(adjacent_item_status == item_type::status_type::TO_REMOVE)
        {
            if(adjacent_item.data)
            {
                _items_map.erase(adjacent_item.data);
            }

            _free_tiles_count += adjacent_item.tiles_count;
        }
        else
        {
            if(_biggest_free_iterator.id() == adjacent_id)
            {
                _biggest_free_iterator = _items.end();
            }
        }
    }

    return remove;
}

}
