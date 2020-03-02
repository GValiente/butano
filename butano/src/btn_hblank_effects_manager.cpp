#include "btn_hblank_effects_manager.h"

#include "btn_span.h"
#include "btn_vector.h"
#include "btn_display.h"
#include "btn_bgs_manager.h"
#include "btn_regular_bg_attributes.h"
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
        const void* values_ptr;
        int values_count;
        int target_id;
        unsigned usages;
        fixed last_fixed_value;
        regular_bg_attributes last_regular_bg_attributes;
        uint8_t target;
        bool visible;

        [[nodiscard]] bool last_value_updated()
        {
            bool updated = false;

            switch(target_type(target))
            {

            case target_type::REGULAR_BG_HORIZONTAL_POSITION:
                {
                    fixed new_value = bgs_manager::hw_position(target_id).x();
                    updated = last_fixed_value != new_value;
                    last_fixed_value = new_value;
                }
                break;

            case target_type::REGULAR_BG_VERTICAL_POSITION:
                {
                    fixed new_value = bgs_manager::hw_position(target_id).y();
                    updated = last_fixed_value != new_value;
                    last_fixed_value = new_value;
                }
                break;

            case target_type::REGULAR_BG_ATTRIBUTES:
                {
                    regular_bg_attributes new_regular_bg_attributes = bgs_manager::attributes(target_id);
                    updated = last_regular_bg_attributes != new_regular_bg_attributes;
                    last_regular_bg_attributes = new_regular_bg_attributes;
                }
                break;
            }

            return updated;
        }

        void setup_entry(hw_entry& entry) const
        {
            switch(target_type(target))
            {

            case target_type::REGULAR_BG_HORIZONTAL_POSITION:
                {
                    auto fixed_values_ptr = reinterpret_cast<const fixed*>(values_ptr);
                    bgs_manager::fill_horizontal_hw_positions(last_fixed_value, fixed_values_ptr[0],
                            display::height(), entry.src[0]);
                    entry.dest = hw::bgs::regular_horizontal_position_register(target_id);
                }
                break;

            case target_type::REGULAR_BG_VERTICAL_POSITION:
                {
                    auto fixed_values_ptr = reinterpret_cast<const fixed*>(values_ptr);
                    bgs_manager::fill_vertical_hw_positions(last_fixed_value, fixed_values_ptr[0],
                            display::height(), entry.src[0]);
                    entry.dest = hw::bgs::regular_vertical_position_register(target_id);
                }
                break;

            case target_type::REGULAR_BG_ATTRIBUTES:
                {
                    auto regular_bg_attributes_ptr = reinterpret_cast<const regular_bg_attributes*>(values_ptr);
                    bgs_manager::fill_hw_attributes(target_id, regular_bg_attributes_ptr[0], display::height(),
                            entry.src[0]);
                    entry.dest = hw::bgs::attributes_register(target_id);
                }
                break;
            }
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

    int _create(const void* values_ptr, int values_count, target_type target, int target_id)
    {
        if(data.free_item_indexes.empty())
        {
            return -1;
        }

        int item_index = data.free_item_indexes.back();
        data.free_item_indexes.pop_back();

        item_type& new_item = data.items[item_index];
        new_item.values_ptr = values_ptr;
        new_item.values_count = values_count;
        new_item.target_id = target_id;
        new_item.usages = 1;
        new_item.target = uint8_t(target);
        new_item.visible = true;
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

int create(const span<const fixed>& fixed_values_ref, target_type target, int target_id)
{
    BTN_ASSERT(fixed_values_ref.size() >= display::height(),
               "Invalid fixed values ref size: ", fixed_values_ref.size(), " - ", display::height());

    return _create(fixed_values_ref.data(), fixed_values_ref.size(), target, target_id);
}

int create(const span<const regular_bg_attributes>& regular_bg_attributes_ref, int target_id)
{
    BTN_ASSERT(regular_bg_attributes_ref.size() >= display::height(),
               "Invalid regular bg attributes ref size: ", regular_bg_attributes_ref.size(), " - ", display::height());

    return _create(regular_bg_attributes_ref.data(), regular_bg_attributes_ref.size(),
                   target_type::REGULAR_BG_ATTRIBUTES, target_id);
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

        if(item.visible)
        {
            item.visible = false;
            data.update = true;
        }
    }
}

span<const fixed> fixed_values_ref(int id)
{
    const item_type& item = data.items[id];
    auto fixed_values_ptr = reinterpret_cast<const fixed*>(item.values_ptr);
    return span<const fixed>(fixed_values_ptr, item.values_count);
}

void set_values_ref(int id, const void* values_ptr, int values_count)
{
    BTN_ASSERT(values_ptr, "Values ptr is null");
    BTN_ASSERT(values_count >= display::height(), "Invalid values count: ", values_count, " - ", display::height());

    item_type& item = data.items[id];
    item.values_ptr = values_ptr;
    item.values_count = values_count;

    if(item.visible)
    {
        data.update = true;
    }
}

void reload_values_ref(int id)
{
    item_type& item = data.items[id];

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
    item.visible = visible;

    if(item.visible)
    {
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
            if(item.last_value_updated())
            {
                update = true;
            }
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
            if(item.visible)
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

void stop()
{
    disable();
}

}
