/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_SCENE_H
#define BF_SCENE_H

#include "bn_optional.h"

namespace bf
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
