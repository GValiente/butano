#ifndef BTN_SRAM_H
#define BTN_SRAM_H

#include "btn_assert.h"
#include "btn_type_traits.h"
#include "../hw/include/btn_hw_sram_constants.h"

namespace _btn::sram
{
    void unsafe_write(const void* source, int size, int offset);

    void unsafe_read(void* destination, int size, int offset);
}

namespace btn::sram
{
    template<typename Type>
    void write(const Type& source)
    {
        static_assert(is_trivially_copyable<Type>(), "Type is not trivially copyable");
        BTN_CONSTEXPR_ASSERT(int(sizeof(Type)) <= max_size(), "Size is too high");

        _btn::sram::unsafe_write(&source, int(sizeof(Type)), 0);
    }

    template<typename Type>
    void write_offset(const Type& source, int offset)
    {
        static_assert(is_trivially_copyable<Type>(), "Type is not trivially copyable");
        BTN_ASSERT(offset >= 0, "Invalid offset: ", offset);
        BTN_ASSERT(int(sizeof(Type)) + offset <= max_size(), "Size and offset are too high: ", sizeof(Type), " - ", offset);

        _btn::sram::unsafe_write(&source, int(sizeof(Type)), offset);
    }

    template<typename Type>
    void read(Type& destination)
    {
        static_assert(is_trivially_copyable<Type>(), "Type is not trivially copyable");
        BTN_CONSTEXPR_ASSERT(int(sizeof(Type)) <= max_size(), "Size is too high");

        _btn::sram::unsafe_read(&destination, int(sizeof(Type)), 0);
    }

    template<typename Type>
    void read_offset(Type& destination, int offset)
    {
        static_assert(is_trivially_copyable<Type>(), "Type is not trivially copyable");
        BTN_ASSERT(offset >= 0, "Invalid offset: ", offset);
        BTN_ASSERT(int(sizeof(Type)) + offset <= max_size(), "Size and offset are too high: ", sizeof(Type), " - ", offset);

        _btn::sram::unsafe_read(&destination, int(sizeof(Type)), offset);
    }
}

#endif
