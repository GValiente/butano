/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_MEMORY_H
#define BN_HW_MEMORY_H

#include "bn_hw_tonc.h"
#include "../3rd_party/agbabi/include/aeabi.h"
#include "../3rd_party/agbabi/include/agbabi.h"

extern unsigned BN_IWRAM_START;
extern unsigned BN_IWRAM_TOP;
extern unsigned BN_IWRAM_END;
extern unsigned BN_ROM_START;
extern unsigned BN_ROM_END;

extern unsigned __ewram_start;
extern char __eheap_start[], __eheap_end[];

namespace bn::hw::memory
{
    constexpr unsigned memctrl_address = 0x4000800;
    constexpr unsigned fast_ewram_memctrl_value = 0x0E000020;

    void init();

    [[nodiscard]] inline int stack_address()
    {
        unsigned result = 0;
        asm volatile("mov %0, sp" : "=r" (result));
        return int(result);
    }

    [[nodiscard]] inline bool in_vram(void* ptr)
    {
        return int(ptr) >= MEM_PAL && int(ptr) < MEM_ROM;
    }

    [[nodiscard]] inline int used_stack_iwram(int current_stack_address)
    {
        auto iwram_top = reinterpret_cast<uint8_t*>(&BN_IWRAM_TOP);
        auto iwram_stack = reinterpret_cast<uint8_t*>(&current_stack_address);
        return iwram_top - iwram_stack;
    }

    [[nodiscard]] inline int used_static_iwram()
    {
        auto iwram_start = reinterpret_cast<uint8_t*>(&BN_IWRAM_START);
        auto iwram_end = reinterpret_cast<uint8_t*>(&BN_IWRAM_END);
        return iwram_end - iwram_start;
    }

    [[nodiscard]] inline int used_static_ewram()
    {
        auto ewram_start = reinterpret_cast<uint8_t*>(&__ewram_start);
        auto eheap_start = reinterpret_cast<uint8_t*>(&__eheap_start);
        return eheap_start - ewram_start;
    }

    [[nodiscard]] inline char* ewram_heap_start()
    {
        return __eheap_start;
    }

    [[nodiscard]] inline char* ewram_heap_end()
    {
        return __eheap_end;
    }

    [[nodiscard]] inline int used_rom()
    {
        auto rom_start = reinterpret_cast<uint8_t*>(&BN_ROM_START);
        auto rom_end = reinterpret_cast<uint8_t*>(&BN_ROM_END);
        return rom_end - rom_start;
    }

    [[nodiscard]] inline bool fast_ewram()
    {
        volatile unsigned& memctrl_register = *reinterpret_cast<unsigned*>(memctrl_address);
        return memctrl_register == fast_ewram_memctrl_value;
    }

    inline void copy_bytes(const void* source, int bytes, void* destination)
    {
        __aeabi_memcpy(destination, source, size_t(bytes));
    }

    inline void copy_half_words(const void* source, int half_words, void* destination)
    {
        __agbabi_memcpy2(destination, source, size_t(half_words) * 2);
    }

    inline void copy_words(const void* source, int words, void* destination)
    {
        __aeabi_memcpy4(destination, source, size_t(words) * 4);
    }

    inline void copy_words_fiq(const void* source, int words, void* destination)
    {
        constexpr int words_per_slot = 48 / 4;

        if(int slots = words / words_per_slot)
        {
            int fiq_words = slots * words_per_slot;
            __agbabi_fiq_memcpy4x4(destination, source, size_t(fiq_words) * 4);
            source = static_cast<const uint32_t*>(source) + fiq_words;
            destination = static_cast<uint32_t*>(destination) + fiq_words;
            words -= fiq_words;
        }

        __aeabi_memcpy4(destination, source, size_t(words) * 4);
    }

    inline void set_bytes(uint8_t value, int bytes, void* destination)
    {
        __aeabi_memset(destination, size_t(bytes), int(value));
    }

    inline void set_half_words(uint16_t value, int half_words, void* destination)
    {
        memset16(destination, value, unsigned(half_words));
    }

    inline void set_words(unsigned value, int words, void* destination)
    {
        __agbabi_wordset4(destination, size_t(words) * 4, int(value));
    }
}

#endif
