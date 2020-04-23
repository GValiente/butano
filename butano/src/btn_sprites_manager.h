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
class sprite_shape_size;
class sprite_palette_ptr;
class sprite_affine_mat_ptr;
enum class sprite_double_size_mode;

namespace sprites_manager
{
    using id_type = void*;

    void init();

    [[nodiscard]] int used_sprites_count();

    [[nodiscard]] int available_sprites_count();

    [[nodiscard]] int used_items_count();

    [[nodiscard]] int available_items_count();

    [[nodiscard]] id_type create(const fixed_point& position, const sprite_shape_size& shape_size,
                                 sprite_tiles_ptr&& tiles, sprite_palette_ptr&& palette);

    [[nodiscard]] id_type optional_create(const fixed_point& position, const sprite_shape_size& shape_size,
                                          sprite_tiles_ptr&& tiles, sprite_palette_ptr&& palette);

    [[nodiscard]] id_type create(sprite_builder&& builder);

    [[nodiscard]] id_type optional_create(sprite_builder&& builder);

    void increase_usages(id_type id);

    void decrease_usages(id_type id);

    [[nodiscard]] optional<int> hw_id(id_type id);

    [[nodiscard]] sprite_shape_size shape_size(id_type id);

    [[nodiscard]] size dimensions(id_type id);

    [[nodiscard]] const sprite_tiles_ptr& tiles(id_type id);

    void set_tiles(id_type id, const sprite_tiles_ptr& tiles_ptr);

    void set_tiles(id_type id, sprite_tiles_ptr&& tiles_ptr);

    void set_tiles(id_type id, const sprite_shape_size& shape_size, const sprite_tiles_ptr& tiles_ptr);

    void set_tiles(id_type id, const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles_ptr);

    [[nodiscard]] const sprite_palette_ptr& palette(id_type id);

    void set_palette(id_type id, const sprite_palette_ptr& palette_ptr);

    void set_palette(id_type id, sprite_palette_ptr&& palette_ptr);

    void set_tiles_and_palette(id_type id, const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles_ptr,
                               sprite_palette_ptr&& palette_ptr);

    [[nodiscard]] const fixed_point& position(id_type id);

    void set_position(id_type id, const fixed_point& position);

    [[nodiscard]] int bg_priority(id_type id);

    void set_bg_priority(id_type id, int bg_priority);

    [[nodiscard]] int z_order(id_type id);

    void set_z_order(id_type id, int z_order);

    void put_in_front_of_sort_layer(id_type id);

    [[nodiscard]] bool horizontal_flip(id_type id);

    void set_horizontal_flip(id_type id, bool horizontal_flip);

    [[nodiscard]] bool vertical_flip(id_type id);

    void set_vertical_flip(id_type id, bool vertical_flip);

    [[nodiscard]] bool mosaic_enabled(id_type id);

    void set_mosaic_enabled(id_type id, bool mosaic_enabled);

    [[nodiscard]] bool blending_enabled(id_type id);

    void set_blending_enabled(id_type id, bool blending_enabled);

    [[nodiscard]] bool window_enabled(id_type id);

    void set_window_enabled(id_type id, bool blending_enabled);

    [[nodiscard]] bool double_size(id_type id);

    [[nodiscard]] sprite_double_size_mode double_size_mode(id_type id);

    void set_double_size_mode(id_type id, sprite_double_size_mode double_size_mode);

    [[nodiscard]] bool visible(id_type id);

    void set_visible(id_type id, bool visible);

    [[nodiscard]] bool ignore_camera(id_type id);

    void set_ignore_camera(id_type id, bool ignore_camera);

    [[nodiscard]] optional<sprite_affine_mat_ptr>& affine_mat(id_type id);

    void set_affine_mat(id_type id, const optional<sprite_affine_mat_ptr>& affine_mat);

    void set_affine_mat(id_type id, optional<sprite_affine_mat_ptr>&& affine_mat);

    [[nodiscard]] bool remove_affine_mat_when_not_needed(id_type id);

    void set_remove_affine_mat_when_not_needed(id_type id, bool remove_when_not_needed);

    void update_camera();

    void update();

    void commit();

    [[nodiscard]] BTN_CODE_IWRAM bool _check_items_on_screen_impl();

    struct update_camera_impl_result
    {
        bool check_items_on_screen = false;
        bool rebuild_handles = false;
    };

    [[nodiscard]] BTN_CODE_IWRAM update_camera_impl_result _update_camera_impl(fixed_point camera_position);
}

}

#endif
