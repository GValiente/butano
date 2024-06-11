/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BG_PALETTE_PTR_H
#define BN_BG_PALETTE_PTR_H

/**
 * @file
 * bn::bg_palette_ptr header file.
 *
 * @ingroup bg
 * @ingroup palette
 */

#include "bn_span.h"
#include "bn_fixed.h"
#include "bn_optional.h"

namespace bn
{

class color;
class bg_palette_item;
enum class bpp_mode : uint8_t;

/**
 * @brief std::shared_ptr like smart pointer that retains shared ownership of a background color palette.
 *
 * Several bg_palette_ptr objects may own the same background color palette.
 *
 * The background color palette is released when the last remaining bg_palette_ptr owning it is destroyed.
 *
 * @ingroup bg
 * @ingroup palette
 */
class bg_palette_ptr
{

public:
    /**
     * @brief Searches for a bg_palette_ptr which contains the given colors.
     * @param palette_item bg_palette_item which references the colors to search.
     * @return bg_palette_ptr which contains the referenced colors by palette_item if it has been found;
     * bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<bg_palette_ptr> find(const bg_palette_item& palette_item);

    /**
     * @brief Searches for a bg_palette_ptr which contains the given colors.
     * If it is not found, it creates a bg_palette_ptr which contains them.
     * @param palette_item bg_palette_item which references the colors of the palette to search or create.
     * @return The requested bg_palette_ptr.
     */
    [[nodiscard]] static bg_palette_ptr create(const bg_palette_item& palette_item);

    /**
     * @brief Creates a bg_palette_ptr which contains the given colors.
     * @param palette_item bg_palette_item which references the colors of the new palette.
     * @return The requested bg_palette_ptr.
     */
    [[nodiscard]] static bg_palette_ptr create_new(const bg_palette_item& palette_item);

    /**
     * @brief Searches for a bg_palette_ptr which contains the given colors.
     * If it is not found, it creates a bg_palette_ptr which contains them.
     * @param palette_item bg_palette_item which references the colors of the palette to search or create.
     * @return The requested bg_palette_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<bg_palette_ptr> create_optional(const bg_palette_item& palette_item);

    /**
     * @brief Creates a bg_palette_ptr which contains the given colors.
     * @param palette_item bg_palette_item which references the colors of the new palette.
     * @return The requested bg_palette_ptr if it could be allocated; bn::nullopt otherwise.
     */
    [[nodiscard]] static optional<bg_palette_ptr> create_new_optional(const bg_palette_item& palette_item);

    /**
     * @brief Copy constructor.
     * @param other bg_palette_ptr to copy.
     */
    bg_palette_ptr(const bg_palette_ptr& other);

    /**
     * @brief Copy assignment operator.
     * @param other bg_palette_ptr to copy.
     * @return Reference to this.
     */
    bg_palette_ptr& operator=(const bg_palette_ptr& other);

    /**
     * @brief Move constructor.
     * @param other bg_palette_ptr to move.
     */
    bg_palette_ptr(bg_palette_ptr&& other) noexcept :
        _id(other._id)
    {
        other._id = -1;
    }

    /**
     * @brief Move assignment operator.
     * @param other bg_palette_ptr to move.
     * @return Reference to this.
     */
    bg_palette_ptr& operator=(bg_palette_ptr&& other) noexcept
    {
        bn::swap(_id, other._id);
        return *this;
    }

    /**
     * @brief Releases the referenced background color palette if no more bg_palette_ptr objects reference to it.
     */
    ~bg_palette_ptr();

    /**
     * @brief Returns the internal id.
     */
    [[nodiscard]] int id() const
    {
        return _id;
    }

    /**
     * @brief Returns the number of colors contained in this palette.
     */
    [[nodiscard]] int colors_count() const;

    /**
     * @brief Returns the colors contained in this palette.
     */
    [[nodiscard]] span<const color> colors() const;

    /**
     * @brief Sets the colors contained in this palette.
     * @param palette_item bg_palette_item which references the colors to copy to this palette.
     */
    void set_colors(const bg_palette_item& palette_item);

    /**
     * @brief Sets a single color in this palette.
     * @param color_index Index of the color to set in the palette.
     * @param color New color to set.
     */
    void set_color(int color_index, color color);

    /**
     * @brief Returns the bits per pixel of this palette.
     */
    [[nodiscard]] bpp_mode bpp() const;

    /**
     * @brief Indicates if the colors of this palette are inverted or not.
     */
    [[nodiscard]] bool inverted() const;

    /**
     * @brief Sets if the colors of this palette must be inverted or not.
     */
    void set_inverted(bool inverted);

    /**
     * @brief Returns the intensity of the grayscale effect applied to the colors of this palette.
     */
    [[nodiscard]] fixed grayscale_intensity() const;

    /**
     * @brief Sets the intensity of the grayscale effect applied to the colors of this palette.
     * @param intensity New intensity in the range [0..1].
     */
    void set_grayscale_intensity(fixed intensity);

    /**
     * @brief Returns the intensity of the hue shift effect applied to the colors of this palette.
     */
    [[nodiscard]] fixed hue_shift_intensity() const;

    /**
     * @brief Sets the intensity of the hue shift effect applied to the colors of this palette.
     * @param intensity New intensity in the range [0..1].
     */
    void set_hue_shift_intensity(fixed intensity);

    /**
     * @brief Returns the color of the fade effect applied to the colors of this palette.
     */
    [[nodiscard]] color fade_color() const;

    /**
     * @brief Returns the intensity of the fade effect applied to the colors of this palette.
     */
    [[nodiscard]] fixed fade_intensity() const;

    /**
     * @brief Sets the color of the fade effect applied to the colors of this palette.
     */
    void set_fade_color(color color);

    /**
     * @brief Sets the intensity of the fade effect applied to the colors of this palette.
     * @param intensity New intensity in the range [0..1].
     */
    void set_fade_intensity(fixed intensity);

    /**
     * @brief Sets the color and the intensity of the fade effect applied to the colors of this palette.
     * @param color New color.
     * @param intensity New intensity in the range [0..1].
     */
    void set_fade(color color, fixed intensity);

    /**
     * @brief Returns the number of colors to rotate to the right in this palette.
     */
    [[nodiscard]] int rotate_count() const;

    /**
     * @brief Sets the number of colors to rotate to the right in this palette.
     * @param count Number of colors to rotate to the right in the range
     * [1 - rotate_range_size() .. rotate_range_size() - 1].
     */
    void set_rotate_count(int count);

    /**
     * @brief Returns the starting color index of the rotation range in this palette.
     */
    [[nodiscard]] int rotate_range_start() const;

    /**
     * @brief Returns the number of colors of the rotation range in this palette.
     */
    [[nodiscard]] int rotate_range_size() const;

    /**
     * @brief Sets the rotation range of this palette.
     * @param start Starting color index in the range [0 .. colors_count() - 2].
     * @param size Number of colors in the range [2 .. colors_count()].
     */
    void set_rotate_range(int start, int size);

    /**
     * @brief Exchanges the contents of this bg_palette_ptr with those of the other one.
     * @param other bg_palette_ptr to exchange the contents with.
     */
    void swap(bg_palette_ptr& other)
    {
        bn::swap(_id, other._id);
    }

    /**
     * @brief Exchanges the contents of a bg_palette_ptr with those of another one.
     * @param a First bg_palette_ptr to exchange the contents with.
     * @param b Second bg_palette_ptr to exchange the contents with.
     */
    friend void swap(bg_palette_ptr& a, bg_palette_ptr& b)
    {
        bn::swap(a._id, b._id);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const bg_palette_ptr& a, const bg_palette_ptr& b) = default;

private:
    int8_t _id;

    explicit bg_palette_ptr(int id) :
        _id(int8_t(id))
    {
    }
};


/**
 * @brief Hash support for bg_palette_ptr.
 *
 * @ingroup bg
 * @ingroup palette
 * @ingroup functional
 */
template<>
struct hash<bg_palette_ptr>
{
    /**
     * @brief Returns the hash of the given bg_palette_ptr.
     */
    [[nodiscard]] unsigned operator()(const bg_palette_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif
