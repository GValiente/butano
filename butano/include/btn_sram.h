#ifndef BTN_SRAM_H
#define BTN_SRAM_H

#include "btn_type_traits.h"

namespace btn::sram
{
    [[nodiscard]] BTN_CODE_CONST int max_size();

    void write(const void* source, int size);

    void write_offset(const void* source, int size, int offset);

    void read(void* destination, int size);

    void read_offset(void* destination, int size, int offset);

    template<typename Type>
    void write(const Type& source)
    {
        static_assert(is_trivially_copyable<Type>(), "Source is not trivially copyable");

        write(&source, sizeof(Type));
    }

    template<typename Type>
    void write_offset(const Type& source, int offset)
    {
        static_assert(is_trivially_copyable<Type>(), "Source is not trivially copyable");

        write_offset(&source, sizeof(Type), offset);
    }

    template<typename Type>
    void read(Type& destination)
    {
        static_assert(is_trivially_copyable<Type>(), "Destination is not trivially copyable");

        read(&destination, sizeof(Type));
    }

    template<typename Type>
    void read_offset(Type& destination, int offset)
    {
        static_assert(is_trivially_copyable<Type>(), "Destination is not trivially copyable");

        read_offset(&destination, sizeof(Type), offset);
    }
}

#endif
