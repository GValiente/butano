/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_TEXT_GENERATOR_H
#define BN_SPRITE_TEXT_GENERATOR_H

/**
 * @file
 * bn::sprite_text_generator header file.
 *
 * @ingroup sprite
 * @ingroup text
 */

#include "bn_vector.h"
#include "bn_fixed_point.h"
#include "bn_sprite_font.h"
#include "bn_string_view.h"

namespace bn
{

class sprite_ptr;

/**
 * @brief Generates sprites containing text from a given sprite_font.
 *
 * Currently, it supports 4 bits per pixel (16 colors) fixed width AND variable width characters.
 *
 * Text can be printed in one sprite per character or multiple characters per sprite.
 *
 * Also, UTF-8 characters are supported.
 *
 * @ingroup sprite
 * @ingroup text
 */
class sprite_text_generator
{

public:
    /**
     * @brief Available horizontal alignment types.
     */
    enum class alignment_type : uint8_t
    {
        LEFT, //!< Aligns with the left text edge.
        CENTER, //!< Aligns with the middle of the text.
        RIGHT //!< Aligns with the right text edge.
    };

    /**
     * @brief Constructor.
     * @param font Sprite font for drawing text.
     */
    explicit sprite_text_generator(const sprite_font& font);

    /**
     * @brief Constructor.
     * @param font Sprite font for drawing text.
     * @param palette_item 16 colors (4 bits per pixel) sprite_palette_item
     * that generates the color palette used by the text sprites.
     */
    sprite_text_generator(const sprite_font& font, const sprite_palette_item& palette_item);

    /**
     * @brief Returns the sprite font for drawing text.
     */
    [[nodiscard]] const sprite_font& font() const
    {
        return _font;
    }

    /**
     * @brief Returns the sprite_palette_item that generates the color palette used by the text sprites.
     */
    [[nodiscard]] const sprite_palette_item& palette_item() const
    {
        return _palette_item;
    }

    /**
     * @brief Sets the 16 colors (4 bits per pixel) sprite_palette_item
     * that generates the color palette used by the text sprites.
     */
    void set_palette_item(const sprite_palette_item& palette_item);

    /**
     * @brief Returns the horizontal alignment of the output text sprites.
     */
    [[nodiscard]] alignment_type alignment() const
    {
        return _alignment;
    }

    /**
     * @brief Sets the horizontal alignment of the output text sprites.
     */
    void set_alignment(alignment_type alignment)
    {
        _alignment = alignment;
    }

    /**
     * @brief Sets the horizontal alignment of the output text sprites to the left.
     */
    void set_left_alignment()
    {
        _alignment = alignment_type::LEFT;
    }

    /**
     * @brief Sets the horizontal alignment of the output text sprites to the center.
     */
    void set_center_alignment()
    {
        _alignment = alignment_type::CENTER;
    }

    /**
     * @brief Sets the horizontal alignment of the output text sprites to the right.
     */
    void set_right_alignment()
    {
        _alignment = alignment_type::RIGHT;
    }

    /**
     * @brief Returns the priority of the output sprites priority relative to backgrounds.
     *
     * Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
     * Sprites cover backgrounds of the same priority.
     */
    [[nodiscard]] int bg_priority() const
    {
        return _bg_priority;
    }

    /**
     * @brief Sets the priority of the output sprites relative to backgrounds.
     *
     * Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
     * Sprites cover backgrounds of the same priority.
     *
     * @param bg_priority Priority relative to backgrounds in the range [0..3].
     */
    void set_bg_priority(int bg_priority);

    /**
     * @brief Returns the priority of the output sprites relative to other sprites.
     *
     * Sprites with higher z orders are drawn first (and therefore can be covered by later sprites).
     */
    [[nodiscard]] int z_order() const
    {
        return _z_order;
    }

    /**
     * @brief Sets the priority of the output sprites relative to other sprites.
     *
     * Sprites with higher z orders are drawn first (and therefore can be covered by later sprites).
     *
     * @param z_order Priority relative to other sprites in the range [-32767..32767].
     */
    void set_z_order(int z_order);

    /**
     * @brief Indicates if this sprite_text_generator must generate one sprite per character or
     * print multiple characters in each output sprite, generating as less sprites as possible.
     */
    [[nodiscard]] bool one_sprite_per_character() const
    {
        return _one_sprite_per_character;
    }

    /**
     * @brief Sets if this sprite_text_generator must generate one sprite per character or
     * print multiple characters in each output sprite, generating as less sprites as possible.
     */
    void set_one_sprite_per_character(bool one_sprite_per_character)
    {
        _one_sprite_per_character = one_sprite_per_character;
    }

    /**
     * @brief Returns the width in pixels of the given text.
     */
    [[nodiscard]] int width(const string_view& text) const;

    /**
     * @brief Generates text sprites for the given single line of text.
     * @tparam MaxSprites Maximum size of the returned sprite_ptr vector.
     * @param x Horizontal position of the first generated sprite, considering the current alignment.
     * @param y Vertical position of the first generated sprite, considering the current alignment.
     * @param text Single line of text to print.
     * @return sprite_ptr vector containing the generated text sprites.
     */
    template<int MaxSprites>
    [[nodiscard]] vector<sprite_ptr, MaxSprites> generate(fixed x, fixed y, const string_view& text) const
    {
        vector<sprite_ptr, MaxSprites> output_sprites;
        generate(x, y, text, output_sprites);
        return output_sprites;
    }

    /**
     * @brief Generates text sprites for the given single line of text.
     * @tparam MaxSprites Maximum size of the returned sprite_ptr vector.
     * @param position Position of the first generated sprite, considering the current alignment.
     * @param text Single line of text to print.
     * @return sprite_ptr vector containing the generated text sprites.
     */
    template<int MaxSprites>
    [[nodiscard]] vector<sprite_ptr, MaxSprites> generate(const fixed_point& position, const string_view& text) const
    {
        vector<sprite_ptr, MaxSprites> output_sprites;
        generate(position, text, output_sprites);
        return output_sprites;
    }

    /**
     * @brief Generates text sprites for the given single line of text.
     * @param x Horizontal position of the first generated sprite, considering the current alignment.
     * @param y Vertical position of the first generated sprite, considering the current alignment.
     * @param text Single line of text to print.
     * @param output_sprites Generated text sprites are stored in this vector.
     *
     * Keep in mind that this vector is not cleared before generating text.
     */
    void generate(fixed x, fixed y, const string_view& text, ivector<sprite_ptr>& output_sprites) const;

    /**
     * @brief Generates text sprites for the given single line of text.
     * @param position Position of the first generated sprite, considering the current alignment.
     * @param text Single line of text to print.
     * @param output_sprites Generated text sprites are stored in this vector.
     *
     * Keep in mind that this vector is not cleared before generating text.
     */
    void generate(const fixed_point& position, const string_view& text, ivector<sprite_ptr>& output_sprites) const;

    /**
     * @brief Generates text sprites for the given single line of text.
     * @param x Horizontal position of the first generated sprite, considering the current alignment.
     * @param y Vertical position of the first generated sprite, considering the current alignment.
     * @param text Single line of text to print.
     * @param output_sprites Generated text sprites are stored in this vector.
     *
     * Keep in mind that this vector is not cleared before generating text.
     *
     * @return `true` if the text generation finished successfully, otherwise `false`.
     */
    [[nodiscard]] bool generate_optional(fixed x, fixed y, const string_view& text,
                                         ivector<sprite_ptr>& output_sprites) const;

    /**
     * @brief Generates text sprites for the given single line of text.
     * @param position Position of the first generated sprite, considering the current alignment.
     * @param text Single line of text to print.
     * @param output_sprites Generated text sprites are stored in this vector.
     *
     * Keep in mind that this vector is not cleared before generating text.
     *
     * @return `true` if the text generation finished successfully, otherwise `false`.
     */
    [[nodiscard]] bool generate_optional(const fixed_point& position, const string_view& text,
                                         ivector<sprite_ptr>& output_sprites) const;

private:
    sprite_font _font;
    sprite_palette_item _palette_item;
    int8_t _bg_priority = 3;
    int8_t _z_order = 0;
    alignment_type _alignment = alignment_type::LEFT;
    bool _one_sprite_per_character = false;
    int8_t _max_character_width;
    int8_t _character_height;
    bool _font_one_sprite_per_character;

    void _init();
};

}

#endif

