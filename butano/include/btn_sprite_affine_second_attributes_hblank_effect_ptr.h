#ifndef BTN_SPRITE_AFFINE_SECOND_ATTRIBUTES_HBLANK_EFFECT_PTR_H
#define BTN_SPRITE_AFFINE_SECOND_ATTRIBUTES_HBLANK_EFFECT_PTR_H

#include "btn_sprite_ptr.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class sprite_affine_second_attributes_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static sprite_affine_second_attributes_hblank_effect_ptr create(
            sprite_ptr sprite, const span<const sprite_affine_second_attributes>& attributes_ref);

    [[nodiscard]] static optional<sprite_affine_second_attributes_hblank_effect_ptr> create_optional(
            sprite_ptr sprite, const span<const sprite_affine_second_attributes>& attributes_ref);

    sprite_affine_second_attributes_hblank_effect_ptr(
            const sprite_affine_second_attributes_hblank_effect_ptr& other) = default;

    sprite_affine_second_attributes_hblank_effect_ptr& operator=(
            const sprite_affine_second_attributes_hblank_effect_ptr& other) = default;

    sprite_affine_second_attributes_hblank_effect_ptr(
            sprite_affine_second_attributes_hblank_effect_ptr&& other) noexcept;

    sprite_affine_second_attributes_hblank_effect_ptr& operator=(
            sprite_affine_second_attributes_hblank_effect_ptr&& other) noexcept;

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite;
    }

    [[nodiscard]] span<const sprite_affine_second_attributes> attributes_ref() const;

    void set_attributes_ref(const span<const sprite_affine_second_attributes>& attributes_ref);

    void reload_attributes_ref();

    void swap(sprite_affine_second_attributes_hblank_effect_ptr& other);

    friend void swap(sprite_affine_second_attributes_hblank_effect_ptr& a,
                     sprite_affine_second_attributes_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    sprite_ptr _sprite;

    sprite_affine_second_attributes_hblank_effect_ptr(int id, sprite_ptr&& sprite);
};

}

#endif
