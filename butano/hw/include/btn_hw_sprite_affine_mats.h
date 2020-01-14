#ifndef BTN_HW_SPRITE_AFFINE_MATS_H
#define BTN_HW_SPRITE_AFFINE_MATS_H

#include "btn_fixed_fwd.h"

namespace btn
{
    class sprite_affine_mat_builder;
}

namespace btn::hw::sprite_affine_mats
{
    class item_type
    {

    public:
        int16_t sin;
        int16_t cos;
        uint16_t sx;
        uint16_t sy;
        int8_t hflip;
        int8_t vflip;

        void init(const sprite_affine_mat_builder& builder);

        void set_rotation_angle(fixed rotation_angle);

        void set_scale_x(fixed scale_x);

        void set_scale_y(fixed scale_y);

        void set_horizontal_flip(bool horizontal_flip);

        void set_vertical_flip(bool vertical_flip);
    };

    class handle
    {

    public:
        int data[8];
    };

    [[nodiscard]] constexpr int count()
    {
        return 32;
    }

    void setup(const item_type& item, handle& affine_mat);
}

#endif
