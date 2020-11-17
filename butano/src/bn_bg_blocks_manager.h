/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BG_BLOCKS_MANAGER_H
#define BN_BG_BLOCKS_MANAGER_H

#include "bn_span_fwd.h"
#include "bn_config_log.h"
#include "bn_optional_fwd.h"
#include "bn_regular_bg_map_cell.h"

namespace bn
{
    class size;
    class tile;
    class bg_tiles_ptr;
    class bg_palette_ptr;
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

    #if BN_CFG_LOG_ENABLED
        void log_status();
    #endif

    [[nodiscard]] int find_tiles(const span<const tile>& tiles_ref);

    [[nodiscard]] int find_regular_map(const regular_bg_map_cell& map_cells_ref, const size& map_dimensions,
                                       const bg_tiles_ptr& tiles, const bg_palette_ptr& palette);

    [[nodiscard]] int create_tiles(const span<const tile>& tiles_ref);

    [[nodiscard]] int create_regular_map(const regular_bg_map_cell& map_cells_ref, const size& map_dimensions,
                                         bg_tiles_ptr&& tiles, bg_palette_ptr&& palette);

    [[nodiscard]] int create_new_tiles(const span<const tile>& tiles_ref);

    [[nodiscard]] int create_new_regular_map(const regular_bg_map_cell& map_cells_ref, const size& map_dimensions,
                                             bg_tiles_ptr&& tiles, bg_palette_ptr&& palette);

    [[nodiscard]] int allocate_tiles(int tiles_count);

    [[nodiscard]] int allocate_regular_map(const size& map_dimensions, bg_tiles_ptr&& tiles,
                                           bg_palette_ptr&& palette);

    [[nodiscard]] int create_tiles_optional(const span<const tile>& tiles_ref);

    [[nodiscard]] int create_regular_map_optional(const regular_bg_map_cell& map_cells_ref, const size& map_dimensions,
                                                  bg_tiles_ptr&& tiles, bg_palette_ptr&& palette);

    [[nodiscard]] int create_new_tiles_optional(const span<const tile>& tiles_ref);

    [[nodiscard]] int create_new_regular_map_optional(const regular_bg_map_cell& map_cells_ref,
                                                      const size& map_dimensions, bg_tiles_ptr&& tiles,
                                                      bg_palette_ptr&& palette);

    [[nodiscard]] int allocate_tiles_optional(int tiles_count);

    [[nodiscard]] int allocate_regular_map_optional(const size& map_dimensions, bg_tiles_ptr&& tiles,
                                                    bg_palette_ptr&& palette);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] int hw_id(int id);

    [[nodiscard]] int hw_tiles_cbb(int id);

    [[nodiscard]] int tiles_count(int id);

    [[nodiscard]] size map_dimensions(int id);

    [[nodiscard]] optional<span<const tile>> tiles_ref(int id);

    [[nodiscard]] optional<span<const regular_bg_map_cell>> regular_map_cells_ref(int id);

    void set_tiles_ref(int id, const span<const tile>& tiles_ref);

    void set_regular_map_cells_ref(int id, const regular_bg_map_cell& map_cells_ref, const size& map_dimensions);

    void reload(int id);

    [[nodiscard]] const bg_tiles_ptr& map_tiles(int id);

    void set_map_tiles(int id, bg_tiles_ptr&& tiles);

    void remove_map_tiles(int id);

    [[nodiscard]] const bg_palette_ptr& map_palette(int id);

    void set_map_palette(int id, bg_palette_ptr&& palette);

    void remove_map_palette(int id);

    void set_map_tiles_and_palette(int id, bg_tiles_ptr&& tiles, bg_palette_ptr&& palette);

    [[nodiscard]] optional<span<tile>> tiles_vram(int id);

    [[nodiscard]] optional<span<regular_bg_map_cell>> regular_map_vram(int id);

    void set_regular_map_position(int id, int x, int y);

    void update();

    void commit();
}

#endif
