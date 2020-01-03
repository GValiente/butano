#ifndef BTN_HW_COMMON_H
#define BTN_HW_COMMON_H

/*!
 * @brief Store data in EWRAM.
 */
#define BTN_DATA_EWRAM __attribute__((section(".ewram")))

/*!
 * @brief Store data in IWRAM.
 */
#define BTN_DATA_IWRAM __attribute__((section(".iwram")))

/*!
 * @brief Store code in EWRAM.
 */
#define BTN_CODE_EWRAM __attribute__((section(".ewram"), long_call))

/*!
 * @brief Store code in IWRAM.
 */
#define BTN_CODE_IWRAM __attribute__((section(".iwram"), long_call))

#endif
