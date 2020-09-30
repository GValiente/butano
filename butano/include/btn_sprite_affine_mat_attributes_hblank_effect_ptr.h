#ifndef BTN_SPRITE_AFFINE_MAT_ATTRIBUTES_HBLANK_EFFECT_PTR_H
#define BTN_SPRITE_AFFINE_MAT_ATTRIBUTES_HBLANK_EFFECT_PTR_H

#include "btn_sprite_affine_mat_pa_register_hblank_effect_ptr.h"
#include "btn_sprite_affine_mat_pb_register_hblank_effect_ptr.h"
#include "btn_sprite_affine_mat_pc_register_hblank_effect_ptr.h"
#include "btn_sprite_affine_mat_pd_register_hblank_effect_ptr.h"

namespace btn
{

class sprite_affine_mat_attributes_hblank_effect_ptr
{

public:
    [[nodiscard]] static sprite_affine_mat_attributes_hblank_effect_ptr create(
            const sprite_affine_mat_ptr& affine_mat, const span<const sprite_affine_mat_attributes>& attributes_ref);

    [[nodiscard]] static optional<sprite_affine_mat_attributes_hblank_effect_ptr> create_optional(
            const sprite_affine_mat_ptr& affine_mat, const span<const sprite_affine_mat_attributes>& attributes_ref);

    [[nodiscard]] int id() const
    {
        return _pa_hblank_effect_ptr.id();
    }

    [[nodiscard]] bool visible() const
    {
        return _pa_hblank_effect_ptr.visible();
    }

    void set_visible(bool visible);

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return _pa_hblank_effect_ptr.affine_mat();
    }

    [[nodiscard]] span<const sprite_affine_mat_attributes> attributes_ref() const;

    void set_attributes_ref(const span<const sprite_affine_mat_attributes>& attributes_ref);

    void reload_attributes_ref();

    void swap(sprite_affine_mat_attributes_hblank_effect_ptr& other);

    friend void swap(sprite_affine_mat_attributes_hblank_effect_ptr& a,
                     sprite_affine_mat_attributes_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

    [[nodiscard]] friend bool operator==(const sprite_affine_mat_attributes_hblank_effect_ptr& a,
                                         const sprite_affine_mat_attributes_hblank_effect_ptr& b)
    {
        return a._pa_hblank_effect_ptr == b._pa_hblank_effect_ptr;
    }

    [[nodiscard]] friend bool operator!=(const sprite_affine_mat_attributes_hblank_effect_ptr& a,
                                         const sprite_affine_mat_attributes_hblank_effect_ptr& b)
    {
        return ! (a == b);
    }

private:
    sprite_affine_mat_pa_register_hblank_effect_ptr _pa_hblank_effect_ptr;
    sprite_affine_mat_pb_register_hblank_effect_ptr _pb_hblank_effect_ptr;
    sprite_affine_mat_pc_register_hblank_effect_ptr _pc_hblank_effect_ptr;
    sprite_affine_mat_pd_register_hblank_effect_ptr _pd_hblank_effect_ptr;

    sprite_affine_mat_attributes_hblank_effect_ptr(
            sprite_affine_mat_pa_register_hblank_effect_ptr&& pa_hblank_effect_ptr,
            sprite_affine_mat_pb_register_hblank_effect_ptr&& pb_hblank_effect_ptr,
            sprite_affine_mat_pc_register_hblank_effect_ptr&& pc_hblank_effect_ptr,
            sprite_affine_mat_pd_register_hblank_effect_ptr&& pd_hblank_effect_ptr);
};


template<>
struct hash<sprite_affine_mat_attributes_hblank_effect_ptr>
{
    [[nodiscard]] unsigned operator()(const sprite_affine_mat_attributes_hblank_effect_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
