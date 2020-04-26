#include "btn_hblank_effects_manager.h"

#include "btn_any.h"
#include "btn_vector.h"
#include "btn_display.h"
#include "btn_hblank_effect_handler.h"
#include "btn_config_hblank_effects.h"
#include "../hw/include/btn_hw_hblank_effects.h"

namespace btn::hblank_effects_manager
{

namespace
{
    constexpr const int max_items = BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS;

    static_assert(max_items > 0 && max_items <= numeric_limits<int8_t>::max());

    using last_value_type = any<8 * sizeof(int)>;
    using hw_entry = hw::hblank_effects::entry;

    class item_type
    {

    public:
        hblank_effect_handler* handler = nullptr;
        const void* values_ptr = nullptr;
        int values_count = 0;
        int target_id = 0;
        last_value_type target_last_value;
        unsigned usages = 0;
        uint16_t dest_values_a[display::height()];
        uint16_t dest_values_b[display::height()];
        unsigned visible: 1;
        unsigned update: 1;
        unsigned on_screen: 1;
        unsigned dest_values_a_active: 1;
        unsigned output_values_written: 1;

        item_type() :
            visible(false),
            update(false),
            on_screen(false),
            dest_values_a_active(false),
            output_values_written(false)
        {
        }

        [[nodiscard]] bool check_update()
        {
            bool updated = update;
            update = false;

            bool old_on_screen = on_screen;
            on_screen = handler->target_visible(target_id);

            if(on_screen)
            {
                updated |= handler->target_updated(target_id, target_last_value);

                if(! output_values_written)
                {
                    updated = true;
                    output_values_written = true;
                }

                if(updated)
                {
                    uint16_t* dest_values_ptr;

                    if(dest_values_a_active)
                    {
                        dest_values_ptr = dest_values_b;
                        dest_values_a_active = false;
                    }
                    else
                    {
                        dest_values_ptr = dest_values_a;
                        dest_values_a_active = true;
                    }

                    handler->write_output_values(target_id, target_last_value, values_count, values_ptr, dest_values_ptr);
                }
            }

            updated |= old_on_screen != on_screen;
            return updated;
        }

        void setup_entry(hw_entry& entry) const
        {
            entry.src = dest_values_a_active ? dest_values_a : dest_values_b;
            entry.dest = handler->output_register(target_id);
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
        bool update = false;
        bool commit = false;
        bool enabled = false;
    };

    BTN_DATA_EWRAM static_data data;

    int _create(const void* values_ptr, int values_count, int target_id, hblank_effect_handler& handler)
    {
        int item_index = data.free_item_indexes.back();
        data.free_item_indexes.pop_back();

        item_type& new_item = data.items[item_index];
        new_item.handler = &handler;
        new_item.values_ptr = values_ptr;
        new_item.values_count = values_count;
        new_item.target_id = target_id;
        new_item.usages = 1;
        new_item.visible = true;
        new_item.update = true;
        new_item.on_screen = false;
        new_item.output_values_written = false;
        handler.setup_target(target_id, new_item.target_last_value);
        data.update = true;
        return item_index;
    }
}

void init()
{
    hw::hblank_effects::init();

    for(int index = max_items - 1; index >= 0; --index)
    {
        data.free_item_indexes.push_back(int8_t(index));
    }
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

int create(const void* values_ptr, int values_count, int target_id, hblank_effect_handler& handler)
{
    BTN_ASSERT(values_ptr, "Values ptr is null");
    BTN_ASSERT(values_count >= display::height(), "Invalid values count: ", values_count, " - ", display::height());
    BTN_ASSERT(! data.free_item_indexes.empty(), "No more available HBlank effects");

    return _create(values_ptr, values_count, target_id, handler);
}

int optional_create(const void* values_ptr, int values_count, int target_id, hblank_effect_handler& handler)
{
    BTN_ASSERT(values_ptr, "Values ptr is null");
    BTN_ASSERT(values_count >= display::height(), "Invalid values count: ", values_count, " - ", display::height());

    if(data.free_item_indexes.empty())
    {
        return -1;
    }

    return _create(values_ptr, values_count, target_id, handler);
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
        item.target_last_value.reset();
        item.update = false;

        if(item.visible)
        {
            item.visible = false;
            data.update = true;
        }
    }
}

const void* values_ref(int id)
{
    const item_type& item = data.items[id];
    return item.values_ptr;
}

int values_count(int id)
{
    const item_type& item = data.items[id];
    return item.values_count;
}

void set_values_ref(int id, const void* values_ptr, int values_count)
{
    BTN_ASSERT(values_ptr, "Values ptr is null");
    BTN_ASSERT(values_count >= display::height(), "Invalid values count: ", values_count, " - ", display::height());

    item_type& item = data.items[id];
    item.values_ptr = values_ptr;
    item.values_count = values_count;
    item.update = true;

    if(item.visible)
    {
        data.update = true;
    }
}

void reload_values_ref(int id)
{
    item_type& item = data.items[id];
    item.update = true;

    if(item.visible)
    {
        data.update = true;
    }
}

[[nodiscard]] bool visible(int id)
{
    item_type& item = data.items[id];
    return item.visible;
}

void set_visible(int id, bool visible)
{
    item_type& item = data.items[id];

    if(visible != item.visible)
    {
        item.visible = visible;
        data.update = true;
    }
}

void update()
{
    bool update = data.update;
    data.update = false;

    for(item_type& item : data.items)
    {
        if(item.visible)
        {
            update |= item.check_update();
        }
    }

    if(update)
    {
        hw_entry* entries;
        int entries_count = 0;

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

        for(item_type& item : data.items)
        {
            if(item.visible && item.on_screen)
            {
                hw_entry& entry = entries[entries_count];
                item.setup_entry(entry);
                ++entries_count;
            }
        }

        data.entries_count = int8_t(entries_count);
        data.commit = true;
    }
}

void commit()
{
    if(data.commit)
    {
        data.commit = false;

        if(data.entries_a_active)
        {
            hw::hblank_effects::commit(data.entries_a, data.entries_count);
        }
        else
        {
            hw::hblank_effects::commit(data.entries_b, data.entries_count);
        }

        if(data.entries_count)
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
}

}
