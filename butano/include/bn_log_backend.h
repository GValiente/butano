/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_LOG_BACKEND_H
#define BN_LOG_BACKEND_H

/**
 * @file
 * Available log backends header file.
 *
 * @ingroup log
 */

#include "bn_common.h"

/**
 * @def BN_LOG_BACKEND_MGBA
 *
 * mGBA emulator (https://mgba.io) log backend.
 *
 * @ingroup log
 */
#define BN_LOG_BACKEND_MGBA        0

/**
 * @def BN_LOG_BACKEND_NOCASHGBA
 *
 * No$gba emulator (https://problemkaputt.de/gba.htm) log backend.
 *
 * @ingroup log
 */
#define BN_LOG_BACKEND_NOCASHGBA   1

/**
 * @def BN_LOG_BACKEND_VBA
 *
 * VisualBoyAdvance emulator (https://en.wikipedia.org/wiki/VisualBoyAdvance) log backend.
 *
 * It crashes on hardware and on other emulators!
 *
 * @ingroup log
 */
#define BN_LOG_BACKEND_VBA         2

#endif
