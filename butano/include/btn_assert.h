#ifndef BTN_ASSERT_H
#define BTN_ASSERT_H

#include "btn_common.h"
#include "btn_config_assert.h"

#if BTN_CFG_ASSERT_ENABLED
    #include "btn_input_string_stream.h"

    #define BTN_ASSERT(condition, ...) \
        do \
        { \
            if(! BTN_UNLIKELY(condition)) \
            { \
                btn::string<BTN_CFG_ASSERT_BUFFER_SIZE> _btn_string; \
                btn::input_string_stream _btn_input_string_stream(_btn_string); \
                _btn_input_string_stream.append_args(__VA_ARGS__); \
                _btn::assert::show(#condition, _btn::assert::base_name(__FILE__), __func__, __LINE__, _btn_string); \
            } \
        } while(false)

    #define BTN_CONSTEXPR_ASSERT(condition, message) \
        _btn::assert::constexpr_check(condition, _btn::assert::base_name(__FILE__), __func__, __LINE__, message)

    #define BTN_ERROR(...) \
        do \
        { \
            btn::string<BTN_CFG_ASSERT_BUFFER_SIZE> _btn_string; \
            btn::input_string_stream _btn_input_string_stream(_btn_string); \
            _btn_input_string_stream.append_args(__VA_ARGS__); \
            _btn::assert::show("", _btn::assert::base_name(__FILE__), __func__, __LINE__, _btn_string); \
        } while(false)

    namespace _btn::assert
    {
        void show(const char* condition, const char* file_name, const char* function, int line,
                  const btn::istring& message);

        template<size_t Size>
        [[nodiscard]] constexpr const char* base_name_impl(const char (&char_array)[Size], size_t index) noexcept
        {
            return index >= Size ?
                        char_array :
                        char_array[Size - index] == '/' ?
                            char_array + Size - index + 1 :
                            base_name_impl(char_array, index + 1);
        }

        template<size_t Size>
        [[nodiscard]] constexpr const char* base_name(const char (&char_array)[Size]) noexcept
        {
            return base_name_impl(char_array, 2);
        }

        inline void constexpr_error(const char* file, const char* function, int line, const char* message)
        {
            btn::string<BTN_CFG_ASSERT_BUFFER_SIZE> message_string;
            message_string.append(message);
            show("", file, function, line, message_string);
        }

        constexpr bool constexpr_check(bool condition, const char* file, const char* function, int line,
                                       const char* message)
        {
            return BTN_LIKELY(condition) ?
                        true :
                        (constexpr_error(file, function, line, message), false);
        }
    }
#else
    #define BTN_ASSERT(condition, ...) \
        do \
        { \
        } while(false)

    #define BTN_CONSTEXPR_ASSERT(condition, message) \
        _btn::assert::dummy_constexpr_check()

    #define BTN_ERROR(...) \
        do \
        { \
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
