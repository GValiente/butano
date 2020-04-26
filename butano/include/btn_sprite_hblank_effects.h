#ifndef BTN_SPRITE_HBLANK_EFFECTS_H
#define BTN_SPRITE_HBLANK_EFFECTS_H

#include "btn_sprite_ptr.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class sprite_regular_second_attributes_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static sprite_regular_second_attributes_hblank_effect_ptr create(
            sprite_ptr sprite_ptr, const span<const sprite_regular_second_attributes>& attributes_ref);

    [[nodiscard]] static optional<sprite_regular_second_attributes_hblank_effect_ptr> optional_create(
            sprite_ptr sprite_ptr, const span<const sprite_regular_second_attributes>& attributes_ref);

    sprite_regular_second_attributes_hblank_effect_ptr(
            const sprite_regular_second_attributes_hblank_effect_ptr& other);

    sprite_regular_second_attributes_hblank_effect_ptr& operator=(
            const sprite_regular_second_attributes_hblank_effect_ptr& other);

    sprite_regular_second_attributes_hblank_effect_ptr(
            sprite_regular_second_attributes_hblank_effect_ptr&& other);

    sprite_regular_second_attributes_hblank_effect_ptr& operator=(
            sprite_regular_second_attributes_hblank_effect_ptr&& other);

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite_ptr;
    }

    [[nodiscard]] span<const sprite_regular_second_attributes> attributes_ref() const;

    void set_attributes_ref(const span<const sprite_regular_second_attributes>& attributes_ref);

    void reload_attributes_ref();

    void swap(sprite_regular_second_attributes_hblank_effect_ptr& other);

    friend void swap(sprite_regular_second_attributes_hblank_effect_ptr& a,
                     sprite_regular_second_attributes_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    sprite_ptr _sprite_ptr;

    sprite_regular_second_attributes_hblank_effect_ptr(int id, sprite_ptr&& sprite_ptr);
};


class sprite_affine_second_attributes_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static sprite_affine_second_attributes_hblank_effect_ptr create(
            sprite_ptr sprite_ptr, const span<const sprite_affine_second_attributes>& attributes_ref);

    [[nodiscard]] static optional<sprite_affine_second_attributes_hblank_effect_ptr> optional_create(
            sprite_ptr sprite_ptr, const span<const sprite_affine_second_attributes>& attributes_ref);

    sprite_affine_second_attributes_hblank_effect_ptr(
            const sprite_affine_second_attributes_hblank_effect_ptr& other);

    sprite_affine_second_attributes_hblank_effect_ptr& operator=(
            const sprite_affine_second_attributes_hblank_effect_ptr& other);

    sprite_affine_second_attributes_hblank_effect_ptr(
            sprite_affine_second_attributes_hblank_effect_ptr&& other);

    sprite_affine_second_attributes_hblank_effect_ptr& operator=(
            sprite_affine_second_attributes_hblank_effect_ptr&& other);

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite_ptr;
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
    sprite_ptr _sprite_ptr;

    sprite_affine_second_attributes_hblank_effect_ptr(int id, sprite_ptr&& sprite_ptr);
};


class sprite_third_attributes_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static sprite_third_attributes_hblank_effect_ptr create(
            sprite_ptr sprite_ptr, const span<const sprite_third_attributes>& attributes_ref);

    [[nodiscard]] static optional<sprite_third_attributes_hblank_effect_ptr> optional_create(
            sprite_ptr sprite_ptr, const span<const sprite_third_attributes>& attributes_ref);

    sprite_third_attributes_hblank_effect_ptr(const sprite_third_attributes_hblank_effect_ptr& other);

    sprite_third_attributes_hblank_effect_ptr& operator=(const sprite_third_attributes_hblank_effect_ptr& other);

    sprite_third_attributes_hblank_effect_ptr(sprite_third_attributes_hblank_effect_ptr&& other);

    sprite_third_attributes_hblank_effect_ptr& operator=(sprite_third_attributes_hblank_effect_ptr&& other);

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite_ptr;
    }

    [[nodiscard]] span<const sprite_third_attributes> attributes_ref() const;

    void set_attributes_ref(const span<const sprite_third_attributes>& attributes_ref);

    void reload_attributes_ref();

    void swap(sprite_third_attributes_hblank_effect_ptr& other);

    friend void swap(sprite_third_attributes_hblank_effect_ptr& a, sprite_third_attributes_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    sprite_ptr _sprite_ptr;

    sprite_third_attributes_hblank_effect_ptr(int id, sprite_ptr&& sprite_ptr);
};

}

#endif
