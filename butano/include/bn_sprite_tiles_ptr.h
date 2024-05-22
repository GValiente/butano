/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_TILES_PTR_H
#define BN_SPRITE_TILES_PTR_H

/**
 * @file
 * bn::sprite_tiles_ptr header file.
 *
 * @ingroup sprite
 * @ingroup tile
 */

#include "bn_span.h"
#include "bn_optional.h"

namespace bn
{

class tile;
class sprite_tiles_item;
enum class bpp_mode : uint8_t;
enum class compression_type : uint8_t;

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of the tiles of a sprite.
 *
 * Several sprite_tiles_ptr objects may own the same sprite tiles.
 *
 * The sprite tiles are released when the last remaining sprite_tiles_ptr owning them is destroyed.
 *
 * The tiles are not copied but referenced, so they should outlive the sprite_tiles_ptr to avoid dangling references.
 *
 * @ingroup sprite
 * @ingroup tile
 */
class sprite_tiles_ptr
{

public:
    /**
     * @brief Searches for a sprite_tiles_ptr which references the given tiles.
     * @param tiles_item sprite_tiles_item which references the tiles to search.
     * @return sprite_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<sprite_tiles_ptr> find(const sprite_tiles_item& tiles_item);

    /**
     * @brief Searches for a sprite_tiles_ptr which references the given tiles.
     * @param tiles_item sprite_tiles_item which references the tiles to search.
     * @param graphics_index Index of the tile set to search in sprite_tiles_item.
     * @return sprite_tiles_ptr which references tiles_item.graphics_tiles_ref(graphics_index) if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<sprite_tiles_ptr> find(const sprite_tiles_item& tiles_item, int graphics_index);

    /**
     * @brief Searches for a sprite_tiles_ptr which references the given tiles.
     * If it is not found, it creates a sprite_tiles_ptr which references them.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the sprite_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item sprite_tiles_item which references the tiles to search or handle.
     * @return sprite_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found;
     * otherwise it returns a sprite_tiles_ptr which references them.
     */
    [[nodiscard]] static sprite_tiles_ptr create(const sprite_tiles_item& tiles_item);

    /**
     * @brief Searches for a sprite_tiles_ptr which references the given tiles.
     * If it is not found, it creates a sprite_tiles_ptr which references them.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the sprite_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item sprite_tiles_item which references the tiles to search or handle.
     * @param graphics_index Index of the tile set to search in sprite_tiles_item.
     * @return sprite_tiles_ptr which references tiles_item.graphics_tiles_ref(graphics_index) if it has been found;
     * otherwise it returns a sprite_tiles_ptr which references them.
     */
    [[nodiscard]] static sprite_tiles_ptr create(const sprite_tiles_item& tiles_item, int graphics_index);

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call create() method instead")]]
    [[nodiscard]] static sprite_tiles_ptr create_new(const sprite_tiles_item& tiles_item)
    {
        return create(tiles_item);
    }

    [[deprecated("Call create() method instead")]]
    [[nodiscard]] static sprite_tiles_ptr create_new(const sprite_tiles_item& tiles_item, int graphics_index)
    {
        return create(tiles_item, graphics_index);
    }

    /// @endcond

    /**
     * @brief Creates a sprite_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
     * @param tiles_count Number of tiles to allocate.
     * @param bpp Bits per pixel of the tiles to allocate.
     * @return sprite_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
     */
    [[nodiscard]] static sprite_tiles_ptr allocate(int tiles_count, bpp_mode bpp);

    /**
     * @brief Searches for a sprite_tiles_ptr which references the given tiles.
     * If it is not found, it creates a sprite_tiles_ptr which references them.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the sprite_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item sprite_tiles_item which references the tiles to search or handle.
     * @return sprite_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found;
     * otherwise it returns a sprite_tiles_ptr which references them if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<sprite_tiles_ptr> create_optional(const sprite_tiles_item& tiles_item);

    /**
     * @brief Searches for a sprite_tiles_ptr which references the given tiles.
     * If it is not found, it creates a sprite_tiles_ptr which references them.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the sprite_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item sprite_tiles_item which references the tiles to search or handle.
     * @param graphics_index Index of the tile set to search in sprite_tiles_item.
     * @return sprite_tiles_ptr which references tiles_item.graphics_tiles_ref(graphics_index) if it has been found;
     * otherwise it returns a sprite_tiles_ptr which references them if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<sprite_tiles_ptr> create_optional(const sprite_tiles_item& tiles_item,
                                                                    int graphics_index);

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call create_optional() method instead")]]
    [[nodiscard]] static optional<sprite_tiles_ptr> create_new_optional(const sprite_tiles_item& tiles_item)
    {
        return create_optional(tiles_item);
    }

    [[deprecated("Call create_optional() method instead")]]
    [[nodiscard]] static optional<sprite_tiles_ptr> create_new_optional(
            const sprite_tiles_item& tiles_item, int graphics_index)
    {
        return create_optional(tiles_item, graphics_index);
    }

    /// @endcond

    /**
     * @brief Creates a sprite_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
     * @param tiles_count Number of tiles to allocate.
     * @param bpp Bits per pixel of the tiles to allocate.
     * @return sprite_tiles_ptr which references a chunk of VRAM tiles
     * not visible on the screen if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<sprite_tiles_ptr> allocate_optional(int tiles_count, bpp_mode bpp);

    /**
     * @brief Copy constructor.
     * @param other sprite_tiles_ptr to copy.
     */
    sprite_tiles_ptr(const sprite_tiles_ptr& other);

    /**
     * @brief Copy assignment operator.
     * @param other sprite_tiles_ptr to copy.
     * @return Reference to this.
     */
    sprite_tiles_ptr& operator=(const sprite_tiles_ptr& other);

    /**
     * @brief Move constructor.
     * @param other sprite_tiles_ptr to move.
     */
    sprite_tiles_ptr(sprite_tiles_ptr&& other) noexcept :
        sprite_tiles_ptr(other._handle)
    {
        other._handle = -1;
    }

    /**
     * @brief Move assignment operator.
     * @param other sprite_tiles_ptr to move.
     * @return Reference to this.
     */
    sprite_tiles_ptr& operator=(sprite_tiles_ptr&& other) noexcept
    {
        bn::swap(_handle, other._handle);
        return *this;
    }

    /**
     * @brief Releases the referenced sprite tiles if no more sprite_tiles_ptr objects reference to them.
     */
    ~sprite_tiles_ptr();

    /**
     * @brief Returns the internal id.
     */
    [[nodiscard]] int id() const;

    /**
     * @brief Returns the referenced number of tiles.
     */
    [[nodiscard]] int tiles_count() const;

    /**
     * @brief Returns the compression of the referenced tiles.
     */
    [[nodiscard]] compression_type compression() const;

    /**
     * @brief Returns the referenced tiles unless it was created with allocate or allocate_optional.
     * In that case, it returns bn::nullopt.
     */
    [[nodiscard]] optional<span<const tile>> tiles_ref() const;

    /**
     * @brief Sets the tiles to handle.
     *
     * Remember that the sprite tiles system does not support multiple sprite_tiles_ptr items
     * referencing to the same tiles.
     *
     * Remember also that the tiles are not copied but referenced,
     * so they should outlive the sprite_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item sprite_tiles_item which references the tiles to handle.
     */
    void set_tiles_ref(const sprite_tiles_item& tiles_item);

    /**
     * @brief Sets the tiles to handle.
     *
     * Remember that the sprite tiles system does not support multiple sprite_tiles_ptr items
     * referencing to the same tiles.
     *
     * Remember also that the tiles are not copied but referenced,
     * so they should outlive the sprite_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item sprite_tiles_item which references the tiles to handle.
     * @param graphics_index Index of the tile set to reference in sprite_tiles_item.
     */
    void set_tiles_ref(const sprite_tiles_item& tiles_item, int graphics_index);

    /**
     * @brief Uploads the referenced tiles to VRAM again to make visible the possible changes in them.
     */
    void reload_tiles_ref();

    /**
     * @brief Returns the allocated memory in VRAM
     * if this sprite_tiles_ptr was created with allocate or allocate_optional; bn::nullopt otherwise.
     */
    [[nodiscard]] optional<span<tile>> vram();

    /**
     * @brief Returns the internal handle.
     */
    [[nodiscard]] int handle() const
    {
        return _handle;
    }

    /**
     * @brief Exchanges the contents of this sprite_tiles_ptr with those of the other one.
     * @param other sprite_tiles_ptr to exchange the contents with.
     */
    void swap(sprite_tiles_ptr& other)
    {
        bn::swap(_handle, other._handle);
    }

    /**
     * @brief Exchanges the contents of a sprite_tiles_ptr with those of another one.
     * @param a First sprite_tiles_ptr to exchange the contents with.
     * @param b Second sprite_tiles_ptr to exchange the contents with.
     */
    friend void swap(sprite_tiles_ptr& a, sprite_tiles_ptr& b)
    {
        bn::swap(a._handle, b._handle);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const sprite_tiles_ptr& a, const sprite_tiles_ptr& b) = default;

private:
    int16_t _handle;

    explicit sprite_tiles_ptr(int handle) :
        _handle(int16_t(handle))
    {
    }
};


/**
 * @brief Hash support for sprite_tiles_ptr.
 *
 * @ingroup sprite
 * @ingroup tile
 * @ingroup functional
 */
template<>
struct hash<sprite_tiles_ptr>
{
    /**
     * @brief Returns the hash of the given sprite_tiles_ptr.
     */
    [[nodiscard]] unsigned operator()(const sprite_tiles_ptr& value) const
    {
        return make_hash(value.handle());
    }
};

}

#endif
