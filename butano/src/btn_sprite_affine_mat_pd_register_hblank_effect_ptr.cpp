#include "btn_sprite_affine_mat_pd_register_hblank_effect_ptr.h"

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
    class pd_attributes_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        pd_attributes_hblank_effect_handler() = default;

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

        [[nodiscard]] uint16_t* output_register(int target_id) final
        {
            int16_t* result = hw::sprite_affine_mats::pd_register(target_id);
            return reinterpret_cast<uint16_t*>(result);
        }

        void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr) final
        {
            auto attributes_ptr = reinterpret_cast<const sprite_affine_mat_attributes*>(input_values_ptr);

            for(int index = 0; index < display::height(); ++index)
            {
                output_values_ptr[index] = uint16_t(attributes_ptr[index].pd_register_value());
            }
        }
    };


    class pd_values_hblank_effect_handler : public hblank_effect_handler
    {

    public:
        pd_values_hblank_effect_handler() = default;

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

        [[nodiscard]] uint16_t* output_register(int target_id) final
        {
            int16_t* result = hw::sprite_affine_mats::pd_register(target_id);
            return reinterpret_cast<uint16_t*>(result);
        }

        void write_output_values(int, const iany&, const void* input_values_ptr, uint16_t* output_values_ptr) final
        {
            auto int_source = static_cast<const unsigned*>(input_values_ptr);
            auto int_destination = reinterpret_cast<unsigned*>(output_values_ptr);
            memory::copy(*int_source, display::height() / 2, *int_destination);
        }
    };


    class static_data
    {

    public:
        pd_attributes_hblank_effect_handler attributes_handler;
        pd_values_hblank_effect_handler values_handler;
    };

    BTN_DATA_EWRAM static_data data;
}

sprite_affine_mat_pd_register_hblank_effect_ptr sprite_affine_mat_pd_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat, const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), affine_mat.id(),
                                            data.attributes_handler);
    return sprite_affine_mat_pd_register_hblank_effect_ptr(id, true, move(affine_mat));
}

sprite_affine_mat_pd_register_hblank_effect_ptr sprite_affine_mat_pd_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create(values_ref.data(), values_ref.size(), affine_mat.id(),
                                            data.values_handler);
    return sprite_affine_mat_pd_register_hblank_effect_ptr(id, false, move(affine_mat));
}

optional<sprite_affine_mat_pd_register_hblank_effect_ptr> sprite_affine_mat_pd_register_hblank_effect_ptr::create_optional(
        sprite_affine_mat_ptr affine_mat, const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(), affine_mat.id(),
                                                     data.attributes_handler);
    optional<sprite_affine_mat_pd_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_pd_register_hblank_effect_ptr(id, true, move(affine_mat));
    }

    return result;
}

optional<sprite_affine_mat_pd_register_hblank_effect_ptr> sprite_affine_mat_pd_register_hblank_effect_ptr::create_optional(
        sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create_optional(values_ref.data(), values_ref.size(), affine_mat.id(),
                                                     data.values_handler);
    optional<sprite_affine_mat_pd_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_pd_register_hblank_effect_ptr(id, false, move(affine_mat));
    }

    return result;
}

sprite_affine_mat_pd_register_hblank_effect_ptr::sprite_affine_mat_pd_register_hblank_effect_ptr(
        sprite_affine_mat_pd_register_hblank_effect_ptr&& other) noexcept :
    hblank_effect_ptr(move(other)),
    _from_attributes(other._from_attributes),
    _affine_mat(move(other._affine_mat))
{
}

sprite_affine_mat_pd_register_hblank_effect_ptr& sprite_affine_mat_pd_register_hblank_effect_ptr::operator=(
        sprite_affine_mat_pd_register_hblank_effect_ptr&& other) noexcept
{
    _from_attributes = other._from_attributes;
    _affine_mat = move(other._affine_mat);
    hblank_effect_ptr::operator=(move(other));
    return *this;
}

span<const sprite_affine_mat_attributes> sprite_affine_mat_pd_register_hblank_effect_ptr::attributes_ref() const
{
    BTN_ASSERT(_from_attributes, "Built from values");

    auto attributes_ptr = reinterpret_cast<const sprite_affine_mat_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_affine_mat_attributes>(attributes_ptr, display::height());
}

span<const int16_t> sprite_affine_mat_pd_register_hblank_effect_ptr::values_ref() const
{
    BTN_ASSERT(! _from_attributes, "Built from attributes");

    auto values_ptr = reinterpret_cast<const int16_t*>(hblank_effects_manager::values_ref(id()));
    return span<const int16_t>(values_ptr, display::height());
}

void sprite_affine_mat_pd_register_hblank_effect_ptr::set_attributes_ref(
        const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    BTN_ASSERT(_from_attributes, "Built from values");

    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void sprite_affine_mat_pd_register_hblank_effect_ptr::set_values_ref(
        const span<const int16_t>& values_ref)
{
    BTN_ASSERT(! _from_attributes, "Built from attributes");

    hblank_effects_manager::set_values_ref(id(), values_ref.data(), values_ref.size());
}

void sprite_affine_mat_pd_register_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_pd_register_hblank_effect_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_pd_register_hblank_effect_ptr::swap(
        sprite_affine_mat_pd_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_from_attributes, other._from_attributes);
    btn::swap(_affine_mat, other._affine_mat);
}

sprite_affine_mat_pd_register_hblank_effect_ptr::sprite_affine_mat_pd_register_hblank_effect_ptr(
        int id, bool from_attributes, sprite_affine_mat_ptr&& affine_mat) :
    hblank_effect_ptr(id),
    _from_attributes(from_attributes),
    _affine_mat(move(affine_mat))
{
}

}
