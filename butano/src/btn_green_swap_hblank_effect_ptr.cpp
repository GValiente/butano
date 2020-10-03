#include "btn_green_swap_hblank_effect_ptr.h"

#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_display_manager.h"
#include "btn_hblank_effect_handler.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_display.h"

namespace btn
{

namespace
{
    class green_swap_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        green_swap_hblank_effect_handler() = default;

        void setup_target(int, iany&) final
        {
        }

        [[nodiscard]] bool target_visible(int) final
        {
            return true;
        }

        [[nodiscard]] bool target_updated(int, iany&) final
        {
            return false;
        }

        [[nodiscard]] uint16_t* output_register(int) final
        {
            return hw::display::green_swap_register();
        }

        void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr) final
        {
            auto states_ptr = reinterpret_cast<const bool*>(input_values_ptr);
            display_manager::fill_green_swap_hblank_effect_states(states_ptr, output_values_ptr);
        }

        void cleanup(int) final
        {
            display_manager::reload_green_swap();
        }
    };


    class static_data
    {

    public:
        green_swap_hblank_effect_handler handler;
    };

    BTN_DATA_EWRAM static_data data;
}

green_swap_hblank_effect_ptr green_swap_hblank_effect_ptr::create(const span<const bool>& states_ref)
{
    int id = hblank_effects_manager::create(states_ref.data(), states_ref.size(), 0, data.handler);
    return green_swap_hblank_effect_ptr(id);
}

optional<green_swap_hblank_effect_ptr> green_swap_hblank_effect_ptr::create_optional(const span<const bool>& states_ref)
{
    int id = hblank_effects_manager::create_optional(states_ref.data(), states_ref.size(), 0, data.handler);
    optional<green_swap_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = green_swap_hblank_effect_ptr(id);
    }

    return result;
}

span<const bool> green_swap_hblank_effect_ptr::states_ref() const
{
    auto values_ptr = reinterpret_cast<const bool*>(hblank_effects_manager::values_ref(id()));
    return span<const bool>(values_ptr, display::height());
}

void green_swap_hblank_effect_ptr::set_states_ref(const span<const bool>& states_ref)
{
    hblank_effects_manager::set_values_ref(id(), states_ref.data(), states_ref.size());
}

void green_swap_hblank_effect_ptr::reload_states_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void green_swap_hblank_effect_ptr::swap(green_swap_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
}

green_swap_hblank_effect_ptr::green_swap_hblank_effect_ptr(int id) :
    hblank_effect_ptr(id)
{
}

}
