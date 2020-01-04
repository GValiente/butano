#ifndef BTN_SPRITE_TEXT_GENERATOR_H
#define BTN_SPRITE_TEXT_GENERATOR_H

#include "btn_fixed.h"
#include "btn_vector.h"
#include "btn_hash_map.h"
#include "btn_sprite_font.h"
#include "btn_sprite_palette_ptr.h"
#include "btn_config_sprite_text.h"
#include "btn_horizontal_alignment_type.h"

namespace btn
{

class sprite_ptr;
class fixed_point;
class string_view;

class sprite_text_generator
{

public:
    explicit sprite_text_generator(const sprite_font& font);

    [[nodiscard]] const sprite_font& font() const
    {
        return _font;
    }

    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return _palette_ptr;
    }

    void set_palette(sprite_palette_ptr palette_ptr);

    [[nodiscard]] horizontal_alignment_type alignment() const
    {
        return _alignment;
    }

    void set_alignment(horizontal_alignment_type alignment)
    {
        _alignment = alignment;
    }

    [[nodiscard]] int bg_priority() const
    {
        return _bg_priority;
    }

    void set_bg_priority(int bg_priority);

    [[nodiscard]] int z_order() const
    {
        return _z_order;
    }

    void set_z_order(int z_order);

    [[nodiscard]] bool ignore_camera() const
    {
        return _ignore_camera;
    }

    void set_ignore_camera(bool ignore_camera)
    {
        _ignore_camera = ignore_camera;
    }

    [[nodiscard]] bool one_sprite_per_character() const
    {
        return _one_sprite_per_character;
    }

    void set_one_sprite_per_character(bool one_sprite_per_character)
    {
        _one_sprite_per_character = one_sprite_per_character;
    }

    [[nodiscard]] fixed width(const string_view& text) const;

    template<size_t MaxSprites>
    [[nodiscard]] vector<sprite_ptr, MaxSprites> generate(fixed x, fixed y, const string_view& text) const
    {
        vector<sprite_ptr, MaxSprites> output_sprites;
        generate(x, y, text, output_sprites);
        return output_sprites;
    }

    template<size_t MaxSprites>
    [[nodiscard]] vector<sprite_ptr, MaxSprites> generate(const fixed_point& position, const string_view& text) const
    {
        vector<sprite_ptr, MaxSprites> output_sprites;
        generate(position, text, output_sprites);
        return output_sprites;
    }

    void generate(fixed x, fixed y, const string_view& text, ivector<sprite_ptr>& output_sprites) const;

    void generate(const fixed_point& position, const string_view& text, ivector<sprite_ptr>& output_sprites) const;

private:
    sprite_font _font;
    sprite_palette_ptr _palette_ptr;
    hash_map<int, int16_t, BTN_CFG_SPRITE_TEXT_MAX_UTF8_CHARACTERS> _ut8_characters_map;
    horizontal_alignment_type _alignment = horizontal_alignment_type::LEFT;
    int _bg_priority = 0;
    int _z_order = 0;
    bool _ignore_camera = true;
    bool _one_sprite_per_character = false;
};

}

#endif

