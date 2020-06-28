#include "polygon.h"

void polygon::_setup_affine_mat_register_values(p_fixed tymul, p_fixed scale_y, int base_y, bool negative)
{
    // pc: 8*iy*(tymul - 1)/(tymul + 1)

    p_fixed tymul_minus1_8 = tymul - 1;
    tymul_minus1_8 = tymul_minus1_8.hp_multiplication(scale_y);

    if(negative)
    {
        tymul_minus1_8 *= -8;
    }
    else
    {
        tymul_minus1_8 *= 8;
    }

    p_fixed tymul_plus_1 = tymul + 1;
    p_fixed tymul_plus_1_rec = p_fixed(1).unsafe_division(tymul_plus_1);
    int16_t* register_data = _affine_mat_register_values.data();

    for(int index = 0; index < 64; ++index)
    {
        p_fixed iy = index - 32;
        p_fixed iy_tymul_minus1_8 = iy.unsafe_multiplication(tymul_minus1_8);
        p_fixed pc = iy_tymul_minus1_8.unsafe_multiplication(tymul_plus_1_rec);
        register_data[index + base_y] = int16_t(pc.integer());
    }
}
