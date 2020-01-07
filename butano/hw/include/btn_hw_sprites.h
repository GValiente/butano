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
    class handle
    {

    public:
        int data[2];
    };

    [[nodiscard]] constexpr int available_sprites()
    {
        return 128;
    }

    [[nodiscard]] constexpr int max_bg_priority()
    {
        return 3;
    }

    void init();

    void setup(const sprite_builder& builder, int tile_id, int palette_id, bool eight_bits_per_pixel,
               int x, int y, handle& sprite);

    [[nodiscard]] size dimensions(const handle& sprite);

    void set_tile(int tile_id, handle& sprite);

    void set_palette(int palette_id, handle& sprite);

    void set_position(int x, int y, handle& sprite);

    void set_bg_priority(int bg_priority, handle& sprite);

    [[nodiscard]] bool horizontal_flip(const handle& sprite);

    void set_horizontal_flip(bool horizontal_flip, handle& sprite);

    [[nodiscard]] bool vertical_flip(const handle& sprite);

    void set_vertical_flip(bool vertical_flip, handle& sprite);

    [[nodiscard]] bool mosaic_enabled(const handle& sprite);

    void set_mosaic_enabled(bool mosaic_enabled, handle& sprite);

    void hide(handle& sprite);

    void commit(const handle& sprites_ref, int offset, int count);
}
}

#endif
