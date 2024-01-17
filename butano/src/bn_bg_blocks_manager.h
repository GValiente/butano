/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BG_BLOCKS_MANAGER_H
#define BN_BG_BLOCKS_MANAGER_H

#include "bn_span.h"
#include "bn_optional.h"
#include "bn_config_log.h"
#include "bn_affine_bg_map_cell.h"
#include "bn_regular_bg_map_cell.h"

namespace bn
{
    class size;
    class tile;
    class bg_palette_ptr;
    class affine_bg_map_item;
    class affine_bg_tiles_ptr;
    class affine_bg_tiles_item;
    class regular_bg_map_item;
    class regular_bg_tiles_ptr;
    class regular_bg_tiles_item;
    enum class bpp_mode : uint8_t;
    enum class compression_type : uint8_t;
    enum class affine_bg_big_map_canvas_size : uint8_t;
}

namespace bn::bg_blocks_manager
{
    void init();

    [[nodiscard]] int used_tiles_count();

    [[nodiscard]] int available_tiles_count();

    [[nodiscard]] int used_tile_blocks_count();

    [[nodiscard]] int available_tile_blocks_count();

    [[nodiscard]] int used_map_cells_count();

    [[nodiscard]] int available_map_cells_count();

    [[nodiscard]] int used_map_blocks_count();

    [[nodiscard]] int available_map_blocks_count();

    [[nodiscard]] affine_bg_big_map_canvas_size new_affine_big_map_canvas_size();

    void set_new_affine_big_map_canvas_size(affine_bg_big_map_canvas_size affine_big_map_canvas_size);

    [[nodiscard]] bool allow_tiles_offset();

    void set_allow_tiles_offset(bool allow_tiles_offset);

    #if BN_CFG_LOG_ENABLED
        void log_status();
    #endif

    [[nodiscard]] int find_regular_tiles(const regular_bg_tiles_item& tiles_item);

    [[nodiscard]] int find_affine_tiles(const affine_bg_tiles_item& tiles_item);

    [[nodiscard]] int find_regular_map(
                const regular_bg_map_item& map_item, const regular_bg_map_cell* data_ptr,
                const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette);

    [[nodiscard]] int find_affine_map(
                const affine_bg_map_item& map_item, const affine_bg_map_cell* data_ptr,
                const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette);

    [[nodiscard]] int create_regular_tiles(const regular_bg_tiles_item& tiles_item, bool optional);

    [[nodiscard]] int create_affine_tiles(const affine_bg_tiles_item& tiles_item, bool optional);

    [[nodiscard]] int create_regular_map(
                const regular_bg_map_item& map_item, const regular_bg_map_cell* data_ptr,
                regular_bg_tiles_ptr&& tiles, bg_palette_ptr&& palette, bool optional);

    [[nodiscard]] int create_affine_map(
                const affine_bg_map_item& map_item, const affine_bg_map_cell* data_ptr,
                affine_bg_tiles_ptr&& tiles, bg_palette_ptr&& palette, bool optional);

    [[nodiscard]] int create_new_regular_map(
                const regular_bg_map_item& map_item, const regular_bg_map_cell* data_ptr,
                regular_bg_tiles_ptr&& tiles, bg_palette_ptr&& palette, bool optional);

    [[nodiscard]] int create_new_affine_map(
                const affine_bg_map_item& map_item, const affine_bg_map_cell* data_ptr,
                affine_bg_tiles_ptr&& tiles, bg_palette_ptr&& palette, bool optional);

    [[nodiscard]] int allocate_regular_tiles(int tiles_count, bpp_mode bpp, bool optional);

    [[nodiscard]] int allocate_affine_tiles(int tiles_count, bool optional);

    [[nodiscard]] int allocate_regular_map(const size& map_dimensions, regular_bg_tiles_ptr&& tiles,
                                           bg_palette_ptr&& palette, bool optional);

    [[nodiscard]] int allocate_affine_map(const size& map_dimensions, affine_bg_tiles_ptr&& tiles,
                                          bg_palette_ptr&& palette, bool optional);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] int hw_id(int id);

    [[nodiscard]] int hw_tiles_cbb(int id);

    [[nodiscard]] int tiles_count(int id);

    [[nodiscard]] size map_dimensions(int id);

    [[nodiscard]] bool big_map(int id);

    [[nodiscard]] affine_bg_big_map_canvas_size affine_big_map_canvas_size(int id);

    [[nodiscard]] int regular_tiles_offset(int id);

    [[nodiscard]] int affine_tiles_offset(int id);

    [[nodiscard]] int palette_offset(int id);

    [[nodiscard]] compression_type compression(int id);

    [[nodiscard]] optional<span<const tile>> tiles_ref(int id);

    [[nodiscard]] optional<span<const regular_bg_map_cell>> regular_map_cells_ref(int id);

    [[nodiscard]] optional<span<const affine_bg_map_cell>> affine_map_cells_ref(int id);

    void set_regular_tiles_ref(int id, const regular_bg_tiles_item& tiles_item);

    void set_affine_tiles_ref(int id, const affine_bg_tiles_item& tiles_item);

    void set_regular_map_cells_ref(int id, const regular_bg_map_item& map_item, const regular_bg_map_cell* data_ptr);

    void set_affine_map_cells_ref(int id, const affine_bg_map_item& map_item, const affine_bg_map_cell* data_ptr);

    void reload(int id);

    [[nodiscard]] const regular_bg_tiles_ptr& regular_map_tiles(int id);

    [[nodiscard]] const affine_bg_tiles_ptr& affine_map_tiles(int id);

    void set_regular_map_tiles(int id, regular_bg_tiles_ptr&& tiles);

    void set_affine_map_tiles(int id, affine_bg_tiles_ptr&& tiles);

    void remove_regular_map_tiles(int id);

    void remove_affine_map_tiles(int id);

    [[nodiscard]] const bg_palette_ptr& map_palette(int id);

    void set_regular_map_palette(int id, bg_palette_ptr&& palette);

    void set_affine_map_palette(int id, bg_palette_ptr&& palette);

    void remove_map_palette(int id);

    void set_regular_map_tiles_and_palette(int id, regular_bg_tiles_ptr&& tiles, bg_palette_ptr&& palette);

    [[nodiscard]] optional<span<tile>> tiles_vram(int id);

    [[nodiscard]] optional<span<regular_bg_map_cell>> regular_map_vram(int id);

    [[nodiscard]] optional<span<affine_bg_map_cell>> affine_map_vram(int id);

    [[nodiscard]] bool must_commit(int id);

    void update_regular_map_col(int id, int x, int y);

    inline void update_regular_map_left_col(int id, int x, int y)
    {
        update_regular_map_col(id, x, y);
    }

    inline void update_regular_map_right_col(int id, int x, int y)
    {
        update_regular_map_col(id, x + 31, y);
    }

    void update_affine_map_col(int id, int x, int y);

    inline void update_affine_map_left_col(int id, int x, int y)
    {
        update_affine_map_col(id, x, y);
    }

    void update_affine_map_right_col(int id, int x, int y);

    void update_regular_map_row(int id, int x, int y);

    inline void update_regular_map_top_row(int id, int x, int y)
    {
        update_regular_map_row(id, x, y);
    }

    inline void update_regular_map_bottom_row(int id, int x, int y)
    {
        update_regular_map_row(id, x, y + 31);
    }

    void update_affine_map_row(int id, int x, int y);

    inline void update_affine_map_top_row(int id, int x, int y)
    {
        update_affine_map_row(id, x, y);
    }

    void update_affine_map_bottom_row(int id, int x, int y);

    void set_regular_map_position(int id, int x, int y);

    void set_affine_map_position(int id, int x, int y);

    void update();

    void commit_uncompressed(bool use_dma);

    void commit_compressed();
}

#endif
