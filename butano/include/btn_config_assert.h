#ifndef BTN_CONFIG_ASSERT_H
#define BTN_CONFIG_ASSERT_H

#include "btn_common.h"

#ifndef BTN_CFG_ASSERT_ENABLED
    #define BTN_CFG_ASSERT_ENABLED true
#endif

#ifndef BTN_CFG_ASSERT_BUFFER_SIZE
    #define BTN_CFG_ASSERT_BUFFER_SIZE 512
#endif

#endif
