/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_AFFINE_MAT_PTR_H
#define BN_SPRITE_AFFINE_MAT_PTR_H

/**
 * @file
 * bn::sprite_affine_mat_ptr header file.
 *
 * @ingroup sprite
 * @ingroup affine_mat
 */

#include "bn_fixed.h"
#include "bn_optional.h"

namespace bn
{

class affine_mat_attributes;

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a sprite affine transformation matrix.
 *
 * Several sprite_affine_mat_ptr objects may own the same sprite affine transformation matrix.
 *
 * The sprite affine transformation matrix is released
 * when the last remaining sprite_affine_mat_ptr owning it is destroyed.
 *
 * @ingroup sprite
 * @ingroup affine_mat
 */
class sprite_affine_mat_ptr
{

public:
    /**
     * @brief Creates an identity affine transformation matrix.
     * @return The requested sprite_affine_mat_ptr.
     */
    [[nodiscard]] static sprite_affine_mat_ptr create();

    /**
     * @brief Creates an affine transformation matrix with the specified attributes.
     * @param attributes affine_mat_attributes of the output matrix.
     * @return The requested sprite_affine_mat_ptr.
     */
    [[nodiscard]] static sprite_affine_mat_ptr create(const affine_mat_attributes& attributes);

    /**
     * @brief Creates an identity affine transformation matrix.
     * @return The requested sprite_affine_mat_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<sprite_affine_mat_ptr> create_optional();

    /**
     * @brief Creates an affine transformation matrix with the specified attributes.
     * @param attributes affine_mat_attributes of the output matrix.
     * @return The requested sprite_affine_mat_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<sprite_affine_mat_ptr> create_optional(const affine_mat_attributes& attributes);

    /**
     * @brief Copy constructor.
     * @param other sprite_affine_mat_ptr to copy.
     */
    sprite_affine_mat_ptr(const sprite_affine_mat_ptr& other);

    /**
     * @brief Copy assignment operator.
     * @param other sprite_affine_mat_ptr to copy.
     * @return Reference to this.
     */
    sprite_affine_mat_ptr& operator=(const sprite_affine_mat_ptr& other);

    /**
     * @brief Move constructor.
     * @param other sprite_affine_mat_ptr to move.
     */
    sprite_affine_mat_ptr(sprite_affine_mat_ptr&& other) noexcept :
        sprite_affine_mat_ptr(other._id)
    {
        other._id = -1;
    }

    /**
     * @brief Move assignment operator.
     * @param other sprite_affine_mat_ptr to move.
     * @return Reference to this.
     */
    sprite_affine_mat_ptr& operator=(sprite_affine_mat_ptr&& other) noexcept
    {
        bn::swap(_id, other._id);
        return *this;
    }

    /**
     * @brief Releases the referenced sprite affine transformation matrix
     * if no more sprite_affine_mat_ptr objects reference to it.
     */
    ~sprite_affine_mat_ptr();

    /**
     * @brief Returns the internal id.
     */
    [[nodiscard]] int id() const
    {
        return _id;
    }

    /**
     * @brief Returns the rotation angle in degrees.
     */
    [[nodiscard]] fixed rotation_angle() const;

    /**
     * @brief Sets the rotation angle in degrees.
     * @param rotation_angle Rotation angle in degrees, in the range [0..360].
     */
    void set_rotation_angle(fixed rotation_angle);

    /**
     * @brief Sets the rotation angle in degrees.
     * @param rotation_angle Rotation angle in degrees, in any range.
     */
    void set_rotation_angle_safe(fixed rotation_angle);

    /**
     * @brief Returns the horizontal scale.
     */
    [[nodiscard]] fixed horizontal_scale() const;

    /**
     * @brief Sets the horizontal scale.
     */
    void set_horizontal_scale(fixed horizontal_scale);

    /**
     * @brief Returns the vertical scale.
     */
    [[nodiscard]] fixed vertical_scale() const;

    /**
     * @brief Sets the vertical scale.
     */
    void set_vertical_scale(fixed vertical_scale);

    /**
     * @brief Sets the scale.
     */
    void set_scale(fixed scale);

    /**
     * @brief Sets the scale.
     * @param horizontal_scale Horizontal scale.
     * @param vertical_scale Vertical scale.
     */
    void set_scale(fixed horizontal_scale, fixed vertical_scale);

    /**
     * @brief Returns the horizontal shear.
     */
    [[nodiscard]] fixed horizontal_shear() const;

    /**
     * @brief Sets the horizontal shear.
     */
    void set_horizontal_shear(fixed horizontal_shear);

    /**
     * @brief Returns the vertical shear.
     */
    [[nodiscard]] fixed vertical_shear() const;

    /**
     * @brief Sets the vertical shear.
     */
    void set_vertical_shear(fixed vertical_shear);

    /**
     * @brief Sets the shear.
     */
    void set_shear(fixed shear);

    /**
     * @brief Sets the shear.
     * @param horizontal_shear Horizontal shear.
     * @param vertical_shear Vertical shear.
     */
    void set_shear(fixed horizontal_shear, fixed vertical_shear);

    /**
     * @brief Indicates if this matrix is flipped in the horizontal axis or not.
     */
    [[nodiscard]] bool horizontal_flip() const;

    /**
     * @brief Sets if this matrix is flipped in the horizontal axis or not.
     */
    void set_horizontal_flip(bool horizontal_flip);

    /**
     * @brief Indicates if this matrix is flipped in the vertical axis or not.
     */
    [[nodiscard]] bool vertical_flip() const;

    /**
     * @brief Sets if this matrix is flipped in the vertical axis or not.
     */
    void set_vertical_flip(bool vertical_flip);

    /**
     * @brief Returns the attributes of this matrix.
     */
    [[nodiscard]] const affine_mat_attributes& attributes() const;

    /**
     * @brief Sets the attributes of this matrix.
     */
    void set_attributes(const affine_mat_attributes& attributes);

    /**
     * @brief Indicates if this matrix is equal to the identity matrix or not.
     */
    [[nodiscard]] bool identity() const;

    /**
     * @brief Indicates if this matrix is equal to the identity matrix or not, ignoring flip attributes.
     */
    [[nodiscard]] bool flipped_identity() const;

    /**
     * @brief Exchanges the contents of this sprite_affine_mat_ptr with those of the other one.
     * @param other sprite_affine_mat_ptr to exchange the contents with.
     */
    void swap(sprite_affine_mat_ptr& other)
    {
        bn::swap(_id, other._id);
    }

    /**
     * @brief Exchanges the contents of a sprite_affine_mat_ptr with those of another one.
     * @param a First sprite_affine_mat_ptr to exchange the contents with.
     * @param b Second sprite_affine_mat_ptr to exchange the contents with.
     */
    friend void swap(sprite_affine_mat_ptr& a, sprite_affine_mat_ptr& b)
    {
        bn::swap(a._id, b._id);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const sprite_affine_mat_ptr& a, const sprite_affine_mat_ptr& b) = default;

private:
    int8_t _id;

    explicit sprite_affine_mat_ptr(int id) :
        _id(int8_t(id))
    {
    }
};


/**
 * @brief Hash support for sprite_affine_mat_ptr.
 *
 * @ingroup sprite
 * @ingroup affine_mat
 * @ingroup functional
 */
template<>
struct hash<sprite_affine_mat_ptr>
{
    /**
     * @brief Returns the hash of the given sprite_affine_mat_ptr.
     */
    [[nodiscard]] unsigned operator()(const sprite_affine_mat_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
