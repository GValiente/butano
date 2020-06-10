#ifndef BTN_COMMON_H
#define BTN_COMMON_H

#include <stdint.h>
#include "../hw/include/btn_hw_common.h"

/*!
 * @brief If control flow reaches the point of this macro, the program is undefined.
 */
#define BTN_UNREACHABLE __builtin_unreachable

#endif
