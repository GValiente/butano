#ifndef BTN_LOG_H
#define BTN_LOG_H

#include "btn_common.h"
#include "btn_config_log.h"

#if BTN_CFG_LOG_ENABLED
    #include "btn_input_string_stream.h"

    #define BTN_LOG(...) \
        do \
        { \
            btn::string<0x100> _btn_string; \
            btn::input_string_stream _btn_input_string_stream(_btn_string); \
            _btn_input_string_stream.append_args(__VA_ARGS__); \
            btn::log(_btn_string); \
        } while(false)

    namespace btn
    {
        void log(const istring& message);
    }
#else
    #define BTN_LOG(...) \
        do \
        { \
        } while(false)
#endif

#endif
