/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_LOG_H
#define BN_LOG_H

/**
 * @file
 * BN_LOG header file.
 *
 * @ingroup log
 */

#include "bn_config_log.h"
#include "bn_config_doxygen.h"

/**
 * @def BN_LOG(...)
 *
 * Prints in one line of text the representation of the given parameters
 * with the default log level (bn::log_level::WARN).
 *
 * Example:
 *
 * @code{.cpp}
 * BN_LOG("Invalid integer: ", integer);
 * @endcode
 *
 * Custom parameter types are supported by overloading bn::ostringstream::operator<<.
 *
 * Example:
 *
 * @code{.cpp}
 * bn::ostringstream& operator<<(bn::ostringstream& stream, const custom_type& custom_value)
 * {
 *     stream.append("custom_type: ");
 *     stream.append(custom_value.data);
 *     return stream;
 * }
 * @endcode
 *
 * @ingroup log
 */

/**
 * @def BN_LOG_LEVEL(level, ...)
 *
 * Prints in one line of text the representation of the given parameters with the specified log level.
 *
 * Example:
 *
 * @code{.cpp}
 * BN_LOG_LEVEL(bn::log_level::ERROR, "Invalid integer: ", integer);
 * @endcode
 *
 * Custom parameter types are supported by overloading bn::ostringstream::operator<<.
 *
 * Example:
 *
 * @code{.cpp}
 * bn::ostringstream& operator<<(bn::ostringstream& stream, const custom_type& custom_value)
 * {
 *     stream.append("custom_type: ");
 *     stream.append(custom_value.data);
 *     return stream;
 * }
 * @endcode
 *
 * @ingroup log
 */

namespace bn
{
    enum class log_level : uint8_t;
}

#if BN_CFG_LOG_ENABLED || BN_DOXYGEN
    #include "bn_sstream.h"
    #include "bn_istring_base.h"

    #define BN_LOG(...) \
        do \
        { \
            char _bn_string[BN_CFG_LOG_MAX_SIZE]; \
            bn::istring_base _bn_istring(_bn_string); \
            bn::ostringstream _bn_string_stream(_bn_istring); \
            _bn_string_stream.append_args(__VA_ARGS__); \
            bn::log(_bn_istring); \
        } while(false)

    #define BN_LOG_LEVEL(level, ...) \
        do \
        { \
            char _bn_string[BN_CFG_LOG_MAX_SIZE]; \
            bn::istring_base _bn_istring(_bn_string); \
            bn::ostringstream _bn_string_stream(_bn_istring); \
            _bn_string_stream.append_args(__VA_ARGS__); \
            bn::log(level, _bn_istring); \
        } while(false)

    namespace bn
    {
        /**
         * @brief Prints in one line of text the given message with the default log level (bn::log_level::WARN).
         *
         * @ingroup log
         */
        void log(const istring_base& message);

        /**
         * @brief Prints in one line of text the given message.
         * @param level Message level.
         * @param message Message to print.
         *
         * @ingroup log
         */
        void log(log_level level, const istring_base& message);
    }
#else
    #define BN_LOG(...) \
        do \
        { \
        } while(false)

    #define BN_LOG_LEVEL(level, ...) \
        do \
        { \
        } while(false)
#endif

#endif
