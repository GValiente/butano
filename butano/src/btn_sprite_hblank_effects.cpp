#include "btn_sprite_hblank_effects.h"

#include "btn_any.h"
#include "btn_span.h"
#include "btn_optional.h"
#include "btn_third_sprite_attributes.h"
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

        void setup_target(int target_id, iany& target_last_value) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            target_last_value = sprites_manager::shape_size(handle);
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
            optional<int> hw_id = sprites_manager::hw_id(handle);
            BTN_ASSERT(hw_id, "Sprite HW ID retrieve failed");

            return hw::sprites::third_attributes_register(*hw_id);
        }

        void write_output_values(int, const iany& target_last_value, bool first_write, int,
                                 const void* input_values_ptr, uint16_t* output_values_ptr) final
        {
            const sprite_shape_size& last_value = target_last_value.value<sprite_shape_size>();
            auto third_sprite_attributes_ptr = reinterpret_cast<const third_sprite_attributes*>(input_values_ptr);
            sprites_manager::fill_hblank_effect_third_attributes(
                        last_value, first_write, third_sprite_attributes_ptr, output_values_ptr);
        }
    };


    class static_data
    {

    public:
        third_attributes_hblank_effect_handler third_attributes_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

third_sprite_attributes_hblank_effect_ptr third_sprite_attributes_hblank_effect_ptr::create(
        sprite_ptr sprite_ptr, const span<const third_sprite_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(),
                                            int(sprite_ptr.handle()), data.third_attributes_handler);
    return third_sprite_attributes_hblank_effect_ptr(id, move(sprite_ptr));
}

optional<third_sprite_attributes_hblank_effect_ptr> third_sprite_attributes_hblank_effect_ptr::optional_create(
        sprite_ptr sprite_ptr, const span<const third_sprite_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::optional_create(attributes_ref.data(), attributes_ref.size(),
                                                     int(sprite_ptr.handle()), data.third_attributes_handler);
    optional<third_sprite_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = third_sprite_attributes_hblank_effect_ptr(id, move(sprite_ptr));
    }

    return result;
}

third_sprite_attributes_hblank_effect_ptr::third_sprite_attributes_hblank_effect_ptr(
        const third_sprite_attributes_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _sprite_ptr(other._sprite_ptr)
{
}

third_sprite_attributes_hblank_effect_ptr& third_sprite_attributes_hblank_effect_ptr::operator=(
        const third_sprite_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _sprite_ptr = other._sprite_ptr;
    return *this;
}

third_sprite_attributes_hblank_effect_ptr::third_sprite_attributes_hblank_effect_ptr(
        third_sprite_attributes_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _sprite_ptr(move(other._sprite_ptr))
{
}

third_sprite_attributes_hblank_effect_ptr& third_sprite_attributes_hblank_effect_ptr::operator=(
        third_sprite_attributes_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _sprite_ptr = move(other._sprite_ptr);
    return *this;
}

span<const third_sprite_attributes> third_sprite_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const third_sprite_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const third_sprite_attributes>(values_ptr, hblank_effects_manager::values_count(id()));
}

void third_sprite_attributes_hblank_effect_ptr::set_attributes_ref(
        const span<const third_sprite_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void third_sprite_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void third_sprite_attributes_hblank_effect_ptr::swap(third_sprite_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_sprite_ptr, other._sprite_ptr);
}

third_sprite_attributes_hblank_effect_ptr::third_sprite_attributes_hblank_effect_ptr(int id, sprite_ptr&& sprite_ptr) :
    hblank_effect_ptr(id),
    _sprite_ptr(move(sprite_ptr))
{
}

}
