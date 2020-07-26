#include "btn_hblank_effects_manager.h"

#include "btn_any.h"
#include "btn_vector.h"
#include "btn_display.h"
#include "btn_alignment.h"
#include "btn_hblank_effect_handler.h"
#include "btn_config_hblank_effects.h"
#include "../hw/include/btn_hw_hblank_effects.h"

namespace btn::hblank_effects_manager
{

namespace
{
    constexpr const int max_items = BTN_CFG_HBLANK_EFFECTS_MAX_ITEMS;

    static_assert(max_items > 0 && max_items <= 8);

    using last_value_type = any<8 * sizeof(int)>;
    using hw_entry = hw::hblank_effects::entry;

    class item_type
    {

    public:
        hblank_effect_handler* handler = nullptr;
        const void* values_ptr = nullptr;
        int target_id = 0;
        last_value_type target_last_value;
        unsigned usages = 0;
        uint16_t* output_register = nullptr;
        alignas(alignof(int)) uint16_t output_values_a[display::height()] = {};
        alignas(alignof(int)) uint16_t output_values_b[display::height()] = {};
        bool visible: 1 = false;
        bool update: 1 = false;
        bool on_screen: 1 = false;
        bool output_values_a_active: 1 = false;
        bool output_values_written: 1 = false;

        [[nodiscard]] bool check_update()
        {
            bool updated = update;
            update = false;

            bool old_on_screen = on_screen;
            bool new_on_screen = handler->target_visible(target_id);
            on_screen = new_on_screen;

            if(new_on_screen)
            {
                updated |= handler->target_updated(target_id, target_last_value);

                if(! output_values_written)
                {
                    updated = true;
                    output_values_written = true;
                }

                if(updated)
                {
                    uint16_t* output_values_ptr;

                    if(output_values_a_active)
                    {
                        output_values_ptr = output_values_b;
                        output_values_a_active = false;
                    }
                    else
                    {
                        output_values_ptr = output_values_a;
                        output_values_a_active = true;
                    }

                    handler->write_output_values(target_id, target_last_value, values_ptr, output_values_ptr);
                }

                uint16_t* old_output_register = output_register;
                output_register = handler->output_register(target_id);
                updated |= old_output_register != output_register;
            }

            updated |= old_on_screen != new_on_screen;
            return updated;
        }

        void setup_entry(hw_entry& entry) const
        {
            entry.src = output_values_a_active ? output_values_a : output_values_b;
            entry.dest = output_register;
        }
    };

    class static_external_data
    {

    public:
        item_type items[max_items];
        vector<int8_t, max_items> free_item_indexes;
        int8_t old_entries_count = 0;
        int8_t new_entries_count = 0;
        bool entries_a_active = false;
        bool update = false;
        bool commit = false;
        bool enabled = false;
    };

    class static_internal_data
    {

    public:
        hw_entry entries_a[max_items];
        hw_entry entries_b[max_items];
    };

    BTN_DATA_EWRAM static_external_data external_data;
    static_internal_data internal_data;

    int _create(const void* values_ptr, int target_id, hblank_effect_handler& handler)
    {
        int item_index = external_data.free_item_indexes.back();
        external_data.free_item_indexes.pop_back();

        item_type& new_item = external_data.items[item_index];
        new_item.handler = &handler;
        new_item.values_ptr = values_ptr;
        new_item.target_id = target_id;
        new_item.usages = 1;
        new_item.output_register = nullptr;
        new_item.visible = true;
        new_item.update = true;
        new_item.on_screen = false;
        new_item.output_values_written = false;
        handler.setup_target(target_id, new_item.target_last_value);
        external_data.update = true;
        return item_index;
    }
}

void init()
{
    hw::hblank_effects::init();

    for(int index = max_items - 1; index >= 0; --index)
    {
        external_data.free_item_indexes.push_back(int8_t(index));
    }
}

int used_count()
{
    return external_data.free_item_indexes.available();
}

int available_count()
{
    return external_data.free_item_indexes.size();
}

void enable()
{
    if(external_data.enabled)
    {
        hw::hblank_effects::enable();
    }
}

void disable()
{
    if(external_data.enabled)
    {
        hw::hblank_effects::disable();
    }
}

int create(const void* values_ptr, [[maybe_unused]] int values_count, int target_id,
           hblank_effect_handler& handler)
{
    BTN_ASSERT(values_ptr, "Values ptr is null");
    BTN_ASSERT(aligned<alignof(int)>(values_ptr), "Values are not aligned");
    BTN_ASSERT(values_count == display::height(), "Invalid values count: ", values_count, " - ", display::height());
    BTN_ASSERT(! external_data.free_item_indexes.empty(), "No more available HBlank effects");

    return _create(values_ptr, target_id, handler);
}

int create_optional(const void* values_ptr, [[maybe_unused]] int values_count, int target_id,
                    hblank_effect_handler& handler)
{
    BTN_ASSERT(values_ptr, "Values ptr is null");
    BTN_ASSERT(aligned<alignof(int)>(values_ptr), "Values are not aligned");
    BTN_ASSERT(values_count == display::height(), "Invalid values count: ", values_count, " - ", display::height());

    if(external_data.free_item_indexes.empty())
    {
        return -1;
    }

    return _create(values_ptr, target_id, handler);
}

void increase_usages(int id)
{
    item_type& item = external_data.items[id];
    ++item.usages;
}

void decrease_usages(int id)
{
    item_type& item = external_data.items[id];
    --item.usages;

    if(! item.usages)
    {
        external_data.free_item_indexes.push_back(int8_t(id));
        item.target_last_value.reset();
        item.update = false;

        if(item.visible)
        {
            item.visible = false;
            external_data.update = true;
        }
    }
}

const void* values_ref(int id)
{
    const item_type& item = external_data.items[id];
    return item.values_ptr;
}

void set_values_ref(int id, const void* values_ptr, [[maybe_unused]] int values_count)
{
    BTN_ASSERT(values_ptr, "Values ptr is null");
    BTN_ASSERT(aligned<alignof(int)>(values_ptr), "Values are not aligned");
    BTN_ASSERT(values_count == display::height(), "Invalid values count: ", values_count, " - ", display::height());

    item_type& item = external_data.items[id];
    item.values_ptr = values_ptr;
    item.update = true;

    if(item.visible)
    {
        external_data.update = true;
    }
}

void reload_values_ref(int id)
{
    item_type& item = external_data.items[id];
    item.update = true;

    if(item.visible)
    {
        external_data.update = true;
    }
}

[[nodiscard]] bool visible(int id)
{
    const item_type& item = external_data.items[id];
    return item.visible;
}

void set_visible(int id, bool visible)
{
    item_type& item = external_data.items[id];

    if(visible != item.visible)
    {
        item.visible = visible;
        external_data.update = true;
    }
}

void update()
{
    bool update = external_data.update;
    external_data.update = false;

    for(item_type& item : external_data.items)
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

        if(external_data.entries_a_active)
        {
            entries = internal_data.entries_b;
            external_data.entries_a_active = false;
        }
        else
        {
            entries = internal_data.entries_a;
            external_data.entries_a_active = true;
        }

        for(const item_type& item : external_data.items)
        {
            if(item.visible && item.on_screen)
            {
                hw_entry& entry = entries[entries_count];
                item.setup_entry(entry);
                ++entries_count;
            }
        }

        external_data.new_entries_count = int8_t(entries_count);
        external_data.commit = true;
    }
}

void commit()
{
    if(external_data.commit)
    {
        int old_entries_count = external_data.old_entries_count;
        int new_entries_count = external_data.new_entries_count;
        external_data.old_entries_count = external_data.new_entries_count;
        external_data.commit = false;

        if(new_entries_count)
        {
            hw_entry* entries_ptr = external_data.entries_a_active ? internal_data.entries_a : internal_data.entries_b;
            hw::hblank_effects::commit_entries_ptr(entries_ptr);

            if(old_entries_count != new_entries_count)
            {
                hw::hblank_effects::commit_entries_count(new_entries_count);
                external_data.enabled = true;
            }
            else if(! external_data.enabled)
            {
                external_data.enabled = true;
                hw::hblank_effects::enable();
            }
        }
        else
        {
            if(external_data.enabled)
            {
                external_data.enabled = false;
                hw::hblank_effects::disable();
            }
        }
    }
}

}
