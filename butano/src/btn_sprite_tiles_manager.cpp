#include "btn_sprite_tiles_manager.h"

#include "btn_span.h"
#include "btn_tile.h"
#include "btn_optional.h"
#include "btn_sprite_tiles_bank.h"

#if BTN_CFG_SPRITE_TILES_LOG_ENABLED
    #include "btn_log.h"
    #include "btn_tile.h"

    static_assert(BTN_CFG_LOG_ENABLED, "Log is not enabled");
#endif

#include "btn_profiler.h"

namespace btn::sprite_tiles_manager
{

namespace
{
    constexpr const int high_bank_delta = sprite_tiles_bank::max_items;

    class static_data
    {

    public:
        sprite_tiles_bank high_bank;
        sprite_tiles_bank low_bank;
        hash_map<const tile*, uint16_t, high_bank_delta * 2 * 2> items_map;
    };

    BTN_DATA_EWRAM static_data data;

    #if BTN_CFG_SPRITE_TILES_LOG_ENABLED
        void log_banks()
        {
            BTN_LOG("sprite_tiles_manager - High bank: ");
            data.high_bank.log();
            BTN_LOG("sprite_tiles_manager - Low bank: ");
            data.low_bank.log();
            BTN_LOG(' ');
        }

        #define BTN_SPRITE_TILES_LOG BTN_LOG

        #define BTN_SPRITE_TILES_LOG_BANKS \
            log_banks
    #else
        #define BTN_SPRITE_TILES_LOG(...) \
            do \
            { \
            } while(false)

        #define BTN_SPRITE_TILES_LOG_BANKS(...) \
            do \
            { \
            } while(false)
    #endif
}

void init()
{
    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - INIT");

    data.high_bank.init(sprite_tiles_bank::type::HIGH, data.items_map);
    data.low_bank.init(sprite_tiles_bank::type::LOW, data.items_map);

    BTN_SPRITE_TILES_LOG_BANKS();
}

optional<int> find(const span<const tile>& tiles_ref)
{
    BTN_PROFILER_START("find tiles");

    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - FIND: ", tiles_ref.data(), " - ", tiles_ref.size());

    const tile* tiles_ptr = tiles_ref.data();
    BTN_ASSERT(tiles_ptr, "Tiles ref is null");

    auto items_map_iterator = data.items_map.find(tiles_ptr);
    optional<int> result;

    if(items_map_iterator != data.items_map.end())
    {
        int id = items_map_iterator->second;

        if(id >= high_bank_delta)
        {
            data.high_bank.find(id - high_bank_delta, tiles_ref);

            BTN_SPRITE_TILES_LOG("FOUND in high bank. start_tile: ", data.high_bank.start_tile(id - high_bank_delta));
        }
        else
        {
            data.low_bank.find(id, tiles_ref);

            BTN_SPRITE_TILES_LOG("FOUND in low bank. start_tile: ", data.low_bank.start_tile(*result));
        }

        result = id;
    }
    else
    {
        BTN_SPRITE_TILES_LOG("NOT FOUND");
    }

    BTN_SPRITE_TILES_LOG_BANKS();

    BTN_PROFILER_STOP();

    return result;
}

optional<int> create(const span<const tile>& tiles_ref)
{
    BTN_PROFILER_START("create tiles");

    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - CREATE: ", tiles_ref.data(), " - ", tiles_ref.size());

    BTN_ASSERT(data.items_map.find(tiles_ref.data()) == data.items_map.end(),
               "Multiple copies of the same tiles data not supported");

    optional<int> result = data.high_bank.create(tiles_ref);

    if(result)
    {
        BTN_SPRITE_TILES_LOG("CREATED in high bank. start_tile: ", data.high_bank.start_tile(*result));

        *result += high_bank_delta;
        data.items_map.insert(tiles_ref.data(), *result);
    }
    else
    {
        result = data.low_bank.create(tiles_ref);

        if(result)
        {
            BTN_SPRITE_TILES_LOG("CREATED in low bank. start_tile: ", data.low_bank.start_tile(*result));

            data.items_map.insert(tiles_ref.data(), *result);
        }
        else
        {
            BTN_SPRITE_TILES_LOG("NOT CREATED in any bank");
        }
    }

    BTN_SPRITE_TILES_LOG_BANKS();

    BTN_PROFILER_STOP();

    return result;
}

optional<int> allocate(int tiles)
{
    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - ALLOCATE: ", tiles);

    optional<int> result = data.high_bank.allocate(tiles);

    if(result)
    {
        BTN_SPRITE_TILES_LOG("ALLOCATED in high bank. start_tile: ", data.high_bank.start_tile(*result));

        *result += high_bank_delta;
    }
    else
    {
        result = data.low_bank.allocate(tiles);

        if(result)
        {
            BTN_SPRITE_TILES_LOG("ALLOCATED in low bank. start_tile: ", data.low_bank.start_tile(*result));
        }
        else
        {
            BTN_SPRITE_TILES_LOG("NOT ALLOCATED in any bank");
        }
    }

    BTN_SPRITE_TILES_LOG_BANKS();

    return result;
}

void increase_usages(int id)
{
    if(id >= high_bank_delta)
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - INCREASE_USAGES: ", data.high_bank.start_tile(id - high_bank_delta));

        data.high_bank.increase_usages(id - high_bank_delta);
    }
    else
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - INCREASE_USAGES: ", data.low_bank.start_tile(id));

        data.low_bank.increase_usages(id);
    }

    BTN_SPRITE_TILES_LOG_BANKS();
}

void decrease_usages(int id)
{
    if(id >= high_bank_delta)
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - DECREASE_USAGES: ", data.high_bank.start_tile(id - high_bank_delta));

        data.high_bank.decrease_usages(id - high_bank_delta);
    }
    else
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - DECREASE_USAGES: ", data.low_bank.start_tile(id));

        data.low_bank.decrease_usages(id);
    }

    BTN_SPRITE_TILES_LOG_BANKS();
}

int start_tile(int id)
{
    if(id >= high_bank_delta)
    {
        return data.high_bank.start_tile(id - high_bank_delta);
    }
    else
    {
        return data.low_bank.start_tile(id);
    }
}

int tiles_count(int id)
{
    if(id >= high_bank_delta)
    {
        return data.high_bank.tiles_count(id - high_bank_delta);
    }
    else
    {
        return data.low_bank.tiles_count(id);
    }
}

optional<span<const tile>> tiles_ref(int id)
{
    if(id >= high_bank_delta)
    {
        return data.high_bank.tiles_ref(id - high_bank_delta);
    }
    else
    {
        return data.low_bank.tiles_ref(id);
    }
}

void set_tiles_ref(int id, const span<const tile>& tiles_ref)
{
    BTN_ASSERT(data.items_map.find(tiles_ref.data()) == data.items_map.end(),
               "Multiple copies of the same tiles data not supported");

    if(id >= high_bank_delta)
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - SET_TILES_REF: ", data.high_bank.start_tile(id - high_bank_delta),
                             " - ", tiles_ref.data(), " - ", tiles_ref.size());

        optional<span<const tile>> old_tiles_ref_opt = data.high_bank.tiles_ref(id - high_bank_delta);
        BTN_ASSERT(old_tiles_ref_opt, "Item has no data");

        const span<const tile>& old_tiles_ref = *old_tiles_ref_opt;
        BTN_ASSERT(tiles_ref.size() == old_tiles_ref.size(), "Tiles count does not match item tiles count: ",
                   tiles_ref.size(), " - ", old_tiles_ref.size());

        const tile* old_tiles_data = old_tiles_ref.data();
        const tile* tiles_data = tiles_ref.data();

        if(old_tiles_data != tiles_data)
        {
            data.items_map.erase(old_tiles_data);
            data.high_bank.set_tiles_ref(id - high_bank_delta, *tiles_data);
            data.items_map.insert(tiles_data, id);
        }
    }
    else
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - SET_TILES_REF: ", data.low_bank.start_tile(id),
                             " - ", tiles_ref.data(), " - ", tiles_ref.size());

        optional<span<const tile>> old_tiles_ref_opt = data.low_bank.tiles_ref(id);
        BTN_ASSERT(old_tiles_ref_opt, "Item has no data");

        const span<const tile>& old_tiles_ref = *old_tiles_ref_opt;
        BTN_ASSERT(tiles_ref.size() == old_tiles_ref.size(), "Tiles count does not match item tiles count: ",
                   tiles_ref.size(), " - ", old_tiles_ref.size());

        const tile* old_tiles_data = old_tiles_ref.data();
        const tile* tiles_data = tiles_ref.data();

        if(old_tiles_data != tiles_data)
        {
            data.items_map.erase(old_tiles_data);
            data.low_bank.set_tiles_ref(id, *tiles_data);
            data.items_map.insert(tiles_data, id);
        }
    }

    BTN_SPRITE_TILES_LOG_BANKS();
}

void reload_tiles_ref(int id)
{
    if(id >= high_bank_delta)
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - RELOAD_TILES_REF: ", data.high_bank.start_tile(id - high_bank_delta));

        data.high_bank.reload_tiles_ref(id - high_bank_delta);
    }
    else
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - RELOAD_TILES_REF: ", data.low_bank.start_tile(id));

        data.low_bank.reload_tiles_ref(id);
    }

    BTN_SPRITE_TILES_LOG_BANKS();
}

optional<span<tile>> vram(int id)
{
    if(id >= high_bank_delta)
    {
        return data.high_bank.vram(id - high_bank_delta);
    }
    else
    {
        return data.low_bank.vram(id);
    }
}

void update()
{
    bool updated = false;

    if(data.high_bank.update())
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - UPDATE: high bank");

        updated = true;
    }

    if(data.low_bank.update())
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - UPDATE: low bank");

        updated = true;
    }

    if(updated)
    {
        BTN_SPRITE_TILES_LOG_BANKS();
    }
}

void commit()
{
    bool committed = false;

    if(data.high_bank.commit())
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - COMMIT: high bank");

        committed = true;
    }

    if(data.low_bank.commit())
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - COMMIT: low bank");

        committed = true;
    }

    if(committed)
    {
        BTN_SPRITE_TILES_LOG_BANKS();
    }
}

}
