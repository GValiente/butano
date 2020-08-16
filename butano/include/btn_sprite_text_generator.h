#ifndef BTN_SPRITE_TEXT_GENERATOR_H
#define BTN_SPRITE_TEXT_GENERATOR_H

#include "btn_fixed.h"
#include "btn_vector.h"
#include "btn_sprite_font.h"
#include "btn_config_camera.h"
#include "btn_unordered_map.h"
#include "btn_config_sprite_text.h"
#include "btn_horizontal_alignment_type.h"

namespace btn
{

class sprite_ptr;
class fixed_point;

class sprite_text_generator
{

public:
    using utf8_characters_map_type = unordered_map<int, int, BTN_CFG_SPRITE_TEXT_MAX_UTF8_CHARACTERS>;

    explicit sprite_text_generator(const sprite_font& font);

    [[nodiscard]] const sprite_font& font() const
    {
        return _font;
    }

    [[nodiscard]] const sprite_palette_item& palette_item() const
    {
        return _palette_item;
    }

    void set_palette_item(const sprite_palette_item& palette_item);

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

    #if BTN_CFG_CAMERA_ENABLED
        [[nodiscard]] bool ignore_camera() const
        {
            return _ignore_camera;
        }

        void set_ignore_camera(bool ignore_camera)
        {
            _ignore_camera = ignore_camera;
        }
    #endif

    [[nodiscard]] bool one_sprite_per_character() const
    {
        return _one_sprite_per_character;
    }

    void set_one_sprite_per_character(bool one_sprite_per_character)
    {
        _one_sprite_per_character = one_sprite_per_character;
    }

    [[nodiscard]] const utf8_characters_map_type& utf8_characters_map() const
    {
        return _utf8_characters_map;
    }

    [[nodiscard]] int width(const string_view& text) const;

    template<int MaxSprites>
    [[nodiscard]] vector<sprite_ptr, MaxSprites> generate(fixed x, fixed y, const string_view& text) const
    {
        vector<sprite_ptr, MaxSprites> output_sprites;
        generate(x, y, text, output_sprites);
        return output_sprites;
    }

    template<int MaxSprites>
    [[nodiscard]] vector<sprite_ptr, MaxSprites> generate(const fixed_point& position, const string_view& text) const
    {
        vector<sprite_ptr, MaxSprites> output_sprites;
        generate(position, text, output_sprites);
        return output_sprites;
    }

    void generate(fixed x, fixed y, const string_view& text, ivector<sprite_ptr>& output_sprites) const;

    void generate(const fixed_point& position, const string_view& text, ivector<sprite_ptr>& output_sprites) const;

    [[nodiscard]] bool generate_optional(fixed x, fixed y, const string_view& text,
                                         ivector<sprite_ptr>& output_sprites) const;

    [[nodiscard]] bool generate_optional(const fixed_point& position, const string_view& text,
                                         ivector<sprite_ptr>& output_sprites) const;

private:
    sprite_font _font;
    sprite_palette_item _palette_item;
    utf8_characters_map_type _utf8_characters_map;
    horizontal_alignment_type _alignment = horizontal_alignment_type::LEFT;
    int _bg_priority = 3;
    int _z_order = 0;
    #if BTN_CFG_CAMERA_ENABLED
        bool _ignore_camera = true;
    #endif
    bool _one_sprite_per_character = false;
};

}

#endif

