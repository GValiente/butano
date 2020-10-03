#include "btn_sprite_position_hblank_effect_ptr.h"

#include "btn_any.h"
#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_sprites_manager.h"
#include "btn_hblank_effect_handler.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_sprites.h"

namespace btn
{

namespace
{
    class horizontal_position_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        horizontal_position_hblank_effect_handler() = default;

        void setup_target(int, iany& target_last_value) final
        {
            target_last_value = fixed();
        }

        [[nodiscard]] bool target_visible(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            return sprites_manager::hw_id(handle).has_value();
        }

        [[nodiscard]] bool target_updated(int target_id, iany& target_last_value) final
        {
            fixed& last_value = target_last_value.value<fixed>();
            auto handle = reinterpret_cast<void*>(target_id);
            fixed new_value = sprites_manager::hw_position(handle).x();
            bool updated = last_value != new_value;
            last_value = new_value;
            return updated;
        }

        [[nodiscard]] uint16_t* output_register(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            return hw::sprites::second_attributes_register(*sprites_manager::hw_id(handle));
        }

        void write_output_values(int target_id, const iany& target_last_value, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            fixed last_value = target_last_value.value<fixed>();
            auto fixed_values_ptr = reinterpret_cast<const fixed*>(input_values_ptr);
            sprites_manager::fill_hblank_effect_horizontal_positions(handle, last_value, fixed_values_ptr,
                                                                     output_values_ptr);
        }

        void cleanup(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            sprites_manager::reload(handle);
        }
    };


    class vertical_position_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        vertical_position_hblank_effect_handler() = default;

        void setup_target(int, iany& target_last_value) final
        {
            target_last_value = fixed();
        }

        [[nodiscard]] bool target_visible(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            return sprites_manager::hw_id(handle).has_value();
        }

        [[nodiscard]] bool target_updated(int target_id, iany& target_last_value) final
        {
            fixed& last_value = target_last_value.value<fixed>();
            auto handle = reinterpret_cast<void*>(target_id);
            fixed new_value = sprites_manager::hw_position(handle).y();
            bool updated = last_value != new_value;
            last_value = new_value;
            return updated;
        }

        [[nodiscard]] uint16_t* output_register(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            return hw::sprites::first_attributes_register(*sprites_manager::hw_id(handle));
        }

        void write_output_values(int target_id, const iany& target_last_value, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            fixed last_value = target_last_value.value<fixed>();
            auto fixed_values_ptr = reinterpret_cast<const fixed*>(input_values_ptr);
            sprites_manager::fill_hblank_effect_vertical_positions(handle, last_value, fixed_values_ptr,
                                                                   output_values_ptr);
        }

        void cleanup(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            sprites_manager::reload(handle);
        }
    };


    class static_data
    {

    public:
        horizontal_position_hblank_effect_handler horizontal_position_handler;
        vertical_position_hblank_effect_handler vertical_position_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

sprite_position_hblank_effect_ptr sprite_position_hblank_effect_ptr::create_horizontal(
        sprite_ptr sprite, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), int(sprite.handle()),
                                            data.horizontal_position_handler);
    return sprite_position_hblank_effect_ptr(id, move(sprite));
}

optional<sprite_position_hblank_effect_ptr> sprite_position_hblank_effect_ptr::create_horizontal_optional(
        sprite_ptr sprite, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(deltas_ref.data(), deltas_ref.size(), int(sprite.handle()),
                                                     data.horizontal_position_handler);
    optional<sprite_position_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_position_hblank_effect_ptr(id, move(sprite));
    }

    return result;
}

sprite_position_hblank_effect_ptr sprite_position_hblank_effect_ptr::create_vertical(
        sprite_ptr sprite, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), int(sprite.handle()),
                                            data.vertical_position_handler);
    return sprite_position_hblank_effect_ptr(id, move(sprite));
}

optional<sprite_position_hblank_effect_ptr> sprite_position_hblank_effect_ptr::create_vertical_optional(
        sprite_ptr sprite, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(deltas_ref.data(), deltas_ref.size(), int(sprite.handle()),
                                                     data.vertical_position_handler);
    optional<sprite_position_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_position_hblank_effect_ptr(id, move(sprite));
    }

    return result;
}

sprite_position_hblank_effect_ptr::sprite_position_hblank_effect_ptr(
        sprite_position_hblank_effect_ptr&& other) noexcept :
    hblank_effect_ptr(move(other)),
    _sprite(move(other._sprite))
{
}

sprite_position_hblank_effect_ptr& sprite_position_hblank_effect_ptr::operator=(
        sprite_position_hblank_effect_ptr&& other) noexcept
{
    _sprite = move(other._sprite);
    hblank_effect_ptr::operator=(move(other));
    return *this;
}

span<const fixed> sprite_position_hblank_effect_ptr::deltas_ref() const
{
    auto values_ptr = reinterpret_cast<const fixed*>(hblank_effects_manager::values_ref(id()));
    return span<const fixed>(values_ptr, display::height());
}

void sprite_position_hblank_effect_ptr::set_deltas_ref(const span<const fixed>& deltas_ref)
{
    hblank_effects_manager::set_values_ref(id(), deltas_ref.data(), deltas_ref.size());
}

void sprite_position_hblank_effect_ptr::reload_deltas_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_position_hblank_effect_ptr::swap(sprite_position_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_sprite, other._sprite);
}

sprite_position_hblank_effect_ptr::sprite_position_hblank_effect_ptr(int id, sprite_ptr&& sprite) :
    hblank_effect_ptr(id),
    _sprite(move(sprite))
{
}

}
