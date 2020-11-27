/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BGS_MANAGER_H
#define BN_BGS_MANAGER_H

#include "bn_limits.h"
#include "bn_fixed_fwd.h"
#include "bn_optional_fwd.h"

namespace bn
{

class size;
class point;
class camera_ptr;
class fixed_point;
class affine_bg_builder;
class affine_bg_map_ptr;
class regular_bg_builder;
class regular_bg_map_ptr;
class affine_bg_attributes;
class regular_bg_attributes;
class affine_mat_attributes;
class affine_bg_mat_attributes;
enum class bpp_mode;

namespace bgs_manager
{
    using id_type = void*;

    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();

    [[nodiscard]] id_type create(regular_bg_builder&& builder);

    [[nodiscard]] id_type create(affine_bg_builder&& builder);

    [[nodiscard]] id_type create_optional(regular_bg_builder&& builder);

    [[nodiscard]] id_type create_optional(affine_bg_builder&& builder);

    void increase_usages(id_type id);

    void decrease_usages(id_type id);

    [[nodiscard]] optional<int> hw_id(id_type id);

    [[nodiscard]] size dimensions(id_type id);

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

    [[nodiscard]] bool above(id_type id, id_type other_id);

    void put_above(id_type id);

    [[nodiscard]] bool wrapping_enabled(id_type id);

    void set_wrapping_enabled(id_type id, bool wrapping_enabled);

    [[nodiscard]] bool mosaic_enabled(id_type id);

    void set_mosaic_enabled(id_type id, bool mosaic_enabled);

    [[nodiscard]] regular_bg_attributes regular_attributes(id_type id);

    [[nodiscard]] affine_bg_attributes affine_attributes(id_type id);

    void set_regular_attributes(id_type id, const regular_bg_attributes& attributes);

    void set_affine_attributes(id_type id, const affine_bg_attributes& attributes);

    [[nodiscard]] bool blending_enabled(id_type id);

    void set_blending_enabled(id_type id, bool blending_enabled);

    [[nodiscard]] bool visible(id_type id);

    void set_visible(id_type id, bool visible);

    [[nodiscard]] const optional<camera_ptr>& camera(id_type id);

    void set_camera(id_type id, camera_ptr&& camera);

    void remove_camera(id_type id);

    void update_cameras();

    void update_regular_map_tiles_cbb(int map_id, int tiles_cbb);

    void update_affine_map_tiles_cbb(int map_id, int tiles_cbb);

    void update_regular_map_palette_bpp(int map_id, bpp_mode bpp);

    void reload();

    void fill_hblank_effect_regular_positions(int base_position, const fixed* positions_ptr, uint16_t* dest_ptr);

    void fill_hblank_effect_pivot_positions(int base_position, const fixed* positions_ptr, uint16_t* dest_ptr);

    void fill_hblank_effect_regular_attributes(id_type id, const regular_bg_attributes* attributes_ptr,
                                               uint16_t* dest_ptr);

    void fill_hblank_effect_affine_attributes(id_type id, const affine_bg_attributes* attributes_ptr,
                                              uint16_t* dest_ptr);

    void update();

    void commit();

    void commit_big_maps();
}

}

#endif
