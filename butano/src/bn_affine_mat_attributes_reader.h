/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_mat_attributes.h"

namespace bn
{
    class affine_mat_attributes_reader
    {

    public:
        affine_mat_attributes_reader(const affine_mat_attributes& attributes) :
            _attributes(attributes)
        {
        }

        [[nodiscard]] int sin() const
        {
            return _attributes._sin;
        }

        [[nodiscard]] int cos() const
        {
            return _attributes._cos;
        }

        [[nodiscard]] unsigned sx() const
        {
            return _attributes._sx;
        }

        [[nodiscard]] unsigned sy() const
        {
            return _attributes._sy;
        }

    private:
        const affine_mat_attributes& _attributes;
    };
}
