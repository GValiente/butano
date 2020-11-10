/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_LOG_H
#define BTN_LOG_H

/**
 * @file
 * BTN_LOG header file.
 *
 * @ingroup log
 */

#include "btn_config_log.h"

/**
 * @def BTN_LOG(...)
 *
 * Prints in one line of text the representation of the given parameters.
 *
 * Example:
 *
 * @code{.cpp}
 * BTN_LOG("Invalid integer: ", integer);
 * @endcode
 *
 * Custom parameter types are supported by overloading btn::ostringstream::operator<<.
 *
 * Example:
 *
 * @code{.cpp}
 * btn::ostringstream& operator<<(btn::ostringstream& stream, const custom_type& custom_value)
 * {
 *     stream.append("custom_type: ");
 *     stream.append(custom_value.data);
 *     return stream;
 * }
 * @endcode
 *
 * @ingroup log
 */

#if BTN_CFG_LOG_ENABLED
    #include "btn_sstream.h"
    #include "btn_istring_base.h"

    #define BTN_LOG(...) \
        do \
        { \
            char _btn_string[BTN_CFG_LOG_MAX_SIZE]; \
            btn::istring_base _btn_istring(_btn_string); \
            btn::ostringstream _btn_string_stream(_btn_istring); \
            _btn_string_stream.append_args(__VA_ARGS__); \
            btn::log(_btn_istring); \
        } while(false)

    namespace btn
    {
        /**
         * @brief Prints in one line of text the given message.
         *
         * @ingroup log
         */
        void log(const istring_base& message);
    }
#else
    #define BTN_LOG(...) \
        do \
        { \
        } while(false)
#endif

#endif
