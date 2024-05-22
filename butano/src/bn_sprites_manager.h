/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITES_MANAGER_H
#define BN_SPRITES_MANAGER_H

#include "bn_fixed_fwd.h"
#include "bn_optional_fwd.h"
#include "bn_fixed_point_fwd.h"
#include "bn_intrusive_list_fwd.h"

namespace bn
{

class size;
class point;
class camera_ptr;
class sprite_builder;
class sprite_tiles_ptr;
class sprite_shape_size;
class sprite_palette_ptr;
class affine_mat_attributes;
class sprite_affine_mat_ptr;
class sprite_first_attributes;
class sprite_third_attributes;
class sprite_regular_second_attributes;
class sprite_affine_second_attributes;
enum class bpp_mode : uint8_t;
enum class sprite_size : uint8_t;
enum class sprite_shape : uint8_t;
enum class sprite_double_size_mode : uint8_t;

namespace sorted_sprites
{
    class layer;
}

namespace sprites_manager
{
    using id_type = void*;

    void init();

    [[nodiscard]] int used_items_count();

    [[nodiscard]] int available_items_count();

    [[nodiscard]] id_type create(const fixed_point& position, const sprite_shape_size& shape_size,
                                 sprite_tiles_ptr&& tiles, sprite_palette_ptr&& palette);

    [[nodiscard]] id_type create_optional(const fixed_point& position, const sprite_shape_size& shape_size,
                                          sprite_tiles_ptr&& tiles, sprite_palette_ptr&& palette);

    [[nodiscard]] id_type create(sprite_builder&& builder);

    [[nodiscard]] id_type create_optional(sprite_builder&& builder);

    void increase_usages(id_type id);

    void decrease_usages(id_type id);

    [[nodiscard]] int hw_id(id_type id);

    [[nodiscard]] sprite_shape shape(id_type id);

    [[nodiscard]] sprite_size size(id_type id);

    [[nodiscard]] sprite_shape_size shape_size(id_type id);

    [[nodiscard]] bn::size dimensions(id_type id);

    [[nodiscard]] const sprite_tiles_ptr& tiles(id_type id);

    void set_tiles(id_type id, const sprite_tiles_ptr& tiles);

    void set_tiles(id_type id, sprite_tiles_ptr&& tiles);

    void set_tiles(id_type id, const sprite_shape_size& shape_size, const sprite_tiles_ptr& tiles);

    void set_tiles(id_type id, const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles);

    void remove_tiles(id_type id);

    [[nodiscard]] const sprite_palette_ptr& palette(id_type id);

    void set_palette(id_type id, bpp_mode old_bpp, const sprite_palette_ptr& palette);

    void set_palette(id_type id, bpp_mode old_bpp, sprite_palette_ptr&& palette);

    void remove_palette(id_type id);

    void set_tiles_and_palette(id_type id, const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles,
                               sprite_palette_ptr&& palette);

    [[nodiscard]] const fixed_point& position(id_type id);

    [[nodiscard]] const point& hw_position(id_type id);

    void set_x(id_type id, fixed x);

    void set_y(id_type id, fixed y);

    void set_position(id_type id, const fixed_point& position);

    [[nodiscard]] int bg_priority(id_type id);

    void set_bg_priority(id_type id, int bg_priority);

    [[nodiscard]] int z_order(id_type id);

    void set_z_order(id_type id, int z_order);

    void put_above(id_type id);

    void put_below(id_type id);

    [[nodiscard]] bool horizontal_flip(id_type id);

    void set_horizontal_flip(id_type id, bool horizontal_flip);

    [[nodiscard]] bool vertical_flip(id_type id);

    void set_vertical_flip(id_type id, bool vertical_flip);

    [[nodiscard]] bool mosaic_enabled(id_type id);

    void set_mosaic_enabled(id_type id, bool mosaic_enabled);

    [[nodiscard]] bool blending_enabled(id_type id);

    void set_blending_enabled(id_type id, bool blending_enabled);

    [[nodiscard]] bool window_enabled(id_type id);

    void set_window_enabled(id_type id, bool window_enabled);

    [[nodiscard]] int view_mode(id_type id);

    [[nodiscard]] bool double_size(id_type id);

    [[nodiscard]] sprite_double_size_mode double_size_mode(id_type id);

    void set_double_size_mode(id_type id, sprite_double_size_mode double_size_mode);

    [[nodiscard]] bool visible(id_type id);

    void set_visible(id_type id, bool visible);

    [[nodiscard]] const optional<camera_ptr>& camera(id_type id);

    void set_camera(id_type id, camera_ptr&& camera);

    void remove_camera(id_type id);

    [[nodiscard]] optional<sprite_affine_mat_ptr>& affine_mat(id_type id);

    void set_affine_mat(id_type id, const sprite_affine_mat_ptr& affine_mat);

    void set_affine_mat(id_type id, sprite_affine_mat_ptr&& affine_mat);

    void set_new_affine_mat(id_type id, affine_mat_attributes& mat_attributes);

    void remove_affine_mat(id_type id);

    [[nodiscard]] bool remove_affine_mat_when_not_needed(id_type id);

    void set_remove_affine_mat_when_not_needed(id_type id, bool remove_when_not_needed);

    [[nodiscard]] sprite_first_attributes first_attributes(id_type id);

    void set_first_attributes(id_type id, const sprite_first_attributes& first_attributes);

    [[nodiscard]] sprite_regular_second_attributes regular_second_attributes(id_type id);

    void set_regular_second_attributes(id_type id, const sprite_regular_second_attributes& second_attributes);

    [[nodiscard]] sprite_affine_second_attributes affine_second_attributes(id_type id);

    void set_affine_second_attributes(id_type id, const sprite_affine_second_attributes& second_attributes);

    [[nodiscard]] sprite_third_attributes third_attributes(id_type id);

    void set_third_attributes(id_type id, const sprite_third_attributes& third_attributes);

    int reserved_handles_count();

    void set_reserved_handles_count(int reserved_handles_count);

    void reload(id_type id);

    void reload_blending();

    void reload_all();

    void fill_hblank_effect_horizontal_positions(id_type id, int hw_x, const fixed* positions_ptr, uint16_t* dest_ptr);

    void fill_hblank_effect_vertical_positions(id_type id, int hw_y, const fixed* positions_ptr, uint16_t* dest_ptr);

    void fill_hblank_effect_first_attributes(int hw_y, sprite_shape shape, bpp_mode bpp, int view_mode,
                                             const sprite_first_attributes* first_attributes_ptr, uint16_t* dest_ptr);

    void fill_hblank_effect_regular_second_attributes(
            id_type id, int hw_x, sprite_size size, const sprite_regular_second_attributes* second_attributes_ptr,
            uint16_t* dest_ptr);

    void fill_hblank_effect_affine_second_attributes(
            id_type id, int hw_x, sprite_size size, const sprite_affine_second_attributes* second_attributes_ptr,
            uint16_t* dest_ptr);

    void fill_hblank_effect_third_attributes(
            sprite_shape_size shape_size, const sprite_third_attributes* third_attributes_ptr, uint16_t* dest_ptr);

    void update_cameras();

    void remove_identity_affine_mat_when_not_needed(id_type id);

    void update_auto_double_size(id_type id);

    void update();

    void commit(bool use_dma);

    BN_CODE_IWRAM void _check_items_on_screen(intrusive_list<sorted_sprites::layer>& layers);

    [[nodiscard]] BN_CODE_IWRAM int _rebuild_handles_impl(
            int reserved_handles_count, void* hw_handles, intrusive_list<sorted_sprites::layer>& layers);

    [[nodiscard]] BN_CODE_IWRAM bool _update_cameras_impl(intrusive_list<sorted_sprites::layer>& layers);
}

}

#endif
