/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_TILES_PTR_H
#define BN_AFFINE_BG_TILES_PTR_H

/**
 * @file
 * bn::affine_bg_tiles_ptr header file.
 *
 * @ingroup affine_bg
 * @ingroup tile
 */

#include "bn_span.h"
#include "bn_optional.h"

namespace bn
{

class tile;
class affine_bg_tiles_item;
enum class compression_type : uint8_t;

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of the tiles of an affine background.
 *
 * Several affine_bg_tiles_ptr objects may own the same background tiles.
 *
 * The background tiles are released when the last remaining affine_bg_tiles_ptr owning them is destroyed.
 *
 * The tiles are not copied but referenced, so they should outlive the affine_bg_tiles_ptr
 * to avoid dangling references.
 *
 * @ingroup affine_bg
 * @ingroup tile
 */
class affine_bg_tiles_ptr
{

public:
    /**
     * @brief Searches for an affine_bg_tiles_ptr which references the given tiles.
     * @param tiles_item affine_bg_tiles_item which references the tiles to search.
     * @return affine_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_tiles_ptr> find(const affine_bg_tiles_item& tiles_item);

    /**
     * @brief Searches for an affine_bg_tiles_ptr which references the given tiles.
     * If it is not found, it creates an affine_bg_tiles_ptr which references them.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the affine_bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item affine_bg_tiles_item which references the tiles to search or handle.
     * @return affine_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found;
     * otherwise it returns an affine_bg_tiles_ptr which references them.
     */
    [[nodiscard]] static affine_bg_tiles_ptr create(const affine_bg_tiles_item& tiles_item);

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call create() method instead")]]
    [[nodiscard]] static affine_bg_tiles_ptr create_new(const affine_bg_tiles_item& tiles_item)
    {
        return create(tiles_item);
    }

    /// @endcond

    /**
     * @brief Creates an affine_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
     * @param tiles_count Number of tiles to allocate.
     * @return affine_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
     */
    [[nodiscard]] static affine_bg_tiles_ptr allocate(int tiles_count);

    /**
     * @brief Searches for an affine_bg_tiles_ptr which references the given tiles.
     * If it is not found, it creates an affine_bg_tiles_ptr which references them.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the affine_bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item affine_bg_tiles_item which references the tiles to search or handle.
     * @return affine_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found;
     * otherwise it returns an affine_bg_tiles_ptr which references them if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_tiles_ptr> create_optional(const affine_bg_tiles_item& tiles_item);

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call create_optional() method instead")]]
    [[nodiscard]] static optional<affine_bg_tiles_ptr> create_new_optional(const affine_bg_tiles_item& tiles_item)
    {
        return create_optional(tiles_item);
    }

    /// @endcond

    /**
     * @brief Creates an affine_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
     * @param tiles_count Number of tiles to allocate.
     * @return affine_bg_tiles_ptr which references a chunk of VRAM tiles
     * not visible on the screen if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<affine_bg_tiles_ptr> allocate_optional(int tiles_count);

    /**
     * @brief Copy constructor.
     * @param other affine_bg_tiles_ptr to copy.
     */
    affine_bg_tiles_ptr(const affine_bg_tiles_ptr& other);

    /**
     * @brief Copy assignment operator.
     * @param other affine_bg_tiles_ptr to copy.
     * @return Reference to this.
     */
    affine_bg_tiles_ptr& operator=(const affine_bg_tiles_ptr& other);

    /**
     * @brief Move constructor.
     * @param other affine_bg_tiles_ptr to move.
     */
    affine_bg_tiles_ptr(affine_bg_tiles_ptr&& other) noexcept :
        affine_bg_tiles_ptr(other._handle)
    {
        other._handle = -1;
    }

    /**
     * @brief Move assignment operator.
     * @param other affine_bg_tiles_ptr to move.
     * @return Reference to this.
     */
    affine_bg_tiles_ptr& operator=(affine_bg_tiles_ptr&& other) noexcept
    {
        bn::swap(_handle, other._handle);
        return *this;
    }

    /**
     * @brief Releases the referenced background tiles if no more affine_bg_tiles_ptr objects reference to them.
     */
    ~affine_bg_tiles_ptr();

    /**
     * @brief Returns the internal id.
     */
    [[nodiscard]] int id() const;

    /**
     * @brief Returns the character base block of the referenced tiles.
     */
    [[nodiscard]] int cbb() const;

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
     * Remember that the background tiles system does not support multiple affine_bg_tiles_ptr items
     * referencing to the same tiles.
     *
     * Remember also that the tiles are not copied but referenced,
     * so they should outlive the affine_bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item affine_bg_tiles_item which references the tiles to handle.
     */
    void set_tiles_ref(const affine_bg_tiles_item& tiles_item);

    /**
     * @brief Uploads the referenced tiles to VRAM again to make visible the possible changes in them.
     */
    void reload_tiles_ref();

    /**
     * @brief Returns the allocated memory in VRAM
     * if this affine_bg_tiles_ptr was created with allocate or allocate_optional; bn::nullopt otherwise.
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
     * @brief Exchanges the contents of this affine_bg_tiles_ptr with those of the other one.
     * @param other affine_bg_tiles_ptr to exchange the contents with.
     */
    void swap(affine_bg_tiles_ptr& other)
    {
        bn::swap(_handle, other._handle);
    }

    /**
     * @brief Exchanges the contents of an affine_bg_tiles_ptr with those of another one.
     * @param a First affine_bg_tiles_ptr to exchange the contents with.
     * @param b Second affine_bg_tiles_ptr to exchange the contents with.
     */
    friend void swap(affine_bg_tiles_ptr& a, affine_bg_tiles_ptr& b)
    {
        bn::swap(a._handle, b._handle);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const affine_bg_tiles_ptr& a, const affine_bg_tiles_ptr& b) = default;

private:
    int8_t _handle;

    explicit affine_bg_tiles_ptr(int handle) :
        _handle(int8_t(handle))
    {
    }
};


/**
 * @brief Hash support for affine_bg_tiles_ptr.
 *
 * @ingroup affine_bg
 * @ingroup tile
 * @ingroup functional
 */
template<>
struct hash<affine_bg_tiles_ptr>
{
    /**
     * @brief Returns the hash of the given affine_bg_tiles_ptr.
     */
    [[nodiscard]] unsigned operator()(const affine_bg_tiles_ptr& value) const
    {
        return make_hash(value.handle());
    }
};

}

#endif
