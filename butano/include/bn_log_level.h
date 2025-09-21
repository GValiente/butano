/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_LOG_LEVEL_H
#define BN_LOG_LEVEL_H

/**
 * @file
 * bn::log::level header file.
 *
 * @ingroup log
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Specifies the level of a log message.
 *
 * Most emulators ignore this log level.
 *
 * @ingroup log
 */
enum class log_level : uint8_t
{
    FATAL, //!< Fatal. A message with this level stops the execution.
    ERROR, //!< Error.
    WARN, //!< Warn (default).
    INFO, //!< Info.
    DEBUG //!< Debug.
};

}

#endif

