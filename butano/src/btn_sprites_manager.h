#ifndef BTN_SPRITES_MANAGER_H
#define BTN_SPRITES_MANAGER_H

#include "btn_limits.h"
#include "btn_optional_fwd.h"

namespace btn
{

class size;
class fixed_point;
class sprite_builder;
class sprite_tiles_ptr;
class sprite_palette_ptr;
class sprite_affine_mat_ptr;
enum class sprite_double_size_mode;

namespace sprites_manager
{
    using id_type = void*;

    [[nodiscard]] BTN_CODE_CONST int max_bg_priority();

    [[nodiscard]] BTN_CODE_CONST int z_orders();

    void init();

    [[nodiscard]] optional<id_type> create(sprite_builder&& builder);

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

    [[nodiscard]] bool horizontal_flip(id_type id);

    void set_horizontal_flip(id_type id, bool horizontal_flip);

    [[nodiscard]] bool vertical_flip(id_type id);

    void set_vertical_flip(id_type id, bool vertical_flip);

    [[nodiscard]] bool mosaic_enabled(id_type id);

    void set_mosaic_enabled(id_type id, bool mosaic_enabled);

    [[nodiscard]] bool double_size(id_type id);

    [[nodiscard]] sprite_double_size_mode double_size_mode(id_type id);

    void set_double_size_mode(id_type id, sprite_double_size_mode double_size_mode);

    [[nodiscard]] bool visible(id_type id);

    void set_visible(id_type id, bool visible);

    [[nodiscard]] bool ignore_camera(id_type id);

    void set_ignore_camera(id_type id, bool ignore_camera);

    [[nodiscard]] optional<sprite_affine_mat_ptr>& affine_mat_ptr(id_type id);

    void set_affine_mat_ptr(id_type id, optional<sprite_affine_mat_ptr> affine_mat_ptr);

    [[nodiscard]] bool remove_affine_mat_when_not_needed(id_type id);

    void set_remove_affine_mat_when_not_needed(id_type id, bool remove_when_not_needed);

    void update_camera();

    void update();

    void commit();
}

}

#endif
