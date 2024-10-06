/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_text_generator.h"

#include "bn_sprites.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_builder.h"
#include "../hw/include/bn_hw_sprite_tiles.h"

namespace bn
{

namespace
{
    template<sprite_size size>
    [[nodiscard]] tile* _build_sprite_optional(
        const sprite_text_generator& generator, const sprite_palette_ptr& palette,
        const fixed_point& current_position, ivector<sprite_ptr>& output_sprites)
    {
        constexpr sprite_shape_size shape_size(sprite_shape::WIDE, size);
        constexpr int tiles_count = (shape_size.width() / 8) * (shape_size.height() / 8);

        if(output_sprites.full())
        {
            return nullptr;
        }

        optional<sprite_tiles_ptr> tiles = sprite_tiles_ptr::allocate_optional(tiles_count, bpp_mode::BPP_4);
        sprite_tiles_ptr* tiles_ptr = tiles.get();

        if(! tiles_ptr)
        {
            return nullptr;
        }

        optional<span<tile>> tiles_vram = tiles_ptr->vram();

        sprite_builder builder(shape_size, move(*tiles_ptr), palette);
        builder.set_position(current_position);
        builder.set_bg_priority(generator.bg_priority());
        builder.set_z_order(generator.z_order());

        optional<sprite_ptr> sprite = sprite_ptr::create_optional(move(builder));
        sprite_ptr* sprite_ptr = sprite.get();

        if(! sprite_ptr)
        {
            return nullptr;
        }

        output_sprites.push_back(move(*sprite_ptr));
        return tiles_vram->data();
    }

    template<sprite_size size>
    [[nodiscard]] tile* _build_sprite_assert(
        const sprite_text_generator& generator, const sprite_palette_ptr& palette,
        const fixed_point& current_position, ivector<sprite_ptr>& output_sprites)
    {
        constexpr sprite_shape_size shape_size(sprite_shape::WIDE, size);
        constexpr int tiles_count = (shape_size.width() / 8) * (shape_size.height() / 8);

        BN_BASIC_ASSERT(! output_sprites.full(), "output_sprites vector is full,\ncan't hold more sprites");

        sprite_tiles_ptr tiles_ptr = sprite_tiles_ptr::allocate(tiles_count, bpp_mode::BPP_4);
        optional<span<tile>> tiles_vram = tiles_ptr.vram();

        sprite_builder builder(shape_size, move(tiles_ptr), palette);
        builder.set_position(current_position);
        builder.set_bg_priority(generator.bg_priority());
        builder.set_z_order(generator.z_order());
        output_sprites.push_back(sprite_ptr::create(move(builder)));
        return tiles_vram->data();
    }

    template<sprite_size size, bool allow_failure>
    [[nodiscard]] tile* _build_sprite(const sprite_text_generator& generator, const sprite_palette_ptr& palette,
                                      const fixed_point& current_position, ivector<sprite_ptr>& output_sprites)
    {
        if(allow_failure)
        {
            return _build_sprite_optional<size>(generator, palette, current_position, output_sprites);
        }

        return _build_sprite_assert<size>(generator, palette, current_position, output_sprites);
    }


    class fixed_width_no_space_between_characters_painter
    {

    public:
        static constexpr bool can_fail = false;

        explicit fixed_width_no_space_between_characters_painter(int character_width) :
            _character_width(character_width)
        {
        }

        [[nodiscard]] int width() const
        {
            return _width;
        }

        void paint_space()
        {
            _width += _character_width;
        }

        void paint_tab()
        {
            _width += _character_width * 4;
        }

        [[nodiscard]] bool paint_character([[maybe_unused]] int graphics_index)
        {
            _width += _character_width;
            return true;
        }

    private:
        int _character_width;
        int _width = 0;
    };


    class fixed_width_space_between_characters_painter
    {

    public:
        static constexpr bool can_fail = false;

        fixed_width_space_between_characters_painter(int character_width, int space_between_characters) :
            _character_width(character_width),
            _space_between_characters(space_between_characters)
        {
        }

        [[nodiscard]] int width() const
        {
            return _width;
        }

        void paint_space()
        {
            _width += _character_width + _space_between_characters;
        }

        void paint_tab()
        {
            _width += (_character_width * 4) + _space_between_characters;
        }

        [[nodiscard]] bool paint_character([[maybe_unused]] int graphics_index)
        {
            _width += _character_width + _space_between_characters;
            return true;
        }

    private:
        int _character_width;
        int _space_between_characters;
        int _width = 0;
    };


    class variable_width_no_space_between_characters_painter
    {

    public:
        static constexpr bool can_fail = false;

        explicit variable_width_no_space_between_characters_painter(const int8_t* character_widths) :
            _character_widths(character_widths)
        {
        }

        [[nodiscard]] int width() const
        {
            return _width;
        }

        void paint_space()
        {
            _width += _character_widths[0];
        }

        void paint_tab()
        {
            _width += _character_widths[0] * 4;
        }

        [[nodiscard]] bool paint_character(int graphics_index)
        {
            _width += _character_widths[graphics_index + 1];
            return true;
        }

    private:
        const int8_t* _character_widths;
        int _width = 0;
    };


    class variable_width_space_between_characters_painter
    {

    public:
        static constexpr bool can_fail = false;

        variable_width_space_between_characters_painter(const int8_t* character_widths, int space_between_characters) :
            _character_widths(character_widths),
            _space_between_characters(space_between_characters)
        {
        }

        [[nodiscard]] int width() const
        {
            return _width;
        }

        void paint_space()
        {
            _width += _character_widths[0] + _space_between_characters;
        }

        void paint_tab()
        {
            _width += (_character_widths[0] * 4) + _space_between_characters;
        }

        [[nodiscard]] bool paint_character(int graphics_index)
        {
            _width += _character_widths[graphics_index + 1] + _space_between_characters;
            return true;
        }

    private:
        const int8_t* _character_widths;
        int _space_between_characters;
        int _width = 0;
    };


    template<bool allow_failure>
    class fixed_one_sprite_per_character_painter
    {

    public:
        static constexpr bool can_fail = allow_failure;

        fixed_one_sprite_per_character_painter(
                const sprite_text_generator& generator, sprite_palette_ptr&& palette,
                const fixed_point& position, int character_width, ivector<sprite_ptr>& output_sprites) :
            _generator(generator),
            _output_sprites(output_sprites),
            _current_position(position.x() + (character_width / 2), position.y()),
            _character_width(character_width),
            _space_between_characters(generator.font().space_between_characters()),
            _palette(move(palette))
        {
        }

        void paint_space()
        {
            _current_position.set_x(_current_position.x() + _character_width + _space_between_characters);
        }

        void paint_tab()
        {
            _current_position.set_x(_current_position.x() + (_character_width * 4) + _space_between_characters);
        }

        [[nodiscard]] bool paint_character(int graphics_index)
        {
            if(allow_failure)
            {
                if(_output_sprites.full())
                {
                    return false;
                }
            }
            else
            {
                BN_BASIC_ASSERT(! _output_sprites.full(), "output_sprites vector is full,\ncan't hold more sprites");
            }

            const sprite_item& item = _generator.font().item();
            const sprite_tiles_item& tiles_item = item.tiles_item();
            optional<sprite_tiles_ptr> source_tiles;

            if(allow_failure)
            {
                source_tiles = sprite_tiles_ptr::create_optional(tiles_item, graphics_index);

                if(! source_tiles)
                {
                    return false;
                }
            }
            else
            {
                source_tiles = sprite_tiles_ptr::create(tiles_item, graphics_index);
            }

            sprite_builder builder(item.shape_size(), move(*source_tiles), _palette);
            builder.set_position(_current_position);
            builder.set_bg_priority(_generator.bg_priority());
            builder.set_z_order(_generator.z_order());

            if(allow_failure)
            {
                optional<sprite_ptr> sprite = sprite_ptr::create_optional(move(builder));
                sprite_ptr* sprite_ptr = sprite.get();

                if(! sprite_ptr)
                {
                    return false;
                }

                _output_sprites.push_back(move(*sprite_ptr));
            }
            else
            {
                _output_sprites.push_back(sprite_ptr::create(move(builder)));
            }

            _current_position.set_x(_current_position.x() + _character_width + _space_between_characters);
            return true;
        }

    private:
        const sprite_text_generator& _generator;
        ivector<sprite_ptr>& _output_sprites;
        fixed_point _current_position;
        int _character_width;
        int _space_between_characters;
        sprite_palette_ptr _palette;
    };


    template<bool allow_failure>
    class variable_one_sprite_per_character_painter
    {

    public:
        static constexpr bool can_fail = allow_failure;

        variable_one_sprite_per_character_painter(
                const sprite_text_generator& generator, sprite_palette_ptr&& palette, const fixed_point& position,
                int max_character_width, ivector<sprite_ptr>& output_sprites) :
            _generator(generator),
            _character_widths(generator.font().character_widths_ref().data()),
            _output_sprites(output_sprites),
            _current_position(position.x() + (max_character_width / 2), position.y()),
            _space_between_characters(generator.font().space_between_characters()),
            _palette(move(palette))
        {
        }

        void paint_space()
        {
            _current_position.set_x(_current_position.x() + _character_widths[0] + _space_between_characters);
        }

        void paint_tab()
        {
            _current_position.set_x(_current_position.x() + (_character_widths[0] * 4) + _space_between_characters);
        }

        [[nodiscard]] bool paint_character(int graphics_index)
        {
            if(int character_width = _character_widths[graphics_index + 1])
            {
                if(allow_failure)
                {
                    if(_output_sprites.full())
                    {
                        return false;
                    }
                }
                else
                {
                    BN_BASIC_ASSERT(! _output_sprites.full(), "output_sprites vector is full,\ncan't hold more sprites");
                }

                const sprite_item& item = _generator.font().item();
                const sprite_tiles_item& tiles_item = item.tiles_item();
                optional<sprite_tiles_ptr> source_tiles;

                if(allow_failure)
                {
                    source_tiles = sprite_tiles_ptr::create_optional(tiles_item, graphics_index);

                    if(! source_tiles)
                    {
                        return false;
                    }
                }
                else
                {
                    source_tiles = sprite_tiles_ptr::create(tiles_item, graphics_index);
                }

                sprite_builder builder(item.shape_size(), move(*source_tiles), _palette);
                builder.set_position(_current_position);
                builder.set_bg_priority(_generator.bg_priority());
                builder.set_z_order(_generator.z_order());

                if(allow_failure)
                {
                    optional<sprite_ptr> sprite = sprite_ptr::create_optional(move(builder));
                    sprite_ptr* sprite_ptr = sprite.get();

                    if(! sprite_ptr)
                    {
                        return false;
                    }

                    _output_sprites.push_back(move(*sprite_ptr));
                }
                else
                {
                    _output_sprites.push_back(sprite_ptr::create(move(builder)));
                }

                _current_position.set_x(_current_position.x() + character_width + _space_between_characters);
            }
            else
            {
                _current_position.set_x(_current_position.x() + _space_between_characters);
            }

            return true;
        }

    private:
        const sprite_text_generator& _generator;
        const int8_t* _character_widths;
        ivector<sprite_ptr>& _output_sprites;
        fixed_point _current_position;
        int _space_between_characters;
        sprite_palette_ptr _palette;
    };


    template<int character_width, bool allow_failure>
    class fixed_height_8_painter
    {

    public:
        static constexpr bool can_fail = allow_failure;

        fixed_height_8_painter(const sprite_text_generator& generator, sprite_palette_ptr&& palette,
                               const fixed_point& position, ivector<sprite_ptr>& output_sprites) :
            _generator(generator),
            _output_sprites(output_sprites),
            _current_position(position.x() + (_max_columns_per_sprite / 2), position.y()),
            _space_between_characters(generator.font().space_between_characters()),
            _palette(move(palette))
        {
        }

        ~fixed_height_8_painter()
        {
            _clear_left();
        }

        void paint_space()
        {
            if(_sprite_character_index < _max_characters_per_sprite)
            {
                _clear(1);
                ++_sprite_character_index;
            }

            _current_position.set_x(_current_position.x() + character_width + _space_between_characters);
        }

        void paint_tab()
        {
            _clear_left();
            _current_position.set_x(_current_position.x() + (character_width * 4) + _space_between_characters);
        }

        [[nodiscard]] bool paint_character(int graphics_index)
        {
            tile* tiles_vram;

            if(_sprite_character_index == _max_characters_per_sprite)
            {
                tiles_vram = _build_sprite<sprite_size::NORMAL, allow_failure>(
                            _generator, _palette, _current_position, _output_sprites);
                _tiles_vram = tiles_vram;

                if(allow_failure && ! tiles_vram)
                {
                    return false;
                }

                _sprite_character_index = 0;
            }
            else
            {
                tiles_vram = _tiles_vram;
            }

            const sprite_item& item = _generator.font().item();
            const tile* source_tiles_data = item.tiles_item().graphics_tiles_ref(graphics_index).data();
            hw::sprite_tiles::copy_tiles(source_tiles_data, _tiles_per_character,
                                         tiles_vram + (_sprite_character_index * _tiles_per_character));

            _current_position.set_x(_current_position.x() + character_width + _space_between_characters);
            ++_sprite_character_index;
            return true;
        }

    private:
        static constexpr int _max_columns_per_sprite = 32;
        static constexpr int _tiles_per_character = character_width / 8;
        static constexpr int _max_characters_per_sprite = _max_columns_per_sprite / character_width;

        const sprite_text_generator& _generator;
        ivector<sprite_ptr>& _output_sprites;
        fixed_point _current_position;
        tile* _tiles_vram = nullptr;
        int _sprite_character_index = _max_characters_per_sprite;
        int _space_between_characters;
        sprite_palette_ptr _palette;

        void _clear(int characters) const
        {
            hw::sprite_tiles::clear_tiles(characters * _tiles_per_character,
                                          _tiles_vram + (_sprite_character_index * _tiles_per_character));
        }

        void _clear_left()
        {
            if(_sprite_character_index < _max_characters_per_sprite)
            {
                _clear(_max_characters_per_sprite - _sprite_character_index);
                _sprite_character_index = _max_characters_per_sprite;
            }
        }
    };


    template<bool allow_failure>
    class variable_8x8_painter
    {

    public:
        static constexpr bool can_fail = allow_failure;

        variable_8x8_painter(const sprite_text_generator& generator, sprite_palette_ptr&& palette,
                             const fixed_point& position, ivector<sprite_ptr>& output_sprites) :
            _generator(generator),
            _character_widths(generator.font().character_widths_ref().data()),
            _output_sprites(output_sprites),
            _current_position(position.x() + (_max_columns_per_sprite / 2), position.y()),
            _space_between_characters(generator.font().space_between_characters()),
            _palette(move(palette))
        {
        }

        void paint_space()
        {
            int width_with_space = _character_widths[0] + _space_between_characters;
            _sprite_column += width_with_space;
            _current_position.set_x(_current_position.x() + width_with_space);
        }

        void paint_tab()
        {
            int width_with_space = (_character_widths[0] * 4) + _space_between_characters;
            _sprite_column += width_with_space;
            _current_position.set_x(_current_position.x() + width_with_space);
        }

        [[nodiscard]] bool paint_character(int graphics_index)
        {
            if(int width = _character_widths[graphics_index + 1])
            {
                tile* tiles_vram;
                int width_with_space = width + _space_between_characters;

                if(_sprite_column + width_with_space > _max_columns_per_sprite)
                {
                    tiles_vram = _build_sprite<sprite_size::NORMAL, allow_failure>(
                                _generator, _palette, _current_position, _output_sprites);
                    _tiles_vram = tiles_vram;

                    if(allow_failure && ! tiles_vram)
                    {
                        return false;
                    }

                    hw::sprite_tiles::clear_tiles(4, tiles_vram);
                    _sprite_column = 0;
                }
                else
                {
                    tiles_vram = _tiles_vram;
                }

                const sprite_tiles_item& tiles_item = _generator.font().item().tiles_item();
                const tile* source_tiles_data = tiles_item.tiles_ref().data();
                int source_y = graphics_index * _character_height;
                hw::sprite_tiles::plot_tiles(width, source_tiles_data, source_y, _sprite_column, tiles_vram);

                _current_position.set_x(_current_position.x() + width_with_space);
                _sprite_column += width_with_space;
            }
            else
            {
                _current_position.set_x(_current_position.x() + _space_between_characters);
                _sprite_column += _space_between_characters;
            }

            return true;
        }

    private:
        static constexpr int _character_height = 8;
        static constexpr int _max_columns_per_sprite = 32;

        const sprite_text_generator& _generator;
        const int8_t* _character_widths;
        ivector<sprite_ptr>& _output_sprites;
        fixed_point _current_position;
        tile* _tiles_vram = nullptr;
        int _space_between_characters;
        int _sprite_column = _max_columns_per_sprite;
        sprite_palette_ptr _palette;
    };


    template<int character_width, bool allow_failure>
    class fixed_height_16_painter
    {

    public:
        static constexpr bool can_fail = allow_failure;

        fixed_height_16_painter(const sprite_text_generator& generator, sprite_palette_ptr&& palette,
                                const fixed_point& position, ivector<sprite_ptr>& output_sprites) :
            _generator(generator),
            _output_sprites(output_sprites),
            _current_position(position.x() + (_max_columns_per_sprite / 2), position.y()),
            _space_between_characters(generator.font().space_between_characters()),
            _palette(move(palette))
        {
        }

        ~fixed_height_16_painter()
        {
            _clear_left();
        }

        void paint_space()
        {
            if(_sprite_character_index < _max_characters_per_sprite)
            {
                _clear(1);
                ++_sprite_character_index;
            }

            _current_position.set_x(_current_position.x() + character_width + _space_between_characters);
        }

        void paint_tab()
        {
            _clear_left();
            _current_position.set_x(_current_position.x() + (character_width * 4) + _space_between_characters);
        }

        [[nodiscard]] bool paint_character(int graphics_index)
        {
            tile* tiles_vram;

            if(_sprite_character_index == _max_characters_per_sprite)
            {
                tiles_vram = _build_sprite<sprite_size::BIG, allow_failure>(
                            _generator, _palette, _current_position, _output_sprites);
                _tiles_vram = tiles_vram;

                if(allow_failure && ! tiles_vram)
                {
                    return false;
                }

                _sprite_character_index = 0;
            }
            else
            {
                tiles_vram = _tiles_vram;
            }

            const sprite_item& item = _generator.font().item();
            const tile* source_tiles_data = item.tiles_item().graphics_tiles_ref(graphics_index).data();
            tile* up_tiles_vram_ptr = tiles_vram + (_sprite_character_index * _half_tiles_per_character);
            hw::sprite_tiles::copy_tiles(source_tiles_data, _half_tiles_per_character, up_tiles_vram_ptr);

            tile* down_tiles_vram_ptr = up_tiles_vram_ptr + _half_tiles;
            hw::sprite_tiles::copy_tiles(source_tiles_data + _half_tiles_per_character, _half_tiles_per_character,
                                         down_tiles_vram_ptr);

            _current_position.set_x(_current_position.x() + character_width + _space_between_characters);
            ++_sprite_character_index;
            return true;
        }

    private:
        static constexpr int _max_columns_per_sprite = 32;
        static constexpr int _max_characters_per_sprite = _max_columns_per_sprite / character_width;
        static constexpr int _half_tiles = _max_columns_per_sprite / 8;
        static constexpr int _half_tiles_per_character = character_width / 8;

        const sprite_text_generator& _generator;
        ivector<sprite_ptr>& _output_sprites;
        fixed_point _current_position;
        tile* _tiles_vram = nullptr;
        int _space_between_characters;
        int _sprite_character_index = _max_characters_per_sprite;
        sprite_palette_ptr _palette;

        void _clear(int characters) const
        {
            int half_tiles_count = characters * _half_tiles_per_character;
            tile* up_tiles_vram_ptr = _tiles_vram + (_sprite_character_index * _half_tiles_per_character);
            hw::sprite_tiles::clear_tiles(half_tiles_count, up_tiles_vram_ptr);

            tile* down_tiles_vram_ptr = up_tiles_vram_ptr + _half_tiles;
            hw::sprite_tiles::clear_tiles(half_tiles_count, down_tiles_vram_ptr);
        }

        void _clear_left()
        {
            if(_sprite_character_index < _max_characters_per_sprite)
            {
                _clear(_max_characters_per_sprite - _sprite_character_index);
                _sprite_character_index = _max_characters_per_sprite;
            }
        }
    };


    template<bool allow_failure>
    class variable_8x16_painter
    {

    public:
        static constexpr bool can_fail = allow_failure;

        variable_8x16_painter(const sprite_text_generator& generator, sprite_palette_ptr&& palette,
                              const fixed_point& position, ivector<sprite_ptr>& output_sprites) :
            _generator(generator),
            _character_widths(generator.font().character_widths_ref().data()),
            _output_sprites(output_sprites),
            _current_position(position.x() + (_max_columns_per_sprite / 2), position.y()),
            _space_between_characters(generator.font().space_between_characters()),
            _palette(move(palette))
        {
        }

        void paint_space()
        {
            int width_with_space = _character_widths[0] + _space_between_characters;
            _sprite_column += width_with_space;
            _current_position.set_x(_current_position.x() + width_with_space);
        }

        void paint_tab()
        {
            int width_with_space = (_character_widths[0] * 4) + _space_between_characters;
            _sprite_column += width_with_space;
            _current_position.set_x(_current_position.x() + width_with_space);
        }

        [[nodiscard]] bool paint_character(int graphics_index)
        {
            if(int width = _character_widths[graphics_index + 1])
            {
                tile* tiles_vram;
                int width_with_space = width + _space_between_characters;

                if(_sprite_column + width_with_space > _max_columns_per_sprite)
                {
                    tiles_vram = _build_sprite<sprite_size::BIG, allow_failure>(
                                _generator, _palette, _current_position, _output_sprites);
                    _tiles_vram = tiles_vram;

                    if(allow_failure && ! tiles_vram)
                    {
                        return false;
                    }

                    hw::sprite_tiles::clear_tiles(4 * 2, tiles_vram);
                    _sprite_column = 0;
                }
                else
                {
                    tiles_vram = _tiles_vram;
                }

                const sprite_tiles_item& tiles_item = _generator.font().item().tiles_item();
                const tile* source_tiles_data = tiles_item.tiles_ref().data();
                int source_y = graphics_index * _character_height;
                hw::sprite_tiles::plot_tiles(width, source_tiles_data, source_y, _sprite_column, tiles_vram);
                hw::sprite_tiles::plot_tiles(width, source_tiles_data, source_y + (_character_height / 2),
                                             _sprite_column + (_character_height * 2), tiles_vram);

                _current_position.set_x(_current_position.x() + width_with_space);
                _sprite_column += width_with_space;
            }
            else
            {
                _current_position.set_x(_current_position.x() + _space_between_characters);
                _sprite_column += _space_between_characters;
            }

            return true;
        }

    private:
        static constexpr int _character_height = 16;
        static constexpr int _max_columns_per_sprite = 32;

        const sprite_text_generator& _generator;
        const int8_t* _character_widths;
        ivector<sprite_ptr>& _output_sprites;
        fixed_point _current_position;
        tile* _tiles_vram = nullptr;
        int _space_between_characters;
        int _sprite_column = _max_columns_per_sprite;
        sprite_palette_ptr _palette;
    };


    template<bool allow_failure>
    class variable_16x16_painter
    {

    public:
        static constexpr bool can_fail = allow_failure;

        variable_16x16_painter(const sprite_text_generator& generator, sprite_palette_ptr&& palette,
                               const fixed_point& position, ivector<sprite_ptr>& output_sprites) :
            _generator(generator),
            _character_widths(generator.font().character_widths_ref().data()),
            _output_sprites(output_sprites),
            _current_position(position.x() + (_max_columns_per_sprite / 2), position.y()),
            _space_between_characters(generator.font().space_between_characters()),
            _palette(move(palette))
        {
        }

        void paint_space()
        {
            int width_with_space = _character_widths[0] + _space_between_characters;
            _sprite_column += width_with_space;
            _current_position.set_x(_current_position.x() + width_with_space);
        }

        void paint_tab()
        {
            int width_with_space = (_character_widths[0] * 4) + _space_between_characters;
            _sprite_column += width_with_space;
            _current_position.set_x(_current_position.x() + width_with_space);
        }

        [[nodiscard]] bool paint_character(int graphics_index)
        {
            if(int width = _character_widths[graphics_index + 1])
            {
                tile* tiles_vram;
                int width_with_space = width + _space_between_characters;

                if(_sprite_column + width_with_space > _max_columns_per_sprite)
                {
                    tiles_vram = _build_sprite<sprite_size::BIG, allow_failure>(
                                _generator, _palette, _current_position, _output_sprites);
                    _tiles_vram = tiles_vram;

                    if(allow_failure && ! tiles_vram)
                    {
                        return false;
                    }

                    hw::sprite_tiles::clear_tiles(4 * 2, tiles_vram);
                    _sprite_column = 0;
                }
                else
                {
                    tiles_vram = _tiles_vram;
                }

                const sprite_tiles_item& tiles_item = _generator.font().item().tiles_item();
                const tile* source_tiles_data = tiles_item.tiles_ref().data();
                int source_y = graphics_index * _character_height * 2;

                if(width > 8)
                {
                    hw::sprite_tiles::plot_tiles(width, source_tiles_data, source_y,
                                                 _sprite_column, tiles_vram);
                    hw::sprite_tiles::plot_tiles(width, source_tiles_data, source_y + _character_height,
                                                 _sprite_column + (_character_height * 2), tiles_vram);
                    source_tiles_data += 1;
                    tiles_vram += 1;
                    width -= 8;
                }

                hw::sprite_tiles::plot_tiles(width, source_tiles_data, source_y,
                                             _sprite_column, tiles_vram);
                hw::sprite_tiles::plot_tiles(width, source_tiles_data, source_y + _character_height,
                                             _sprite_column + (_character_height * 2), tiles_vram);

                _current_position.set_x(_current_position.x() + width_with_space);
                _sprite_column += width_with_space;
            }
            else
            {
                _current_position.set_x(_current_position.x() + _space_between_characters);
                _sprite_column += _space_between_characters;
            }

            return true;
        }

    private:
        static constexpr int _character_height = 16;
        static constexpr int _max_columns_per_sprite = 32;

        const sprite_text_generator& _generator;
        const int8_t* _character_widths;
        ivector<sprite_ptr>& _output_sprites;
        fixed_point _current_position;
        tile* _tiles_vram = nullptr;
        int _space_between_characters;
        int _sprite_column = _max_columns_per_sprite;
        sprite_palette_ptr _palette;
    };


    [[nodiscard]] int _graphics_index(char character, const utf8_characters_map_ref& utf8_characters_map,
                                      const char* text_data, int& text_index)
    {
        int result;

        if(character <= '~')
        {
            result = character - '!';
            ++text_index;
        }
        else
        {
            utf8_character utf8_char(text_data[text_index]);
            result = utf8_characters_map.index(utf8_char) + sprite_font::minimum_graphics;
            text_index += utf8_char.size();
        }

        return result;
    }


    template<class Painter>
    [[nodiscard]] bool _paint(const string_view& text, const utf8_characters_map_ref& utf8_characters_map,
                              Painter& painter)
    {
        const char* text_data = text.data();
        int text_index = 0;
        int text_size = text.size();

        while(text_index < text_size)
        {
            char character = text_data[text_index];

            if(character == ' ')
            {
                painter.paint_space();
                ++text_index;
            }
            else if(character == '\t')
            {
                painter.paint_tab();
                ++text_index;
            }
            else if(character >= '!')
            {
                int graphics_index = _graphics_index(character, utf8_characters_map, text_data, text_index);
                bool success = painter.paint_character(graphics_index);

                if(Painter::can_fail && ! success)
                {
                    return false;
                }
            }
            else
            {
                BN_ERROR("Invalid character: ", character, " (text: ", text, ")");
            }
        }

        return true;
    }

    template<bool allow_failure>
    bool _generate(const sprite_text_generator& generator, const fixed_point& position, const string_view& text,
                   const utf8_characters_map_ref& utf8_characters_map, int max_character_width, int character_height,
                   bool one_sprite_per_character, ivector<sprite_ptr>& output_sprites)
    {
        optional<sprite_palette_ptr> palette;
        sprite_palette_ptr* palette_ptr;

        if(allow_failure)
        {
            palette = generator.palette_item().create_palette_optional();
            palette_ptr = palette.get();

            if(! palette_ptr)
            {
                return false;
            }
        }
        else
        {
            palette = generator.palette_item().create_palette();
            palette_ptr = palette.get();
        }

        fixed_point aligned_position = position;

        switch(generator.alignment())
        {

        case sprite_text_generator::alignment_type::LEFT:
            break;

        case sprite_text_generator::alignment_type::CENTER:
            aligned_position.set_x(aligned_position.x() - (generator.width(text) / 2));
            break;

        case sprite_text_generator::alignment_type::RIGHT:
            aligned_position.set_x(aligned_position.x() - generator.width(text));
            break;

        default:
            BN_ERROR("Invalid alignment: ", int(generator.alignment()));
            break;
        }

        const sprite_font& font = generator.font();
        int output_sprites_count = output_sprites.size();
        bool fixed_width = font.character_widths_ref().empty();
        bool success;

        if(one_sprite_per_character)
        {
            if(fixed_width)
            {
                fixed_one_sprite_per_character_painter<allow_failure> painter(
                            generator, move(*palette_ptr), aligned_position, max_character_width, output_sprites);
                success = _paint(text, utf8_characters_map, painter);
            }
            else
            {
                variable_one_sprite_per_character_painter<allow_failure> painter(
                            generator, move(*palette_ptr), aligned_position, max_character_width, output_sprites);
                success = _paint(text, utf8_characters_map, painter);
            }
        }
        else
        {
            if(character_height == 8)
            {
                if(fixed_width)
                {
                    if(max_character_width == 8)
                    {
                        fixed_height_8_painter<8, allow_failure> painter(
                                    generator, move(*palette_ptr), aligned_position, output_sprites);
                        success = _paint(text, utf8_characters_map, painter);
                    }
                    else
                    {
                        fixed_height_8_painter<16, allow_failure> painter(
                                    generator, move(*palette_ptr), aligned_position, output_sprites);
                        success = _paint(text, utf8_characters_map, painter);
                    }
                }
                else
                {
                    variable_8x8_painter<allow_failure> painter(
                                generator, move(*palette_ptr), aligned_position, output_sprites);
                    success = _paint(text, utf8_characters_map, painter);
                }
            }
            else
            {
                if(fixed_width)
                {
                    if(max_character_width == 8)
                    {
                        fixed_height_16_painter<8, allow_failure> painter(
                                    generator, move(*palette_ptr), aligned_position, output_sprites);
                        success = _paint(text, utf8_characters_map, painter);
                    }
                    else
                    {
                        fixed_height_16_painter<16, allow_failure> painter(
                                    generator, move(*palette_ptr), aligned_position, output_sprites);
                        success = _paint(text, utf8_characters_map, painter);
                    }
                }
                else
                {
                    if(max_character_width == 8)
                    {
                        variable_8x16_painter<allow_failure> painter(
                                    generator, move(*palette_ptr), aligned_position, output_sprites);
                        success = _paint(text, utf8_characters_map, painter);
                    }
                    else
                    {
                        variable_16x16_painter<allow_failure> painter(
                                    generator, move(*palette_ptr), aligned_position, output_sprites);
                        success = _paint(text, utf8_characters_map, painter);
                    }
                }
            }
        }

        if(allow_failure && ! success)
        {
            output_sprites.shrink(output_sprites_count);
        }

        return success;
    }
}

sprite_text_generator::sprite_text_generator(const sprite_font& font) :
    _font(font),
    _palette_item(font.item().palette_item())
{
    _init();
}

sprite_text_generator::sprite_text_generator(const sprite_font& font, const sprite_palette_item& palette_item) :
    _font(font),
    _palette_item(palette_item)
{
    BN_ASSERT(palette_item.bpp() == bpp_mode::BPP_4, "8BPP fonts not supported");

    _init();
}

void sprite_text_generator::set_palette_item(const sprite_palette_item& palette_item)
{
    BN_ASSERT(palette_item.bpp() == bpp_mode::BPP_4, "8BPP fonts not supported");

    _palette_item = palette_item;
}

void sprite_text_generator::set_bg_priority(int bg_priority)
{
    BN_ASSERT(bg_priority >= 0 && bg_priority <= sprites::max_bg_priority(), "Invalid BG priority: ", bg_priority);

    _bg_priority = int8_t(bg_priority);
}

void sprite_text_generator::set_z_order(int z_order)
{
    BN_ASSERT(z_order >= sprites::min_z_order() && z_order <= sprites::max_z_order(), "Invalid z order: ", z_order);

    _z_order = int8_t(z_order);
}

int sprite_text_generator::width(const string_view& text) const
{
    const span<const int8_t>& character_widths = _font.character_widths_ref();
    const utf8_characters_map_ref& utf8_characters_map = _font.utf8_characters_ref();

    if(int space_between_characters = _font.space_between_characters())
    {
        if(character_widths.empty())
        {
            fixed_width_space_between_characters_painter painter(_max_character_width, space_between_characters);
            [[maybe_unused]] bool success = _paint(text, utf8_characters_map, painter);
            return painter.width();
        }
        else
        {
            variable_width_space_between_characters_painter painter(character_widths.data(), space_between_characters);
            [[maybe_unused]] bool success = _paint(text, utf8_characters_map, painter);
            return painter.width();
        }
    }
    else
    {
        if(character_widths.empty())
        {
            fixed_width_no_space_between_characters_painter painter(_max_character_width);
            [[maybe_unused]] bool success = _paint(text, utf8_characters_map, painter);
            return painter.width();
        }
        else
        {
            variable_width_no_space_between_characters_painter painter(character_widths.data());
            [[maybe_unused]] bool success = _paint(text, utf8_characters_map, painter);
            return painter.width();
        }
    }
}

void sprite_text_generator::generate(const string_view& text, ivector<sprite_ptr>& output_sprites) const
{
    bool one_sprite_per_character = _one_sprite_per_character || _font_one_sprite_per_character;
    _generate<false>(*this, fixed_point(), text, _font.utf8_characters_ref(), _max_character_width,
                     _character_height, one_sprite_per_character, output_sprites);
}

void sprite_text_generator::generate(fixed x, fixed y, const string_view& text,
                                     ivector<sprite_ptr>& output_sprites) const
{
    bool one_sprite_per_character = _one_sprite_per_character || _font_one_sprite_per_character;
    _generate<false>(*this, fixed_point(x, y), text, _font.utf8_characters_ref(), _max_character_width,
                     _character_height, one_sprite_per_character, output_sprites);
}

void sprite_text_generator::generate(const fixed_point& position, const string_view& text,
                                     ivector<sprite_ptr>& output_sprites) const
{
    bool one_sprite_per_character = _one_sprite_per_character || _font_one_sprite_per_character;
    _generate<false>(*this, position, text, _font.utf8_characters_ref(), _max_character_width,
                     _character_height, one_sprite_per_character, output_sprites);
}

bool sprite_text_generator::generate_optional(const string_view& text, ivector<sprite_ptr>& output_sprites) const
{
    bool one_sprite_per_character = _one_sprite_per_character || _font_one_sprite_per_character;
    return _generate<true>(*this, fixed_point(), text, _font.utf8_characters_ref(), _max_character_width,
                           _character_height, one_sprite_per_character, output_sprites);
}

bool sprite_text_generator::generate_optional(fixed x, fixed y, const string_view& text,
                                              ivector<sprite_ptr>& output_sprites) const
{
    bool one_sprite_per_character = _one_sprite_per_character || _font_one_sprite_per_character;
    return _generate<true>(*this, fixed_point(x, y), text, _font.utf8_characters_ref(), _max_character_width,
                           _character_height, one_sprite_per_character, output_sprites);
}

bool sprite_text_generator::generate_optional(const fixed_point& position, const string_view& text,
                                              ivector<sprite_ptr>& output_sprites) const
{
    bool one_sprite_per_character = _one_sprite_per_character || _font_one_sprite_per_character;
    return _generate<true>(*this, position, text, _font.utf8_characters_ref(), _max_character_width,
                           _character_height, one_sprite_per_character, output_sprites);
}

void sprite_text_generator::_init()
{
    const sprite_shape_size& shape_size = _font.item().shape_size();
    int width = shape_size.width();
    int height = shape_size.height();
    _max_character_width = int8_t(width);
    _character_height = int8_t(height);

    if(_font.character_widths_ref().empty())
    {
        if(_font.space_between_characters() || height > 16)
        {
            _font_one_sprite_per_character = true;
        }
        else
        {
            // 8x8      32x8    true
            // 16x8     32x8    true
            // 32x8     32x8    false
            // 8x16     32x16   true
            // 16x16    32x16   true
            // 32x16    32x16   false
            _font_one_sprite_per_character = width > 16;
        }
    }
    else
    {
        bool vwf = (width == 8 && height == 8) || (width == 8 && height == 16) || (width == 16 && height == 16);
        _font_one_sprite_per_character = ! vwf;
    }
}

}
