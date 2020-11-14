/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_TILES_MANAGER_H
#define BTN_SPRITE_TILES_MANAGER_H

#include "btn_span_fwd.h"
#include "btn_config_log.h"
#include "btn_optional_fwd.h"

namespace btn
{
    class tile;
}

namespace btn::sprite_tiles_manager
{
    void init();

    [[nodiscard]] int used_tiles_count();

    [[nodiscard]] int available_tiles_count();

    [[nodiscard]] int used_items_count();

    [[nodiscard]] int available_items_count();

    #if BTN_CFG_LOG_ENABLED
        void log_status();
    #endif

    [[nodiscard]] int find(const span<const tile>& tiles_ref);

    [[nodiscard]] int create(const span<const tile>& tiles_ref);

    [[nodiscard]] int create_new(const span<const tile>& tiles_ref);

    [[nodiscard]] int allocate(int tiles_count);

    [[nodiscard]] int create_optional(const span<const tile>& tiles_ref);

    [[nodiscard]] int create_new_optional(const span<const tile>& tiles_ref);

    [[nodiscard]] int allocate_optional(int tiles_count);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] int start_tile(int id);

    [[nodiscard]] int tiles_count(int id);

    [[nodiscard]] optional<span<const tile>> tiles_ref(int id);

    void set_tiles_ref(int id, const span<const tile>& tiles_ref);

    void reload_tiles_ref(int id);

    [[nodiscard]] optional<span<tile>> vram(int id);

    void update();

    void commit();
}

#endif
