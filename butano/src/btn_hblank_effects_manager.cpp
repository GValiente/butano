#include "btn_hblank_effects_manager.h"

#include "btn_span.h"
#include "btn_vector.h"
#include "btn_memory.h"
#include "btn_display.h"
#include "btn_bgs_manager.h"
#include "btn_config_hblank_effects.h"
#include "../hw/include/btn_hw_bgs.h"
#include "../hw/include/btn_hw_hblank_effects.h"

namespace btn::hblank_effects_manager
{

namespace
{
    constexpr const int max_items = BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS;

    static_assert(max_items > 0 && max_items <= numeric_limits<int8_t>::max());

    using hw_entry = hw::hblank_effects::entry;

    class item_type
    {

    public:
        span<const int16_t> values_ref;
        int target_id;
        unsigned usages;
        int16_t last_relative_value;
        uint8_t target;
        bool update;

        [[nodiscard]] bool relative() const
        {
            switch(target_type(target))
            {

            case target_type::REGULAR_BG_HORIZONTAL_POSITION_ABSOLUTE:
                return false;

            case target_type::REGULAR_BG_HORIZONTAL_POSITION_RELATIVE:
                return true;
            }

            return false;
        }

        [[nodiscard]] int relative_value() const
        {
            switch(target_type(target))
            {

            case target_type::REGULAR_BG_HORIZONTAL_POSITION_ABSOLUTE:
                return 0;

            case target_type::REGULAR_BG_HORIZONTAL_POSITION_RELATIVE:
                return bgs_manager::hw_horizontal_position(target_id);
            }

            return 0;
        }

        [[nodiscard]] uint16_t* hw_register()
        {
            switch(target_type(target))
            {

            case target_type::REGULAR_BG_HORIZONTAL_POSITION_ABSOLUTE:
            case target_type::REGULAR_BG_HORIZONTAL_POSITION_RELATIVE:
                return hw::bgs::regular_bg_horizontal_position_register(target_id);
            }

            return nullptr;
        }
    };

    class static_data
    {

    public:
        item_type items[max_items];
        vector<int8_t, max_items> free_item_indexes;
        hw_entry entries_a[max_items];
        hw_entry entries_b[max_items];
        int8_t entries_count = 0;
        bool entries_a_active = false;
        bool enabled = false;
    };

    BTN_DATA_EWRAM static_data data;
}

void init()
{
    hw::hblank_effects::init();

    for(int index = max_items - 1; index >= 0; --index)
    {
        data.free_item_indexes.push_back(int8_t(index));
    }
}

int count()
{
    return max_items;
}

int used_count()
{
    return data.free_item_indexes.available();
}

int available_count()
{
    return data.free_item_indexes.size();
}

void enable()
{
    if(data.enabled)
    {
        hw::hblank_effects::enable();
    }
}

void disable()
{
    if(data.enabled)
    {
        hw::hblank_effects::disable();
    }
}

int create(const span<const int16_t>& values_ref, target_type target, int target_id)
{
    BTN_ASSERT(values_ref.size() >= display::height(), "Invalid values ref size: ", values_ref.size(), " - ",
               display::height());

    if(data.free_item_indexes.empty())
    {
        return -1;
    }

    int item_index = data.free_item_indexes.back();
    data.free_item_indexes.pop_back();

    item_type& new_item = data.items[item_index];
    new_item.values_ref = values_ref;
    new_item.target_id = target_id;
    new_item.usages = 1;
    new_item.last_relative_value = 0;
    new_item.target = uint8_t(target);
    new_item.update = true;
    return item_index;
}

void increase_usages(int id)
{
    item_type& item = data.items[id];
    ++item.usages;
}

void decrease_usages(int id)
{
    item_type& item = data.items[id];
    --item.usages;

    if(! item.usages)
    {
        data.free_item_indexes.push_back(int8_t(id));
    }
}

const span<const int16_t>& values_ref(int id)
{
    const item_type& item = data.items[id];
    return item.values_ref;
}

void set_values_ref(int id, const span<const int16_t>& values_ref)
{
    BTN_ASSERT(values_ref.size() >= display::height(), "Invalid values ref size: ", values_ref.size(), " - ",
               display::height());

    item_type& item = data.items[id];
    item.values_ref = values_ref;
    item.update = true;
}

void reload_values_ref(int id)
{
    item_type& item = data.items[id];
    item.update = true;
}

void update()
{
    hw_entry* entries = nullptr;
    int display_height = display::height();
    int entries_count = 0;

    for(item_type& item : data.items)
    {
        if(item.usages)
        {
            int new_relative_value = 0;
            bool relative = item.relative();
            bool update = item.update;
            item.update = false;

            if(relative)
            {
                new_relative_value = item.relative_value();

                if(new_relative_value != item.last_relative_value)
                {
                    item.last_relative_value = int16_t(new_relative_value);
                    update = true;
                }
            }

            if(update)
            {
                if(! entries)
                {
                    if(data.entries_a_active)
                    {
                        entries = data.entries_b;
                        data.entries_a_active = false;
                    }
                    else
                    {
                        entries = data.entries_a;
                        data.entries_a_active = true;
                    }
                }

                hw_entry& entry = entries[entries_count];
                entry.dest = item.hw_register();
                ++entries_count;

                if(relative)
                {
                    const int16_t* item_values = item.values_ref.data();
                    int16_t* entry_src = entry.src;

                    for(int index = 0; index < display_height; ++index)
                    {
                        entry_src[index] = int16_t(item_values[index] + new_relative_value);
                    }
                }
                else
                {
                    memory::copy(item.values_ref[0], display_height, entry.src[0]);
                }
            }
        }
    }

    data.entries_count = int8_t(entries_count);
}

void commit()
{
    if(data.entries_count)
    {
        if(data.entries_a_active)
        {
            hw::hblank_effects::commit(data.entries_a, data.entries_count);
        }
        else
        {
            hw::hblank_effects::commit(data.entries_b, data.entries_count);
        }

        data.entries_count = 0;
    }

    if(used_count())
    {
        if(! data.enabled)
        {
            data.enabled = true;
            hw::hblank_effects::enable();
        }
    }
    else
    {
        if(data.enabled)
        {
            data.enabled = false;
            hw::hblank_effects::disable();
        }
    }
}

void stop()
{
    disable();
}

}
