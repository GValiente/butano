#ifndef BTN_SPRITES_MANAGER_H
#define BTN_SPRITES_MANAGER_H

#include "btn_limits.h"

namespace btn
{

class size;
class fixed_point;
class sprite_builder;
class sprite_tiles_ptr;
class sprite_palette_ptr;

namespace sprites_manager
{
    using id_type = void*;

    [[nodiscard]] constexpr unsigned z_orders()
    {
        return btn::integral_limits<uint16_t>::max;
    }

    [[nodiscard]] BTN_CODE_CONST int max_bg_priority();

    void init();

    [[nodiscard]] id_type create(const sprite_builder& builder);

    void increase_usages(id_type id);

    void decrease_usages(id_type id);

    [[nodiscard]] size dimensions(id_type id);

    [[nodiscard]] const sprite_tiles_ptr& tiles_ptr(id_type id);

    void set_tiles_ptr(id_type id, sprite_tiles_ptr tiles_ptr);

    [[nodiscard]] const sprite_palette_ptr& palette_ptr(id_type id);

    void set_palette_ptr(id_type id, sprite_palette_ptr palette_ptr);

    [[nodiscard]] const fixed_point& position(id_type id);

    void set_position(id_type id, const fixed_point& position);

    [[nodiscard]] int bg_priority(id_type id);

    void set_bg_priority(id_type id, int bg_priority);

    [[nodiscard]] int z_order(id_type id);

    void set_z_order(id_type id, int z_order);

    [[nodiscard]] bool visible(id_type id);

    void set_visible(id_type id, bool visible);

    void update();

    void commit();
}

}

#endif
