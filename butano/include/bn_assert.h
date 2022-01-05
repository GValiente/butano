/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_ASSERT_H
#define BN_ASSERT_H

/**
 * @file
 * BN_ASSERT and BN_ERROR header file.
 *
 * @ingroup assert
 */

#include <cassert>
#include "bn_type_traits.h"
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
 * Note that it can be used in constexpr contexts (is_constant_evaluated() returns `true`).
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
 * Note that it can be used in constexpr contexts (is_constant_evaluated() returns `true`).
 *
 * @ingroup assert
 */

#if BN_CFG_ASSERT_ENABLED || BN_DOXYGEN
    #include "bn_sstream.h"
    #include "bn_istring_base.h"

    #ifndef BN_ASSERT

        #define BN_ASSERT(condition, ...) \
            do \
            { \
                if(bn::is_constant_evaluated()) \
                { \
                    assert(condition); \
                } \
                else \
                { \
                    if(! (condition)) [[unlikely]] \
                    { \
                        constexpr _bn::assert::file_name<_bn::assert::file_name_size(__FILE__) + 1> _bn_assert_file_name(__FILE__); \
                        _bn::assert::show_args(#condition, _bn_assert_file_name.characters, __func__, \
                                __LINE__ __VA_OPT__(, ) __VA_ARGS__); \
                    } \
                } \
            } while(false)

    #endif

    #ifndef BN_ERROR

        #define BN_ERROR(...) \
            do \
            { \
                if(bn::is_constant_evaluated()) \
                { \
                    assert(false); \
                } \
                else \
                { \
                    constexpr _bn::assert::file_name<_bn::assert::file_name_size(__FILE__) + 1> _bn_error_file_name(__FILE__); \
                    _bn::assert::show_args("", _bn_error_file_name.characters, __func__, \
                            __LINE__ __VA_OPT__(, ) __VA_ARGS__); \
                } \
            } while(false)

    #endif

    /// @cond DO_NOT_DOCUMENT

    namespace _bn::assert
    {
        static_assert(BN_CFG_ASSERT_BUFFER_SIZE >= 64);

        constexpr int file_name_size(const char* path)
        {
            const char* name_path = path;

            while(*path)
            {
                if(*path == '/' || *path == '\\')
                {
                    name_path = path + 1;
                }

                ++path;
            }

            return path - name_path;
        }

        template<int Size>
        struct file_name
        {
            static_assert(Size > 0 && Size < BN_CFG_ASSERT_BUFFER_SIZE);

            alignas(int) char characters[Size < 20 ? 20 : Size];

            constexpr explicit file_name(const char* path) :
                characters()
            {
                int index = 0;

                while(*path)
                {
                    if(*path == '/' || *path == '\\')
                    {
                        index = 0;
                    }
                    else if(index < Size)
                    {
                        characters[index] = *path;
                        ++index;
                    }

                    ++path;
                }

                characters[index] = '\0';
            }
        };

        [[noreturn]] void show(const char* condition, const char* file_name, const char* function, int line,
                               const char* message);

        [[noreturn]] void show(const char* condition, const char* file_name, const char* function, int line,
                               const bn::istring_base& message);

        [[noreturn]] inline void show_args(const char* condition_msg, const char* file, const char* function,
                                           int line)
        {
            _bn::assert::show(condition_msg, file, function, line, "");
        }

        template<typename Arg>
        [[noreturn]] void show_args(const char* condition_msg, const char* file, const char* function, int line,
                                    const Arg& arg)
        {
            char istring_buffer[BN_CFG_ASSERT_BUFFER_SIZE];
            bn::istring_base istring(istring_buffer);
            bn::ostringstream string_stream(istring);
            string_stream.append(arg);
            _bn::assert::show(condition_msg, file, function, line, istring);
        }

        template<typename Arg1, typename Arg2>
        [[noreturn]] void show_args(const char* condition_msg, const char* file, const char* function, int line,
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
        [[noreturn]] void show_args(const char* condition_msg, const char* file, const char* function, int line,
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
                if(bn::is_constant_evaluated()) \
                { \
                    assert(condition); \
                } \
            } while(false)

    #endif

    #ifndef BN_ERROR

        #define BN_ERROR(...) \
            do \
            { \
                if(bn::is_constant_evaluated()) \
                { \
                    assert(false); \
                } \
            } while(false)

    #endif
#endif

#endif
