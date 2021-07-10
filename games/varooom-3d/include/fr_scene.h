/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_SCENE_H
#define FR_SCENE_H

#include "bn_optional_fwd.h"

namespace fr
{

enum class scene_type;

class scene
{

public:
    virtual ~scene() = default;

    [[nodiscard]] virtual bn::optional<scene_type> update() = 0;

protected:
    scene() = default;
};

}

#endif
