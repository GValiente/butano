#include "btn_sprite_first_attributes_hblank_effect_ptr.h"

#include "btn_any.h"
#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_sprite_first_attributes.h"
#include "btn_sprites_manager.h"
#include "btn_display_manager.h"
#include "btn_hblank_effect_handler.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_sprites.h"

namespace btn
{

namespace
{
    class first_attributes_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        first_attributes_hblank_effect_handler() = default;

        void setup_target(int, iany& target_last_value) final
        {
            target_last_value = last_value_type();
        }

        [[nodiscard]] bool target_visible(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            return sprites_manager::hw_id(handle).has_value();
        }

        [[nodiscard]] bool target_updated(int target_id, iany& target_last_value) final
        {
            last_value_type& last_value = target_last_value.value<last_value_type>();
            last_value_type new_value(target_id);
            bool updated = last_value != new_value;
            last_value = new_value;
            return updated;
        }

        [[nodiscard]] uint16_t* output_register(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            return hw::sprites::first_attributes_register(*sprites_manager::hw_id(handle));
        }

        void write_output_values(int, const iany& target_last_value, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            const last_value_type& last_value = target_last_value.value<last_value_type>();
            auto sprite_first_attributes_ptr = reinterpret_cast<const sprite_first_attributes*>(input_values_ptr);
            sprites_manager::fill_hblank_effect_first_attributes(
                        last_value.hw_y, btn::sprite_shape(last_value.shape),
                        btn::palette_bpp_mode(last_value.bpp_mode), last_value.affine_mode,
                        sprite_first_attributes_ptr, output_values_ptr);
        }

        void show(int) final
        {
        }

        void cleanup(int target_id) final
        {
            auto handle = reinterpret_cast<void*>(target_id);
            sprites_manager::reload(handle);
        }

    private:
        class alignas(int) last_value_type
        {

        public:
            int hw_y = -1;
            int affine_mode = -1;
            uint8_t shape = 0;
            uint8_t bpp_mode = 0;
            bool fade_enabled = false;

            last_value_type() = default;

            explicit last_value_type(void* handle) :
                hw_y(sprites_manager::hw_position(handle).y()),
                affine_mode(sprites_manager::affine_mode(handle)),
                shape(uint8_t(sprites_manager::shape(handle))),
                bpp_mode(uint8_t(sprites_manager::palette(handle).bpp_mode())),
                fade_enabled(display_manager::blending_fade_enabled())
            {
            }

            explicit last_value_type(int target_id) :
                last_value_type(reinterpret_cast<void*>(target_id))
            {
            }

            [[nodiscard]] friend bool operator==(const last_value_type& a, const last_value_type& b) = default;
        };
    };


    class static_data
    {

    public:
        first_attributes_hblank_effect_handler first_attributes_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

sprite_first_attributes_hblank_effect_ptr sprite_first_attributes_hblank_effect_ptr::create(
        sprite_ptr sprite, const span<const sprite_first_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(),
                                            int(sprite.handle()), data.first_attributes_handler);
    return sprite_first_attributes_hblank_effect_ptr(id, move(sprite));
}

optional<sprite_first_attributes_hblank_effect_ptr> sprite_first_attributes_hblank_effect_ptr::create_optional(
        sprite_ptr sprite, const span<const sprite_first_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(),
                                                     int(sprite.handle()), data.first_attributes_handler);
    optional<sprite_first_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_first_attributes_hblank_effect_ptr(id, move(sprite));
    }

    return result;
}

sprite_first_attributes_hblank_effect_ptr::sprite_first_attributes_hblank_effect_ptr(
        sprite_first_attributes_hblank_effect_ptr&& other) noexcept :
    hblank_effect_ptr(move(other)),
    _sprite(move(other._sprite))
{
}

sprite_first_attributes_hblank_effect_ptr& sprite_first_attributes_hblank_effect_ptr::operator=(
        sprite_first_attributes_hblank_effect_ptr&& other) noexcept
{
    _sprite = move(other._sprite);
    hblank_effect_ptr::operator=(move(other));
    return *this;
}

span<const sprite_first_attributes> sprite_first_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const sprite_first_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_first_attributes>(values_ptr, display::height());
}

void sprite_first_attributes_hblank_effect_ptr::set_attributes_ref(
        const span<const sprite_first_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void sprite_first_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_first_attributes_hblank_effect_ptr::swap(sprite_first_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_sprite, other._sprite);
}

sprite_first_attributes_hblank_effect_ptr::sprite_first_attributes_hblank_effect_ptr(int id, sprite_ptr&& sprite) :
    hblank_effect_ptr(id),
    _sprite(move(sprite))
{
}

}
