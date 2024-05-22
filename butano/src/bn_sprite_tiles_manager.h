/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_TILES_MANAGER_H
#define BN_SPRITE_TILES_MANAGER_H

#include "bn_span.h"
#include "bn_optional.h"
#include "bn_config_log.h"

namespace bn
{
    class tile;
    enum class bpp_mode : uint8_t;
    enum class compression_type : uint8_t;
}

namespace bn::sprite_tiles_manager
{
    void init();

    [[nodiscard]] int used_tiles_count();

    [[nodiscard]] int available_tiles_count();

    [[nodiscard]] int used_items_count();

    [[nodiscard]] int available_items_count();

    #if BN_CFG_LOG_ENABLED
        void log_status();
    #endif

    [[nodiscard]] int find(const span<const tile>& tiles_ref, compression_type compression);

    [[nodiscard]] int create(const span<const tile>& tiles_ref, compression_type compression);

    [[nodiscard]] int allocate(int tiles_count, bpp_mode bpp);

    [[nodiscard]] int create_optional(const span<const tile>& tiles_ref, compression_type compression);

    [[nodiscard]] int allocate_optional(int tiles_count, bpp_mode bpp);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] int start_tile(int id);

    [[nodiscard]] int tiles_count(int id);

    [[nodiscard]] compression_type compression(int id);

    [[nodiscard]] optional<span<const tile>> tiles_ref(int id);

    void set_tiles_ref(int id, const span<const tile>& tiles_ref, compression_type compression);

    void reload_tiles_ref(int id);

    [[nodiscard]] optional<span<tile>> vram(int id);

    void update();

    void commit_uncompressed(bool use_dma);

    void commit_compressed();
}

#endif
