#ifndef BTN_HW_COMMON_H
#define BTN_HW_COMMON_H

/*!
 * @brief Store data in EWRAM.
 */
#define BTN_DATA_EWRAM __attribute__((section(".ewram")))

/*!
 * @brief Store code in IWRAM.
 */
#define BTN_CODE_IWRAM __attribute__((section(".iwram"), long_call))

#endif
