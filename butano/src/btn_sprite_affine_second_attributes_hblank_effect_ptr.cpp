#include "btn_sprite_affine_second_attributes_hblank_effect_ptr.h"

#include "btn_any.h"
#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_sprite_affine_second_attributes.h"
#include "btn_sprites_manager.h"
#include "btn_hblank_effect_handler.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_sprites.h"

namespace btn
{

namespace
{
    class alignas(alignof(int)) second_attributes_last_value_type
    {

    public:
        fixed hw_x = -1;
        sprite_size size = sprite_size::SMALL;

        second_attributes_last_value_type() = default;

        explicit second_attributes_last_value_type(void* handle) :
            hw_x(sprites_manager::hw_position(handle).x()),
            size(sprites_manager::size(handle))
        {
        }

        explicit second_attributes_last_value_type(int target_id) :
            second_attributes_last_value_type(reinterpret_cast<void*>(target_id))
        {
        }

        [[nodiscard]] friend bool operator==(const second_attributes_last_value_type& a,
                                             const second_attributes_last_value_type& b) = default;
    };


    class affine_second_attributes_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        affine_second_attributes_hblank_effect_handler() = default;

        void setup_target(int, iany& target_last_value) final
        {
            target_last_value = second_attributes_last_value_type();
        }

        [[nodiscard]] bool target_visible(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            return sprites_manager::hw_id(handle).has_value();
        }

        [[nodiscard]] bool target_updated(int target_id, iany& target_last_value) final
        {
            second_attributes_last_value_type& last_value = target_last_value.value<second_attributes_last_value_type>();
            second_attributes_last_value_type new_value(target_id);
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
            const second_attributes_last_value_type& last_value = target_last_value.value<second_attributes_last_value_type>();
            auto second_attributes_ptr = reinterpret_cast<const sprite_affine_second_attributes*>(input_values_ptr);
            sprites_manager::fill_hblank_effect_affine_second_attributes(
                        handle, last_value.hw_x, last_value.size, second_attributes_ptr, output_values_ptr);
        }
    };


    class static_data
    {

    public:
        affine_second_attributes_hblank_effect_handler affine_second_attributes_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

sprite_affine_second_attributes_hblank_effect_ptr sprite_affine_second_attributes_hblank_effect_ptr::create(
        sprite_ptr sprite, const span<const sprite_affine_second_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(),
                                            int(sprite.handle()), data.affine_second_attributes_handler);
    return sprite_affine_second_attributes_hblank_effect_ptr(id, move(sprite));
}

optional<sprite_affine_second_attributes_hblank_effect_ptr> sprite_affine_second_attributes_hblank_effect_ptr::create_optional(
        sprite_ptr sprite, const span<const sprite_affine_second_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(),
                                                     int(sprite.handle()), data.affine_second_attributes_handler);
    optional<sprite_affine_second_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_second_attributes_hblank_effect_ptr(id, move(sprite));
    }

    return result;
}

sprite_affine_second_attributes_hblank_effect_ptr::sprite_affine_second_attributes_hblank_effect_ptr(
        sprite_affine_second_attributes_hblank_effect_ptr&& other) noexcept :
    hblank_effect_ptr(move(other)),
    _sprite(move(other._sprite))
{
}

sprite_affine_second_attributes_hblank_effect_ptr& sprite_affine_second_attributes_hblank_effect_ptr::operator=(
        sprite_affine_second_attributes_hblank_effect_ptr&& other) noexcept
{
    _sprite = move(other._sprite);
    hblank_effect_ptr::operator=(move(other));
    return *this;
}

span<const sprite_affine_second_attributes> sprite_affine_second_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const sprite_affine_second_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_affine_second_attributes>(values_ptr, display::height());
}

void sprite_affine_second_attributes_hblank_effect_ptr::set_attributes_ref(
        const span<const sprite_affine_second_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void sprite_affine_second_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_second_attributes_hblank_effect_ptr::swap(
        sprite_affine_second_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_sprite, other._sprite);
}

sprite_affine_second_attributes_hblank_effect_ptr::sprite_affine_second_attributes_hblank_effect_ptr(
        int id, sprite_ptr&& sprite) :
    hblank_effect_ptr(id),
    _sprite(move(sprite))
{
}

}
