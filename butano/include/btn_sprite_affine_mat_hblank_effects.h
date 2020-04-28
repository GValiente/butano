#ifndef BTN_SPRITE_AFFINE_MAT_HBLANK_EFFECTS_H
#define BTN_SPRITE_AFFINE_MAT_HBLANK_EFFECTS_H

#include "btn_hblank_effect_ptr.h"
#include "btn_sprite_affine_mat_ptr.h"

namespace btn
{

class sprite_affine_mat_first_register_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static sprite_affine_mat_first_register_hblank_effect_ptr create(
            sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref);

    [[nodiscard]] static optional<sprite_affine_mat_first_register_hblank_effect_ptr> optional_create(
            sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref);

    sprite_affine_mat_first_register_hblank_effect_ptr(
            const sprite_affine_mat_first_register_hblank_effect_ptr& other);

    sprite_affine_mat_first_register_hblank_effect_ptr& operator=(
            const sprite_affine_mat_first_register_hblank_effect_ptr& other);

    sprite_affine_mat_first_register_hblank_effect_ptr(
            sprite_affine_mat_first_register_hblank_effect_ptr&& other);

    sprite_affine_mat_first_register_hblank_effect_ptr& operator=(
            sprite_affine_mat_first_register_hblank_effect_ptr&& other);

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return _affine_mat_ptr;
    }

    [[nodiscard]] span<const int16_t> values_ref() const;

    void set_values_ref(const span<const int16_t>& values_ref);

    void reload_values_ref();

    void swap(sprite_affine_mat_first_register_hblank_effect_ptr& other);

    friend void swap(sprite_affine_mat_first_register_hblank_effect_ptr& a,
                     sprite_affine_mat_first_register_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    sprite_affine_mat_ptr _affine_mat_ptr;

    sprite_affine_mat_first_register_hblank_effect_ptr(int id, sprite_affine_mat_ptr&& affine_mat_ptr);
};


class sprite_affine_mat_second_register_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static sprite_affine_mat_second_register_hblank_effect_ptr create(
            sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref);

    [[nodiscard]] static optional<sprite_affine_mat_second_register_hblank_effect_ptr> optional_create(
            sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref);

    sprite_affine_mat_second_register_hblank_effect_ptr(
            const sprite_affine_mat_second_register_hblank_effect_ptr& other);

    sprite_affine_mat_second_register_hblank_effect_ptr& operator=(
            const sprite_affine_mat_second_register_hblank_effect_ptr& other);

    sprite_affine_mat_second_register_hblank_effect_ptr(
            sprite_affine_mat_second_register_hblank_effect_ptr&& other);

    sprite_affine_mat_second_register_hblank_effect_ptr& operator=(
            sprite_affine_mat_second_register_hblank_effect_ptr&& other);

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return _affine_mat_ptr;
    }

    [[nodiscard]] span<const int16_t> values_ref() const;

    void set_values_ref(const span<const int16_t>& values_ref);

    void reload_values_ref();

    void swap(sprite_affine_mat_second_register_hblank_effect_ptr& other);

    friend void swap(sprite_affine_mat_second_register_hblank_effect_ptr& a,
                     sprite_affine_mat_second_register_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    sprite_affine_mat_ptr _affine_mat_ptr;

    sprite_affine_mat_second_register_hblank_effect_ptr(int id, sprite_affine_mat_ptr&& affine_mat_ptr);
};


class sprite_affine_mat_third_register_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static sprite_affine_mat_third_register_hblank_effect_ptr create(
            sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref);

    [[nodiscard]] static optional<sprite_affine_mat_third_register_hblank_effect_ptr> optional_create(
            sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref);

    sprite_affine_mat_third_register_hblank_effect_ptr(
            const sprite_affine_mat_third_register_hblank_effect_ptr& other);

    sprite_affine_mat_third_register_hblank_effect_ptr& operator=(
            const sprite_affine_mat_third_register_hblank_effect_ptr& other);

    sprite_affine_mat_third_register_hblank_effect_ptr(
            sprite_affine_mat_third_register_hblank_effect_ptr&& other);

    sprite_affine_mat_third_register_hblank_effect_ptr& operator=(
            sprite_affine_mat_third_register_hblank_effect_ptr&& other);

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return _affine_mat_ptr;
    }

    [[nodiscard]] span<const int16_t> values_ref() const;

    void set_values_ref(const span<const int16_t>& values_ref);

    void reload_values_ref();

    void swap(sprite_affine_mat_third_register_hblank_effect_ptr& other);

    friend void swap(sprite_affine_mat_third_register_hblank_effect_ptr& a,
                     sprite_affine_mat_third_register_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    sprite_affine_mat_ptr _affine_mat_ptr;

    sprite_affine_mat_third_register_hblank_effect_ptr(int id, sprite_affine_mat_ptr&& affine_mat_ptr);
};


class sprite_affine_mat_fourth_register_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static sprite_affine_mat_fourth_register_hblank_effect_ptr create(
            sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref);

    [[nodiscard]] static optional<sprite_affine_mat_fourth_register_hblank_effect_ptr> optional_create(
            sprite_affine_mat_ptr affine_mat_ptr, const span<const int16_t>& values_ref);

    sprite_affine_mat_fourth_register_hblank_effect_ptr(
            const sprite_affine_mat_fourth_register_hblank_effect_ptr& other);

    sprite_affine_mat_fourth_register_hblank_effect_ptr& operator=(
            const sprite_affine_mat_fourth_register_hblank_effect_ptr& other);

    sprite_affine_mat_fourth_register_hblank_effect_ptr(
            sprite_affine_mat_fourth_register_hblank_effect_ptr&& other);

    sprite_affine_mat_fourth_register_hblank_effect_ptr& operator=(
            sprite_affine_mat_fourth_register_hblank_effect_ptr&& other);

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return _affine_mat_ptr;
    }

    [[nodiscard]] span<const int16_t> values_ref() const;

    void set_values_ref(const span<const int16_t>& values_ref);

    void reload_values_ref();

    void swap(sprite_affine_mat_fourth_register_hblank_effect_ptr& other);

    friend void swap(sprite_affine_mat_fourth_register_hblank_effect_ptr& a,
                     sprite_affine_mat_fourth_register_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    sprite_affine_mat_ptr _affine_mat_ptr;

    sprite_affine_mat_fourth_register_hblank_effect_ptr(int id, sprite_affine_mat_ptr&& affine_mat_ptr);
};

}

#endif
