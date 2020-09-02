#ifndef BTN_ASSERT_H
#define BTN_ASSERT_H

#include <assert.h>
#include "btn_type_traits.h"
#include "btn_config_assert.h"

#if BTN_CFG_ASSERT_ENABLED
    #include "btn_sstream.h"

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
