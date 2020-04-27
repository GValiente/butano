#ifndef BTN_HW_SPRITE_AFFINE_MATS_H
#define BTN_HW_SPRITE_AFFINE_MATS_H

#include "tonc.h"
#include "btn_math.h"
#include "btn_algorithm.h"
#include "btn_sprite_affine_mat_attributes.h"

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

        void init()
        {
            sin = 0;
            cos = int16_t(fixed(1).value());
            sx = uint16_t(fixed_t<8>(1).value());
            sy = uint16_t(fixed_t<8>(1).value());
            hflip = 1;
            vflip = 1;
        }

        void init(const sprite_affine_mat_attributes& attributes)
        {
            set_rotation_angle(attributes.rotation_angle());
            set_scale_x(attributes.scale_x());
            set_scale_y(attributes.scale_y());
            set_horizontal_flip(attributes.horizontal_flip());
            set_vertical_flip(attributes.vertical_flip());
        }

        void set_rotation_angle(fixed rotation_angle)
        {
            if(rotation_angle == 0)
            {
                sin = 0;
                cos = int16_t(fixed(1).value());
            }
            else
            {
                sin = int16_t(degrees_sin(rotation_angle).value());
                cos = int16_t(degrees_cos(rotation_angle).value());
            }
        }

        void set_scale_x(fixed scale_x)
        {
            if(scale_x == 1)
            {
                sx = uint16_t(fixed_t<8>(1).value());
            }
            else
            {
                fixed inv_scale_x = 1 / scale_x;
                sx = uint16_t(fixed_t<8>(inv_scale_x).value());
            }
        }

        void set_scale_y(fixed scale_y)
        {
            if(scale_y == 1)
            {
                sy = uint16_t(fixed_t<8>(1).value());
            }
            else
            {
                fixed inv_scale_y = 1 / scale_y;
                sy = uint16_t(fixed_t<8>(inv_scale_y).value());
            }
        }

        void set_horizontal_flip(bool horizontal_flip)
        {
            hflip = 1 - (2 * horizontal_flip);
        }

        void set_vertical_flip(bool vertical_flip)
        {
            vflip = 1 - (2 * vertical_flip);
        }
    };


    class handle
    {

    public:
        int data[8];
    };


    namespace
    {
        static_assert(sizeof(OBJ_AFFINE) == sizeof(handle));
        static_assert(alignof(OBJ_AFFINE) == alignof(handle));
    }


    [[nodiscard]] inline bool double_size(const item_type& item, fixed scale)
    {
        int cos = abs(int(item.cos));
        int sin = abs(int(item.sin));
        int size = (cos + sin) * scale.value();
        return size > 256 << 16;
    }

    inline void setup(handle& affine_mat)
    {
        auto affine_mat_ptr = reinterpret_cast<OBJ_AFFINE*>(&affine_mat);
        affine_mat_ptr->pa = 1 << 8;
        affine_mat_ptr->pb = 0;
        affine_mat_ptr->pc = 0;
        affine_mat_ptr->pd = 1 << 8;
    };

    inline void setup(const item_type& item, handle& affine_mat)
    {
        auto affine_mat_ptr = reinterpret_cast<OBJ_AFFINE*>(&affine_mat);
        int cos = item.cos;
        int sin = item.sin;
        int sx = item.hflip * item.sx;
        int sy = item.vflip * item.sy;
        affine_mat_ptr->pa = int16_t(cos * sx >> 12);
        affine_mat_ptr->pb = int16_t(-sin * sx >> 12);
        affine_mat_ptr->pc = int16_t(sin * sy >> 12);
        affine_mat_ptr->pd = int16_t(cos * sy >> 12);
    };
}

#endif
