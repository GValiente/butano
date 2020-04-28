#ifndef BTN_HW_SPRITE_AFFINE_MATS_H
#define BTN_HW_SPRITE_AFFINE_MATS_H

#include "tonc.h"
#include "btn_sprite_affine_mat_attributes.h"

namespace btn::hw::sprite_affine_mats
{
    class handle
    {

    public:
        int data[8];
    };


    namespace
    {
        static_assert(sizeof(OBJ_AFFINE) == sizeof(handle));
        static_assert(alignof(OBJ_AFFINE) == alignof(handle));

        handle* vram()
        {
            return reinterpret_cast<handle*>(MEM_OAM);
        }
    }


    inline void setup(handle& affine_mat)
    {
        auto affine_mat_ptr = reinterpret_cast<OBJ_AFFINE*>(&affine_mat);
        affine_mat_ptr->pa = 1 << 8;
        affine_mat_ptr->pb = 0;
        affine_mat_ptr->pc = 0;
        affine_mat_ptr->pd = 1 << 8;
    };

    inline void setup(const sprite_affine_mat_attributes& attributes, handle& affine_mat)
    {
        auto affine_mat_ptr = reinterpret_cast<OBJ_AFFINE*>(&affine_mat);
        affine_mat_ptr->pa = int16_t(attributes.first_register_value());
        affine_mat_ptr->pb = int16_t(attributes.second_register_value());
        affine_mat_ptr->pc = int16_t(attributes.third_register_value());
        affine_mat_ptr->pd = int16_t(attributes.fourth_register_value());
    };

    inline void update_scale_x(const sprite_affine_mat_attributes& attributes, handle& affine_mat)
    {
        auto affine_mat_ptr = reinterpret_cast<OBJ_AFFINE*>(&affine_mat);
        affine_mat_ptr->pa = int16_t(attributes.first_register_value());
        affine_mat_ptr->pb = int16_t(attributes.second_register_value());
    };

    inline void update_scale_y(const sprite_affine_mat_attributes& attributes, handle& affine_mat)
    {
        auto affine_mat_ptr = reinterpret_cast<OBJ_AFFINE*>(&affine_mat);
        affine_mat_ptr->pc = int16_t(attributes.third_register_value());
        affine_mat_ptr->pd = int16_t(attributes.fourth_register_value());
    };

    [[nodiscard]] inline int16_t* first_register(int id)
    {
        handle& handle = vram()[id];
        auto affine_mat_ptr = reinterpret_cast<OBJ_AFFINE*>(&handle);
        return &affine_mat_ptr->pa;
    }

    [[nodiscard]] inline int16_t* second_register(int id)
    {
        handle& handle = vram()[id];
        auto affine_mat_ptr = reinterpret_cast<OBJ_AFFINE*>(&handle);
        return &affine_mat_ptr->pb;
    }

    [[nodiscard]] inline int16_t* third_register(int id)
    {
        handle& handle = vram()[id];
        auto affine_mat_ptr = reinterpret_cast<OBJ_AFFINE*>(&handle);
        return &affine_mat_ptr->pc;
    }

    [[nodiscard]] inline int16_t* fourth_register(int id)
    {
        handle& handle = vram()[id];
        auto affine_mat_ptr = reinterpret_cast<OBJ_AFFINE*>(&handle);
        return &affine_mat_ptr->pd;
    }
}

#endif
