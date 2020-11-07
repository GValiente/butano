/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BG_TILES_PTR_H
#define BTN_BG_TILES_PTR_H

#include "btn_utility.h"
#include "btn_span_fwd.h"
#include "btn_functional.h"
#include "btn_optional_fwd.h"

namespace btn
{

class tile;
class bg_tiles_item;
enum class palette_bpp_mode;

/**
 * @brief Smart pointer that retains shared ownership of the tiles of a background.
 *
 * Several bg_tiles_ptr objects may own the same background tiles.
 *
 * The background tiles are released when the last remaining bg_tiles_ptr owning them is destroyed.
 *
 * The tiles are not copied but referenced, so they should outlive the bg_tiles_ptr to avoid dangling references.
 *
 * @ingroup bg
 * @ingroup tile
 */
class bg_tiles_ptr
{

public:
    /**
     * @brief Searches for a bg_tiles_ptr which references the given tiles.
     * @param tiles_ref Reference to the tiles to search.
     * @return bg_tiles_ptr which references tiles_ref if it has been found; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<bg_tiles_ptr> find(const span<const tile>& tiles_ref);

    /**
     * @brief Searches for a bg_tiles_ptr which references the given tiles.
     * @param tiles_item bg_tiles_item which references the tiles to search.
     * @return bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found;
     * <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<bg_tiles_ptr> find(const bg_tiles_item& tiles_item);

    /**
     * @brief Searches for a bg_tiles_ptr which references the given tiles.
     * If it is not found, it creates a bg_tiles_ptr which references them.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_ref Reference to the tiles to search or handle.
     * @return bg_tiles_ptr which references tiles_ref if it has been found;
     * otherwise it returns a bg_tiles_ptr which references them.
     */
    [[nodiscard]] static bg_tiles_ptr create(const span<const tile>& tiles_ref);

    /**
     * @brief Searches for a bg_tiles_ptr which references the given tiles.
     * If it is not found, it creates a bg_tiles_ptr which references them.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item bg_tiles_item which references the tiles to search or handle.
     * @return bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found;
     * otherwise it returns a bg_tiles_ptr which references them.
     */
    [[nodiscard]] static bg_tiles_ptr create(const bg_tiles_item& tiles_item);

    /**
     * @brief Creates a bg_tiles_ptr which references the given tiles.
     *
     * The background tiles system does not support multiple bg_tiles_ptr items referencing to the same tiles.
     * If you are not sure if the given tiles are already referenced or not,
     * you should use the static create methods instead.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_ref Reference to the tiles to handle.
     * @return bg_tiles_ptr which references tiles_ref.
     */
    [[nodiscard]] static bg_tiles_ptr create_new(const span<const tile>& tiles_ref);

    /**
     * @brief Creates a bg_tiles_ptr which references the given tiles.
     *
     * The background tiles system does not support multiple bg_tiles_ptr items referencing to the same tiles.
     * If you are not sure if the given tiles are already referenced or not,
     * you should use the static create methods instead.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item bg_tiles_item which references the tiles to handle.
     * @return bg_tiles_ptr which references tiles_item.graphics_tiles_ref().
     */
    [[nodiscard]] static bg_tiles_ptr create_new(const bg_tiles_item& tiles_item);

    /**
     * @brief Creates a bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
     * @param tiles_count Number of tiles to allocate.
     * @return bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
     */
    [[nodiscard]] static bg_tiles_ptr allocate(int tiles_count);

    /**
     * @brief Searches for a bg_tiles_ptr which references the given tiles.
     * If it is not found, it creates a bg_tiles_ptr which references them.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_ref Reference to the tiles to search or handle.
     * @return bg_tiles_ptr which references tiles_ref if it has been found;
     * otherwise it returns a bg_tiles_ptr which references them if it could be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<bg_tiles_ptr> create_optional(const span<const tile>& tiles_ref);

    /**
     * @brief Searches for a bg_tiles_ptr which references the given tiles.
     * If it is not found, it creates a bg_tiles_ptr which references them.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item bg_tiles_item which references the tiles to search or handle.
     * @return bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it has been found;
     * otherwise it returns a bg_tiles_ptr which references them if it could be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<bg_tiles_ptr> create_optional(const bg_tiles_item& tiles_item);

    /**
     * @brief Creates a bg_tiles_ptr which references the given tiles.
     *
     * The background tiles system does not support multiple bg_tiles_ptr items referencing to the same tiles.
     * If you are not sure if the given tiles are already referenced or not,
     * you should use the static create_optional methods instead.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_ref Reference to the tiles to handle.
     * @return bg_tiles_ptr which references tiles_ref if it could be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<bg_tiles_ptr> create_new_optional(const span<const tile>& tiles_ref);

    /**
     * @brief Creates a bg_tiles_ptr which references the given tiles.
     *
     * The background tiles system does not support multiple bg_tiles_ptr items referencing to the same tiles.
     * If you are not sure if the given tiles are already referenced or not,
     * you should use the static create_optional methods instead.
     *
     * The tiles are not copied but referenced,
     * so they should outlive the bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item bg_tiles_item which references the tiles to handle.
     * @return bg_tiles_ptr which references tiles_item.graphics_tiles_ref() if it could be allocated;
     * <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<bg_tiles_ptr> create_new_optional(const bg_tiles_item& tiles_item);

    /**
     * @brief Creates a bg_tiles_ptr which references a chunk of VRAM tiles not visible on the screen.
     * @param tiles_count Number of tiles to allocate.
     * @return bg_tiles_ptr which references a chunk of VRAM tiles
     * not visible on the screen if it could be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] static optional<bg_tiles_ptr> allocate_optional(int tiles_count);

    /**
     * @brief Copy constructor.
     * @param other bg_tiles_ptr to copy.
     */
    bg_tiles_ptr(const bg_tiles_ptr& other);

    /**
     * @brief Copy assignment operator.
     * @param other bg_tiles_ptr to copy.
     * @return Reference to this.
     */
    bg_tiles_ptr& operator=(const bg_tiles_ptr& other);

    /**
     * @brief Move constructor.
     * @param other bg_tiles_ptr to move.
     */
    bg_tiles_ptr(bg_tiles_ptr&& other) noexcept :
        bg_tiles_ptr(other._handle)
    {
        other._handle = -1;
    }

    /**
     * @brief Move assignment operator.
     * @param other bg_tiles_ptr to move.
     * @return Reference to this.
     */
    bg_tiles_ptr& operator=(bg_tiles_ptr&& other) noexcept
    {
        btn::swap(_handle, other._handle);
        return *this;
    }

    /**
     * @brief Releases the referenced background tiles if no more bg_tiles_ptr objects reference to them.
     */
    ~bg_tiles_ptr()
    {
        if(_handle >= 0)
        {
            _destroy();
        }
    }

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
     * @brief Indicates if the referenced tiles are valid for the specified bits per pixel or not.
     */
    [[nodiscard]] bool valid_tiles_count(palette_bpp_mode bpp_mode) const;

    /**
     * @brief Returns the referenced tiles unless it was created with allocate or allocate_optional.
     * In that case, it returns <b>nullopt</b>.
     */
    [[nodiscard]] optional<span<const tile>> tiles_ref() const;

    /**
     * @brief Sets the tiles to handle.
     *
     * Remember that the background tiles system does not support multiple bg_tiles_ptr items
     * referencing to the same tiles.
     *
     * Remember also that the tiles are not copied but referenced,
     * so they should outlive the bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_ref Reference to the tiles to handle.
     */
    void set_tiles_ref(const span<const tile>& tiles_ref);

    /**
     * @brief Sets the tiles to handle.
     *
     * Remember that the background tiles system does not support multiple bg_tiles_ptr items
     * referencing to the same tiles.
     *
     * Remember also that the tiles are not copied but referenced,
     * so they should outlive the bg_tiles_ptr to avoid dangling references.
     *
     * @param tiles_item bg_tiles_item which references the tiles to handle.
     */
    void set_tiles_ref(const bg_tiles_item& tiles_item);

    /**
     * @brief Uploads the referenced tiles to VRAM again to make visible the possible changes in them.
     */
    void reload_tiles_ref();

    /**
     * @brief Returns the allocated memory in VRAM
     * if this bg_tiles_ptr was created with allocate or allocate_optional; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] optional<span<tile>> vram();

    /**
     * @brief Returns the hash of the internal handle.
     */
    [[nodiscard]] unsigned hash() const
    {
        return make_hash(_handle);
    }

    /**
     * @brief Exchanges the contents of this bg_tiles_ptr with those of the other one.
     * @param other bg_tiles_ptr to exchange the contents with.
     */
    void swap(bg_tiles_ptr& other)
    {
        btn::swap(_handle, other._handle);
    }

    /**
     * @brief Exchanges the contents of a bg_tiles_ptr with those of another one.
     * @param a First bg_tiles_ptr to exchange the contents with.
     * @param b Second bg_tiles_ptr to exchange the contents with.
     */
    friend void swap(bg_tiles_ptr& a, bg_tiles_ptr& b)
    {
        btn::swap(a._handle, b._handle);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const bg_tiles_ptr& a, const bg_tiles_ptr& b) = default;

private:
    int8_t _handle;

    explicit bg_tiles_ptr(int handle) :
        _handle(int8_t(handle))
    {
    }

    void _destroy();
};


/**
 * @brief Hash support for bg_tiles_ptr.
 *
 * @ingroup bg
 * @ingroup tile
 * @ingroup functional
 */
template<>
struct hash<bg_tiles_ptr>
{
    /**
     * @brief Returns the hash of the given bg_tiles_ptr.
     */
    [[nodiscard]] unsigned operator()(const bg_tiles_ptr& value) const
    {
        return value.hash();
    }
};

}

#endif
