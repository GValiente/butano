#include "btn_sprite_tiles_manager.h"

#include "btn_span.h"
#include "btn_optional.h"
#include "btn_sprite_tiles_bank.h"

#if BTN_CFG_SPRITE_TILES_LOG_ENABLED
    #include "btn_log.h"
    #include "btn_tile.h"

    static_assert(BTN_CFG_LOG_ENABLED, "Log is not enabled");
#endif

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

    data.high_bank.init(sprite_tiles_bank::type::HIGH);
    data.low_bank.init(sprite_tiles_bank::type::LOW);

    BTN_SPRITE_TILES_LOG_BANKS();
}

optional<int> find(const span<const tile>& tiles_ref)
{
    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - FIND: ", tiles_ref.data(), " - ", tiles_ref.size());

    optional<int> result = data.high_bank.find(tiles_ref);

    if(result)
    {
        BTN_SPRITE_TILES_LOG("FOUND in high bank. start_tile: ", data.high_bank.start_tile(*result));

        *result += high_bank_delta;
    }
    else
    {
        result = data.low_bank.find(tiles_ref);

        if(result)
        {
            BTN_SPRITE_TILES_LOG("FOUND in low bank. start_tile: ", data.low_bank.start_tile(*result));
        }
        else
        {
            BTN_SPRITE_TILES_LOG("NOT FOUND");
        }
    }

    BTN_SPRITE_TILES_LOG_BANKS();

    return result;
}

optional<int> create(const span<const tile>& tiles_ref)
{
    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - CREATE: ", tiles_ref.data(), " - ", tiles_ref.size());

    optional<int> result = data.high_bank.create(tiles_ref);

    if(result)
    {
        BTN_SPRITE_TILES_LOG("CREATED in high bank. start_tile: ", data.high_bank.start_tile(*result));

        *result += high_bank_delta;
    }
    else
    {
        result = data.low_bank.create(tiles_ref);

        if(result)
        {
            BTN_SPRITE_TILES_LOG("CREATED in low bank. start_tile: ", data.low_bank.start_tile(*result));
        }
        else
        {
            BTN_SPRITE_TILES_LOG("NOT CREATED in any bank");
        }
    }

    BTN_SPRITE_TILES_LOG_BANKS();

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
    if(id >= high_bank_delta)
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - SET_TILES_REF: ", data.high_bank.start_tile(id - high_bank_delta),
                             " - ", tiles_ref.data(), " - ", tiles_ref.size());

        data.high_bank.set_tiles_ref(id - high_bank_delta, tiles_ref);
    }
    else
    {
        BTN_SPRITE_TILES_LOG("sprite_tiles_manager - SET_TILES_REF: ", data.low_bank.start_tile(id),
                             " - ", tiles_ref.data(), " - ", tiles_ref.size());

        data.low_bank.set_tiles_ref(id, tiles_ref);
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
