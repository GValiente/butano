#include "btn_bg_palettes_hblank_effects.h"

#include "btn_span.h"
#include "btn_color.h"
#include "btn_optional.h"
#include "btn_hblank_effect_handler.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_palettes.h"

namespace btn
{

namespace
{
    class transparent_color_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        transparent_color_hblank_effect_handler() = default;

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
            return hw::palettes::bg_transparent_color_register();
        }

        void write_output_values(int, const iany&, bool, int values_count, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            auto color_values_ptr = reinterpret_cast<const color*>(input_values_ptr);

            for(int index = 0; index < values_count; ++index)
            {
                output_values_ptr[index] = uint16_t(color_values_ptr[index].value());
            }
        }
    };


    class static_data
    {

    public:
        transparent_color_hblank_effect_handler transparent_color_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

bg_palettes_transparent_color_hblank_effect_ptr bg_palettes_transparent_color_hblank_effect_ptr::create(
        const span<const color>& colors_ref)
{
    int id = hblank_effects_manager::create(colors_ref.data(), colors_ref.size(), 0,
                                            data.transparent_color_handler);
    return bg_palettes_transparent_color_hblank_effect_ptr(id);
}

optional<bg_palettes_transparent_color_hblank_effect_ptr> bg_palettes_transparent_color_hblank_effect_ptr::optional_create(
        const span<const color>& colors_ref)
{
    int id = hblank_effects_manager::optional_create(colors_ref.data(), colors_ref.size(), 0,
                                                     data.transparent_color_handler);
    optional<bg_palettes_transparent_color_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = bg_palettes_transparent_color_hblank_effect_ptr(id);
    }

    return result;
}

bg_palettes_transparent_color_hblank_effect_ptr::bg_palettes_transparent_color_hblank_effect_ptr(
        const bg_palettes_transparent_color_hblank_effect_ptr& other) :
    hblank_effect_ptr(other)
{
}

bg_palettes_transparent_color_hblank_effect_ptr& bg_palettes_transparent_color_hblank_effect_ptr::operator=(
        const bg_palettes_transparent_color_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    return *this;
}

bg_palettes_transparent_color_hblank_effect_ptr::bg_palettes_transparent_color_hblank_effect_ptr(
        bg_palettes_transparent_color_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other))
{
}

bg_palettes_transparent_color_hblank_effect_ptr& bg_palettes_transparent_color_hblank_effect_ptr::operator=(
        bg_palettes_transparent_color_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    return *this;
}

span<const color> bg_palettes_transparent_color_hblank_effect_ptr::colors_ref() const
{
    auto values_ptr = reinterpret_cast<const color*>(hblank_effects_manager::values_ref(id()));
    return span<const color>(values_ptr, hblank_effects_manager::values_count(id()));
}

void bg_palettes_transparent_color_hblank_effect_ptr::set_colors_ref(
        const span<const color>& colors_ref)
{
    hblank_effects_manager::set_values_ref(id(), colors_ref.data(), colors_ref.size());
}

void bg_palettes_transparent_color_hblank_effect_ptr::reload_colors_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void bg_palettes_transparent_color_hblank_effect_ptr::swap(bg_palettes_transparent_color_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
}

bg_palettes_transparent_color_hblank_effect_ptr::bg_palettes_transparent_color_hblank_effect_ptr(int id) :
    hblank_effect_ptr(id)
{
}

}
