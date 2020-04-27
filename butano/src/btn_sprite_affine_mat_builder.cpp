#include "btn_sprite_affine_mat_builder.h"

#include "btn_optional.h"
#include "btn_sprite_affine_mat_ptr.h"

namespace btn
{

sprite_affine_mat_ptr sprite_affine_mat_builder::build() const
{
    return sprite_affine_mat_ptr::create(_attributes);
}

optional<sprite_affine_mat_ptr> sprite_affine_mat_builder::optional_build() const
{
    return sprite_affine_mat_ptr::optional_create(_attributes);
}

}
