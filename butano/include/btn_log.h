#ifndef BTN_LOG_H
#define BTN_LOG_H

#include "btn_config_log.h"

#if BTN_CFG_LOG_ENABLED
    #include "btn_input_string_stream.h"

    #define BTN_LOG(...) \
        do \
        { \
            char _btn_string[BTN_CFG_LOG_MAX_SIZE]; \
            btn::istring_base _btn_istring(_btn_string); \
            btn::input_string_stream _btn_input_string_stream(_btn_istring); \
            _btn_input_string_stream.append_args(__VA_ARGS__); \
            btn::log(_btn_istring); \
        } while(false)

    namespace btn
    {
        void log(const istring_base& message);
    }
#else
    #define BTN_LOG(...) \
        do \
        { \
        } while(false)
#endif

#endif
