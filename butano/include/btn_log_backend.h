/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_LOG_BACKEND_H
#define BTN_LOG_BACKEND_H

#include "btn_common.h"

/*!
 * @brief VisualBoyAdvance emulator (https://en.wikipedia.org/wiki/VisualBoyAdvance) log backend.
 *
 * It crashes on hardware and on other emulators!
 */
#define BTN_LOG_BACKEND_VBA         0

/*!
 * @brief No$gba emulator (https://problemkaputt.de/gba.htm) log backend.
 */
#define BTN_LOG_BACKEND_NOCASHGBA   1

/*!
 * @brief mGBA emulator (https://mgba.io) log backend.
 */
#define BTN_LOG_BACKEND_MGBA        2

#endif
