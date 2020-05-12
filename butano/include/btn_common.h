#ifndef BTN_COMMON_H
#define BTN_COMMON_H

#include <stddef.h>
#include <stdint.h>
#include "../hw/include/btn_hw_common.h"

/*!
 * @brief If control flow reaches the point of this macro, the program is undefined.
 */
#define BTN_UNREACHABLE __builtin_unreachable

/*!
 * @brief Creates a compiler level memory barrier forcing optimizer to not re-order memory accesses across the barrier.
 *
 * https://stackoverflow.com/questions/14950614/working-of-asm-volatile-memory
 */
#define BTN_BARRIER asm volatile("" ::: "memory")

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
