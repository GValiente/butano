/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_CONSTANTS_3D_H
#define FR_CONSTANTS_3D_H

#include "bn_common.h"

#ifndef FR_PROFILE
    #define FR_PROFILE false
#endif

#ifndef FR_DETAILED_PROFILE
    #define FR_DETAILED_PROFILE false
#endif

#ifndef FR_SHOW_CPU_USAGE
    #define FR_SHOW_CPU_USAGE false
#endif

#ifndef FR_SHOW_CPU_USAGE_MAX
    #define FR_SHOW_CPU_USAGE_MAX false
#endif

#ifndef FR_SHOW_CPU_USAGE_MIN
    #define FR_SHOW_CPU_USAGE_MIN false
#endif

#ifndef FR_SHOW_CPU_USAGE_CURRENT
    #define FR_SHOW_CPU_USAGE_CURRENT false
#endif

#ifndef FR_LOG_POLYGONS_PER_SECOND
    #define FR_LOG_POLYGONS_PER_SECOND false
#endif

#ifndef FR_SKIP_RACE_INTRO
    #define FR_SKIP_RACE_INTRO false
#endif

namespace fr::constants_3d
{
    constexpr int focal_length_shift = 8;

    constexpr int max_dynamic_models = 4;
    constexpr int max_static_models = 32 - max_dynamic_models;
    constexpr int max_stage_models = 1024;
    constexpr int max_sprites = 8;

    constexpr int camera_min_y = 224;
    constexpr int camera_max_y = 256;
    constexpr int camera_diff_y = camera_max_y - camera_min_y;

    constexpr int max_rival_cars = 8;

    constexpr int music_volume_data = 1 << 11;
}

#endif
