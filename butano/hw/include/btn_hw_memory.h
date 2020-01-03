#ifndef BTN_HW_MEMORY_H
#define BTN_HW_MEMORY_H

#include "btn_span.h"

namespace btn::hw
{
    [[nodiscard]] int used_static_iwram();

    [[nodiscard]] int used_static_ewram();

    [[nodiscard]] int used_malloc_ewram();

    void memcpy(void* destination, const void* source, int bytes);

    template<typename Type>
    void memcpy(void* destination, const span<Type>& source)
    {
        BTN_ASSERT(destination, "Destination is null");
        BTN_ASSERT(! source.empty(), "Source is empty");

        btn::hw::memcpy(destination, source.data(), source.size() * sizeof(Type));
    }

    void memcpy16(void* destination, const void* source, int half_words);

    template<typename Type>
    void memcpy16(void* destination, const span<Type>& source)
    {
        static_assert(sizeof(Type) % 2 == 0, "Source type invalid size");

        BTN_ASSERT(destination, "Destination is null");
        BTN_ASSERT(! source.empty(), "Source is empty");

        btn::hw::memcpy16(destination, source.data(), source.size() * (sizeof(Type) / 2));
    }

    void memcpy32(void* destination, const void* source, int words);

    template<typename Type>
    void memcpy32(void* destination, const span<Type>& source)
    {
        static_assert(sizeof(Type) % 4 == 0, "Source type invalid size");

        BTN_ASSERT(destination, "Destination is null");
        BTN_ASSERT(! source.empty(), "Source is empty");

        btn::hw::memcpy32(destination, source.data(), source.size() * (sizeof(Type) / 4));
    }

    void memset(void* destination, uint8_t value, int bytes);

    void memset16(void* destination, uint16_t value, int half_words);

    void memset32(void* destination, unsigned value, int words);
}

#endif
