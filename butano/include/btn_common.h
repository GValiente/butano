#ifndef BTN_COMMON_H
#define BTN_COMMON_H

#include <stddef.h>
#include <stdint.h>
#include "../hw/include/btn_hw_common.h"

/*!
 * @brief The condition is likely to be true.
 */
#define BTN_LIKELY(condition) __builtin_expect(!!(condition), true)

/*!
 * @brief The condition is likely to be false.
 */
#define BTN_UNLIKELY(condition) __builtin_expect(!!(condition), false)

/*!
 * @brief If control flow reaches the point of this macro, the program is undefined.
 */
#define BTN_UNREACHABLE __builtin_unreachable

/*!
 * @brief Const function (return value depends only on the parameters).
 */
#define BTN_CODE_CONST __attribute__((const))

/*!
 * @brief Pure function (return value depends only on the parameters and/or global variables).
 */
#define BTN_CODE_PURE __attribute__((pure))

namespace btn
{
    using nullptr_t = decltype(nullptr);

    [[nodiscard]] void* malloc(size_t bytes);

    [[nodiscard]] void* malloc(int bytes);

    void free(void* ptr);

    void memcpy(void* destination, const void* source, size_t bytes);

    void memcpy(void* destination, const void* source, int bytes);

    void memset(void* destination, uint8_t value, size_t bytes);

    void memset(void* destination, uint8_t value, int bytes);
}

void* operator new(size_t bytes);

void operator delete(void* ptr) noexcept;

void operator delete(void* ptr, size_t bytes) noexcept;

void* operator new[](size_t bytes);

void operator delete[](void* ptr) noexcept;

void operator delete[](void* ptr, size_t bytes) noexcept;

#endif
