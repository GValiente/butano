#include "btn_bg_palette_hblank_effects.h"

#include "btn_span.h"
#include "btn_color.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_hblank_effect_handler.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_palettes.h"

namespace btn
{

namespace
{
    class color_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        color_hblank_effect_handler() = default;

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

        [[nodiscard]] uint16_t* output_register(int target_id) final
        {
            return hw::palettes::bg_color_register(target_id);
        }

        void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr) final
        {
            memory::copy(*static_cast<const uint16_t*>(input_values_ptr), display::height(), *output_values_ptr);
        }
    };


    class static_data
    {

    public:
        color_hblank_effect_handler color_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

bg_palette_color_hblank_effect_ptr bg_palette_color_hblank_effect_ptr::create(
        bg_palette_ptr palette, int color_index, const span<const color>& colors_ref)
{
    BTN_ASSERT(color_index >= 0 && color_index < palette.colors_count(),
               "Invalid color index: ", color_index, " - ", palette.colors_count());

    int final_color_index = (palette.id() * 16) + color_index;
    int id = hblank_effects_manager::create(colors_ref.data(), colors_ref.size(), final_color_index,
                                            data.color_handler);
    return bg_palette_color_hblank_effect_ptr(id, color_index, move(palette));
}

optional<bg_palette_color_hblank_effect_ptr> bg_palette_color_hblank_effect_ptr::optional_create(
        bg_palette_ptr palette, int color_index, const span<const color>& colors_ref)
{
    BTN_ASSERT(color_index >= 0 && color_index < palette.colors_count(),
               "Invalid color index: ", color_index, " - ", palette.colors_count());

    int final_color_index = (palette.id() * 16) + color_index;
    int id = hblank_effects_manager::optional_create(colors_ref.data(), colors_ref.size(), final_color_index,
                                                     data.color_handler);
    optional<bg_palette_color_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = bg_palette_color_hblank_effect_ptr(id, color_index, move(palette));
    }

    return result;
}

bg_palette_color_hblank_effect_ptr::bg_palette_color_hblank_effect_ptr(
        const bg_palette_color_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _palette(other._palette),
    _color_index(other._color_index)
{
}

bg_palette_color_hblank_effect_ptr& bg_palette_color_hblank_effect_ptr::operator=(
        const bg_palette_color_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _palette = other._palette;
    _color_index = other._color_index;
    return *this;
}

bg_palette_color_hblank_effect_ptr::bg_palette_color_hblank_effect_ptr(
        bg_palette_color_hblank_effect_ptr&& other) noexcept :
    hblank_effect_ptr(move(other)),
    _palette(move(other._palette)),
    _color_index(other._color_index)
{
}

bg_palette_color_hblank_effect_ptr& bg_palette_color_hblank_effect_ptr::operator=(
        bg_palette_color_hblank_effect_ptr&& other) noexcept
{
    hblank_effect_ptr::operator=(move(other));
    _palette = move(other._palette);
    _color_index = other._color_index;
    return *this;
}

span<const color> bg_palette_color_hblank_effect_ptr::colors_ref() const
{
    auto values_ptr = reinterpret_cast<const color*>(hblank_effects_manager::values_ref(id()));
    return span<const color>(values_ptr, display::height());
}

void bg_palette_color_hblank_effect_ptr::set_colors_ref(
        const span<const color>& colors_ref)
{
    hblank_effects_manager::set_values_ref(id(), colors_ref.data(), colors_ref.size());
}

void bg_palette_color_hblank_effect_ptr::reload_colors_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void bg_palette_color_hblank_effect_ptr::swap(bg_palette_color_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
}

bg_palette_color_hblank_effect_ptr::bg_palette_color_hblank_effect_ptr(
        int id, int color_index, bg_palette_ptr&& palette) :
    hblank_effect_ptr(id),
    _palette(move(palette)),
    _color_index(color_index)
{
}

}
