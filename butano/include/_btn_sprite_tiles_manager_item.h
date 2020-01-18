#ifndef BTN_SPRITE_TILES_MANAGER_ITEM_H
#define BTN_SPRITE_TILES_MANAGER_ITEM_H

#include "btn_tile.h"
#include "btn_forward_list.h"

namespace _btn
{

class sprite_tiles_manager_item
{

public:
    enum class status_type
    {
        FREE,
        USED,
        TO_REMOVE
    };

    enum class bank_type
    {
        HIGH,
        LOW
    };

    using list_iterator = btn::iforward_list<sprite_tiles_manager_item>::iterator;

    const btn::tile* data = nullptr;
    unsigned usages = 0;
    uint16_t start_tile = 0;
    uint16_t tiles_count = 0;
    bool commit = false;

private:
    uint8_t _status = uint8_t(status_type::FREE);
    uint8_t _bank = uint8_t(bank_type::LOW);

public:
    [[nodiscard]] status_type status() const
    {
        return static_cast<status_type>(_status);
    }

    void set_status(status_type status)
    {
        _status = uint8_t(status);
    }

    [[nodiscard]] bank_type bank() const
    {
        return static_cast<bank_type>(_bank);
    }

    void set_bank(bank_type bank)
    {
        _bank = uint8_t(bank);
    }
};

}

#endif
