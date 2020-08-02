#ifndef BTN_ASSERT_H
#define BTN_ASSERT_H

#include "btn_config_assert.h"

#if BTN_CFG_ASSERT_ENABLED
    #include "btn_sstream.h"

    #define BTN_ASSERT(condition, ...) \
        do \
        { \
            if(! (condition)) [[unlikely]] \
            { \
                char _btn_string[BTN_CFG_ASSERT_BUFFER_SIZE]; \
                btn::istring_base _btn_istring(_btn_string); \
                btn::ostringstream _btn_string_stream(_btn_istring); \
                _btn::assert::append_args(_btn_string_stream, __VA_ARGS__); \
                _btn::assert::show(#condition, _btn::assert::base_name(__FILE__), __func__, __LINE__, _btn_istring); \
            } \
        } while(false)

    #define BTN_CONSTEXPR_ASSERT(condition, message) \
        _btn::assert::constexpr_check(condition, _btn::assert::base_name(__FILE__), __func__, __LINE__, message)

    #define BTN_CONSTEXPR_ERROR(message) \
        _btn::assert::constexpr_check(false, _btn::assert::base_name(__FILE__), __func__, __LINE__, message)

    #define BTN_ERROR(...) \
        do \
        { \
            char _btn_string[BTN_CFG_ASSERT_BUFFER_SIZE]; \
            btn::istring_base _btn_istring(_btn_string); \
            btn::ostringstream _btn_string_stream(_btn_istring); \
            _btn::assert::append_args(_btn_string_stream, __VA_ARGS__); \
            _btn::assert::show("", _btn::assert::base_name(__FILE__), __func__, __LINE__, _btn_istring); \
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

        [[noreturn]] inline void constexpr_error(const char* file, const char* function, int line, const char* message)
        {
            show("", file, function, line, message);
        }

        constexpr bool constexpr_check(bool condition, const char* file, const char* function, int line,
                                       const char* message)
        {
            if(condition) [[likely]]
            {
                return true;
            }

            constexpr_error(file, function, line, message);
        }

        inline void append_args(btn::ostringstream&)
        {
        }

        template<typename Type, typename... Args>
        void append_args(btn::ostringstream& string_stream, const Type& value, const Args&... args)
        {
            string_stream << value;
            append_args(string_stream, args...);
        }

        template<typename Type>
        void append_args(btn::ostringstream& string_stream, const Type& value)
        {
            string_stream << value;
        }
    }
#else
    #define BTN_ASSERT(condition, ...) \
        do \
        { \
        } while(false)

    #define BTN_CONSTEXPR_ASSERT(condition, message) \
        _btn::assert::dummy_constexpr_check()

    #define BTN_CONSTEXPR_ERROR(message) \
        _btn::assert::dummy_constexpr_check()

    #define BTN_ERROR(...) \
        do \
        { \
            BTN_UNREACHABLE(); \
        } while(false)

    namespace _btn::assert
    {
        constexpr bool dummy_constexpr_check()
        {
            return true;
        }
    }
#endif

#endif
