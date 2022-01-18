#ifndef CULT_OF_GBA_BIOS_H
#define CULT_OF_GBA_BIOS_H

#ifdef __cplusplus
extern "C"
{
#endif

void swi_RLUnCompReadNormalWrite8bit(const void* src, void* dst) __attribute__((section(".iwram"), long_call));

void swi_RLUnCompReadNormalWrite16bit(const void* src, void* dst) __attribute__((section(".iwram"), long_call));

void swi_LZ77UnCompWrite8bit(const void* src, void* dst) __attribute__((section(".iwram"), long_call));

void swi_LZ77UnCompWrite16bit(const void* src, void* dst) __attribute__((section(".iwram"), long_call));

void swi_HuffUnCompReadNormal(const void* src, void* dst) __attribute__((section(".iwram"), long_call));

#ifdef __cplusplus
}
#endif

#endif
