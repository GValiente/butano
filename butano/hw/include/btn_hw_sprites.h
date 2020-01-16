#ifndef BTN_HW_SPRITES_H
#define BTN_HW_SPRITES_H

#include "btn_common.h"

namespace btn
{

class size;
class sprite_builder;
enum class sprite_shape;
enum class sprite_size;

namespace hw::sprites
{
    class alignas(sizeof(int)) handle
    {

    public:
        uint16_t attr0;
        uint16_t attr1;
        uint16_t attr2;
        int16_t fill;

        void copy_to(handle& other)
        {
            other.attr0 = attr0;
            other.attr1 = attr1;
            other.attr2 = attr2;
        }
    };

    [[nodiscard]] constexpr int count()
    {
        return 128;
    }

    [[nodiscard]] constexpr int max_bg_priority()
    {
        return 3;
    }

    void init();

    void setup_regular(const sprite_builder& builder, int tile_id, int palette_id, bool eight_bits_per_pixel,
                       handle& sprite);

    void setup_affine(const sprite_builder& builder, int tile_id, int palette_id, int affine_mat_id,
                      bool eight_bits_per_pixel, handle& sprite);

    [[nodiscard]] size dimensions(const handle& sprite);

    void set_tile(int tile_id, handle& sprite);

    void set_palette(int palette_id, handle& sprite);

    void set_affine_mat(int affine_mat_id, bool double_size, handle& sprite);

    void remove_affine_mat(handle& sprite);

    void set_position(int x, int y, handle& sprite);

    void set_bg_priority(int bg_priority, handle& sprite);

    [[nodiscard]] bool horizontal_flip(const handle& sprite);

    void set_horizontal_flip(bool horizontal_flip, handle& sprite);

    [[nodiscard]] bool vertical_flip(const handle& sprite);

    void set_vertical_flip(bool vertical_flip, handle& sprite);

    [[nodiscard]] bool mosaic_enabled(const handle& sprite);

    void set_mosaic_enabled(bool mosaic_enabled, handle& sprite);

    void hide(int count, handle& sprites_ref);

    void commit(const handle& sprites_ref, int offset, int count);
}
}

#endif
