/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_REGULAR_BG_BUILDER_H
#define BN_REGULAR_BG_BUILDER_H

/**
 * @file
 * bn::regular_bg_builder header file.
 *
 * @ingroup regular_bg
 */

#include "bn_camera_ptr.h"
#include "bn_green_swap_mode.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_ptr.h"

namespace bn
{

class regular_bg_ptr;

/**
 * @brief Creates regular_bg_map_ptr objects with custom attributes.
 *
 * If some of the attributes of the regular background to create differs from the default ones,
 * using this class improves performance.
 *
 * @ingroup regular_bg
 */
class regular_bg_builder
{

public:
    /**
     * @brief Constructor.
     * @param item regular_bg_item containing the required information to generate regular backgrounds.
     */
    explicit regular_bg_builder(const regular_bg_item& item) :
        _item(item)
    {
    }

    /**
     * @brief Constructor.
     * @param item regular_bg_item containing the required information to generate regular backgrounds.
     * @param map_index Index of the map to reference in item.map_item() to generate regular backgrounds.
     */
    regular_bg_builder(const regular_bg_item& item, int map_index);

    /**
     * @brief Constructor.
     * @param map regular_bg_map_ptr to copy for generating regular backgrounds.
     */
    explicit regular_bg_builder(const regular_bg_map_ptr& map) :
        _map(map)
    {
    }

    /**
     * @brief Constructor.
     * @param map regular_bg_map_ptr to move for generating regular backgrounds.
     */
    explicit regular_bg_builder(regular_bg_map_ptr&& map) :
        _map(move(map))
    {
    }

    /**
     * @brief Returns the regular_bg_item containing the required information to generate regular backgrounds
     * if it has one; bn::nullopt otherwise.
     */
    [[nodiscard]] const optional<regular_bg_item>& item() const
    {
        return _item;
    }

    /**
     * @brief Index of the map to reference in item.map_item() to generate regular backgrounds.
     */
    [[nodiscard]] int map_index() const
    {
        return _map_index;
    }

    /**
     * @brief Returns the horizontal position of the regular backgrounds to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed x() const
    {
        return _position.x();
    }

    /**
     * @brief Sets the horizontal position of the regular backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param x Horizontal position of the regular backgrounds to generate.
     * @return Reference to this.
     */
    regular_bg_builder& set_x(fixed x)
    {
        _position.set_x(x);
        return *this;
    }

    /**
     * @brief Returns the vertical position of the regular backgrounds to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed y() const
    {
        return _position.y();
    }

    /**
     * @brief Sets the vertical position of the regular backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param y vertical position of the regular backgrounds to generate.
     * @return Reference to this.
     */
    regular_bg_builder& set_y(fixed y)
    {
        _position.set_y(y);
        return *this;
    }

    /**
     * @brief Returns the position of the regular backgrounds to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] const fixed_point& position() const
    {
        return _position;
    }

    /**
     * @brief Sets the position of the regular backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param x Horizontal position of the regular backgrounds to generate.
     * @param y Vertical position of the regular backgrounds to generate.
     * @return Reference to this.
     */
    regular_bg_builder& set_position(fixed x, fixed y)
    {
        _position = fixed_point(x, y);
        return *this;
    }

    /**
     * @brief Sets the position of the regular backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param position Position of the regular backgrounds to generate.
     * @return Reference to this.
     */
    regular_bg_builder& set_position(const fixed_point& position)
    {
        _position = position;
        return *this;
    }

    /**
     * @brief Returns the horizontal top-left position of the regular backgrounds to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed top_left_x() const;

    /**
     * @brief Sets the horizontal top-left position of the regular backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param top_left_x Horizontal top-left position of the regular backgrounds to generate.
     * @return Reference to this.
     */
    regular_bg_builder& set_top_left_x(fixed top_left_x);

    /**
     * @brief Returns the vertical top-left position of the regular backgrounds to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed top_left_y() const;

    /**
     * @brief Sets the vertical top-left position of the regular backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param top_left_y Vertical top-left position of the regular backgrounds to generate.
     * @return Reference to this.
     */
    regular_bg_builder& set_top_left_y(fixed top_left_y);

    /**
     * @brief Returns the top-left position of the regular backgrounds to generate
     * (relative to their camera, if they are going to have one).
     */
    [[nodiscard]] fixed_point top_left_position() const;

    /**
     * @brief Sets the top-left position of the regular backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param top_left_x Horizontal top-left position of the regular backgrounds to generate.
     * @param top_left_y Vertical top-left position of the regular backgrounds to generate.
     * @return Reference to this.
     */
    regular_bg_builder& set_top_left_position(fixed top_left_x, fixed top_left_y);

    /**
     * @brief Sets the top-left position of the regular backgrounds to generate
     * (relative to their camera, if they are going to have one).
     * @param top_left_position Top-left position of the regular backgrounds to generate.
     * @return Reference to this.
     */
    regular_bg_builder& set_top_left_position(const fixed_point& top_left_position);

    /**
     * @brief Returns the priority of the regular backgrounds to generate relative to sprites and other backgrounds.
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     */
    [[nodiscard]] int priority() const
    {
        return _priority;
    }

    /**
     * @brief Sets the priority of the regular backgrounds to generate relative to sprites and other backgrounds.
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     *
     * @param priority Priority in the range [0..3].
     * @return Reference to this.
     */
    regular_bg_builder& set_priority(int priority);

    /**
     * @brief Returns the priority of the regular backgrounds to generate relative to other backgrounds,
     * excluding sprites.
     *
     * Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
     *
     * Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
     */
    [[nodiscard]] int z_order() const
    {
        return _z_order;
    }

    /**
     * @brief Sets the priority of the regular backgrounds to generate relative to other backgrounds,
     * excluding sprites.
     *
     * Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
     *
     * Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
     *
     * @param z_order Priority relative to other backgrounds, excluding sprites, in the range [-32767..32767].
     * @return Reference to this.
     */
    regular_bg_builder& set_z_order(int z_order);

    /**
     * @brief Indicates if the mosaic effect must be applied to the regular backgrounds to generate or not.
     */
    [[nodiscard]] bool mosaic_enabled() const
    {
        return _mosaic_enabled;
    }

    /**
     * @brief Sets if the mosaic effect must be applied to the regular backgrounds to generate or not.
     * @param mosaic_enabled `true` if the mosaic effect must be applied; `false` otherwise.
     * @return Reference to this.
     */
    regular_bg_builder& set_mosaic_enabled(bool mosaic_enabled)
    {
        _mosaic_enabled = mosaic_enabled;
        return *this;
    }

    /**
     * @brief Indicates if blending must be applied to the regular backgrounds to generate or not.
     *
     * Blending is applied to regular backgrounds by making them part of the blending top layer.
     */
    [[nodiscard]] bool blending_enabled() const
    {
        return blending_top_enabled();
    }

    /**
     * @brief Sets if blending must be applied to the regular backgrounds to generate or not.
     *
     * Blending is applied to regular backgrounds by making them part of the blending top layer.
     * @param blending_enabled `true` if blending must be applied; `false` otherwise.
     * @return Reference to this.
     */
    regular_bg_builder& set_blending_enabled(bool blending_enabled)
    {
        set_blending_top_enabled(blending_enabled);
        return *this;
    }

    /**
     * @brief Indicates if the regular backgrounds to generate must be part of the blending top layer or not.
     */
    [[nodiscard]] bool blending_top_enabled() const
    {
        return _blending_top_enabled;
    }

    /**
     * @brief Sets if the regular backgrounds to generate must be part of the blending top layer or not.
     * @param blending_top_enabled `true` if generated backgrounds must be part of the blending top layer;
     * `false` otherwise.
     * @return Reference to this.
     */
    regular_bg_builder& set_blending_top_enabled(bool blending_top_enabled)
    {
        _blending_top_enabled = blending_top_enabled;
        return *this;
    }

    /**
     * @brief Indicates if the regular backgrounds to generate must be part of the blending bottom layer or not.
     */
    [[nodiscard]] bool blending_bottom_enabled() const
    {
        return _blending_bottom_enabled;
    }

    /**
     * @brief Sets if the regular backgrounds to generate must be part of the blending bottom layer or not.
     * @param blending_bottom_enabled `true` if generated backgrounds must be part of the blending bottom layer;
     * `false` otherwise.
     * @return Reference to this.
     */
    regular_bg_builder& set_blending_bottom_enabled(bool blending_bottom_enabled)
    {
        _blending_bottom_enabled = blending_bottom_enabled;
        return *this;
    }

    /**
     * @brief Indicates how the regular backgrounds to generate must be displayed when green swap is enabled.
     */
    [[nodiscard]] bn::green_swap_mode green_swap_mode() const
    {
        return _green_swap_mode;
    }

    /**
     * @brief Sets how the regular backgrounds to generate must be displayed when green swap is enabled.
     * @param green_swap_mode Green swap mode.
     * @return Reference to this.
     */
    regular_bg_builder& set_green_swap_mode(bn::green_swap_mode green_swap_mode)
    {
        _green_swap_mode = green_swap_mode;
        return *this;
    }

    /**
     * @brief Indicates if the regular backgrounds to generate must be committed to the GBA or not.
     */
    [[nodiscard]] bool visible() const
    {
        return _visible;
    }

    /**
     * @brief Sets if the regular backgrounds to generate must be committed to the GBA or not.
     * @param visible `true` if the regular backgrounds must be committed to the GBA; `false` otherwise.
     * @return Reference to this.
     */
    regular_bg_builder& set_visible(bool visible)
    {
        _visible = visible;
        return *this;
    }

    /**
     * @brief Returns the camera_ptr to attach to the regular backgrounds to generate (if any).
     */
    [[nodiscard]] const optional<camera_ptr>& camera() const
    {
        return _camera;
    }

    /**
     * @brief Sets the camera_ptr to attach to the regular backgrounds to generate.
     * @param camera camera_ptr to copy to the builder.
     * @return Reference to this.
     */
    regular_bg_builder& set_camera(const camera_ptr& camera)
    {
        _camera = camera;
        return *this;
    }

    /**
     * @brief Sets the camera_ptr to attach to the regular backgrounds to generate.
     * @param camera camera_ptr to move to the builder.
     * @return Reference to this.
     */
    regular_bg_builder& set_camera(camera_ptr&& camera)
    {
        _camera = move(camera);
        return *this;
    }

    /**
     * @brief Sets or removes the camera_ptr to attach to the regular backgrounds to generate.
     * @param camera Optional camera_ptr to copy to the builder.
     * @return Reference to this.
     */
    regular_bg_builder& set_camera(const optional<camera_ptr>& camera)
    {
        _camera = camera;
        return *this;
    }

    /**
     * @brief Sets or removes the camera_ptr to attach to the regular backgrounds to generate.
     * @param camera Optional camera_ptr to move to the builder.
     * @return Reference to this.
     */
    regular_bg_builder& set_camera(optional<camera_ptr>&& camera)
    {
        _camera = move(camera);
        return *this;
    }

    /**
     * @brief Removes the camera_ptr to attach to the regular backgrounds to generate.
     * @return Reference to this.
     */
    regular_bg_builder& remove_camera()
    {
        _camera.reset();
        return *this;
    }

    /**
     * @brief Releases and returns the camera_ptr to attach to the regular backgrounds to generate (if any).
     */
    [[nodiscard]] optional<camera_ptr> release_camera();

    /**
     * @brief Generates and returns a regular_bg_ptr without releasing the acquired resources.
     */
    [[nodiscard]] regular_bg_ptr build() const;

    /**
     * @brief Generates and returns a regular_bg_ptr releasing the acquired resources.
     *
     * regular_bg_ptr generation after calling this method may stop working.
     */
    [[nodiscard]] regular_bg_ptr release_build();

    /**
     * @brief Generates and returns a regular_bg_ptr
     * without releasing the acquired resources if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_ptr> build_optional() const;

    /**
     * @brief Generates and returns a regular_bg_ptr releasing the acquired resources if it could be allocated;
     * bn::nullopt otherwise.
     *
     * regular_bg_ptr generation after calling this method may stop working.
     */
    [[nodiscard]] optional<regular_bg_ptr> release_build_optional();

    /**
     * @brief Generates and returns a regular_bg_map_ptr without releasing the acquired resources.
     */
    [[nodiscard]] regular_bg_map_ptr map() const;

    /**
     * @brief Generates and returns a regular_bg_map_ptr without releasing the acquired resources
     * if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<regular_bg_map_ptr> map_optional() const;

    /**
     * @brief Generates and returns a regular_bg_map_ptr releasing the acquired resources.
     *
     * regular_bg_ptr generation after calling this method may stop working.
     */
    [[nodiscard]] regular_bg_map_ptr release_map();

    /**
     * @brief Generates and returns a regular_bg_map_ptr releasing the acquired resources
     * if it could be allocated; bn::nullopt otherwise.
     *
     * regular_bg_ptr generation after calling this method may stop working.
     */
    [[nodiscard]] optional<regular_bg_map_ptr> release_map_optional();

private:
    optional<regular_bg_item> _item;
    fixed_point _position;
    int _map_index = 0;
    int _z_order = 0;
    optional<regular_bg_map_ptr> _map;
    optional<camera_ptr> _camera;
    bn::green_swap_mode _green_swap_mode = green_swap_mode::DEFAULT;
    uint8_t _priority = 3;
    bool _mosaic_enabled = false;
    bool _blending_top_enabled = false;
    bool _blending_bottom_enabled = true;
    bool _visible = true;

    [[nodiscard]] size _dimensions() const;
};

}

#endif

