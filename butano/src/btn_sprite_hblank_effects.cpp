#include "btn_sprite_hblank_effects.h"

#include "btn_any.h"
#include "btn_span.h"
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

        [[nodiscard]] uint16_t* output_register(int, const iany& target_last_value) final
        {
            const last_value_type& last_value = target_last_value.value<last_value_type>();
            return hw::sprites::first_attributes_register(last_value.hw_id);
        }

        void write_output_values(int, const iany& target_last_value, int,
                                 const void* input_values_ptr, uint16_t* output_values_ptr) final
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
            int hw_id;
            int hw_y;
            sprite_shape shape;
            palette_bpp_mode bpp_mode;
            int affine_mode;

            last_value_type() :
                hw_id(-1),
                hw_y(-1),
                shape(sprite_shape::SQUARE),
                bpp_mode(palette_bpp_mode::BPP_4),
                affine_mode(-1)
            {
            }

            explicit last_value_type(void* handle) :
                hw_id(*sprites_manager::hw_id(handle)),
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

            [[nodiscard]] friend bool operator==(const last_value_type& a, const last_value_type& b)
            {
                return a.hw_id == b.hw_id && a.hw_y == b.hw_y && a.shape == b.shape && a.bpp_mode == b.bpp_mode &&
                        a.affine_mode == b.affine_mode;
            }

            [[nodiscard]] friend bool operator!=(const last_value_type& a, const last_value_type& b)
            {
                return ! (a == b);
            }
        };
    };


    class alignas(alignof(int)) second_attributes_last_value_type
    {

    public:
        int hw_id;
        int hw_x;
        sprite_size size;

        second_attributes_last_value_type() :
            hw_id(-1),
            hw_x(-1),
            size(sprite_size::SMALL)
        {
        }

        explicit second_attributes_last_value_type(void* handle) :
            hw_id(*sprites_manager::hw_id(handle)),
            hw_x(sprites_manager::hw_position(handle).x()),
            size(sprites_manager::size(handle))
        {
        }

        explicit second_attributes_last_value_type(int target_id) :
            second_attributes_last_value_type(reinterpret_cast<void*>(target_id))
        {
        }

        [[nodiscard]] friend bool operator==(const second_attributes_last_value_type& a,
                                             const second_attributes_last_value_type& b)
        {
            return a.hw_id == b.hw_id && a.hw_x == b.hw_x && a.size == b.size;
        }

        [[nodiscard]] friend bool operator!=(const second_attributes_last_value_type& a,
                                             const second_attributes_last_value_type& b)
        {
            return ! (a == b);
        }
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

        [[nodiscard]] uint16_t* output_register(int, const iany& target_last_value) final
        {
            const second_attributes_last_value_type& last_value = target_last_value.value<second_attributes_last_value_type>();
            return hw::sprites::second_attributes_register(last_value.hw_id);
        }

        void write_output_values(int target_id, const iany& target_last_value, int,
                                 const void* input_values_ptr, uint16_t* output_values_ptr) final
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

        [[nodiscard]] uint16_t* output_register(int, const iany& target_last_value) final
        {
            const second_attributes_last_value_type& last_value = target_last_value.value<second_attributes_last_value_type>();
            return hw::sprites::second_attributes_register(last_value.hw_id);
        }

        void write_output_values(int target_id, const iany& target_last_value, int,
                                 const void* input_values_ptr, uint16_t* output_values_ptr) final
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

        [[nodiscard]] uint16_t* output_register(int, const iany& target_last_value) final
        {
            const last_value_type& last_value = target_last_value.value<last_value_type>();
            return hw::sprites::third_attributes_register(last_value.hw_id);
        }

        void write_output_values(int, const iany& target_last_value, int,
                                 const void* input_values_ptr, uint16_t* output_values_ptr) final
        {
            const last_value_type& last_value = target_last_value.value<last_value_type>();
            auto sprite_third_attributes_ptr = reinterpret_cast<const sprite_third_attributes*>(input_values_ptr);
            sprites_manager::fill_hblank_effect_third_attributes(
                        last_value.shape_size, sprite_third_attributes_ptr, output_values_ptr);
        }

    private:
        class alignas(alignof(int)) last_value_type
        {

        public:
            int hw_id;
            sprite_shape_size shape_size;

            last_value_type() :
                hw_id(-1),
                shape_size(sprite_shape::SQUARE, sprite_size::SMALL)
            {
            }

            explicit last_value_type(void* handle) :
                hw_id(*sprites_manager::hw_id(handle)),
                shape_size(sprites_manager::shape_size(handle))
            {
            }

            explicit last_value_type(int target_id) :
                last_value_type(reinterpret_cast<void*>(target_id))
            {
            }

            [[nodiscard]] friend bool operator==(const last_value_type& a, const last_value_type& b)
            {
                return a.hw_id == b.hw_id && a.shape_size == b.shape_size;
            }

            [[nodiscard]] friend bool operator!=(const last_value_type& a, const last_value_type& b)
            {
                return ! (a == b);
            }
        };
    };


    class static_data
    {

    public:
        first_attributes_hblank_effect_handler first_attributes_handler;
        regular_second_attributes_hblank_effect_handler regular_second_attributes_handler;
        affine_second_attributes_hblank_effect_handler affine_second_attributes_handler;
        third_attributes_hblank_effect_handler third_attributes_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

sprite_first_attributes_hblank_effect_ptr sprite_first_attributes_hblank_effect_ptr::create(
        sprite_ptr sprite_ptr, const span<const sprite_first_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(),
                                            int(sprite_ptr.handle()), data.first_attributes_handler);
    return sprite_first_attributes_hblank_effect_ptr(id, move(sprite_ptr));
}

optional<sprite_first_attributes_hblank_effect_ptr> sprite_first_attributes_hblank_effect_ptr::optional_create(
        sprite_ptr sprite_ptr, const span<const sprite_first_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::optional_create(attributes_ref.data(), attributes_ref.size(),
                                                     int(sprite_ptr.handle()), data.first_attributes_handler);
    optional<sprite_first_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_first_attributes_hblank_effect_ptr(id, move(sprite_ptr));
    }

    return result;
}

sprite_first_attributes_hblank_effect_ptr::sprite_first_attributes_hblank_effect_ptr(
        const sprite_first_attributes_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _sprite_ptr(other._sprite_ptr)
{
}

sprite_first_attributes_hblank_effect_ptr& sprite_first_attributes_hblank_effect_ptr::operator=(
        const sprite_first_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _sprite_ptr = other._sprite_ptr;
    return *this;
}

sprite_first_attributes_hblank_effect_ptr::sprite_first_attributes_hblank_effect_ptr(
        sprite_first_attributes_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _sprite_ptr(move(other._sprite_ptr))
{
}

sprite_first_attributes_hblank_effect_ptr& sprite_first_attributes_hblank_effect_ptr::operator=(
        sprite_first_attributes_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _sprite_ptr = move(other._sprite_ptr);
    return *this;
}

span<const sprite_first_attributes> sprite_first_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const sprite_first_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_first_attributes>(values_ptr, hblank_effects_manager::values_count(id()));
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
    btn::swap(_sprite_ptr, other._sprite_ptr);
}

sprite_first_attributes_hblank_effect_ptr::sprite_first_attributes_hblank_effect_ptr(int id, sprite_ptr&& sprite_ptr) :
    hblank_effect_ptr(id),
    _sprite_ptr(move(sprite_ptr))
{
}

sprite_regular_second_attributes_hblank_effect_ptr sprite_regular_second_attributes_hblank_effect_ptr::create(
        sprite_ptr sprite_ptr, const span<const sprite_regular_second_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(),
                                            int(sprite_ptr.handle()), data.regular_second_attributes_handler);
    return sprite_regular_second_attributes_hblank_effect_ptr(id, move(sprite_ptr));
}

optional<sprite_regular_second_attributes_hblank_effect_ptr> sprite_regular_second_attributes_hblank_effect_ptr::optional_create(
        sprite_ptr sprite_ptr, const span<const sprite_regular_second_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::optional_create(attributes_ref.data(), attributes_ref.size(),
                                                     int(sprite_ptr.handle()), data.regular_second_attributes_handler);
    optional<sprite_regular_second_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_regular_second_attributes_hblank_effect_ptr(id, move(sprite_ptr));
    }

    return result;
}

sprite_regular_second_attributes_hblank_effect_ptr::sprite_regular_second_attributes_hblank_effect_ptr(
        const sprite_regular_second_attributes_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _sprite_ptr(other._sprite_ptr)
{
}

sprite_regular_second_attributes_hblank_effect_ptr& sprite_regular_second_attributes_hblank_effect_ptr::operator=(
        const sprite_regular_second_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _sprite_ptr = other._sprite_ptr;
    return *this;
}

sprite_regular_second_attributes_hblank_effect_ptr::sprite_regular_second_attributes_hblank_effect_ptr(
        sprite_regular_second_attributes_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _sprite_ptr(move(other._sprite_ptr))
{
}

sprite_regular_second_attributes_hblank_effect_ptr& sprite_regular_second_attributes_hblank_effect_ptr::operator=(
        sprite_regular_second_attributes_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _sprite_ptr = move(other._sprite_ptr);
    return *this;
}

span<const sprite_regular_second_attributes> sprite_regular_second_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const sprite_regular_second_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_regular_second_attributes>(values_ptr, hblank_effects_manager::values_count(id()));
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
    btn::swap(_sprite_ptr, other._sprite_ptr);
}

sprite_regular_second_attributes_hblank_effect_ptr::sprite_regular_second_attributes_hblank_effect_ptr(
        int id, sprite_ptr&& sprite_ptr) :
    hblank_effect_ptr(id),
    _sprite_ptr(move(sprite_ptr))
{
}

sprite_affine_second_attributes_hblank_effect_ptr sprite_affine_second_attributes_hblank_effect_ptr::create(
        sprite_ptr sprite_ptr, const span<const sprite_affine_second_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(),
                                            int(sprite_ptr.handle()), data.affine_second_attributes_handler);
    return sprite_affine_second_attributes_hblank_effect_ptr(id, move(sprite_ptr));
}

optional<sprite_affine_second_attributes_hblank_effect_ptr> sprite_affine_second_attributes_hblank_effect_ptr::optional_create(
        sprite_ptr sprite_ptr, const span<const sprite_affine_second_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::optional_create(attributes_ref.data(), attributes_ref.size(),
                                                     int(sprite_ptr.handle()), data.affine_second_attributes_handler);
    optional<sprite_affine_second_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_second_attributes_hblank_effect_ptr(id, move(sprite_ptr));
    }

    return result;
}

sprite_affine_second_attributes_hblank_effect_ptr::sprite_affine_second_attributes_hblank_effect_ptr(
        const sprite_affine_second_attributes_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _sprite_ptr(other._sprite_ptr)
{
}

sprite_affine_second_attributes_hblank_effect_ptr& sprite_affine_second_attributes_hblank_effect_ptr::operator=(
        const sprite_affine_second_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _sprite_ptr = other._sprite_ptr;
    return *this;
}

sprite_affine_second_attributes_hblank_effect_ptr::sprite_affine_second_attributes_hblank_effect_ptr(
        sprite_affine_second_attributes_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _sprite_ptr(move(other._sprite_ptr))
{
}

sprite_affine_second_attributes_hblank_effect_ptr& sprite_affine_second_attributes_hblank_effect_ptr::operator=(
        sprite_affine_second_attributes_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _sprite_ptr = move(other._sprite_ptr);
    return *this;
}

span<const sprite_affine_second_attributes> sprite_affine_second_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const sprite_affine_second_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_affine_second_attributes>(values_ptr, hblank_effects_manager::values_count(id()));
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
    btn::swap(_sprite_ptr, other._sprite_ptr);
}

sprite_affine_second_attributes_hblank_effect_ptr::sprite_affine_second_attributes_hblank_effect_ptr(
        int id, sprite_ptr&& sprite_ptr) :
    hblank_effect_ptr(id),
    _sprite_ptr(move(sprite_ptr))
{
}

sprite_third_attributes_hblank_effect_ptr sprite_third_attributes_hblank_effect_ptr::create(
        sprite_ptr sprite_ptr, const span<const sprite_third_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(),
                                            int(sprite_ptr.handle()), data.third_attributes_handler);
    return sprite_third_attributes_hblank_effect_ptr(id, move(sprite_ptr));
}

optional<sprite_third_attributes_hblank_effect_ptr> sprite_third_attributes_hblank_effect_ptr::optional_create(
        sprite_ptr sprite_ptr, const span<const sprite_third_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::optional_create(attributes_ref.data(), attributes_ref.size(),
                                                     int(sprite_ptr.handle()), data.third_attributes_handler);
    optional<sprite_third_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_third_attributes_hblank_effect_ptr(id, move(sprite_ptr));
    }

    return result;
}

sprite_third_attributes_hblank_effect_ptr::sprite_third_attributes_hblank_effect_ptr(
        const sprite_third_attributes_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _sprite_ptr(other._sprite_ptr)
{
}

sprite_third_attributes_hblank_effect_ptr& sprite_third_attributes_hblank_effect_ptr::operator=(
        const sprite_third_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _sprite_ptr = other._sprite_ptr;
    return *this;
}

sprite_third_attributes_hblank_effect_ptr::sprite_third_attributes_hblank_effect_ptr(
        sprite_third_attributes_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _sprite_ptr(move(other._sprite_ptr))
{
}

sprite_third_attributes_hblank_effect_ptr& sprite_third_attributes_hblank_effect_ptr::operator=(
        sprite_third_attributes_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _sprite_ptr = move(other._sprite_ptr);
    return *this;
}

span<const sprite_third_attributes> sprite_third_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const sprite_third_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_third_attributes>(values_ptr, hblank_effects_manager::values_count(id()));
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
    btn::swap(_sprite_ptr, other._sprite_ptr);
}

sprite_third_attributes_hblank_effect_ptr::sprite_third_attributes_hblank_effect_ptr(int id, sprite_ptr&& sprite_ptr) :
    hblank_effect_ptr(id),
    _sprite_ptr(move(sprite_ptr))
{
}

}
