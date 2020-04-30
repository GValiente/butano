#include "btn_sprite_affine_mat_hblank_effects.h"

#include "btn_any.h"
#include "btn_span.h"
#include "btn_memory.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_hblank_effect_handler.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_sprite_affine_mats.h"

namespace btn
{

namespace
{
    class first_attributes_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        first_attributes_hblank_effect_handler() = default;

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
            auto attributes_ptr = reinterpret_cast<const sprite_affine_mat_attributes*>(input_values_ptr);

            for(int index = 0; index < btn::display::height(); ++index)
            {
                output_values_ptr[index] = uint16_t(attributes_ptr[index].first_register_value());
            }
        }
    };


    class second_attributes_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        second_attributes_hblank_effect_handler() = default;

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
            auto attributes_ptr = reinterpret_cast<const sprite_affine_mat_attributes*>(input_values_ptr);

            for(int index = 0; index < btn::display::height(); ++index)
            {
                output_values_ptr[index] = uint16_t(attributes_ptr[index].second_register_value());
            }
        }
    };


    class third_attributes_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        third_attributes_hblank_effect_handler() = default;

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
            auto attributes_ptr = reinterpret_cast<const sprite_affine_mat_attributes*>(input_values_ptr);

            for(int index = 0; index < btn::display::height(); ++index)
            {
                output_values_ptr[index] = uint16_t(attributes_ptr[index].third_register_value());
            }
        }
    };


    class fourth_attributes_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        fourth_attributes_hblank_effect_handler() = default;

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
            auto attributes_ptr = reinterpret_cast<const sprite_affine_mat_attributes*>(input_values_ptr);

            for(int index = 0; index < btn::display::height(); ++index)
            {
                output_values_ptr[index] = uint16_t(attributes_ptr[index].fourth_register_value());
            }
        }
    };


    class first_values_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        first_values_hblank_effect_handler() = default;

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
            memory::copy(*static_cast<const uint16_t*>(input_values_ptr), btn::display::height(), *output_values_ptr);
        }
    };


    class second_values_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        second_values_hblank_effect_handler() = default;

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
            memory::copy(*static_cast<const uint16_t*>(input_values_ptr), btn::display::height(), *output_values_ptr);
        }
    };


    class third_values_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        third_values_hblank_effect_handler() = default;

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
            memory::copy(*static_cast<const uint16_t*>(input_values_ptr), btn::display::height(), *output_values_ptr);
        }
    };


    class fourth_values_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        fourth_values_hblank_effect_handler() = default;

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
            memory::copy(*static_cast<const uint16_t*>(input_values_ptr), btn::display::height(), *output_values_ptr);
        }
    };


    class static_data
    {

    public:
        first_attributes_hblank_effect_handler first_attributes_handler;
        second_attributes_hblank_effect_handler second_attributes_handler;
        third_attributes_hblank_effect_handler third_attributes_handler;
        fourth_attributes_hblank_effect_handler fourth_attributes_handler;
        first_values_hblank_effect_handler first_values_handler;
        second_values_hblank_effect_handler second_values_handler;
        third_values_hblank_effect_handler third_values_handler;
        fourth_values_hblank_effect_handler fourth_values_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

sprite_affine_mat_first_register_hblank_effect_ptr sprite_affine_mat_first_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), affine_mat_ptr.id(),
                                            data.first_attributes_handler);
    return sprite_affine_mat_first_register_hblank_effect_ptr(id, true, move(affine_mat_ptr));
}

sprite_affine_mat_first_register_hblank_effect_ptr sprite_affine_mat_first_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                            data.first_values_handler);
    return sprite_affine_mat_first_register_hblank_effect_ptr(id, false, move(affine_mat_ptr));
}

optional<sprite_affine_mat_first_register_hblank_effect_ptr> sprite_affine_mat_first_register_hblank_effect_ptr::optional_create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::optional_create(attributes_ref.data(), attributes_ref.size(), affine_mat_ptr.id(),
                                                     data.first_attributes_handler);
    optional<sprite_affine_mat_first_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_first_register_hblank_effect_ptr(id, true, move(affine_mat_ptr));
    }

    return result;
}

optional<sprite_affine_mat_first_register_hblank_effect_ptr> sprite_affine_mat_first_register_hblank_effect_ptr::optional_create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::optional_create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                                     data.first_values_handler);
    optional<sprite_affine_mat_first_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_first_register_hblank_effect_ptr(id, false, move(affine_mat_ptr));
    }

    return result;
}

sprite_affine_mat_first_register_hblank_effect_ptr::sprite_affine_mat_first_register_hblank_effect_ptr(
        const sprite_affine_mat_first_register_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _from_attributes(other._from_attributes),
    _affine_mat_ptr(other._affine_mat_ptr)
{
}

sprite_affine_mat_first_register_hblank_effect_ptr& sprite_affine_mat_first_register_hblank_effect_ptr::operator=(
        const sprite_affine_mat_first_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _from_attributes = other._from_attributes;
    _affine_mat_ptr = other._affine_mat_ptr;
    return *this;
}

sprite_affine_mat_first_register_hblank_effect_ptr::sprite_affine_mat_first_register_hblank_effect_ptr(
        sprite_affine_mat_first_register_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _from_attributes(other._from_attributes),
    _affine_mat_ptr(move(other._affine_mat_ptr))
{
}

sprite_affine_mat_first_register_hblank_effect_ptr& sprite_affine_mat_first_register_hblank_effect_ptr::operator=(
        sprite_affine_mat_first_register_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _from_attributes = other._from_attributes;
    _affine_mat_ptr = move(other._affine_mat_ptr);
    return *this;
}

span<const sprite_affine_mat_attributes> sprite_affine_mat_first_register_hblank_effect_ptr::attributes_ref() const
{
    BTN_ASSERT(_from_attributes, "Built from values");

    auto attributes_ptr = reinterpret_cast<const sprite_affine_mat_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_affine_mat_attributes>(attributes_ptr, hblank_effects_manager::values_count(id()));
}

span<const int16_t> sprite_affine_mat_first_register_hblank_effect_ptr::values_ref() const
{
    BTN_ASSERT(! _from_attributes, "Built from attributes");

    auto values_ptr = reinterpret_cast<const int16_t*>(hblank_effects_manager::values_ref(id()));
    return span<const int16_t>(values_ptr, hblank_effects_manager::values_count(id()));
}

void sprite_affine_mat_first_register_hblank_effect_ptr::set_attributes_ref(
        const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    BTN_ASSERT(_from_attributes, "Built from values");

    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void sprite_affine_mat_first_register_hblank_effect_ptr::set_values_ref(
        const span<const int16_t>& values_ref)
{
    BTN_ASSERT(! _from_attributes, "Built from attributes");

    hblank_effects_manager::set_values_ref(id(), values_ref.data(), values_ref.size());
}

void sprite_affine_mat_first_register_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_first_register_hblank_effect_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_first_register_hblank_effect_ptr::swap(
        sprite_affine_mat_first_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_from_attributes, other._from_attributes);
    btn::swap(_affine_mat_ptr, other._affine_mat_ptr);
}

sprite_affine_mat_first_register_hblank_effect_ptr::sprite_affine_mat_first_register_hblank_effect_ptr(
        int id, bool from_attributes, sprite_affine_mat_ptr&& affine_mat_ptr) :
    hblank_effect_ptr(id),
    _from_attributes(from_attributes),
    _affine_mat_ptr(move(affine_mat_ptr))
{
}

sprite_affine_mat_second_register_hblank_effect_ptr sprite_affine_mat_second_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), affine_mat_ptr.id(),
                                            data.second_attributes_handler);
    return sprite_affine_mat_second_register_hblank_effect_ptr(id, true, move(affine_mat_ptr));
}

sprite_affine_mat_second_register_hblank_effect_ptr sprite_affine_mat_second_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                            data.second_values_handler);
    return sprite_affine_mat_second_register_hblank_effect_ptr(id, false, move(affine_mat_ptr));
}

optional<sprite_affine_mat_second_register_hblank_effect_ptr> sprite_affine_mat_second_register_hblank_effect_ptr::optional_create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::optional_create(attributes_ref.data(), attributes_ref.size(), affine_mat_ptr.id(),
                                                     data.second_attributes_handler);
    optional<sprite_affine_mat_second_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_second_register_hblank_effect_ptr(id, true, move(affine_mat_ptr));
    }

    return result;
}

optional<sprite_affine_mat_second_register_hblank_effect_ptr> sprite_affine_mat_second_register_hblank_effect_ptr::optional_create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::optional_create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                                     data.second_values_handler);
    optional<sprite_affine_mat_second_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_second_register_hblank_effect_ptr(id, false, move(affine_mat_ptr));
    }

    return result;
}

sprite_affine_mat_second_register_hblank_effect_ptr::sprite_affine_mat_second_register_hblank_effect_ptr(
        const sprite_affine_mat_second_register_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _from_attributes(other._from_attributes),
    _affine_mat_ptr(other._affine_mat_ptr)
{
}

sprite_affine_mat_second_register_hblank_effect_ptr& sprite_affine_mat_second_register_hblank_effect_ptr::operator=(
        const sprite_affine_mat_second_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _from_attributes = other._from_attributes;
    _affine_mat_ptr = other._affine_mat_ptr;
    return *this;
}

sprite_affine_mat_second_register_hblank_effect_ptr::sprite_affine_mat_second_register_hblank_effect_ptr(
        sprite_affine_mat_second_register_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _from_attributes(other._from_attributes),
    _affine_mat_ptr(move(other._affine_mat_ptr))
{
}

sprite_affine_mat_second_register_hblank_effect_ptr& sprite_affine_mat_second_register_hblank_effect_ptr::operator=(
        sprite_affine_mat_second_register_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _from_attributes = other._from_attributes;
    _affine_mat_ptr = move(other._affine_mat_ptr);
    return *this;
}

span<const sprite_affine_mat_attributes> sprite_affine_mat_second_register_hblank_effect_ptr::attributes_ref() const
{
    BTN_ASSERT(_from_attributes, "Built from values");

    auto attributes_ptr = reinterpret_cast<const sprite_affine_mat_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_affine_mat_attributes>(attributes_ptr, hblank_effects_manager::values_count(id()));
}

span<const int16_t> sprite_affine_mat_second_register_hblank_effect_ptr::values_ref() const
{
    BTN_ASSERT(! _from_attributes, "Built from attributes");

    auto values_ptr = reinterpret_cast<const int16_t*>(hblank_effects_manager::values_ref(id()));
    return span<const int16_t>(values_ptr, hblank_effects_manager::values_count(id()));
}

void sprite_affine_mat_second_register_hblank_effect_ptr::set_attributes_ref(
        const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    BTN_ASSERT(_from_attributes, "Built from values");

    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void sprite_affine_mat_second_register_hblank_effect_ptr::set_values_ref(
        const span<const int16_t>& values_ref)
{
    BTN_ASSERT(! _from_attributes, "Built from attributes");

    hblank_effects_manager::set_values_ref(id(), values_ref.data(), values_ref.size());
}

void sprite_affine_mat_second_register_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_second_register_hblank_effect_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_second_register_hblank_effect_ptr::swap(
        sprite_affine_mat_second_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_from_attributes, other._from_attributes);
    btn::swap(_affine_mat_ptr, other._affine_mat_ptr);
}

sprite_affine_mat_second_register_hblank_effect_ptr::sprite_affine_mat_second_register_hblank_effect_ptr(
        int id, bool from_attributes, sprite_affine_mat_ptr&& affine_mat_ptr) :
    hblank_effect_ptr(id),
    _from_attributes(from_attributes),
    _affine_mat_ptr(move(affine_mat_ptr))
{
}

sprite_affine_mat_third_register_hblank_effect_ptr sprite_affine_mat_third_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), affine_mat_ptr.id(),
                                            data.third_attributes_handler);
    return sprite_affine_mat_third_register_hblank_effect_ptr(id, true, move(affine_mat_ptr));
}

sprite_affine_mat_third_register_hblank_effect_ptr sprite_affine_mat_third_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                            data.third_values_handler);
    return sprite_affine_mat_third_register_hblank_effect_ptr(id, false, move(affine_mat_ptr));
}

optional<sprite_affine_mat_third_register_hblank_effect_ptr> sprite_affine_mat_third_register_hblank_effect_ptr::optional_create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::optional_create(attributes_ref.data(), attributes_ref.size(), affine_mat_ptr.id(),
                                                     data.third_attributes_handler);
    optional<sprite_affine_mat_third_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_third_register_hblank_effect_ptr(id, true, move(affine_mat_ptr));
    }

    return result;
}

optional<sprite_affine_mat_third_register_hblank_effect_ptr> sprite_affine_mat_third_register_hblank_effect_ptr::optional_create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::optional_create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                                     data.third_values_handler);
    optional<sprite_affine_mat_third_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_third_register_hblank_effect_ptr(id, false, move(affine_mat_ptr));
    }

    return result;
}

sprite_affine_mat_third_register_hblank_effect_ptr::sprite_affine_mat_third_register_hblank_effect_ptr(
        const sprite_affine_mat_third_register_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _from_attributes(other._from_attributes),
    _affine_mat_ptr(other._affine_mat_ptr)
{
}

sprite_affine_mat_third_register_hblank_effect_ptr& sprite_affine_mat_third_register_hblank_effect_ptr::operator=(
        const sprite_affine_mat_third_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _from_attributes = other._from_attributes;
    _affine_mat_ptr = other._affine_mat_ptr;
    return *this;
}

sprite_affine_mat_third_register_hblank_effect_ptr::sprite_affine_mat_third_register_hblank_effect_ptr(
        sprite_affine_mat_third_register_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _from_attributes(other._from_attributes),
    _affine_mat_ptr(move(other._affine_mat_ptr))
{
}

sprite_affine_mat_third_register_hblank_effect_ptr& sprite_affine_mat_third_register_hblank_effect_ptr::operator=(
        sprite_affine_mat_third_register_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _from_attributes = other._from_attributes;
    _affine_mat_ptr = move(other._affine_mat_ptr);
    return *this;
}

span<const sprite_affine_mat_attributes> sprite_affine_mat_third_register_hblank_effect_ptr::attributes_ref() const
{
    BTN_ASSERT(_from_attributes, "Built from values");

    auto attributes_ptr = reinterpret_cast<const sprite_affine_mat_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_affine_mat_attributes>(attributes_ptr, hblank_effects_manager::values_count(id()));
}

span<const int16_t> sprite_affine_mat_third_register_hblank_effect_ptr::values_ref() const
{
    BTN_ASSERT(! _from_attributes, "Built from attributes");

    auto values_ptr = reinterpret_cast<const int16_t*>(hblank_effects_manager::values_ref(id()));
    return span<const int16_t>(values_ptr, hblank_effects_manager::values_count(id()));
}

void sprite_affine_mat_third_register_hblank_effect_ptr::set_attributes_ref(
        const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    BTN_ASSERT(_from_attributes, "Built from values");

    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void sprite_affine_mat_third_register_hblank_effect_ptr::set_values_ref(
        const span<const int16_t>& values_ref)
{
    BTN_ASSERT(! _from_attributes, "Built from attributes");

    hblank_effects_manager::set_values_ref(id(), values_ref.data(), values_ref.size());
}

void sprite_affine_mat_third_register_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_third_register_hblank_effect_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_third_register_hblank_effect_ptr::swap(
        sprite_affine_mat_third_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_from_attributes, other._from_attributes);
    btn::swap(_affine_mat_ptr, other._affine_mat_ptr);
}

sprite_affine_mat_third_register_hblank_effect_ptr::sprite_affine_mat_third_register_hblank_effect_ptr(
        int id, bool from_attributes, sprite_affine_mat_ptr&& affine_mat_ptr) :
    hblank_effect_ptr(id),
    _from_attributes(from_attributes),
    _affine_mat_ptr(move(affine_mat_ptr))
{
}

sprite_affine_mat_fourth_register_hblank_effect_ptr sprite_affine_mat_fourth_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), affine_mat_ptr.id(),
                                            data.fourth_attributes_handler);
    return sprite_affine_mat_fourth_register_hblank_effect_ptr(id, true, move(affine_mat_ptr));
}

sprite_affine_mat_fourth_register_hblank_effect_ptr sprite_affine_mat_fourth_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                            data.fourth_values_handler);
    return sprite_affine_mat_fourth_register_hblank_effect_ptr(id, false, move(affine_mat_ptr));
}

optional<sprite_affine_mat_fourth_register_hblank_effect_ptr> sprite_affine_mat_fourth_register_hblank_effect_ptr::optional_create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::optional_create(attributes_ref.data(), attributes_ref.size(), affine_mat_ptr.id(),
                                                     data.fourth_attributes_handler);
    optional<sprite_affine_mat_fourth_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_fourth_register_hblank_effect_ptr(id, true, move(affine_mat_ptr));
    }

    return result;
}

optional<sprite_affine_mat_fourth_register_hblank_effect_ptr> sprite_affine_mat_fourth_register_hblank_effect_ptr::optional_create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::optional_create(values_ref.data(), values_ref.size(), affine_mat_ptr.id(),
                                                     data.fourth_values_handler);
    optional<sprite_affine_mat_fourth_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_fourth_register_hblank_effect_ptr(id, false, move(affine_mat_ptr));
    }

    return result;
}

sprite_affine_mat_fourth_register_hblank_effect_ptr::sprite_affine_mat_fourth_register_hblank_effect_ptr(
        const sprite_affine_mat_fourth_register_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _from_attributes(other._from_attributes),
    _affine_mat_ptr(other._affine_mat_ptr)
{
}

sprite_affine_mat_fourth_register_hblank_effect_ptr& sprite_affine_mat_fourth_register_hblank_effect_ptr::operator=(
        const sprite_affine_mat_fourth_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _from_attributes = other._from_attributes;
    _affine_mat_ptr = other._affine_mat_ptr;
    return *this;
}

sprite_affine_mat_fourth_register_hblank_effect_ptr::sprite_affine_mat_fourth_register_hblank_effect_ptr(
        sprite_affine_mat_fourth_register_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _from_attributes(other._from_attributes),
    _affine_mat_ptr(move(other._affine_mat_ptr))
{
}

sprite_affine_mat_fourth_register_hblank_effect_ptr& sprite_affine_mat_fourth_register_hblank_effect_ptr::operator=(
        sprite_affine_mat_fourth_register_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _from_attributes = other._from_attributes;
    _affine_mat_ptr = move(other._affine_mat_ptr);
    return *this;
}

span<const sprite_affine_mat_attributes> sprite_affine_mat_fourth_register_hblank_effect_ptr::attributes_ref() const
{
    BTN_ASSERT(_from_attributes, "Built from values");

    auto attributes_ptr = reinterpret_cast<const sprite_affine_mat_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_affine_mat_attributes>(attributes_ptr, hblank_effects_manager::values_count(id()));
}

span<const int16_t> sprite_affine_mat_fourth_register_hblank_effect_ptr::values_ref() const
{
    BTN_ASSERT(! _from_attributes, "Built from attributes");

    auto values_ptr = reinterpret_cast<const int16_t*>(hblank_effects_manager::values_ref(id()));
    return span<const int16_t>(values_ptr, hblank_effects_manager::values_count(id()));
}

void sprite_affine_mat_fourth_register_hblank_effect_ptr::set_attributes_ref(
        const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    BTN_ASSERT(_from_attributes, "Built from values");

    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void sprite_affine_mat_fourth_register_hblank_effect_ptr::set_values_ref(
        const span<const int16_t>& values_ref)
{
    BTN_ASSERT(! _from_attributes, "Built from attributes");

    hblank_effects_manager::set_values_ref(id(), values_ref.data(), values_ref.size());
}

void sprite_affine_mat_fourth_register_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_fourth_register_hblank_effect_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_fourth_register_hblank_effect_ptr::swap(
        sprite_affine_mat_fourth_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_from_attributes, other._from_attributes);
    btn::swap(_affine_mat_ptr, other._affine_mat_ptr);
}

sprite_affine_mat_fourth_register_hblank_effect_ptr::sprite_affine_mat_fourth_register_hblank_effect_ptr(
        int id, bool from_attributes, sprite_affine_mat_ptr&& affine_mat_ptr) :
    hblank_effect_ptr(id),
    _from_attributes(from_attributes),
    _affine_mat_ptr(move(affine_mat_ptr))
{
}

sprite_affine_mat_attributes_hblank_effect_ptr sprite_affine_mat_attributes_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    return sprite_affine_mat_attributes_hblank_effect_ptr(
                sprite_affine_mat_first_register_hblank_effect_ptr::create(affine_mat_ptr, attributes_ref),
                sprite_affine_mat_second_register_hblank_effect_ptr::create(affine_mat_ptr, attributes_ref),
                sprite_affine_mat_third_register_hblank_effect_ptr::create(affine_mat_ptr, attributes_ref),
                sprite_affine_mat_fourth_register_hblank_effect_ptr::create(affine_mat_ptr, attributes_ref));
}

optional<sprite_affine_mat_attributes_hblank_effect_ptr> sprite_affine_mat_attributes_hblank_effect_ptr::optional_create(
        sprite_affine_mat_ptr affine_mat_ptr, const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    optional<sprite_affine_mat_attributes_hblank_effect_ptr> result;

    if(auto first = sprite_affine_mat_first_register_hblank_effect_ptr::optional_create(
                affine_mat_ptr, attributes_ref))
    {
        if(auto second = sprite_affine_mat_second_register_hblank_effect_ptr::optional_create(
                    affine_mat_ptr, attributes_ref))
        {
            if(auto third = sprite_affine_mat_third_register_hblank_effect_ptr::optional_create(
                        affine_mat_ptr, attributes_ref))
            {
                if(auto fourth = sprite_affine_mat_fourth_register_hblank_effect_ptr::optional_create(
                            affine_mat_ptr, move(attributes_ref)))
                {
                    result = sprite_affine_mat_attributes_hblank_effect_ptr(
                                move(*first), move(*second), move(*third), move(*fourth));
                }
            }
        }
    }

    return result;
}

void sprite_affine_mat_attributes_hblank_effect_ptr::set_visible(bool visible)
{
    _first_hblank_effect_ptr.set_visible(visible);
    _second_hblank_effect_ptr.set_visible(visible);
    _third_hblank_effect_ptr.set_visible(visible);
    _fourth_hblank_effect_ptr.set_visible(visible);
}

span<const sprite_affine_mat_attributes> sprite_affine_mat_attributes_hblank_effect_ptr::attributes_ref() const
{
    return _first_hblank_effect_ptr.attributes_ref();
}

void sprite_affine_mat_attributes_hblank_effect_ptr::set_attributes_ref(
        const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    _first_hblank_effect_ptr.set_attributes_ref(attributes_ref);
    _second_hblank_effect_ptr.set_attributes_ref(attributes_ref);
    _third_hblank_effect_ptr.set_attributes_ref(attributes_ref);
    _fourth_hblank_effect_ptr.set_attributes_ref(attributes_ref);
}

void sprite_affine_mat_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    _first_hblank_effect_ptr.reload_attributes_ref();
    _second_hblank_effect_ptr.reload_attributes_ref();
    _third_hblank_effect_ptr.reload_attributes_ref();
    _fourth_hblank_effect_ptr.reload_attributes_ref();
}

void sprite_affine_mat_attributes_hblank_effect_ptr::swap(sprite_affine_mat_attributes_hblank_effect_ptr& other)
{
    _first_hblank_effect_ptr.swap(other._first_hblank_effect_ptr);
    _second_hblank_effect_ptr.swap(other._second_hblank_effect_ptr);
    _third_hblank_effect_ptr.swap(other._third_hblank_effect_ptr);
    _fourth_hblank_effect_ptr.swap(other._fourth_hblank_effect_ptr);
}

sprite_affine_mat_attributes_hblank_effect_ptr::sprite_affine_mat_attributes_hblank_effect_ptr(
        sprite_affine_mat_first_register_hblank_effect_ptr&& first_hblank_effect_ptr,
        sprite_affine_mat_second_register_hblank_effect_ptr&& second_hblank_effect_ptr,
        sprite_affine_mat_third_register_hblank_effect_ptr&& third_hblank_effect_ptr,
        sprite_affine_mat_fourth_register_hblank_effect_ptr&& fourth_hblank_effect_ptr) :
    _first_hblank_effect_ptr(move(first_hblank_effect_ptr)),
    _second_hblank_effect_ptr(move(second_hblank_effect_ptr)),
    _third_hblank_effect_ptr(move(third_hblank_effect_ptr)),
    _fourth_hblank_effect_ptr(move(fourth_hblank_effect_ptr))
{
}

}
