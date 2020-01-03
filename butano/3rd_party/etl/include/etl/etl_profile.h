#ifndef __ETL_PROFILE_H__
#define __ETL_PROFILE_H__

#include "btn_common.h"
#include "btn_config_assert.h"

#if BTN_CFG_ASSERT_ENABLED
    #define ETL_LOG_ERRORS
    #define ETL_VERBOSE_ERRORS
    #define ETL_CHECK_PUSH_POP
#else
    #define ETL_NO_CHECKS
#endif

#include "profiles/gcc_generic.h"

#endif
