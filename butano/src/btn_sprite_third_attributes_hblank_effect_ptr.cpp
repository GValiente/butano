#include "btn_sprite_third_attributes_hblank_effect_ptr.h"

#include "btn_any.h"
#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_sprite_third_attributes.h"
#include "btn_sprites_manager.h"
#include "btn_hblank_effect_handler.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_sprites.h"

namespace btn
{

namespace
{
    class third_attributes_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        third_attributes_hblank_effect_handler() = default;

        void setup_target(int, iany& target_last_value) final
        {
            target_last_value = sprite_shape_size(sprite_shape::SQUARE, sprite_size::SMALL);
        }

        [[nodiscard]] bool target_visible(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            return sprites_manager::hw_id(handle).has_value();
        }

        [[nodiscard]] bool target_updated(int target_id, iany& target_last_value) final
        {
            sprite_shape_size& last_value = target_last_value.value<sprite_shape_size>();
            auto handle = reinterpret_cast<void*>(target_id);
            sprite_shape_size new_value = sprites_manager::shape_size(handle);
            bool updated = last_value != new_value;
            last_value = new_value;
            return updated;
        }

        [[nodiscard]] uint16_t* output_register(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            return hw::sprites::third_attributes_register(*sprites_manager::hw_id(handle));
        }

        void write_output_values(int, const iany& target_last_value, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            const sprite_shape_size& shape_size = target_last_value.value<sprite_shape_size>();
            auto sprite_third_attributes_ptr = reinterpret_cast<const sprite_third_attributes*>(input_values_ptr);
            sprites_manager::fill_hblank_effect_third_attributes(
                        shape_size, sprite_third_attributes_ptr, output_values_ptr);
        }
    };


    class static_data
    {

    public:
        third_attributes_hblank_effect_handler third_attributes_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

sprite_third_attributes_hblank_effect_ptr sprite_third_attributes_hblank_effect_ptr::create(
        sprite_ptr sprite, const span<const sprite_third_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(),
                                            int(sprite.handle()), data.third_attributes_handler);
    return sprite_third_attributes_hblank_effect_ptr(id, move(sprite));
}

optional<sprite_third_attributes_hblank_effect_ptr> sprite_third_attributes_hblank_effect_ptr::create_optional(
        sprite_ptr sprite, const span<const sprite_third_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(),
                                                     int(sprite.handle()), data.third_attributes_handler);
    optional<sprite_third_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_third_attributes_hblank_effect_ptr(id, move(sprite));
    }

    return result;
}

sprite_third_attributes_hblank_effect_ptr::sprite_third_attributes_hblank_effect_ptr(
        sprite_third_attributes_hblank_effect_ptr&& other) noexcept :
    hblank_effect_ptr(move(other)),
    _sprite(move(other._sprite))
{
}

sprite_third_attributes_hblank_effect_ptr& sprite_third_attributes_hblank_effect_ptr::operator=(
        sprite_third_attributes_hblank_effect_ptr&& other) noexcept
{
    _sprite = move(other._sprite);
    hblank_effect_ptr::operator=(move(other));
    return *this;
}

span<const sprite_third_attributes> sprite_third_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const sprite_third_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_third_attributes>(values_ptr, display::height());
}

void sprite_third_attributes_hblank_effect_ptr::set_attributes_ref(
        const span<const sprite_third_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void sprite_third_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_third_attributes_hblank_effect_ptr::swap(sprite_third_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_sprite, other._sprite);
}

sprite_third_attributes_hblank_effect_ptr::sprite_third_attributes_hblank_effect_ptr(int id, sprite_ptr&& sprite) :
    hblank_effect_ptr(id),
    _sprite(move(sprite))
{
}

}
