/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_MAP_PTR_H
#define BN_AFFINE_BG_MAP_PTR_H

/**
 * @file
 * bn::affine_bg_map_ptr header file.
 *
 * @ingroup affine_bg
 * @ingroup bg_map
 */

#include "bn_span.h"
#include "bn_optional.h"
#include "bn_affine_bg_map_cell.h"

namespace bn
{

class size;
class affine_bg_item;
class bg_palette_ptr;
class bg_palette_item;
class affine_bg_map_item;
class affine_bg_tiles_ptr;
class affine_bg_tiles_item;
enum class compression_type : uint8_t;
enum class affine_bg_big_map_canvas_size : uint8_t;

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of an affine background map.
 *
 * Several affine_bg_map_ptr objects may own the same affine background.
 *
 * The affine background map is released when the last remaining affine_bg_map_ptr owning it is destroyed.
 *
 * @ingroup affine_bg
 * @ingroup bg_map
 */
class affine_bg_map_ptr
{

public:
    /**
     * @brief Searches for an affine_bg_map_ptr which references the given information.
     * @param map_item affine_bg_map_item which references the map cells to search.
     * @param tiles Referenced tiles of the map to search.
     * @param palette Referenced color palette of the map to search.
     * @return affine_bg_map_ptr which references the given information if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_map_ptr> find(
            const affine_bg_map_item& map_item, const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette);

    /**
     * @brief Searches for an affine_bg_map_ptr which references the given information.
     * @param map_item affine_bg_map_item which references the map cells to search.
     * @param tiles Referenced tiles of the map to search.
     * @param palette Referenced color palette of the map to search.
     * @param map_index Index of the referenced map to search.
     * @return affine_bg_map_ptr which references the given information if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_map_ptr> find(
            const affine_bg_map_item& map_item, const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette,
            int map_index);

    /**
     * @brief Searches for an affine_bg_map_ptr which references the given information.
     * @param item affine_bg_item which references the tiles, the color palette and the map cells to search.
     * @return affine_bg_map_ptr which references the given information if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_map_ptr> find(const affine_bg_item& item);

    /**
     * @brief Searches for an affine_bg_map_ptr which references the given information.
     * @param item affine_bg_item which references the tiles, the color palette and the map cells to search.
     * @param map_index Index of the referenced map to search.
     * @return affine_bg_map_ptr which references the given information if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_map_ptr> find(const affine_bg_item& item, int map_index);

    /**
     * @brief Searches for an affine_bg_map_ptr which references the given information.
     * If it is not found, it creates an affine_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param map_item affine_bg_map_item which references the map cells to search or handle.
     * @param tiles Referenced tiles of the map to search or handle.
     * @param palette Referenced color palette of the map to search or handle.
     * @return affine_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns an affine_bg_map_ptr which references it.
     */
    [[nodiscard]] static affine_bg_map_ptr create(
            const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette);

    /**
     * @brief Searches for an affine_bg_map_ptr which references the given information.
     * If it is not found, it creates an affine_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param map_item affine_bg_map_item which references the map cells to search or handle.
     * @param tiles Referenced tiles of the map to search or handle.
     * @param palette Referenced color palette of the map to search or handle.
     * @param map_index Index of the referenced map to search or handle.
     * @return affine_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns an affine_bg_map_ptr which references it.
     */
    [[nodiscard]] static affine_bg_map_ptr create(
            const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index);

    /**
     * @brief Searches for an affine_bg_map_ptr which references the given information.
     * If it is not found, it creates an affine_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param item affine_bg_item which references the tiles,
     * the color palette and the map cells to search or handle.
     * @return affine_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns an affine_bg_map_ptr which references it.
     */
    [[nodiscard]] static affine_bg_map_ptr create(const affine_bg_item& item);

    /**
     * @brief Searches for an affine_bg_map_ptr which references the given information.
     * If it is not found, it creates an affine_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param item affine_bg_item which references the tiles,
     * the color palette and the map cells to search or handle.
     * @param map_index Index of the referenced map to search or handle.
     * @return affine_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns an affine_bg_map_ptr which references it.
     */
    [[nodiscard]] static affine_bg_map_ptr create(const affine_bg_item& item, int map_index);

    /**
     * @brief Creates an affine_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple affine_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param map_item affine_bg_map_item which references the map cells to handle.
     * @param tiles Referenced tiles of the map to handle.
     * @param palette Referenced color palette of the map to handle.
     * @return affine_bg_map_ptr which references the given information.
     */
    [[nodiscard]] static affine_bg_map_ptr create_new(
            const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette);

    /**
     * @brief Creates an affine_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple affine_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param map_item affine_bg_map_item which references the map cells to handle.
     * @param tiles Referenced tiles of the map to handle.
     * @param palette Referenced color palette of the map to handle.
     * @param map_index Index of the referenced map to handle.
     * @return affine_bg_map_ptr which references the given information.
     */
    [[nodiscard]] static affine_bg_map_ptr create_new(
            const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index);

    /**
     * @brief Creates an affine_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple affine_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param item affine_bg_item which references the tiles, the color palette and the map cells to handle.
     * @return affine_bg_map_ptr which references the given information.
     */
    [[nodiscard]] static affine_bg_map_ptr create_new(const affine_bg_item& item);

    /**
     * @brief Creates an affine_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple affine_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param item affine_bg_item which references the tiles, the color palette and the map cells to handle.
     * @param map_index Index of the referenced map to handle.
     * @return affine_bg_map_ptr which references the given information.
     */
    [[nodiscard]] static affine_bg_map_ptr create_new(const affine_bg_item& item, int map_index);

    /**
     * @brief Creates an affine_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen.
     * @param dimensions Size in map cells of the map to allocate.
     * @param tiles Referenced tiles of the map to allocate.
     * @param palette Referenced color palette of the map to allocate.
     * @return affine_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen.
     */
    [[nodiscard]] static affine_bg_map_ptr allocate(
            const size& dimensions, affine_bg_tiles_ptr tiles, bg_palette_ptr palette);

    /**
     * @brief Searches for an affine_bg_map_ptr which references the given information.
     * If it is not found, it creates an affine_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param map_item affine_bg_map_item which references the map cells to search or handle.
     * @param tiles Referenced tiles of the map to search or handle.
     * @param palette Referenced color palette of the map to search or handle.
     * @return affine_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns an affine_bg_map_ptr which references it if the affine_bg_map_ptr can be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_map_ptr> create_optional(
            const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette);

    /**
     * @brief Searches for an affine_bg_map_ptr which references the given information.
     * If it is not found, it creates an affine_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param map_item affine_bg_map_item which references the map cells to search or handle.
     * @param tiles Referenced tiles of the map to search or handle.
     * @param palette Referenced color palette of the map to search or handle.
     * @param map_index Index of the referenced map to search or handle.
     * @return affine_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns an affine_bg_map_ptr which references it if the affine_bg_map_ptr can be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_map_ptr> create_optional(
            const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index);

    /**
     * @brief Searches for an affine_bg_map_ptr which references the given information.
     * If it is not found, it creates an affine_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param item affine_bg_item which references the tiles,
     * the color palette and the map cells to search or handle.
     * @return affine_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns an affine_bg_map_ptr which references it if the affine_bg_map_ptr can be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_map_ptr> create_optional(const affine_bg_item& item);

    /**
     * @brief Searches for an affine_bg_map_ptr which references the given information.
     * If it is not found, it creates an affine_bg_map_ptr which references it.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param item affine_bg_item which references the tiles,
     * the color palette and the map cells to search or handle.
     * @param map_index Index of the referenced map to search or handle.
     * @return affine_bg_map_ptr which references the given information if it has been found;
     * otherwise it returns an affine_bg_map_ptr which references it if the affine_bg_map_ptr can be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_map_ptr> create_optional(const affine_bg_item& item, int map_index);

    /**
     * @brief Creates an affine_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple affine_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param map_item affine_bg_map_item which references the map cells to handle.
     * @param tiles Referenced tiles of the map to handle.
     * @param palette Referenced color palette of the map to handle.
     * @return affine_bg_map_ptr which references the given information
     * if the affine_bg_map_ptr can be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_map_ptr> create_new_optional(
            const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette);

    /**
     * @brief Creates an affine_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple affine_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param map_item affine_bg_map_item which references the map cells to handle.
     * @param tiles Referenced tiles of the map to handle.
     * @param palette Referenced color palette of the map to handle.
     * @param map_index Index of the referenced map to handle.
     * @return affine_bg_map_ptr which references the given information
     * if the affine_bg_map_ptr can be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_map_ptr> create_new_optional(
            const affine_bg_map_item& map_item, affine_bg_tiles_ptr tiles, bg_palette_ptr palette, int map_index);

    /**
     * @brief Creates an affine_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple affine_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param item affine_bg_item which references the tiles, the color palette and the map cells to handle.
     * @return affine_bg_map_ptr which references the given information
     * if the affine_bg_map_ptr can be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_map_ptr> create_new_optional(const affine_bg_item& item);

    /**
     * @brief Creates an affine_bg_map_ptr which references the given map cells.
     *
     * The map system does not support multiple affine_bg_map_ptr items referencing to the same map cells.
     * If you are not sure if the given map cells are already referenced or not,
     * you should use the static create methods instead.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param item affine_bg_item which references the tiles, the color palette and the map cells to handle.
     * @param map_index Index of the referenced map to handle.
     * @return affine_bg_map_ptr which references the given information
     * if the affine_bg_map_ptr can be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_map_ptr> create_new_optional(const affine_bg_item& item, int map_index);

    /**
     * @brief Creates an affine_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen.
     * @param dimensions Size in map cells of the map to allocate.
     * @param tiles Referenced tiles of the map to allocate.
     * @param palette Referenced color palette of the map to allocate.
     * @return affine_bg_map_ptr which references a chunk of VRAM map cells not visible on the screen
     * if the affine_bg_map_ptr can be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_map_ptr> allocate_optional(
            const size& dimensions, affine_bg_tiles_ptr tiles, bg_palette_ptr palette);

    /**
     * @brief Copy constructor.
     * @param other affine_bg_map_ptr to copy.
     */
    affine_bg_map_ptr(const affine_bg_map_ptr& other);

    /**
     * @brief Copy assignment operator.
     * @param other affine_bg_map_ptr to copy.
     * @return Reference to this.
     */
    affine_bg_map_ptr& operator=(const affine_bg_map_ptr& other);

    /**
     * @brief Move constructor.
     * @param other affine_bg_map_ptr to move.
     */
    affine_bg_map_ptr(affine_bg_map_ptr&& other) noexcept :
        affine_bg_map_ptr(other._handle)
    {
        other._handle = -1;
    }

    /**
     * @brief Move assignment operator.
     * @param other affine_bg_map_ptr to move.
     * @return Reference to this.
     */
    affine_bg_map_ptr& operator=(affine_bg_map_ptr&& other) noexcept
    {
        bn::swap(_handle, other._handle);
        return *this;
    }

    /**
     * @brief Releases the referenced map cells if no more affine_bg_map_ptr objects reference to them.
     */
    ~affine_bg_map_ptr();

    /**
     * @brief Returns the internal id.
     */
    [[nodiscard]] int id() const;

    /**
     * @brief Returns the size in map cells of the referenced map.
     */
    [[nodiscard]] size dimensions() const;

    /**
     * @brief Indicates if this map is big or not.
     *
     * Big backgrounds are slower CPU wise, but can have any width or height multiple of 256 pixels.
     */
    [[nodiscard]] bool big() const;

    /**
     * @brief Returns the size of the canvas used to render this big map.
     */
    [[nodiscard]] affine_bg_big_map_canvas_size big_canvas_size() const;

    /**
     * @brief Returns how much tiles to offset in the cells of this map before writing them in VRAM.
     */
    [[nodiscard]] int tiles_offset() const;

    /**
     * @brief Returns the compression of the referenced map cells.
     */
    [[nodiscard]] compression_type compression() const;

    /**
     * @brief Returns the referenced map cells unless it was created with allocate or allocate_optional.
     * In that case, it returns bn::nullopt.
     */
    [[nodiscard]] optional<span<const affine_bg_map_cell>> cells_ref() const;

    /**
     * @brief Sets the map cells to handle.
     *
     * The map system does not support multiple affine_bg_map_ptr items referencing to the same map cells.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param map_item affine_bg_map_item which references the map cells to set.
     */
    void set_cells_ref(const affine_bg_map_item& map_item);

    /**
     * @brief Sets the map cells to handle.
     *
     * The map system does not support multiple affine_bg_map_ptr items referencing to the same map cells.
     *
     * The map cells are not copied but referenced,
     * so they should outlive the affine_bg_map_ptr to avoid dangling references.
     *
     * @param map_item affine_bg_map_item which references the map cells to set.
     * @param map_index Index of the referenced map to set.
     */
    void set_cells_ref(const affine_bg_map_item& map_item, int map_index);

    /**
     * @brief Uploads the referenced map cells to VRAM again to make visible the possible changes in them.
     */
    void reload_cells_ref();

    /**
     * @brief Returns the referenced tiles.
     */
    [[nodiscard]] const affine_bg_tiles_ptr& tiles() const;

    /**
     * @brief Sets the referenced tiles.
     * @param tiles affine_bg_tiles_ptr to copy.
     */
    void set_tiles(const affine_bg_tiles_ptr& tiles);

    /**
     * @brief Sets the referenced tiles.
     * @param tiles affine_bg_tiles_ptr to move.
     */
    void set_tiles(affine_bg_tiles_ptr&& tiles);

    /**
     * @brief Replaces the referenced tiles with a new tile set created with the given affine_bg_tiles_item.
     *
     * Before creating a new background tile set, the affine_bg_tiles_ptr referenced by this map is removed,
     * so VRAM usage is reduced.
     *
     * @param tiles_item It creates the new background tiles to reference.
     */
    void set_tiles(const affine_bg_tiles_item& tiles_item);

    /**
     * @brief Returns the referenced color palette.
     */
    [[nodiscard]] const bg_palette_ptr& palette() const;

    /**
     * @brief Sets the referenced color palette.
     * @param palette bg_palette_ptr to copy.
     */
    void set_palette(const bg_palette_ptr& palette);

    /**
     * @brief Sets the referenced color palette.
     * @param palette bg_palette_ptr to move.
     */
    void set_palette(bg_palette_ptr&& palette);

    /**
     * @brief Replaces the referenced color palette with a new tile set created with the given bg_palette_item.
     *
     * Before creating a new color palette, the bg_palette_ptr referenced by this map is removed,
     * so VRAM usage is reduced.
     *
     * @param palette_item It creates the new color palette to reference.
     */
    void set_palette(const bg_palette_item& palette_item);

    /**
     * @brief Returns the allocated memory in VRAM
     * if this affine_bg_map_cell was created with allocate or allocate_optional; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<span<affine_bg_map_cell>> vram();

    /**
     * @brief Returns the internal handle.
     */
    [[nodiscard]] int handle() const
    {
        return _handle;
    }

    /**
     * @brief Exchanges the contents of this affine_bg_map_ptr with those of the other one.
     * @param other affine_bg_map_ptr to exchange the contents with.
     */
    void swap(affine_bg_map_ptr& other)
    {
        bn::swap(_handle, other._handle);
    }

    /**
     * @brief Exchanges the contents of an affine_bg_map_ptr with those of another one.
     * @param a First affine_bg_map_ptr to exchange the contents with.
     * @param b Second affine_bg_map_ptr to exchange the contents with.
     */
    friend void swap(affine_bg_map_ptr& a, affine_bg_map_ptr& b)
    {
        bn::swap(a._handle, b._handle);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const affine_bg_map_ptr& a, const affine_bg_map_ptr& b) = default;

private:
    int8_t _handle;

    explicit affine_bg_map_ptr(int handle) :
        _handle(int8_t(handle))
    {
    }
};


/**
 * @brief Hash support for affine_bg_map_ptr.
 *
 * @ingroup affine_bg
 * @ingroup bg_map
 * @ingroup functional
 */
template<>
struct hash<affine_bg_map_ptr>
{
    /**
     * @brief Returns the hash of the given affine_bg_map_ptr.
     */
    [[nodiscard]] unsigned operator()(const affine_bg_map_ptr& value) const
    {
        return make_hash(value.handle());
    }
};

}

#endif
