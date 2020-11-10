/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_ASSERT_H
#define BTN_ASSERT_H

/**
 * @file
 * BTN_ASSERT and BTN_ERROR header file.
 *
 * @ingroup assert
 */

#include <cassert>
#include "btn_type_traits.h"
#include "btn_config_assert.h"

/**
 * @def BTN_ASSERT(condition, ...)
 *
 * Checks if the specified \a condition is true.
 *
 * If it doesn't, the execution is stopped and diagnostic information is shown on the screen.
 *
 * More information can be shown by passing it by argument.
 *
 * Example:
 *
 * @code{.cpp}
 * BTN_ASSERT(integer > 0, "Invalid integer: ", integer);
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
 * Note that it can be used in constexpr contexts (is_constant_evaluated() returns `true`).
 *
 * @ingroup assert
 */

/**
 * @def BTN_ERROR(...)
 *
 * If this is called, the execution is stopped and diagnostic information is shown on the screen.
 *
 * More information can be shown by passing it by argument.
 *
 * Example:
 *
 * @code{.cpp}
 * BTN_ERROR("Invalid integer: ", integer);
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
 * Note that it can be used in constexpr contexts (is_constant_evaluated() returns `true`).
 *
 * @ingroup assert
 */

#if BTN_CFG_ASSERT_ENABLED
    #include "btn_sstream.h"
    #include "btn_istring_base.h"

    #define BTN_ASSERT(condition, ...) \
        do \
        { \
            if(btn::is_constant_evaluated()) \
            { \
                assert(condition); \
            } \
            else \
            { \
                if(! (condition)) [[unlikely]] \
                { \
                    _btn::assert::constexpr_check(true, #condition, _btn::assert::base_name(__FILE__), __func__, \
                            __LINE__ __VA_OPT__(, ) __VA_ARGS__); \
                } \
            } \
        } while(false)

    #define BTN_ERROR(...) \
        do \
        { \
            if(btn::is_constant_evaluated()) \
            { \
                assert(false); \
            } \
            else \
            { \
                _btn::assert::constexpr_check(true, "", _btn::assert::base_name(__FILE__), __func__, \
                        __LINE__ __VA_OPT__(, ) __VA_ARGS__); \
            } \
        } while(false)

    /// @cond DO_NOT_DOCUMENT

    namespace _btn::assert
    {
        static_assert(BTN_CFG_ASSERT_BUFFER_SIZE >= 64);

        [[noreturn]] void show(const char* condition, const char* file_name, const char* function, int line,
                               const char* message);

        [[noreturn]] void show(const char* condition, const char* file_name, const char* function, int line,
                               const btn::istring_base& message);

        template<int Size>
        [[nodiscard]] constexpr const char* base_name_impl(const char (&char_array)[Size], int index)
        {
            return index >= Size ?
                        char_array :
                        char_array[Size - index] == '/' ?
                            char_array + Size - index + 1 :
                            base_name_impl(char_array, index + 1);
        }

        template<int Size>
        [[nodiscard]] constexpr const char* base_name(const char (&char_array)[Size])
        {
            return base_name_impl(char_array, 2);
        }

        template<typename... Args>
        [[noreturn]] inline void show_args(const char* condition_msg, const char* file, const char* function,
                                           int line, const Args&... args)
        {
            char istring_buffer[BTN_CFG_ASSERT_BUFFER_SIZE];
            btn::istring_base istring(istring_buffer);
            btn::ostringstream string_stream(istring);
            string_stream.append_args(args...);
            _btn::assert::show(condition_msg, file, function, line, istring);
        }

        template<typename... Args>
        constexpr void constexpr_check(bool condition_failed, const char* condition_msg, const char* file,
                                       const char* function, int line, const Args&... args)
        {
            if(condition_failed) [[likely]]
            {
                show_args(condition_msg, file, function, line, args...);
            }
        }
    }

    /// @endcond
#else
    #define BTN_ASSERT(condition, ...) \
        do \
        { \
            if(btn::is_constant_evaluated()) \
            { \
                assert(condition); \
            } \
        } while(false)

    #define BTN_ERROR(...) \
        do \
        { \
            if(btn::is_constant_evaluated()) \
            { \
                assert(false); \
            } \
        } while(false)
#endif

#endif
