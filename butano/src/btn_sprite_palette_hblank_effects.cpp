#include "btn_sprite_palette_hblank_effects.h"

#include "btn_display.h"
#include "btn_palettes_bank.h"
#include "btn_palettes_manager.h"
#include "btn_hblank_effect_handler.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_palettes.h"

namespace btn
{

namespace
{
    struct alignas(alignof(int)) palette_target_id
    {
        union
        {
            struct
            {
                int16_t palette_id;
                int16_t final_color_index;
            } params;
            int target_id;
        };

        palette_target_id(int palette_id, int color_index) :
            params({ int16_t(palette_id), int16_t((palette_id * 16) + color_index) })
        {
        }

        explicit palette_target_id(int _target_id) :
            target_id(_target_id)
        {
        }
    };

    static_assert(sizeof(palette_target_id) == sizeof(int));
    static_assert(alignof(palette_target_id) == alignof(int));


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

        [[nodiscard]] bool target_updated(int target_id, iany&) final
        {
            if(optional<palettes_bank::commit_data> commit_data =
                    palettes_manager::sprite_palettes_bank().retrieve_commit_data())
            {
                palette_target_id palette_target_id(target_id);
                int target_color = palette_target_id.params.final_color_index;
                int first_color = commit_data->offset;
                int last_color = first_color + commit_data->count - 1;
                return target_color >= first_color && target_color <= last_color;
            }

            return false;
        }

        [[nodiscard]] uint16_t* output_register(int target_id) final
        {
            palette_target_id palette_target_id(target_id);
            return hw::palettes::sprite_color_register(palette_target_id.params.final_color_index);
        }

        void write_output_values(int target_id, const iany&, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            palette_target_id palette_target_id(target_id);
            int palette_id = palette_target_id.params.palette_id;
            palettes_manager::sprite_palettes_bank().fill_hblank_effect_colors(
                        palette_id, reinterpret_cast<const color*>(input_values_ptr), output_values_ptr);
        }
    };


    class static_data
    {

    public:
        color_hblank_effect_handler color_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

sprite_palette_color_hblank_effect_ptr sprite_palette_color_hblank_effect_ptr::create(
        sprite_palette_ptr palette, int color_index, const span<const color>& colors_ref)
{
    BTN_ASSERT(color_index >= 0 && color_index < palette.colors_count(),
               "Invalid color index: ", color_index, " - ", palette.colors_count());

    palette_target_id palette_target_id(palette.id(), color_index);
    int id = hblank_effects_manager::create(
                colors_ref.data(), colors_ref.size(), palette_target_id.target_id, data.color_handler);
    return sprite_palette_color_hblank_effect_ptr(id, color_index, move(palette));
}

optional<sprite_palette_color_hblank_effect_ptr> sprite_palette_color_hblank_effect_ptr::create_optional(
        sprite_palette_ptr palette, int color_index, const span<const color>& colors_ref)
{
    BTN_ASSERT(color_index >= 0 && color_index < palette.colors_count(),
               "Invalid color index: ", color_index, " - ", palette.colors_count());

    palette_target_id palette_target_id(palette.id(), color_index);
    int id = hblank_effects_manager::create_optional(
                colors_ref.data(), colors_ref.size(), palette_target_id.target_id, data.color_handler);
    optional<sprite_palette_color_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_palette_color_hblank_effect_ptr(id, color_index, move(palette));
    }

    return result;
}

sprite_palette_color_hblank_effect_ptr::sprite_palette_color_hblank_effect_ptr(
        sprite_palette_color_hblank_effect_ptr&& other) noexcept :
    hblank_effect_ptr(move(other)),
    _palette(move(other._palette)),
    _color_index(other._color_index)
{
}

sprite_palette_color_hblank_effect_ptr& sprite_palette_color_hblank_effect_ptr::operator=(
        sprite_palette_color_hblank_effect_ptr&& other) noexcept
{
    _palette = move(other._palette);
    _color_index = other._color_index;
    hblank_effect_ptr::operator=(move(other));
    return *this;
}

span<const color> sprite_palette_color_hblank_effect_ptr::colors_ref() const
{
    auto values_ptr = reinterpret_cast<const color*>(hblank_effects_manager::values_ref(id()));
    return span<const color>(values_ptr, display::height());
}

void sprite_palette_color_hblank_effect_ptr::set_colors_ref(
        const span<const color>& colors_ref)
{
    hblank_effects_manager::set_values_ref(id(), colors_ref.data(), colors_ref.size());
}

void sprite_palette_color_hblank_effect_ptr::reload_colors_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_palette_color_hblank_effect_ptr::swap(sprite_palette_color_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
}

sprite_palette_color_hblank_effect_ptr::sprite_palette_color_hblank_effect_ptr(
        int id, int color_index, sprite_palette_ptr&& palette) :
    hblank_effect_ptr(id),
    _palette(move(palette)),
    _color_index(color_index)
{
}

}
