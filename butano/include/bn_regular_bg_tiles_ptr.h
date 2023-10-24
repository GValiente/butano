/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_REGULAR_BG_TILES_PTR_H
#define BN_REGULAR_BG_TILES_PTR_H

/**
 * @file
 * bn::regular_bg_tiles_ptr header file.
 *
 * @ingroup regular_bg
 * @ingroup tile
 */

#include "bn_span.h"
#include "bn_optional.h"

namespace bn
{

class tile;
class regular_bg_tiles_item;
enum class bpp_mode : uint8_t;
enum class compression_type : uint8_t;

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of the tiles of a regular background.
 *
 * Several regular_bg_tiles_ptr objects may own the same background tiles.
 *
 * The background tiles are released when the last remaining regular_bg_tiles_ptr owning them is destroyed.
 *
 * The tiles are not copied but referenced, so they should outlive the regular_bg_tiles_ptr
 * to avoid dangling references.
 *
 * @ingroup regular_bg
 * @ingroup tile
 */
class regular_bg_tiles_ptr
{

public:
    /**
     * @brief Searches for a regular_bg_tiles_ptr which references the given tiles.
     * @param tiles_item regular_bg_tiles_item which references the tiles to search.
     * @return regular_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_tiles_ptr> find(const regular_bg_tiles_item& tiles_item);

    /**
     * @brief Searches for a regular_bg_tiles_ptr which references the given tiles.
     * If it is not found, it creates a regular_bg_tiles_ptr which references them.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the regular_bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item regular_bg_tiles_item which references the tiles to search or handle.
     * @return regular_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found;
     * otherwise it returns a regular_bg_tiles_ptr which references them.
     */
    [[nodiscard]] static regular_bg_tiles_ptr create(const regular_bg_tiles_item& tiles_item);

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call create() method instead")]]
    [[nodiscard]] static regular_bg_tiles_ptr create_new(const regular_bg_tiles_item& tiles_item)
    {
        return create(tiles_item);
    }

    /// @endcond

    /**
     * @brief Creates a regular_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
     * @param tiles_count Number of tiles to allocate.
     * @param bpp Bits per pixel of the tiles to allocate.
     * @return regular_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
     */
    [[nodiscard]] static regular_bg_tiles_ptr allocate(int tiles_count, bpp_mode bpp);

    /**
     * @brief Searches for a regular_bg_tiles_ptr which references the given tiles.
     * If it is not found, it creates a regular_bg_tiles_ptr which references them.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the regular_bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item regular_bg_tiles_item which references the tiles to search or handle.
     * @return regular_bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found;
     * otherwise it returns a regular_bg_tiles_ptr which references them if it could be allocated;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_tiles_ptr> create_optional(const regular_bg_tiles_item& tiles_item);

    /// @cond DO_NOT_DOCUMENT

    [[deprecated("Call create_optional() method instead")]]
    [[nodiscard]] static optional<regular_bg_tiles_ptr> create_new_optional(const regular_bg_tiles_item& tiles_item)
    {
        return create_optional(tiles_item);
    }

    /// @endcond

    /**
     * @brief Creates a regular_bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
     * @param tiles_count Number of tiles to allocate.
     * @param bpp Bits per pixel of the tiles to allocate.
     * @return regular_bg_tiles_ptr which references a chunk of VRAM tiles
     * not visible on the screen if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<regular_bg_tiles_ptr> allocate_optional(int tiles_count, bpp_mode bpp);

    /**
     * @brief Copy constructor.
     * @param other regular_bg_tiles_ptr to copy.
     */
    regular_bg_tiles_ptr(const regular_bg_tiles_ptr& other);

    /**
     * @brief Copy assignment operator.
     * @param other regular_bg_tiles_ptr to copy.
     * @return Reference to this.
     */
    regular_bg_tiles_ptr& operator=(const regular_bg_tiles_ptr& other);

    /**
     * @brief Move constructor.
     * @param other regular_bg_tiles_ptr to move.
     */
    regular_bg_tiles_ptr(regular_bg_tiles_ptr&& other) noexcept :
        regular_bg_tiles_ptr(other._handle)
    {
        other._handle = -1;
    }

    /**
     * @brief Move assignment operator.
     * @param other regular_bg_tiles_ptr to move.
     * @return Reference to this.
     */
    regular_bg_tiles_ptr& operator=(regular_bg_tiles_ptr&& other) noexcept
    {
        bn::swap(_handle, other._handle);
        return *this;
    }

    /**
     * @brief Releases the referenced background tiles if no more regular_bg_tiles_ptr objects reference to them.
     */
    ~regular_bg_tiles_ptr();

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
     * Remember that the background tiles system does not support multiple regular_bg_tiles_ptr items
     * referencing to the same tiles.
     *
     * Remember also that the tiles are not copied but referenced,
     * so they should outlive the regular_bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item regular_bg_tiles_item which references the tiles to handle.
     */
    void set_tiles_ref(const regular_bg_tiles_item& tiles_item);

    /**
     * @brief Uploads the referenced tiles to VRAM again to make visible the possible changes in them.
     */
    void reload_tiles_ref();

    /**
     * @brief Returns the allocated memory in VRAM
     * if this regular_bg_tiles_ptr was created with allocate or allocate_optional; bn::nullopt otherwise.
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
     * @brief Exchanges the contents of this regular_bg_tiles_ptr with those of the other one.
     * @param other regular_bg_tiles_ptr to exchange the contents with.
     */
    void swap(regular_bg_tiles_ptr& other)
    {
        bn::swap(_handle, other._handle);
    }

    /**
     * @brief Exchanges the contents of a regular_bg_tiles_ptr with those of another one.
     * @param a First regular_bg_tiles_ptr to exchange the contents with.
     * @param b Second regular_bg_tiles_ptr to exchange the contents with.
     */
    friend void swap(regular_bg_tiles_ptr& a, regular_bg_tiles_ptr& b)
    {
        bn::swap(a._handle, b._handle);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const regular_bg_tiles_ptr& a, const regular_bg_tiles_ptr& b) = default;

private:
    int8_t _handle;

    explicit regular_bg_tiles_ptr(int handle) :
        _handle(int8_t(handle))
    {
    }
};


/**
 * @brief Hash support for regular_bg_tiles_ptr.
 *
 * @ingroup regular_bg
 * @ingroup tile
 * @ingroup functional
 */
template<>
struct hash<regular_bg_tiles_ptr>
{
    /**
     * @brief Returns the hash of the given regular_bg_tiles_ptr.
     */
    [[nodiscard]] unsigned operator()(const regular_bg_tiles_ptr& value) const
    {
        return make_hash(value.handle());
    }
};

}

#endif
