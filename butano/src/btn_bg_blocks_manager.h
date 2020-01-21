#ifndef BTN_BG_BLOCKS_MANAGER_H
#define BTN_BG_BLOCKS_MANAGER_H

#include "btn_span_fwd.h"
#include "btn_optional_fwd.h"

namespace btn
{
    class size;
}

namespace btn::bg_blocks_manager
{
    void init();

    [[nodiscard]] optional<int> find(const uint16_t& data_ref, const size& dimensions);

    [[nodiscard]] optional<int> create(const uint16_t& data_ref, const size& dimensions, bool aligned);

    [[nodiscard]] optional<int> allocate(const size& dimensions, bool aligned);

    void increase_usages(int id);

    void decrease_usages(int id);

    [[nodiscard]] int hw_id(int id, bool aligned);

    [[nodiscard]] size dimensions(int id);

    [[nodiscard]] const uint16_t* data_ref(int id);

    void set_data_ref(int id, const uint16_t& data_ref, const size& dimensions);

    void reload_data_ref(int id);

    [[nodiscard]] optional<span<uint16_t>> vram(int id);

    void update();

    void commit();
}

#endif
