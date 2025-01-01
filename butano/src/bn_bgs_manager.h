/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BGS_MANAGER_H
#define BN_BGS_MANAGER_H

#include "bn_fixed_fwd.h"
#include "bn_optional_fwd.h"
#include "bn_fixed_point_fwd.h"

namespace bn
{

class size;
class point;
class camera_ptr;
class affine_bg_builder;
class affine_bg_map_ptr;
class regular_bg_builder;
class regular_bg_map_ptr;
class affine_bg_attributes;
class regular_bg_attributes;
class affine_mat_attributes;
class affine_bg_mat_attributes;
enum class bpp_mode : uint8_t;
enum class green_swap_mode : uint8_t;

namespace bgs_manager
{
    using id_type = void*;

    void init();

    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();

    [[nodiscard]] id_type create(regular_bg_builder&& builder);

    [[nodiscard]] id_type create(affine_bg_builder&& builder);

    [[nodiscard]] id_type create_optional(regular_bg_builder&& builder);

    [[nodiscard]] id_type create_optional(affine_bg_builder&& builder);

    void increase_usages(id_type id);

    void decrease_usages(id_type id);

    [[nodiscard]] int hw_id(id_type id);

    [[nodiscard]] size dimensions(id_type id);

    [[nodiscard]] bool big(id_type id);

    [[nodiscard]] const regular_bg_map_ptr& regular_map(id_type id);

    [[nodiscard]] const affine_bg_map_ptr& affine_map(id_type id);

    void set_regular_map(id_type id, const regular_bg_map_ptr& map);

    void set_affine_map(id_type id, const affine_bg_map_ptr& map);

    void set_regular_map(id_type id, regular_bg_map_ptr&& map);

    void set_affine_map(id_type id, affine_bg_map_ptr&& map);

    void remove_regular_map(id_type id);

    void remove_affine_map(id_type id);

    [[nodiscard]] const fixed_point& position(id_type id);

    [[nodiscard]] const point& hw_position(id_type id);

    void set_regular_x(id_type id, fixed x);

    void set_affine_x(id_type id, fixed x);

    void set_regular_y(id_type id, fixed y);

    void set_affine_y(id_type id, fixed y);

    void set_regular_position(id_type id, const fixed_point& position);

    void set_affine_position(id_type id, const fixed_point& position);

    [[nodiscard]] fixed rotation_angle(id_type id);

    void set_rotation_angle(id_type id, fixed rotation_angle);

    [[nodiscard]] fixed horizontal_scale(id_type id);

    void set_horizontal_scale(id_type id, fixed horizontal_scale);

    [[nodiscard]] fixed vertical_scale(id_type id);

    void set_vertical_scale(id_type id, fixed vertical_scale);

    void set_scale(id_type id, fixed scale);

    void set_scale(id_type id, fixed horizontal_scale, fixed vertical_scale);

    [[nodiscard]] fixed horizontal_shear(id_type id);

    void set_horizontal_shear(id_type id, fixed horizontal_shear);

    [[nodiscard]] fixed vertical_shear(id_type id);

    void set_vertical_shear(id_type id, fixed vertical_shear);

    void set_shear(id_type id, fixed shear);

    void set_shear(id_type id, fixed horizontal_shear, fixed vertical_shear);

    [[nodiscard]] bool horizontal_flip(id_type id);

    void set_horizontal_flip(id_type id, bool horizontal_flip);

    [[nodiscard]] bool vertical_flip(id_type id);

    void set_vertical_flip(id_type id, bool vertical_flip);

    [[nodiscard]] const fixed_point& pivot_position(id_type id);

    void set_pivot_x(id_type id, fixed pivot_x);

    void set_pivot_y(id_type id, fixed pivot_y);

    void set_pivot_position(id_type id, const fixed_point& pivot_position);

    [[nodiscard]] const affine_bg_mat_attributes& mat_attributes(id_type id);

    void set_mat_attributes(id_type id, const affine_mat_attributes& mat_attributes);

    [[nodiscard]] int priority(id_type id);

    void set_priority(id_type id, int priority);

    [[nodiscard]] int z_order(id_type id);

    void set_z_order(id_type id, int z_order);

    void put_above(id_type id);

    void put_below(id_type id);

    [[nodiscard]] bool wrapping_enabled(id_type id);

    void set_wrapping_enabled(id_type id, bool wrapping_enabled);

    [[nodiscard]] bool mosaic_enabled(id_type id);

    void set_mosaic_enabled(id_type id, bool mosaic_enabled);

    [[nodiscard]] bn::green_swap_mode green_swap_mode(id_type id);

    void set_green_swap_mode(id_type id, bn::green_swap_mode green_swap_mode);

    [[nodiscard]] regular_bg_attributes regular_attributes(id_type id);

    [[nodiscard]] affine_bg_attributes affine_attributes(id_type id);

    void set_regular_attributes(id_type id, const regular_bg_attributes& attributes);

    void set_affine_attributes(id_type id, const affine_bg_attributes& attributes);

    [[nodiscard]] bool blending_top_enabled(id_type id);

    void set_blending_top_enabled(id_type id, bool blending_top_enabled);

    [[nodiscard]] bool blending_bottom_enabled(id_type id);

    void set_blending_bottom_enabled(id_type id, bool blending_bottom_enabled);

    [[nodiscard]] bool visible(id_type id);

    void set_visible(id_type id, bool visible);

    [[nodiscard]] bool show_in_window(id_type id, int window);

    void set_show_in_window(id_type id, int window, bool show);

    void set_show_all_in_window(int window, bool show);

    void update_windows_flags(unsigned* windows_flags);

    [[nodiscard]] const optional<camera_ptr>& camera(id_type id);

    void set_camera(id_type id, camera_ptr&& camera);

    void remove_camera(id_type id);

    void update_cameras();

    void update_regular_map_tiles_cbb(int map_id, int tiles_cbb);

    void update_affine_map_tiles_cbb(int map_id, int tiles_cbb);

    void update_regular_map_palette_bpp(int map_id, bpp_mode bpp);

    void reload();

    void fill_hblank_effect_regular_positions(int base_position, const fixed* positions_ptr, uint16_t* dest_ptr);

    void fill_hblank_effect_pivot_horizontal_positions(id_type id, const fixed* positions_ptr, unsigned* dest_ptr);

    void fill_hblank_effect_pivot_vertical_positions(id_type id, const fixed* positions_ptr, unsigned* dest_ptr);

    void fill_hblank_effect_regular_attributes(id_type id, const regular_bg_attributes* attributes_ptr,
                                               uint16_t* dest_ptr);

    void fill_hblank_effect_affine_attributes(id_type id, const affine_bg_attributes* attributes_ptr,
                                              uint16_t* dest_ptr);

    void rebuild_handles();

    void update();

    void commit(bool use_dma);

    void commit_big_maps();

    void stop();
}

}

#endif
