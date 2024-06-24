/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
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
 * <a href="https://mgba.io">mGBA</a> log backend.
 *
 * It also works with <a href="https://github.com/nba-emu/NanoBoyAdvance">NanoBoyAdvance</a>
 * and <a href="https://github.com/SourMesen/Mesen2">Mesen</a>.
 *
 * @ingroup log
 */
#define BN_LOG_BACKEND_MGBA        0

/**
 * @def BN_LOG_BACKEND_NOCASHGBA
 *
 * <a href="https://problemkaputt.de/gba.htm">No$gba</a> log backend.
 *
 * @ingroup log
 */
#define BN_LOG_BACKEND_NOCASHGBA   1

/**
 * @def BN_LOG_BACKEND_VBA
 *
 * <a href="https://en.wikipedia.org/wiki/VisualBoyAdvance">VisualBoyAdvance</a> log backend.
 *
 * It crashes on hardware and on other emulators!
 *
 * @ingroup log
 */
#define BN_LOG_BACKEND_VBA         2

#endif
