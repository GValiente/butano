#ifndef BTN_BG_BLOCKS_MANAGER_H
#define BTN_BG_BLOCKS_MANAGER_H

#include "btn_span_fwd.h"
#include "btn_optional_fwd.h"

namespace btn
{
    class bg_block;
}

namespace btn::bg_blocks_manager
{
    void init();

    [[nodiscard]] optional<int> find(const span<const bg_block>& blocks_ref);

    [[nodiscard]] optional<int> create(const span<const bg_block>& blocks_ref, bool aligned);

    [[nodiscard]] optional<int> allocate(int blocks, bool aligned);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] int start_block(int id);

    [[nodiscard]] int blocks_count(int id);

    [[nodiscard]] optional<span<const bg_block>> blocks_ref(int id);

    void set_blocks_ref(int id, const span<const bg_block>& blocks_ref);

    void reload_blocks_ref(int id);

    [[nodiscard]] optional<span<bg_block>> vram(int id);

    void update();

    void commit();
}

#endif
