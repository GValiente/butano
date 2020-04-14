#include "btn_hblank_effects_manager.h"

#include "btn_span.h"
#include "btn_vector.h"
#include "btn_display.h"
#include "btn_variant.h"
#include "btn_bgs_manager.h"
#include "btn_display_manager.h"
#include "btn_regular_bg_attributes.h"
#include "btn_config_hblank_effects.h"
#include "../hw/include/btn_hw_bgs.h"
#include "../hw/include/btn_hw_display.h"
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
        using last_value_type = variant<fixed, regular_bg_attributes, pair<fixed, fixed>>;

        const void* values_ptr = nullptr;
        int values_count = 0;
        int target_id = 0;
        unsigned usages = 0;
        last_value_type last_value_variant;
        uint8_t target = 0;
        bool visible = false;
        bool update = false;
        bool dest_values_a_active = false;
        uint16_t dest_values_a[display::height()];
        uint16_t dest_values_b[display::height()];

        [[nodiscard]] bool check_update()
        {
            bool updated = update;
            update = false;

            switch(target_type(target))
            {

            case target_type::REGULAR_BG_HORIZONTAL_POSITION:
                {
                    fixed& last_value = last_value_variant.get<fixed>();
                    fixed new_value = bgs_manager::hw_position(target_id).x();
                    updated |= last_value.integer() != new_value.integer();
                    last_value = new_value;

                    if(updated)
                    {
                        auto fixed_values_ptr = reinterpret_cast<const fixed*>(values_ptr);
                        bgs_manager::fill_hblank_effect_horizontal_positions(
                                    last_value, fixed_values_ptr, _switch_dest_values());
                    }
                }
                break;

            case target_type::REGULAR_BG_VERTICAL_POSITION:
                {
                    fixed& last_value = last_value_variant.get<fixed>();
                    fixed new_value = bgs_manager::hw_position(target_id).y();
                    updated |= last_value.integer() != new_value.integer();
                    last_value = new_value;

                    if(updated)
                    {
                        auto fixed_values_ptr = reinterpret_cast<const fixed*>(values_ptr);
                        bgs_manager::fill_hblank_effect_vertical_positions(
                                    last_value, fixed_values_ptr, _switch_dest_values());
                    }
                }
                break;

            case target_type::REGULAR_BG_ATTRIBUTES:
                {
                    regular_bg_attributes& last_value = last_value_variant.get<regular_bg_attributes>();
                    regular_bg_attributes new_value = bgs_manager::attributes(target_id);
                    updated |= last_value != new_value;
                    last_value = new_value;

                    if(updated)
                    {
                        auto regular_bg_attributes_ptr = reinterpret_cast<const regular_bg_attributes*>(values_ptr);
                        bgs_manager::fill_hblank_effect_attributes(
                                    target_id, regular_bg_attributes_ptr, _switch_dest_values());
                    }
                }
                break;

            case target_type::RECT_WINDOW_HORIZONTAL_BOUNDARIES:
                {
                    pair<fixed, fixed>& last_value = last_value_variant.get<pair<fixed, fixed>>();
                    pair<fixed, fixed> new_value = display_manager::rect_window_hw_horizontal_boundaries(target_id);
                    updated |= last_value.first.integer() != new_value.first.integer() ||
                            last_value.second.integer() != new_value.second.integer();
                    last_value = new_value;

                    if(updated)
                    {
                        auto fixed_pairs_ptr = reinterpret_cast<const pair<fixed, fixed>*>(values_ptr);
                        display_manager::fill_rect_window_hblank_effect_horizontal_boundaries(
                                    last_value, fixed_pairs_ptr, _switch_dest_values());
                    }
                }
                break;

            case target_type::RECT_WINDOW_VERTICAL_BOUNDARIES:
                {
                    pair<fixed, fixed>& last_value = last_value_variant.get<pair<fixed, fixed>>();
                    pair<fixed, fixed> new_value = display_manager::rect_window_hw_vertical_boundaries(target_id);
                    updated |= last_value.first.integer() != new_value.first.integer() ||
                            last_value.second.integer() != new_value.second.integer();
                    last_value = new_value;

                    if(updated)
                    {
                        auto fixed_pairs_ptr = reinterpret_cast<const pair<fixed, fixed>*>(values_ptr);
                        display_manager::fill_rect_window_hblank_effect_vertical_boundaries(
                                    last_value, fixed_pairs_ptr, _switch_dest_values());
                    }
                }
                break;
            }

            return updated;
        }

        void setup_entry(hw_entry& entry) const
        {
            entry.src = dest_values_a_active ? dest_values_a : dest_values_b;

            switch(target_type(target))
            {

            case target_type::REGULAR_BG_HORIZONTAL_POSITION:
                entry.dest = hw::bgs::regular_horizontal_position_register(target_id);
                break;

            case target_type::REGULAR_BG_VERTICAL_POSITION:
                entry.dest = hw::bgs::regular_vertical_position_register(target_id);
                break;

            case target_type::REGULAR_BG_ATTRIBUTES:
                entry.dest = hw::bgs::attributes_register(target_id);
                break;

            case target_type::RECT_WINDOW_HORIZONTAL_BOUNDARIES:
                entry.dest = hw::display::window_horizontal_boundaries_register(target_id);
                break;

            case target_type::RECT_WINDOW_VERTICAL_BOUNDARIES:
                entry.dest = hw::display::window_vertical_boundaries_register(target_id);
                break;
            }
        }

    private:
        [[nodiscard]] uint16_t* _switch_dest_values()
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

            return dest_values_ptr;
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
        int item_index = data.free_item_indexes.back();
        data.free_item_indexes.pop_back();

        item_type& new_item = data.items[item_index];
        new_item.values_ptr = values_ptr;
        new_item.values_count = values_count;
        new_item.target_id = target_id;
        new_item.usages = 1;
        new_item.target = uint8_t(target);
        new_item.visible = true;
        new_item.update = true;
        data.update = true;

        switch(target)
        {

        case target_type::REGULAR_BG_HORIZONTAL_POSITION:
        case target_type::REGULAR_BG_VERTICAL_POSITION:
            new_item.last_value_variant = fixed();
            break;

        case target_type::REGULAR_BG_ATTRIBUTES:
            new_item.last_value_variant = regular_bg_attributes();
            break;

        case target_type::RECT_WINDOW_HORIZONTAL_BOUNDARIES:
        case target_type::RECT_WINDOW_VERTICAL_BOUNDARIES:
            new_item.last_value_variant = pair<fixed, fixed>();
            break;
        }

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

int create(const span<const fixed>& fixed_values_ref, target_type target, int target_id)
{
    BTN_ASSERT(fixed_values_ref.size() >= display::height(),
               "Invalid fixed values ref size: ", fixed_values_ref.size(), " - ", display::height());
    BTN_ASSERT(! data.free_item_indexes.empty(), "No more available HBlank effects");

    return _create(fixed_values_ref.data(), fixed_values_ref.size(), target, target_id);
}

int optional_create(const span<const fixed>& fixed_values_ref, target_type target, int target_id)
{
    BTN_ASSERT(fixed_values_ref.size() >= display::height(),
               "Invalid fixed values ref size: ", fixed_values_ref.size(), " - ", display::height());

    if(data.free_item_indexes.empty())
    {
        return -1;
    }

    return _create(fixed_values_ref.data(), fixed_values_ref.size(), target, target_id);
}

int create(const span<const regular_bg_attributes>& regular_bg_attributes_ref, int target_id)
{
    BTN_ASSERT(regular_bg_attributes_ref.size() >= display::height(),
               "Invalid regular bg attributes ref size: ", regular_bg_attributes_ref.size(), " - ", display::height());
    BTN_ASSERT(! data.free_item_indexes.empty(), "No more available HBlank effects");

    return _create(regular_bg_attributes_ref.data(), regular_bg_attributes_ref.size(),
                   target_type::REGULAR_BG_ATTRIBUTES, target_id);
}

int optional_create(const span<const regular_bg_attributes>& regular_bg_attributes_ref, int target_id)
{
    BTN_ASSERT(regular_bg_attributes_ref.size() >= display::height(),
               "Invalid regular bg attributes ref size: ", regular_bg_attributes_ref.size(), " - ", display::height());

    if(data.free_item_indexes.empty())
    {
        return -1;
    }

    return _create(regular_bg_attributes_ref.data(), regular_bg_attributes_ref.size(),
                   target_type::REGULAR_BG_ATTRIBUTES, target_id);
}

int create(const span<const pair<fixed, fixed>>& fixed_pairs_ref, target_type target, int target_id)
{
    BTN_ASSERT(fixed_pairs_ref.size() >= display::height(),
               "Invalid fixed pairs ref size: ", fixed_pairs_ref.size(), " - ", display::height());
    BTN_ASSERT(! data.free_item_indexes.empty(), "No more available HBlank effects");

    return _create(fixed_pairs_ref.data(), fixed_pairs_ref.size(), target, target_id);
}

int optional_create(const span<const pair<fixed, fixed>>& fixed_pairs_ref, target_type target, int target_id)
{
    BTN_ASSERT(fixed_pairs_ref.size() >= display::height(),
               "Invalid fixed pairs ref size: ", fixed_pairs_ref.size(), " - ", display::height());

    if(data.free_item_indexes.empty())
    {
        return -1;
    }

    return _create(fixed_pairs_ref.data(), fixed_pairs_ref.size(), target, target_id);
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
        item.update = false;

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

span<const regular_bg_attributes> regular_bg_attributes_ref(int id)
{
    const item_type& item = data.items[id];
    auto regular_bg_attributes_ptr = reinterpret_cast<const regular_bg_attributes*>(item.values_ptr);
    return span<const regular_bg_attributes>(regular_bg_attributes_ptr, item.values_count);
}

span<const pair<fixed, fixed>> fixed_pairs_ref(int id)
{
    const item_type& item = data.items[id];
    auto fixed_pairs_ptr = reinterpret_cast<const pair<fixed, fixed>*>(item.values_ptr);
    return span<const pair<fixed, fixed>>(fixed_pairs_ptr, item.values_count);
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

}
