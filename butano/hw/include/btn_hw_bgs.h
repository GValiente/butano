#ifndef BTN_HW_BGS_H
#define BTN_HW_BGS_H

#include "btn_common.h"

namespace btn
{

class size;
class bg_builder;

namespace hw::bgs
{
    class handle
    {

    public:
        uint16_t cnt;
        uint16_t hofs;
        uint16_t vofs;
    };

    [[nodiscard]] constexpr int count()
    {
        return 4;
    }

    [[nodiscard]] constexpr int max_priority()
    {
        return count() - 1;
    }

    void setup(const bg_builder& builder, int tiles_id, int map_id, const size& map_dimensions,
               bool eight_bits_per_pixel, handle& bg);

    void set_tiles(int tiles_id, handle& bg);

    void set_map(int map_id, const size& map_dimensions, handle& bg);

    void set_position(int x, int y, handle& bg);

    [[nodiscard]] int priority(const handle& bg);

    void set_priority(int priority, handle& bg);

    [[nodiscard]] bool mosaic_enabled(const handle& bg);

    void set_mosaic_enabled(bool mosaic_enabled, handle& bg);

    void commit(const handle& bgs_ref);
}
}

#endif
