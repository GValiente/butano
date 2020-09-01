#include "btn_sprite_hblank_effects.h"

#include "btn_any.h"
#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_sprite_first_attributes.h"
#include "btn_sprite_third_attributes.h"
#include "btn_sprite_affine_second_attributes.h"
#include "btn_sprite_regular_second_attributes.h"
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
    };


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
                        last_value.hw_y, last_value.shape, last_value.bpp_mode, last_value.affine_mode,
                        sprite_first_attributes_ptr, output_values_ptr);
        }

    private:
        class alignas(alignof(int)) last_value_type
        {

        public:
            fixed hw_y = -1;
            sprite_shape shape = sprite_shape::SQUARE;
            palette_bpp_mode bpp_mode = palette_bpp_mode::BPP_4;
            int affine_mode = -1;

            last_value_type() = default;

            explicit last_value_type(void* handle) :
                hw_y(sprites_manager::hw_position(handle).y()),
                shape(sprites_manager::shape(handle)),
                bpp_mode(sprites_manager::palette(handle).bpp_mode()),
                affine_mode(sprites_manager::affine_mode(handle))
            {
            }

            explicit last_value_type(int target_id) :
                last_value_type(reinterpret_cast<void*>(target_id))
            {
            }

            [[nodiscard]] friend bool operator==(const last_value_type& a, const last_value_type& b) = default;
        };
    };


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


    class regular_second_attributes_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        regular_second_attributes_hblank_effect_handler() = default;

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
            auto second_attributes_ptr = reinterpret_cast<const sprite_regular_second_attributes*>(input_values_ptr);
            sprites_manager::fill_hblank_effect_regular_second_attributes(
                        handle, last_value.hw_x, last_value.size, second_attributes_ptr, output_values_ptr);
        }
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
        horizontal_position_hblank_effect_handler horizontal_position_handler;
        vertical_position_hblank_effect_handler vertical_position_handler;
        first_attributes_hblank_effect_handler first_attributes_handler;
        regular_second_attributes_hblank_effect_handler regular_second_attributes_handler;
        affine_second_attributes_hblank_effect_handler affine_second_attributes_handler;
        third_attributes_hblank_effect_handler third_attributes_handler;
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

sprite_regular_second_attributes_hblank_effect_ptr sprite_regular_second_attributes_hblank_effect_ptr::create(
        sprite_ptr sprite, const span<const sprite_regular_second_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(),
                                            int(sprite.handle()), data.regular_second_attributes_handler);
    return sprite_regular_second_attributes_hblank_effect_ptr(id, move(sprite));
}

optional<sprite_regular_second_attributes_hblank_effect_ptr> sprite_regular_second_attributes_hblank_effect_ptr::create_optional(
        sprite_ptr sprite, const span<const sprite_regular_second_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(),
                                                     int(sprite.handle()), data.regular_second_attributes_handler);
    optional<sprite_regular_second_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_regular_second_attributes_hblank_effect_ptr(id, move(sprite));
    }

    return result;
}

sprite_regular_second_attributes_hblank_effect_ptr::sprite_regular_second_attributes_hblank_effect_ptr(
        sprite_regular_second_attributes_hblank_effect_ptr&& other) noexcept :
    hblank_effect_ptr(move(other)),
    _sprite(move(other._sprite))
{
}

sprite_regular_second_attributes_hblank_effect_ptr& sprite_regular_second_attributes_hblank_effect_ptr::operator=(
        sprite_regular_second_attributes_hblank_effect_ptr&& other) noexcept
{
    _sprite = move(other._sprite);
    hblank_effect_ptr::operator=(move(other));
    return *this;
}

span<const sprite_regular_second_attributes> sprite_regular_second_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const sprite_regular_second_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_regular_second_attributes>(values_ptr, display::height());
}

void sprite_regular_second_attributes_hblank_effect_ptr::set_attributes_ref(
        const span<const sprite_regular_second_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void sprite_regular_second_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_regular_second_attributes_hblank_effect_ptr::swap(
        sprite_regular_second_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_sprite, other._sprite);
}

sprite_regular_second_attributes_hblank_effect_ptr::sprite_regular_second_attributes_hblank_effect_ptr(
        int id, sprite_ptr&& sprite) :
    hblank_effect_ptr(id),
    _sprite(move(sprite))
{
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
