/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_SCENE_TYPE_H
#define BF_SCENE_TYPE_H

#include "bn_common.h"

namespace bf
{
    enum class scene_type
    {
        INTRO,
        TITLE,
        GAME,
        HOW_TO_PLAY_AND_TITLE,
        HOW_TO_PLAY_AND_GAME,
        CREDITS,
        ENDING
    };
}

#endif
