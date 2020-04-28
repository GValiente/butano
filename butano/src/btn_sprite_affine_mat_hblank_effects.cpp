#include "btn_sprite_affine_mat_hblank_effects.h"

#include "btn_any.h"
#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_hblank_effect_handler.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_sprite_affine_mats.h"

namespace btn
{

namespace
{
    class first_register_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        first_register_hblank_effect_handler() = default;

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

        [[nodiscard]] uint16_t* output_register(int target_id, const iany&) final
        {
            int16_t* result = hw::sprite_affine_mats::first_register(target_id);
            return reinterpret_cast<uint16_t*>(result);
        }

        void write_output_values(int, const iany&, int, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            memcpy(output_values_ptr, input_values_ptr, btn::display::height() * sizeof(uint16_t));
        }
    };


    class second_register_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        second_register_hblank_effect_handler() = default;

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

        [[nodiscard]] uint16_t* output_register(int target_id, const iany&) final
        {
            int16_t* result = hw::sprite_affine_mats::second_register(target_id);
            return reinterpret_cast<uint16_t*>(result);
        }

        void write_output_values(int, const iany&, int, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            memcpy(output_values_ptr, input_values_ptr, btn::display::height() * sizeof(uint16_t));
        }
    };


    class third_register_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        third_register_hblank_effect_handler() = default;

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

        [[nodiscard]] uint16_t* output_register(int target_id, const iany&) final
        {
            int16_t* result = hw::sprite_affine_mats::third_register(target_id);
            return reinterpret_cast<uint16_t*>(result);
        }

        void write_output_values(int, const iany&, int, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            memcpy(output_values_ptr, input_values_ptr, btn::display::height() * sizeof(uint16_t));
        }
    };


    class fourth_register_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        fourth_register_hblank_effect_handler() = default;

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

        [[nodiscard]] uint16_t* output_register(int target_id, const iany&) final
        {
            int16_t* result = hw::sprite_affine_mats::fourth_register(target_id);
            return reinterpret_cast<uint16_t*>(result);
        }

        void write_output_values(int, const iany&, int, const void* input_values_ptr,
                                 uint16_t* output_values_ptr) final
        {
            memcpy(output_values_ptr, input_values_ptr, btn::display::height() * sizeof(uint16_t));
        }
    };


    class static_data
    {

    public:
        first_register_hblank_effect_handler first_register_handler;
        second_register_hblank_effect_handler second_register_handler;
        third_register_hblank_effect_handler third_register_handler;
        fourth_register_hblank_effect_handler fourth_register_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

sprite_affine_mat_first_register_hblank_effect_ptr sprite_affine_mat_first_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                            data.first_register_handler);
    return sprite_affine_mat_first_register_hblank_effect_ptr(id, move(affine_mat_ptr));
}

optional<sprite_affine_mat_first_register_hblank_effect_ptr> sprite_affine_mat_first_register_hblank_effect_ptr::optional_create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::optional_create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                                     data.first_register_handler);
    optional<sprite_affine_mat_first_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_first_register_hblank_effect_ptr(id, move(affine_mat_ptr));
    }

    return result;
}

sprite_affine_mat_first_register_hblank_effect_ptr::sprite_affine_mat_first_register_hblank_effect_ptr(
        const sprite_affine_mat_first_register_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _affine_mat_ptr(other._affine_mat_ptr)
{
}

sprite_affine_mat_first_register_hblank_effect_ptr& sprite_affine_mat_first_register_hblank_effect_ptr::operator=(
        const sprite_affine_mat_first_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _affine_mat_ptr = other._affine_mat_ptr;
    return *this;
}

sprite_affine_mat_first_register_hblank_effect_ptr::sprite_affine_mat_first_register_hblank_effect_ptr(
        sprite_affine_mat_first_register_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _affine_mat_ptr(move(other._affine_mat_ptr))
{
}

sprite_affine_mat_first_register_hblank_effect_ptr& sprite_affine_mat_first_register_hblank_effect_ptr::operator=(
        sprite_affine_mat_first_register_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _affine_mat_ptr = move(other._affine_mat_ptr);
    return *this;
}

span<const int16_t> sprite_affine_mat_first_register_hblank_effect_ptr::values_ref() const
{
    auto values_ptr = reinterpret_cast<const int16_t*>(hblank_effects_manager::values_ref(id()));
    return span<const int16_t>(values_ptr, hblank_effects_manager::values_count(id()));
}

void sprite_affine_mat_first_register_hblank_effect_ptr::set_values_ref(
        const span<const int16_t>& values_ref)
{
    hblank_effects_manager::set_values_ref(id(), values_ref.data(), values_ref.size());
}

void sprite_affine_mat_first_register_hblank_effect_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_first_register_hblank_effect_ptr::swap(
        sprite_affine_mat_first_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_affine_mat_ptr, other._affine_mat_ptr);
}

sprite_affine_mat_first_register_hblank_effect_ptr::sprite_affine_mat_first_register_hblank_effect_ptr(
        int id, sprite_affine_mat_ptr&& affine_mat_ptr) :
    hblank_effect_ptr(id),
    _affine_mat_ptr(move(affine_mat_ptr))
{
}

sprite_affine_mat_second_register_hblank_effect_ptr sprite_affine_mat_second_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                            data.second_register_handler);
    return sprite_affine_mat_second_register_hblank_effect_ptr(id, move(affine_mat_ptr));
}

optional<sprite_affine_mat_second_register_hblank_effect_ptr> sprite_affine_mat_second_register_hblank_effect_ptr::optional_create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::optional_create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                                     data.second_register_handler);
    optional<sprite_affine_mat_second_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_second_register_hblank_effect_ptr(id, move(affine_mat_ptr));
    }

    return result;
}

sprite_affine_mat_second_register_hblank_effect_ptr::sprite_affine_mat_second_register_hblank_effect_ptr(
        const sprite_affine_mat_second_register_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _affine_mat_ptr(other._affine_mat_ptr)
{
}

sprite_affine_mat_second_register_hblank_effect_ptr& sprite_affine_mat_second_register_hblank_effect_ptr::operator=(
        const sprite_affine_mat_second_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _affine_mat_ptr = other._affine_mat_ptr;
    return *this;
}

sprite_affine_mat_second_register_hblank_effect_ptr::sprite_affine_mat_second_register_hblank_effect_ptr(
        sprite_affine_mat_second_register_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _affine_mat_ptr(move(other._affine_mat_ptr))
{
}

sprite_affine_mat_second_register_hblank_effect_ptr& sprite_affine_mat_second_register_hblank_effect_ptr::operator=(
        sprite_affine_mat_second_register_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _affine_mat_ptr = move(other._affine_mat_ptr);
    return *this;
}

span<const int16_t> sprite_affine_mat_second_register_hblank_effect_ptr::values_ref() const
{
    auto values_ptr = reinterpret_cast<const int16_t*>(hblank_effects_manager::values_ref(id()));
    return span<const int16_t>(values_ptr, hblank_effects_manager::values_count(id()));
}

void sprite_affine_mat_second_register_hblank_effect_ptr::set_values_ref(
        const span<const int16_t>& values_ref)
{
    hblank_effects_manager::set_values_ref(id(), values_ref.data(), values_ref.size());
}

void sprite_affine_mat_second_register_hblank_effect_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_second_register_hblank_effect_ptr::swap(
        sprite_affine_mat_second_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_affine_mat_ptr, other._affine_mat_ptr);
}

sprite_affine_mat_second_register_hblank_effect_ptr::sprite_affine_mat_second_register_hblank_effect_ptr(
        int id, sprite_affine_mat_ptr&& affine_mat_ptr) :
    hblank_effect_ptr(id),
    _affine_mat_ptr(move(affine_mat_ptr))
{
}

sprite_affine_mat_third_register_hblank_effect_ptr sprite_affine_mat_third_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                            data.third_register_handler);
    return sprite_affine_mat_third_register_hblank_effect_ptr(id, move(affine_mat_ptr));
}

optional<sprite_affine_mat_third_register_hblank_effect_ptr> sprite_affine_mat_third_register_hblank_effect_ptr::optional_create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::optional_create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                                     data.third_register_handler);
    optional<sprite_affine_mat_third_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_third_register_hblank_effect_ptr(id, move(affine_mat_ptr));
    }

    return result;
}

sprite_affine_mat_third_register_hblank_effect_ptr::sprite_affine_mat_third_register_hblank_effect_ptr(
        const sprite_affine_mat_third_register_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _affine_mat_ptr(other._affine_mat_ptr)
{
}

sprite_affine_mat_third_register_hblank_effect_ptr& sprite_affine_mat_third_register_hblank_effect_ptr::operator=(
        const sprite_affine_mat_third_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _affine_mat_ptr = other._affine_mat_ptr;
    return *this;
}

sprite_affine_mat_third_register_hblank_effect_ptr::sprite_affine_mat_third_register_hblank_effect_ptr(
        sprite_affine_mat_third_register_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _affine_mat_ptr(move(other._affine_mat_ptr))
{
}

sprite_affine_mat_third_register_hblank_effect_ptr& sprite_affine_mat_third_register_hblank_effect_ptr::operator=(
        sprite_affine_mat_third_register_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _affine_mat_ptr = move(other._affine_mat_ptr);
    return *this;
}

span<const int16_t> sprite_affine_mat_third_register_hblank_effect_ptr::values_ref() const
{
    auto values_ptr = reinterpret_cast<const int16_t*>(hblank_effects_manager::values_ref(id()));
    return span<const int16_t>(values_ptr, hblank_effects_manager::values_count(id()));
}

void sprite_affine_mat_third_register_hblank_effect_ptr::set_values_ref(
        const span<const int16_t>& values_ref)
{
    hblank_effects_manager::set_values_ref(id(), values_ref.data(), values_ref.size());
}

void sprite_affine_mat_third_register_hblank_effect_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_third_register_hblank_effect_ptr::swap(
        sprite_affine_mat_third_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_affine_mat_ptr, other._affine_mat_ptr);
}

sprite_affine_mat_third_register_hblank_effect_ptr::sprite_affine_mat_third_register_hblank_effect_ptr(
        int id, sprite_affine_mat_ptr&& affine_mat_ptr) :
    hblank_effect_ptr(id),
    _affine_mat_ptr(move(affine_mat_ptr))
{
}

sprite_affine_mat_fourth_register_hblank_effect_ptr sprite_affine_mat_fourth_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                            data.fourth_register_handler);
    return sprite_affine_mat_fourth_register_hblank_effect_ptr(id, move(affine_mat_ptr));
}

optional<sprite_affine_mat_fourth_register_hblank_effect_ptr> sprite_affine_mat_fourth_register_hblank_effect_ptr::optional_create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::optional_create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                                     data.fourth_register_handler);
    optional<sprite_affine_mat_fourth_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_fourth_register_hblank_effect_ptr(id, move(affine_mat_ptr));
    }

    return result;
}

sprite_affine_mat_fourth_register_hblank_effect_ptr::sprite_affine_mat_fourth_register_hblank_effect_ptr(
        const sprite_affine_mat_fourth_register_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _affine_mat_ptr(other._affine_mat_ptr)
{
}

sprite_affine_mat_fourth_register_hblank_effect_ptr& sprite_affine_mat_fourth_register_hblank_effect_ptr::operator=(
        const sprite_affine_mat_fourth_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _affine_mat_ptr = other._affine_mat_ptr;
    return *this;
}

sprite_affine_mat_fourth_register_hblank_effect_ptr::sprite_affine_mat_fourth_register_hblank_effect_ptr(
        sprite_affine_mat_fourth_register_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _affine_mat_ptr(move(other._affine_mat_ptr))
{
}

sprite_affine_mat_fourth_register_hblank_effect_ptr& sprite_affine_mat_fourth_register_hblank_effect_ptr::operator=(
        sprite_affine_mat_fourth_register_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _affine_mat_ptr = move(other._affine_mat_ptr);
    return *this;
}

span<const int16_t> sprite_affine_mat_fourth_register_hblank_effect_ptr::values_ref() const
{
    auto values_ptr = reinterpret_cast<const int16_t*>(hblank_effects_manager::values_ref(id()));
    return span<const int16_t>(values_ptr, hblank_effects_manager::values_count(id()));
}

void sprite_affine_mat_fourth_register_hblank_effect_ptr::set_values_ref(
        const span<const int16_t>& values_ref)
{
    hblank_effects_manager::set_values_ref(id(), values_ref.data(), values_ref.size());
}

void sprite_affine_mat_fourth_register_hblank_effect_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_fourth_register_hblank_effect_ptr::swap(
        sprite_affine_mat_fourth_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_affine_mat_ptr, other._affine_mat_ptr);
}

sprite_affine_mat_fourth_register_hblank_effect_ptr::sprite_affine_mat_fourth_register_hblank_effect_ptr(
        int id, sprite_affine_mat_ptr&& affine_mat_ptr) :
    hblank_effect_ptr(id),
    _affine_mat_ptr(move(affine_mat_ptr))
{
}

}
