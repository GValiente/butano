/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HW_SPRITE_AFFINE_MATS_H
#define BTN_HW_SPRITE_AFFINE_MATS_H

#include "btn_sprite_affine_mat_attributes.h"
#include "btn_hw_tonc.h"

namespace btn::hw::sprite_affine_mats
{
    using handle = OBJ_AFFINE;


    namespace
    {
        handle* vram()
        {
            return reinterpret_cast<handle*>(MEM_OAM);
        }
    }


    inline void setup(handle& affine_mat)
    {
        affine_mat.pa = 1 << 8;
        affine_mat.pb = 0;
        affine_mat.pc = 0;
        affine_mat.pd = 1 << 8;
    }

    inline void setup(const sprite_affine_mat_attributes& attributes, handle& affine_mat)
    {
        affine_mat.pa = int16_t(attributes.pa_register_value());
        affine_mat.pb = int16_t(attributes.pb_register_value());
        affine_mat.pc = int16_t(attributes.pc_register_value());
        affine_mat.pd = int16_t(attributes.pd_register_value());
    }

    inline void update_horizontal_scale(const sprite_affine_mat_attributes& attributes, handle& affine_mat)
    {
        affine_mat.pa = int16_t(attributes.pa_register_value());
        affine_mat.pb = int16_t(attributes.pb_register_value());
    }

    inline void update_vertical_scale(const sprite_affine_mat_attributes& attributes, handle& affine_mat)
    {
        affine_mat.pc = int16_t(attributes.pc_register_value());
        affine_mat.pd = int16_t(attributes.pd_register_value());
    }

    [[nodiscard]] inline int16_t* pa_register(int id)
    {
        handle& affine_mat = vram()[id];
        return &affine_mat.pa;
    }

    [[nodiscard]] inline int16_t* pb_register(int id)
    {
        handle& affine_mat = vram()[id];
        return &affine_mat.pb;
    }

    [[nodiscard]] inline int16_t* pc_register(int id)
    {
        handle& affine_mat = vram()[id];
        return &affine_mat.pc;
    }

    [[nodiscard]] inline int16_t* pd_register(int id)
    {
        handle& affine_mat = vram()[id];
        return &affine_mat.pd;
    }
}

#endif
