/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_REGULAR_BG_PTR_H
#define BN_REGULAR_BG_PTR_H

/**
 * @file
 * bn::regular_bg_ptr header file.
 *
 * @ingroup regular_bg
 */

#include "bn_optional.h"
#include "bn_fixed_point.h"

namespace bn
{

class size;
class window;
class camera_ptr;
class bg_palette_ptr;
class bg_palette_item;
class regular_bg_item;
class regular_bg_builder;
class regular_bg_map_ptr;
class regular_bg_map_item;
class regular_bg_tiles_ptr;
class regular_bg_tiles_item;
class regular_bg_attributes;

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a regular background.
 *
 * Several regular_bg_ptr objects may own the same regular background.
 *
 * The regular background is released when the last remaining regular_bg_ptr owning it is destroyed.
 *
 * @ingroup regular_bg
 */
class regular_bg_ptr
{

public:
    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_item.
     * @param item regular_bg_item containing the required information to generate the regular background.
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] static regular_bg_ptr create(const regular_bg_item& item);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_item.
     * @param item regular_bg_item containing the required information to generate the regular background.
     * @param map_index Index of the map to reference in item.map_item().
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] static regular_bg_ptr create(const regular_bg_item& item, int map_index);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_map_ptr.
     * @param map regular_bg_map_ptr used by the regular background.
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] static regular_bg_ptr create(regular_bg_map_ptr map);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_item.
     * @param x Horizontal position of the regular background.
     * @param y Vertical position of the regular background.
     * @param item regular_bg_item containing the required information to generate the regular background.
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] static regular_bg_ptr create(fixed x, fixed y, const regular_bg_item& item);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_item.
     * @param x Horizontal position of the regular background.
     * @param y Vertical position of the regular background.
     * @param item regular_bg_item containing the required information to generate the regular background.
     * @param map_index Index of the map to reference in item.map_item().
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] static regular_bg_ptr create(fixed x, fixed y, const regular_bg_item& item, int map_index);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_item.
     * @param position Position of the regular background.
     * @param item regular_bg_item containing the required information to generate the regular background.
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] static regular_bg_ptr create(const fixed_point& position, const regular_bg_item& item);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_item.
     * @param position Position of the regular background.
     * @param item regular_bg_item containing the required information to generate the regular background.
     * @param map_index Index of the map to reference in item.map_item().
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] static regular_bg_ptr create(
            const fixed_point& position, const regular_bg_item& item, int map_index);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_map_ptr.
     * @param x Horizontal position of the regular background.
     * @param y Vertical position of the regular background.
     * @param map regular_bg_map_ptr used by the regular background.
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] static regular_bg_ptr create(fixed x, fixed y, regular_bg_map_ptr map);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_map_ptr.
     * @param position Position of the regular background.
     * @param map regular_bg_map_ptr used by the regular background.
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] static regular_bg_ptr create(const fixed_point& position, regular_bg_map_ptr map);

    /**
     * @brief Creates a regular_bg_ptr from a regular_bg_builder reference.
     * @param builder regular_bg_builder reference.
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] static regular_bg_ptr create(const regular_bg_builder& builder);

    /**
     * @brief Creates a regular_bg_ptr from a moved regular_bg_builder.
     * @param builder regular_bg_builder to move.
     * @return The requested regular_bg_ptr.
     */
    [[nodiscard]] static regular_bg_ptr create(regular_bg_builder&& builder);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_item.
     * @param item regular_bg_item containing the required information to generate the regular background.
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_ptr> create_optional(const regular_bg_item& item);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_item.
     * @param item regular_bg_item containing the required information to generate the regular background.
     * @param map_index Index of the map to reference in item.map_item().
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_ptr> create_optional(const regular_bg_item& item, int map_index);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_map_ptr.
     * @param map regular_bg_map_ptr used by the regular background.
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_ptr> create_optional(regular_bg_map_ptr map);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_item.
     * @param x Horizontal position of the regular background.
     * @param y Vertical position of the regular background.
     * @param item regular_bg_item containing the required information to generate the regular background.
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_ptr> create_optional(fixed x, fixed y, const regular_bg_item& item);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_item.
     * @param x Horizontal position of the regular background.
     * @param y Vertical position of the regular background.
     * @param item regular_bg_item containing the required information to generate the regular background.
     * @param map_index Index of the map to reference in item.map_item().
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_ptr> create_optional(
            fixed x, fixed y, const regular_bg_item& item, int map_index);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_item.
     * @param position Position of the regular background.
     * @param item regular_bg_item containing the required information to generate the regular background.
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_ptr> create_optional(
            const fixed_point& position, const regular_bg_item& item);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_item.
     * @param position Position of the regular background.
     * @param item regular_bg_item containing the required information to generate the regular background.
     * @param map_index Index of the map to reference in item.map_item().
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_ptr> create_optional(
            const fixed_point& position, const regular_bg_item& item, int map_index);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_map_ptr.
     * @param x Horizontal position of the regular background.
     * @param y Vertical position of the regular background.
     * @param map regular_bg_map_ptr used by the regular background.
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_ptr> create_optional(fixed x, fixed y, regular_bg_map_ptr map);

    /**
     * @brief Creates a regular_bg_ptr from the given regular_bg_map_ptr.
     * @param position Position of the regular background.
     * @param map regular_bg_map_ptr used by the regular background.
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_ptr> create_optional(
            const fixed_point& position, regular_bg_map_ptr map);

    /**
     * @brief Creates a regular_bg_ptr from a regular_bg_builder reference.
     * @param builder regular_bg_builder reference.
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_ptr> create_optional(const regular_bg_builder& builder);

    /**
     * @brief Creates a regular_bg_ptr from a moved regular_bg_builder.
     * @param builder regular_bg_builder to move.
     * @return The requested regular_bg_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_ptr> create_optional(regular_bg_builder&& builder);

    /**
     * @brief Copy constructor.
     * @param other regular_bg_ptr to copy.
     */
    regular_bg_ptr(const regular_bg_ptr& other);

    /**
     * @brief Copy assignment operator.
     * @param other regular_bg_ptr to copy.
     * @return Reference to this.
     */
    regular_bg_ptr& operator=(const regular_bg_ptr& other);

    /**
     * @brief Move constructor.
     * @param other regular_bg_ptr to move.
     */
    regular_bg_ptr(regular_bg_ptr&& other) noexcept :
        regular_bg_ptr(other._handle)
    {
        other._handle = nullptr;
    }

    /**
     * @brief Move assignment operator.
     * @param other regular_bg_ptr to move.
     * @return Reference to this.
     */
    regular_bg_ptr& operator=(regular_bg_ptr&& other) noexcept
    {
        bn::swap(_handle, other._handle);
        return *this;
    }

    /**
     * @brief Releases the referenced regular background if no more regular_bg_ptr objects reference to it.
     */
    ~regular_bg_ptr();

    /**
     * @brief Returns the size in pixels of the regular background.
     */
    [[nodiscard]] size dimensions() const;

    /**
     * @brief Indicates if this regular background is big or not.
     *
     * Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
     */
    [[nodiscard]] bool big() const;

    /**
     * @brief Returns the tiles used by this regular background.
     */
    [[nodiscard]] const regular_bg_tiles_ptr& tiles() const;

    /**
     * @brief Sets the tiles used by this regular background.
     * @param tiles regular_bg_tiles_ptr to copy.
     *
     * It must be compatible with the current map of the regular background.
     */
    void set_tiles(const regular_bg_tiles_ptr& tiles);

    /**
     * @brief Sets the tiles used by this regular background.
     * @param tiles regular_bg_tiles_ptr to move.
     *
     * It must be compatible with the current map of the regular background.
     */
    void set_tiles(regular_bg_tiles_ptr&& tiles);

    /**
     * @brief Replaces the tiles used by this regular background
     * with a new tile set created with the given regular_bg_tiles_item.
     *
     * Before creating a new background tile set,
     * the regular_bg_tiles_ptr used by this regular background is removed, so VRAM usage is reduced.
     *
     * The new background tiles must be compatible with the current map of the regular background.
     *
     * @param tiles_item It creates the new background tiles to use by this regular background.
     */
    void set_tiles(const regular_bg_tiles_item& tiles_item);

    /**
     * @brief Returns the color palette used by this regular background.
     */
    [[nodiscard]] const bg_palette_ptr& palette() const;

    /**
     * @brief Sets the color palette to use by this regular background.
     * @param palette bg_palette_ptr to copy.
     *
     * It must be compatible with the current map of the regular background.
     */
    void set_palette(const bg_palette_ptr& palette);

    /**
     * @brief Sets the color palette to use by this regular background.
     * @param palette bg_palette_ptr to move.
     *
     * It must be compatible with the current map of the regular background.
     */
    void set_palette(bg_palette_ptr&& palette);

    /**
     * @brief Replaces the color palette used by this regular background
     * with a new one created with the given bg_palette_item.
     *
     * Before creating a new color palette, the bg_palette_ptr used by this regular background is removed,
     * so VRAM usage is reduced.
     *
     * The new color palette must be compatible with the current map of the regular background.
     *
     * @param palette_item It creates the color palette to use by this regular background.
     */
    void set_palette(const bg_palette_item& palette_item);

    /**
     * @brief Sets the tiles and the color palette to use by this regular background.
     *
     * The given parameters must be compatible with the current map of the regular background.
     *
     * @param tiles regular_bg_tiles_ptr to set.
     * @param palette bg_palette_ptr to set.
     */
    void set_tiles_and_palette(regular_bg_tiles_ptr tiles, bg_palette_ptr palette);

    /**
     * @brief Replaces the tiles and the color palette used by this regular background
     * with the created with the given regular_bg_tiles_item and bg_palette_item.
     *
     * Before creating new resources, the resources used by this regular background are removed,
     * so VRAM usage is reduced.
     *
     * @param tiles_item It creates the new background tiles to use by this regular background.
     * @param palette_item It creates the color palette to use by this regular background.
     */
    void set_tiles_and_palette(const regular_bg_tiles_item& tiles_item, const bg_palette_item& palette_item);

    /**
     * @brief Returns the map used by this regular background.
     */
    [[nodiscard]] const regular_bg_map_ptr& map() const;

    /**
     * @brief Sets the map used by this regular background.
     * @param map regular_bg_map_ptr to copy.
     */
    void set_map(const regular_bg_map_ptr& map);

    /**
     * @brief Sets the map used by this regular background.
     * @param map regular_bg_map_ptr to move.
     */
    void set_map(regular_bg_map_ptr&& map);

    /**
     * @brief Replaces the map used by this regular background
     * with a new map created with the given regular_bg_map_item.
     *
     * Before creating a new map, the regular_bg_map_ptr used by this regular background is removed,
     * so VRAM usage is reduced.
     *
     * @param map_item It creates the new map to use by this regular background.
     */
    void set_map(const regular_bg_map_item& map_item);

    /**
     * @brief Replaces the map used by this regular background
     * with a new map created with the given regular_bg_map_item.
     *
     * Before creating a new map, the regular_bg_map_ptr used by this regular background is removed,
     * so VRAM usage is reduced.
     *
     * @param map_item It creates the new map to use by this regular background.
     * @param map_index Index of the map to reference in map_item.
     */
    void set_map(const regular_bg_map_item& map_item, int map_index);

    /**
     * @brief Replaces the tiles, the color palette and the map used by this regular background
     * with the created with the given regular_bg_item.
     *
     * Before creating new resources, the resources used by this regular background are removed,
     * so VRAM usage is reduced.
     *
     * @param item It creates the resources to use by this regular background.
     */
    void set_item(const regular_bg_item& item);

    /**
     * @brief Replaces the tiles, the color palette and the map used by this regular background
     * with the created with the given regular_bg_item.
     *
     * Before creating new resources, the resources used by this regular background are removed,
     * so VRAM usage is reduced.
     *
     * @param item It creates the resources to use by this regular background.
     * @param map_index Index of the map to reference in item.map_item().
     */
    void set_item(const regular_bg_item& item, int map_index);

    /**
     * @brief Returns the horizontal position of the regular background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed x() const;

    /**
     * @brief Sets the horizontal position of the regular background (relative to its camera, if it has one).
     */
    void set_x(fixed x);

    /**
     * @brief Returns the vertical position of the regular background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed y() const;

    /**
     * @brief Sets the vertical position of the regular background (relative to its camera, if it has one).
     */
    void set_y(fixed y);

    /**
     * @brief Returns the position of the regular background (relative to its camera, if it has one).
     */
    [[nodiscard]] const fixed_point& position() const;

    /**
     * @brief Sets the position of the regular background (relative to its camera, if it has one).
     * @param x Horizontal position of the regular background (relative to its camera, if it has one).
     * @param y Vertical position of the regular background (relative to its camera, if it has one).
     */
    void set_position(fixed x, fixed y);

    /**
     * @brief Sets the position of the regular background (relative to its camera, if it has one).
     */
    void set_position(const fixed_point& position);

    /**
     * @brief Returns the horizontal top-left position of the regular background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed top_left_x() const;

    /**
     * @brief Sets the horizontal top-left position of the regular background (relative to its camera, if it has one).
     */
    void set_top_left_x(fixed top_left_x);

    /**
     * @brief Returns the vertical top-left position of the regular background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed top_left_y() const;

    /**
     * @brief Sets the vertical top-left position of the regular background (relative to its camera, if it has one).
     */
    void set_top_left_y(fixed top_left_y);

    /**
     * @brief Returns the top-left position of the regular background (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed_point top_left_position() const;

    /**
     * @brief Sets the top-left position of the regular background (relative to its camera, if it has one).
     * @param top_left_x Horizontal top-left position of the regular background (relative to its camera, if it has one).
     * @param top_left_y Vertical top-left position of the regular background (relative to its camera, if it has one).
     */
    void set_top_left_position(fixed top_left_x, fixed top_left_y);

    /**
     * @brief Sets the top-left position of the regular background (relative to its camera, if it has one).
     */
    void set_top_left_position(const fixed_point& top_left_position);

    /**
     * @brief Returns the priority of the regular background relative to sprites and other backgrounds.
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     */
    [[nodiscard]] int priority() const;

    /**
     * @brief Sets the priority of the regular background relative to sprites and other backgrounds.
     *
     * Backgrounds with higher priority are drawn first
     * (and therefore can be covered by later sprites and backgrounds).
     *
     * @param priority Priority in the range [0..3].
     */
    void set_priority(int priority);

    /**
     * @brief Returns the priority of the regular background relative to other backgrounds, excluding sprites.
     *
     * Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
     *
     * Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
     */
    [[nodiscard]] int z_order() const;

    /**
     * @brief Sets the priority of the regular background relative to other backgrounds, excluding sprites.
     *
     * Backgrounds with higher z orders are drawn first (and therefore can be covered by later backgrounds).
     *
     * Due to hardware limitations, affine backgrounds can be drawn before regular backgrounds with higher z order.
     *
     * @param z_order Priority relative to other backgrounds, excluding sprites, in the range [-32767..32767].
     */
    void set_z_order(int z_order);

    /**
     * @brief Modify this regular background to be drawn above all of the other backgrounds with the same priority.
     */
    void put_above();

    /**
     * @brief Modify this regular background to be drawn below all of the other backgrounds with the same priority.
     */
    void put_below();

    /**
     * @brief Indicates if the mosaic effect must be applied to this regular background or not.
     */
    [[nodiscard]] bool mosaic_enabled() const;

    /**
     * @brief Sets if the mosaic effect must be applied to this regular background or not.
     */
    void set_mosaic_enabled(bool mosaic_enabled);

    /**
     * @brief Indicates if blending must be applied to this regular background or not.
     *
     * Blending is applied to this regular background by making it part of the blending top layer.
     */
    [[nodiscard]] bool blending_enabled() const
    {
        return blending_top_enabled();
    }

    /**
     * @brief Sets if blending must be applied to this regular background or not.
     *
     * Blending is applied to this regular background by making it part of the blending top layer.
     */
    void set_blending_enabled(bool blending_enabled)
    {
        set_blending_top_enabled(blending_enabled);
    }

    /**
     * @brief Indicates if this regular background is part of the blending top layer or not.
     */
    [[nodiscard]] bool blending_top_enabled() const;

    /**
     * @brief Sets if this regular background is part of the blending top layer or not.
     */
    void set_blending_top_enabled(bool blending_top_enabled);

    /**
     * @brief Indicates if this regular background is part of the blending bottom layer or not.
     */
    [[nodiscard]] bool blending_bottom_enabled() const;

    /**
     * @brief Sets if this regular background is part of the blending bottom layer or not.
     */
    void set_blending_bottom_enabled(bool blending_bottom_enabled);

    /**
     * @brief Indicates if this regular background must be committed to the GBA or not.
     */
    [[nodiscard]] bool visible() const;

    /**
     * @brief Sets if this regular background must be committed to the GBA or not.
     */
    void set_visible(bool visible);

    /**
     * @brief Indicates if this regular background is visible in the given window or not.
     */
    [[nodiscard]] bool visible_in_window(const window& window) const;

    /**
     * @brief Sets if this regular background must be visible in the given window or not.
     */
    void set_visible_in_window(bool visible, window& window);

    /**
     * @brief Returns the camera_ptr attached to this regular background (if any).
     */
    [[nodiscard]] const optional<camera_ptr>& camera() const;

    /**
     * @brief Sets the camera_ptr attached to this regular background.
     * @param camera camera_ptr to copy to this regular background.
     */
    void set_camera(const camera_ptr& camera);

    /**
     * @brief Sets the camera_ptr attached to this regular background.
     * @param camera camera_ptr to move to this regular background.
     */
    void set_camera(camera_ptr&& camera);

    /**
     * @brief Sets or removes the camera_ptr attached to this regular background.
     * @param camera Optional camera_ptr to copy to this regular background.
     */
    void set_camera(const optional<camera_ptr>& camera);

    /**
     * @brief Sets or removes the camera_ptr attached to this regular background.
     * @param camera Optional camera_ptr to move to this regular background.
     */
    void set_camera(optional<camera_ptr>&& camera);

    /**
     * @brief Removes the camera_ptr attached to this regular background (if any).
     */
    void remove_camera();

    /**
     * @brief Returns the attributes to commit to the GBA for this regular background.
     */
    [[nodiscard]] regular_bg_attributes attributes() const;

    /**
     * @brief Sets the attributes to commit to the GBA for this regular background.
     */
    void set_attributes(const regular_bg_attributes& attributes);

    /**
     * @brief Returns the hardware ID assigned to this background or bn::nullopt if no hardware ID has been assigned.
     *
     * Normally you should not need to call this function, but it can be useful for messing with HDMA for example.
     *
     * A background doesn't have an assigned hardware ID if it is not visible.
     *
     * Assigned hardware ID can change after calling this method
     * if some properties of this background or others are updated.
     *
     * Call this method at your own risk.
     */
    [[nodiscard]] optional<int> hw_id() const;

    /**
     * @brief Returns the internal handle.
     */
    [[nodiscard]] const void* handle() const
    {
        return _handle;
    }

    /**
     * @brief Exchanges the contents of this regular_bg_ptr with those of the other one.
     * @param other regular_bg_ptr to exchange the contents with.
     */
    void swap(regular_bg_ptr& other)
    {
        bn::swap(_handle, other._handle);
    }

    /**
     * @brief Exchanges the contents of a regular_bg_ptr with those of another one.
     * @param a First regular_bg_ptr to exchange the contents with.
     * @param b Second regular_bg_ptr to exchange the contents with.
     */
    friend void swap(regular_bg_ptr& a, regular_bg_ptr& b)
    {
        bn::swap(a._handle, b._handle);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const regular_bg_ptr& a, const regular_bg_ptr& b) = default;

private:
    using handle_type = void*;

    handle_type _handle;

    explicit regular_bg_ptr(handle_type handle) :
        _handle(handle)
    {
    }
};


/**
 * @brief Hash support for regular_bg_ptr.
 *
 * @ingroup regular_bg
 * @ingroup functional
 */
template<>
struct hash<regular_bg_ptr>
{
    /**
     * @brief Returns the hash of the given regular_bg_ptr.
     */
    [[nodiscard]] unsigned operator()(const regular_bg_ptr& value) const
    {
        return make_hash(value.handle());
    }
};

}

#endif
