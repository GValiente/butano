/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_ASSERT_H
#define BN_ASSERT_H

/**
 * @file
 * BN_ASSERT, BN_BASIC_ASSERT and BN_ERROR header file.
 *
 * @ingroup assert
 */

#include <cassert>
#include "bn_config_assert.h"
#include "bn_config_doxygen.h"

/**
 * @def BN_ASSERT(condition, ...)
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
 * BN_ASSERT(integer > 0, "Invalid integer: ", integer);
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
 * Note that it can be used in constexpr contexts (`if consteval` returns `true`).
 *
 * @ingroup assert
 */

/**
 * @def BN_BASIC_ASSERT(condition, ...)
 *
 * Checks if the specified \a condition is true.
 *
 * If it doesn't, the execution is stopped and basic diagnostic information is shown on the screen.
 *
 * More information can be shown by passing it by argument.
 *
 * Example:
 *
 * @code{.cpp}
 * BN_BASIC_ASSERT(integer > 0, "Invalid integer: ", integer);
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
 * Note that it can be used in constexpr contexts (`if consteval` returns `true`).
 *
 * @ingroup assert
 */

/**
 * @def BN_ERROR(...)
 *
 * If this is called, the execution is stopped and diagnostic information is shown on the screen.
 *
 * More information can be shown by passing it by argument.
 *
 * Example:
 *
 * @code{.cpp}
 * BN_ERROR("Invalid integer: ", integer);
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
 * Note that it can be used in constexpr contexts (`if consteval` returns `true`).
 *
 * @ingroup assert
 */

/**
 * @def BN_ASSERT_FILE
 *
 * Current input file name if available; otherwise an empty string.
 *
 * @ingroup assert
 */

#ifdef __FILE_NAME__
    #define BN_ASSERT_FILE __FILE_NAME__
#else
    #define BN_ASSERT_FILE ""
#endif

#if BN_CFG_ASSERT_ENABLED || BN_DOXYGEN
    #include "bn_sstream.h"
    #include "bn_istring_base.h"
    #include "bn_assert_callback_type.h"

    /**
     * @brief Assert related functions.
     *
     * @ingroup assert
     */
    namespace bn::assert
    {
        /**
         * @brief Returns the user function called when an assert is triggered, before stopping execution.
         */
        [[nodiscard]] callback_type callback();

        /**
         * @brief Sets the user function called when an assert is triggered, before stopping execution.
         */
        void set_callback(callback_type callback);
    }

    #ifndef BN_ASSERT
        #if BN_CFG_ASSERT_SHOW_DIAGNOSTIC
            #define BN_ASSERT(condition, ...) \
                do \
                { \
                    if consteval \
                    { \
                        assert(condition); \
                    } \
                    else \
                    { \
                        if(! (condition)) [[unlikely]] \
                        { \
                            _bn::assert::show_args(#condition, BN_ASSERT_FILE, __func__, __LINE__ __VA_OPT__(, ) __VA_ARGS__); \
                        } \
                    } \
                } while(false)
        #else
            #define BN_ASSERT(condition, ...) \
                do \
                { \
                    if consteval \
                    { \
                        assert(condition); \
                    } \
                    else \
                    { \
                        if(! (condition)) [[unlikely]] \
                        { \
                            _bn::assert::show(BN_ASSERT_FILE, __LINE__); \
                        } \
                    } \
                } while(false)
        #endif
    #endif

    #ifndef BN_BASIC_ASSERT
        #if BN_CFG_ASSERT_SHOW_DIAGNOSTIC
            #define BN_BASIC_ASSERT(condition, ...) \
                do \
                { \
                    if consteval \
                    { \
                        assert(condition); \
                    } \
                    else \
                    { \
                        if(! (condition)) [[unlikely]] \
                        { \
                            _bn::assert::show_args("", BN_ASSERT_FILE, __func__, __LINE__ __VA_OPT__(, ) __VA_ARGS__); \
                        } \
                    } \
                } while(false)
        #else
            #define BN_BASIC_ASSERT BN_ASSERT
        #endif
    #endif

    #ifndef BN_ERROR
        #if BN_CFG_ASSERT_SHOW_DIAGNOSTIC
            #define BN_ERROR(...) \
                do \
                { \
                    if consteval \
                    { \
                        assert(false); \
                    } \
                    else \
                    { \
                        _bn::assert::show_args("", BN_ASSERT_FILE, __func__, __LINE__ __VA_OPT__(, ) __VA_ARGS__); \
                    } \
                } while(false)
        #else
            #define BN_ERROR(...) \
                do \
                { \
                    if consteval \
                    { \
                        assert(false); \
                    } \
                    else \
                    { \
                        _bn::assert::show(BN_ASSERT_FILE, __LINE__); \
                    } \
                } while(false)
        #endif
    #endif

    /// @cond DO_NOT_DOCUMENT

    namespace _bn::assert
    {
        static_assert(BN_CFG_ASSERT_BUFFER_SIZE >= 64);

        [[noreturn]] void show(const char* file_name, int line);

        [[noreturn]] void show(const char* condition, const char* file_name, const char* function, int line);

        [[noreturn]] void show(const char* condition, const char* file_name, const char* function, int line,
                               const char* message);

        [[noreturn]] void show(const char* condition, const char* file_name, const char* function, int line,
                               const bn::istring_base& message);

        [[noreturn]] inline void show_args(
            const char* condition_msg, const char* file, const char* function, int line)
        {
            _bn::assert::show(condition_msg, file, function, line);
        }

        template<typename Arg>
        void show_args(const char* condition_msg, const char* file, const char* function, int line,
                       const Arg& arg)
        {
            char istring_buffer[BN_CFG_ASSERT_BUFFER_SIZE];
            bn::istring_base istring(istring_buffer);
            bn::ostringstream string_stream(istring);
            string_stream.append(arg);
            _bn::assert::show(condition_msg, file, function, line, istring);
        }

        template<typename Arg1, typename Arg2>
        void show_args(const char* condition_msg, const char* file, const char* function, int line,
                       const Arg1& arg1, const Arg2& arg2)
        {
            char istring_buffer[BN_CFG_ASSERT_BUFFER_SIZE];
            bn::istring_base istring(istring_buffer);
            bn::ostringstream string_stream(istring);
            string_stream.append(arg1);
            string_stream.append(arg2);
            _bn::assert::show(condition_msg, file, function, line, istring);
        }

        template<typename... Args>
        void show_args(const char* condition_msg, const char* file, const char* function, int line,
                       const Args&... args)
        {
            char istring_buffer[BN_CFG_ASSERT_BUFFER_SIZE];
            bn::istring_base istring(istring_buffer);
            bn::ostringstream string_stream(istring);
            string_stream.append_args(args...);
            _bn::assert::show(condition_msg, file, function, line, istring);
        }
    }

    /// @endcond
#else
    #ifndef BN_ASSERT
        #define BN_ASSERT(condition, ...) \
            do \
            { \
                if consteval \
                { \
                    assert(condition); \
                } \
                else \
                { \
                    [[assume(condition)]]; \
                } \
            } while(false)
    #endif

    #ifndef BN_BASIC_ASSERT
        #define BN_BASIC_ASSERT BN_ASSERT
    #endif

    #ifndef BN_ERROR
        #define BN_ERROR(...) \
            do \
            { \
                if consteval \
                { \
                    assert(false); \
                } \
                else \
                { \
                    [[assume(false)]]; \
                } \
            } while(false)
    #endif
#endif

#endif
