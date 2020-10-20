/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_SCENE_H
#define BF_SCENE_H

#include "btn_optional_fwd.h"

namespace bf
{

enum class scene_type;

class scene
{

public:
    virtual ~scene() = default;

    [[nodiscard]] virtual btn::optional<scene_type> update() = 0;

protected:
    scene() = default;
};

}

#endif
