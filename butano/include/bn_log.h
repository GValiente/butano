/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
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
 * Prints in one line of text the representation of the given parameters.
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

    namespace bn
    {
        /**
         * @brief Prints in one line of text the given message.
         *
         * @ingroup log
         */
        void log(const istring_base& message);
    }
#else
    #define BN_LOG(...) \
        do \
        { \
        } while(false)
#endif

#endif
