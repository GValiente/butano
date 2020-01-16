#include "btn_sprite_text_generator.h"

#include "btn_sprite_ptr.h"
#include "btn_string_view.h"
#include "btn_utf8_character.h"
#include "btn_sprite_builder.h"
#include "btn_sprites_manager.h"
#include "../hw/include/btn_hw_sprite_tiles.h"

namespace btn
{

namespace
{
    static_assert(BTN_CFG_SPRITE_TEXT_MAX_UTF8_CHARACTERS > 0);
    static_assert(power_of_two(BTN_CFG_SPRITE_TEXT_MAX_UTF8_CHARACTERS));

    constexpr const int max_columns_per_sprite = 32;
    constexpr const int fixed_character_width = 8;
    constexpr const int fixed_max_characters_per_sprite = max_columns_per_sprite / fixed_character_width;

    template<sprite_size size, int max_tiles_per_sprite>
    tile* build_sprite(const sprite_text_generator& generator, const fixed_point& current_position,
                       ivector<sprite_ptr>& output_sprites)
    {
        BTN_ASSERT(! output_sprites.full(), "Output sprites vector is full");

        sprite_tiles_ptr tiles_ptr = sprite_tiles_ptr::allocate(max_tiles_per_sprite);
        optional<span<tile>> tiles_vram = tiles_ptr.vram();
        BTN_ASSERT(tiles_vram, "Tiles VRAM retrieve failed");

        sprite_builder builder(sprite_shape::WIDE, size, move(tiles_ptr), generator.palette());
        builder.set_position(current_position);
        builder.set_bg_priority(generator.bg_priority());
        builder.set_z_order(generator.z_order());
        builder.set_ignore_camera(generator.ignore_camera());
        output_sprites.push_back(builder.build_and_release());
        return tiles_vram->data();
    }


    class fixed_width_painter
    {

    public:
        explicit fixed_width_painter(const sprite_text_generator& generator) :
            _generator(generator)
        {
        }

        [[nodiscard]] const sprite_text_generator& generator() const
        {
            return _generator;
        }

        [[nodiscard]] fixed width() const
        {
            return _width;
        }

        void paint_space()
        {
            _width += fixed_character_width;
        }

        void paint_tab()
        {
            _width += (fixed_character_width * 4);
        }

        void paint_character([[maybe_unused]] int graphics_index)
        {
            _width += fixed_character_width;
        }

    private:
        const sprite_text_generator& _generator;
        fixed _width = 0;
    };


    class variable_width_painter
    {

    public:
        explicit variable_width_painter(const sprite_text_generator& generator) :
            _generator(generator)
        {
        }

        [[nodiscard]] const sprite_text_generator& generator() const
        {
            return _generator;
        }

        [[nodiscard]] fixed width() const
        {
            return _width;
        }

        void paint_space()
        {
            _width += _generator.font().character_widths()[0];
        }

        void paint_tab()
        {
            _width += (_generator.font().character_widths()[0] * 4);
        }

        void paint_character(int graphics_index)
        {
            _width += _generator.font().character_widths()[size_t(graphics_index + 1)];
        }

    private:
        const sprite_text_generator& _generator;
        fixed _width = 0;
    };


    class fixed_one_sprite_per_character_painter
    {

    public:
        fixed_one_sprite_per_character_painter(const sprite_text_generator& generator, const fixed_point& position,
                                               ivector<sprite_ptr>& output_sprites) :
            _generator(generator),
            _output_sprites(output_sprites),
            _current_position(position.x() + (fixed_character_width / 2), position.y())
        {
        }

        [[nodiscard]] const sprite_text_generator& generator() const
        {
            return _generator;
        }

        void paint_space()
        {
            _current_position.set_x(_current_position.x() + fixed_character_width);
        }

        void paint_tab()
        {
            _current_position.set_x(_current_position.x() + (fixed_character_width * 4));
        }

        void paint_character(int graphics_index)
        {
            BTN_ASSERT(! _output_sprites.full(), "Output sprites vector is full");

            const sprite_item& item = _generator.font().item();
            span<const tile> source_tiles_ref = item.tiles_item().tiles_ref(graphics_index);
            sprite_tiles_ptr source_tiles_ptr = sprite_tiles_ptr::find_or_create(source_tiles_ref);
            sprite_builder builder(item.shape(), sprite_size::SMALL, move(source_tiles_ptr), _generator.palette());
            builder.set_position(_current_position);
            builder.set_bg_priority(_generator.bg_priority());
            builder.set_z_order(_generator.z_order());
            builder.set_ignore_camera(_generator.ignore_camera());
            _output_sprites.push_back(builder.build_and_release());
            _current_position.set_x(_current_position.x() + fixed_character_width);
        }

    private:
        const sprite_text_generator& _generator;
        ivector<sprite_ptr>& _output_sprites;
        fixed_point _current_position;
    };


    class variable_one_sprite_per_character_painter
    {

    public:
        variable_one_sprite_per_character_painter(const sprite_text_generator& generator, const fixed_point& position,
                                                  ivector<sprite_ptr>& output_sprites) :
            _generator(generator),
            _output_sprites(output_sprites),
            _current_position(position.x() + (fixed_character_width / 2), position.y())
        {
        }

        [[nodiscard]] const sprite_text_generator& generator() const
        {
            return _generator;
        }

        void paint_space()
        {
            _current_position.set_x(_current_position.x() + _generator.font().character_widths()[0]);
        }

        void paint_tab()
        {
            _current_position.set_x(_current_position.x() + (_generator.font().character_widths()[0] * 4));
        }

        void paint_character(int graphics_index)
        {
            BTN_ASSERT(! _output_sprites.full(), "Output sprites vector is full");

            const sprite_font& font = _generator.font();
            const sprite_item& item = font.item();
            span<const tile> source_tiles_ref = item.tiles_item().tiles_ref(graphics_index);
            sprite_tiles_ptr source_tiles_ptr = sprite_tiles_ptr::find_or_create(source_tiles_ref);
            sprite_builder builder(item.shape(), sprite_size::SMALL, move(source_tiles_ptr), _generator.palette());
            builder.set_position(_current_position);
            builder.set_bg_priority(_generator.bg_priority());
            builder.set_z_order(_generator.z_order());
            builder.set_ignore_camera(_generator.ignore_camera());
            _output_sprites.push_back(builder.build_and_release());
            _current_position.set_x(_current_position.x() + font.character_widths()[size_t(graphics_index + 1)]);
        }

    private:
        const sprite_text_generator& _generator;
        ivector<sprite_ptr>& _output_sprites;
        fixed_point _current_position;
    };


    class fixed_8x8_painter
    {

    public:
        fixed_8x8_painter(const sprite_text_generator& generator, const fixed_point& position,
                          ivector<sprite_ptr>& output_sprites) :
            _generator(generator),
            _output_sprites(output_sprites),
            _current_position(position.x() + (max_columns_per_sprite / 2), position.y())
        {
        }

        ~fixed_8x8_painter()
        {
            _clear_left();
        }

        [[nodiscard]] const sprite_text_generator& generator() const
        {
            return _generator;
        }

        void paint_space()
        {
            if(_sprite_character_index < fixed_max_characters_per_sprite)
            {
                _clear(1);
                ++_sprite_character_index;
            }

            _current_position.set_x(_current_position.x() + fixed_character_width);
        }

        void paint_tab()
        {
            _clear_left();
            _current_position.set_x(_current_position.x() + (fixed_character_width * 4));
        }

        void paint_character(int graphics_index)
        {
            if(_sprite_character_index == fixed_max_characters_per_sprite)
            {
                _tiles_vram = build_sprite<sprite_size::NORMAL, fixed_max_characters_per_sprite>(
                            _generator, _current_position, _output_sprites);
                _sprite_character_index = 0;
            }

            const sprite_item& item = _generator.font().item();
            span<const tile> source_tiles_ref = item.tiles_item().tiles_ref(graphics_index);
            hw::sprite_tiles::copy_tiles(source_tiles_ref[0], 1, _tiles_vram[_sprite_character_index]);
            _current_position.set_x(_current_position.x() + fixed_character_width);
            ++_sprite_character_index;
        }

    private:
        const sprite_text_generator& _generator;
        ivector<sprite_ptr>& _output_sprites;
        fixed_point _current_position;
        tile* _tiles_vram = nullptr;
        int _sprite_character_index = fixed_max_characters_per_sprite;

        void _clear(int characters)
        {
            hw::sprite_tiles::clear_tiles(characters, _tiles_vram[_sprite_character_index]);
        }

        void _clear_left()
        {
            if(_sprite_character_index < fixed_max_characters_per_sprite)
            {
                _clear(fixed_max_characters_per_sprite - _sprite_character_index);
                _sprite_character_index = fixed_max_characters_per_sprite;
            }
        }
    };


    class variable_8x8_painter
    {

    public:
        variable_8x8_painter(const sprite_text_generator& generator, const fixed_point& position,
                              ivector<sprite_ptr>& output_sprites) :
            _generator(generator),
            _output_sprites(output_sprites),
            _current_position(position.x() + (max_columns_per_sprite / 2), position.y())
        {
        }

        [[nodiscard]] const sprite_text_generator& generator() const
        {
            return _generator;
        }

        void paint_space()
        {
            int width = _generator.font().character_widths()[0];
            _sprite_column += width;
            _current_position.set_x(_current_position.x() + width);
        }

        void paint_tab()
        {
            int width = _generator.font().character_widths()[0] * 4;
            _sprite_column += width;
            _current_position.set_x(_current_position.x() + width);
        }

        void paint_character(int graphics_index)
        {
            const sprite_font& font = _generator.font();

            if(int width = font.character_widths()[size_t(graphics_index + 1)])
            {
                if(_sprite_column + width > max_columns_per_sprite)
                {
                    _tiles_vram = build_sprite<sprite_size::NORMAL, _tiles>(
                                _generator, _current_position, _output_sprites);
                    hw::sprite_tiles::clear_tiles(_tiles, *_tiles_vram);
                    _sprite_column = 0;
                }

                const sprite_tiles_item& tiles_item = font.item().tiles_item();
                const tile& source_tiles_ref = tiles_item.tiles().front();
                int source_height = tiles_item.graphics() * _character_height;
                int source_y = graphics_index * _character_height;
                hw::sprite_tiles::plot_tiles(width, source_tiles_ref, source_height, source_y, _sprite_column,
                                             *_tiles_vram);
                _current_position.set_x(_current_position.x() + width);
                _sprite_column += width;
            }
        }

    private:
        static constexpr const int _character_height = 8;
        static constexpr const int _tiles = 4;

        const sprite_text_generator& _generator;
        ivector<sprite_ptr>& _output_sprites;
        fixed_point _current_position;
        tile* _tiles_vram = nullptr;
        int _sprite_column = max_columns_per_sprite;
    };


    class fixed_8x16_painter
    {

    public:
        fixed_8x16_painter(const sprite_text_generator& generator, const fixed_point& position,
                           ivector<sprite_ptr>& output_sprites) :
            _generator(generator),
            _output_sprites(output_sprites),
            _current_position(position.x() + (max_columns_per_sprite / 2), position.y())
        {
        }

        ~fixed_8x16_painter()
        {
            _clear_left();
        }

        [[nodiscard]] const sprite_text_generator& generator() const
        {
            return _generator;
        }

        void paint_space()
        {
            if(_sprite_character_index < fixed_max_characters_per_sprite)
            {
                _clear(1);
                ++_sprite_character_index;
            }

            _current_position.set_x(_current_position.x() + fixed_character_width);
        }

        void paint_tab()
        {
            _clear_left();
            _current_position.set_x(_current_position.x() + (fixed_character_width * 4));
        }

        void paint_character(int graphics_index)
        {
            if(_sprite_character_index == fixed_max_characters_per_sprite)
            {
                _tiles_vram = build_sprite<sprite_size::BIG, fixed_max_characters_per_sprite * 2>(
                            _generator, _current_position, _output_sprites);
                _sprite_character_index = 0;
            }

            const sprite_item& item = _generator.font().item();
            span<const tile> source_tiles_ref = item.tiles_item().tiles_ref(graphics_index);
            auto source_tiles_data = source_tiles_ref.data();
            tile* up_tiles_vram_ptr = _tiles_vram + _sprite_character_index;
            hw::sprite_tiles::copy_tiles(source_tiles_data[0], 1, *up_tiles_vram_ptr);

            tile* down_tiles_vram_ptr = up_tiles_vram_ptr + fixed_max_characters_per_sprite;
            hw::sprite_tiles::copy_tiles(source_tiles_data[1], 1, *down_tiles_vram_ptr);

            _current_position.set_x(_current_position.x() + fixed_character_width);
            ++_sprite_character_index;
        }

    private:
        const sprite_text_generator& _generator;
        ivector<sprite_ptr>& _output_sprites;
        fixed_point _current_position;
        tile* _tiles_vram = nullptr;
        int _sprite_character_index = fixed_max_characters_per_sprite;

        void _clear(int characters)
        {
            tile* up_tiles_vram_ptr = _tiles_vram + _sprite_character_index;
            hw::sprite_tiles::clear_tiles(characters, *up_tiles_vram_ptr);

            tile* down_tiles_vram_ptr = up_tiles_vram_ptr + fixed_max_characters_per_sprite;
            hw::sprite_tiles::clear_tiles(characters, *down_tiles_vram_ptr);
        }

        void _clear_left()
        {
            if(_sprite_character_index < fixed_max_characters_per_sprite)
            {
                _clear(fixed_max_characters_per_sprite - _sprite_character_index);
                _sprite_character_index = fixed_max_characters_per_sprite;
            }
        }
    };


    class variable_8x16_painter
    {

    public:
        variable_8x16_painter(const sprite_text_generator& generator, const fixed_point& position,
                              ivector<sprite_ptr>& output_sprites) :
            _generator(generator),
            _output_sprites(output_sprites),
            _current_position(position.x() + (max_columns_per_sprite / 2), position.y())
        {
        }

        [[nodiscard]] const sprite_text_generator& generator() const
        {
            return _generator;
        }

        void paint_space()
        {
            int width = _generator.font().character_widths()[0];
            _sprite_column += width;
            _current_position.set_x(_current_position.x() + width);
        }

        void paint_tab()
        {
            int width = _generator.font().character_widths()[0] * 4;
            _sprite_column += width;
            _current_position.set_x(_current_position.x() + width);
        }

        void paint_character(int graphics_index)
        {
            const sprite_font& font = _generator.font();

            if(int width = font.character_widths()[size_t(graphics_index + 1)])
            {
                if(_sprite_column + width > max_columns_per_sprite)
                {
                    _tiles_vram = build_sprite<sprite_size::BIG, _tiles>(
                                _generator, _current_position, _output_sprites);
                    hw::sprite_tiles::clear_tiles(_tiles, *_tiles_vram);
                    _sprite_column = 0;
                }

                const sprite_tiles_item& tiles_item = font.item().tiles_item();
                const tile& source_tiles_ref = tiles_item.tiles().front();
                int source_height = tiles_item.graphics() * _character_height;
                int source_y = graphics_index * _character_height;
                hw::sprite_tiles::plot_tiles(width, source_tiles_ref, source_height,
                                             source_y,
                                             _sprite_column, *_tiles_vram);
                hw::sprite_tiles::plot_tiles(width, source_tiles_ref, source_height,
                                             source_y + (_character_height / 2),
                                             _sprite_column + (_character_height * 2), *_tiles_vram);
                _current_position.set_x(_current_position.x() + width);
                _sprite_column += width;
            }
        }

    private:
        static constexpr const int _character_height = 16;
        static constexpr const int _tiles = 8;

        const sprite_text_generator& _generator;
        ivector<sprite_ptr>& _output_sprites;
        fixed_point _current_position;
        tile* _tiles_vram = nullptr;
        int _sprite_column = max_columns_per_sprite;
    };


    template<class Painter>
    void paint(const string_view& text, Painter& painter)
    {
        const sprite_text_generator& generator = painter.generator();
        const auto& utf8_characters_map = generator.utf8_characters_map();
        const char* text_data = text.data();
        size_t text_index = 0;
        size_t text_size = text.size();

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
                int graphics_index;

                if(character <= '~')
                {
                    graphics_index = character - '!';
                    ++text_index;
                }
                else
                {
                    utf8_character utf8_char(text_data + text_index);
                    auto it = utf8_characters_map.find(utf8_char.value());
                    BTN_ASSERT(it != utf8_characters_map.end(), "Utf8 character not found: ", text);

                    graphics_index = it->second;
                    text_index += size_t(utf8_char.size());
                }

                painter.paint_character(graphics_index);
            }
            else
            {
                BTN_ERROR("Invalid character: ", character, " (text: ", text, ")");
            }
        }
    }
}

sprite_text_generator::sprite_text_generator(const sprite_font& font) :
    _font(font),
    _palette_ptr(font.item().palette_item().palette_ptr(create_mode::FIND_OR_CREATE))
{
    int utf8_character_index = sprite_font::minimum_graphics;

    for(const string_view& utf8_character_text : font.utf8_characters())
    {
        utf8_character utf8_char(utf8_character_text.data());
        _utf8_characters_map.insert(utf8_char.value(), utf8_character_index);
        ++utf8_character_index;
    }
}

void sprite_text_generator::set_palette(sprite_palette_ptr palette_ptr)
{
    BTN_ASSERT(palette_ptr.colors_count() == _palette_ptr.colors_count(), "Invalid colors count: ",
               palette_ptr.colors_count(), " - ", _palette_ptr.colors_count());

    _palette_ptr = move(palette_ptr);
}

void sprite_text_generator::set_bg_priority(int bg_priority)
{
    BTN_ASSERT(bg_priority >= 0 && bg_priority <= sprites_manager::max_bg_priority(), "Invalid bg priority: ",
               bg_priority);

    _bg_priority = bg_priority;
}

void sprite_text_generator::set_z_order(int z_order)
{
    BTN_ASSERT(z_order >= 0 && z_order < int(sprites_manager::z_orders()), "Invalid z order: ", z_order);

    _z_order = z_order;
}

fixed sprite_text_generator::width(const string_view& text) const
{
    if(_font.character_widths().empty())
    {
        fixed_width_painter painter(*this);
        paint(text, painter);
        return painter.width();
    }

    variable_width_painter painter(*this);
    paint(text, painter);
    return painter.width();
}

void sprite_text_generator::generate(fixed x, fixed y, const string_view& text,
                                     ivector<sprite_ptr>& output_sprites) const
{
    generate(fixed_point(x, y), text, output_sprites);
}

void sprite_text_generator::generate(const fixed_point& position, const string_view& text,
                                     ivector<sprite_ptr>& output_sprites) const
{
    const sprite_item& sprite_item = _font.item();
    fixed_point aligned_position = position;

    switch(_alignment)
    {

    case horizontal_alignment_type::LEFT:
        break;

    case horizontal_alignment_type::CENTER:
        aligned_position.set_x(aligned_position.x() - (width(text) / 2));
        break;

    case horizontal_alignment_type::RIGHT:
        aligned_position.set_x(aligned_position.x() - width(text));
        break;
    }

    if(_one_sprite_per_character)
    {
        if(_font.character_widths().empty())
        {
            fixed_one_sprite_per_character_painter painter(*this, aligned_position, output_sprites);
            paint(text, painter);
        }
        else
        {
            variable_one_sprite_per_character_painter painter(*this, aligned_position, output_sprites);
            paint(text, painter);
        }
    }
    else
    {
        if(sprite_item.shape_size().height() == 8)
        {
            if(_font.character_widths().empty())
            {
                fixed_8x8_painter painter(*this, aligned_position, output_sprites);
                paint(text, painter);
            }
            else
            {
                variable_8x8_painter painter(*this, aligned_position, output_sprites);
                paint(text, painter);
            }
        }
        else
        {
            if(_font.character_widths().empty())
            {
                fixed_8x16_painter painter(*this, aligned_position, output_sprites);
                paint(text, painter);
            }
            else
            {
                variable_8x16_painter painter(*this, aligned_position, output_sprites);
                paint(text, painter);
            }
        }
    }
}

}
