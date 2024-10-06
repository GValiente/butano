/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CAMERA_PTR_H
#define BN_CAMERA_PTR_H

/**
 * @file
 * bn::camera_ptr header file.
 *
 * @ingroup camera
 */

#include "bn_optional.h"
#include "bn_fixed_point.h"

namespace bn
{

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a camera.
 *
 * Several camera_ptr objects may own the same camera.
 *
 * The camera is released when the last remaining camera_ptr owning it is destroyed.
 *
 * @ingroup camera
 */
class camera_ptr
{

public:
    /**
     * @brief Creates a camera_ptr.
     * @return The requested camera_ptr.
     */
    [[nodiscard]] static camera_ptr create();

    /**
     * @brief Creates a camera_ptr.
     * @param x Horizontal position of the camera.
     * @param y Vertical position of the camera.
     * @return The requested camera_ptr.
     */
    [[nodiscard]] static camera_ptr create(fixed x, fixed y);

    /**
     * @brief Creates a camera_ptr.
     * @param position Position of the camera.
     * @return The requested camera_ptr.
     */
    [[nodiscard]] static camera_ptr create(const fixed_point& position);

    /**
     * @brief Creates a camera_ptr.
     * @return The requested camera_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<camera_ptr> create_optional();

    /**
     * @brief Creates a camera_ptr.
     * @param x Horizontal position of the camera.
     * @param y Vertical position of the camera.
     * @return The requested camera_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<camera_ptr> create_optional(fixed x, fixed y);

    /**
     * @brief Creates a camera_ptr.
     * @param position Position of the camera.
     * @return The requested camera_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<camera_ptr> create_optional(const fixed_point& position);

    /**
     * @brief Copy constructor.
     * @param other camera_ptr to copy.
     */
    camera_ptr(const camera_ptr& other);

    /**
     * @brief Copy assignment operator.
     * @param other camera_ptr to copy.
     * @return Reference to this.
     */
    camera_ptr& operator=(const camera_ptr& other);

    /**
     * @brief Move constructor.
     * @param other camera_ptr to move.
     */
    camera_ptr(camera_ptr&& other) noexcept :
        camera_ptr(other._id)
    {
        other._id = -1;
    }

    /**
     * @brief Move assignment operator.
     * @param other camera_ptr to move.
     * @return Reference to this.
     */
    camera_ptr& operator=(camera_ptr&& other) noexcept
    {
        bn::swap(_id, other._id);
        return *this;
    }

    /**
     * @brief Releases the referenced camera if no more camera_ptr objects reference to it.
     */
    ~camera_ptr();

    /**
     * @brief Returns the internal id.
     */
    [[nodiscard]] int id() const
    {
        return _id;
    }

    /**
     * @brief Returns the horizontal position of the camera.
     */
    [[nodiscard]] fixed x() const;

    /**
     * @brief Sets the horizontal position of the camera.
     */
    void set_x(fixed x);

    /**
     * @brief Returns the vertical position of the camera.
     */
    [[nodiscard]] fixed y() const;

    /**
     * @brief Sets the vertical position of the camera.
     */
    void set_y(fixed y);

    /**
     * @brief Returns the position of the camera.
     */
    [[nodiscard]] const fixed_point& position() const;

    /**
     * @brief Sets the position of the camera.
     * @param x Horizontal position of the camera.
     * @param y Vertical position of the camera.
     */
    void set_position(fixed x, fixed y);

    /**
     * @brief Sets the position of the camera.
     */
    void set_position(const fixed_point& position);

    /**
     * @brief Exchanges the contents of this camera_ptr with those of the other one.
     * @param other camera_ptr to exchange the contents with.
     */
    void swap(camera_ptr& other)
    {
        bn::swap(_id, other._id);
    }

    /**
     * @brief Exchanges the contents of a camera_ptr with those of another one.
     * @param a First camera_ptr to exchange the contents with.
     * @param b Second camera_ptr to exchange the contents with.
     */
    friend void swap(camera_ptr& a, camera_ptr& b)
    {
        bn::swap(a._id, b._id);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const camera_ptr& a, const camera_ptr& b) = default;

private:
    int8_t _id;

    explicit camera_ptr(int id) :
        _id(int8_t(id))
    {
    }
};


/**
 * @brief Hash support for camera_ptr.
 *
 * @ingroup camera
 * @ingroup functional
 */
template<>
struct hash<camera_ptr>
{
    /**
     * @brief Returns the hash of the given camera_ptr.
     */
    [[nodiscard]] unsigned operator()(const camera_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
