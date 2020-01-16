#include "btn_sprite_tiles_manager.h"

#include "btn_span.h"
#include "btn_optional.h"
#include "btn_sprite_tiles_bank.h"

#if BTN_CFG_SPRITE_TILES_LOG_ENABLED
    #include "btn_log.h"

    static_assert(BTN_CFG_LOG_ENABLED, "Log is not enabled");
#endif

namespace btn::sprite_tiles_manager
{

namespace
{
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

    data.high_bank.init(item_type::bank_type::HIGH);
    data.low_bank.init(item_type::bank_type::LOW);

    BTN_SPRITE_TILES_LOG_BANKS();
}

optional<item_type::list_iterator> find(const span<const tile>& tiles_ref)
{
    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - FIND: ", tiles_ref.data(), " - ", tiles_ref.size());

    optional<item_type::list_iterator> result = data.high_bank.find(tiles_ref);

    if(result)
    {
        BTN_SPRITE_TILES_LOG("FOUND in high bank. start_tile: ", (*result)->start_tile);
    }
    else
    {
        result = data.low_bank.find(tiles_ref);

        if(result)
        {
            BTN_SPRITE_TILES_LOG("FOUND in low bank. start_tile: ", (*result)->start_tile);
        }
        else
        {
            BTN_SPRITE_TILES_LOG("NOT FOUND");
        }
    }

    BTN_SPRITE_TILES_LOG_BANKS();

    return result;
}

optional<item_type::list_iterator> create(const span<const tile>& tiles_ref)
{
    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - CREATE: ", tiles_ref.data(), " - ", tiles_ref.size());

    optional<item_type::list_iterator> result = data.high_bank.create(tiles_ref);

    if(result)
    {
        BTN_SPRITE_TILES_LOG("CREATED in high bank. start_tile: ", (*result)->start_tile);
    }
    else
    {
        result = data.low_bank.create(tiles_ref);

        if(result)
        {
            BTN_SPRITE_TILES_LOG("CREATED in low bank. start_tile: ", (*result)->start_tile);
        }
        else
        {
            BTN_SPRITE_TILES_LOG("NOT CREATED in any bank");
        }
    }

    BTN_SPRITE_TILES_LOG_BANKS();

    return result;
}

optional<item_type::list_iterator> allocate(int tiles)
{
    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - ALLOCATE: ", tiles);

    optional<item_type::list_iterator> result = data.high_bank.allocate(tiles);

    if(result)
    {
        BTN_SPRITE_TILES_LOG("ALLOCATED in high bank. start_tile: ", (*result)->start_tile);
    }
    else
    {
        result = data.low_bank.allocate(tiles);

        if(result)
        {
            BTN_SPRITE_TILES_LOG("ALLOCATED in low bank. start_tile: ", (*result)->start_tile);
        }
        else
        {
            BTN_SPRITE_TILES_LOG("NOT ALLOCATED in any bank");
        }
    }

    BTN_SPRITE_TILES_LOG_BANKS();

    return result;
}

item_type::list_iterator invalid_iterator()
{
    return data.high_bank.invalid_iterator();
}

void increase_usages(item_type::list_iterator iterator)
{
    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - INCREASE_USAGES: ", iterator->start_tile);

    if(iterator->bank() == item_type::bank_type::HIGH)
    {
        data.high_bank.increase_usages(iterator);
    }
    else
    {
        data.low_bank.increase_usages(iterator);
    }

    BTN_SPRITE_TILES_LOG_BANKS();
}

void decrease_usages(item_type::list_iterator iterator)
{
    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - DECREASE_USAGES: ", iterator->start_tile);

    if(iterator->bank() == item_type::bank_type::HIGH)
    {
        data.high_bank.decrease_usages(iterator);
    }
    else
    {
        data.low_bank.decrease_usages(iterator);
    }

    BTN_SPRITE_TILES_LOG_BANKS();
}

void set_tiles_ref(item_type::list_iterator iterator, const span<const tile>& tiles_ref)
{
    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - SET_TILES_REF: ", iterator->start_tile, " - ",
                         tiles_ref.data(), " - ", tiles_ref.size());

    if(iterator->bank() == item_type::bank_type::HIGH)
    {
        data.high_bank.set_tiles_ref(iterator, tiles_ref);
    }
    else
    {
        data.low_bank.set_tiles_ref(iterator, tiles_ref);
    }

    BTN_SPRITE_TILES_LOG_BANKS();
}

void reload_tiles_ref(item_type::list_iterator iterator)
{
    BTN_SPRITE_TILES_LOG("sprite_tiles_manager - RELOAD_TILES_REF: ", iterator->start_tile);

    if(iterator->bank() == item_type::bank_type::HIGH)
    {
        data.high_bank.reload_tiles_ref(iterator);
    }
    else
    {
        data.low_bank.reload_tiles_ref(iterator);
    }

    BTN_SPRITE_TILES_LOG_BANKS();
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
